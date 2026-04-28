%option noyywrap

%{
#include "parser.tab.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
%}

%%

"if"        { return IF; }
"else"      { return ELSE; }
"while"     { return WHILE; }
"int"       { return INT; }
"bool"      { return BOOL; }
"print"     { return PRINT; }

[0-9]+      { yylval.intval = atoi(yytext); return NUMBER; }

[a-zA-Z_][a-zA-Z0-9_]* { yylval.str = strdup(yytext); return ID; }

"=="        { return EQ; }
"!="        { return NE; }
">"         { return GT; }
"<"         { return LT; }

"+"         { return PLUS; }
"-"         { return MINUS; }
"*"         { return MUL; }
"/"         { return DIV; }

"="         { return ASSIGN; }
";"         { return SEMI; }

"("         { return LP; }
")"         { return RP; }
"{"         { return LB; }
"}"         { return RB; }

[ \t\n]+    ;

"//".*      ;

.           { printf("LEXICAL ERROR: %s\n", yytext); }

%%
