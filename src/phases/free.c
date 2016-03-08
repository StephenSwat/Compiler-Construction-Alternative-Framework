#include <stdlib.h>
#include "alloc.h"
#include "traverse.h"
#include "htable.h"

node *FREEassign(node * arg_node, info * arg_info) {
    traverse_opt(ASSIGN_VAR(arg_node), arg_info);
    traverse_opt(ASSIGN_EXPRESSION(arg_node), arg_info);
    free(arg_node);
    return NULL;
}

node *FREEbinop(node * arg_node, info * arg_info) {
    traverse_opt(BINOP_LEFT(arg_node), arg_info);
    traverse_opt(BINOP_RIGHT(arg_node), arg_info);
    free(arg_node);
    return NULL;
}

node *FREEbool(node * arg_node, info * arg_info) {
    free(arg_node);
    return NULL;
}

node *FREEdeclarations(node * arg_node, info * arg_info) {
    traverse_opt(DECLARATIONS_NEXT(arg_node), arg_info);
    traverse_opt(DECLARATIONS_DECLARATION(arg_node), arg_info);
    free(arg_node);
    return NULL;
}

node *FREEfloat(node * arg_node, info * arg_info) {
    free(arg_node);
    return NULL;
}

node *FREEint(node * arg_node, info * arg_info) {
    free(arg_node);
    return NULL;
}

node *FREEprogram(node * arg_node, info * arg_info) {
    traverse_opt(PROGRAM_DECLARATIONS(arg_node), arg_info);
    free(arg_node);
    return NULL;
}

node *FREEstatements(node * arg_node, info * arg_info) {
    traverse_opt(STATEMENTS_NEXT(arg_node), arg_info);
    traverse_opt(STATEMENTS_STATEMENT(arg_node), arg_info);
    free(arg_node);
    return NULL;
}

node *FREEvar(node * arg_node, info * arg_info) {
    free(VAR_NAME(arg_node));
    free(arg_node);
    return NULL;
}

node *FREEvardec(node * arg_node, info * arg_info) {
    traverse_opt(VARDEC_VAR(arg_node), arg_info);
    free(arg_node);
    return NULL;
}

traverse_fun_t free_select_fun(node * arg_node) {
    switch (arg_node->nodetype) {
        case N_assign: return FREEassign;
        case N_binop: return FREEbinop;
        case N_bool: return FREEbool;
        case N_declarations: return FREEdeclarations;
        case N_float: return FREEfloat;
        case N_int: return FREEint;
        case N_program: return FREEprogram;
        case N_statements: return FREEstatements;
        case N_var: return FREEvar;
        case N_vardec: return FREEvardec;
        default: return NULL;
    }
}

node *free_tree(node * free_node) {
    return traverse_do(free_select_fun, free_node, NULL);
}
