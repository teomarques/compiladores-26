/* symtab.h - Symbol Table */
#ifndef SYMTAB_H
#define SYMTAB_H

typedef struct Symbol {
    char *name;
    char *type;          /* "int", "double", "boolean", "void", "String []", "undef" */
    int is_param;        /* 1 if parameter */
    char *param_types;   /* for methods: "(int,double)" etc. NULL for variables */
    struct Symbol *next;
} Symbol;

typedef struct SymTable {
    char *name;          /* table name: "Class X" or "Method foo(int)" */
    Symbol *symbols;
    Symbol *last;        /* for maintaining insertion order */
    struct SymTable *next;
} SymTable;

/* Create a new symbol table with the given name */
SymTable *symtab_create(const char *name);

/* Add a symbol to a table. Returns 0 on success, 1 if already defined */
int symtab_add(SymTable *table, const char *name, const char *type, int is_param, const char *param_types);

/* Look up a symbol by name in a table */
Symbol *symtab_lookup(SymTable *table, const char *name);

/* Find all methods with a given name in a table */
int symtab_find_methods(SymTable *table, const char *name, Symbol **results, int max_results);

/* Print a symbol table */
void symtab_print(SymTable *table);

/* Free a symbol table */
void symtab_free(SymTable *table);

/* Free entire chain of symbol tables */
void symtab_free_all(SymTable *table);

#endif
