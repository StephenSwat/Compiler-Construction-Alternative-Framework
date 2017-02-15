#include <stdlib.h>
#include "traverse.h"
#include "alloc.h"

static traverse_choice_t travstack;

node *traverse_none(node * this, info * info) {
    return this;
}

node *traverse_children(node * this, info * info) {
    switch (this->nodetype) {
        case N_program:
            this->N_program.Statements = traverse_init(this->N_program.Statements, info);
            break;
        case N_statements:
            this->N_statements.Statement = traverse_init(this->N_statements.Statement, info);
            this->N_statements.Next = traverse_init(this->N_statements.Next, info);
            break;
        case N_assign:
            this->N_assign.Var = traverse_init(this->N_assign.Var, info);
            this->N_assign.Expression = traverse_init(this->N_assign.Expression, info);
            break;
        case N_binop:
            this->N_binop.Left = traverse_init(this->N_binop.Left, info);
            this->N_binop.Right = traverse_init(this->N_binop.Right, info);
            break;
        default:
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
