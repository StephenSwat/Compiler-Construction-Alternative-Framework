#pragma once
#include <stdbool.h>
#include <stdio.h>

typedef struct INFO info;

typedef struct NODE node;

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
    BO_add, BO_sub, BO_mul, BO_div, BO_mod
} binop;

typedef enum {
    N_program, N_declarations, N_statements, N_assign, N_binop, N_vardec,
    N_var, N_int, N_float, N_bool
} nodetype;

struct NODE {
    nodetype nodetype;
    int lineno, colno;
    union {
        struct {
            node *Var;
            node *Expression;
        } N_assign;
        struct {
            binop Op;
            node *Left;
            node *Right;
        } N_binop;
        struct {
            node *Declaration;
            node *Next;
        } N_declarations;
        struct {
            node *Declarations;
        } N_program;
        struct {
            node *Statement;
            node *Next;
        } N_statements;
        struct {
            char *Name;
        } N_var;
        struct {
            typet Type;
            node *Var;
        } N_vardec;
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

typedef struct GLOBALS_T {
    compiler_phase_t compiler_phase;
    compiler_phase_t compiler_subphase;
    bool treecheck;
    bool ast_colour;

    int verbosity;
    int errors;
    int warnings;

    compiler_phase_t break_phase;
    compiler_phase_t break_subphase;

    char *infile;
    FILE *outfile;
    char *currentfile;

    int line;
    int col;

    backend_t backend;

    bool changed;
} globals_t;

globals_t global;
char *nodetype_string(nodetype t);
char *type_string(typet t);
bool isExpression(node *);
bool isStatement(node *);
