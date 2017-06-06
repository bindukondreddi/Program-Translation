
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ast.h"
#include "stringtable.h"
#include "allocate.h"
#include "gc.h"

AST getNewNode(void)
{
	//return NEW(ASTNODE);
	return getNewAST();
}

/* Return boolean name based off the value of the incoming integer definition */
char* getBoolName(const int i)
{
	if(i == BOOL_TRUE)
		return "true";
	else if(i == BOOL_FALSE)
		return "false";
	else	
		return NULL;
}

/* Return boolean definition based off the value of the incoming boolean string */
int getBoolDef(const char* s)
{
	if(strcmp(s,"true") == 0)
		return BOOL_TRUE;
	else if(strcmp(s,"false") == 0)
		return BOOL_FALSE;
	else
		return -1;
}

/* Return operator name based off the value of the incoming integer definition */
char* getOperatorName(const int i)
{
	if(i == PLUSOP_OK)
		return "+";
	else if(i == SUBOP_OK)
		return "-";
	else if(i == MULTOP_OK)
		return "*";
	else if(i == DIVOP_OK)
		return "/";
	else if(i == GT_OK)
		return ">";
	else if(i == LT_OK)
		return "<";
	else if(i == EQ_OK)
		return "==";
	else if(i == ANDOP_OK)
		return "and";
	else if(i == OROP_OK)
		return "or";
	else if(i == NOTOP_OK)
		return "not";
	else
		return NULL;
}

/* Return operator definition based off the value of the incoming operator string */
int getOperatorDef(const char* c)
{
	if(strcmp(c,"+") == 0)
		return PLUSOP_OK;
	else if(strcmp(c,"-") == 0)
		return SUBOP_OK;
	else if(strcmp(c,"*") == 0)
		return MULTOP_OK;
	else if(strcmp(c,"/") == 0)
		return DIVOP_OK;
	else if(strcmp(c,">") == 0)
		return GT_OK;
	else if(strcmp(c,"<") == 0)
		return LT_OK;
	else if(strcmp(c,"==") == 0)
		return EQ_OK;
	else if(strcmp(c,"and") == 0)
		return ANDOP_OK;
	else if(strcmp(c,"or") == 0)
		return OROP_OK;
	else if(strcmp(c,"not") == 0)
		return NOTOP_OK;
	else
		return -1;
}

/* Return function name based off the value of the incoming integer definition */
char* getFunctionName(const int n)
{
	if(n == ISNULL_FK)
		return "isNull";
	else if(n == ISFUNC_FK)
		return "isFunc";
	else if(n == ISINT_FK)
		return "isInt";
	else if(n == ISBOOL_FK)
		return "isBool";
	else if(n == ISACTION_FK)
		return "isAction";
	else if(n == ISCHAR_FK)
		return "isChar";
	else if(n == ISLIST_FK)
		return "isList";
	else if(n == HEAD_FK)
		return "head";
	else if(n == TAIL_FK)
		return "tail";
	else if(n == PRINT_FK)
		return "print";
	else if(n == PRILST_FK)
		return "printList";
	else if(n == PROD_FK)
		return "produce";
	else if(n == READC_FK)
		return "readChar";
	else if(n == READI_FK)
		return "readInt";
	else
		return NULL;
}

/* Return function definition based off the value of the incoming function string */
int getFunctionDef(const char* s)
{	
	if(strcmp(s,"isNull") == 0)
		return ISNULL_FK;
	else if(strcmp(s,"ifFunc") == 0)
		return ISFUNC_FK;
	else if(strcmp(s,"ifInt") == 0)
		return ISINT_FK;
	else if(strcmp(s,"isBool") == 0)
		return ISBOOL_FK;
	else if(strcmp(s,"isAction") == 0)
		return ISACTION_FK;
	else if(strcmp(s,"isChar") == 0)
		return ISCHAR_FK;
	else if(strcmp(s,"head") == 0)
		return HEAD_FK;
	else if(strcmp(s,"isList") == 0)
		return ISLIST_FK;
	else if(strcmp(s,"tail") == 0)
		return TAIL_FK;
	else if(strcmp(s,"print") == 0)
		return PRINT_FK;
	else if(strcmp(s,"printList") == 0)
		return PRILST_FK;
	else if(strcmp(s,"produce") == 0)
		return PROD_FK;
	else if(strcmp(s,"readChar") == 0)
		return READC_FK;
	else if(strcmp(s,"readInt") == 0)
		return READI_FK;
	else
		return -1;
}


