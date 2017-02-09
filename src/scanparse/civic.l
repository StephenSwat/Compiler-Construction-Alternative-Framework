%{
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include "types.h"
#include "alloc.h"
#include "civic.parser.h"
#include "logging.h"

#define FILTER(token) global.col += yyleng; return(token);
#define NEWLINE() global.line += 1; global.col = 1; yyless(1);
#define YY_NO_UNPUT
#define YY_NO_INPUT
%}

%x PREPROCESSOR
%x IGNORELINE
%option nounput

%%

\n                         { NEWLINE(); }
"#"                        { BEGIN(PREPROCESSOR); }

<PREPROCESSOR>{
    [0-9]+                 { global.line = atoi(yytext) - 1; }
    \"[^\"]+\"             { free(global.currentfile);
                             global.currentfile = malloc(strlen(yytext));
                             memcpy(global.currentfile, yytext + 1, strlen(yytext) - 2);
                             global.currentfile[strlen(yytext) - 2] = '\0';
                             BEGIN(IGNORELINE);
                           }
    [ \t]                  { }
    .                      { BEGIN(IGNORELINE); }
    \n                     { NEWLINE(); BEGIN(INITIAL); }
}

<IGNORELINE>{
    .                      { }
    \n                     { NEWLINE(); BEGIN(INITIAL); }
}


"("                        { FILTER('('); }
")"                        { FILTER(')'); }
","                        { FILTER(','); }
";"                        { FILTER(';'); }

"-"                        { FILTER('-'); }
"+"                        { FILTER('+');  }
"*"                        { FILTER('*');  }
"/"                        { FILTER('/');  }
"%"                        { FILTER('%');  }

"<="                       { FILTER(LE);    }
"<"                        { FILTER(LT);    }
">="                       { FILTER(GE);    }
">"                        { FILTER(GT);    }
"=="                       { FILTER(EQ);    }
"!="                       { FILTER(NE);    }
"&&"                       { FILTER(AND);    }
"||"                       { FILTER(OR);    }

"="                        { FILTER('=');    }

"true"                     { FILTER(TRUE); }
"false"                    { FILTER(FALSE); }

[A-Za-z_][A-Za-z0-9_]*     { yylval.id = strdup(yytext); FILTER(ID); }
[0-9]+                     { yylval.cint = atoi(yytext); FILTER(INT); }
[ \t]                      { global.col += yyleng; }

%%

int yywrap() {
    pclose(yyin);
    return 1;
}
