#include <stdio.h>
#include "types.h"
#include "traverse.h"

struct info {
    node *parent;
};

static node *output_dot_generic(node * this, info * info) {
    fprintf(global.outfile, "%lu [label=%s];\n", (long) this, nodetype_string(this->nodetype));

    if (info->parent) {
        fprintf(global.outfile, "%lu -> %lu;\n", (long) info->parent, (long) this);

        if (info->parent->nodetype == this->nodetype && this->nodetype == N_statements) {
            fprintf(global.outfile, "{rank = same; %lu; %lu;}\n", (long) info->parent, (long) this);
        }
    }

    node *old_parent = info->parent;
    info->parent = this;
    traverse_children(this, info);
    info->parent = old_parent;

    return this;
}

static traverse_fun_t output_dot_select_fun(node * this) {
    switch (this->nodetype) {
        default: return output_dot_generic;
    }
}

node *output_dot_init(node * syntaxtree) {
    info info = { NULL };

    fprintf(global.outfile, "digraph \"%s\" {\n", global.infile);
    fprintf(global.outfile, "node [shape=box]\n");
    traverse_do(output_dot_select_fun, syntaxtree, &info);
    fprintf(global.outfile, "}\n");

    return syntaxtree;
}
