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
    traverse_init(arg_node->N_program.Declarations, arg_info);
    return arg_node;
}

node *PRTstatements(node * arg_node, info * arg_info) {
    logging_print_indent();
    traverse_init(arg_node->N_statements.Statement, arg_info);
    printf(";\n");
    traverse_opt(arg_node->N_statements.Next, arg_info);

    return arg_node;
}

node *PRTdeclarations(node * arg_node, info * arg_info) {
    traverse_init(arg_node->N_declarations.Declaration, arg_info);
    traverse_opt(arg_node->N_declarations.Next, arg_info);
    return arg_node;
}

node *PRTvardec(node * arg_node, info * arg_info) {
    printf("%s ", print_type(arg_node->N_vardec.Type));
    traverse_init(arg_node->N_vardec.Var, arg_info);
    return arg_node;
}

node *PRTassign(node * arg_node, info * arg_info) {
    traverse_init(arg_node->N_assign.Var, arg_info);
    printf(" = ");
    traverse_init(arg_node->N_assign.Expression, arg_info);
    return arg_node;
}

node *PRTbinop(node * arg_node, info * arg_info) {
    printf("(");
    traverse_init(arg_node->N_binop.Left, arg_info);

    switch (arg_node->N_binop.Op) {
        case BO_add: printf(" + "); break;
        case BO_sub: printf(" - "); break;
        case BO_mul: printf(" * "); break;
        case BO_div: printf(" / "); break;
        case BO_mod: printf(" %% "); break;
    }

    traverse_init(arg_node->N_binop.Right, arg_info);
    printf(")");

    return arg_node;
}

node *PRTfloat(node * arg_node, info * arg_info) {
    printf("%f", arg_node->N_float.Value);
    return arg_node;
}

node *PRTint(node * arg_node, info * arg_info) {
    printf("%i", arg_node->N_int.Value);
    return arg_node;
}

node *PRTbool(node * arg_node, info * arg_info) {
    printf(arg_node->N_bool.Value ? "true" : "false");
    return arg_node;
}

node *PRTvar(node * arg_node, info * arg_info) {
    printf("%s", arg_node->N_var.Name);
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
