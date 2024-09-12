struct funcoes {
	char nome[45];
	List *inicio;
	struct funcoes *prox;
};
typedef struct funcoes Funcoes;

int whatsIt(Token *T,Pilha *P,Funcoes *F) {
	if(strcmp(T->tokenName,"def")==0)
		return 0;

	while(P!=NULL && strcmp(P->conteudo.nomeVar,T->tokenName)!=0)
		P=P->prox;
	if(P!=NULL)
		return 1; // Busco uma variavel com o mesmo nome

	while(F!=NULL && strcmp(F->nome,T->tokenName)!=0)
		F=F->prox;
	if(F!=NULL)
		return 2; // Busco uma função com o mesmo nome

	if(strcmp(T->tokenName,"if")==0 ||
	        strcmp(T->tokenName,"while")==0 ||
	        strcmp(T->tokenName,"for")==0 ||
	        strcmp(T->tokenName,"elif")==0 ||
	        strcmp(T->tokenName,"else")==0 ||
	        strcmp(T->tokenName,"do")==0 ||
	        strcmp(T->tokenName,"switch")==0 ||
			strcmp(T->tokenName,"print")==0)
		return 3;// Se for função do sistema
	return 10;// Se for criação de variavel
}

void createFunctionsList (Funcoes **F, List *L) {
	Funcoes * aux, *ultimo;
	Token *tAux;

	*F = NULL;
	while(L!=NULL) {
		if(strcmp(L->pToken->tokenName,"def")==0) {
			aux = (Funcoes*)malloc(sizeof(Funcoes));
			tAux = L->pToken->prox;
			strcpy(aux->nome,tAux->tokenName);
			aux->inicio = L;
			aux->prox = NULL;
			if(!(*F)) {
				*F = aux;
				ultimo = *F;
			} else {
				ultimo->prox = aux;
				ultimo = aux;
			}
		}
		L=L->prox;
	}
}

// Funcão de teste de exibição de funções
void exibirFuncoes (Funcoes *F) {
	int c = 42, l = 12;
	while(F) {
		gotoxy(c,l);
		printf("%s",F->nome);
		l++;
		F = F->prox;
	}
}
