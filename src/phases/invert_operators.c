#include <stdlib.h>
#include "traverse.h"

static node *invert_binop(node *this, info *info) {
    if (this->N_binop.Op == BO_add) {
        this->N_binop.Op = BO_sub;
    } else if (this->N_binop.Op == BO_sub) {
        this->N_binop.Op = BO_add;
    }

    return traverse_children(this, info);
}

traverse_fun_t invert_operators_select_fun(node *this) {
    switch (this->nodetype) {
        case N_binop: return invert_binop;
        default: return traverse_children;
    }
}

node *invert_operators_init(node *tree) {
    return traverse_do(invert_operators_select_fun, tree, NULL);
}
