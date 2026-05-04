/*
 * Autores:
 *   Simão Tomás Botas Carvalho - 2021223055
 *   Teodoro Marques          - 2023211717
 *
 * Meta 3 -- Analisador Semântico
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "semantic.h"
#include "ast.h"

/* ============================================================
 * HELPER MACROS
 * ============================================================ */

#define INITIAL_CAPACITY 10

/* ============================================================
 * SYMBOL TABLE IMPLEMENTATION
 * ============================================================ */

class_table_t *create_class_table(const char *class_name)
{
    class_table_t *table = (class_table_t *)malloc(sizeof(class_table_t));
    if (!table) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    table->class_name = (char *)malloc(strlen(class_name) + 1);
    if (!table->class_name) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    strcpy(table->class_name, class_name);

    table->fields = NULL;
    table->methods = NULL;

    return table;
}

method_table_t *create_method_table(const char *name, type_t return_type)
{
    method_table_t *method = (method_table_t *)malloc(sizeof(method_table_t));
    if (!method) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    method->method_name = (char *)malloc(strlen(name) + 1);
    if (!method->method_name) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    strcpy(method->method_name, name);

    method->return_type = return_type;
    method->params = NULL;
    method->locals = NULL;
    method->all = NULL;

    return method;
}

void add_method_to_table(class_table_t *table, method_table_t *method)
{
    method_list_t *new_method = (method_list_t *)malloc(sizeof(method_list_t));
    if (!new_method) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    new_method->method = method;
    new_method->next = table->methods;
    table->methods = new_method;
}

void add_field_to_table(class_table_t *table, const char *name, type_t type, int is_array)
{
    symbol_list_t *new_sym = (symbol_list_t *)malloc(sizeof(symbol_list_t));
    if (!new_sym) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    symbol_t *sym = (symbol_t *)malloc(sizeof(symbol_t));
    if (!sym) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    sym->name = (char *)malloc(strlen(name) + 1);
    if (!sym->name) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    strcpy(sym->name, name);

    sym->type = type;
    sym->is_array = is_array;
    sym->is_param = 0;

    new_sym->symbol = sym;
    new_sym->next = table->fields;
    table->fields = new_sym;
}

void add_param_to_method(method_table_t *method, const char *name, type_t type, int is_array)
{
    symbol_list_t *new_sym = (symbol_list_t *)malloc(sizeof(symbol_list_t));
    if (!new_sym) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    symbol_t *sym = (symbol_t *)malloc(sizeof(symbol_t));
    if (!sym) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    sym->name = (char *)malloc(strlen(name) + 1);
    if (!sym->name) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    strcpy(sym->name, name);

    sym->type = type;
    sym->is_array = is_array;
    sym->is_param = 1;

    new_sym->symbol = sym;
    new_sym->next = method->params;
    method->params = new_sym;
}

void add_local_to_method(method_table_t *method, const char *name, type_t type, int is_array)
{
    symbol_list_t *new_sym = (symbol_list_t *)malloc(sizeof(symbol_list_t));
    if (!new_sym) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    symbol_t *sym = (symbol_t *)malloc(sizeof(symbol_t));
    if (!sym) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    sym->name = (char *)malloc(strlen(name) + 1);
    if (!sym->name) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    strcpy(sym->name, name);

    sym->type = type;
    sym->is_array = is_array;
    sym->is_param = 0;

    new_sym->symbol = sym;
    new_sym->next = method->locals;
    method->locals = new_sym;
}

void add_return_to_method(method_table_t *method, type_t return_type)
{
    symbol_list_t *new_sym = (symbol_list_t *)malloc(sizeof(symbol_list_t));
    if (!new_sym) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    symbol_t *sym = (symbol_t *)malloc(sizeof(symbol_t));
    if (!sym) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    sym->name = (char *)malloc(strlen("return") + 1);
    if (!sym->name) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    strcpy(sym->name, "return");

    sym->type = return_type;
    sym->is_array = 0;
    sym->is_param = 0;

    new_sym->symbol = sym;
    new_sym->next = method->all;
    method->all = new_sym;
}

symbol_t *lookup_symbol(method_table_t *method, const char *name)
{
    if (!method) return NULL;

    /* Check all symbols (params + locals + return) */
    symbol_list_t *list = method->all;
    while (list) {
        if (strcmp(list->symbol->name, name) == 0) {
            return list->symbol;
        }
        list = list->next;
    }

    return NULL;
}

symbol_t *lookup_field(class_table_t *table, const char *name)
{
    if (!table) return NULL;

    symbol_list_t *list = table->fields;
    while (list) {
        if (strcmp(list->symbol->name, name) == 0) {
            return list->symbol;
        }
        list = list->next;
    }

    return NULL;
}

method_table_t *lookup_method(class_table_t *table, const char *name, int param_count)
{
    if (!table) return NULL;

    method_list_t *list = table->methods;
    while (list) {
        if (strcmp(list->method->method_name, name) == 0) {
            /* TODO: Check parameter count and types */
            return list->method;
        }
        list = list->next;
    }

    return NULL;
}

