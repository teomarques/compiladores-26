# Meta 2: Syntactic Analysis

## Overview

Meta 2 implements the **syntactic analysis** phase of the Java compiler using the `yacc` tool combined with the lexer from Meta 1. This phase takes the token stream and constructs an Abstract Syntax Tree (AST) representing the program structure.

**Files in this folder:**
- `jucompiler.y` - Parser specification file (input to yacc)
- `ast.h` - Abstract Syntax Tree header with node definitions
- `ast.c` - Abstract Syntax Tree implementation
- `README.md` - This file

## What the Parser Does

The parser:

1. **Reads Tokens** - Consumes token stream from lexer (Meta 1)
2. **Applies Grammar Rules** - Uses grammar rules to validate token sequences
3. **Builds AST** - Constructs Abstract Syntax Tree representing program structure
4. **Error Detection** - Reports syntax errors with line/column information
5. **AST Annotation** - Stores semantic information in AST nodes

## Java Grammar Structure

The parser handles:

### Program Structure
```
Program
  ├─ ClassDeclaration (one or more)
  │   ├─ Class name and optional super class
  │   └─ Members (fields and methods)
```

### Classes
```
ClassDecl
  ├─ ClassHeader (modifiers, name, extends)
  └─ Members
      ├─ FieldDeclaration (static/instance variables)
      └─ MethodDeclaration
```

### Methods
```
MethodDecl
  ├─ MethodHeader (return type, name, parameters)
  └─ MethodBody
      ├─ Variables (local declarations)
      └─ Statements
```

### Expressions
```
Expression
  ├─ Literals (numbers, strings, booleans)
  ├─ Identifiers (variable/method names)
  ├─ Binary Operations (arithmetic, logical, comparison)
  ├─ Unary Operations (negation, logical NOT)
  ├─ Method Calls (with parameters)
  ├─ Array Access (indexing)
  ├─ Field Access (dot notation)
  └─ Parenthesized Expressions
```

### Statements
```
Statement
  ├─ Expression Statement
  ├─ Block (compound statement)
  ├─ If Statement (with optional else)
  ├─ While Loop
  ├─ Return Statement
  ├─ Variable Declaration
  └─ Empty Statement
```

## File Structure

### jucompiler.y

The parser specification file contains three sections separated by `%%`:

1. **Header Section** (before first %%)
   - Include statements
   - Token declarations (%token)
   - Type declarations (%union)
   - Precedence declarations (%left, %right, %nonassoc)

2. **Grammar Rules Section** (between %% marks)
   - Production rules for the Java grammar
   - Semantic actions in C code
   - Actions build the AST

3. **Auxiliary Code Section** (after second %%)
   - yyerror() function for error handling
   - main() function (if present)
   - Other helper functions

### ast.h

Header file defining:
- Node type enumeration (node_category_t)
- Node structure (struct node) containing:
  - Category (type of node)
  - Value information
  - Child nodes list
  - Sibling node pointer
  - Line/column information

### ast.c

Implementation file providing:
- Node creation functions
- Tree traversal functions
- Tree printing/debugging functions
- Node linking/manipulation

## How to Use

### Compile the Parser

The parser is compiled as part of the full project build:

```bash
cd /path/to/compiladores-26
make clean
make
```

This runs:
```bash
yacc -d meta2/jucompiler.y         # Generates y.tab.c, y.tab.h
cc -c y.tab.c                      # Compiles the parser
lex meta1/jucompiler.l             # Generates lex.yy.c from lexer
cc -c lex.yy.c                     # Compiles the lexer
cc -c meta2/ast.c                  # Compiles AST functions
cc -o jucompiler *.o -lfl          # Links everything together
```

### Generated Files

When `yacc` processes `jucompiler.y`, it generates:

- `y.tab.c` - C source code implementing the parser
- `y.tab.h` - Header with token definitions and yylval union

These are generated in the parent directory (root of project).

## AST Node Categories

The AST represents different language constructs:

- **N_Program** - Root of entire program
- **N_ClassDeclaration** - Class definition
- **N_FieldDeclaration** - Instance/static variable
- **N_MethodDeclaration** - Method definition
- **N_MethodHeader** - Method signature
- **N_Type** - Type specification
- **N_Identifier** - Variable/class name
- **N_MethodCall** - Function invocation
- **N_BinaryOp** - Binary operation (a + b)
- **N_UnaryOp** - Unary operation (!a)
- **N_ArrayAccess** - Array indexing (arr[i])
- **N_FieldAccess** - Field access (obj.field)
- **N_IfStatement** - Conditional branch
- **N_WhileStatement** - Loop
- **N_ReturnStatement** - Method return
- **N_Block** - Compound statement
- **N_Literal** - Constant value

## Integration with Other Metas

