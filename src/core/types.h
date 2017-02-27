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

typedef enum nodetype_t nodetype_t;

char *nodetype_string(nodetype_t t);
char *type_string(type_t t);
char *binop_string(binop_t t);
