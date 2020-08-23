#include<stdlib.h>
#include<stdio.h>

char* getLine(void){
	const size_t sizeIncrement=10;
	char* buffer=(char*)malloc(sizeIncrement);
	char* currentPosition;
	currentPosition=buffer;
	size_t length=0;
	size_t maxLength;
	maxLength=sizeIncrement;
	int character;
	if(currentPosition==NULL)return NULL;

	while(1){
		character = fgetc(stdin);
		if(character=='\n')break;

		if(++length>=maxLength){
			char* newBuff = realloc(buffer, maxLength+=sizeIncrement);

			if(newBuff==NULL){
				free(buffer);
				return NULL;	
			}
			currentPosition=newBuff+(currentPosition-buffer);
			buffer=newBuff;
		}
		*currentPosition++=character;
	}
	*currentPosition='\0';
	return buffer;
}

int main(){
	char* b = getLine();
	printf("%s\n",b);
	//while(*b!='\0'){ printf(" %c ", *b++); }

}
