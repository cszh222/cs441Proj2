%{
#include <stdlib.h>
#include "calc3.h"
#include "y.tab.h"
#include <string.h>
void yyerror(char *);
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
"int"           return INTEGER;
"double"        return DOUBLE;
"do"            return DO;
"repeat"        return REPEAT;
"until"         return UNTIL;

[a-zA-Z_][a-zA-Z0-9_]+ { 
                yylval.sVariable = strdup(yytext);
                return VARIABLE;
              }

0             {
                yylval.iValue = atoi(yytext);
                return INTEGER;
              }

[1-9][0-9]*   {
                yylval.iValue = atoi(yytext);
                return INTEGER;
              }
0.0           {
                yylval.fValue = atof(yytext);
                return DOUBLE;
              }

[-()<>=+*/;{}.,] {
                return *yytext;
             }

[ \t\n]+        {
                    if(strcmp(yytext,"\n") == 0){
                        printf("%d", lineno);
                        lineno++;
                    }
                }/* ignore whitespace */

.               yyerror("Unknown character");
%%
int yywrap(void) {
    return 1;
}
