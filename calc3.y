%{
#include <stdbool.h>
#include <iostream>
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
nodeType *doub(float value);
nodeType *newId(const char* i);
nodeType *declareProc(nodeType *p);
nodeType *declareDoub(nodeType *p);
nodeType *declareInt(nodeType *p);
void freeNode(nodeType *p);
int ex(nodeType *p);
int yylex(void);
bool checkOpError(int op1, int op2);
bool errorFound;
bool declaringNewVar;
int errToken;

std::string typeOpError = "Operating between int and float values";

int progBlkLvl;
int lineno;
PstackCode myPStack;

void yyerror(std::string s);
/*int sym[26]; */                   /* symbol table */
%}

%union {
    int iValue;                 /* integer value */
    float fValue;               /*float value*/
    char* sVariable;            /* identifier name */
    nodeType *nPtr;             /* node pointer */
};

%token <iValue> INTEGER
%token <sVariable> VARIABLE
%token <fValue> DOUBLE
%token WHILE IF PRINT DO REPEAT UNTIL INT DOUB PROCEDURE FOR TO STEP CALL
%token BG ED 
%nonassoc IFX
%nonassoc ELSE

%left GE LE EQ NE '>' '<'
%left '+' '-'
%left '*' '/'
%nonassoc UMINUS

%type <nPtr> stmt expr stmt_list declaration variable blk procedure call
%%

program:
        function                {}
        ;

function:
          function stmt         { ex($2); freeNode($2);}
        | /* NULL */
        ;

stmt:
          ';'                            { $$ = opr(';', 2, NULL, NULL); }
        | procedure  {pushSymbolTable();} 
            '{'  stmt_list '}' 
            { $$ = opr(PROCEDURE, 2, $1, $4);}
        | call ';'                       { $$ = opr(CALL, 1, $1);}                    
        | blk                            { $$ = $1;}
        | expr ';'                       { $$ = $1;}                    
        | PRINT expr ';'                 { $$ = opr(PRINT, 1, $2); }          
        | VARIABLE '=' expr ';'          { $$ = opr('=', 2, id($1), $3); }
        | WHILE '(' expr ')' stmt        { $$ = opr(WHILE, 2, $3, $5); }
        | IF '(' expr ')' stmt %prec IFX { $$ = opr(IF, 2, $3, $5); }
        | IF '(' expr ')' stmt ELSE stmt { $$ = opr(IF, 3, $3, $5, $7); }
        | '{' stmt_list '}'              { $$ = $2;}        
        | declaration ';'                { $$ = $1;}                
        | DO stmt WHILE '(' expr ')'     { $$ = opr(DO, 2, $2, $5);}
        | REPEAT stmt UNTIL '(' expr ')' { $$ = opr(REPEAT, 2, $2, $5);}
        | FOR '(' VARIABLE '=' expr STEP expr TO expr ')' stmt
            { $$ = opr(FOR, 5, id($3), $5, $7, $9, $11); }
        | call { errToken = yychar;} error '\n' /*error productions*/
        | expr { errToken = yychar;} error '\n'
        | '{' stmt_list  { errToken = yychar;} error '\n'  
        | declaration { errToken = yychar;} error '\n'  
        | PRINT expr { errToken = yychar;} error '\n'
        | VARIABLE '=' expr { errToken = yychar;} error '\n'   
        | '\n' {lineno++;} 
        ;        

blk:    {pushSymbolTable(); progBlkLvl = getCurrentLevel();} 
            BG stmt_list ED 
            {$$ = opr(BG, 1, $3); progBlkLvl--;}
        ;
procedure: PROCEDURE VARIABLE '(' ')' {$$ = declareProc(newId($2)); }
        ;

call: VARIABLE '(' ')'  {$$ = id($1);}
        ;


declaration:   INT variable            {$$ = opr(INT, 1, declareInt($2));}
            | DOUB variable            {$$ = opr(DOUB, 1, declareDoub($2));}
            | INT { errToken = yychar;} error '\n'          
            | DOUB { errToken = yychar;} error '\n'         
            ;

variable: VARIABLE '=' expr ',' variable {$$ = opr(',', 2, opr('=', 2, newId($1), $3), $5);}
        | VARIABLE  '=' expr             {$$ = opr('=', 2, newId($1), $3);}
        | VARIABLE ',' variable          {$$ = opr(',', 2, newId($1), $3);}
        | VARIABLE                       {$$ = newId($1);}
        | VARIABLE { errToken = yychar;} error '\n'
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
        | '(' { errToken = yychar;} error ')'
        ;

