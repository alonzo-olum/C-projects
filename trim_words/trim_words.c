#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char * trim(word)
	char* word;
{
	char* old=word;
	char* new=word;
	while(*old==' '){
		old++;
	}	
	while(*old!='\0'){
		*(new++)=*(old++);
	}
	*new='\0';
	return (char*)realloc(word,strlen(word)+1);
}

int main(){
	char * word=(char*)malloc(strlen("   ninjitsu")+1);
	strcpy(word,"   ninjitsu");
	printf("%s\n", trim(word));
}
