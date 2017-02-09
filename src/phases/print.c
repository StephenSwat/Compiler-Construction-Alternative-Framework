#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "traverse.h"
#include "logging.h"
#include "alloc.h"

#define CSTACK_PUSH(c) char *old_c = info->colour; info->colour = c; if (global.ast_colour) fprintf(stderr, "%s", info->colour);
#define CSTACK_POP() info->colour = old_c; if (global.ast_colour) fprintf(stderr, "%s", info->colour);

#define RESET            "\033[39m" // Used for returning to normal
#define RED              "\033[31m" // used for statements
#define GREEN            "\033[32m" // Used for functions
#define YELLOW           "\033[33m" // Used for constants
#define BLUE             "\033[34m" // Used for variable declarations
#define MAGENTA          "\033[35m" // Used for expressions
#define CYAN             "\033[36m" // Used for variables

static char *print_type(typet type) {
    switch (type) {
        case TY_int:
            return "int";
        case TY_bool:
            return "bool";
        case TY_float:
            return "float";
        case TY_void:
            return "void";
        default:
            logging_log(ABORT, "Unknown types should not exist in the AST.");
            return NULL;
    }
}

struct info {
    char *colour;
};

node *print_program(node * this, info * info) {
    traverse_init(this->N_program.Statements, info);
    return this;
}

node *print_statements(node * this, info * info) {
    logging_print_indent();

    CSTACK_PUSH(RED);
    traverse_init(this->N_statements.Statement, info);
    CSTACK_POP();

    fprintf(stderr, ";\n");
    traverse_init(this->N_statements.Next, info);

    return this;
}

node *print_assign(node * this, info * info) {
    traverse_init(this->N_assign.Var, info);
    fprintf(stderr, " = ");
    traverse_init(this->N_assign.Expression, info);
    return this;
}

node *print_binop(node * this, info * info) {
    CSTACK_PUSH(MAGENTA);
    fprintf(stderr, "(");
    traverse_init(this->N_binop.Left, info);

    switch (this->N_binop.Op) {
        case BO_add: fprintf(stderr, " + "); break;
        case BO_sub: fprintf(stderr, " - "); break;
        case BO_mul: fprintf(stderr, " * "); break;
        case BO_div: fprintf(stderr, " / "); break;
        case BO_mod: fprintf(stderr, " %% "); break;
        case BO_lt: fprintf(stderr, " < "); break;
        case BO_le: fprintf(stderr, " <= "); break;
        case BO_gt: fprintf(stderr, " > "); break;
        case BO_ge: fprintf(stderr, " >= "); break;
        case BO_eq: fprintf(stderr, " == "); break;
        case BO_ne: fprintf(stderr, " != "); break;
        case BO_or: fprintf(stderr, " || "); break;
        case BO_and: fprintf(stderr, " && "); break;
    }

    traverse_init(this->N_binop.Right, info);
    fprintf(stderr, ")");
    CSTACK_POP();

    return this;
}

node *print_float(node * this, info * info) {
    CSTACK_PUSH(YELLOW);
    fprintf(stderr, "%f", this->N_float.Value);
    CSTACK_POP();
    return this;
}

node *print_int(node * this, info * info) {
    CSTACK_PUSH(YELLOW);
    fprintf(stderr, "%i", this->N_int.Value);
    CSTACK_POP();
    return this;
}

node *print_bool(node * this, info * info) {
    CSTACK_PUSH(YELLOW);
    fprintf(stderr, this->N_bool.Value ? "true" : "false");
    CSTACK_POP();
    return this;
}

node *print_var(node * this, info * info) {
    CSTACK_PUSH(CYAN);
    fprintf(stderr, "%s", this->N_var.Name);
    CSTACK_POP();
    return this;
}

traverse_fun_t print_select_fun(node * this) {
    switch (this->nodetype) {
        case N_assign: return print_assign;
        case N_binop: return print_binop;
        case N_bool: return print_bool;
        case N_float: return print_float;
        case N_int: return print_int;
        case N_program: return print_program;
        case N_statements: return print_statements;
        case N_var: return print_var;
        default: logging_log(ABORT, "This node has no print function!"); return NULL;
    }
}

node *print_init(node * syntaxtree) {info info;
    info.colour = RESET;
    return traverse_do(print_select_fun, syntaxtree, &info);
}
