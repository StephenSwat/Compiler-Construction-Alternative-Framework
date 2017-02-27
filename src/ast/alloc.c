#include <stdlib.h>
#include <string.h>
#include "alloc.h"
#include "logging.h"
#include "main.h"

static node *alloc_node(nodetype_t type) {
    node *result = (node *) malloc(sizeof(node));
    result->nodetype = type;
    result->lineno = line;
    result->colno = col;
    return result;
}

node *alloc_program(node * statements) {
    node *this = alloc_node(program_n);
    this->program_n.statements = statements;
    return this;
}

node *alloc_statements(node * statement, node * next) {
    // You could add some typechecking here
    node *this = alloc_node(statements_n);
    this->statements_n.statement = statement;
    this->statements_n.next = next;
    return this;
}

node *alloc_assign(node * var, node * expression) {
    if (!expression) return NULL;
    node *this = alloc_node(assign_n);
    this->assign_n.var = var;
    this->assign_n.expression = expression;
    return this;
}

node *alloc_binop(binop_t op, node * left, node * right) {
    node *this = alloc_node(binop_n);
    this->binop_n.left = left;
    this->binop_n.right = right;
    this->binop_n.op = op;
    return this;
}

node *alloc_var(char *name, node * next) {
    node *this = alloc_node(var_n);
    this->var_n.name = name;
    return this;
}

node *alloc_int(int value) {
    node *this = alloc_node(int_n);
    this->int_n.value = value;
    return this;
}

node *alloc_float(float value) {
    node *this = alloc_node(float_n);
    this->float_n.value = value;
    return this;
}

node *alloc_bool(bool value) {
    node *this = alloc_node(bool_n);
    this->bool_n.value = value;
    return this;
}
