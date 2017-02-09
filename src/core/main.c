#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <getopt.h>
#include "logging.h"

node *phase_run();

void print_usage() {
    printf("Usage: civicc [options] file\n");

    fprintf(stderr,
        "\nSPECIAL OPTIONS:\n"
        "    -h              Display this useful help text.\n");

    fprintf(stderr,
        "\nGENERAL OPTIONS:\n"
        "    <filename>      Name of program file to compile.\n"
        "    -o <filename>   Name of output file. Using '-' selects standard out.\n"
        "    -u              Disable syntax colouring for the printed AST.\n");

    fprintf(stderr,
        "\nBACKEND OPTIONS:\n"
        "    civicc has support for modular backends. Currently, only the Civilised C\n"
        "    virtual machine and the dot graph language are implemented. To specify a\n"
        "    backend to use, use the following option:\n\n"
        "    -z <backend>    Name of program file to compile (default civvm).\n\n"
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
        "        and other processes.\n", global.verbosity);

    fprintf(stderr,
        "\nBREAK OPTIONS:\n"
        "    Break options allow you to stop the compilation process after a particular\n"
        "    phase, subphase or cycle optimisation. These can be specified as follows:\n\n"
        "    -b <spec>       Break after the compilation stage given by <spec>, where\n"
        "                    <spec> is a choice from the specifiers listed below.\n");

    fprintf(stderr,
        "\nBREAK OPTION SPECIFIERS:\n"
        "    cpp : Running C preprocessor\n"
        "    prs : Parsing CiviC program\n"
        "    ast : Print AST\n"
        "    sin : Summing integers (dummy phase)\n"
        "    inv : Inverting addition and subtraction (dummy phase)\n"
        "    ast : Print AST\n"
        "    gen : Generating bytecode\n"
        "    frt : De-allocating syntax tree representation\n");
}

void globals_init() {
    global.compiler_phase = NULL;
    global.ast_colour = true;

    global.verbosity = 2;
    global.errors = 0;
    global.warnings = 0;

    global.break_phase = NULL;

    global.infile = NULL;
    global.outfile = NULL;
    global.currentfile = NULL;

    global.line = 1;
    global.col = 1;

    global.backend = BE_civvm;
}


void check_options(int argc, char **argv) {
    int opt;
    char *outfile = "-";

    while ((opt = getopt(argc, argv, "hub:o:v:z:")) != -1) {
        switch (opt) {
        case 'b':
            global.break_phase = optarg;
            break;
        case 'v':
            global.verbosity = atoi(optarg);
            break;
        case 'u':
            global.ast_colour = false;
            break;
        case 'o':
            outfile = optarg;
            break;
        case 'z':
            if (!strcmp(optarg, "civvm")) {
                global.backend = BE_civvm;
            } else if (!strcmp(optarg, "none")) {
                global.backend = BE_none;
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

    global.outfile = strcmp(outfile, "-") ? fopen(outfile, "w") : stdout;

    for (int i = optind; i < argc; i++)
        global.infile = argv[i];

    if (!global.infile) {
        logging_log(ABORT, "No input file was supplied.");
    }
}

void internal_error(int sig) {
    logging_log(ABORT, "The compiler encountered a critical error and will now exit.");
}

int main(int argc, char *argv[]) {
    signal(SIGSEGV, internal_error);
    signal(SIGBUS, internal_error);

    globals_init();
    check_options(argc, argv);

    if (global.break_phase)
        logging_log(STATE, "Compiler will exit after %s:%s\n", global.break_phase, global.break_subphase);

    phase_run();

    free(global.currentfile);
    logging_quit(true);
}
