#include <string.h>
#include <stdio.h>
#include "types.h"

node *output_civvm_init(node *, FILE *);

node *output_init(node *tree) {
    FILE *output = strcmp(global.outfile, "-") ? fopen(global.outfile, "w") : stdout;

    switch (global.backend) {
        case BE_civvm:
            output_civvm_init(tree, output);
            break;
    }

    if (output != stdout) fclose(output);

    return tree;
}
