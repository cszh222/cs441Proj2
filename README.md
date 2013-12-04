CS441 Final Project

Authors:
Adam Goedde
Chang Zheng

Milestone 1 complete.
C++ style variables
C++ style single and multiple lines
Symbol table using symbol_table.cc
Can declared int and double
do-while and repeat-until loops works

Milestone 2 complete.
During milestone 2, the build file was removed. In order to complile, make is required.
Flex and Bison files are now compiled into c++ files instead of c.
This was done to use the PStack class. c does not support classes.
PStack code generation for the following:
	expressions
	variables
	if statements
	while loop
	do-while loo
	repeat-until loop
The PStack code generated has only been tested manually.
Automated tests have not been run.

Can be compiled with the makefile.
Can be cleaned with make clean.

