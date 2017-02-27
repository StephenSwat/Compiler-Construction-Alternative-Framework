#include "types.h"

char *nodetype_string(nodetype_t t) {
    switch (t) {
        case program_n: return "program";
        case statements_n: return "statements";
        case assign_n: return "assign";
        case binop_n: return "binop";
        case var_n: return "var";
        case int_n: return "int";
        case float_n: return "float";
        case bool_n: return "bool";
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
