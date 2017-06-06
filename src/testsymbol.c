#include <stdio.h>
#include "ast.h"
#include "symboltable.h"

char *randstring(size_t length) {

    static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";        
    char *randomString = NULL;
	int n;

    if (length) {
        randomString = malloc(sizeof(char) * (length +1));

        if (randomString) {            
            for (n = 0;n < length;n = n + 1) {            
                int key = rand() % (int)(sizeof(charset) -1);
                randomString[n] = charset[key];
            }

            randomString[length] = '\0';
        }
    }

    return randomString;
}

int main()
{

AST fn1 = idNode("x");
AST fn2 = idNode("x");
AST fn3 = applyOp(fn1, fn2, "+");
AST fn4 = applyFunction(fn3, 1, "x");

int i;

for(i = 0; i < 5000; i = i + 1)
{
	char *str = randstring((i%10)+5);
	if(insertTree(str,fn4) == -1)
	{
		printf("Table already contains function %s\n", str);
	}	
}
displayContents();
deleteTable();

	if(insertTree("test",fn4) == -1)
	{
		printf("Table already contains function test\n");
	}
	if(insertTree("test",fn4) == -1)
	{
		printf("Table already contains function test\n");
	}

insertTree("main",fn4);
insertTree("slope",fn4);
insertTree("jupiter",fn4);
insertTree("mars",fn4);
insertTree("apple",fn4);
insertTree("banana",fn3);
displayContents();

if(hasSymbol("test") == 1) 
{
	AST A = getTree("test");
	
	displayAST(A);
}

if(hasSymbol("apple") == 1) 
{
	printf("Found apple\n");
	AST A = getTree("apple");
	
	displayAST(A);
}

if(hasSymbol("mars") == 1) 
{
	printf("Found mars\n");

	AST A = getTree("mars");
	
	displayAST(A);
}

if(hasSymbol("banana") == 1) 
{
	printf("Found banana\n");

	AST A = getTree("banana");
	
	displayAST(A);
}

deleteTable();
displayContents();

insertTree("main",fn4);
insertTree("slope",fn4);
insertTree("jupiter",fn4);
insertTree("mars",fn4);
insertTree("apple",fn4);
insertTree("banana",fn3);

displayContents();
deleteTable();
return 0;
}
