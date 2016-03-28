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
            traverse_opt(this->N_program.Declarations, info);
            break;
        case N_declarations:
            traverse_opt(this->N_declarations.Declaration, info);
            traverse_opt(this->N_declarations.Next, info);
            break;
        case N_statements:
            traverse_opt(this->N_statements.Statement, info);
            traverse_opt(this->N_statements.Next, info);
            break;
        case N_assign:
            traverse_opt(this->N_assign.Var, info);
            traverse_opt(this->N_assign.Expression, info);
            break;
        case N_binop:
            traverse_opt(this->N_binop.Left, info);
            traverse_opt(this->N_binop.Right, info);
            break;
        case N_vardec:
            traverse_opt(this->N_vardec.Var, info);
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
    global.line = this->lineno;
    global.col = this->colno;
    return travstack(this)(this, info);
}

node *traverse_opt(node * this, info * info) {
    return this ? traverse_init(this, info) : this;
}

node *traverse_do(traverse_choice_t fun, node * this, info * info) {
    traverse_choice_t old = travstack;
    travstack = fun;
    node *rv = traverse_init(this, info);
    travstack = old;
    return rv;
}
