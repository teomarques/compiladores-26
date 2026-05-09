#ifndef _CODEGEN_H
#define _CODEGEN_H

#include "ast.h"
#include "semantic.h"

void codegen(struct node *root, ClassTable *ct);

#endif
