#pragma once
#include <stdbool.h>
#include <stdio.h>

typedef struct INFO info;

typedef struct node node;

typedef char *compiler_phase_t;

typedef node *(*traverse_fun_t)(node *, info*);

typedef traverse_fun_t (*traverse_choice_t)(node *);

typedef enum {
    BE_civvm, BE_none
} backend_t;

typedef enum {
    TY_bool, TY_int, TY_float, TY_void, TY_unknown
} typet;

typedef enum {
    BO_add, BO_sub, BO_mul, BO_div, BO_mod, BO_lt, BO_le, BO_gt, BO_ge, BO_eq,
    BO_ne, BO_and, BO_or
} binop;

typedef enum {
    N_program, N_statements, N_assign, N_binop, N_var, N_int, N_float, N_bool
} nodetype;

struct node {
    nodetype nodetype;
    int lineno, colno;
    union {
        struct {
            node *Var, *Expression;
        } N_assign;
        struct {
            binop Op;
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

struct globals_t {
    compiler_phase_t compiler_phase;
    bool treecheck;
    bool ast_colour;

    int verbosity;
    int errors;
    int warnings;

    compiler_phase_t break_phase;

    char *infile;
    FILE *outfile;
    char *currentfile;

    int line;
    int col;

    backend_t backend;

    bool changed;
};

struct globals_t global;
char *nodetype_string(nodetype t);
char *type_string(typet t);
