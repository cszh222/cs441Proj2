%{
#include <stdlib.h>
#include "calc3.h"
#include "y.tab.h"
#include <string>
void yyerror(std::string s);
extern int lineno;
%}

%%
">="            return GE;
"<="            return LE;
"=="            return EQ;
"!="            return NE;
"while"         return WHILE;
"if"            return IF;
"else"          return ELSE;
"print"         return PRINT;
"int"           return INT;
"double"        return DOUB;
"do"            return DO;
"repeat"        return REPEAT;
"until"         return UNTIL;
"begin"         return BG;
"end"           return ED;
"procedure"     return PROCEDURE;
"for"           return FOR;
"to"            return TO;
"step"          return STEP;
":="            return FORINIT;

[a-zA-Z_][a-zA-Z0-9_]* { 
                yylval.sVariable = strdup(yytext);
                return VARIABLE;
              }

[0][.][0]           {
                yylval.fValue = atof(yytext);
                return DOUBLE;
              }
[1-9][0-9]*[.][0-9][0-9]*      {
                              yylval.fValue = atof(yytext);
                              return DOUBLE;
                             }    

0             {
                yylval.iValue = atoi(yytext);
                return INTEGER;
              }

[1-9][0-9]*   {
                yylval.iValue = atoi(yytext);
                return INTEGER;
              }
[-()<>=+*/;{}.,] {
                return *yytext;
             }

[ \t\n]+        {
                    if(strcmp(yytext,"\n") == 0){
                      lineno++;
                    }
                }/* ignore whitespace */

.               yyerror("Unknown character");
%%
int yywrap(void) {
    return 1;
}
