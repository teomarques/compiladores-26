/*
 * Autores:
 *   Simão Tomás Botas Carvalho - 2021223055
 *   Teodoro Marques          - 2023211717
 */

#include <stdlib.h>
#include <stdio.h>
#include "ast.h"

/* Definido em jucompiler.y */
extern const char *category_name[];

struct node *newnode(enum category category, char *token)
{
    struct node *n = malloc(sizeof(struct node));
    n->category = category;
    n->token    = token;
    n->children = malloc(sizeof(struct node_list));
    n->children->node = NULL;
    n->children->next = NULL;
    return n;
}

void addchild(struct node *parent, struct node *child)
{
    struct node_list *c;
    struct node_list *nl = malloc(sizeof(struct node_list));
    nl->node = child;
    nl->next = NULL;
    c = parent->children;
    while (c->next) c = c->next;
    c->next = nl;
}

static void printast_rec(struct node *n, int depth)
{
    int i;
    struct node_list *c;
    if (!n) return;

    for (i = 0; i < depth; i++) printf("..");

    printf("%s", category_name[n->category]);
    if (n->token) printf("(%s)", n->token);
    printf("\n");

    for (c = n->children; c; c = c->next)
        printast_rec(c->node, depth + 1);
}

void printast(struct node *root)
{
    printast_rec(root, 0);
}
