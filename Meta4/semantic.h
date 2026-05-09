#ifndef _SEMANTIC_H
#define _SEMANTIC_H

#include "ast.h"
#include "symtab.h"

ClassTable *build_symbol_tables(struct node *program);
void print_symbol_tables(ClassTable *ct);
void check_and_annotate_ast(struct node *program, ClassTable *ct);
void free_class_table(ClassTable *ct);

JType node_to_jtype(struct node *type_node);

#endif
