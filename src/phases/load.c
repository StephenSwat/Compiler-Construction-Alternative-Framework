#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "logging.h"

FILE *yyin;

node *load_file_init(node *syntax_tree) {
    char cppcallstr[128];

    if (access(global.infile, F_OK) == -1)
        logging_log(ABORT, "Could not open file %s!", global.infile);

    sprintf(cppcallstr, "gcc -o - -x c -E %s", global.infile);
    yyin = popen(cppcallstr, "r");

    return NULL;
}
