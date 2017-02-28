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
#include "node.gen.h"
#include "main.h"

FILE *yyin;
static node *parseresult = NULL;
extern int yylex(void);
extern int yylex_destroy(void);
static int yyerror(char *errname);
%}

%union {
    nodetype_t nodetype;
    char *id;
    int cint;
    bool cbool;
    float cfloat;
    binop_t cbinop;
    type_t ctype_t;
    node *node;
}

%token LE LT GE GT EQ NE OR AND TRUE FALSE
%token <cint> INT
%token <cbool> BOOL
%token <cfloat> FLOAT
%token <id> ID

%type <node> program assign expression constant statement statements
%type <cbinop> binop
%type <id> var

%start program

%%

program
    : statements { parseresult = alloc_program($1); }
    ;

statements
    : statement statements { $$ = alloc_statements($1, $2); }
    | %empty { $$ = NULL; };

statement
    : assign ';'

var
    : ID
    ;

expression
    : '(' expression binop expression ')' { $$ = alloc_binop($2, $4, $3); }
    | var { $$ = alloc_var($1); }
    | constant { $$ = $1; }
    ;

binop
    : LE { $$ = BO_le; }
    | LT { $$ = BO_lt; }
    | GE { $$ = BO_ge; }
    | GT { $$ = BO_gt; }
    | EQ { $$ = BO_eq; }
    | NE { $$ = BO_ne; }
    | OR { $$ = BO_or; }
    | AND { $$ = BO_and; }
    | '+' { $$ = BO_add; }
    | '-' { $$ = BO_sub; }
    | '*' { $$ = BO_mul; }
    | '/' { $$ = BO_div; }
    | '%' { $$ = BO_mod; }
    ;

assign
    : var '=' expression { $$ = alloc_assign(alloc_var($1), $3); }
    ;

constant
    : FLOAT { $$ = alloc_float($1); }
    | INT { $$ = alloc_int($1); }
    | TRUE { $$ = alloc_bool(true); }
    | FALSE { $$ = alloc_bool(false); };

%%

static int yyerror(char *error) {
    logging_log(ABORT, "Error parsing source code: %s\n", error);
    return( 0);
}

node *load_file_init(node *syntax_tree) {
    char cppcallstr[128];

    if (access(infile, F_OK) == -1)
        logging_log(ABORT, "Could not open file %s!", infile);

    sprintf(cppcallstr, "gcc -o - -x c -E %s", infile);
    yyin = popen(cppcallstr, "r");

    return NULL;
}

node *parse_init(node *syntax_tree) {
    yyparse();
    yylex_destroy();
    return parseresult;
}
