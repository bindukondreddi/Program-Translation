#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "symboltable.h"
#include "simplify.h"
#include "gc.h"

VL headptr;

void abortProgram(char *s, AST t)
{
	printf("Runtime error: %s\n", s);
	displayAST(t);
	exit(-1);
}

AST getNewASTNode(void)
{
	//return NEW(ASTNODE);
	return getNewAST();
}

/* This function will return the head of a list, emptylist is checked before this
function is called */

AST getHead(AST t)
{
	AST s = simplify(t);
	if(s->kind == CONS_NK)
		return s->fields.subtrees.s1;
	else
	{
		abortProgram("Not a valid list",t);
		return NULL;
	}	
}

/* This function will return the tail of a list, emptylist is checked before this
function is called */

AST getTail(AST t)
{	
	AST s = simplify(t->fields.subtrees.s2);
	if((s->kind == CONS_NK) || (s->kind == EMPTYLIST))
		return s;
	else
	{
		abortProgram("Not a valid list",t);
		return NULL;
	}
}

/* The following 4 functions describe the means by which this simplifier saves and a
assigned the paramNode values in an AST.  I use a linked list to store the old and new values
during a tree traversal and copy. */


/* This function looks for the old paramNode value in the VALNODE list and returns the new
reassignment value, if the value does not exist a -1 is returned */

int getVal(int x)
{
	VL tmpptr = headptr;
	if(tmpptr == NULL)
		return -1;
	else
	{
		while(tmpptr != NULL)
		{
			if(tmpptr->oldval == x)
				return tmpptr->newval;
			else
				tmpptr = tmpptr->next;
		}
		return -1;
	}
}

/* This function creates a new VAL node to save the old and new paramNode id values */

VL newValNode(VL inPtr, int val)
{
	VL vn = NEW(VALNODE);
	vn->next = inPtr;
	vn->oldval = val;
	vn->newval = val + 1000;
	return vn;
}

/* This function scans the AST defined by r and looks for any paramNodes, if one is found the old and new values
are stored in a VALNODE and added to the VALLIST which headptr points to */

void scanTree(AST r)
{
	if(r != NULL)
	{
		if(r->kind == FUNC_NK)
		{
			if(getVal(r->extra) == -1)
			{
				headptr = newValNode(headptr, r->extra);
				scanTree(r->fields.subtrees.s1);
			}
		}
		else if(r->kind == OP_NK)
		{
			AST z = getTree(r->fields.stringval);
			scanTree(z);
		}
		else if((r->kind == BASIC_FUNC_NK) || (r->kind == OP_NK) || (r->kind == APPLY_NK)
			|| (r->kind == BRANCH_NK) || (r->kind == ACTION_NK) || (r->kind == COLON_NK)
			|| (r->kind == CONS_NK))
		{
			scanTree(r->fields.subtrees.s1);
			scanTree(r->fields.subtrees.s2);
			scanTree(r->fields.subtrees.s3);	
		}
	}
}

/* This function deletes the VALNODE list after a tree copy is complete. */

void deleteValList(VL ptr)
{
	if(ptr != NULL)
	{
		deleteValList(ptr->next);
		free(ptr);
	}
}	

/* This is the inital function called when a tree copy is invoked.  The function starts by scanning the tree R 
and generates a list of old/new values for any paramNodes encountered.  Once the scan is complete a copyAST will 
copy tree R and replace any paramNode with a value of x by tree S.  Copy AST will return the copied tree to t and
then the VALNODE list will be cleared for the next copy attempt.  The inital tree remains unaltered by this function.
*/

AST copyTree(AST r, AST s, int x)
{
	headptr = NULL;
	scanTree(r);
	AST t = copyAST(r,s,x);
	deleteValList(headptr);
	headptr = NULL;
	return t;
}

/* This function is initally called by copyTree and will recursivly generate a copy of the original tree defined by r. Any
param nodes with the value of x will be replaced by AST s. The original tree is left unaltered. */

