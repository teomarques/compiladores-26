/*
 * Autores:
 *   Simão Tomás Botas Carvalho - 2021223055
 *   Teodoro Marques          - 2023211717
 *
 * Meta 3 -- Symbol Table Building (Phase 1)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"
#include "ast.h"
#include "semantic.h"

/* ============================================================
 * TYPE EXTRACTION
 * ============================================================ */

type_t extract_type_from_node(struct node *type_node, int *is_array)
{
    type_t type = TYPE_UNDEF;

    if (!type_node) {
        if (is_array) *is_array = 0;
        return TYPE_UNDEF;
    }

    *is_array = 0;

    switch (type_node->category) {
        case N_Int:
            type = TYPE_INT;
            break;
        case N_Double:
            type = TYPE_DOUBLE;
            break;
        case N_Bool:
            type = TYPE_BOOL;
            break;
        case N_StringArray:
            type = TYPE_STRING_ARRAY;
            *is_array = 1;
            break;
        case N_Void:
            type = TYPE_VOID;
            break;
        default:
            type = TYPE_UNDEF;
            break;
    }

    return type;
}

/* ============================================================
 * DUPLICATE SYMBOL CHECKING
 * ============================================================ */

int symbol_exists(method_table_t *method, const char *name)
{
    if (!method) return 0;

    symbol_list_t *list = method->all;
    while (list) {
        if (strcmp(list->symbol->name, name) == 0) {
            return 1;
        }
        list = list->next;
    }

    return 0;
}

int field_exists(class_table_t *table, const char *name)
{
    if (!table) return 0;

    symbol_list_t *list = table->fields;
    while (list) {
        if (strcmp(list->symbol->name, name) == 0) {
            return 1;
        }
        list = list->next;
    }

    return 0;
}

/* ============================================================
 * HELPER: Get method signature string
 * ============================================================ */

char *get_method_signature(method_table_t *method)
{
    static char buffer[256];
    int pos = 0;

    if (!method) return "";

    pos = snprintf(buffer, sizeof(buffer), "%s ( ", method->method_name);

    /* Add parameter types */
    symbol_list_t *param_list = method->params;
    int first = 1;
    while (param_list) {
        if (!first) {
            pos += snprintf(buffer + pos, sizeof(buffer) - pos, ", ");
        }

        const char *type_str = type_to_string(
            param_list->symbol->type,
            param_list->symbol->is_array
        );
        pos += snprintf(buffer + pos, sizeof(buffer) - pos, "%s", type_str);

        first = 0;
        param_list = param_list->next;
    }

    pos += snprintf(buffer + pos, sizeof(buffer) - pos, " )");

    return buffer;
}

/* ============================================================
 * PHASE 1: BUILD CLASS SYMBOL TABLE
 * ============================================================ */

void build_class_symbol_table(struct node *root, class_table_t *class_table)
{
    if (!root || root->category != N_Program) return;

    struct node_list *child = root->children;

    /* Skip class identifier (first child) */
    if (child) child = child->next;

    /* Process class members (fields and methods) */
    while (child) {
        struct node *member = child->node;

        if (!member) {
            child = child->next;
            continue;
        }

        switch (member->category) {
            case N_FieldDecl: {
                /* FieldDecl has structure:
                 * - type node
                 * - identifier(s)
                 */
                struct node_list *field_child = member->children;

                if (field_child && field_child->node) {
                    struct node *type_node = field_child->node;
                    int is_array = 0;
                    type_t type = extract_type_from_node(type_node, &is_array);

                    /* Move to identifier(s) */
                    field_child = field_child->next;
                    while (field_child && field_child->node) {
                        struct node *id_node = field_child->node;

                        if (id_node->category == N_Identifier && id_node->token) {
                            add_field_to_table(
                                class_table,
                                id_node->token,
                                type,
                                is_array
                            );
                        }

                        field_child = field_child->next;
                    }
                }
                break;
            }

            case N_MethodDecl: {
                /* Extract method header info */
                struct node_list *method_child = member->children;

                if (method_child && method_child->node) {
                    struct node *header = method_child->node;

                    if (header->category == N_MethodHeader) {
                        struct node_list *header_child = header->children;

                        /* Extract return type */
                        if (header_child && header_child->node) {
                            int is_array = 0;
                            type_t return_type = extract_type_from_node(
                                header_child->node,
                                &is_array
                            );
                            header_child = header_child->next;

                            /* Extract method name */
                            if (header_child && header_child->node &&
                                header_child->node->category == N_Identifier) {
                                char *method_name = header_child->node->token;

                                method_table_t *method_table =
                                    create_method_table(method_name, return_type);

                                add_method_to_table(class_table, method_table);
                            }
                        }
                    }
                }
                break;
            }

            default:
                break;
        }

        child = child->next;
    }
}

