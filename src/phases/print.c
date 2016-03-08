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

node *PRTprogram(node * arg_node, info * arg_info) {
    traverse_init(PROGRAM_DECLARATIONS(arg_node), arg_info);
    return arg_node;
}

node *PRTstatements(node * arg_node, info * arg_info) {
    logging_print_indent();
    traverse_init(STATEMENTS_STATEMENT(arg_node), arg_info);
    printf(";\n");
    traverse_opt(STATEMENTS_NEXT(arg_node), arg_info);

    return arg_node;
}

node *PRTdeclarations(node * arg_node, info * arg_info) {
    traverse_init(DECLARATIONS_DECLARATION(arg_node), arg_info);
    traverse_opt(DECLARATIONS_NEXT(arg_node), arg_info);
    return arg_node;
}

node *PRTvardec(node * arg_node, info * arg_info) {
    printf("%s ", print_type(VARDEC_TYPE(arg_node)));
    traverse_init(VARDEC_VAR(arg_node), arg_info);
    return arg_node;
}

node *PRTassign(node * arg_node, info * arg_info) {
    traverse_init(ASSIGN_VAR(arg_node), arg_info);
    printf(" = ");
    traverse_init(ASSIGN_EXPRESSION(arg_node), arg_info);
    return arg_node;
}

node *PRTbinop(node * arg_node, info * arg_info) {
    printf("(");
    traverse_init(BINOP_LEFT(arg_node), arg_info);

    switch (BINOP_OP(arg_node)) {
        case BO_add: printf(" + "); break;
        case BO_sub: printf(" - "); break;
        case BO_mul: printf(" * "); break;
        case BO_div: printf(" / "); break;
        case BO_mod: printf(" %% "); break;
    }

    traverse_init(BINOP_RIGHT(arg_node), arg_info);
    printf(")");

    return arg_node;
}

node *PRTfloat(node * arg_node, info * arg_info) {
    printf("%f", FLOAT_VALUE(arg_node));
    return arg_node;
}

node *PRTint(node * arg_node, info * arg_info) {
    printf("%i", INT_VALUE(arg_node));
    return arg_node;
}

node *PRTbool(node * arg_node, info * arg_info) {
    printf(BOOL_VALUE(arg_node) ? "true" : "false");
    return arg_node;
}

node *PRTvar(node * arg_node, info * arg_info) {
    printf("%s", VAR_NAME(arg_node));
    return arg_node;
}

traverse_fun_t print_select_fun(node * arg_node) {
    switch (arg_node->nodetype) {
        case N_assign: return PRTassign;
        case N_binop: return PRTbinop;
        case N_bool: return PRTbool;
        case N_declarations: return PRTdeclarations;
        case N_float: return PRTfloat;
        case N_int: return PRTint;
        case N_program: return PRTprogram;
        case N_statements: return PRTstatements;
        case N_var: return PRTvar;
        case N_vardec: return PRTvardec;
        default: logging_log(ABORT, "This node has no print function!"); return NULL;
    }
}

node *print_init(node * syntaxtree) {
    return traverse_do(print_select_fun, syntaxtree, NULL);
}
