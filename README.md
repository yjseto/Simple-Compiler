//SIMPLE COMPILER USING FLEX AND BISON //

To Run:
bison -d parse.y
flex lexer.l
gcc -o compiler compiler.c parse.tab.c lex.yy.c -lm
./compiler Examples/name_of_file.txt

