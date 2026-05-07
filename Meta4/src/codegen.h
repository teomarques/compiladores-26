/*
 * codegen.h - LLVM IR Code Generator for Juc Compiler - Meta 4
 * Autores:
 *   Simão Tomás Botas Carvalho - 2021223055
 *   Teodoro Marques - 2023211717
 */

#ifndef _CODEGEN_H
#define _CODEGEN_H

#include "ast.h"
#include "semantic.h"

/* Main entry point for code generation */
void codegen_program(struct node *program, ClassTable *ct);

#endif /* _CODEGEN_H */
