#include <stdio.h>
#include "types.h"
#include "traverse.h"

node *output_civvm_init(node * syntaxtree) {
    traverse_fun_t select_fun(node * this) {
        return traverse_children;
    }

    return traverse_do(select_fun, syntaxtree, NULL);
}
