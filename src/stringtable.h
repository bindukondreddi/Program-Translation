/*

	Name: Adrian J. Paparelli
	Class: CSCI 5220
	Session: Spring 2016

	Description: stringtable.h defines all functions used in storing 
		strings into a string list.

	Change Log:
	2016-11-14: Initial Revision

*/

#ifndef STRINGTABLE_H
#define STRINGTABLE_H

#include <stdlib.h>

typedef struct node
{
	char* data;
	struct node* next;
} Node;

typedef Node* List;

char* perm(const char* s);

char* intern(const char* s);

void clearList();

#endif


