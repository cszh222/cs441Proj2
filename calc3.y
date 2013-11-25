%{
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "calc3.h"
#include "symbol_table.h"

/* prototypes */
nodeType *opr(int oper, int nops, ...);
nodeType *id(char* i);
nodeType *integer(int value);
nodeType *doub(double value);
nodeType *newId(char* i);
void freeNode(nodeType *p);
int ex(nodeType *p);
int yylex(void);

int lineno;

void yyerror(char *s);
/*int sym[26]; */                   /* symbol table */
%}

%union {
    int iValue;                 /* integer value */
    double fValue;
    char* sVariable;                /* symbol table index */
    nodeType *nPtr;             /* node pointer */
};

%token <iValue> INTEGER
%token <sVariable> VARIABLE
%token <fValue> DOUBLE
%token WHILE IF PRINT DO REPEAT UNTIL
%nonassoc IFX
%nonassoc ELSE

%left GE LE EQ NE '>' '<'
%left '+' '-'
%left '*' '/'
%nonassoc UMINUS

%type <nPtr> stmt expr stmt_list declaration variable

%%

program:
        function                { exit(0); }
        ;

function:
          function stmt         { ex($2); freeNode($2); }
        | /* NULL */
        ;

stmt:
          ';'                            { $$ = opr(';', 2, NULL, NULL); }
        | expr ';'                       { $$ = $1; }
        | PRINT expr ';'                 { $$ = opr(PRINT, 1, $2); }
        | VARIABLE '=' expr ';'          { $$ = opr('=', 2, id($1), $3); }
        | WHILE '(' expr ')' stmt        { $$ = opr(WHILE, 2, $3, $5); }
        | IF '(' expr ')' stmt %prec IFX { $$ = opr(IF, 2, $3, $5); }
        | IF '(' expr ')' stmt ELSE stmt { $$ = opr(IF, 3, $3, $5, $7); }
        | '{' stmt_list '}'              { $$ = $2; }
        | declaration ';'                { $$ = $1;}
        | DO stmt WHILE '(' expr ')'     { $$ = opr(DO, 2, $2, opr(WHILE, 2, $5, $2));}
        | REPEAT stmt UNTIL '(' expr ')' { $$ = opr(REPEAT, 2, $2, opr(UNTIL, 2, $5, $2));}
        ;

declaration:   INTEGER variable          {$$ = opr(INTEGER, 1, $2);}
            | DOUBLE variable            {$$ = opr(DOUBLE, 1, $2);}
            ;

variable: VARIABLE '=' expr ',' variable {$$ = opr(',', 2, opr('=', 2, newId($1), $3), $5);}
        | VARIABLE  '=' expr             {$$ = opr('=', 2, newId($1), $3);}
        | VARIABLE ',' variable          {$$ = opr(',', 2, newId($1), $3);}
        | VARIABLE                       {$$ = newId($1);}
        ;

stmt_list:
          stmt                  { $$ = $1; }
        | stmt_list stmt        { $$ = opr(';', 2, $1, $2); }
        ;

expr:
          INTEGER               { $$ = integer($1); }
        | DOUBLE                { $$ = doub($1);  }  
        | VARIABLE              { $$ = id($1); }
        | '-' expr %prec UMINUS { $$ = opr(UMINUS, 1, $2); }
        | expr '+' expr         { $$ = opr('+', 2, $1, $3); }
        | expr '-' expr         { $$ = opr('-', 2, $1, $3); }
        | expr '*' expr         { $$ = opr('*', 2, $1, $3); }
        | expr '/' expr         { $$ = opr('/', 2, $1, $3); }
        | expr '<' expr         { $$ = opr('<', 2, $1, $3); }
        | expr '>' expr         { $$ = opr('>', 2, $1, $3); }
        | expr GE expr          { $$ = opr(GE, 2, $1, $3); }
        | expr LE expr          { $$ = opr(LE, 2, $1, $3); }
        | expr NE expr          { $$ = opr(NE, 2, $1, $3); }
        | expr EQ expr          { $$ = opr(EQ, 2, $1, $3); }
        | '(' expr ')'          { $$ = $2; }
        ;

%%

#define SIZEOF_NODETYPE ((char *)&p->con - (char *)p)

int ex(nodeType *p){
    /*to be implemented later*/
    return 0;
}

nodeType *integer(int value) {
    nodeType *p;
    size_t nodeSize;

    /* allocate node */
    nodeSize = SIZEOF_NODETYPE + sizeof(conNodeType);
    if ((p = malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeInt;
    p->con.value = value;

    return p;
}

nodeType *doub(double value){
    nodeType *p;
    size_t nodeSize;

    /* allocate node */
    nodeSize = SIZEOF_NODETYPE + sizeof(floatNodeType);
    if ((p = malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeFloat;
    p->fl.value = value;

    return p;
}

/*nodeType *id(int i) {
    nodeType *p;
    size_t nodeSize;*/

    /* allocate node */
    /*nodeSize = SIZEOF_NODETYPE + sizeof(idNodeType);
    if ((p = malloc(nodeSize)) == NULL)
        yyerror("out of memory");*/

    /* copy information */
    /*p->type = typeId;
    p->id.i = i;

    return p;
}*/

nodeType *id(char* i){
    symbol_entry* entry;
    if((entry=getSymbolEntry(i)) == 0)
        yyerror("missing declaration for identifier"); 
    nodeType *p;
    size_t nodeSize;

    nodeSize = SIZEOF_NODETYPE + sizeof(idNodeType);
    if ((p = malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    p->type = typeId;
    p->id.i = entry;

    return p;
}

nodeType *newId(char* i){
    symbol_entry* newEntry = malloc(sizeof(symbol_entry));
    if(getSymbolEntry(i)!=0)
        yyerror("identifier has already been declared");
    newEntry->name = strdup(i);
    addSymbol(newEntry, lineno);

    nodeType *p;
    size_t nodeSize;

    nodeSize = SIZEOF_NODETYPE + sizeof(idNodeType);
    if ((p = malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    p->type = typeId;
    p->id.i = getSymbolEntry(i);
}

nodeType *opr(int oper, int nops, ...) {
    va_list ap;
    nodeType *p;
    size_t nodeSize;
    int i;

    /* allocate node */
    nodeSize = SIZEOF_NODETYPE + sizeof(oprNodeType) +
        (nops - 1) * sizeof(nodeType*);
    if ((p = malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeOpr;
    p->opr.oper = oper;
    p->opr.nops = nops;
    va_start(ap, nops);
    for (i = 0; i < nops; i++)
        p->opr.op[i] = va_arg(ap, nodeType*);
    va_end(ap);
    return p;
}

void freeNode(nodeType *p) {
    int i;

    if (!p) return;
    if (p->type == typeOpr) {
        for (i = 0; i < p->opr.nops; i++)
            freeNode(p->opr.op[i]);
    }
    free (p);
}

void yyerror(char *s) {
    fprintf(stdout, "%s\n", s);
}

int main(void) {
    lineno = 1;
    yyparse();
    return 0;
}
