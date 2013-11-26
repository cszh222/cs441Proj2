GCC = gcc 
GPP = g++
CXXFLAGS = -g  
YACC = bison
LEX = flex
YYFLAGS = -y -d
LEXFLAGS = -I

all: calc3

y.tab.o lex.yy.o: y.tab.c lex.yy.c
	$(GCC) $(CXXFLAGS) -c y.tab.c lex.yy.c

symbol_table.o: symbol_table.h symbol_table.cc
	$(GPP) $(CXXFLAGS) -c symbol_table.cc

calc3: y.tab.o lex.yy.o symbol_table.o
	$(GPP) $(CXXFLAGS) y.tab.o lex.yy.o symbol_table.o -o calc3

y.tab.c y.tab.h: calc3.y
	$(YACC) $(YYFLAGS) calc3.y

lex.yy.c: calc3.l
	$(LEX) $(LEXFLAGS) calc3.l

clean: 
	rm -rf y.tab.o y.tab.h y.tab.c lex.yy.o lex.yy.h symbol_table.o calc3