/* ============================================================
 * TYPE SYSTEM FUNCTIONS
 * ============================================================ */

const char *type_to_string(type_t type, int is_array)
{
    static char buffer[64];
    const char *type_str;

    switch (type) {
        case TYPE_INT:
            type_str = "int";
            break;
        case TYPE_DOUBLE:
            type_str = "double";
            break;
        case TYPE_BOOL:
            type_str = "boolean";
            break;
        case TYPE_STRING_ARRAY:
            type_str = "String";
            break;
        case TYPE_VOID:
            type_str = "void";
            break;
        case TYPE_UNDEF:
            type_str = "undef";
            break;
        default:
            type_str = "unknown";
            break;
    }

    if (is_array) {
        snprintf(buffer, sizeof(buffer), "%s []", type_str);
    } else {
        snprintf(buffer, sizeof(buffer), "%s", type_str);
    }

    return buffer;
}

int types_equal(type_t t1, type_t t2, int a1, int a2)
{
    return (t1 == t2) && (a1 == a2);
}

int types_compatible(type_t actual, type_t expected, int actual_array, int expected_array)
{
    /* TODO: Implement type compatibility rules */
    return types_equal(actual, expected, actual_array, expected_array);
}

/* ============================================================
 * AST ANNOTATION
 * ============================================================ */

static node_annotation_t *annotations[10000]; /* TODO: Use a better data structure */
static int annotation_count = 0;

node_annotation_t *get_annotation(struct node *node)
{
    if (!node) return NULL;

    /* TODO: Implement proper mapping from node pointer to annotation */
    return NULL;
}

void set_annotation(struct node *node, type_t type, int is_array)
{
    if (!node) return;

    if (annotation_count >= 10000) {
        fprintf(stderr, "Annotation table full\n");
        return;
    }

    node_annotation_t *ann = (node_annotation_t *)malloc(sizeof(node_annotation_t));
    if (!ann) {
        perror("malloc");
        return;
    }

    ann->type = type;
    ann->is_array = is_array;

    /* TODO: Store annotation properly with node pointer mapping */
    annotations[annotation_count++] = ann;
}

/* ============================================================
 * SEMANTIC ANALYSIS
 * ============================================================ */

void semantic_analysis(struct node *root)
{
    if (!root) return;

    /* Phase 1: Build symbol tables */
    printf("Starting semantic analysis...\n");

    /* Phase 2: Type checking and AST annotation */

    /* Phase 3: Error reporting (already done incrementally) */
}

void annotate_ast(struct node *root, class_table_t *class_table)
{
    if (!root) return;

    /* Traverse AST and annotate expression nodes with types */
    /* TODO: Implement recursive traversal and annotation */
}

type_t get_node_type(struct node *node)
{
    if (!node) return TYPE_UNDEF;

    /* TODO: Extract type from node annotation */
    return TYPE_UNDEF;
}

/* ============================================================
 * ERROR REPORTING
 * ============================================================ */

void semantic_error(int line, int col, const char *fmt, ...)
{
    va_list args;
    printf("Line %d, col %d: ", line, col);

    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    printf("\n");
}

/* ============================================================
 * SYMBOL TABLE PRINTING
 * ============================================================ */

void print_class_table(class_table_t *table)
{
    if (!table) return;

    printf("===== Class %s Symbol Table =====\n", table->class_name);

    /* Print methods with their signatures */
    method_list_t *method_list = table->methods;
    while (method_list) {
        printf("%s ( ", method_list->method->method_name);

        /* Print parameter types */
        symbol_list_t *param_list = method_list->method->params;
        int first = 1;
        while (param_list) {
            if (!first) printf(", ");
            printf("%s", type_to_string(param_list->symbol->type, param_list->symbol->is_array));
            first = 0;
            param_list = param_list->next;
        }

        printf(") %s\n", type_to_string(method_list->method->return_type, 0));

        method_list = method_list->next;
    }
}

void print_method_table(class_table_t *class_table, method_table_t *method)
{
    if (!method) return;

    printf("===== Method %s ( ", method->method_name);
    
    /* Print parameter types in signature */
    symbol_list_t *param_list = method->params;
    int first = 1;
    while (param_list) {
        if (!first) printf(", ");
        printf("%s", type_to_string(param_list->symbol->type, param_list->symbol->is_array));
        first = 0;
        param_list = param_list->next;
    }
    printf(" ) Symbol Table =====\n");

    /* Print all symbols in declaration order */
    symbol_list_t *list = method->all;
    while (list) {
        printf("%s\t%s",
               list->symbol->name,
               type_to_string(list->symbol->type, list->symbol->is_array));

        if (list->symbol->is_param) {
            printf("\tparam");
        }
        printf("\n");

        list = list->next;
    }
}

/* ============================================================
 * CLEANUP
 * ============================================================ */

void free_class_table(class_table_t *table)
{
    if (!table) return;

    /* TODO: Implement recursive cleanup of all structures */

    free(table->class_name);
    free(table);
}