/* Constructor for the number Node AST */

AST numberNode(const int n)
{
	AST t = getNewNode();
	t->kind = NUMBER_NK;
	t->fields.intval = n;
	return t;
}

/* Constructor for the ID Node AST, saves ID name in intern */

AST idNode(const char* s)
{
	AST t = getNewNode();
	t->kind = ID_NK;
	t->fields.stringval = intern(s);
	return t;
}

/* Constructor for the Const Char AST, saves Char name in intern */

AST charNode(const char* s)
{
	AST t = getNewNode();
	t->kind = CHARCONST_NK;
	t->fields.stringval = intern(s);
	return t;
}

/* Constructor for boolean Node AST, accepts const char * (ie: "true"/"false") */

AST boolNode(const char* s)
{
	AST t = getNewNode();
	t->kind = BOOL_NK;
	t->extra = getBoolDef(s);
	return t;
}

/* Constructor for Parameter Node AST, accepts int n to define associated function number */

AST applyParam(const int n)
{
	AST t = getNewNode();
	t->kind = PARAM_NK;
	t->fields.intval = n;
	return t;
}

/* Constructor for an EMPTYLIST AST */

AST emptyList(void)
{
	AST t = getNewNode();
	t->kind = EMPTYLIST;
	return t;
}

AST errorNode(const char* s)
{
	AST t = getNewNode();
	t->kind = ERROR_NK;
	t->fields.stringval = intern(s);
	return t;
}

/* Constructor for apply AST, defines APPLY NK to apply AST A, to AST B */

AST applyNode(AST A, AST B)
{
	AST t = getNewNode();
	t->kind = APPLY_NK;
	t->fields.subtrees.s1 = A;
	t->fields.subtrees.s2 = B;	
	return t;
}

/* Constructor for apply Op AST, defines OP_NK AST accepts AST A, AST B and char * opkind(ie: "*","/","and", "==" */

AST applyOp(AST A, AST B, const char* opKind)
{
	AST t = getNewNode();
	t->kind = OP_NK;
	t->extra = getOperatorDef(opKind);	
	t->fields.subtrees.s1 = A;
	if(t->extra == NOTOP_OK)
		t->fields.subtrees.s2 = emptyList();
	else
		t->fields.subtrees.s2 = B;
	return t;
}

/* Constructor for a COLON AST, used to generate a list accepting AST A, and AST B as input. */

AST applyCOLON(AST A, AST B)
{
	AST t = getNewNode();
	t->kind = COLON_NK;
	t->fields.subtrees.s1 = A;
	t->fields.subtrees.s2 = B;
	return t;
}

/* Constructor for a CONS AST, used to generate a list accepting AST A, and AST B as input. */

AST applyCONS(AST A, AST B)
{
	AST t = getNewNode();
	t->kind = CONS_NK;
	t->fields.subtrees.s1 = A;
	t->fields.subtrees.s2 = B;
	return t;
}

/* Constructor for a Branch AST, used to generate a list accepting AST A, and AST B, AST C as input. 
	AST A: represents the if conditional
	AST B: represents the then action
	AST C: represents the else result if applicable.
*/

AST applyBranch(AST A, AST B, AST C)
{
	AST t = getNewNode();
	t->kind = BRANCH_NK;
	t->fields.subtrees.s1 = A;
	t->fields.subtrees.s2 = B;
	t->fields.subtrees.s3 = C;
	return t;
}

/* Constructor for a basic function AST, applies AST A to the function defined by const char* FuncKind. */

AST applyBasicFunc(AST A, const char* FuncKind)
{
	AST t = getNewNode();
	t->kind = BASIC_FUNC_NK;
	t->extra = getFunctionDef(FuncKind);
	t->fields.subtrees.s1 = A;
	return t;
}

/* Constructor for a Action AST */

AST applyAction(AST A, AST B)
{
	AST t = getNewNode();
	t->kind = ACTION_NK;
	t->fields.subtrees.s1 = A;
	t->fields.subtrees.s2 = B;
	return t;
}

