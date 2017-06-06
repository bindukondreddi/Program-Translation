
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
