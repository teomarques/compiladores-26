/* semantics.h */
#ifndef SEMANTICS_H
#define SEMANTICS_H

#include "ast.h"
#include "symtab.h"

/* Run semantic analysis on the AST. Returns number of errors found. */
int semantic_analysis(ASTNode *root);

/* Get the global symbol table (for printing) */
SymTable *get_symbol_tables(void);

/* Free all semantic data */
void semantic_free(void);

#endif
