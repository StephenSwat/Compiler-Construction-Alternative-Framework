#pragma once
#include "types.h"
#include <stdbool.h>

node *traverse_none(node *, info *);
node *traverse_sons(node *, info *);
node *traverse_init(node *, info *);
node *traverse_do(traverse_choice_t, node *, info *);
