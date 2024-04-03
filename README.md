//SIMPLE COMPILER USING FLEX AND BISON //

To Run:
bison -d grammar.y
flex lexer.l
gcc -o compiler compiler.c grammar.tab.c lex.yy.c -lm
./compiler example.txt