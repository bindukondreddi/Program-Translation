
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


