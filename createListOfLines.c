#include <stdio.h>
#include <string.h>
#include <malloc.h>

struct token{
	struct token *prox;
	char tokenName [45];
};
typedef struct token Token;

struct list{
	struct list *prox,*ant;
	Token *pToken;
};
typedef struct list List;

struct desc{
	List *ini,*fim;
};
typedef struct desc Desc;


void createTokens(Token ** pTokens,char string [100]){
	int i=0,j=0;;
	char aux[100];
	Token * fim;
	*pTokens = NULL;
	while(i<strlen(string)-1){
		j=0;
		while(string[i]!='+' && string[i]!='-'&& string[i]!='*' && string[i]!='/' &&string[i]!=' ' &&
		 string[i]!='=' && string[i]!=':' && string[i]!='(' && string[i]!=')' && string[i] != ',' &&
		  i<strlen(string) && string[i]!=39 && string[i]!='"'){
			aux[j++]=string[i++];
		}
		aux[j]='\0';
		
		if(j>0)
		if(*pTokens == NULL){
			
			*pTokens = (Token *)malloc(sizeof(Token));
			(*pTokens)->prox = NULL;
			strcpy((*pTokens)->tokenName,aux);
			fim=*pTokens;
		}
		else
		{
			fim->prox=(Token *)malloc(sizeof(Token));
			fim->prox->prox=NULL;
			strcpy(fim->prox->tokenName,aux);
			fim=fim->prox;
		}
		
		if(string[i]!=' ' && string[i]!=',' && string[i]!=':' && string[i]!='\0' && i<strlen(string)){
			aux[0]=string[i];
			aux[1]='\0';
			fim->prox=(Token *)malloc(sizeof(Token));
			fim->prox->prox=NULL;
			strcpy(fim->prox->tokenName,aux);
			fim=fim->prox;
		}
		
		i++;
	}	
}


void createListOfLines(char caminho [100],List **lista){
	FILE * ptr = fopen(caminho,"r");
	List * aux;
	Desc desc;
	char string [100];
	if( ptr==NULL || *lista!=NULL){
		printf("Lista das linhas de execução ja iniciada!!!\n ERRO! ERRO! ERRO!\n");
	}
	else
	{
		fgets(string,100,ptr);
		if(strlen(string)>1){
			aux=(List*)malloc(sizeof(List));
			aux->prox=NULL;
			aux->ant=NULL;
			createTokens(&aux->pToken,string);
			*lista=aux;
			desc.ini=desc.fim=*lista;
		}
		fgets(string,100,ptr);
		while(!feof(ptr)){
			if(strlen(string)>1){
				aux=(List*)malloc(sizeof(List));
				aux->prox=NULL;
				aux->ant=desc.fim;
				desc.fim->prox=aux;
				desc.fim=aux;
				createTokens(&aux->pToken,string);
			}
			fgets(string,100,ptr);	
		}
	}
}

void exibe(List *l){
	int i=0;
	int j=0;
	Token *p;
	while(l!=NULL){
		printf("%d ",i);
		p=l->pToken;
		j=1;
		while(p!=NULL){
			printf("\n %d.%d %s ",i,j++,p->tokenName);
			p=p->prox;
		}
		i++;
		printf("\n\n");
		l=l->prox;
	}
}



int main (){
	List *l=NULL;
	createListOfLines("teste.py",&l);
	exibe(l);
}
