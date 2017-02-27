#pragma once
#include "types.h"
#include <stdbool.h>
#include <stdarg.h>

typedef enum {
    META = -2,
    ABORT = -1,
    ERROR = 0,
    WARNING = 1,
    STATE = 2,
    NOTE = 3
} logging_t;

extern int verbosity;
extern int errors;

void logging_quit(bool);
void logging_indent(logging_t);
void logging_unindent(logging_t);
void logging_print_indent(void);
void logging_log(logging_t, const char *, ...);
