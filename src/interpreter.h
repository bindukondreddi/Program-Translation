/*

	Name: Adrian J. Paparelli
	Class: CSCI 5220
	Session: Spring 2016

	Description: interpreter.h contains all function definitions used by the SFL interpreter. 

	Change Log:
	2016-03-25: Initial Revision

*/

#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include "ast.h"

int runInterpreter(void);

AST performAction(AST t);

#endif
