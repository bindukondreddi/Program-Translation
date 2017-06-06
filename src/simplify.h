/*

	Name: Adrian J. Paparelli
	Class: CSCI 5220
	Session: Spring 2016

	Description: simplify.h contains all function definitions used by the SFL simplifier. 
	Function descriptions will follow to explain each funtion.

	Change Log:
	2016-03-25: Initial Revision

*/

#ifndef SIMPLIFY_H_
#define SIMPLIFY_H_

#include "ast.h"

typedef struct valnode {
	int oldval;
	int newval;
	struct valnode* next;
} VALNODE;

typedef VALNODE* VL;

AST getHead(AST t);

AST getTail(AST t);

AST simplify(AST t);

AST copyTree(AST r, AST s, int x);

AST copyAST(AST r, AST s, int x);

AST applyValue(AST r, AST s);

#endif
