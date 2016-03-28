#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "traverse.h"
#include "logging.h"
#include "alloc.h"

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

node *print_program(node * this, info * info) {
    traverse_init(this->N_program.Declarations, info);
    return this;
}

node *print_statements(node * this, info * info) {
    logging_print_indent();
    traverse_init(this->N_statements.Statement, info);
    printf(";\n");
    traverse_opt(this->N_statements.Next, info);

    return this;
}

node *print_declarations(node * this, info * info) {
    traverse_init(this->N_declarations.Declaration, info);
    traverse_opt(this->N_declarations.Next, info);
    return this;
}

node *print_vardec(node * this, info * info) {
    printf("%s ", print_type(this->N_vardec.Type));
    traverse_init(this->N_vardec.Var, info);
    return this;
}

node *print_assign(node * this, info * info) {
    traverse_init(this->N_assign.Var, info);
    printf(" = ");
    traverse_init(this->N_assign.Expression, info);
    return this;
}

node *print_binop(node * this, info * info) {
    printf("(");
    traverse_init(this->N_binop.Left, info);

    switch (this->N_binop.Op) {
        case BO_add: printf(" + "); break;
        case BO_sub: printf(" - "); break;
        case BO_mul: printf(" * "); break;
        case BO_div: printf(" / "); break;
        case BO_mod: printf(" %% "); break;
    }

    traverse_init(this->N_binop.Right, info);
    printf(")");

    return this;
}

node *print_float(node * this, info * info) {
    printf("%f", this->N_float.Value);
    return this;
}

node *print_int(node * this, info * info) {
    printf("%i", this->N_int.Value);
    return this;
}

node *print_bool(node * this, info * info) {
    printf(this->N_bool.Value ? "true" : "false");
    return this;
}

node *print_var(node * this, info * info) {
    printf("%s", this->N_var.Name);
    return this;
}

traverse_fun_t print_select_fun(node * this) {
    switch (this->nodetype) {
        case N_assign: return print_assign;
        case N_binop: return print_binop;
        case N_bool: return print_bool;
        case N_declarations: return print_declarations;
        case N_float: return print_float;
        case N_int: return print_int;
        case N_program: return print_program;
        case N_statements: return print_statements;
        case N_var: return print_var;
        case N_vardec: return print_vardec;
        default: logging_log(ABORT, "This node has no print function!"); return NULL;
    }
}

node *print_init(node * syntaxtree) {
    return traverse_do(print_select_fun, syntaxtree, NULL);
}
