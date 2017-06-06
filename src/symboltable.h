/*

	Name: Adrian J. Paparelli
	Class: CSCI 5220
	Session: Spring 2016

	Description: symboltable.h defines all functions used in symbol names and AST trees
		into a symbol table.

	Change Log:
	2016-02-26: Initial Revision

*/

#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_
#include <stdio.h>
#include "allocate.h"
#include "ast.h"

#define TABLE_SIZE 2000

typedef struct hashnode {
	const char* funcName;
	AST astTree;
	struct hashnode* next;
} HASHNODE;

typedef HASHNODE* HASH;
typedef HASH* TABLE;


int hashify(const char* key);

int hasSymbol(const char* key);

void displayContents();

AST getTree(const char* key);

void clearTable();

void deleteTable();

int insertTree(const char* key, AST A);

void markTable(void);

#endif