%%

#define SIZEOF_NODETYPE ((char *)&p->con - (char *)p)


nodeType *declareProc(nodeType *p){
    symbol_entry *entry = p->id.i;
    entry->type = TYPE_PROC;
    return p;
    
}

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

nodeType *doub(float value){
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
    if((entry=getSymbolEntry(i)) == NULL)
        yyerror("missing declaration for identifier");

    if(entry->blk_level < progBlkLvl ){
        symbol_entry *new_entry;
        new_entry = newId(entry->name)->id.i;
        new_entry->type = entry->type;
        if(entry->type == TYPE_INT)
            new_entry->iVal = entry->iVal;
        else if(entry->type == TYPE_INT)
            new_entry->fVal = entry->fVal;
        
        new_entry->relentry = entry;
        new_entry->scoped = true;
        new_entry->prev_type = entry->type;
        entry = new_entry;
    }

    if(progBlkLvl < getCurrentLevel())
        entry->refetch = true;

    nodeType *p;
    size_t nodeSize;

    nodeSize = SIZEOF_NODETYPE + sizeof(idNodeType);
    if ((p = (nodeType*)malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    p->type = typeId;
    p->id.i = entry;

    return p;
}

nodeType *newId(const char* i){
    symbol_entry* newEntry = NULL;
    nodeType *p;
    size_t nodeSize;

    //printSymbolTable();
    if(getSymbolEntry(i)!=0){
        if(getSymbolEntry(i)->blk_level == progBlkLvl && !getSymbolEntry(i)->scoped){
            char error[100]; /*assuming noone will declare variables that nears 100 character*/
            snprintf(error, 100, "Variable already declared \'%s\'", i);
            yyerror(std::string(error));
        }
        else if(getSymbolEntry(i)->blk_level == progBlkLvl && getSymbolEntry(i)->scoped){
            newEntry = getSymbolEntry(i);
        }
    }
    if(newEntry == NULL){
        newEntry = (symbol_entry*)malloc(sizeof(symbol_entry));
        newEntry->name = strdup(i);
        newEntry->size = 1; //this is used to count the number of symbols
        newEntry->relentry = NULL;
        newEntry->prev_type = -1;
        addSymbol(newEntry, lineno);
    }

    newEntry->refetch = false;
    newEntry->scoped = false;

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

bool checkOpError(int op1, int op2){
    if(op1 == TYPE_INT|| op1 == typeInt)
        return !(op2 == TYPE_INT || op2 == typeInt);
    if(op1 == TYPE_FLOAT || op1 == typeFloat)
        return !(op2 == TYPE_FLOAT || op2 == typeFloat);
}

void yyerror(std::string s) {
    std::string tokenName;
    if(errToken != -1 && errToken != YYEMPTY && s=="syntax error"){
        switch(errToken){
        case INTEGER: tokenName = "CONSTANT"; break;
        case DOUBLE: tokenName = "FLOAT"; break;
        case VARIABLE: tokenName = "IDENTIFIER"; break;
        case WHILE: tokenName = "WHILE"; break;
        case IF: tokenName = "IF"; break;
        case PRINT: tokenName = "PRINT"; break;
        case DO: tokenName = "DO"; break;
        case REPEAT: tokenName = "REPEAT"; break;
        case UNTIL: tokenName = "UNTIL"; break;
        case INT: tokenName = "INTTYPE"; break;
        case DOUB: tokenName = "FLOATTYPE"; break;
        case PROCEDURE: tokenName = "PROCEDURE"; break;
        case FOR: tokenName = "FOR"; break;
        case TO: tokenName = "TO"; break;
        case BG: tokenName = "BEGINBLOCK"; break;
        case ED: tokenName = "ENDBLOCK"; break;
        case IFX: tokenName = "IF"; break;
        case ELSE: tokenName = "ELSE"; break;
        case GE: tokenName = ">="; break;
        case LE: tokenName = "<="; break;
        case EQ: tokenName = "=="; break;
        case NE: tokenName = "!="; break;
        case UMINUS: tokenName = "-"; break;
        case '+': tokenName = "+"; break;
        case '-': tokenName = "-"; break;
        case '*': tokenName = "*"; break;
        case '/': tokenName = "/"; break;
        case '>': tokenName = ">"; break;
        case '<': tokenName = "<"; break;
        case '=': tokenName = "="; break;
        }   
        errToken = -1;
        printf("syntax error, unexpected %s near line %d\n", tokenName.c_str(), lineno);
        fflush(stdout);     
    }
    else{
        printf("%s near line %d\n", s.c_str(),lineno);
        fflush(stdout); 
    }
    errorFound = true;    
}

int main(void) {
    pushSymbolTable();
    lineno = 1;
    progBlkLvl = getCurrentLevel();
    errorFound = false;
    declaringNewVar = false;
    myPStack.begin_prog();
    yyparse();
    myPStack.end_prog(getCurrentSymbolTableSize());
    if(!errorFound)
        myPStack.write("calc_out.apm", 1);
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
                        if(p->id.i->relentry != NULL){
                            symbol_entry *entry = p->id.i->relentry;
                            if(entry->type == TYPE_INT){
                                myPStack.add(I_VARIABLE);
                                myPStack.add(0);
                                myPStack.add(p->id.i->offset);
                                
                                myPStack.add(I_VARIABLE);
                                myPStack.add(p->id.i->blk_level-entry->blk_level);
                                myPStack.add(entry->offset);                   

                                myPStack.add(I_VALUE);
                                myPStack.add(I_ASSIGN);
                                myPStack.add(1);
                            }
                            else if(entry->type == TYPE_FLOAT){
                                myPStack.add(R_VARIABLE);
                                myPStack.add(0);
                                myPStack.add(p->id.i->offset);
                                
                                myPStack.add(R_VARIABLE);
                                myPStack.add(p->id.i->blk_level-entry->blk_level);
                                myPStack.add(entry->offset);                   

                                myPStack.add(R_VALUE);
                                myPStack.add(R_ASSIGN);
                                myPStack.add(1);
                            }
                            p->id.i->relentry = NULL;
                        }

                        symbol_entry *curVar;
                        if(p->id.i->refetch) 
                            curVar = getSymbolEntry(p->id.i->name);
                        else
                            curVar = p->id.i;

                        int varType;
                        if(curVar->prev_type != -1)
                            varType = curVar->prev_type;
                        else
                            varType = curVar->type;
                        
                        if (varType == TYPE_INT){

                            myPStack.add(I_VARIABLE);
                            myPStack.add(0);
                            myPStack.add(curVar->offset);
			                myPStack.add(I_VALUE);
                 
                            return TYPE_INT;
                        }
                        else if (varType == TYPE_FLOAT){
                                                      
                            myPStack.add(R_VARIABLE);
                            myPStack.add(0);
                            myPStack.add(curVar->offset);
                            myPStack.add(R_VALUE);

                            return TYPE_FLOAT;
                        }
                        }
    case typeOpr:
        switch(p->opr.oper) {
        case FOR:       {   symbol_entry *entry = p->opr.op[0]->id.i;
                            myPStack.add(I_VARIABLE);
                            myPStack.add(0);
                            myPStack.add(entry->offset);
                            
                            ex(p->opr.op[1]);
                            
                            myPStack.add(I_ASSIGN);
                            myPStack.add(1);

                            int initpos = myPStack.pos();

                            myPStack.add(I_VARIABLE);
                            myPStack.add(0);
                            myPStack.add(entry->offset);
                            myPStack.add(I_VALUE);

                            ex(p->opr.op[3]);

                            myPStack.add(I_EQUAL);
                            myPStack.add(I_JMP_IF_TRUE);
                            int comparepos = myPStack.pos();
                            myPStack.add(0);

                            ex(p->opr.op[4]);

                            myPStack.add(I_VARIABLE);
                            myPStack.add(0);
                            myPStack.add(entry->offset);

                            myPStack.add(I_VARIABLE);
                            myPStack.add(0);
                            myPStack.add(entry->offset);
                            myPStack.add(I_VALUE);

                            ex(p->opr.op[2]);

                            myPStack.add(I_ADD);
                            myPStack.add(I_ASSIGN);
                            myPStack.add(1);

                            myPStack.add(I_JMP);
                            myPStack.add(initpos);

                            myPStack.at(comparepos) = myPStack.pos();  

                            ex(p->opr.op[4]);

                            myPStack.add(I_VARIABLE);
                            myPStack.add(0);
                            myPStack.add(entry->offset);

                            myPStack.add(I_VARIABLE);
                            myPStack.add(0);
                            myPStack.add(entry->offset);
                            myPStack.add(I_VALUE);

                            ex(p->opr.op[2]);

                            myPStack.add(I_ADD);
                            myPStack.add(I_ASSIGN);
                            myPStack.add(1);

                            return 0;                          

                        }
        case PROCEDURE: {
                        symbol_entry* entry = p->opr.op[0]->id.i;
                        myPStack.add(I_JMP);
                        myPStack.add(0);
                        int jmpPos = myPStack.pos()-1;

                        entry->pAddr = myPStack.pos();

                        myPStack.begin_proc();
                        int patch_loc = myPStack.pos()-2;
                        ex(p->opr.op[1]);
                        myPStack.end_proc(patch_loc, getCurrentSymbolTableSize());

                        myPStack.at(jmpPos) = myPStack.pos();

                        popSymbolTable();

                        return 0;
                        }
        case CALL:      {
                        symbol_entry* entry = p->opr.op[0]->id.i;
                        myPStack.add(I_CALL);
                        myPStack.add(entry->blk_level);
                        myPStack.add(entry->pAddr);

                        return 0;
                        }
        case BG:        {
                        myPStack.add(I_JMP);
                        myPStack.add(0);
                        int jmpPos =  myPStack.pos()-1;
                        myPStack.begin_proc();
                        int patch_loc = myPStack.pos()-2;
                        ex(p->opr.op[0]);
                        myPStack.end_proc(patch_loc, getCurrentSymbolTableSize());
                        
                        myPStack.at(jmpPos) = myPStack.pos();
                        myPStack.add(I_CALL);
                        myPStack.add(0);
                        myPStack.add(patch_loc-1);
                        popSymbolTable();

                        return 0;
                        }
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
        case INT:       declaringNewVar = true;
                        if(p->opr.op[0]->type == typeOpr)   
                            ex(p->opr.op[0]);
                        else
                            p->opr.op[0]->id.i->prev_type = -1;
                        declaringNewVar = false;  
                        return 0;
        case DOUB:      declaringNewVar = true;
                        if(p->opr.op[0]->type == typeOpr)
                            ex(p->opr.op[0]); 
                        else
                            p->opr.op[0]->id.i->prev_type = -1;
                        declaringNewVar = false;  
                        return 0;
        case ',':       if(p->opr.op[0]->type == typeOpr)
                            ex(p->opr.op[0]);
                        else
                            p->opr.op[0]->id.i->prev_type = -1;
                        if(p->opr.op[1]->type == typeOpr)
                            ex(p->opr.op[1]);
                        else
                            p->opr.op[1]->id.i->prev_type = -1;
                        return 0;
        case '=':       {
                         if(declaringNewVar)
                            p->opr.op[0]->id.i->prev_type = -1;
                         
                         int valType;
                         if(p->opr.op[0]->id.i->prev_type != -1)
                            valType = p->opr.op[0]->id.i->prev_type;
                         else
                           valType =  p->opr.op[0]->id.i->type;

			             if (valType == TYPE_INT){
                            myPStack.add(I_VARIABLE);
			                myPStack.add(0);
			                myPStack.add(p->opr.op[0]->id.i->offset);
			             
			                int valueType = ex(p->opr.op[1]);
                            if(valueType != TYPE_INT && valueType != typeInt)
                                yyerror("Assigning float into an int variable");
                                                      
                            myPStack.add(I_ASSIGN);
                            myPStack.add(1);
                           
                            p->opr.op[0]->id.i->relentry = NULL;
                            return TYPE_INT;
                        }
                        else if (valType == TYPE_FLOAT){
                            myPStack.add(R_VARIABLE);
			                myPStack.add(0);
			                myPStack.add(p->opr.op[0]->id.i->offset);
                            
                            int valueType = ex(p->opr.op[1]);
                            if(valueType != TYPE_FLOAT && valueType != typeFloat)
                                yyerror("Assigning int into an float variable");                           
                            
                            myPStack.add(R_ASSIGN);
                            myPStack.add(1);
                            
                            p->opr.op[0]->id.i->relentry = NULL;
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
                        
                        myPStack.add(I_JMP);
                        myPStack.add(0);
                        int nextAddr = myPStack.pos();

                        myPStack.at(initAddr-1) = myPStack.pos();
                        if (p->opr.nops > 2)
                            ex(p->opr.op[2]);

                        myPStack.at(nextAddr-1) = myPStack.pos();

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
                         int valueType2 = ex(p->opr.op[1]);  

                         if(checkOpError(valueType, valueType2))
                            yyerror(typeOpError);                     
                                                               
                         if(valueType == TYPE_INT || valueType == typeInt)
                             myPStack.add(I_ADD);
                         else if (valueType == TYPE_FLOAT || valueType == typeFloat)
                             myPStack.add(R_ADD);  

                         return valueType;
                        }
        case '-':       {
                         int valueType = ex(p->opr.op[0]);
                         int valueType2 = ex(p->opr.op[1]); 
         
                         if(checkOpError(valueType, valueType2))
                            yyerror(typeOpError);
                            
                         if(valueType == TYPE_INT || valueType == typeInt)
                             myPStack.add(I_SUBTRACT);
                         else if (valueType == TYPE_FLOAT || valueType == typeFloat)
                             myPStack.add(R_SUBTRACT);  

                         return valueType;
                         }
        case '*':       {
                         int valueType = ex(p->opr.op[0]);
                         int valueType2 = ex(p->opr.op[1]); 
         
                         if(checkOpError(valueType, valueType2))
                            yyerror(typeOpError);
                            
                         if(valueType == TYPE_INT || valueType == typeInt)
                             myPStack.add(I_MULTIPLY);
                         else if (valueType == TYPE_FLOAT || valueType == typeFloat)
                             myPStack.add(R_MULTIPLY);  

                         return valueType;
                         }
        case '/':       {
                         int valueType = ex(p->opr.op[0]);
                         int valueType2 = ex(p->opr.op[1]); 
         
                         if(checkOpError(valueType, valueType2))
                            yyerror(typeOpError);
                            
                         if(valueType == TYPE_INT || valueType == typeInt)
                             myPStack.add(I_DIVIDE);
                         else if (valueType == TYPE_FLOAT || valueType == typeFloat)
                             myPStack.add(R_DIVIDE);  

                         return valueType;
                         }
        case '<':       {
                         int valueType = ex(p->opr.op[0]);
                         int valueType2 = ex(p->opr.op[1]); 
         
                         if(checkOpError(valueType, valueType2))
                            yyerror(typeOpError);
                            
                         if(valueType == TYPE_INT || valueType == typeInt)
                             myPStack.add(I_LESS);
                         else if (valueType == TYPE_FLOAT || valueType == typeFloat)
                             myPStack.add(R_GREATER);  

                         return 0;
                         }
        case '>':       {
                         int valueType = ex(p->opr.op[0]);
                         int valueType2 = ex(p->opr.op[1]); 
         
                         if(checkOpError(valueType, valueType2))
                            yyerror(typeOpError);
                           
                         if(valueType == TYPE_INT || valueType == typeInt)
                             myPStack.add(I_GREATER);
                         else if (valueType == TYPE_FLOAT || valueType == typeFloat)
                             myPStack.add(R_GREATER);  

                         return 0;
                         }
        case GE:        {
                         int valueType = ex(p->opr.op[0]);
                         int valueType2 = ex(p->opr.op[1]); 
         
                         if(checkOpError(valueType, valueType2))
                            yyerror(typeOpError);
                            
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
                        int valueType = ex(p->opr.op[0]);
                         int valueType2 = ex(p->opr.op[1]); 
         
                         if(checkOpError(valueType, valueType2))
                            yyerror(typeOpError);                    
                         
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
                         int valueType = ex(p->opr.op[0]);
                         int valueType2 = ex(p->opr.op[1]); 
         
                         if(checkOpError(valueType, valueType2))
                            yyerror(typeOpError);
                     
                         if(valueType == TYPE_INT || valueType == typeInt)
                             myPStack.add(I_EQUAL);                         
                         else if (valueType == TYPE_FLOAT || valueType == typeFloat)
                             myPStack.add(R_EQUAL);  

                         myPStack.add(I_NOT);
                         return 0;                         
                        }
        case EQ:        {
                         int valueType = ex(p->opr.op[0]);
                         int valueType2 = ex(p->opr.op[1]); 
         
                         if(checkOpError(valueType, valueType2))
                            yyerror(typeOpError);
                            
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