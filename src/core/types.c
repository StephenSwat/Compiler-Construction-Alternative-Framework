#include "types.h"

char *nodetype_string(nodetype t) {
    switch (t) {
        case N_program: return "N_program";
        case N_declarations: return "N_declarations";
        case N_statements: return "N_statements";
        case N_assign: return "N_assign";
        case N_binop: return "N_binop";
        case N_vardec: return "N_vardec";
        case N_var: return "N_var";
        case N_int: return "N_int";
        case N_float: return "N_float";
        case N_bool: return "N_bool";
        default: return "";
    }
}

char *type_string(typet t) {
    switch (t) {
        case TY_bool: return "bool";
        case TY_int: return "int";
        case TY_float: return "float";
        case TY_void: return "void";
        default: return "unknown";
    }
}

bool isExpression(node * arg_node) {
    return (arg_node->nodetype == N_binop ||
            arg_node->nodetype == N_float ||
            arg_node->nodetype == N_int ||
            arg_node->nodetype == N_bool ||
            arg_node->nodetype == N_var);
}

bool isStatement(node * arg_node) {
    return (arg_node->nodetype == N_assign);
}
