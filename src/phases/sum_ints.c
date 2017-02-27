#include <stdlib.h>
#include "logging.h"
#include "traverse.h"

struct info {
    int sum;
};

static node *sum_int(node * this, info * info) {
    info->sum += this->int_n.value;
    return traverse_children(this, info);
}

node *sum_ints_init(node *tree) {
    traverse_fun_t select_fun(node * this) {
        switch (this->nodetype) {
            case int_n: return sum_int;
            default: return traverse_children;
        }
    }

    info info = { 0 };

    node *result = traverse_do(select_fun, tree, &info);
    logging_log(NOTE, "Sum of all integers: %d", info.sum);

    return result;
}
