#include <stdlib.h>
#include "logging.h"
#include "traverse.h"

struct info {
    int sum;
};

static node *sum_int(node * this, info * info) {
    info->sum += this->N_int.Value;
    return traverse_children(this, info);
}

traverse_fun_t sum_ints_select_fun(node * this) {
    switch (this->nodetype) {
        case N_int: return sum_int;
        default: return traverse_children;
    }
}

node *sum_ints_init(node *tree) {
    info info;
    info.sum = 0;

    node *result = traverse_do(sum_ints_select_fun, tree, &info);
    logging_log(NOTE, "Sum of all integers: %d", info.sum);

    return result;
}
