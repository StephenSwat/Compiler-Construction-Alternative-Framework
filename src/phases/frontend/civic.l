%{
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include "types.h"
#include "node.gen.h"
#include "civic.parser.h"
#include "logging.h"
#include "main.h"

#define FILTER(token) col += yyleng; return(token);
#define NEWLINE() line += 1; col = 1; yyless(1);
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
    [0-9]+                 { line = atoi(yytext) - 1; }
    \"[^\"]+\"             { free(currentfile);
                             currentfile = malloc(strlen(yytext));
                             memcpy(currentfile, yytext + 1, strlen(yytext) - 2);
                             currentfile[strlen(yytext) - 2] = '\0';
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
[ \t]                      { col += yyleng; }

%%

int yywrap() {
    pclose(yyin);
    return 1;
}
