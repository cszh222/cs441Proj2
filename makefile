GPP = g++
CXXFLAGS = -g
YACC = bison
LEX = flex
YYFLAGS = -d -o
LEXFLAGS = -I -o

all: calc3p

y.tab.o lex.yy.o: y.tab.cc lex.yy.cc
	$(GPP) $(CXXFLAGS) -c y.tab.cc lex.yy.cc

symbol_table.o: symbol_table.h symbol_table.cc
	$(GPP) $(CXXFLAGS) -c symbol_table.cc

pst.o: pstcode.h pstcode.cc
	$(GPP) $(CXXFLAGS) -c pstcode.cc -o pst.o

calc3p: y.tab.o lex.yy.o symbol_table.o pst.o
	$(GPP) $(CXXFLAGS) y.tab.o lex.yy.o symbol_table.o pst.o -o calc3p

y.tab.cc y.tab.h: calc3.y
	$(YACC) $(YYFLAGS) y.tab.cc calc3.y
	mv y.tab.hh y.tab.h

lex.yy.cc: calc3.l
	$(LEX) $(LEXFLAGS) lex.yy.cc calc3.l 

clean: 
	rm -rf y.tab.o y.tab.h y.tab.cc lex.yy.o lex.yy.cc symbol_table.o pst.o calc3p calc3p.amp calc3p.z.apm
