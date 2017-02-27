#include <stdlib.h>
#include "traverse.h"
#include "main.h"
#include "node.gen.h"

static traverse_choice_t travstack;

node *traverse_none(node * this, info * info) {
    return this;
}

node *traverse_init(node * this, info * info) {
    if (this) {
        line = this->lineno;
        col = this->colno;
        return travstack(this)(this, info);
    } else {
        return NULL;
    }
}

node *traverse_do(traverse_choice_t fun, node * this, info * info) {
    travstack = fun;
    return traverse_init(this, info);
}
