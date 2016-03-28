%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include <stdbool.h>
#include "types.h"
#include "logging.h"
#include "alloc.h"

FILE *yyin;
static node *parseresult = NULL;
extern int yylex(void);
extern int yylex_destroy(void);
static int yyerror(char *errname);
%}

%union {
    nodetype nodetype;
    char *id;
    int cint;
    bool cbool;
    float cfloat;
    binop cbinop;
    typet ctypet;
    node *node;
}

%token LE LT GE GT EQ NE OR AND TRUE FALSE
%token <cint> INT
%token <cbool> BOOL
%token <cfloat> FLOAT
%token <id> ID

%type <node> program declarations declaration

%start program

%%

program
    : declarations { parseresult = alloc_program($1); }
    ;

declarations
    : declaration declarations { $$ = alloc_declarations($1, $2); }
    | %empty { $$ = NULL; }
    ;

declaration
    : AND { $$ = NULL; } // This is clearly wrong.
    ;

%%

static int yyerror(char *error) {
    logging_log(ABORT, "Error parsing source code: %s\n", error);
    return( 0);
}

node *load_file_init(node *syntax_tree) {
    char cppcallstr[128];

    if (access(global.infile, F_OK) == -1)
        logging_log(ABORT, "Could not open file %s!", global.infile);

    sprintf(cppcallstr, "gcc -o - -x c -E %s", global.infile);
    yyin = popen(cppcallstr, "r");

    return NULL;
}

node *parse_init(node *syntax_tree) {
    yyparse();
    yylex_destroy();
    return parseresult;
}
