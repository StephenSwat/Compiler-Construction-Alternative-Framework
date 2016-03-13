#include <stdlib.h>
#include <stdbool.h>
#include "alloc.h"
#include "types.h"
#include "traverse.h"
#include "logging.h"

struct INFO {
    int add, sub;
};

node *count_operators_binop(node * arg_node, info * arg_info) {
    switch (arg_node->N_binop.Op) {
        case BO_add:
            arg_info->add++;
            break;
        case BO_sub:
            arg_info->sub++;
            break;
        default:
            break;
    }

    return traverse_sons(arg_node, arg_info);
}

traverse_fun_t count_operators_select_fun(node * arg_node) {
    switch (arg_node->nodetype) {
        case N_binop: return count_operators_binop;
        default: return traverse_sons;
    }
}

node *count_operators_init(node * syntaxtree) {
    info arg_info;

    arg_info.add = 0;
    arg_info.sub = 0;

    syntaxtree = traverse_do(count_operators_select_fun, syntaxtree, &arg_info);

    logging_log(NOTE, "Number of add operators: %d", arg_info.add);
    logging_log(NOTE, "Number of sub operators: %d", arg_info.sub);

    return syntaxtree;
}
