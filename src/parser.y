/*

	Name: Adrian J. Paparelli
	Class: CSCI 5220
	Session: Spring 2016

	Description: parser.y defines the grammar rules for the SFL language, and adds the abstract syntax trees.

	Change Log:
	2016-04-05: Initial Revision
	2016-04-18: Added Symantic Parser

	Known Issue: Shift Reduce conflict with Action;Expr rule.

*/
%{
#include <stdio.h>
#include <string.h>
#include "token.h"
#include "lexer.h"
#include "interpreter.h"
#include "symboltable.h"
#include "ast.h"
#include "gc.h"
#define YYDEBUG 1
void installDefintions(AST lhs, AST body);
AST installFunction(AST id, AST body);
void yyerror(char const *s);
int error_occured;
int funcNum;
%}

%token TOK_ARROW
%token TOK_ACTION
%token <str> TOK_BOOL
%token <str> TOK_MULOP
%token <str> TOK_ADDOP
%token <str> TOK_RELOP
%token TOK_DEF
%token TOK_END
%token TOK_LET
%token TOK_IN
%token <str> TOK_LIST_FUNC
%token <str> TOK_ACTION_FUNC
%token <str> TOK_CHECK_FUNC
%token TOK_ELSE
%token TOK_CASE
%token <str> TOK_NOT
%token <str> TOK_LOGIC
%token <str> TOK_ID
%token <ival> TOK_INTEGER
%token TOK_THEN
%token <str> TOK_CHARCONST
%token <str> TOK_READ;
%token TOK_SEMI
%token TOK_COLON

%nonassoc TOK_ID TOK_INTEGER TOK_CHARCONST TOK_BOOL '[' '('
%nonassoc TOK_LIST_FUNC TOK_CHECK_FUNC TOK_ACTION_FUNC 
%nonassoc TOK_RELOP TOK_LET TOK_CASE TOK_READ

%left TOK_MULOP
%left TOK_ADDOP
%left TOK_LOGIC
%left TOK_NOT
%left JUXT
%left TOK_SEMI
%left TOK_ACTION

%right TOK_ARROW
%right TOK_COLON

%type <tree> Expr
%type <tree> IdList
%type <tree> Id
%type <tree> List
%type <tree> ExprList
%type <tree> Case
%type <tree> Term
%type <tree> Fact
%type <tree> Action
%type <tree> ListVal

%%
Program		: 
		| Definitions
			{}		
		;

Definitions	: Definition
			{}
		| Definitions Definition
			{}
		;

Definition	: TOK_DEF IdList '=' Expr TOK_END
			{		
				installDefintions($2, $4);
			}
		| error TOK_END
		;

Expr		: '(' Expr ')'
			{
				$$ = $2;
			}
		| TOK_CHARCONST
			{
				$$ = charNode($1);
			}	
		| TOK_BOOL
			{
				$$ = boolNode($1);
			}
		| List
			{
				$$ = $1;
			}
		| Term	
			{
				$$ = $1;
			}
		| Action
			{
				$$ = $1;
			}		
		| Expr Expr %prec JUXT
			{
				$$ = applyNode($1,$2);
			}
		| Expr TOK_ACTION Expr
			{	
				$$ = applyAction($1,$3);
			}
		| Id TOK_ARROW Expr
			{	
				$$ = installFunction($1, $3);	
			}
		| Expr TOK_ADDOP Term
			{
				$$ = applyOp($1,$3,$2);
			}
		| Action TOK_SEMI Action
			{
				$$ = applyAction($1, installFunction(emptyList(),$3));
			}
		| Expr TOK_LOGIC Expr
			{
				$$ = applyOp($1,$3,$2);
			}
		| Expr TOK_RELOP Expr
			{
				$$ = applyOp($1,$3,$2);
			}
		| Expr TOK_COLON Expr
			{
				$$ = applyCOLON($1,$3);
			}
		| TOK_NOT Expr
			{
				$$ = applyOp($2,NULL,$1);
			}
		| TOK_LIST_FUNC ListVal
			{
				$$ = applyBasicFunc($2, $1);
			}
		| TOK_CHECK_FUNC Expr
			{
				$$ = applyBasicFunc($2, $1);
			}
		| TOK_CASE Case TOK_END
			{
				$$ = $2;
			}
		| TOK_LET Id '=' Expr TOK_IN Expr TOK_END
			{
				insertTree($2->fields.stringval, $4);
				$$ = applyNode(installFunction($2, $6),$4);
			}
		;
	
Action 		: TOK_ACTION_FUNC Expr
			{
				$$ = applyBasicFunc($2,$1);
			}
		| TOK_READ
			{
				$$ = applyBasicFunc(emptyList(), $1);
			}
		;

List		: '[' ']'
			{
				$$ = emptyList();
			}
		| '[' ExprList ']'
			{
				$$ = $2;
			}
		;

ListVal		: List
			{
				$$ = $1;
			}
		| Id	
			{
				$$ = $1;
			}
		;

ExprList	: Expr
			{
				$$ = applyCONS($1, emptyList());
			}
		| Expr ',' ExprList
			{
				$$ = applyCONS($1,$3);
			}
		;

IdList		: 
			{
				$$ = emptyList();
			}
		| IdList Id
			{
				$$ = applyCONS($2,$1);
			}
		;

Id		: TOK_ID
			{
				$$ = idNode($1);
			}

Case		: Expr TOK_THEN Expr '|' TOK_ELSE TOK_THEN Expr
			{
				$$ = applyBranch($1,$3,$7);
			}
		| Expr TOK_THEN Expr '|' Case
			{ 
				$$ = applyBranch($1,$3,$5);
			} 
		;

Term 		: Fact
		 	{
				$$ = $1;
			}
		| Term TOK_MULOP Fact
			{
				$$ = applyOp($1,$3,$2);
			}
		;

Fact		: TOK_INTEGER
			{
				$$ = numberNode($1);
			}
		| Id
			{
				$$ = $1;
			}
		; 	

%%
AST installFunction(AST id, AST body)
{
	char *str;
	if(id->kind == EMPTYLIST)
		str = "";
	else
	{
		str = malloc(strlen(id->fields.stringval)+1);
		strcpy(str,id->fields.stringval);
	}
	AST ret = applyFunction(body, funcNum, str);
	funcNum++;
	return ret;
}

void installDefintions(AST lhs, AST body)
{
	AST nb = body;
	AST p = lhs;
	AST defnode = p->fields.subtrees.s1;
	while((p->fields.subtrees.s2 != NULL) && (p->fields.subtrees.s2->kind != EMPTYLIST))
	{
		nb = installFunction(defnode, nb);
		printf("Define %s = \n", defnode->fields.stringval);
                displayAST(nb);
                printf("\n");
                p = p->fields.subtrees.s2;
		defnode = p->fields.subtrees.s1;
	}
	printf("Define %s = \n", defnode->fields.stringval);
        displayAST(nb);
        printf("\n");
	insertTree(defnode->fields.stringval, nb);	

}

int main(int argc, char** argv)
{
	linenum = 0;
	funcNum = 0;
	error_occured = 0;
	if(argc == 2) 
	{
		initialize_gc();
		yyin = fopen(argv[1],"r");
		if(yyin == NULL)
			return 1;
		yydebug = 0;
		yyparse();
		if(!error_occured)
		{			
			return runInterpreter();
		}
	}
	return 1;
}

void yyerror(char const *s)
{
	printf("Syntax Error on line : %d \n",linenum);
	error_occured = 1;
}

