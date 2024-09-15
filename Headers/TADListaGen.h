struct lista{
	char info[15];
	struct lista *prox;
};
typedef struct lista Lista;

union dado {
	float valor;
	char op;
};
typedef union dado Dado;

struct listagen {
	char terminal;
	union dado no;
	struct listagen *cabeca, *cauda;
};
typedef struct listagen ListaGen;

struct pilhaG {
	struct listagen *info;
	struct pilhaG *prox;
};
typedef struct pilhaG PilhaG;

struct filaG {
	struct listagen *info;
	struct filaG *prox;
};
typedef struct filaG FilaG;

struct tptermo {
	char termo[10];
	struct tptermo *prox;
};
typedef struct tptermo TpTermo;

void InitP (PilhaG **P) {
	*P = NULL;
}

void InitF (FilaG **F) {
	*F = NULL;
}

void push (PilhaG **P, ListaGen *L) {
	PilhaG *nova;
	nova = (PilhaG*)malloc(sizeof(PilhaG));
	nova->info = L;
	nova->prox = *P;
	*P = nova;
}

void enqueue (FilaG **F, ListaGen *L) {
	FilaG *nova, *aux;
	nova = (FilaG*)malloc(sizeof(FilaG));
	nova->info = L;
	nova->prox = NULL;
	aux = *F;
	while(aux->prox)
		aux = aux->prox;
	aux->prox = nova;
}

char isEmptyP (PilhaG *P) {
	return P == NULL;
}

char isEmptyF (FilaG *F) {
	return F == NULL;
}

void pop (PilhaG **P, ListaGen **L) {
	PilhaG *aux;
	if(!isEmptyP(*P)) {
		*L = (*P)->info;
		aux = *P;
		*P = (*P)->prox;
		free(aux);
	}
	else
	*L = NULL;
}

void dequeue (FilaG **F, ListaGen **L) {
	FilaG *aux;
	if(!isEmptyF(*F)) {
		*L = (*F)->info;
		aux = *F;
		*F = (*F)->prox;
		free(aux);
	}
	else
	*L = NULL;
}

char Nula (ListaGen *L) {
	return L==NULL;
}

char Atomo (ListaGen *L) {
	return !Nula(L) && L->terminal;
}

ListaGen *Head (ListaGen *L) {
	if(Nula(L) || Atomo(L))
		return NULL;
	return L->cabeca;
}

ListaGen *Tail (ListaGen *L) {
	if(Nula(L) || Atomo(L))
		return NULL;
	return L->cauda;
}
