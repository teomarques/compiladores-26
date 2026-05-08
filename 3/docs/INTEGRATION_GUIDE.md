# Integration Guide for Meta 3 Semantic Analyzer

## File Structure

Your project should have the following structure after integration:

```
compiladores-26/
├── Makefile
├── jucompiler.l          (lexer - existing)
├── jucompiler.y          (parser - existing, needs modification)
├── ast.h / ast.c         (AST structures - existing)
├── semantic.h / semantic.c           (NEW - core semantic analysis)
├── symbol_table.h / symbol_table.c   (NEW - phase 1: symbol building)
├── type_checker.h / type_checker.c   (NEW - phase 2: type checking)
├── error_handler.h / error_handler.c (NEW - phase 3: error handling)
└── testes/               (test cases)
```

## Makefile Modifications

Your current Makefile needs to be updated to compile the new modules:

```makefile
# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -DYYDEBUG=1

# Object files
OBJS = lex.yy.o y.tab.o ast.o semantic.o symbol_table.o type_checker.o error_handler.o

# Main target
jucompiler: $(OBJS)
	$(CC) $(CFLAGS) -o jucompiler $(OBJS)

# Parser
y.tab.c y.tab.h: jucompiler.y
	yacc -d -v jucompiler.y

y.tab.o: y.tab.c y.tab.h ast.h semantic.h symbol_table.h type_checker.h error_handler.h
	$(CC) $(CFLAGS) -c y.tab.c

# Lexer
lex.yy.c: jucompiler.l
	lex jucompiler.l

lex.yy.o: lex.yy.c y.tab.h
	$(CC) $(CFLAGS) -c lex.yy.c

# AST module
ast.o: ast.c ast.h
	$(CC) $(CFLAGS) -c ast.c

# Semantic analysis modules
semantic.o: semantic.c semantic.h ast.h
	$(CC) $(CFLAGS) -c semantic.c

symbol_table.o: symbol_table.c symbol_table.h semantic.h ast.h
	$(CC) $(CFLAGS) -c symbol_table.c

type_checker.o: type_checker.c type_checker.h symbol_table.h semantic.h ast.h
	$(CC) $(CFLAGS) -c type_checker.c

error_handler.o: error_handler.c error_handler.h semantic.h
	$(CC) $(CFLAGS) -c error_handler.c

# Cleanup
clean:
	rm -f jucompiler lex.yy.* y.tab.* *.o *.output

# Rebuild everything
rebuild: clean jucompiler

.PHONY: clean rebuild
```

## jucompiler.y Modifications

### 1. Add Includes at Top

```c
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "semantic.h"
#include "symbol_table.h"
#include "type_checker.h"
#include "error_handler.h"

#define YYDEBUG 1

int  yylex(void);
int  yyparse(void);
void yyerror(const char *s);
void printast(struct node *root);

extern int  lex_line, lex_column;
extern int  syn_line, syn_column;
extern int  lex_errs;
extern char *yytext;
extern int  yychar;
extern int  print_tokens;

struct node *ast = NULL;
int sem_errs = 0;
int skip_semantic = 0;  // NEW: for -t flag

// ... rest of existing code
%}
```

### 2. Modify Main Section (at end of jucompiler.y)

Add a main function or modify the existing one:

```c
/* After the grammar rules, add this section: */

/* Command-line argument handling */
int main(int argc, char *argv[])
{
    int parse_result;
    int print_syntax_tree = 0;
    int tokens_only = 0;

    /* Parse command-line arguments */
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-t") == 0) {
            tokens_only = 1;
            skip_semantic = 1;
        } else if (strcmp(argv[i], "-s") == 0) {
            print_syntax_tree = 1;
        }
    }

    /* Perform parsing */
    parse_result = yyparse();

    /* Handle parse result */
    if (parse_result != 0) {
        /* Syntax errors occurred */
        return 1;
    }

    /* If only syntax checking requested, we're done */
    if (tokens_only) {
        if (print_syntax_tree) {
            /* Print AST if requested with -t -s */
            printast(ast);
        }
        return (lex_errs > 0 || syn_errs > 0) ? 1 : 0;
    }

    /* Phase 1: Build Symbol Tables */
    if (ast && !skip_semantic) {
        class_table_t *class_table = NULL;
        
        /* Extract class name from AST */
        if (ast->children && ast->children->node &&
            ast->children->node->category == N_Identifier) {
            char *class_name = ast->children->node->token;
            class_table = create_class_table(class_name);

            /* Build class symbol table */
            build_class_symbol_table(ast, class_table);
            
            /* Build method symbol tables */
            build_method_symbol_tables(ast, class_table);

            /* Phase 2: Type checking and annotation */
            check_and_annotate_ast(ast, class_table, NULL);

            /* Output symbol tables */
            if (print_syntax_tree) {
                print_class_table(class_table);
                
                /* Print method tables */
                method_list_t *mlist = class_table->methods;
                while (mlist) {
                    printf("\n");
                    print_method_table(class_table, mlist->method);
                    mlist = mlist->next;
                }
                printf("\n");
            }

            /* Cleanup */
            free_class_table(class_table);
        }
    }

    /* Output annotated AST if requested */
    if (print_syntax_tree && ast) {
        printf("\n");
        printast(ast);
    }

    /* Return success/failure based on errors */
    int total_errors = lex_errs + syn_errs + get_semantic_error_count();
    return (total_errors > 0) ? 1 : 0;
}
```

