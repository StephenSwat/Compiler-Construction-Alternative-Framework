#include <stdio.h>
#include "types.h"
#include "traverse.h"
#include "main.h"
#include "node.gen.h"

struct info {
    node *parent;
};

static node *output_dot_generic(node *this, info * info) {
    switch (this->nodetype) {
        case statements_n:
            fprintf(outfile, "%lu [label=%s];\n", (long) this, nodetype_string(this->nodetype));
            if (info->parent && info->parent->nodetype == this->nodetype)
                fprintf(outfile, "{rank = same; %lu; %lu;}\n", (long) info->parent, (long) this);
            break;
        case var_n:
            fprintf(outfile, "%lu [label=\"var (%s)\"];\n", (long) this, this->var_n.name);
            break;
        case binop_n:
            fprintf(outfile, "%lu [label=\"binop (%s)\"];\n", (long) this, binop_string(this->binop_n.op));
            break;
        case int_n:
            fprintf(outfile, "%lu [label=\"int (%d)\"];\n", (long) this, this->int_n.value);
            break;
        case float_n:
            fprintf(outfile, "%lu [label=\"float (%f)\"];\n", (long) this, this->float_n.value);
            break;
        case bool_n:
            fprintf(outfile, "%lu [label=\"bool (%s)\"];\n", (long) this, this->bool_n.value ? "true" : "false");
            break;
        default:
            fprintf(outfile, "%lu [label=%s];\n", (long) this, nodetype_string(this->nodetype));
            break;
    }

    if (info->parent) {
        fprintf(outfile, "%lu -> %lu;\n", (long) info->parent, (long) this);
    }

    node *old_parent = info->parent;
    info->parent = this;
    traverse_children(this, info);
    info->parent = old_parent;

    return this;
}

node *output_dot_init(node * syntaxtree) {
    traverse_fun_t select_fun(node * this) {
        return output_dot_generic;
    }

    info info = { NULL };

    fprintf(outfile, "digraph \"%s\" {\n", infile);
    fprintf(outfile, "node [shape=box]\n");
    syntaxtree = traverse_do(select_fun, syntaxtree, &info);
    fprintf(outfile, "}\n");

    return syntaxtree;
}
