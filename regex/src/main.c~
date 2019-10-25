#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "matchere.h"

int match(regexp, text)
	char * regexp;
	char * text;
{
 	if(regexp[0] == '^')
		return matchhere(regexp+1, text);
	do{
		if(matchhere(regexp, text))
			return 1;
	}while(*text++ != '\0');
	return 0;
}




/* Main Program 
 */

int main(argc, argv)
	int argc;
	char ** argv;
{
	char * contain;
	char * token;
	char * regex = (char *)malloc(10 * sizeof(char));
	token = (char *)strtok(argv[1], ":");
	strcpy(regex,token);
	char * tex = (char *)malloc(10 * sizeof(char));
	token = (char *)strtok(NULL, ":");
	strcpy(tex,token);
	contain=match(regex,tex)?"matches":"does not match";
	printf("%s %s %s\n", regex, contain, tex);
}
