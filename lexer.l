%option noyywrap
%{
#include "compiler.h"
#include "parse.tab.h"
%}

%%

"+"	|
"-" |
"*"	|
"/"	|
"="	|
"|"	|
","	|
";"	|
":"	|
"."	|
"["	|
"]"	|
"{"	|
"}"	|
"("	|
")"	{ return yytext[0]; }

">"	{ yylval.fn = 1; return CMP; }
"<"	{ yylval.fn = 2; return CMP; }
"<>"	{ yylval.fn = 3; return CMP; }
"=="	{ yylval.fn = 4; return CMP; }
">="	{ yylval.fn = 5; return CMP; }
"<="	{ yylval.fn = 6; return CMP; }

"integer"	{ yylval.typeC = 'a'; return STD_TYPE; }
"real"		{ yylval.typeC = 'b'; return STD_TYPE; }

"function"	{ return FUNCTION; }
"var"	{ return VAR; }
"of"	{ return OF; }
"end"	{ return END; }

"if"	{ return IF; }
"else"	{ return ELSE; }
"while"	{ return WHILE; }
"do"	{ return DO; }

"sin"   { return SIN; }
"cos"   { return COS; }
"tan"   { return TAN; }


"print"	{ return PRINT; }

[a-zA-Z][a-zA-Z0-9]*	{ yylval.symbol = lookUp(yytext); return SYMBOL; }

[0-9]+"."[0-9]+	|
[0-9]+	{ yylval.d = atof(yytext); return NUMBER; }

"//".*
[ \t\n]
%%