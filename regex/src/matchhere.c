#include<stdio.h>

#include "matchere.h"


int matchhere(regexp, text)
	char * regexp;
       	char * text;
{
	if(regexp[0] == '\0')
		return 1;
	if(regexp[1] == '*')
		return matchstar(regexp[0],regexp+2,text);
	if(regexp[0] == '$' && regexp[1] == '\0')
		return *text == '\0';
	if(*text!='\0' && (regexp[0]=='.' || regexp[0]==*text))
		return matchhere(regexp+1, text+1);
	return 0;

}

int matchstar(c, regexp, text)
	int c; 
	char * regexp;
       	char * text;
{
	do{
		if(matchhere(regexp, text))
			return 1;
	} while(*text != '\0' && (*text++ == c || c == '.'));
	return 0;
}
