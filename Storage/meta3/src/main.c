/*
 * Autores:
 *   Simão Tomás Botas Carvalho - 2021223055
 *   Teodoro Marques          - 2023211717
 *
 * Meta 3 -- Main Entry Point
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "semantic.h"
#include "symbol_table.h"
#include "type_checker.h"
#include "error_handler.h"

int  yylex(void);
int  yyparse(void);
void printast(struct node *root);

extern struct node *ast;
extern int          syn_errs;
extern int          lex_errs;
extern int          skip_semantic;
extern int          print_tokens;

int main(int argc, char **argv)
{
    int i;
    int parse_status;
    int mode = 2;
    int print_symbol_tables = 0;

    for (i = 1; i < argc; i++) {
        if      (!strcmp(argv[i], "-l"))  mode = 0;
        else if (!strcmp(argv[i], "-e1")) mode = 1;
        else if (!strcmp(argv[i], "-e2")) mode = 2;
        else if (!strcmp(argv[i], "-t"))  { mode = 3; skip_semantic = 1; }
        else if (!strcmp(argv[i], "-s"))  print_symbol_tables = 1;
    }

    if (mode == 0 || mode == 1) {
        print_tokens = (mode == 0);
        while (yylex() != 0) ;
        return 0;
    }

    print_tokens = 0;
    parse_status = yyparse();

    if (parse_status != 0 && lex_errs > 0) {
        while (yylex() != 0) ;
    }

    if (mode == 3 && ast && syn_errs == 0) {
        if (print_symbol_tables)
            printast(ast);
        return 0;
    }

    /* Phase 1: Build Symbol Tables */
    if (ast && !skip_semantic && syn_errs == 0) {
        class_table_t *class_table = NULL;

        /* The first child of Program is a sentinel (empty node_list).
         * The actual first child (Identifier with class name) is in children->next */
        if (ast->children && ast->children->next && ast->children->next->node &&
            ast->children->next->node->category == N_Identifier) {
            char *class_name = ast->children->next->node->token;
            class_table = create_class_table(class_name);

            build_class_symbol_table(ast, class_table);
            build_method_symbol_tables(ast, class_table);

            if (print_symbol_tables) {
                print_class_table(class_table);

                method_list_t *mlist = class_table->methods;
                while (mlist) {
                    printf("\n");
                    print_method_table(class_table, mlist->method);
                    mlist = mlist->next;
                }
                printf("\n");
            }

            check_and_annotate_ast(ast, class_table, NULL);
            free_class_table(class_table);
        }
    }

    if (print_symbol_tables && ast && syn_errs == 0) {
        printf("\n");
        printast(ast);
    }

    return (lex_errs > 0 || syn_errs > 0) ? 1 : 0;
}
