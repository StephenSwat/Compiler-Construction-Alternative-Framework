#include <stdlib.h>
#include "alloc.h"
#include "traverse.h"
#include "htable.h"

node *free_assign(node * arg_node, info * arg_info) {
    traverse_opt(arg_node->N_assign.Var, arg_info);
    traverse_opt(arg_node->N_assign.Expression, arg_info);
    free(arg_node);
    return NULL;
}

node *free_binop(node * arg_node, info * arg_info) {
    traverse_opt(arg_node->N_binop.Left, arg_info);
    traverse_opt(arg_node->N_binop.Right, arg_info);
    free(arg_node);
    return NULL;
}

node *free_bool(node * arg_node, info * arg_info) {
    free(arg_node);
    return NULL;
}

node *free_declarations(node * arg_node, info * arg_info) {
    traverse_opt(arg_node->N_declarations.Next, arg_info);
    traverse_opt(arg_node->N_declarations.Declaration, arg_info);
    free(arg_node);
    return NULL;
}

node *free_float(node * arg_node, info * arg_info) {
    free(arg_node);
    return NULL;
}

node *free_int(node * arg_node, info * arg_info) {
    free(arg_node);
    return NULL;
}

node *free_program(node * arg_node, info * arg_info) {
    traverse_opt(arg_node->N_program.Declarations, arg_info);
    free(arg_node);
    return NULL;
}

node *free_statements(node * arg_node, info * arg_info) {
    traverse_opt(arg_node->N_statements.Next, arg_info);
    traverse_opt(arg_node->N_statements.Statement, arg_info);
    free(arg_node);
    return NULL;
}

node *free_var(node * arg_node, info * arg_info) {
    free(arg_node->N_var.Name);
    free(arg_node);
    return NULL;
}

node *free_vardec(node * arg_node, info * arg_info) {
    traverse_opt(arg_node->N_vardec.Var, arg_info);
    free(arg_node);
    return NULL;
}

traverse_fun_t free_select_fun(node * arg_node) {
    switch (arg_node->nodetype) {
        case N_assign: return free_assign;
        case N_binop: return free_binop;
        case N_bool: return free_bool;
        case N_declarations: return free_declarations;
        case N_float: return free_float;
        case N_int: return free_int;
        case N_program: return free_program;
        case N_statements: return free_statements;
        case N_var: return free_var;
        case N_vardec: return free_vardec;
        default: return NULL;
    }
}

node *free_tree(node * free_node) {
    return traverse_do(free_select_fun, free_node, NULL);
}
