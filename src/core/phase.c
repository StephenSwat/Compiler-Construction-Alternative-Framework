#include <string.h>
#include "logging.h"

#define quit_phase (global.break_phase && !strcmp(global.break_phase, global.compiler_phase))
#define quit_subphase (global.break_subphase && !strcmp(global.break_subphase, global.compiler_subphase))

node *free_tree(node * arg_node);
node *count_operators_init(node *);
node *load_file_init(node *);
node *parse_init(node *);
node *output_civvm_init(node *);
node *print_init(node *);
node *check_init(node *);

node *phase_body(node * (fun) (node *), const char *name, node * syntax_tree, bool cond) {
    if (cond) {
        logging_log(STATE, "%s ...", name);
        logging_indent(STATE);

        do {
            global.changed = false;
            syntax_tree = fun(syntax_tree);
            if (global.errors > 0) logging_quit(false);
            if (global.treecheck && syntax_tree) check_init(syntax_tree);
            if (global.errors > 0) logging_quit(false);
        } while (global.changed);

        logging_unindent(STATE);
    }

    return syntax_tree;
}

node *phase_phase(compiler_phase_t phase, node * (fun) (node *), const char *name, node * syntax_tree, bool cond) {
    global.compiler_phase = phase;
    syntax_tree = phase_body(fun, name, syntax_tree, cond);
    logging_log(STATE, "");

    if (quit_phase) {
        print_init(syntax_tree);
        logging_quit(true);
    }

    return syntax_tree;
}

node *phase_subphase(compiler_phase_t subphase, node * (fun) (node *), const char *name, node * syntax_tree, bool cond) {
    global.compiler_subphase = subphase;
    syntax_tree = phase_body(fun, name, syntax_tree, cond);

    if (quit_phase && quit_subphase) {
        print_init(syntax_tree);
        logging_quit(true);
    }

    return syntax_tree;
}

node *phase_run_ld(node * syntax_tree) {
    syntax_tree = phase_subphase("cpp", load_file_init, "Running C preprocessor", syntax_tree, true);
    syntax_tree = phase_subphase("prs", parse_init, "Parsing CiviC program", syntax_tree, true);
    syntax_tree = phase_subphase("ast", print_init, "Print AST before optimizations", syntax_tree, global.verbosity >= 3);

    return syntax_tree;
}

node *phase_run_met(node * syntax_tree) {
    syntax_tree = phase_subphase("cop", count_operators_init, "Count operators", syntax_tree, true);

    return syntax_tree;
}

node *phase_run_cg(node * syntax_tree) {
    syntax_tree = phase_subphase("gen", output_civvm_init, "Generating civvm output", syntax_tree, global.backend == BE_civvm);
    syntax_tree = phase_subphase("frt", free_tree, "De-allocating syntax tree representation", syntax_tree, true);

    return syntax_tree;
}

node *phase_run() {
    node *syntax_tree = NULL;
    syntax_tree = phase_phase("ld", phase_run_ld, "Loading CiviC program", syntax_tree, true);
    syntax_tree = phase_phase("met", phase_run_met, "Reporting metrics", syntax_tree, true);
    syntax_tree = phase_phase("cg", phase_run_cg, "Generating Code", syntax_tree, true);

    return syntax_tree;
}
