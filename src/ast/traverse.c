#include <stdlib.h>
#include "traverse.h"
#include "alloc.h"
#include "main.h"

static traverse_choice_t travstack;

node *traverse_none(node * this, info * info) {
    return this;
}

node *traverse_children(node * this, info * info) {
    switch (this->nodetype) {
        case program_n:
            this->program_n.statements = traverse_init(this->program_n.statements, info);
            break;
        case statements_n:
            this->statements_n.statement = traverse_init(this->statements_n.statement, info);
            this->statements_n.next = traverse_init(this->statements_n.next, info);
            break;
        case assign_n:
            this->assign_n.var = traverse_init(this->assign_n.var, info);
            this->assign_n.expression = traverse_init(this->assign_n.expression, info);
            break;
        case binop_n:
            this->binop_n.left = traverse_init(this->binop_n.left, info);
            this->binop_n.right = traverse_init(this->binop_n.right, info);
            break;
        default:
            break;
    }

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
