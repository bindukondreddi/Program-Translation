/*
	Description: symboltable.c contains all function used in generating
	a symbol table. Function descriptions will follow to
	explain each funtion.

	The symbol table is a hash table using the seperate chain collision handling method.
	
	Intially the table starts with 2000 symbol locations, but will double in size every time
	70% of the capacity is surpassed. 

	I have not implements function removal as of this time, only symbol find and insertion, and AST tree
	retrieval.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symboltable.h"
#include "ast.h"
#include "gc.h"

TABLE symTable;
int tableSize = TABLE_SIZE;
int prevSize = 0;
int items = 0;

/* Simple hash function to return a hash key for the incoming string */

int hashify(const char* key)
{
	const char* p;
	int g;
	int h = 0;
	
	for(p = key; *p != '\0'; p++)
	{
		h = ( h << 4) + (int)(*p);
		g = h & 0xF0000000;
		if(g != 0)
			h = h ^ (g >> 24);
		h = h & ~g;
	}
	return h % tableSize;
}

/* Function to check for the existance of a symbol in the symbol table
   Returns -1 on empty table or if the symbol does not exist and 1 if it does */

int hasSymbol(const char* key)
{
	int index, hasKey;
	hasKey = -1;
	if(symTable != NULL)
	{		
		HASH hn;
		index = hashify(key);
		hn = symTable[index];
		while(hn != NULL)
		{
			if(strcmp(key,hn->funcName) == 0)
			{
				hasKey = 1;
				break;
			}
			else
				hn = hn->next;
		}
	}
	return hasKey;
}

/* Function to retrieve the tree associated with the symbol, returns NULL if the symbol does not
   exist or the table is empty */ 

AST getTree(const char* key)
{
	int index;
	HASH hn;
	AST tree;
	tree = NULL;
	if((symTable != NULL) && (hasSymbol(key) == 1))
	{
		index = hashify(key);
		hn = symTable[index];
		while(hn != NULL)
		{
			if(strcmp(key,hn->funcName) == 0)
			{
				tree = hn->astTree;
				break;
			}
			else
				hn = hn->next;
		}
	}
	return tree;
}

/* Function to display the contents symbols contained in the symbol table, does not display trees */

void displayContents()
{
	int i = 0;
	if(symTable != NULL)
	{
		for(i = 0; i < tableSize; i = i + 1)
		{
			HASH hn = symTable[i];
			while(hn != NULL)
			{
				printf("%d: %s --> ",i, hn->funcName);
				hn = hn->next;			
			}
			if(symTable[i] != NULL)
				printf("\n");
		}
	}
	else
	{
		printf("Empty Table\n");
	}
}

/* Function to resize and rehash the symbol table if capacity is being approached */

void rehash()
{
	if(symTable != NULL)
	{
		int i = 0;
		int index;
		tableSize = tableSize * 2;
		HASH hn;
		TABLE tmpTable = symTable;
		TABLE newTable = NEWARRAY(HASH, tableSize);

		for(i = 0; i < tableSize; i = i + 1)
			newTable[i] = NULL;

		for(i = 0; i < prevSize; i = i + 1)
		{
			HASH hnOLD = tmpTable[i];
			while(hnOLD != NULL)
			{
				index = hashify(hnOLD->funcName);
				if(newTable[index] == NULL)
				{				
					hn = NEW(HASHNODE);
					hn->funcName = hnOLD->funcName;
					hn->astTree = hnOLD->astTree;
					hn->next = NULL;	
					newTable[index] = hn;
				}
				else
				{
					hn = newTable[index];
					while(hn->next != NULL)
						hn = hn->next;
				
					hn->next = NEW(HASHNODE);
				
					hn = hn->next;
					hn->funcName = hnOLD->funcName;
					hn->astTree = hnOLD->astTree;
					hn->next = NULL;	
				} 
				hnOLD = hnOLD->next;
			}
		}
	
		symTable = newTable;
		free(tmpTable);	
	}
}

/* Function to delete any nodes in the hash table index location */

void deleteList(HASH hn)
{
	if(hn != NULL)
	{
		deleteList(hn->next);
		free(hn);
	}
}

/* Function to clear out the symTable and set all table entries to NULL, does not delete the table though */

void clearTable(void)
{
	if(symTable != NULL)
	{
		int i;
		for(i = 0; i < tableSize; i++)
		{
			if(symTable[i] != NULL)
			{
				deleteList(symTable[i]);
				symTable[i] = NULL;
			}
		}
		items = 0;
	}
}

/* Function to delete the symbol table from memory */

void deleteTable(void)
{
	if(symTable != NULL)
	{
		clearTable();
		free(symTable);
		symTable = NULL;
		items = 0;
		prevSize = 0;
		tableSize = TABLE_SIZE;
	}
}

/* Function to add new a new hashnode to the table */

HASH newHashNode(const char* key, AST A, HASH ptr)
{
	HASH hn = NEW(HASHNODE);
	hn->funcName = key;
	hn->astTree = A;
	hn->next = ptr;
	return hn;
}


/* Function to insert a new AST tree into the table, returns a 1 if successful and -1 if unsuccesful */
int insertTree(const char* key, AST A)
{
	if(symTable == NULL)
	{
		symTable = NEWARRAY(HASH,tableSize);
		int i;
		for(i = 0; i < tableSize; i = i + 1)
			symTable[i] = NULL;
	}
	
	if(hasSymbol(key) == -1)
	{	
		if(((items * 1.0)/(tableSize*1.0)) > 0.7)
		{
			prevSize = tableSize;
			rehash();
		}

		int index;
		index = hashify(key);
		symTable[index] = newHashNode(key,A,symTable[index]);
		items = items + 1;		
		return 1;
	}
	else
	{
		return -1;
	}
}

void markList(HASH hn)
{
	if(hn != NULL)
	{
		markALL(hn->astTree);
		markList(hn->next);
	}
	
}

void markTable(void)
{
	int i = 0;
	if(symTable != NULL)
	{
		for(i = 0; i < tableSize; i = i + 1)
			markList(symTable[i]);
	}	
}
