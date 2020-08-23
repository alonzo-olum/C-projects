#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include "swap.h"

int main(argc, argv)
	int argc;
	char ** argv;
{
	char * token;
	token=(char *)strtok(argv[1], ":");
	char * a= (char *)malloc( 40 * sizeof(char));
	strcpy(a, token);
	char * b= (char *)malloc( 50 * sizeof(char));
	token=(char *)strtok(NULL, ":");
	strcpy(b, token);
	swap(a, b);
	free(a);
	free(b);
}

