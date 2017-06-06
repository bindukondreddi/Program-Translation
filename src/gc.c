
#include "ast.h"
#include "gc.h"
#include "allocate.h"
#include "symboltable.h"
#include <stdio.h>


AST freelist;
REMLIST headptr;
ASTTABLE astMem;

void abortCollect(void)
{
	printf("Error: Unable to allocate any more AST nodes\n");
	exit(-1);
}

REMLIST getMark(void)
{
	return headptr;
}

REMLIST addNewNode(REMLIST h, AST t)
{
	REMLIST ptr = NEW(REMEMBERLIST);
	ptr->tree = &t;
	ptr->next = h;
	return ptr;
}

void remember(AST t)
{
	headptr = addNewNode(headptr, t);
}

void forget(REMLIST inPtr)
{
	REMLIST ptr;
	while(headptr != inPtr)
	{
		ptr = headptr;
		headptr = headptr->next;
		free(ptr);
	}		
}

AST addFreeNode(AST list, AST node)
{
	AST tmpptr = node;
	tmpptr->fields.subtrees.s1 = list;
	return tmpptr;
}

void initialize_gc(void)
{
	int x;
	astMem = NEWARRAY(AST, AST_MEM_SIZE);

	for(x = 0; x < AST_MEM_SIZE; x++)
	{
		astMem[x] = NEW(ASTNODE);
		freelist = addFreeNode(freelist,astMem[x]);
	}
}

AST returnHead(void)
{
	AST tmpptr = freelist;
	freelist = freelist->fields.subtrees.s1;
	return tmpptr;
}

AST getNewAST()
{		
	return NEW(ASTNODE);
/*
	if(freelist == NULL)
	{
		int s = garbageCollect();
		if(s > 0)
		{
			return returnHead();
		}
		else
		{
			abortCollect();
			return NULL;
		}
	}
	else
		return returnHead();
*/
}

void mark(AST t)
{
	if(t == NULL)
		return;
	else
	{
	 	t->mark = 1;
	 	switch(t->kind)
	 	{
			case BRANCH_NK :
				mark(t->fields.subtrees.s1);
				mark(t->fields.subtrees.s2);
				mark(t->fields.subtrees.s3);
				break;
			case FUNC_NK :
				mark(t->fields.subtrees.s1);
				break;
			case BASIC_FUNC_NK :
				mark(t->fields.subtrees.s1);
				break;
			case COLON_NK :
				mark(t->fields.subtrees.s1);
				mark(t->fields.subtrees.s2);
				break;
			case CONS_NK :
				mark(t->fields.subtrees.s1);
				mark(t->fields.subtrees.s2);
				break;
			case OP_NK :
				mark(t->fields.subtrees.s1);
				mark(t->fields.subtrees.s2);
				break;
			case ACTION_NK :
				mark(t->fields.subtrees.s1);
				mark(t->fields.subtrees.s2);
				break;
			default :
				break;
		}
	}	
}

			
void markALL(AST t)
{
	mark(t);	
}

int sweepNodes()
{
	int i;
	int freeNodes = 0;
	for(i = 1; i < AST_MEM_SIZE; i++)
	{
		if(astMem[i]->mark == 0)
		{
			freelist = addFreeNode(freelist, astMem[i]);
			freeNodes++;
		}
		astMem[i] -> mark = 0;
	}			
	return freeNodes;
}

void markRemember(void)
{
	REMLIST ptr = headptr;
	while(ptr != NULL)
	{
		markALL(*ptr->tree);
		ptr = ptr->next;
	}		
}

int garbageCollect()
{
	int x;

	for(x = 0; x < AST_MEM_SIZE-1; x++)
	{
		if(astMem[x] != NULL)
			astMem[x]->mark = 0;
	} 
	markTable();	
	markRemember();
	x = sweepNodes();
	return x;
}
		
