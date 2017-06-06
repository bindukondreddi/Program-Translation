/*

	Name: Adrian J. Paparelli
	Class: CSCI 5220
	Session: Spring 2016

	Description: ast.h contains all function used in generating
	abstract syntax trees. Function descriptions will follow to
	explain each funtion.

	Change Log:
	2016-02-17: Initial Revision

*/

#ifndef AST_H_
#define AST_H_


//
#define NUMBER_NK	1
#define APPLY_NK 	2
#define ID_NK		3
#define OP_NK		4
#define BRANCH_NK	5
#define FUNC_NK		6
#define BASIC_FUNC_NK	7
#define COLON_NK 	8
#define CONS_NK		9
#define BOOL_NK		10
#define ACTION_NK	11
#define PARAM_NK	12
#define CHARCONST_NK	13
#define ERROR_NK	98
#define EMPTYLIST       99

//Op definitions
#define PLUSOP_OK       40
#define SUBOP_OK	41
#define MULTOP_OK	42
#define DIVOP_OK	43
#define GT_OK		44
#define LT_OK		45
#define EQ_OK		46
#define ANDOP_OK	47
#define OROP_OK		48
#define NOTOP_OK	49
#define SEMICOL_OK	50
#define IF_OK		51
#define ELSE_OK		52

//Basic Function Defs
#define ISNULL_FK	60
#define ISFUNC_FK	61
#define ISINT_FK	62
#define ISBOOL_FK	63
#define ISACTION_FK	64
#define ISCHAR_FK	65
#define HEAD_FK		66
#define TAIL_FK		67
#define PRINT_FK	68
#define PRILST_FK	69
#define PROD_FK		70
#define READC_FK	71
#define READI_FK	72
#define ISLIST_FK	73


#define BOOL_TRUE	91
#define BOOL_FALSE	92



typedef struct astnode {
	int kind;
	int mark;
	int extra;
	
	union {
		int intval;
		const char* stringval;
		struct {
			struct astnode* s1;
			struct astnode* s2;
			struct astnode* s3;
		} subtrees;
	} fields;
}
ASTNODE;

typedef ASTNODE* AST;

AST numberNode(const int n);

AST idNode(const char* s);

AST charNode(const char* s);

AST boolNode(const char* s);

AST emptyList(void);

AST errorNode(const char* s);

AST applyNode(AST A, AST B);

AST applyOp(AST A, AST B, const char* opKind);

AST applyCOLON(AST A, AST B);

AST applyCONS(AST A, AST B);

AST applyBranch(AST A, AST B, AST C);

AST applyBasicFunc(AST A, const char* FuncKind);

AST applyAction(AST A, AST B);

AST applyFunction(AST A, const int n, const char* s);

AST applyParam(const int n);

AST replaceID(const char* s, const int n, AST E);

void displayTree(AST A, int indent);

void displayAST(AST A);

#endif
