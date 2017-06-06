/*

	Name: Adrian J. Paparelli
	Class: CSCI 5220
	Session: Spring 2016

	Description: gc.h contains all headers for function used by the garbage collector. 
	

	Change Log:
	2016-04-27: Initial Revision

*/

#ifndef GC_H_
#define GC_H_
#include "ast.h"

#define AST_MEM_SIZE 10000

typedef struct rememberlist {
	ASTNODE **tree;
	struct rememberlist* next;
}
REMEMBERLIST;

typedef REMEMBERLIST* REMLIST;



void initialize_gc(void);

AST getNewAST(void);

int garbageCollect(void);

void markALL(AST t);

REMLIST getMark(void);

void remember(AST t);

void forget(REMLIST inPtr);

#endif
