#include <stdio.h>
#include "types.h"
#include "traverse.h"
#include "main.h"

struct info {
    node *parent;
};

static node *output_dot_generic(node *this, info * info) {
    switch (this->nodetype) {
        case N_statements:
            fprintf(outfile, "%lu [label=%s];\n", (long) this, nodetype_string(this->nodetype));
            if (info->parent && info->parent->nodetype == this->nodetype)
                fprintf(outfile, "{rank = same; %lu; %lu;}\n", (long) info->parent, (long) this);
            break;
        case N_var:
            fprintf(outfile, "%lu [label=\"var (%s)\"];\n", (long) this, this->N_var.Name);
            break;
        case N_binop:
            fprintf(outfile, "%lu [label=\"binop (%s)\"];\n", (long) this, binop_string(this->N_binop.Op));
            break;
        case N_int:
            fprintf(outfile, "%lu [label=\"int (%d)\"];\n", (long) this, this->N_int.Value);
            break;
        case N_float:
            fprintf(outfile, "%lu [label=\"float (%f)\"];\n", (long) this, this->N_float.Value);
            break;
        case N_bool:
            fprintf(outfile, "%lu [label=\"bool (%s)\"];\n", (long) this, this->N_bool.Value ? "true" : "false");
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
