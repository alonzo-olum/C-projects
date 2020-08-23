#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "hash.h"

#define SIZE 256

struct nlist * main( argc, argv )
	int argc;
	char * argv[];
{
	char * token;
	char name[SIZE];
	char defn[SIZE];
	token = (char *)strtok(argv[1], ":");
	strcpy(name,token);
	token = (char *)strtok(NULL, ":");
	strcpy(defn,token);
	hashtable[hash(name)] = install(name,defn);
	printf(" hashtable address : %p with member name : %s ", hashtable, hash(name));

}
