#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <getopt.h>
#include "logging.h"
#include "phases.h"
#include "main.h"

char *infile = NULL;
FILE *outfile = NULL;
char *currentfile = NULL;

int line = 1;
int col = 1;

backend_t backend = BE_civvm;

extern bool ast_colour;

void print_usage() {
    printf("Usage: civicc [options] file\n");

    fprintf(stderr,
        "\nSPECIAL OPTIONS:\n"
        "    -h              Display this useful help text.\n");

    fprintf(stderr,
        "\nGENERAL OPTIONS:\n"
        "    <filename>      name of program file to compile.\n"
        "    -o <filename>   name of output file. Using '-' selects standard out.\n"
        "    -u              Disable syntax colouring for the printed AST.\n");

    fprintf(stderr,
        "\nBACKEND OPTIONS:\n"
        "    civicc has support for modular backends. Currently, only the Civilised C\n"
        "    virtual machine and the dot graph language are implemented. To specify a\n"
        "    backend to use, use the following option:\n\n"
        "    -z <backend>    name of program file to compile (default civvm).\n\n"
        "    To use the Civilised C virtual machine backend, specify 'civvm'. You can\n"
        "    also turn off output by using 'none'.\n\n");

    fprintf(stderr,
        "\nVERBOSITY OPTIONS:\n"
        "    civicc comes with several different levels of verbosity, ranging from -2\n"
        "    to 3. In practice, only levels 0 to 3 are used and the negative levels\n"
        "    are only internally relevant, but nothing is stopping you from selecting\n"
        "    them. To set a verbosity level, use the following flag:\n\n"
        "    -v <n>          Verbosity level (default: %d).\n\n"
        "    The verbosity levels are then as follows. Note that each verbosity level\n"
        "    also includes anything logged at any lower level.\n\n"
        "    0 : Display (critical) errors.\n"
        "    1 : Display warnings.\n"
        "    2 : Display state messages, such as the intro message and the current\n"
        "        compiler phase.\n"
        "    3 : Display notes which provide insight into non-critical optimizations\n"
        "        and other processes.\n", verbosity);

    fprintf(stderr,
        "\nBREAK OPTIONS:\n"
        "    Break options allow you to stop the compilation process after a particular\n"
        "    phase, subphase or cycle optimisation. These can be specified as follows:\n\n"
        "    -b <spec>       Break after the compilation stage given by <spec>, where\n"
        "                    <spec> is a choice from the specifiers listed below.\n");

    fprintf(stderr, "\nBREAK OPTION SPECIFIERS:\n");

    for (int i = 0; i < sizeof(phase_list) / sizeof(struct phase_t); i++)
        fprintf(stderr, "    %s : %s\n", phase_list[i].name, phase_list[i].description);
}

void check_options(int argc, char **argv) {
    int opt;
    char *outfile_str = "-";

    while ((opt = getopt(argc, argv, "hub:o:v:z:")) != -1) {
        switch (opt) {
        case 'b':
            break_phase = optarg;
            break;
        case 'v':
            verbosity = atoi(optarg);
            break;
        case 'u':
            ast_colour = false;
            break;
        case 'o':
            outfile_str = optarg;
            break;
        case 'z':
            if (!strcmp(optarg, "civvm")) {
                backend = BE_civvm;
            } else if (!strcmp(optarg, "dot")) {
                backend = BE_dot;
            } else if (!strcmp(optarg, "none")) {
                backend = BE_none;
            } else {
                logging_log(ABORT, "Backend '%s' is not implemented.", optarg);
            }
            break;
        case '?':
        case 'h':
        default:
            print_usage();
            exit(EXIT_SUCCESS);
        }
    }

    outfile = strcmp(outfile_str, "-") ? fopen(outfile_str, "w") : stdout;

    for (int i = optind; i < argc; i++)
        infile = argv[i];

    if (!infile)
        logging_log(ABORT, "No input file was supplied.");
}

void internal_error(int sig) {
    logging_log(ABORT, "The compiler encountered a critical error and will now exit.");
}

int main(int argc, char *argv[]) {
    signal(SIGSEGV, internal_error);
    signal(SIGBUS, internal_error);

    check_options(argc, argv);

    if (break_phase)
        logging_log(STATE, "Compiler will exit after %s\n", break_phase);

    node *syntax_tree = NULL;

    for (int i = 0; i < sizeof(phase_list) / sizeof(struct phase_t); i++)
        syntax_tree = traverse_phase(phase_list[i], syntax_tree);

    free(currentfile);
    logging_quit(true);
}
