#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "compiler.h"

static unsigned symbolHash(char *symbol) {
	unsigned int hash = 0;

	unsigned ch;

	while(ch = *symbol++) {
		hash = hash * 9 ^ ch;

		return hash;
	}
}

struct symbol *lookUp(char *symbol) {
	struct symbol *s = &symbolTable[symbolHash(symbol) % NHASH];

	int symbolCount = NHASH;

	while(--symbolCount >= 0) {
		if(s->name && !strcmp(s->name, symbol)) {
			return s;
		}
		if(!s->name) {
			s->name = strdup(symbol);
			s->value = 0;

			return s;
		}
		if(++s >= symbolTable + NHASH) {
			s = symbolTable;
		}
	}

	yyerror("symbol table overflow occurred\n");

	abort();
}

struct ast *newAst(int nodeType, struct ast *left, struct ast *right) {
	struct ast *a = malloc(sizeof(struct ast));

	if(!a) {
		yyerror("out of space");

		exit(0);
	}

	a->nodeType = nodeType;
	a->left = left;
	a->right = right;

	return a;
}

struct ast *newNum(double number) {
	struct numVal *n = malloc(sizeof(struct numVal));

	if(!n) {
		yyerror("out of space");

		exit(0);
	}

	n->nodeType = 'K';
	n->number = number;

	return (struct ast *)n;
}

struct ast *newCompare(int compareType, struct ast *left, struct ast *right) {
	struct ast *a = malloc(sizeof(struct ast));

	if(!a) {
		yyerror("out of space");

		exit(0);
	}

	a->nodeType = '0' + compareType;
	a->left = left;
	a->right = right;

	return a;
}

struct ast *newPrint(struct ast *left) {
	struct printCall *p = malloc(sizeof(struct printCall));

	if(!p) {
		yyerror("out of space");

		exit(0);
	}

	p->nodeType = 'P';
	p->left = left;

	return (struct ast *)p;
}

struct ast *newReference(struct symbol *symbol) {
	struct symbolReference *sr = malloc(sizeof(struct symbolReference));

	if(!sr) {
		yyerror("out of space");

		exit(0);
	}

	sr->nodeType = 'N';
	sr->symbol = symbol;

	return (struct ast *)sr;
}


struct ast *newAssign(struct symbol *symbol, struct ast *v) {
	struct symbolAssign *sa = malloc(sizeof(struct symbolAssign));

	if(!sa) {
		yyerror("out of space");

		exit(0);
	}

	sa->nodeType = '=';
	sa->symbol = symbol;
	sa->v = v;

	return (struct ast *)sa;
}



struct ast *newDeclaration(struct symbolList *symbolList, char type) {
	struct declaration *d = malloc(sizeof(struct declaration));

	if(!d) {
		yyerror("out of space");
		
		exit(0);
	}

	d->nodeType ='X';
	d->symbolList = symbolList;
	d->type = type;

	return (struct ast *)d;
}


struct symbolList *newSymbolList(struct symbol *symbol, struct symbolList *next) {
	struct symbolList *sl = malloc(sizeof(struct symbolList));

    	if(!sl) {
        	yyerror("out of space");

        	exit(0);
 	}
 
    	sl->symbol = symbol;
    	sl->next = next;
     
	return sl;
}


struct ast *newFlow(int nodeType, struct ast *cond, struct ast *treeList, struct ast *evalList) {
	struct flow *f = malloc(sizeof(struct flow));

	if(!f) {
		yyerror("out of space");

		exit(0);
	}

	f->nodeType = nodeType;
	f->cond = cond;
	f->treeList = treeList;
	f->evalList = evalList;

	return (struct ast *)f;
}

void treeFree(struct ast *a) {
	switch(a->nodeType) {
		case '+':

		case '-':

		case '*':

		case '/':

		case '1': 

		case '2': 
		
		case '3': 

		case '4': 

		case '5': 
	
		case '6':

		case 'L':
			treeFree(a->right);

		case '|':

		case 'M': 

		case 'P':
			treeFree(a->left);

		case 'K': 

		case 'N': 

		case 'U': 

		case 'V': 

		case 'T':
			break;

		case '=':
			free(((struct symbolAssign *)a)->v);

			break;

		case 'I': case 'W':
			free(((struct flow *)a)->cond);

			if(((struct flow *)a)->treeList) {
				treeFree(((struct flow *)a)->treeList);
			}
			if(((struct flow *)a)->evalList) {
				treeFree(((struct flow *)a)->evalList);
			}

			break;

		case 'X':
			free(((struct declaration *)a)->symbolList);

			break;

		case 'Y':
			free(((struct declarationArr *)a)->symbolList);

			break;

		default: printf("internal error: free bad node %c\n", a->nodeType);
	}

	free(a);
}

static double callPrint(struct printCall *);
static double callDeclaration(struct declaration *);


