#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 10000
#define MAX_KEY_LENS 1000
#define MAX_VALUE_LENS 1000 // should be care of the 1 character of '='

struct dict {
	char key[MAX_KEY_LENS];	
	char value[MAX_VALUE_LENS];	
	struct dict *next;
};
int strsplit(char stringbuf[], struct dict * dictnode ,  char splitor){
	int len = strlen(stringbuf);
	int i = 0;
	for( ; i < len ; i++){
		if (stringbuf[i] == splitor)
		{
			stringbuf[i] = '\0';
			strncpy( dictnode->key, stringbuf,MAX_KEY_LENS);	
			strncpy( dictnode->value, stringbuf+i+1,MAX_VALUE_LENS);
			return 0;
		}
	}	
	strncpy(stringbuf, dictnode->key, MAX_KEY_LENS);	
	dictnode->value[0] = '\0';
	return 1; // only have a key
}


int main(int argc, char *argv[]){

	FILE *ofp = NULL;
	FILE *tfp = NULL;
	char buf[MAX_KEY_LENS+MAX_VALUE_LENS];
	struct dict * odict = (struct dict *)malloc(sizeof( struct dict));
	struct dict * tdict = (struct dict *)malloc(sizeof( struct dict));

	const struct dict * fodict = odict;
	const struct dict * ftdict = tdict;
	if (3 != argc){
		printf("usage: \n");	
		return -1;
	}
	
	if((ofp = fopen (argv[1], "r")) == NULL)
	{
		printf("can't open orignal file\n");
	}

	if((tfp = fopen (argv[2], "r")) == NULL)
	{
		printf("can't open translated file\n");
	}
	
	for (int i = 0 ; i < MAX_LINE && (fgets(buf,MAX_KEY_LENS + MAX_VALUE_LENS , ofp ) !=NULL) ; i++){
	
		if (buf[0] != '#' && (strlen(buf) >= 3) ){
			strsplit(buf,odict,'=') ;
			if ( (odict->next = (struct dict *)malloc(sizeof( struct dict))) == NULL ){
				printf("error when requesting for memory\n");
			}
			else{
				odict = odict->next ;
				odict->next = NULL;
			}
	       	}
	}
	
	for (int i = 0 ; i < MAX_LINE && (fgets(buf,MAX_KEY_LENS + MAX_VALUE_LENS , tfp ) !=NULL) ; i++){
	
		if (buf[0] != '#' && (strlen(buf) >= 3) ){
			strsplit(buf,tdict,'=') ;
			if ( (tdict->next = (struct dict *)malloc(sizeof( struct dict))) == NULL ){
				printf("error when requesting for memory\n");
			}
			else{
				tdict = tdict->next;
				tdict->next = NULL;
				
			}
	       	}
	}
	for (odict = fodict; odict->next != NULL ; odict = odict->next ){
		

		for (tdict = ftdict; tdict->next != NULL ; tdict = tdict->next ){
			if ( 0 == strcmp(odict->key,tdict->key))
			{
				break;
			}
		}
		if (NULL == tdict->next){
			printf("%s=%s", odict->key,odict->value);
		}
	}

	for (odict = fodict; odict->next != NULL ;  ){
		struct dict * tempdict = odict->next;
		free(odict);
		odict = tempdict;
	
	}
	free(odict);

	for (tdict = ftdict; tdict->next != NULL ;  ){
		struct dict * tempdict = tdict->next;
		free(tdict);
		tdict = tempdict;
	
	}
	free(tdict);
	return 0;
}
