#include <stdlib.h>
#include <string.h>
#include "logging.h"
#include "main.h"
#include "traverse.h"
#include "node.gen.h"

extern node *print_init(node *);
static traverse_choice_t travstack;

compiler_phase_t compiler_phase = NULL;
compiler_phase_t break_phase = NULL;
bool changed;

node *traverse_phase(phase_t phase, node *syntax_tree) {
    compiler_phase = phase.name;

    if (phase.enabled()) {
        logging_log(STATE, "%s ...", phase.description);
        logging_indent(STATE);

        do {
            changed = false;
            syntax_tree = phase.init(syntax_tree);
            if (errors > 0) logging_quit(false);
        } while (changed);

        logging_unindent(STATE);
    }

    if (break_phase && !strcmp(break_phase, compiler_phase)) {
        print_init(syntax_tree);
        logging_quit(true);
    }

    return syntax_tree;
}

node *traverse_none(node * this, info * info) {
    return this;
}

node *traverse_init(node * this, info * info) {
    if (this) {
        line = this->lineno;
        col = this->colno;
        return travstack(this)(this, info);
    } else {
        return NULL;
    }
}

node *traverse_do(traverse_choice_t fun, node * this, info * info) {
    travstack = fun;
    return traverse_init(this, info);
}

bool traverse_always(void) {
    return true;
}

bool traverse_never(void) {
    return false;
}
