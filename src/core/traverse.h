#pragma once
#include "types.h"
#include <stdbool.h>

extern compiler_phase_t compiler_phase;
extern compiler_phase_t break_phase;
extern bool changed;

node *traverse_phase(compiler_phase_t, traverse_init_t, const char *, node *, bool);
node *traverse_none(node *, info *);
node *traverse_children(node *, info *);
node *traverse_init(node *, info *);
node *traverse_do(traverse_choice_t, node *, info *);

bool traverse_always(void);
bool traverse_never(void);
