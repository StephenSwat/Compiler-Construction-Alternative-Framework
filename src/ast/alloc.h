#pragma once
#include "types.h"
#include <stdbool.h>

node *alloc_program(node * Declarations);
node *alloc_statements(node * statement, node * next);

node *alloc_var(char *name, node * next);
node *alloc_assign(node * var, node * expression);

node *alloc_binop(binop_t op, node * left, node * right);

node *alloc_int(int value);
node *alloc_bool(bool value);
node *alloc_float(float value);
