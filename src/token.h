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
