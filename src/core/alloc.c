#include <stdlib.h>
#include <string.h>
#include "alloc.h"
#include "logging.h"

static node *alloc_node(nodetype type) {
    node *result = (node *) malloc(sizeof(node));
    result->nodetype = type;
    result->lineno = global.line;
    result->colno = global.col;
    return result;
}

node *alloc_program(node * Declarations) {
    node *this = alloc_node(N_program);
    PROGRAM_DECLARATIONS(this) = Declarations;
    return this;
}

node *alloc_declarations(node * Declaration, node * Next) {
    node *this = alloc_node(N_declarations);
    DECLARATIONS_DECLARATION(this) = Declaration;
    DECLARATIONS_NEXT(this) = Next;
    return this;
}

node *alloc_statements(node * Statement, node * Next) {
    node *this = alloc_node(N_statements);
    STATEMENTS_STATEMENT(this) = Statement;
    STATEMENTS_NEXT(this) = Next;
    return this;
}

node *alloc_assign(node * Var, node * Expression) {
    if (!Expression) return NULL;
    node *this = alloc_node(N_assign);
    ASSIGN_VAR(this) = Var;
    ASSIGN_EXPRESSION(this) = Expression;
    return this;
}

node *alloc_binop(binop Op, node * Left, node * Right) {
    node *this = alloc_node(N_binop);
    BINOP_LEFT(this) = Left;
    BINOP_RIGHT(this) = Right;
    BINOP_OP(this) = Op;
    return this;
}

node *alloc_vardec(typet Type, node * Var) {
    node *this = alloc_node(N_vardec);
    VARDEC_VAR(this) = Var;
    VARDEC_TYPE(this) = Type;
    return this;
}

node *alloc_var(char *Name, node * Index, node * Next) {
    node *this = alloc_node(N_var);
    VAR_NAME(this) = Name;
    return this;
}

node *alloc_int(int Value) {
    node *this = alloc_node(N_int);
    INT_VALUE(this) = Value;
    return this;
}

node *alloc_float(float Value) {
    node *this = alloc_node(N_float);
    FLOAT_VALUE(this) = Value;
    return this;
}

node *alloc_bool(bool Value) {
    node *this = alloc_node(N_bool);
    BOOL_VALUE(this) = Value;
    return this;
}
