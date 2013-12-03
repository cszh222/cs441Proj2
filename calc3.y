%{
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string>
#include <cstring>
#include "calc3.h"
#include "symbol_table.h"
#include "pstcode.h"
#include "apm.h"

/* prototypes */
nodeType *opr(int oper, int nops, ...);
nodeType *id(char* i);
nodeType *integer(int value);
nodeType *doub(double value);
nodeType *newId(char* i);
nodeType *declareDoub(nodeType *p);
nodeType *declareInt(nodeType *p);
void freeNode(nodeType *p);
value* ex(nodeType *p);
int yylex(void);

int lineno;
PstackCode myPStack;

void yyerror(std::string s);
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
        function                {}
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
    if ((p = (nodeType*) malloc(nodeSize)) == NULL)
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
    if ((p = (nodeType*) malloc(nodeSize)) == NULL)
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
    if ((p = (nodeType*)malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    p->type = typeId;
    p->id.i = entry;

    return p;
}

nodeType *newId(char* i){
    symbol_entry* newEntry = (symbol_entry*)malloc(sizeof(symbol_entry));
    nodeType *p;
    size_t nodeSize;

    if(getSymbolEntry(i)!=0)
        yyerror("identifier has already been declared");
    newEntry->name = strdup(i);
    newEntry->size = 1; //this is used to count the number of symbols
    addSymbol(newEntry, lineno);

    nodeSize = SIZEOF_NODETYPE + sizeof(idNodeType);
    if ((p = (nodeType*)malloc(nodeSize)) == NULL)
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
    if ((p = (nodeType*)malloc(nodeSize)) == NULL)
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

void yyerror(std::string s) {
    fprintf(stderr, "%s\n", s.c_str());
    exit(1);
}

int main(void) {
    pushSymbolTable();
    lineno = 1;
    myPStack.begin_prog();
    yyparse();
    myPStack.end_prog(getCurrentSymbolTableSize());
    myPStack.write("calc3p.apm", 1);
    return 0;
}

value* ex(nodeType *p) {
    if (!p) return 0;
    switch(p->type) {
    case typeInt:       {
                            value* newVal = (value*)malloc(sizeof(value));
                            newVal->type = intValType;
                            newVal->intVal = p->con.value;
                            
                            myPStack.add(I_CONSTANT);
                            myPStack.add(newVal->intVal);
                            
                            return newVal;
                        }
    case typeFloat:     {
                            value* newVal = (value*)malloc(sizeof(value));
                            newVal->type = floatValType;
                            newVal->floatVal = p->fl.value;
                            
                            myPStack.add(R_CONSTANT);
                            myPStack.add(newVal->floatVal);
                            
                            return newVal;
                        }
    case typeId:        {
                        value* newVal = (value*)malloc(sizeof(value));
                        if (p->id.i->type == TYPE_INT){
                            newVal->type = intValType;
                            newVal->intVal = p->id.i->iVal;
                            
                            myPStack.add(I_VARIABLE);
                            myPStack.add(0);
                            myPStack.add(p->id.i->offset);
                            
                            return newVal;
                        }
                        else if (p->id.i->type == TYPE_FLOAT){
                            newVal->type = floatValType;
                            newVal->floatVal = p->id.i->fVal;
                            
                            myPStack.add(R_VARIABLE);
                            myPStack.add(0);
                            myPStack.add(p->id.i->offset);
                            
                            return newVal;
                        }
                        }
    case typeOpr:
        switch(p->opr.oper) {
        case WHILE:     {//while(ex(p->opr.op[0])->boolVal)
                        //    ex(p->opr.op[1]); 

                        ex(p->opr.op[0]);   
                        int jmpAddr = myPStack.pos();                     
                        myPStack.add(I_JMP_IF_FALSE);
                        myPStack.add(0);
                        int initAddr = myPStack.pos();
                        ex(p->opr.op[1]);
                        ex(p->opr.op[0]);
                        myPStack.add(I_JMP);
                        myPStack.add(jmpAddr);
                        myPStack.at(initAddr-1) = myPStack.pos();
                        return 0;
                        }
        case DO:        {/*do{ 
                        ex(p->opr.op[0]);
                            }
                        while(ex(p->opr.op[1])->boolVal);*/
                        int jmpAddr = myPStack.pos();
                        ex(p->opr.op[0]);                        
                        ex(p->opr.op[1]);
                        myPStack.add(I_JMP_IF_TRUE);
                        myPStack.add(jmpAddr);
                        }

                        return 0;
        case REPEAT:    {/*do
                            ex(p->opr.op[0]);
                        while(!ex(p->opr.op[1])->boolVal);*/
                        int jmpAddr = myPStack.pos();
                        ex(p->opr.op[0]);                        
                        ex(p->opr.op[1]);
                        myPStack.add(I_JMP_IF_FALSE);
                        myPStack.add(jmpAddr);
                        }
                        return 0;
        case INT:       ex(p->opr.op[0]); 
                        return 0;
        case DOUB:      ex(p->opr.op[0]);  
                        return 0;
        case ',':       ex(p->opr.op[0]);
                        ex(p->opr.op[1]);
                        return 0;
        case '=':       {
                        value* retval;
                        ex(p->opr.op[0]);
                        if (p->opr.op[0]->id.i->type == TYPE_INT){
                            retval = ex(p->opr.op[1]);
                            p->opr.op[0]->id.i->iVal = retval->intVal;
                            
                            myPStack.add(I_ASSIGN);
                            myPStack.add(1);
                           
                            return retval;
                        }
                        else if (p->opr.op[0]->id.i->type == TYPE_FLOAT){
                            retval = ex(p->opr.op[1]);
                            p->opr.op[0]->id.i->fVal = retval->floatVal;
                            
                            myPStack.add(R_ASSIGN);
                            myPStack.add(1);
                            
                            return retval;
                        }
                        }
        case IF:        {                        
                        ex(p->opr.op[0]);
                        
                        myPStack.add(I_JMP_IF_FALSE);
                        myPStack.add(0);
                        
                        int initAddr = myPStack.pos();
                        //fprintf(stderr, "%d", initAddr);
                        ex(p->opr.op[1]);
                        
                        myPStack.at(initAddr-1) = myPStack.pos();

                        if (p->opr.nops > 2)
                            ex(p->opr.op[2]);
                        //if (ex(p->opr.op[0])->boolVal)
                        //    ex(p->opr.op[1]);
                        //else if (p->opr.nops > 2)
                        //    ex(p->opr.op[2]);
                        return 0;
                        }
        case PRINT:     {
                        value* newVal = ex(p->opr.op[0]);
                        if(p->opr.op[0]->type == typeId)
                            myPStack.add(I_VALUE);
                        if(newVal->type == intValType){
                            
                            myPStack.add(I_WRITE);
                            myPStack.add(1);

                            //printf("%d\n", newVal->intVal);
                        }
                        else{
                            
                            myPStack.add(R_WRITE);
                            myPStack.add(1);
                            //printf("%f\n", newVal->floatVal);
                        }
                            fflush(stdout);
                            return 0;
                        }
        case ';':       ex(p->opr.op[0]); return ex(p->opr.op[1]);
        case UMINUS:    {
                        value* newVal = ex(p->opr.op[0]);                        
                        if(newVal->type == intValType){
                          if(p->opr.op[0]->type == typeId){
                            ex(p->opr.op[0]);
                            myPStack.add(I_VALUE);
                            myPStack.add(I_MINUS);
                            myPStack.add(I_ASSIGN);
                            myPStack.add(1);
                            ex(p->opr.op[0]);
                          }
                          else
                            myPStack.add(I_MINUS);
                          newVal->intVal = -1*newVal->intVal;
                          }
                        else{
                         if(p->opr.op[0]->type == typeId){
                            ex(p->opr.op[0]);
                            myPStack.add(R_VALUE);
                            myPStack.add(R_MINUS);
                            myPStack.add(R_ASSIGN);
                            myPStack.add(1);
                            ex(p->opr.op[0]);
                           }
                           else
                            myPStack.add(R_MINUS);
                            newVal->floatVal = -1*newVal->floatVal;
                         }
                        return newVal;
                        }
        case '+':       {
                         value* newVal2 = ex(p->opr.op[0]);
                         if(p->opr.op[0]->type == typeId)
                            myPStack.add(I_VALUE);
                         value* newVal1 = ex(p->opr.op[1]);
                         if(p->opr.op[1]->type == typeId)
                            myPStack.add(I_VALUE);
                         if(newVal1->type == intValType){
                         
                            myPStack.add(I_ADD);
                         
                            newVal1->intVal = newVal1->intVal + newVal2->intVal;
                        }
                         else{
                         
                            myPStack.add(R_ADD);
                         
                            newVal1->floatVal = newVal1->floatVal + newVal2->floatVal;
                        }
                         return newVal1;
                         }
        case '-':       {
                         value* newVal1 = ex(p->opr.op[0]);
                         if(p->opr.op[0]->type == typeId)
                            myPStack.add(I_VALUE);
                         value* newVal2 = ex(p->opr.op[1]);
                         if(p->opr.op[1]->type == typeId)
                            myPStack.add(I_VALUE);
                         if(newVal1->type == intValType){
                         
                            myPStack.add(I_SUBTRACT);
                         
                            newVal1->intVal = newVal1->intVal + newVal2->intVal;
                        }
                         else{
                            
                            myPStack.add(R_SUBTRACT);
                            
                            newVal1->floatVal = newVal1->floatVal + newVal2->floatVal;
                        }
                         return newVal1;
                         }
        case '*':       {
                         value* newVal1 = ex(p->opr.op[0]);
                         if(p->opr.op[0]->type == typeId)
                            myPStack.add(I_VALUE);
                         value* newVal2 = ex(p->opr.op[1]);
                         if(p->opr.op[1]->type == typeId)
                            myPStack.add(I_VALUE);
                         if(newVal1->type == intValType){
                         
                            myPStack.add(I_MULTIPLY);
                          
                            newVal1->intVal = newVal1->intVal + newVal2->intVal;
                        }
                         else{
                         
                            myPStack.add(R_MULTIPLY);
                            
                            newVal1->floatVal = newVal1->floatVal + newVal2->floatVal;
                        }
                         return newVal1;
                         }
        case '/':       {
                         value* newVal1 = ex(p->opr.op[0]);
                         if(p->opr.op[0]->type == typeId)
                            myPStack.add(I_VALUE);
                         value* newVal2 = ex(p->opr.op[1]);
                         if(p->opr.op[1]->type == typeId)
                            myPStack.add(I_VALUE);
                         if(newVal1->type == intValType){
                         
                            myPStack.add(I_DIVIDE);
                         
                            newVal1->intVal = newVal1->intVal + newVal2->intVal;
                        }
                         else{
                         
                            myPStack.add(R_DIVIDE);
                            
                            newVal1->floatVal = newVal1->floatVal + newVal2->floatVal;
                        }
                         return newVal1;
                         }
        case '<':       {
                         value* newVal2 = ex(p->opr.op[0]);
                         if(p->opr.op[0]->type == typeId)
                            myPStack.add(I_VALUE);
                         value* newVal1 = ex(p->opr.op[1]);
                         if(p->opr.op[1]->type == typeId)
                            myPStack.add(I_VALUE);
                         if(p->opr.op[1]->type == typeId)
                            myPStack.add(I_VALUE);

                         value* newVal3 = (value*)malloc(sizeof(value));
                         newVal3->type = boolValType;
                         if(newVal1->type == intValType){

                            if(newVal1->intVal < newVal2->intVal)
                                newVal3->boolVal = true;
                            else
                                newVal3->boolVal = false;
                         
                            myPStack.add(I_LESS);
                        }
                         
                         else{
                            if(newVal1->floatVal < newVal2->floatVal)
                                newVal3->boolVal = true;
                            else
                                newVal3->boolVal = false;
                                newVal1->floatVal = newVal1->floatVal < newVal2->floatVal;

                            myPStack.add(R_LESS);
                        }                      

                         return newVal3;
                         }
        case '>':       {
                         value* newVal1 = ex(p->opr.op[0]);
                         if(p->opr.op[0]->type == typeId)
                            myPStack.add(I_VALUE);
                         value* newVal2 = ex(p->opr.op[1]);
                         if(p->opr.op[1]->type == typeId)
                            myPStack.add(I_VALUE);
                         value* newVal3 = (value*)malloc(sizeof(value));
                         newVal3->type = boolValType;
                         if(newVal1->type == intValType){
                            if(newVal1->intVal > newVal2->intVal)
                                newVal3->boolVal = true;
                            else
                                newVal3->boolVal = false;
                         
                            myPStack.add(I_GREATER);
                            }
                         
                         else{
                            if(newVal1->floatVal > newVal2->floatVal)
                                newVal3->boolVal = true;
                            else
                                newVal3->boolVal = false; 
                            newVal1->floatVal = newVal1->floatVal < newVal2->floatVal;
                         
                            myPStack.add(R_GREATER);
                            }
                         
                         return newVal3;
                         }
        case GE:    {
                         value* newVal1 = ex(p->opr.op[0]);
                         if(p->opr.op[0]->type == typeId)
                            myPStack.add(I_VALUE);
                         value* newVal2 = ex(p->opr.op[1]);
                         if(p->opr.op[1]->type == typeId)
                            myPStack.add(I_VALUE);
                         value* newVal3 = (value*)malloc(sizeof(value));
                         newVal3->type = boolValType;
                         if(newVal1->type == intValType){
                           if(newVal1->intVal >= newVal2->intVal)
                                newVal3->boolVal = true;
                            else
                                newVal3->boolVal = false; 
                            
                            myPStack.add(I_EQUAL);
                            myPStack.add(I_CONSTANT);
                            myPStack.add(0);
                            myPStack.add(I_EQUAL);
                            myPStack.add(I_JMP_IF_FALSE);
                            myPStack.add(0);
                            int initAddr = myPStack.pos();
                            myPStack.add(I_CONSTANT);
                            myPStack.add(0);
                            myPStack.add(I_AND);
                            ex(p->opr.op[0]);
                            myPStack.add(I_OR);
                            ex(p->opr.op[1]);
                            myPStack.add(I_GREATER);
                            myPStack.add(I_JMP);
                            myPStack.add(0);
                            int secondAddr = myPStack.pos();
                            myPStack.at(initAddr - 1) = myPStack.pos();
                            myPStack.add(I_CONSTANT);
                            myPStack.add(1);
                            myPStack.add(I_OR);
                            myPStack.at(secondAddr - 1) = myPStack.pos();
                         }
                         
                         else{
                            if(newVal1->floatVal >= newVal2->floatVal)
                                newVal3->boolVal = true;
                            else
                                newVal3->boolVal = false;
                                
                            myPStack.add(R_EQUAL);
                            myPStack.add(R_CONSTANT);
                            myPStack.add(0);
                            myPStack.add(R_EQUAL);
                            myPStack.add(I_JMP_IF_FALSE);
                            myPStack.add(0);
                            int initAddr = myPStack.pos();
                            myPStack.add(I_CONSTANT);
                            myPStack.add(0);
                            myPStack.add(I_AND);
                            ex(p->opr.op[0]);
                            myPStack.add(I_OR);
                            ex(p->opr.op[1]);
                            myPStack.add(R_GREATER);
                            myPStack.add(I_JMP);
                            myPStack.add(0);
                            int secondAddr = myPStack.pos();
                            myPStack.at(initAddr - 1) = myPStack.pos();
                            myPStack.add(I_CONSTANT);
                            myPStack.add(1);
                            myPStack.add(I_OR);
                            myPStack.at(secondAddr - 1) = myPStack.pos();
                            
                         return NULL;
                         }
                    }
        case LE:    {
        
                         value* newVal1 = ex(p->opr.op[0]);
                         if(p->opr.op[0]->type == typeId)
                            myPStack.add(I_VALUE);
                         value* newVal2 = ex(p->opr.op[1]);
                         if(p->opr.op[1]->type == typeId)
                            myPStack.add(I_VALUE);
                         value* newVal3 = (value*)malloc(sizeof(value));
                         newVal3->type = boolValType;
                         if(newVal1->type == intValType){
                           if(newVal1->intVal <= newVal2->intVal)
                                newVal3->boolVal = true;
                            else
                                newVal3->boolVal = false; 
                            
                            myPStack.add(I_EQUAL);
                            myPStack.add(I_CONSTANT);
                            myPStack.add(0);
                            myPStack.add(I_EQUAL);
                            myPStack.add(I_JMP_IF_FALSE);
                            myPStack.add(0);
                            int initAddr = myPStack.pos();
                            myPStack.add(I_CONSTANT);
                            myPStack.add(0);
                            myPStack.add(I_AND);
                            ex(p->opr.op[0]);
                            myPStack.add(I_OR);
                            ex(p->opr.op[1]);
                            myPStack.add(I_LESS);
                            myPStack.add(I_JMP);
                            myPStack.add(0);
                            int secondAddr = myPStack.pos();
                            myPStack.at(initAddr - 1) = myPStack.pos();
                            myPStack.add(I_CONSTANT);
                            myPStack.add(1);
                            myPStack.add(I_OR);
                            myPStack.at(secondAddr - 1) = myPStack.pos();
                         }
                         
                         else {
                            if(newVal1->floatVal <= newVal2->floatVal)
                                newVal3->boolVal = true;
                            else
                                newVal3->boolVal = false;
                                
                            myPStack.add(R_EQUAL);
                            myPStack.add(R_CONSTANT);
                            myPStack.add(0);
                            myPStack.add(R_EQUAL);
                            myPStack.add(I_JMP_IF_FALSE);
                            myPStack.add(0);
                            int initAddr = myPStack.pos();
                            myPStack.add(I_CONSTANT);
                            myPStack.add(0);
                            myPStack.add(I_AND);
                            ex(p->opr.op[0]);
                            myPStack.add(I_OR);
                            ex(p->opr.op[1]);
                            myPStack.add(R_LESS);
                            myPStack.add(I_JMP);
                            myPStack.add(0);
                            int secondAddr = myPStack.pos();
                            myPStack.at(initAddr - 1) = myPStack.pos();
                            myPStack.add(I_CONSTANT);
                            myPStack.add(1);
                            myPStack.add(I_OR);
                            myPStack.at(secondAddr - 1) = myPStack.pos();
                            
                         return NULL;
                         }
                    }
        case NE:        {
                         value* newVal1 = ex(p->opr.op[0]);
                         if(p->opr.op[0]->type == typeId)
                            myPStack.add(I_VALUE);
                         value* newVal2 = ex(p->opr.op[1]);
                         if(p->opr.op[1]->type == typeId)
                            myPStack.add(I_VALUE);
                         value* newVal3 = (value*)malloc(sizeof(value));
                         newVal3->type = boolValType;
                         if(newVal1->type == intValType){
                            if(newVal1->intVal != newVal2->intVal)
                                newVal3->boolVal = true;
                            else
                                newVal3->boolVal = false; 

                            myPStack.add(I_EQUAL);
                            myPStack.add(I_NOT); 
                        }                              
                         else{
                            if(newVal1->floatVal != newVal2->floatVal)
                                newVal3->boolVal = true;
                            else
                                newVal3->boolVal = false; 
                            newVal1->floatVal = newVal1->floatVal < newVal2->floatVal;
                            }
                            myPStack.add(I_EQUAL);
                            myPStack.add(I_NOT); 
                         return newVal3;
                         }
        case EQ:        {
                         value* newVal1 = ex(p->opr.op[0]);
                         if(p->opr.op[0]->type == typeId)
                            myPStack.add(I_VALUE);
                         value* newVal2 = ex(p->opr.op[1]);
                         if(p->opr.op[1]->type == typeId)
                            myPStack.add(I_VALUE);
                         value* newVal3 = (value*)malloc(sizeof(value));
                         newVal3->type = boolValType;
                         if(newVal1->type == intValType){
                            if(newVal1->intVal == newVal2->intVal)
                                newVal3->boolVal = true;
                            else
                                newVal3->boolVal = false;
                         
                         myPStack.add(I_EQUAL);}
                         
                         else{
                            if(newVal1->floatVal == newVal2->floatVal)
                               newVal3->boolVal = true;
                            else
                                newVal3->boolVal = false; 
                            newVal1->floatVal = newVal1->floatVal < newVal2->floatVal;
                         
                         myPStack.add(R_EQUAL);}
                         
                         return newVal3;
                         }		
        }
    }
    return 0;
}
