#pragma once
#include "types.h"
#include "traverse.h"

extern node *free_tree(node *);
extern node *sum_ints_init(node *);
extern node *invert_operators_init(node *);
extern node *load_file_init(node *);
extern node *parse_init(node *);
extern node *output_civvm_init(node *);
extern node *output_dot_init(node *);
extern node *print_init(node *);

extern bool print_enabled(void);
extern bool output_civvm_enabled(void);
extern bool output_dot_enabled(void);

static phase_t phase_list[] = {
    {"cpp", load_file_init, "Running C preprocessor", traverse_always},
    {"prs", parse_init, "Parsing CiviC program", traverse_always},
    {"ast", print_init, "Print AST before optimizations", print_enabled},
    {"sin", sum_ints_init, "Summing integers", traverse_always},
    {"inv", invert_operators_init, "Inverting addition and subtraction", traverse_always},
    {"ast", print_init, "Print AST after optimizations", print_enabled},
    {"gen", output_civvm_init, "Generating civvm output", output_civvm_enabled},
    {"dot", output_dot_init, "Generating dot output", output_dot_enabled},
    {"frt", free_tree, "De-allocating syntax tree representation", traverse_always},
};
