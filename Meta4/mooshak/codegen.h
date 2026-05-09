/* codegen.h */
#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"
#include "symtab.h"

/* Generate LLVM IR code from the annotated AST */
void codegen(ASTNode *root, SymTable *tables);

#endif
