/* ast.c - Meta 3 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

static const char *node_names[] = {
    "Program", "FieldDecl", "VarDecl",
    "MethodDecl", "MethodHeader", "MethodParams", "ParamDecl", "MethodBody",
    "Block", "If", "While", "Return", "Call", "Print", "ParseArgs", "Assign",
    "Or", "And", "Eq", "Ne", "Lt", "Gt", "Le", "Ge",
    "Add", "Sub", "Mul", "Div", "Mod", "Lshift", "Rshift", "Xor",
    "Not", "Minus", "Plus", "Length",
    "Bool", "BoolLit", "Double", "Decimal", "Identifier", "Int",
    "Natural", "StrLit", "StringArray", "Void"
};

const char *node_type_name(NodeType type) { return node_names[type]; }

ASTNode *ast_new_node(NodeType type, char *value) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    if (!node) { fprintf(stderr, "Error: malloc failed\n"); exit(1); }
    node->type = type;
    node->value = value ? strdup(value) : NULL;
    node->annotation = NULL;
    node->line = 0;
    node->col = 0;
    node->child = NULL;
    node->sibling = NULL;
    return node;
}

void ast_add_child(ASTNode *parent, ASTNode *child) {
    if (!parent || !child) return;
    if (!parent->child) { parent->child = child; }
    else {
        ASTNode *last = parent->child;
        while (last->sibling) last = last->sibling;
        last->sibling = child;
    }
}

ASTNode *ast_add_sibling(ASTNode *node, ASTNode *sibling) {
    if (!node) return sibling;
    if (!sibling) return node;
    ASTNode *last = node;
    while (last->sibling) last = last->sibling;
    last->sibling = sibling;
    return node;
}

void ast_print(ASTNode *node, int depth) {
    if (!node) return;
    for (int i = 0; i < depth; i++) printf("..");
    if (node->value)
        printf("%s(%s)", node_type_name(node->type), node->value);
    else
        printf("%s", node_type_name(node->type));
    if (node->annotation)
        printf(" - %s", node->annotation);
    printf("\n");
    ast_print(node->child, depth + 1);
    ast_print(node->sibling, depth);
}

void ast_free(ASTNode *node) {
    if (!node) return;
    ast_free(node->child);
    ast_free(node->sibling);
    if (node->value) free(node->value);
    if (node->annotation) free(node->annotation);
    free(node);
}

int ast_child_count(ASTNode *node) {
    if (!node) return 0;
    int count = 0;
    ASTNode *c = node->child;
    while (c) { count++; c = c->sibling; }
    return count;
}
