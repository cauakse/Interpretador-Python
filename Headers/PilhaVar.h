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

void createNewVar(char nome[45],Pilha **P) {
	Pilha *aux = (Pilha*)malloc(sizeof(Pilha));
	aux->prox = *P;
	aux->ant = NULL;
	strcpy(aux->conteudo.nomeVar,nome);
	aux->conteudo.val.flag = 6;// Deixando a variável com tipo indefinido
	if(*P != NULL) {
		(*P)->ant = aux;
	} else {
		aux->prox = NULL;
	}
	*P = aux;
}