double eval(struct ast *a) {
	double v;

	if(!a) {
		yyerror("internal error, null eval");

		return 0.0;
	}

	switch(a->nodeType) {
	
		case 'K': 
			v = ((struct numVal *)a)->number; 

			break;

		case 'N': 
			if((((struct symbolReference *)a)->symbol)->type != 'a' && (((struct symbolReference *)a)->symbol)->type != 'b') { 
				printf("using undeclared ID: %s\n", (((struct symbolReference *)a)->symbol)->name); 
			}

			v = (((struct symbolReference *)a)->symbol)->value; 

			break;

		case '=': 
			if((((struct symbolAssign *)a)->symbol)->type != 'a' && (((struct symbolAssign *)a)->symbol)->type != 'b') { 
				printf("using undeclared ID: %s\n", (((struct symbolAssign *)a)->symbol)->name); 
			} 

			v = ((struct symbolAssign *)a)->symbol->value = eval(((struct symbolAssign *)a)->v); 

			break;

		case '+': 
			v = eval(a->left) + eval(a->right); 
			
			break;

		case '-': 
			v = eval(a->left) - eval(a->right); 
			
			break;

		case '*': 
			v = eval(a->left) * eval(a->right); 

			break;

		case '/': 
			v = eval(a->left) / eval(a->right); 

			break;

		case 'S': 
			v = sin(eval(a->left)); 

			break;

		case 'C': 
			v = cos(eval(a->left)); 

			break;
		
		case 'T': 
			v = tan(eval(a->left)); 

			break;

		case '|': 
			v = fabs(eval(a->left)); 
		
			break;

		case 'M': 
			v = -eval(a->left); 

			break;

		case '1': 
			v = (eval(a->left) > eval(a->right))? 1 : 0; 
			
			break;

		case '2': 
			v = (eval(a->left) < eval(a->right))? 1 : 0; 
			
			break;
		
		case '3': 
			v = (eval(a->left) != eval(a->right))? 1 : 0; 
			
			break;

		case '4': 
			v = (eval(a->left) == eval(a->right))? 1 : 0; 
			
			break;

		case '5': 
			v = (eval(a->left) >= eval(a->right))? 1 : 0; 
			
			break;

		case '6': 
			v = (eval(a->left) <= eval(a->right))? 1 : 0; 
			
			break;

		case 'I':
			if(eval(((struct flow *)a)->cond) != 0) {
				if(((struct flow *)a)->treeList) {
					v = eval(((struct flow *)a)->treeList);
				} 
				else {
					v = 0.0;
				}
			} 
			else {
				if(((struct flow *)a)->evalList) {
					v = eval(((struct flow *)a)->evalList);
				} 
				else {
					v = 0.0;
				}
			}

			break;

		case 'W':
			v = 0.0;

			if(((struct flow *)a)->treeList) {
				while(eval(((struct flow *)a)->cond) != 0)
					v = eval(((struct flow *)a)->treeList);
			}

			break;

		case 'L': 
			eval(a->left); v = eval(a->right); 

			break;

		case 'P': 
			v = callPrint((struct printCall *)a); 

			break;

		case 'X': 
			v = callDeclaration((struct declaration *)a); 

			break;

		default: 
			printf("internal error: bad node %c\n", a->nodeType);
		}

	return v;
}

static double callDeclaration(struct declaration *d) {
	if(d->type == 'a') {
		while(d->symbolList) {
			((d->symbolList)->symbol)->value = 0;
			((d->symbolList)->symbol)->type = 'a';
			((d->symbolList)->symbol)->arrLength = 0;
			((d->symbolList)->symbol)->initialIndex = 0;
			((d->symbolList)->symbol)->arrHead = NULL;

			d->symbolList = (d->symbolList)->next;
		}
	}
	else if(d->type == 'b') {
		while(d->symbolList) {
			((d->symbolList)->symbol)->value = 0.0;
			((d->symbolList)->symbol)->type = 'b';
			((d->symbolList)->symbol)->arrLength = 0;
			((d->symbolList)->symbol)->initialIndex = 0;
			((d->symbolList)->symbol)->arrHead = NULL;

			d->symbolList = (d->symbolList)->next;
		}
	}
	else {
		printf("bad declaration type: %c\n", d->type);	

		return 1;
	}

	return 0;
}


static double callPrint(struct printCall *f) {
	int i;

	if((f->left)->nodeType == 'N') {
		printf("%s", (((struct symbolReference *)(f->left))->symbol)->name);
	}

	if((f->left)->nodeType == 'N' && (((struct symbolReference *)(f->left))->symbol)->arrLength > 0) {
		printf(" = {");

		for(i = 0; i < (((struct symbolReference *)(f->left))->symbol)->arrLength -1; i++) {
			printf("%4.4g, ", *((((struct symbolReference *)(f->left))->symbol)->arrHead + i));
		}

		printf("%4.4g", *((((struct symbolReference *)(f->left))->symbol)->arrHead + (((struct symbolReference *)(f->left))->symbol)->arrLength - 1));

		printf("}\n");

		return 0;
	}

	double v = eval(f->left);

	printf(" = %4.4g\n", v);
	
	return v;
}

void yyerror(char *s) {
	fprintf(stderr, "error: %s\n", s);
}

int main(int argc, char**argv) {
	extern FILE *yyin;

	++argv; 
	--argc;

	yyin = fopen(argv[0], "r");

	return yyparse();
}