### 3. Update printast Function

The existing `printast` function needs to be aware of annotations:

```c
/* Modify your existing printast function to show type annotations */
void printast(struct node *root)
{
    printast_indent(root, 0);
}

void printast_indent(struct node *root, int indent)
{
    if (!root) return;

    for (int i = 0; i < indent; i++) printf(".");
    
    /* Print node type */
    printf("%s", get_category_name(root->category));
    
    /* Print token if present */
    if (root->token) {
        printf("(%s)", root->token);
    }
    
    /* Print type annotation for expression nodes */
    node_annotation_t *ann = get_annotation(root);
    if (ann && ann->type != TYPE_UNDEF) {
        printf(" - %s", type_to_string(ann->type, ann->is_array));
    }
    
    printf("\n");

    /* Recursively print children */
    struct node_list *child = root->children;
    while (child) {
        printast_indent(child->node, indent + 2);
        child = child->next;
    }
}
```

## Expected Command-Line Behavior

After integration, your compiler should work as follows:

### 1. Syntax checking only (existing)
```bash
./jucompiler -t < test.java
# Outputs: syntax errors only, exits without semantic analysis
```

### 2. Syntax checking with AST (existing)
```bash
./jucompiler -t -s < test.java
# Outputs: syntax errors and AST, exits without semantic analysis
```

### 3. Full semantic analysis (NEW)
```bash
./jucompiler < test.java
# Outputs: semantic errors (if any), symbol tables, annotated AST
```

### 4. Semantic analysis with debugging (NEW)
```bash
./jucompiler -s < test.java
# Outputs: semantic errors, symbol tables, annotated AST
```

## Testing Checklist

Use your Factorial.java test case to verify:

- [ ] Symbol table for class is printed correctly
- [ ] Symbol tables for each method are printed with proper format
- [ ] Symbols appear in declaration order
- [ ] Type annotations show in AST for expressions
- [ ] Non-expression nodes are NOT annotated
- [ ] Error messages format matches specification exactly
- [ ] Multiple errors are all reported
- [ ] Program continues after errors (pseudo-type undef used)

## Common Integration Issues

### Issue: "undefined reference to `build_class_symbol_table`"
**Solution**: Make sure all .o files are linked. Check Makefile includes all modules.

### Issue: Type annotations not appearing in output
**Solution**: Ensure `set_annotation()` is called during type checking and `get_annotation()` is used in printast.

### Issue: Symbol table order is wrong
**Solution**: Maintain linked lists with proper ordering during insertion (prepend or append appropriately).

### Issue: "-s" flag not recognized
**Solution**: Add flag parsing in main function, set appropriate variables before parsing.

## Next Implementation Steps

1. Copy all scaffold files to your project directory
2. Update Makefile with new compilation rules
3. Modify jucompiler.y with main() function and includes
4. Implement full AST traversal in symbol_table.c
5. Complete operator validation in type_checker.c
6. Test with provided test cases
7. Refine error reporting and output formatting

## Files to Copy

All files from outputs/ to your compiladores-26/ directory:
```bash
semantic.h
semantic.c
symbol_table.h
symbol_table.c
type_checker.h
type_checker.c
error_handler.h
error_handler.c
```

Good luck with the implementation! The scaffolding provides the structure; now you'll fill in the AST traversal and type system details.