AST copyAST(AST r, AST s, int x)
{
	if(r == NULL)
	{
		return NULL;
	}
	else if(r->kind == FUNC_NK)
	{
		int nv = getVal(r->extra);
		if(nv != -1)
		{
			AST z = getNewASTNode();			
			z->kind = r->kind;
			z->extra = nv;
			z->fields.subtrees.s1 = copyAST(r->fields.subtrees.s1,s,x);
			return z;
		}
		else
		{
			abortProgram("Invalid Function Value",r);
			return NULL;
		}
		
	}
	else if(r->kind == PARAM_NK)
	{
		if(r->fields.intval == x)		
			return s;
		else
		{
			int nv = getVal(r->fields.intval);
			if(nv != -1)
			{
				return applyParam(nv);
			}
			else
			{
				abortProgram("Parameter value does not exist",r);
				return NULL;
			}
		}
	}
	else if(r->kind == OP_NK)
	{
		AST z = getNewASTNode();		
		z->kind = r->kind;
		z->extra = r->extra;
		z->fields.subtrees.s1 = copyAST(r->fields.subtrees.s1,s,x);
		z->fields.subtrees.s2 = copyAST(r->fields.subtrees.s2,s,x);
		return z;		
	}
	else if(r->kind == NUMBER_NK)
	{
		AST z = getNewASTNode();	
		z->kind = r->kind;
		z->fields.intval = r->fields.intval;
		return z;
	}
	else if(r->kind == APPLY_NK)
	{
		AST z = getNewASTNode();
		z->kind = r->kind;
		z->fields.subtrees.s1 = copyAST(r->fields.subtrees.s1,s,x);
		z->fields.subtrees.s2 = copyAST(r->fields.subtrees.s2,s,x);
		return z;		
	}
	else if(r->kind == ID_NK)
	{		
		return r;
	}
	else if((r->kind == CHARCONST_NK))
	{
		AST z = getNewASTNode();
		z->kind = r->kind;
		z->fields.stringval = r->fields.stringval;
		return z;		
	}
	else if(r->kind == BRANCH_NK)
	{
		AST z = getNewASTNode();		
		z->kind = r->kind;
		z->fields.subtrees.s1 = copyAST(r->fields.subtrees.s1,s,x);
		z->fields.subtrees.s2 = copyAST(r->fields.subtrees.s2,s,x);
		z->fields.subtrees.s3 = copyAST(r->fields.subtrees.s3,s,x);
		return z;		
	}
	else if(r->kind == BASIC_FUNC_NK)
	{
		AST z = getNewASTNode();
		z->kind = r->kind;
		z->extra = r->extra;
		z->fields.subtrees.s1 = copyAST(r->fields.subtrees.s1,s,x);
		return z;		
	}
	else if((r->kind == COLON_NK) || (r->kind == CONS_NK) || (r->kind == ACTION_NK))
	{
		AST z = getNewASTNode();	
		z->kind = r->kind;
		z->extra = r->extra;
		z->fields.subtrees.s1 = copyAST(r->fields.subtrees.s1,s,x);
		z->fields.subtrees.s2 = copyAST(r->fields.subtrees.s2,s,x);
		return z;		
	}
	else if(r->kind == BOOL_NK)
	{
		AST z = getNewASTNode();
		z->kind = r->kind;
		z->extra = r->extra;
		return z;		
	}
	else if(r->kind == EMPTYLIST)
	{
		AST z = getNewASTNode();		
		z->kind = r->kind;
		return z;		
	}	
	else
		return NULL;
}

/* This function calls the copy function to apply AST s to any paramNode in the subtree of the current function, the replacement
value for the paramNode is defined the the present functions ID. */ 
	
AST applyValue(AST r, AST s)
{
	int i;
	i = r->extra;
	return copyTree(r->fields.subtrees.s1, s, i);
	
}

/* This is the simplify function defined by the SFL simplify project, this function is called by the interpreter to simplify the AST so
that the tree can be interpreted and relvent actions applied. A simplified tree is returned as specfied by the defintion on the class website
if there are any errors during the simplification an errorNode is returned to help define what the encountered error was*/

