#include <stdio.h>
#include "types.h"
#include "traverse.h"

static traverse_fun_t output_civvm_select_fun(node * this) {
    switch (this->nodetype) {
        default: return traverse_sons;
    }
}

node *output_civvm_init(node * syntaxtree) {
    syntaxtree = traverse_do(output_civvm_select_fun, syntaxtree, NULL);
    return syntaxtree;
}
