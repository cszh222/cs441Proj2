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
nodeType *declareDoub(nodeType *p);
nodeType *declareInt(nodeType *p);
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
%token WHILE IF PRINT DO REPEAT UNTIL INT DOUB
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
        | DO stmt WHILE '(' expr ')'     { $$ = opr(DO, 2, $2, $5);}
        | REPEAT stmt UNTIL '(' expr ')' { $$ = opr(REPEAT, 2, $2, $5);}
        ;

declaration:   INT variable            {$$ = opr(INT, 1, declareInt($2));}
            | DOUB variable            {$$ = opr(DOUB, 1, declareDoub($2));}
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
    nodeType *p;
    size_t nodeSize;

    if(getSymbolEntry(i)!=0)
        yyerror("identifier has already been declared");
    newEntry->name = strdup(i);
    addSymbol(newEntry, lineno);

    nodeSize = SIZEOF_NODETYPE + sizeof(idNodeType);
    if ((p = malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    p->type = typeId;
    p->id.i = getSymbolEntry(i);

    return p;
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

/* setting type of declared variables to INT*/
nodeType *declareInt(nodeType *p){
    switch(p->type){
        case typeId: {
            symbol_entry *entry = p->id.i;
            entry->type = TYPE_INT;
            return p;
        }
        case typeOpr:
            switch(p->opr.oper){
                case ',': 
                    declareInt(p->opr.op[0]);
                    declareInt(p->opr.op[1]);
                    return p;
                case '=': 
                    declareInt(p->opr.op[0]);
                    return p;
        }
    }
}

/* setting type of declared variables to DOUB*/
nodeType *declareDoub(nodeType *p){
    switch(p->type){
        case typeId: {
            symbol_entry *entry = p->id.i;
            entry->type = TYPE_FLOAT;
            return p;
        }
        case typeOpr:
            switch(p->opr.oper){
                case ',': 
                    declareDoub(p->opr.op[0]);
                    declareDoub(p->opr.op[1]);
                    return p;
                case '=': 
                    declareDoub(p->opr.op[0]);
                    return p;
        }
    }
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
    fprintf(stderr, "%s\n", s);
    exit(1);
}

int main(void) {
    pushSymbolTable();
    lineno = 1;
    yyparse();
    return 0;
}

int ex(nodeType *p) {
    if (!p) return 0;
    switch(p->type) {
    case typeInt:       {
                            value* newVal = malloc(sizeof(value));
                            newVal->type = intValType;
                            newVal->intVal = p->con.value;
                            return newVal;
                        }
    case typeFloat:     return p->fl.value;
    case typeId:        if (p->id.i->type == TYPE_INT)
                            return p->id.i->iVal;
                        else if (p->id.i->type == TYPE_FLOAT)
                            return p->id.i->fVal;
    case typeOpr:
        switch(p->opr.oper) {
        case WHILE:     while(ex(p->opr.op[0]))
                            ex(p->opr.op[1]); 
                        return 0;
        case DO:        do{ 
                           ex(p->opr.op[0]);
                        }
                        while(ex(p->opr.op[1]));
                        return 0;
        case REPEAT:    do
                            ex(p->opr.op[0]);
                        while(!ex(p->opr.op[1]));
                        return 0;
        case INT:       ex(p->opr.op[0]); 
                        return 0;
        case DOUB:      ex(p->opr.op[0]);  
                        return 0;
        case ',':       ex(p->opr.op[0]);
                        ex(p->opr.op[1]);
                        return 0;
        case '=':       if (p->opr.op[0]->id.i->type == TYPE_INT)
                            return p->opr.op[0]->id.i->iVal = ex(p->opr.op[1]);
                        else if (p->opr.op[0]->id.i->type == TYPE_FLOAT)
                            return p->opr.op[0]->id.i->fVal = ex(p->opr.op[1]);
        case IF:        if (ex(p->opr.op[0]))
                            ex(p->opr.op[1]);
                        else if (p->opr.nops > 2)
                            ex(p->opr.op[2]);
                        return 0;
        case PRINT:     printf("%d\n", ex(p->opr.op[0])); return 0;
        case ';':       ex(p->opr.op[0]); return ex(p->opr.op[1]);
        case UMINUS:    {
                        value* newVal = ex(p->opr.op[0]);
                        if(newVal->type == intValType)
                           newVal->intVal = -1*newVal->intVal;
                        else
                            newVal->floatVal = -1*newVal->floatVal
                        return newVal;
                        }
        case '+':       {
                         value* newVal1 = ex(p->opr.op[0]);
                         value* newVal2 = ex(p->opr.op[1]);
                         if(newVal1->type == intValType)
                            newVal1.intVal = newVal1.intVal + newVal2.intVal;
                         else
                            newVal1.floatVal = newVal1.floatVal + newVal2.floatVal;
                         return newVal1;
                         }
        case '-':       {
                         value* newVal1 = ex(p->opr.op[0]);
                         value* newVal2 = ex(p->opr.op[1]);
                         if(newVal1->type == intValType)
                            newVal1.intVal = newVal1.intVal - newVal2.intVal;
                         else
                            newVal1.floatVal = newVal1.floatVal - newVal2.floatVal;
                         return newVal1;
                         }
        case '*':       {
                         value* newVal1 = ex(p->opr.op[0]);
                         value* newVal2 = ex(p->opr.op[1]);
                         if(newVal1->type == intValType)
                            newVal1.intVal = newVal1.intVal * newVal2.intVal;
                         else
                            newVal1.floatVal = newVal1.floatVal * newVal2.floatVal;
                         return newVal1;
                         }
        case '/':       {
                         value* newVal1 = ex(p->opr.op[0]);
                         value* newVal2 = ex(p->opr.op[1]);
                         if(newVal1->type == intValType)
                            newVal1.intVal = newVal1.intVal / newVal2.intVal;
                         else
                            newVal1.floatVal = newVal1.floatVal / newVal2.floatVal;
                         return newVal1;
                         }
        case '<':       {
                         value* newVal1 = ex(p->opr.op[0]);
                         value* newVal2 = ex(p->opr.op[1]);
                         if(newVal1->type == intValType)
                            newVal1.intVal = newVal1.intVal < newVal2.intVal;
                         else
                            newVal1.floatVal = newVal1.floatVal < newVal2.floatVal;
                         return newVal1;
                         }
        case '>':       {
                         value* newVal1 = ex(p->opr.op[0]);
                         value* newVal2 = ex(p->opr.op[1]);
                         if(newVal1->type == intValType)
                            newVal1.intVal = newVal1.intVal > newVal2.intVal;
                         else
                            newVal1.floatVal = newVal1.floatVal > newVal2.floatVal;
                         return newVal1;
                         }
        case GE:        {
                         value* newVal1 = ex(p->opr.op[0]);
                         value* newVal2 = ex(p->opr.op[1]);
                         if(newVal1->type == intValType)
                            newVal1.intVal = newVal1.intVal >= newVal2.intVal;
                         else
                            newVal1.floatVal = newVal1.floatVal >= newVal2.floatVal;
                         return newVal1;
                         }
        case LE:        {
                         value* newVal1 = ex(p->opr.op[0]);
                         value* newVal2 = ex(p->opr.op[1]);
                         if(newVal1->type == intValType)
                            newVal1.intVal = newVal1.intVal <= newVal2.intVal;
                         else
                            newVal1.floatVal = newVal1.floatVal <= newVal2.floatVal;
                         return newVal1;
                         }
        case NE:        {
                         value* newVal1 = ex(p->opr.op[0]);
                         value* newVal2 = ex(p->opr.op[1]);
                         if(newVal1->type == intValType)
                            newVal1.intVal = newVal1.intVal != newVal2.intVal;
                         else
                            newVal1.floatVal = newVal1.floatVal != newVal2.floatVal;
                         return newVal1;
                         }
        case EQ:        {
                         value* newVal1 = ex(p->opr.op[0]);
                         value* newVal2 = ex(p->opr.op[1]);
                         if(newVal1->type == intValType)
                            newVal1.intVal = newVal1.intVal == newVal2.intVal;
                         else
                            newVal1.floatVal = newVal1.floatVal == newVal2.floatVal;
                         return newVal1;
                         }		
        }
    }
    return 0;
}
