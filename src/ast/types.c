#include "types.h"

char *nodetype_string(nodetype t) {
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

char *type_string(typet t) {
    switch (t) {
        case TY_bool: return "bool";
        case TY_int: return "int";
        case TY_float: return "float";
        case TY_void: return "void";
        default: return "unknown";
    }
}
