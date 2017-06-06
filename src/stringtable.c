/*

	Name: Adrian J. Paparelli
	Class: CSCI 5220
	Session: Spring 2016

	Description: stringtable.c contains all function used in storing 
		strings into a string list.

	Change Log:
	2016-11-14: Initial Revision

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "allocate.h"
#include "stringtable.h"

List stringList;

// Copied from assignment description to create a string that is
// ready for storage in the string list.
char* perm(const char* s)
{
	char* result = NEWARRAY(char, strlen(s)+1);
	strcpy(result,s);
	return result;
}

// Intern Function, adds string to list if it does not exist,
// returns string if it already exists in the list, or returns
// NULL function fails to add string to list
char* intern(const char* s)
{
	Node* currptr;
  	if(stringList == NULL)
	{
		stringList = NEW(Node);
		stringList->data = perm(s);
		stringList->next = NULL;
		return stringList->data;
	}		
	else
	{
		currptr = stringList;
		while(currptr != NULL)
		{
			if(strcmp(s,currptr->data) == 0)
			{
				return currptr->data;
			}
			else if(currptr->next == NULL)
			{
				currptr->next = NEW(Node);
				currptr = currptr->next;
				currptr->data = perm(s);
				currptr->next = NULL;
				return currptr->data;
			}
			else
				currptr = currptr->next;
		}
		return NULL;
	}	
}

// Clear function, clears the string list from memory when invoked.
void clearList()
{
	if(stringList != NULL)
	{		
		Node* currptr = stringList;
		Node* temp;
		while(currptr != NULL)
		{
			temp = currptr;
			currptr = currptr->next;
			free(temp);
		}
		stringList = NULL;
	}
}
	


