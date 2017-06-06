
#ifndef LEXER_H
#define LEXER_H

int yylex();

extern FILE* yyin;

extern YYSTYPE yylval;

int linenum;

#endif