/* ============================================================
 * PHASE 1B: BUILD METHOD SYMBOL TABLES
 * ============================================================ */

void build_method_symbol_tables(struct node *root, class_table_t *class_table)
{
    if (!root || root->category != N_Program) return;

    struct node_list *child = root->children;

    /* Skip class identifier */
    if (child) child = child->next;

    /* Process class members */
    while (child) {
        struct node *member = child->node;

        if (!member || member->category != N_MethodDecl) {
            child = child->next;
            continue;
        }

        /* Get method header */
        struct node_list *method_child = member->children;
        if (!method_child || !method_child->node ||
            method_child->node->category != N_MethodHeader) {
            child = child->next;
            continue;
        }

        struct node *header = method_child->node;
        struct node_list *header_child = header->children;

        /* Skip return type */
        if (header_child) header_child = header_child->next;

        /* Get method name */
        char *method_name = NULL;
        if (header_child && header_child->node &&
            header_child->node->category == N_Identifier) {
            method_name = header_child->node->token;
            header_child = header_child->next;
        }

        if (!method_name) {
            child = child->next;
            continue;
        }

        /* Lookup the method in class table */
        method_table_t *method_table = lookup_method(class_table, method_name, -1);
        if (!method_table) {
            child = child->next;
            continue;
        }

        /* Extract parameters from MethodParams */
        if (header_child && header_child->node &&
            header_child->node->category == N_MethodParams) {
            struct node *params_node = header_child->node;
            struct node_list *param_list = params_node->children;

            while (param_list && param_list->node) {
                struct node *param = param_list->node;

                if (param->category == N_ParamDecl) {
                    struct node_list *param_child = param->children;

                    /* Get type */
                    if (param_child && param_child->node) {
                        int is_array = 0;
                        type_t param_type =
                            extract_type_from_node(param_child->node, &is_array);
                        param_child = param_child->next;

                        /* Get parameter name */
                        if (param_child && param_child->node &&
                            param_child->node->category == N_Identifier) {
                            add_param_to_method(
                                method_table,
                                param_child->node->token,
                                param_type,
                                is_array
                            );
                        }
                    }
                }

                param_list = param_list->next;
            }
        }

        /* Extract local variables from MethodBody */
        method_child = method_child->next;
        if (method_child && method_child->node &&
            method_child->node->category == N_MethodBody) {
            struct node *body = method_child->node;
            struct node_list *body_child = body->children;

            while (body_child && body_child->node) {
                struct node *stmt = body_child->node;

                if (stmt->category == N_VarDecl) {
                    struct node_list *var_child = stmt->children;

                    /* Get type */
                    if (var_child && var_child->node) {
                        int is_array = 0;
                        type_t var_type =
                            extract_type_from_node(var_child->node, &is_array);
                        var_child = var_child->next;

                        /* Get variable name(s) */
                        while (var_child && var_child->node) {
                            struct node *id_node = var_child->node;

                            if (id_node->category == N_Identifier) {
                                add_local_to_method(
                                    method_table,
                                    id_node->token,
                                    var_type,
                                    is_array
                                );
                            }

                            var_child = var_child->next;
                        }
                    }
                }

                body_child = body_child->next;
            }
        }

        /* Add return symbol to method table */
        add_return_to_method(method_table, method_table->return_type);

        child = child->next;
    }
}
