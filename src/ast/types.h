#pragma once
#include <stdbool.h>
#include <stdio.h>

typedef struct info info;

typedef struct node node;

typedef char *compiler_phase_t;

typedef node *(*traverse_fun_t)(node *, info*);

typedef traverse_fun_t (*traverse_choice_t)(node *);

typedef enum {
    BE_civvm, BE_dot, BE_none
} backend_t;

typedef enum {
    TY_bool, TY_int, TY_float, TY_void
} type_t;

typedef enum {
    BO_add, BO_sub, BO_mul, BO_div, BO_mod, BO_lt, BO_le, BO_gt, BO_ge, BO_eq,
    BO_ne, BO_and, BO_or
} binop_t;

typedef enum {
    program_n, statements_n, assign_n, binop_n, var_n, int_n, float_n, bool_n
} nodetype_t;

struct node {
    nodetype_t nodetype;
    int lineno, colno;

    union {
        struct {
            node *var, *expression;
        } assign_n;
        struct {
            binop_t op;
            node *left, *right;
        } binop_n;
        struct {
            node *statements;
        } program_n;
        struct {
            node *statement, *next;
        } statements_n;
        struct {
            char *name;
        } var_n;
        struct {
            float value;
        } float_n;
        struct {
            bool value;
        } bool_n;
        struct {
            int value;
        } int_n;
    };
};

char *nodetype_string(nodetype_t t);
char *type_string(type_t t);
char *binop_string(binop_t t);