```
┌─────────────────────────────────────┐
│     META 1: Lexical Analysis        │
│  Input: Source code (text)           │
│  Output: Token stream                │
└─────────────────┬────────────────────┘
                  │ (yylex() calls)
                  ▼
┌─────────────────────────────────────┐
│     META 2: Syntactic Analysis      │
│     (meta2/jucompiler.y, ast.*)     │
│  Input: Token stream                 │
│  Output: Abstract Syntax Tree (AST)  │
│                                      │
│  Components:                         │
│  - Parser (from jucompiler.y)       │
│  - AST data structures (ast.h)      │
│  - Node creation (ast.c)            │
└─────────────────┬────────────────────┘
                  │ (AST)
                  ▼
┌─────────────────────────────────────┐
│    META 3: Semantic Analysis        │
│  Input: AST                          │
│  Output: Type-checked AST + Symbols  │
└─────────────────┬────────────────────┘
                  │
                  ▼
┌─────────────────────────────────────┐
│      META 4: Code Generation        │
│  Input: Type-checked AST             │
│  Output: LLVM IR / Machine code      │
└─────────────────────────────────────┘
```

## Building and Testing

### Full Build
```bash
make clean && make
```

### Test with Sample File (Syntax Check Only)
```bash
./jucompiler -t < testes/Factorial.java
```

The `-t` flag (if implemented) performs syntax checking only.

### Test with Semantic Analysis
```bash
./jucompiler -s < testes/Factorial.java
```

The `-s` flag performs full semantic analysis including symbol tables.

### Debug Parser (if needed)
```bash
yacc -d -v meta2/jucompiler.y
cc -c y.tab.c
lex meta1/jucompiler.l
cc -c lex.yy.c
cc -c meta2/ast.c
cc -o testparser y.tab.o lex.yy.o ast.o -lfl
./testparser < testes/Factorial.java
```

The `-v` flag creates `y.output` showing parser states and conflicts.

## Understanding the AST

### Node Structure (ast.h)
```c
struct node {
    node_category_t category;    /* Type of node */
    /* Value fields (varies by type) */
    int int_value;
    double double_value;
    char *str_value;
    /* Position information */
    int line;
    int column;
    /* Tree structure */
    struct node_list *children;  /* Child nodes */
    struct node *next_sibling;   /* Sibling node */
    /* Type annotation (from Meta 3) */
    node_annotation_t *annotation;
};
```

### Creating Nodes (ast.c)
```c
struct node *create_node(node_category_t category);
void add_child(struct node *parent, struct node *child);
void print_tree(struct node *root, int indent);
```

## Common Patterns in jucompiler.y

### Production Rule with Actions
```yacc
ClassName : ID { $$ = create_node(N_Identifier); }
          ;
```

### Reducing to Different Rule
```yacc
Type : "int"     { $$ = create_node(N_Type); /* set to int */ }
     | "double"  { $$ = create_node(N_Type); /* set to double */ }
     ;
```

### Building AST Nodes
```yacc
MethodCall : ID '(' Arguments ')' {
    struct node *call = create_node(N_MethodCall);
    add_child(call, $1);  /* method name */
    add_child(call, $3);  /* arguments */
    $$ = call;
}
```

### Operator Precedence
```yacc
%left '+' '-'
%left '*' '/'
%right UMINUS
```

## Error Handling

### Syntax Errors
- Parser reports with line/column when it cannot match tokens to grammar
- yyerror() function prints error message
- Parser may attempt error recovery to report multiple errors

### Error Production Rules
Some parsers include error recovery:
```yacc
Statement : /* normal statements */
          | error ';' { /* attempt recovery */ }
          ;
```

## Debugging Tips

1. **Check for Shift/Reduce Conflicts** - Check y.output for warnings
2. **Verify Token Declaration** - All tokens used in grammar must be declared
3. **Check Operator Precedence** - Ambiguous expressions need %left/%right
4. **Test Incrementally** - Start with simple statements, add complexity
5. **Use Tree Printing** - print_tree() shows AST structure

## Relationship to Lexer (Meta 1)

The parser and lexer work together:

1. Parser calls `yylex()` when it needs next token
2. Lexer matches input against patterns
3. Lexer returns token type and sets `yylval` to token value
4. Parser processes token according to grammar rules

This happens transparently via yacc-generated code.

## Next Steps

1. Review `jucompiler.y` to understand grammar rules
2. Study `ast.h` and `ast.c` to understand node structures
3. Trace how tokens become AST nodes
4. Understand tree traversal patterns
5. Build and test with sample Java files
6. Prepare for Meta 3 (semantic analysis) by understanding AST structure

---

**Note**: Meta 2 completes the parsing phase. The output is an Abstract Syntax Tree (AST) that Meta 3 will analyze for semantic correctness.