AST simplify(AST t)
{
	AST ret = NULL;
	REMLIST mark;
	if(t != NULL)
	{
		mark = getMark();
		if(t->kind == ID_NK)
			ret = simplify(getTree(t->fields.stringval));
		else if((t->kind == NUMBER_NK) || (t->kind == CHARCONST_NK)
			 || (t->kind == EMPTYLIST) || (t->kind == BOOL_NK))  
			ret = t; 
		else if(t->kind == CONS_NK)
			ret = t;
		else if(t->kind == COLON_NK)
		{	
			AST s = simplify(t->fields.subtrees.s1);
			remember(s);
			AST r = simplify(t->fields.subtrees.s2);
			remember(r);		
			ret = applyCONS(s, r);
		}
		else if(t->kind == BRANCH_NK)
		{
			AST s = simplify(t->fields.subtrees.s1);
			remember(s);
			if(s->kind == ERROR_NK)
				ret = errorNode(s->fields.stringval);
			else if(s->kind == BOOL_NK)
			{
				if(s->extra == BOOL_TRUE)
					ret = simplify(t->fields.subtrees.s2);
				else
					ret = simplify(t->fields.subtrees.s3);
			}
			else
				abortProgram("Conditional does not resolve into a boolean value",t);
		}
		else if(t->kind == OP_NK)
		{
			AST s = simplify(t->fields.subtrees.s1);
			remember(s);
			AST r = simplify(t->fields.subtrees.s2);
			remember(s);
				if(t->extra == PLUSOP_OK)
				{
					if((s->kind == NUMBER_NK) && (r->kind == NUMBER_NK))
					{
						int x = s->fields.intval + r->fields.intval;
						ret = numberNode(x);
					}
					else
						abortProgram("Operand is not the correct type, must be an integer",t);
				}
				else if(t->extra == SUBOP_OK)
				{
					if((s->kind == NUMBER_NK) && (r->kind == NUMBER_NK))
					{
						int x = s->fields.intval - r->fields.intval;
						ret = numberNode(x);
					}
					else
					{
						abortProgram("Operand is not the correct type, must be an integer",t);
					}
				}
				else if(t->extra == MULTOP_OK)
				{
					if((s->kind == NUMBER_NK) && (r->kind == NUMBER_NK))
					{
						int x = s->fields.intval * r->fields.intval;
						ret = numberNode(x);
					}
					else
					{
						abortProgram("Operand is not the correct type, must be an integer",t);
					}
				}
				else if(t->extra == DIVOP_OK)
				{
					if((s->kind == NUMBER_NK) && (r->kind == NUMBER_NK))
					{
						if(r->fields.intval == 0)
							abortProgram("Divide by zero error",t);
						else	
						{
							int x = s->fields.intval / r->fields.intval;
							ret = numberNode(x);
						}
					}
					else
					{
						abortProgram("Operand is not the correct type, must be an integer type",t);
					}
				}
				else if(t->extra == ANDOP_OK)
				{
					if((s->kind == BOOL_NK) && (r->kind == BOOL_NK))
					{
						if((s->extra == BOOL_TRUE) && (r->extra == BOOL_TRUE))
						{
							ret = boolNode("true");
						}
						else
						{
							ret = boolNode("false");
						}
					}
					else
					{
						abortProgram("Operand is not the correct type, must be a boolean type",t);
					}
				}
				else if(t->extra == OROP_OK)
				{
					if((s->kind == BOOL_NK) && (r->kind == BOOL_NK))
					{
						if((s->extra == BOOL_TRUE) || (r->extra == BOOL_TRUE))
						{
							ret = boolNode("true");
						}
						else
						{
							ret = boolNode("false");
						}
					}
					else
					{
						abortProgram("Operand is not the correct type, must be a boolean type",t);
					}
				}
				else if(t->extra == NOTOP_OK)
				{
					if(s->kind == BOOL_NK)
					{
						if(s->extra == BOOL_TRUE)
						{
							ret = boolNode("false");
						}
						else
						{
							ret = boolNode("true");
						}
					}
					else
					{
						abortProgram("Operand is not the correct type, must be a boolean type",t);
					}
				}
				else if(t->extra == GT_OK)
				{
					if((s->kind == NUMBER_NK) && (r->kind == NUMBER_NK))
					{
						if(s->fields.intval > r->fields.intval)
							ret = boolNode("true");
						else					
							ret = boolNode("false");
					}
					else
					{
						abortProgram("Operand is not the correct type, must be an integer",t);
					}
				}
				else if(t->extra == LT_OK)
				{
					if((s->kind == NUMBER_NK) && (r->kind == NUMBER_NK))
					{
						if(s->fields.intval < r->fields.intval)
							ret = boolNode("true");
						else
							ret = boolNode("false");
					}				
					else
					{
						abortProgram("Operand is not the correct type, must be an integer",t);
					}
				}
				else if(t->extra == EQ_OK)
				{
					if((s->kind == NUMBER_NK) && (r->kind == NUMBER_NK))
					{
						if(s->fields.intval == r->fields.intval)
							ret = boolNode("true");
						else
							ret = boolNode("false");
					}
					else if((s->kind == BOOL_NK) && (r->kind == BOOL_NK))
					{
						if(s->extra < r->extra)
							ret = boolNode("true");
						else
							ret = boolNode("false");
					}
					else if((s->kind == CHARCONST_NK) && (r->kind == CHARCONST_NK))
					{
						if(strcmp(s->fields.stringval,r->fields.stringval) == 0)
							ret = boolNode("true");
						else
							ret = boolNode("false");
					}
					else
					{
						abortProgram("Operand is not the correct type, must be an integer",t);
					}
				}
		}
		else if(t->kind == BASIC_FUNC_NK)
		{
			if(t->extra == ISINT_FK)
			{
				AST s = simplify(t->fields.subtrees.s1);
				remember(s);				
				if(s->kind == NUMBER_NK)
					ret = boolNode("true");
				else
					ret = boolNode("false");
			}
			else if(t->extra == ISBOOL_FK)
			{
				AST s = simplify(t->fields.subtrees.s1);
				remember(s);				
				if(s->kind == BOOL_NK)
					ret = boolNode("true");
				else
					ret = boolNode("false");
			}
			else if(t->extra == ISLIST_FK)
			{
				AST s = simplify(t->fields.subtrees.s1);
				remember(s);
				if((s->kind == CONS_NK) || (s->kind == EMPTYLIST))
					ret = boolNode("true");
				else
					ret = boolNode("false");
			}
			else if(t->extra == ISCHAR_FK)
			{
				AST s = simplify(t->fields.subtrees.s1);
				remember(s);
				if(s->kind == CHARCONST_NK)
					ret = boolNode("true");
				else
					ret = boolNode("false");
			}
			else if(t->extra == ISFUNC_FK)
			{
				AST s = simplify(t->fields.subtrees.s1);
				remember(s);
				if((s->kind == FUNC_NK) || (s->kind == BASIC_FUNC_NK))
					ret = boolNode("true");
				else
					ret = boolNode("false");
			}
			else if(t->extra == ISACTION_FK)
			{
				AST s = simplify(t->fields.subtrees.s1);
				remember(s);
				if(s->kind == ACTION_NK)				
					ret = boolNode("true");
				else
					ret = boolNode("false");
			}
			else if(t->extra == ISNULL_FK)
			{
				AST s = simplify(t->fields.subtrees.s1);
				remember(s);
				if(s->kind == EMPTYLIST)
					ret = boolNode("true");
				else
					ret = boolNode("false");
			}
			else if(t->extra == HEAD_FK)
			{
				AST s = simplify(t->fields.subtrees.s1);
				remember(s);
				if(s->kind == EMPTYLIST)
					abortProgram("Cannot return the head of an empty list",t);
				else if(s->kind != CONS_NK)
					abortProgram("Cannot return the head of an non-list",t);
				else
					ret = getHead(s);				
			}
			else if(t->extra == TAIL_FK)
			{
				AST s = simplify(t->fields.subtrees.s1);
				remember(s);
				if(s->kind == EMPTYLIST)
					abortProgram("Cannot return the tail of an empty list",t);
				else if(s->kind != CONS_NK)
					abortProgram("Cannot return the tail of an non-list",t);
				else
					ret = getTail(s);
			}
			else if((t->extra == READI_FK) || (t->extra == READC_FK))
			{
				ret = t;
			}
			else if((t->extra == PRINT_FK) || (t->extra == PRILST_FK) || (t->extra == PROD_FK))
			{
				ret = t;
			}			
		}
		else if(t->kind == ACTION_NK)
		{
			ret = t;
		}
		else if(t->kind == FUNC_NK)
		{
			ret = t;
		}
		else if(t->kind == APPLY_NK)
		{
				AST r = simplify(t->fields.subtrees.s1);
				remember(r);
				if(r->kind != FUNC_NK)
				{					
					abortProgram("Cannot apply a value to a non-funtion",t);
				}
				else
				{
					AST s = simplify(t->fields.subtrees.s2);
					remember(s);
					ret = simplify(applyValue(r,s));
				}
		}
		else
			abortProgram("Unknown Token Value",t);

	}
	remember(ret);
	forget(mark);
	return ret;

}
