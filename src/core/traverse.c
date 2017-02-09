#include <stdlib.h>
#include "traverse.h"
#include "alloc.h"

static traverse_choice_t travstack;

node *traverse_none(node * this, info * info) {
    return this;
}

node *traverse_sons(node * this, info * info) {
    switch (this->nodetype) {
        case N_program:
            traverse_init(this->N_program.Statements, info);
            break;
        case N_statements:
            traverse_init(this->N_statements.Statement, info);
            traverse_init(this->N_statements.Next, info);
            break;
        case N_assign:
            traverse_init(this->N_assign.Var, info);
            traverse_init(this->N_assign.Expression, info);
            break;
        case N_binop:
            traverse_init(this->N_binop.Left, info);
            traverse_init(this->N_binop.Right, info);
            break;
        case N_var:
        case N_int:
        case N_float:
        case N_bool:
            break;
    }

    return this;
}

node *traverse_init(node * this, info * info) {
    if (this) {
        global.line = this->lineno;
        global.col = this->colno;
        return travstack(this)(this, info);
    } else {
        return NULL;
    }
}

node *traverse_do(traverse_choice_t fun, node * this, info * info) {
    travstack = fun;
    return traverse_init(this, info);
}
