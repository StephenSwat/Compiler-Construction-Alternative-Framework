#include <stdlib.h>
#include <stdbool.h>
#include "alloc.h"
#include "types.h"
#include "traverse.h"
#include "logging.h"

struct INFO {
    int add, sub;
};

node *count_operators_binop(node * this, info * info) {
    switch (this->N_binop.Op) {
        case BO_add: info->add++; break;
        case BO_sub: info->sub++; break;
        default: break;
    }

    return traverse_sons(this, info);
}

traverse_fun_t count_operators_select_fun(node * this) {
    switch (this->nodetype) {
        case N_binop: return count_operators_binop;
        default: return traverse_sons;
    }
}

node *count_operators_init(node * syntaxtree) {
    info info;
    info.add = 0;
    info.sub = 0;

    syntaxtree = traverse_do(count_operators_select_fun, syntaxtree, &info);

    logging_log(NOTE, "Number of add operators: %d", info.add);
    logging_log(NOTE, "Number of sub operators: %d", info.sub);

    return syntaxtree;
}
