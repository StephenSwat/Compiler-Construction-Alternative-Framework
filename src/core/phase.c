#include <string.h>
#include "logging.h"

node *free_tree(node * this);
node *sum_ints_init(node *);
node *invert_operators_init(node *);
node *load_file_init(node *);
node *parse_init(node *);
node *output_civvm_init(node *);
node *output_dot_init(node *);
node *print_init(node *);

node *phase_phase(compiler_phase_t phase, node * (fun) (node *), const char *name, node * syntax_tree, bool cond) {
    global.compiler_phase = phase;

    if (cond) {
        logging_log(STATE, "%s ...", name);
        logging_indent(STATE);

        do {
            global.changed = false;
            syntax_tree = fun(syntax_tree);
            if (global.errors > 0) logging_quit(false);
        } while (global.changed);

        logging_unindent(STATE);
    }

    if (global.break_phase && !strcmp(global.break_phase, global.compiler_phase)) {
        print_init(syntax_tree);
        logging_quit(true);
    }

    return syntax_tree;
}

node *phase_run() {
    node *syntax_tree = NULL;

    syntax_tree = phase_phase("cpp", load_file_init, "Running C preprocessor", syntax_tree, true);
    syntax_tree = phase_phase("prs", parse_init, "Parsing CiviC program", syntax_tree, true);
    syntax_tree = phase_phase("ast", print_init, "Print AST before optimizations", syntax_tree, global.verbosity >= 3);
    syntax_tree = phase_phase("sin", sum_ints_init, "Summing integers", syntax_tree, true);
    syntax_tree = phase_phase("inv", invert_operators_init, "Inverting addition and subtraction", syntax_tree, true);
    syntax_tree = phase_phase("ast", print_init, "Print AST after optimizations", syntax_tree, global.verbosity >= 3);
    syntax_tree = phase_phase("gen", output_civvm_init, "Generating civvm output", syntax_tree, global.backend == BE_civvm);
    syntax_tree = phase_phase("dot", output_dot_init, "Generating dot output", syntax_tree, global.backend == BE_dot);
    syntax_tree = phase_phase("frt", free_tree, "De-allocating syntax tree representation", syntax_tree, true);

    return syntax_tree;
}
