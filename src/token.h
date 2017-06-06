/*

	Name: Adrian J. Paparelli
	Class: CSCI 5220
	Session: Spring 2016

	Description: token.h contains all non-character token definitions
	for the Lexical Analyzer.

	Change Log:
	2016-02-08: Initial Revision

*/

#ifndef TOKEN_H
#define TOKEN_H

#include "ast.h"


#define YYSTYPE_IS_DECLARED
typedef union {
  const char* str;
  int   ival;
  AST	tree;
} 
YYSTYPE;

#endif
