%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

void yyerror(const char *s);
int yylex();
extern int yylineno;

ASTNode* root;
%}

%union {
    int intval;
    char* str;
    struct ASTNode* node;
}

/* precedence and associativity (lowest to highest) */
%nonassoc ELSE
%nonassoc IF
%left PLUS MINUS
%left MUL DIV
%nonassoc UMINUS
%left LT GT EQ NE

/* tokens */
%token INT BOOL IF ELSE WHILE PRINT
%token <intval> NUMBER
%token <str> ID

%token PLUS MINUS MUL DIV
%token EQ NE GT LT
%token ASSIGN SEMI
%token LP RP LB RB

/* types */
%type <node> program statements statement expr

%%

/* ---------------- PROGRAM ---------------- */

program:
    statements
    {
        root = $1;
    }
;

/* multiple statements */
statements:
    statements statement
    {
        $$ = createNode("block", $1, $2);
    }
  | statement
    {
        $$ = $1;
    }
;

/* ---------------- STATEMENTS ---------------- */

statement:
    INT ID SEMI
    {
        $$ = createNode("decl",
                        createLeaf($2),
                        createLeaf("int"));
    }

  | BOOL ID SEMI
    {
        $$ = createNode("decl",
                        createLeaf($2),
                        createLeaf("bool"));
    }

  | ID ASSIGN expr SEMI
    {
        $$ = createNode("assign",
                        createLeaf($1),
                        $3);
    }

  | PRINT LP expr RP SEMI
    {
        $$ = createNode("print",
                        $3,
                        NULL);
    }

  | IF LP expr RP statement
    {
        $$ = createNode("if", $3, $5);
    }

  | IF LP expr RP statement ELSE statement %prec ELSE
    {
        ASTNode* temp = createNode("ifelse", $3,
                        createNode("block", $5, $7));
        $$ = temp;
    }

  | WHILE LP expr RP statement
    {
        $$ = createNode("while", $3, $5);
    }

  | LB statements RB
    {
        $$ = createNode("scope", $2, NULL);
    }
;

/* ---------------- EXPRESSIONS ---------------- */

expr:
    expr PLUS expr
    {
        $$ = createNode("add", $1, $3);
    }
  | expr MINUS expr
    {
        $$ = createNode("sub", $1, $3);
    }
  | expr MUL expr
    {
        $$ = createNode("mul", $1, $3);
    }
  | expr DIV expr
    {
        $$ = createNode("div", $1, $3);
    }
  | expr GT expr
    {
        $$ = createNode("gt", $1, $3);
    }
  | expr LT expr
    {
        $$ = createNode("lt", $1, $3);
    }
  | expr EQ expr
    {
        $$ = createNode("eq", $1, $3);
    }
  | expr NE expr
    {
        $$ = createNode("ne", $1, $3);
    }
  | LP expr RP
    {
        $$ = $2;
    }
  | MINUS expr %prec UMINUS
    {
        $$ = createNode("neg", $2, NULL);
    }
  | NUMBER
    {
        $$ = createLeafInt($1);
    }
  | ID
    {
        $$ = createLeaf($1);
    }
;

%%

void yyerror(const char *s)
{
    printf("SYNTAX ERROR at line %d: %s\n", yylineno, s);
}