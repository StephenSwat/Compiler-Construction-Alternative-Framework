#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logging.h"

#define RESET       "\033[0m"
#define WHITE       "\033[37m"
#define BOLDRED     "\033[1m\033[31m"
#define BOLDYELLOW  "\033[1m\033[33m"
#define BOLDCYAN    "\033[1m\033[36m"

int indent = 0;

void logging_quit(bool success) {
    if (global.verbosity >= STATE) {
        fprintf(stderr, "Compilation %s with %d error%s, %d warning%s\n",
                (success ? "successful" : "failed"), global.errors,
                global.errors == 1 ? "" : "s", global.warnings,
                global.warnings == 1 ? "" : "s");
    }
    exit(!success);
}

void logging_indent(logging_t level) {
    if (global.verbosity >= level) indent++;
}

void logging_unindent(logging_t level) {
    if (global.verbosity >= level) indent--;
}

void logging_print_indent(void) {
    for (int i = 0; i < indent; i++)
        fprintf(stderr, "    ");
}

void logging_log(logging_t level, const char *format, ...) {
    const char *header;
    bool quit = false, indent = true, loc = false;
    va_list arg_p;
    va_start(arg_p, format);

    if (global.verbosity >= level) {
        switch (level) {
        case WARNING:
            loc = true;
            header = BOLDYELLOW "Warning: " RESET;
            global.warnings++;
            break;
        case ERROR:
            loc = true;
            header = BOLDRED "Error: " RESET;
            global.errors++;
            break;
        case ABORT:
            loc = true;
            header = BOLDRED "Abort: " RESET;
            global.errors++;
            quit = true;
            break;
        case NOTE:
            loc = true;
            header = BOLDCYAN "Note: " RESET;
            break;
        default:
            header = "";
            break;
        }

        if (indent) {
            logging_print_indent();
        }

        if (loc) {
            fprintf(stderr, "%s", header);
        }

        if (loc && global.currentfile) {
            fprintf(stderr, WHITE "%s:%d:%d: " RESET, global.currentfile, global.line, global.col);
        }

        vfprintf(stderr, format, arg_p);
        fprintf(stderr, "\n");
    }

    va_end(arg_p);

    if (quit) logging_quit(false);
}
