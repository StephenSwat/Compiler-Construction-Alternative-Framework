#pragma once
#include "types.h"
#include <stdbool.h>

node *alloc_program(node * Declarations);
node *alloc_statements(node * Statement, node * Next);

node *alloc_var(char *Name, node * Next);
node *alloc_assign(node * Var, node * Expression);

node *alloc_binop(binop Op, node * Left, node * Right);

node *alloc_int(int Value);
node *alloc_bool(bool Value);
node *alloc_float(float Value);
