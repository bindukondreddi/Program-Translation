

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
