#ifndef _CODEGEN_H
#define _CODEGEN_H

#include "ast.h"
#include "semantic.h"

/* Generate LLVM IR from Meta3 AST and symbol tables.
 * root: AST root (program)
 * ct: class symbol table returned by build_symbol_tables()
 */
void codegen(struct node *root, ClassTable *ct);

#endif
