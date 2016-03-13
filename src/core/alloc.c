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
    this->N_program.Declarations = Declarations;
    return this;
}

node *alloc_declarations(node * Declaration, node * Next) {
    node *this = alloc_node(N_declarations);
    this->N_declarations.Declaration = Declaration;
    this->N_declarations.Next = Next;
    return this;
}

node *alloc_statements(node * Statement, node * Next) {
    node *this = alloc_node(N_statements);
    this->N_statements.Statement = Statement;
    this->N_statements.Next = Next;
    return this;
}

node *alloc_assign(node * Var, node * Expression) {
    if (!Expression) return NULL;
    node *this = alloc_node(N_assign);
    this->N_assign.Var = Var;
    this->N_assign.Expression = Expression;
    return this;
}

node *alloc_binop(binop Op, node * Left, node * Right) {
    node *this = alloc_node(N_binop);
    this->N_binop.Left = Left;
    this->N_binop.Right = Right;
    this->N_binop.Op = Op;
    return this;
}

node *alloc_vardec(typet Type, node * Var) {
    node *this = alloc_node(N_vardec);
    this->N_vardec.Var = Var;
    this->N_vardec.Type = Type;
    return this;
}

node *alloc_var(char *Name, node * Index, node * Next) {
    node *this = alloc_node(N_var);
    this->N_var.Name = Name;
    return this;
}

node *alloc_int(int Value) {
    node *this = alloc_node(N_int);
    this->N_int.Value = Value;
    return this;
}

node *alloc_float(float Value) {
    node *this = alloc_node(N_float);
    this->N_float.Value = Value;
    return this;
}

node *alloc_bool(bool Value) {
    node *this = alloc_node(N_bool);
    this->N_bool.Value = Value;
    return this;
}
