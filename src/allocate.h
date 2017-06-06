/*

	Name: Adrian J. Paparelli
	Class: CSCI 5220
	Session: Spring 2016

	Description: allocate.h defines all functions used in allocating
		memory from the heap, copied from assignment #1 description

	Change Log:
	2016-11-14: Initial Revision

*/

#ifndef ALLOCATE_H
#define ALLOCATE_H

#include <stdlib.h>

#define NEW(T) ((T*) (malloc(sizeof(T))))

#define NEWARRAY(T,N) ((T*)(malloc((N)*(sizeof(T)))))

#endif
