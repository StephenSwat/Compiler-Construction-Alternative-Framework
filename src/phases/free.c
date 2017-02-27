#include <stdlib.h>
#include "traverse.h"
#include "htable.h"
#include "node.gen.h"

static node *free_generic(node *this, info *info) {
    traverse_children(this, info);
    free(this);
    return NULL;
}

static node *free_var(node * this, info * info) {
    free(this->var_n.name);
    free(this);
    return NULL;
}

node *free_tree(node * free_node) {
    traverse_fun_t select_fun(node * this) {
        switch (this->nodetype) {
            case var_n: return free_var;
            default: return free_generic;
        }
    }

    return traverse_do(select_fun, free_node, NULL);
}
