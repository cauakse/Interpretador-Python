//Linhas de execução
struct token {
	char tokenName[45], tokenText[100];
	struct token *prox;
};
typedef struct token Token;

struct list {
	Token *pToken;
	struct list *prox, *ant;
};
typedef struct list List;

struct desc {
	List *ini, *fim;
};
typedef struct desc Desc;

//Pilha de variáveis
union vari {
	int in; //flag = 0
	float fl; //flag = 1
	double db; //flag = 2
	char ch; //flag = 3
	char str[100]; //flag = 4
	List *pont; //flag = 5
	//CASO A FLAG SEJA 6 SIGNIFICA QUE AINDA NAO POSSUI VALOR ATRIBUIDO
};
struct valor {
	char flag;
	union vari variavel;
};
typedef struct valor Valor;

struct conteudo {
	char nomeVar[45];
	Valor val;
};
typedef struct conteudo Conteudo;

struct pilha {
	struct pilha *prox, *ant;
	Conteudo conteudo;
};
typedef struct pilha Pilha;


struct funcoes {
	char nome[45];
	List *inicio;
	struct funcoes *prox;
};
typedef struct funcoes Funcoes;

void createTokens(Token **pTokens, char string[100]) {
	int i = 0, j = 0;
	char aux[100];
	Token *fim;
	*pTokens = NULL;


	while (i < strlen(string) - 1) {
		j = 0;
		while (string[i] != '+' && string[i] != '-' && string[i] != '*' && string[i] != '/' && string[i] != ' ' &&
		        string[i] != '=' && string[i] != ':' && string[i] != '(' && string[i] != ')' && string[i] != ',' &&
		        i < strlen(string) && string[i] != 39 && string[i] != '"') {
			aux[j++] = string[i++];
		}
		aux[j] = '\0';

		if (j > 0)
			if (*pTokens == NULL) {
				*pTokens = (Token *) malloc(sizeof(Token));
				(*pTokens)->prox = NULL;
				if(strcmp(string,"fim") || strcmp(string,"fimdef"))
					strcpy((*pTokens)->tokenText,string);
				strcpy((*pTokens)->tokenName, aux);
				fim = *pTokens;
			} else {
				fim->prox = (Token *) malloc(sizeof(Token));
				fim->prox->prox = NULL;
				strcpy(fim->prox->tokenName, aux);
				fim = fim->prox;
			}

		if (string[i] != ' ' && string[i] != ',' && string[i] != ':' && string[i] != '\0' && i < strlen(string)) {
			aux[0] = string[i];
			aux[1] = '\0';
			fim->prox = (Token*)malloc(sizeof(Token));
			fim->prox->prox = NULL;
			if(strcmp(string,"fim") || strcmp(string,"fimdef"))
				strcpy((*pTokens)->tokenText,string);
			strcpy(fim->prox->tokenName, aux);
			fim = fim->prox;
		}

		i++;
	}
}

void createListOfLines(FILE *ptr, List **lista) {
	List *aux;
	Desc desc;
	int idenAnt = 0, idenAtu = 0, i;
	char string[100], def=0, func=0;

	fgets(string, 100, ptr);
	// Descobrindo se a linha é um def
	if(strlen(string)>4 && (string[0] == 'd' && string[1] == 'e' && string[2] == 'f' && string[3] == ' '))
		def++;
		
	if (strlen(string) > 1) {
		aux = (List*)malloc(sizeof(List));
		aux->prox = NULL;
		aux->ant = NULL;
		createTokens(&aux->pToken, string);
		*lista = aux;
		desc.ini = desc.fim = *lista;
	}

	fgets(string, 100, ptr);
	while (!feof(ptr)) {
		if (strlen(string) > 1) {

			// Descobrindo se a linha é um def
			if(strlen(string)>4 && (string[0] == 'd' && string[1] == 'e' && string[2] == 'f' && string[3] == ' ')) {
				def++;
			}

			i = 0;
			idenAnt = idenAtu;
			idenAtu = 0;
			while (string[i] == ' ') {
				idenAtu++;
				i++;
			}
			if (idenAnt > idenAtu) {
				if(idenAtu != 0 || (idenAtu==0 && !def))
					func++;
				
				aux = (List*)malloc(sizeof(List));
				aux->prox = NULL;
				aux->ant = desc.fim;
				desc.fim->prox = aux;
				desc.fim = aux;

				if(func) {
					func--;
					createTokens(&aux->pToken,"fim");
				} 
				else {
					def--;
					createTokens(&aux->pToken,"fimdef");
				}

			}
			aux = (List*)malloc(sizeof(List));
			aux->prox = NULL;
			aux->ant = desc.fim;
			desc.fim->prox = aux;
			desc.fim = aux;
			createTokens(&aux->pToken, string);
		}
		fgets(string, 100, ptr);
	}
}

// Função de Exibição dos Tokens para testes
void exibe(List *l) {
	int i = 0;
	int j = 0;
	Token *p;

	while (l != NULL) {
		printf("%d ", i);
		p = l->pToken;
		j = 1;
		while (p != NULL) {
			printf("\n %d.%d %s ", i, j++, p->tokenName);
			p = p->prox;
		}
		i++;
		printf("\n\n");
		l = l->prox;
	}
}
