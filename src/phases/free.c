#include <stdlib.h>
#include "alloc.h"
#include "traverse.h"
#include "htable.h"

static node *free_generic(node *this, info *info) {
    traverse_children(this, info);
    free(this);
    return NULL;
}

static node *free_var(node * this, info * info) {
    free(this->N_var.Name);
    free(this);
    return NULL;
}

traverse_fun_t free_select_fun(node * this) {
    switch (this->nodetype) {
        case N_var: return free_var;
        default: return free_generic;
    }
}

node *free_tree(node * free_node) {
    return traverse_do(free_select_fun, free_node, NULL);
}
