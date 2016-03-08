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
    logging_log(META, "Compilation %s with %d error%s, %d warning%s",
                (success ? "successful" : "failed"), global.errors,
                global.errors == 1 ? "" : "s", global.warnings,
                global.warnings == 1 ? "" : "s");

    exit(success ? 0 : 1);
}

void logging_indent(logging_t level) {
    if (global.verbosity >= level) indent++;
}

void logging_unindent(logging_t level) {
    if (global.verbosity >= level) indent--;
}

void logging_print_indent(void) {
    for (int i = 0; i < indent; i++)
        fprintf(stdout, "    ");
}

static void logging_log_helper(logging_t level, bool loc, const char *format, va_list arg_p) {
    const char *header;
    bool quit = false, indent = true;

    if (global.verbosity >= level) {
        switch (level) {
        case WARNING:
            header = BOLDYELLOW "Warning: " RESET;
            global.warnings++;
            break;
        case ERROR:
            header = BOLDRED "Error: " RESET;
            global.errors++;
            break;
        case ABORT:
            header = BOLDRED "Abort: " RESET;
            global.errors++;
            quit = true;
            break;
        case NOTE:
            header = BOLDCYAN "Note: " RESET;
            break;
        case META:
            indent = false;
        default:
            header = "";
            break;
        }

        if (indent) logging_print_indent();
        printf("%s", header);

        if (loc) {
            printf(WHITE "%s:%d:%d: " RESET, global.currentfile, global.line, global.col);
        }

        vprintf(format, arg_p);
        printf("\n");
    }

    if (quit)
        logging_quit(false);
}

void logging_log(logging_t level, const char *format, ...) {
    va_list arg_p;
    va_start(arg_p, format);
    logging_log_helper(level, false, format, arg_p);
    va_end(arg_p);
}

void logging_line(logging_t level, const char *format, ...) {
    va_list arg_p;
    va_start(arg_p, format);
    logging_log_helper(level, true, format, arg_p);
    va_end(arg_p);
}
