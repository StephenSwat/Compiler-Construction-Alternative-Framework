#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "traverse.h"
#include "logging.h"
#include "node.gen.h"

#define CSTACK_PUSH(c) char *old_c = info->colour; info->colour = c; if (ast_colour) fprintf(stderr, "%s", info->colour);
#define CSTACK_POP() info->colour = old_c; if (ast_colour) fprintf(stderr, "%s", info->colour);

#define RESET            "\033[39m" // Used for returning to normal
#define RED              "\033[31m" // used for statements
#define GREEN            "\033[32m" // Used for functions
#define YELLOW           "\033[33m" // Used for constants
#define BLUE             "\033[34m" // Used for variable declarations
#define MAGENTA          "\033[35m" // Used for expressions
#define CYAN             "\033[36m" // Used for variables

bool ast_colour = true;

struct info {
    char *colour;
};

node *print_program(node * this, info * info) {
    traverse_init(this->program_n.statements, info);
    return this;
}

node *print_statements(node * this, info * info) {
    logging_print_indent();

    CSTACK_PUSH(RED);
    traverse_init(this->statements_n.statement, info);
    CSTACK_POP();

    fprintf(stderr, ";\n");
    traverse_init(this->statements_n.next, info);

    return this;
}

node *print_assign(node * this, info * info) {
    traverse_init(this->assign_n.var, info);
    fprintf(stderr, " = ");
    traverse_init(this->assign_n.value, info);
    return this;
}

node *print_binop(node * this, info * info) {
    CSTACK_PUSH(MAGENTA);
    fprintf(stderr, "(");
    traverse_init(this->binop_n.left, info);
    fprintf(stderr, " %s ", binop_string(this->binop_n.op));
    traverse_init(this->binop_n.right, info);
    fprintf(stderr, ")");
    CSTACK_POP();

    return this;
}

node *print_float(node * this, info * info) {
    CSTACK_PUSH(YELLOW);
    fprintf(stderr, "%f", this->float_n.value);
    CSTACK_POP();
    return this;
}

node *print_int(node * this, info * info) {
    CSTACK_PUSH(YELLOW);
    fprintf(stderr, "%i", this->int_n.value);
    CSTACK_POP();
    return this;
}

node *print_bool(node * this, info * info) {
    CSTACK_PUSH(YELLOW);
    fprintf(stderr, this->bool_n.value ? "true" : "false");
    CSTACK_POP();
    return this;
}

node *print_var(node * this, info * info) {
    CSTACK_PUSH(CYAN);
    fprintf(stderr, "%s", this->var_n.name);
    CSTACK_POP();
    return this;
}

node *print_init(node * syntaxtree) {
    traverse_fun_t select_fun(node * this) {
        switch (this->nodetype) {
            case assign_n: return print_assign;
            case binop_n: return print_binop;
            case bool_n: return print_bool;
            case float_n: return print_float;
            case int_n: return print_int;
            case program_n: return print_program;
            case statements_n: return print_statements;
            case var_n: return print_var;
            default: logging_log(ABORT, "This node has no print function!"); return NULL;
        }
    }

    info info = { RESET };

    return traverse_do(select_fun, syntaxtree, &info);
}
