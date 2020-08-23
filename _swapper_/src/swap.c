#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include "swap.h"

void swap(char * a, char * b){

	char * temp=(char *)malloc(50 * sizeof(char));
	printf ( "\n a is %s & b is %s\n", a, b );
	strcpy( temp, a );
	strcpy( a, b );
	strcpy( b, temp );
	printf ( "\n swapped a to %s & b to %s\n", a, b );

}
