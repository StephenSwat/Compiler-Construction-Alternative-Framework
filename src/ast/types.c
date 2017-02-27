#include "types.h"

char *nodetype_string(nodetype_t t) {
    switch (t) {
        case N_program: return "program";
        case N_statements: return "statements";
        case N_assign: return "assign";
        case N_binop: return "binop";
        case N_var: return "var";
        case N_int: return "int";
        case N_float: return "float";
        case N_bool: return "bool";
        default: return "unknown";
    }
}

char *type_string(type_t t) {
    switch (t) {
        case TY_bool: return "bool";
        case TY_int: return "int";
        case TY_float: return "float";
        case TY_void: return "void";
        default: return "unknown";
    }
}

char *binop_string(binop_t t) {
    switch (t) {
        case BO_add: return "+";
        case BO_sub: return "-";
        case BO_mul: return "*";
        case BO_div: return "/";
        case BO_mod: return "%";
        case BO_lt: return "<";
        case BO_le: return "<=";
        case BO_gt: return ">";
        case BO_ge: return ">=";
        case BO_eq: return "==";
        case BO_ne: return "!=";
        case BO_and: return "&&";
        case BO_or: return "||";
        default: return "?";
    }
}
