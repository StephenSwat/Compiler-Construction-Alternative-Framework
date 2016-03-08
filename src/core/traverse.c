#include <stdlib.h>
#include "traverse.h"
#include "alloc.h"

static traverse_choice_t travstack;

node *traverse_none(node * arg_node, info * arg_info) {
    return arg_node;
}

node *traverse_sons(node * arg_node, info * arg_info) {
    switch (arg_node->nodetype) {
        case N_program:
            traverse_opt(PROGRAM_DECLARATIONS(arg_node), arg_info);
            break;
        case N_declarations:
            traverse_opt(DECLARATIONS_DECLARATION(arg_node), arg_info);
            traverse_opt(DECLARATIONS_NEXT(arg_node), arg_info);
            break;
        case N_statements:
            traverse_opt(STATEMENTS_STATEMENT(arg_node), arg_info);
            traverse_opt(STATEMENTS_NEXT(arg_node), arg_info);
            break;
        case N_assign:
            traverse_opt(ASSIGN_VAR(arg_node), arg_info);
            traverse_opt(ASSIGN_EXPRESSION(arg_node), arg_info);
            break;
        case N_binop:
            traverse_opt(BINOP_LEFT(arg_node), arg_info);
            traverse_opt(BINOP_RIGHT(arg_node), arg_info);
            break;
        case N_vardec:
            traverse_opt(VARDEC_VAR(arg_node), arg_info);
            break;
        case N_var:
        case N_int:
        case N_float:
        case N_bool:
            break;
    }

    return arg_node;
}

node *traverse_init(node * arg_node, info * arg_info) {
    global.line = arg_node->lineno;
    global.col = arg_node->colno;
    return travstack(arg_node)(arg_node, arg_info);
}

node *traverse_opt(node * arg_node, info * arg_info) {
    return arg_node ? traverse_init(arg_node, arg_info) : arg_node;
}

node *traverse_do(traverse_choice_t fun, node * arg_node, info * arg_info) {
    traverse_choice_t old = travstack;
    travstack = fun;
    node *rv = traverse_init(arg_node, arg_info);
    travstack = old;
    return rv;
}
