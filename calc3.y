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
int ex(nodeType *p);
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

int ex(nodeType *p) {
    if (!p) return 0;
    switch(p->type) {
    case typeInt:       {
                            myPStack.add(I_CONSTANT);
                            myPStack.add(p->con.value);
                            
                            return typeInt;
                        }
    case typeFloat:     {
                            myPStack.add(R_CONSTANT);
                            myPStack.add(p->fl.value);
                            
                            return typeFloat;
                        }
    case typeId:        {
                        if (p->id.i->type == TYPE_INT){
                                                      
                            myPStack.add(I_VARIABLE);
                            myPStack.add(p->id.i->blk_level);
                            myPStack.add(p->id.i->offset);
			                myPStack.add(I_VALUE);
                            
                            return TYPE_INT;
                        }
                        else if (p->id.i->type == TYPE_FLOAT){
                                                      
                            myPStack.add(R_VARIABLE);
                            myPStack.add(p->id.i->blk_level);
                            myPStack.add(p->id.i->offset);
                            myPStack.add(R_VALUE);
                            return TYPE_FLOAT;
                        }
                        }
    case typeOpr:
        switch(p->opr.oper) {
        case WHILE:     {
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
        case DO:        {
                        int jmpAddr = myPStack.pos();
                        ex(p->opr.op[0]);                        
                        ex(p->opr.op[1]);
                        myPStack.add(I_JMP_IF_TRUE);
                        myPStack.add(jmpAddr);
                        }

                        return 0;
        case REPEAT:    {
                        int jmpAddr = myPStack.pos();
                        ex(p->opr.op[0]);                        
                        ex(p->opr.op[1]);
                        myPStack.add(I_JMP_IF_FALSE);
                        myPStack.add(jmpAddr);
                        
                        return 0;
                        }
        case INT:       ex(p->opr.op[0]); 
                        return 0;
        case DOUB:      ex(p->opr.op[0]);  
                        return 0;
        case ',':       ex(p->opr.op[0]);
                        ex(p->opr.op[1]);
                        return 0;
        case '=':       {
			             if (p->opr.op[0]->id.i->type == TYPE_INT){
                            myPStack.add(I_VARIABLE);
			                myPStack.add(p->opr.op[0]->id.i->blk_level);
			                myPStack.add(p->opr.op[0]->id.i->offset);
			    
			                ex(p->opr.op[1]);
                                                      
                            myPStack.add(I_ASSIGN);
                            myPStack.add(1);
                           
                            return TYPE_INT;
                        }
                        else if (p->opr.op[0]->id.i->type == TYPE_FLOAT){
                            myPStack.add(R_VARIABLE);
			                myPStack.add(p->opr.op[0]->id.i->blk_level);
			                myPStack.add(p->opr.op[0]->id.i->offset);
                            
                            ex(p->opr.op[1]);                            
                            
                            myPStack.add(R_ASSIGN);
                            myPStack.add(1);
                            
                            return TYPE_FLOAT;
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

                        return 0;
                        }
        case PRINT:     { 
                        int valueType = ex(p->opr.op[0]); 
                                             
                        if(valueType == TYPE_INT || valueType == typeInt)
                            myPStack.add(I_WRITE);
                        else if (valueType == TYPE_FLOAT || valueType == typeFloat)
                            myPStack.add(R_WRITE);                                                 
                        
                        myPStack.add(1);
                        
                        return 0;
                        }
        case ';':       ex(p->opr.op[0]); return ex(p->opr.op[1]);
        case UMINUS:    {
                        int valueType = ex(p->opr.op[0]); 
                     
                        if(valueType == TYPE_INT || valueType == typeInt)
                            myPStack.add(I_MINUS);
                        else if (valueType == TYPE_FLOAT || valueType == typeFloat)
                            myPStack.add(R_MINUS);                                                 
                        return valueType;
                        }
        case '+':       {
                         int valueType = ex(p->opr.op[0]);
                         ex(p->opr.op[1]);   
                                                               
                         if(valueType == TYPE_INT || valueType == typeInt)
                             myPStack.add(I_ADD);
                         else if (valueType == TYPE_FLOAT || valueType == typeFloat)
                             myPStack.add(R_ADD);  

                         return valueType;
                        }
        case '-':       {
                         int valueType = ex(p->opr.op[0]);
                         ex(p->opr.op[1]);
 
                         if(p->opr.op[0]->type == typeId)            
                              valueType =p->opr.op[0]->id.i->type;
                         else{
                             valueType = p->opr.op[0]->type;
                         } 
                     
                         if(valueType == TYPE_INT || valueType == typeInt)
                             myPStack.add(I_SUBTRACT);
                         else if (valueType == TYPE_FLOAT || valueType == typeFloat)
                             myPStack.add(R_SUBTRACT);  

                         return valueType;
                         }
        case '*':       {
                         int valueType = ex(p->opr.op[0]);
                         ex(p->opr.op[1]);
    
                         if(p->opr.op[0]->type == typeId)            
                              valueType =p->opr.op[0]->id.i->type;
                         else{
                             valueType = p->opr.op[0]->type;
                         } 
                     
                         if(valueType == TYPE_INT || valueType == typeInt)
                             myPStack.add(I_MULTIPLY);
                         else if (valueType == TYPE_FLOAT || valueType == typeFloat)
                             myPStack.add(R_MULTIPLY);  

                         return valueType;
                         }
        case '/':       {
                         int valueType = ex(p->opr.op[0]);
                         ex(p->opr.op[1]);

                         if(p->opr.op[0]->type == typeId)            
                              valueType =p->opr.op[0]->id.i->type;
                         else{
                             valueType = p->opr.op[0]->type;
                         } 
                     
                         if(valueType == TYPE_INT || valueType == typeInt)
                             myPStack.add(I_DIVIDE);
                         else if (valueType == TYPE_FLOAT || valueType == typeFloat)
                             myPStack.add(R_DIVIDE);  

                         return valueType;
                         }
        case '<':       {
                         ex(p->opr.op[0]);
                         ex(p->opr.op[1]);
    
                         int valueType;
                         if(p->opr.op[0]->type == typeId)            
                              valueType =p->opr.op[0]->id.i->type;
                         else{
                             valueType = p->opr.op[0]->type;
                         } 
                     
                         if(valueType == TYPE_INT || valueType == typeInt)
                             myPStack.add(I_LESS);
                         else if (valueType == TYPE_FLOAT || valueType == typeFloat)
                             myPStack.add(R_GREATER);  

                         return 0;
                         }
        case '>':       {
                         ex(p->opr.op[0]);
                         ex(p->opr.op[1]);
    
                         int valueType;
                         if(p->opr.op[0]->type == typeId)            
                              valueType =p->opr.op[0]->id.i->type;
                         else{
                             valueType = p->opr.op[0]->type;
                         } 
                     
                         if(valueType == TYPE_INT || valueType == typeInt)
                             myPStack.add(I_GREATER);
                         else if (valueType == TYPE_FLOAT || valueType == typeFloat)
                             myPStack.add(R_GREATER);  

                         return 0;
                         }
        case GE:        {
                         ex(p->opr.op[0]);
                         ex(p->opr.op[1]);
    
                         int valueType;
                         if(p->opr.op[0]->type == typeId)            
                              valueType =p->opr.op[0]->id.i->type;
                         else{
                             valueType = p->opr.op[0]->type;
                         } 
                     
                         if(valueType == TYPE_INT || valueType == typeInt)
                             myPStack.add(I_EQUAL);
                         else if (valueType == TYPE_FLOAT || valueType == typeFloat)
                             myPStack.add(R_EQUAL);  

                         ex(p->opr.op[0]);
                         ex(p->opr.op[1]);

                         if(valueType == TYPE_INT || valueType == typeInt)
                             myPStack.add(I_GREATER);
                         else if (valueType == TYPE_FLOAT || valueType == typeFloat)
                             myPStack.add(R_GREATER);
                         
                         myPStack.add(I_OR);
                         return 0;
                        }
        case LE:        {    
                         ex(p->opr.op[0]);
                         ex(p->opr.op[1]);
    
                         int valueType;
                         if(p->opr.op[0]->type == typeId)            
                              valueType =p->opr.op[0]->id.i->type;
                         else{
                             valueType = p->opr.op[0]->type;
                         } 
                     
                         if(valueType == TYPE_INT || valueType == typeInt)
                             myPStack.add(I_EQUAL);
                         else if (valueType == TYPE_FLOAT || valueType == typeFloat)
                             myPStack.add(R_EQUAL);  

                         ex(p->opr.op[0]);
                         ex(p->opr.op[1]);

                         if(valueType == TYPE_INT || valueType == typeInt)
                             myPStack.add(I_LESS);

                         else if (valueType == TYPE_FLOAT || valueType == typeFloat)
                             myPStack.add(R_LESS);

                         myPStack.add(I_OR);
                         return 0;
                        }
        case NE:        {
                         ex(p->opr.op[0]);
                         ex(p->opr.op[1]);
    
                         int valueType;
                         if(p->opr.op[0]->type == typeId)            
                              valueType =p->opr.op[0]->id.i->type;
                         else{
                             valueType = p->opr.op[0]->type;
                         } 
                     
                         if(valueType == TYPE_INT || valueType == typeInt)
                             myPStack.add(I_EQUAL);                         
                         else if (valueType == TYPE_FLOAT || valueType == typeFloat)
                             myPStack.add(R_EQUAL);  

                         myPStack.add(I_NOT);
                         return 0;                         
                        }
        case EQ:        {
                          ex(p->opr.op[0]);
                         ex(p->opr.op[1]);
    
                         int valueType;
                         if(p->opr.op[0]->type == typeId)            
                              valueType =p->opr.op[0]->id.i->type;
                         else{
                             valueType = p->opr.op[0]->type;
                         } 
                     
                         if(valueType == TYPE_INT || valueType == typeInt)
                             myPStack.add(I_EQUAL);                         
                         else if (valueType == TYPE_FLOAT || valueType == typeFloat)
                             myPStack.add(R_EQUAL);  
                         return 0;
                         }		
        }
    }
    return 0;
}
