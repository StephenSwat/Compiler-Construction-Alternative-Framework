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
    N_program, N_statements, N_assign, N_binop, N_var, N_int, N_float, N_bool
} nodetype_t;

struct node {
    nodetype_t nodetype;
    int lineno, colno;

    union {
        struct {
            node *Var, *Expression;
        } N_assign;
        struct {
            binop_t Op;
            node *Left, *Right;
        } N_binop;
        struct {
            node *Statements;
        } N_program;
        struct {
            node *Statement, *Next;
        } N_statements;
        struct {
            char *Name;
        } N_var;
        struct {
            float Value;
        } N_float;
        struct {
            bool Value;
        } N_bool;
        struct {
            int Value;
        } N_int;
    };
};

char *nodetype_string(nodetype_t t);
char *type_string(type_t t);
char *binop_string(binop_t t);
