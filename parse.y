%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "compiler.h"
%}

%union {
	struct ast *a;
	double d;
	struct symbol *symbol;
	struct symbolList *symbolList;
	int fn;
	char typeC;
}

%token <d> NUMBER
%token <symbol> SYMBOL
%token FUNCTION VAR OF INTEGER REAL END IF THEN ELSE WHILE DO DOTS PRINT SIN COS TAN
%token <typeC> STD_TYPE

%nonassoc <fn> CMP
%right '='
%left '+' '-'
%left '*' '/'
%nonassoc '|' UMINUS

%type <a> declarationList declaration statementList statement expression
%type <symbolList> symbolList

%start function
%%

function: FUNCTION SYMBOL '(' symbolList ')' ';' declarationList statementList';' END ';'
	{ eval($7); eval($8); treeFree($8); treeFree($7); printf("parse done.\n"); }
	;

declarationList:	{ $$ = NULL; }
	| declaration ';' declarationList	{ if($3 == NULL) $$ = $1; else $$ = newAst('L', $1, $3); }
	;

declaration: VAR symbolList ':' STD_TYPE	{ $$ = newDeclaration($2, $4); }
	;

statement: IF expression '{' statementList '}'		{ $$ = newFlow('I', $2, $4, NULL); }
	| IF expression '{' statementList '}' ELSE '{' statementList '}'	{ $$ = newFlow('I', $2, $4, $8); }
	| WHILE expression DO '{' statementList '}'	{ $$ = newFlow('W', $2, $5, NULL); }
	| expression
	;

statementList: statement		{ $$ = $1; }
	| statementList ';' statement { $$ = newAst('L', $1, $3); }
	;

expression: expression CMP expression	{ $$ = newCompare($2, $1, $3); }
	| expression '+' expression	{ $$ = newAst('+', $1, $3); }
	| expression '-' expression	{ $$ = newAst('-', $1, $3); }
	| expression '*' expression	{ $$ = newAst('*', $1, $3); }
	| expression '/' expression 	{ $$ = newAst('/', $1, $3); }
	| '|' expression	{ $$ = newAst('|', $2, NULL); }
	| '(' expression ')'	{ $$ = $2; }
	| '-' expression %prec UMINUS	{ $$ = newAst('M', $2, NULL); }
	| NUMBER	{ $$ = newNum($1); }
	| SYMBOL		{ $$ = newReference($1); }
	| SYMBOL '=' expression	{ $$ = newAssign($1, $3); }
	| PRINT '(' expression ')'	{ $$ = newPrint($3); }

	| SIN '(' expression ')' { $$ = newAst('S', $3, NULL); }
	| COS '(' expression ')' { $$ = newAst('C', $3, NULL); }
	| TAN '(' expression ')' { $$ = newAst('T', $3, NULL); }
	;


symbolList: SYMBOL	{ $$ = newSymbolList($1, NULL); }
	| SYMBOL ',' symbolList	{ $$ = newSymbolList($1, $3); }
	;
%%