/* Constructor for a defined function AST, assigns AST A to the function # defined by const int n. */

AST applyFunction(AST A, const int n, const char* s)
{
	AST t = getNewNode();
	t->kind = FUNC_NK;
	t->extra = n;
	t->fields.subtrees.s1 = A;
	replaceID(s, n, t->fields.subtrees.s1);
	return t;
}


/* Function to replace the ID nodes in AST tree E with stringval = s to a paramNode with intval n */

AST replaceID(const char* s, const int n, AST E)
{
	if(E->kind == ID_NK) 
	{
		if(strcmp(s,E->fields.stringval) == 0)
			return applyParam(n);
		else
			return E;
	}
	else if((E->kind == BASIC_FUNC_NK) || (E->kind == FUNC_NK))
	{		
		E->fields.subtrees.s1 = replaceID(s, n, E->fields.subtrees.s1);
		return E;
	}
	else if((E->kind == COLON_NK) || (E->kind == OP_NK) || (E->kind == CONS_NK) ||
		(E->kind == APPLY_NK) || (E->kind == ACTION_NK))
	{
		E->fields.subtrees.s1 = replaceID(s, n, E->fields.subtrees.s1);
		E->fields.subtrees.s2 = replaceID(s, n, E->fields.subtrees.s2);
		return E;
	}
	else if(E->kind == BRANCH_NK)
	{
		E->fields.subtrees.s1 = replaceID(s, n, E->fields.subtrees.s1);
		E->fields.subtrees.s2 = replaceID(s, n, E->fields.subtrees.s2);
		E->fields.subtrees.s3 = replaceID(s, n, E->fields.subtrees.s3);
		return E;
	}
	else
		return E;
		
}

/* Function to display AST tree A, indent defines the level that the tree is at to define display indent */

void displayTree(AST A, int indent)
{
	int curindent = indent;
	if(A->kind == EMPTYLIST)
		printf("%*s%s \n",curindent,"","[]");
	else if(A->kind == ERROR_NK)
		printf("%*s%s\n",curindent,"",A->fields.stringval);
	else if(A->kind == NUMBER_NK)
		printf("%*s%d \n",curindent,"",A->fields.intval);
	else if((A->kind == ID_NK) || (A->kind == CHARCONST_NK))
	{	if(strcmp(A->fields.stringval,"\n") == 0)
			printf("%*s\\n \n",curindent,"");
		else
			printf("%*s%s \n",curindent,"",A->fields.stringval);
	}
	else if(A->kind == BOOL_NK)
		printf("%*s%s \n",curindent,"",getBoolName(A->extra));
	else if(A->kind == PARAM_NK)
		printf("%*sPARAM(%d) \n",curindent,"",A->fields.intval);
	else
 	{
		if(A->kind == COLON_NK)
			printf("%*s%s \n",curindent,"",":");
		else if(A->kind == CONS_NK)
			printf("%*s%s \n",curindent,"","CONS");
		else if(A->kind == OP_NK)
			printf("%*s%s \n",curindent,"",getOperatorName(A->extra));
		else if(A->kind == BASIC_FUNC_NK)
			printf("%*s%s \n",curindent,"",getFunctionName(A->extra));
		else if(A->kind == APPLY_NK)
			printf("%*s%s \n",curindent,"","Apply");
		else if(A->kind == ACTION_NK)
			printf("%*s%s \n",curindent,"","~>");
		else if(A->kind == BRANCH_NK)
			printf("%*s%s \n",curindent,"","Branch");
		else if(A->kind == FUNC_NK)
			printf("%*sFUNCTION(%d) \n",curindent,"",A->extra);


		if(A->fields.subtrees.s1 != NULL)
			displayTree(A->fields.subtrees.s1, curindent+2);
		if(A->fields.subtrees.s2 != NULL)
			displayTree(A->fields.subtrees.s2, curindent+2);
		if(A->fields.subtrees.s3 != NULL)
			displayTree(A->fields.subtrees.s3, curindent+2);
	}
}


/* Main function to display AST, starts at indent 0 then calls DisplayTree function */

void displayAST(AST A)
{
	if(A != NULL)	
		displayTree(A,0);
}	
