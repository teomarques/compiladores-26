#ifndef _SEMANTIC_H
#define _SEMANTIC_H

#include "ast.h"

/* ============================================================
 * TYPE SYSTEM
 * ============================================================ */

typedef enum {
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_BOOL,
    TYPE_STRING_ARRAY,
    TYPE_VOID,
    TYPE_UNDEF
} type_t;

typedef struct {
    char *name;
    type_t type;
    int is_array;          /* 1 if it's an array type */
    int is_param;          /* 1 if it's a formal parameter */
} symbol_t;

typedef struct symbol_list {
    symbol_t *symbol;
    struct symbol_list *next;
} symbol_list_t;

typedef struct {
    char *method_name;     /* e.g., "factorial" or "main" */
    type_t return_type;
    symbol_list_t *params;    /* Parameter list */
    symbol_list_t *locals;    /* Local variables */
    symbol_list_t *all;       /* All symbols (params + locals + return) */
} method_table_t;

typedef struct method_list {
    method_table_t *method;
    struct method_list *next;
} method_list_t;

typedef struct {
    char *class_name;
    symbol_list_t *fields;    /* Global variables */
    method_list_t *methods;   /* Methods */
} class_table_t;

/* ============================================================
 * ANNOTATION FOR AST NODES
 * ============================================================ */

typedef struct {
    type_t type;
    int is_array;
} node_annotation_t;

/* ============================================================
 * SEMANTIC ANALYSIS FUNCTIONS
 * ============================================================ */

/* Symbol table management */
class_table_t *create_class_table(const char *class_name);
void add_field_to_table(class_table_t *table, const char *name, type_t type, int is_array);
void add_method_to_table(class_table_t *table, method_table_t *method);

method_table_t *create_method_table(const char *name, type_t return_type);
void add_param_to_method(method_table_t *method, const char *name, type_t type, int is_array);
void add_local_to_method(method_table_t *method, const char *name, type_t type, int is_array);
void add_return_to_method(method_table_t *method, type_t return_type);

symbol_t *lookup_symbol(method_table_t *method, const char *name);
symbol_t *lookup_field(class_table_t *table, const char *name);
method_table_t *lookup_method(class_table_t *table, const char *name, int param_count);

/* Symbol table printing */
void print_class_table(class_table_t *table);
void print_method_table(class_table_t *class_table, method_table_t *method);

/* Semantic analysis */
void semantic_analysis(struct node *root);

/* Type checking and annotation */
type_t get_node_type(struct node *node);
void annotate_ast(struct node *root, class_table_t *class_table);
node_annotation_t *get_annotation(struct node *node);
void set_annotation(struct node *node, type_t type, int is_array);

/* Error reporting */
void semantic_error(int line, int col, const char *fmt, ...);

/* Type conversion and comparison */
const char *type_to_string(type_t type, int is_array);
int types_compatible(type_t actual, type_t expected, int actual_array, int expected_array);
int types_equal(type_t t1, type_t t2, int a1, int a2);

/* Cleanup */
void free_class_table(class_table_t *table);

#endif
