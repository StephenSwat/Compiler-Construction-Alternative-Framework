#pragma once
#include <stdbool.h>

typedef struct INFO info;

typedef struct NODE node;

typedef char *compiler_phase_t;

typedef node *(*traverse_fun_t)(node *, info*);

typedef traverse_fun_t (*traverse_choice_t)(node *);

typedef enum {
    BE_civvm
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
    } attribs;
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
    char *outfile;
    char *currentfile;

    int line;
    int col;

    backend_t backend;

    bool changed;
} globals_t;

#define PROGRAM_DECLARATIONS(n) ((n)->attribs.N_program.Declarations)

#define DECLARATIONS_DECLARATION(n) ((n)->attribs.N_declarations.Declaration)
#define DECLARATIONS_NEXT(n) ((n)->attribs.N_declarations.Next)

#define STATEMENTS_STATEMENT(n) ((n)->attribs.N_statements.Statement)
#define STATEMENTS_NEXT(n) ((n)->attribs.N_statements.Next)

#define ASSIGN_VAR(n) ((n)->attribs.N_assign.Var)
#define ASSIGN_EXPRESSION(n) ((n)->attribs.N_assign.Expression)

#define BINOP_LEFT(n) ((n)->attribs.N_binop.Left)
#define BINOP_RIGHT(n) ((n)->attribs.N_binop.Right)
#define BINOP_OP(n) ((n)->attribs.N_binop.Op)

#define VARDEC_VAR(n) ((n)->attribs.N_vardec.Var)
#define VARDEC_TYPE(n) ((n)->attribs.N_vardec.Type)

#define VAR_NAME(n) ((n)->attribs.N_var.Name)

#define INT_VALUE(n) ((n)->attribs.N_int.Value)
#define FLOAT_VALUE(n) ((n)->attribs.N_float.Value)
#define BOOL_VALUE(n) ((n)->attribs.N_bool.Value)

globals_t global;
char *nodetype_string(nodetype t);
char *type_string(typet t);
bool isExpression(node *);
bool isStatement(node *);
