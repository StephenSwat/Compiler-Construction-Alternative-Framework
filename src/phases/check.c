#include <string.h>
#include "check.h"
#include "types.h"
#include "traverse.h"
#include "logging.h"
#include "alloc.h"

node *check_assign(node * arg_node, info * arg_info) {
    if (!ASSIGN_EXPRESSION(arg_node) || !isExpression(ASSIGN_EXPRESSION(arg_node)))
        logging_line(ERROR, "ASSIGN_EXPRESSION hasn't the right type or does not exist. It should be: Nodeset: Expression");

    if (!ASSIGN_VAR(arg_node) || ASSIGN_VAR(arg_node)->nodetype != N_var)
        logging_line(ERROR, "ASSIGN_VAR hasn't the right type or does not exist. It should be: N_var");

    traverse_sons(arg_node, arg_info);
    return arg_node;
}

node *check_binop(node * arg_node, info * arg_info) {
    if (!BINOP_LEFT(arg_node) || !(isExpression(BINOP_LEFT(arg_node))))
        logging_line(ERROR, "BINOP_LEFT hasn't the right type or does not exist. It should be: Nodeset: Expression");

    if (!BINOP_RIGHT(arg_node) || !isExpression(BINOP_RIGHT(arg_node)))
        logging_line(ERROR, "BINOP_RIGHT hasn't the right type or does not exist. It should be: Nodeset: Expression");

    traverse_sons(arg_node, arg_info);
    return arg_node;
}

node *check_declarations(node * arg_node, info * arg_info) {
    if (!DECLARATIONS_DECLARATION(arg_node))
        logging_line(ERROR, "DECLARATIONS_DECLARATION hasn't the right type or does not exist. It should be: Nodeset: Declaration");

    if (DECLARATIONS_NEXT(arg_node) && DECLARATIONS_NEXT(arg_node)->nodetype != N_declarations)
        logging_line(ERROR, "DECLARATIONS_NEXT hasnt the right type. It should be: N_declarations");

    traverse_sons(arg_node, arg_info);
    return arg_node;
}

node *check_program(node * arg_node, info * arg_info) {
    if (!PROGRAM_DECLARATIONS(arg_node) || PROGRAM_DECLARATIONS(arg_node)->nodetype != N_declarations)
        logging_line(ERROR, "PROGRAM_DECLARATIONS hasn't the right type or does not exist. It should be: N_declarations");

    traverse_sons(arg_node, arg_info);
    return arg_node;
}

node *check_statements(node * arg_node, info * arg_info) {
    if (STATEMENTS_NEXT(arg_node) && STATEMENTS_NEXT(arg_node)->nodetype != N_statements)
        logging_line(ERROR, "STATEMENTS_NEXT hasnt the right type. It should be: N_statements");

    if (!STATEMENTS_STATEMENT(arg_node) || !isStatement(STATEMENTS_STATEMENT(arg_node)))
        logging_line(ERROR, "STATEMENTS_STATEMENT hasn't the right type or does not exist. It should be: Nodeset: Statement");

    traverse_sons(arg_node, arg_info);
    return arg_node;
}

node *check_var(node * arg_node, info * arg_info) {
    if (!VAR_NAME(arg_node))
        logging_line(ERROR, "mandatory attribute VAR_NAME is NULL");

    traverse_sons(arg_node, arg_info);
    return arg_node;
}

node *check_vardec(node * arg_node, info * arg_info) {
    if (!VARDEC_VAR(arg_node) || VARDEC_VAR(arg_node)->nodetype != N_var)
        logging_line(ERROR, "VARDEC_VAR hasn't the right type or does not exist. It should be: N_var");

    traverse_sons(arg_node, arg_info);
    return arg_node;
}

traverse_fun_t check_select_fun(node * arg_node) {
    switch (arg_node->nodetype) {
        case N_assign: return check_assign;
        case N_binop: return check_binop;
        case N_bool: return traverse_none;
        case N_declarations: return check_declarations;
        case N_float: return traverse_none;
        case N_int: return traverse_none;
        case N_program: return check_program;
        case N_statements: return check_statements;
        case N_var: return check_var;
        case N_vardec: return check_vardec;
        default: logging_log(ABORT, "Cannot check this nodetype."); return NULL;
    }
}

node *check_init(node * syntax_tree) {
    return traverse_do(check_select_fun, syntax_tree, NULL);
}
