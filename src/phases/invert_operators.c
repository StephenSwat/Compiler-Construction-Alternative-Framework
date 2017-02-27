#include <stdlib.h>
#include "traverse.h"
#include "node.gen.h"

static node *invert_binop(node *this, info *info) {
    if (this->binop_n.op == BO_add) this->binop_n.op = BO_sub;
    else if (this->binop_n.op == BO_sub) this->binop_n.op = BO_add;
    return traverse_children(this, info);
}

node *invert_operators_init(node *tree) {
    traverse_fun_t select_fun(node *this) {
        switch (this->nodetype) {
            case binop_n: return invert_binop;
            default: return traverse_children;
        }
    }

    return traverse_do(select_fun, tree, NULL);
}
