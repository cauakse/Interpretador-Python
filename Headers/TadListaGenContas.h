//DECLARAÇÃO LISTAGEN PARA CALCULOS
union tpInfo {
	char operador[3];
	float valor;
	char funcao[20];
};

struct listagenC {
	char terminal;//'V' = valor | 'O' = operador | 'F' = função
	union tpInfo info;
	struct listagenC *cabeca,*cauda;
};
typedef struct listagenC ListaGenC;

struct tpTermo {
	char termo[20];
	struct tpTermo *prox;
};
typedef struct tpTermo Termo;

//-----------------------------------------------------------

ListaGenC * criaNo(char termo[20]) {
	ListaGenC *NC = (ListaGenC *)malloc(sizeof(ListaGenC));
	NC->cauda=NC->cabeca=NULL;
	if(termo[0]=='+'||termo[0]=='-'||termo[0]=='/'||termo[0]=='*') {
		NC->terminal='O';
		strcpy(NC->info.operador,termo);
	} else {
		if(termo[0]>=48 && termo[0]<=57) {
			NC->terminal='V';
			NC->info.valor=atoi(termo);
		} else {
			NC->terminal='F';
			strcpy(NC->info.funcao,termo);
		}
	}

	return NC;
}


//DEFINICAO PILHA PARA LISTAGEN CALCULOS

struct tpPilhaC {
	ListaGenC *valor;
	struct tpPilhaC *prox;
};
typedef struct tpPilhaC PilhaC;

void initPC(PilhaC ** P) {
	*P=NULL;
}

void pushPC(PilhaC **P, ListaGenC *V) {
	PilhaC *nova = (PilhaC *)malloc(sizeof(PilhaC));
	nova->valor=V;
	nova->prox=*P;
	*P=nova;
}

void popPC (PilhaC **P,ListaGenC **V) {
	PilhaC *aux = *P;
	*P=(*P)->prox;
	*V=aux->valor;
	free(aux);
}

char isEmptyPC(PilhaC *P) {
	return P==NULL;
}

void topoPC(PilhaC *P, ListaGenC **V) {
	if(!isEmptyPC(P))
		*V=P->valor;
	else
		*V=NULL;
}
//---------------------------------------------------------


//DEFINICAO FILA PARA LISTAGEN CALCULOS
struct tpFilaC {
	struct tpFilaC *prox;
	ListaGenC *valor;
};
typedef struct tpFilaC FilaC;

void initFC(FilaC ** F) {
	*F=NULL;
}
void enqueueFC(FilaC **F,ListaGenC *V) {
	FilaC *NC = (FilaC *)malloc(sizeof(FilaC));
	FilaC *aux = *F;
	NC->valor =V;
	NC->prox =NULL;
	if(*F==NULL)
		*F=NC;
	else {
		while(aux->prox!=NULL)
			aux=aux->prox;
		aux->prox=NC;
	}
}

void dequeueFC (FilaC **F, ListaGenC **V) {
	FilaC *aux = *F;
	*F=(*F)->prox;
	*V=aux->valor;
	free(aux);
}

char isEmptyFC (FilaC *F) {
	return F==NULL;
}

void topoFC (FilaC *F,ListaGenC **V) {
	if(!isEmptyFC(F))
		*V=F->valor;
	else
		*V=NULL;
}
//---------------------------------------------------------

//DEFINICAO PILHA DE FLOAT
struct tpPilhaI {
	int valor;
	struct tpPilhaI *prox;
};
typedef struct tpPilhaI PilhaI;

void initPI(PilhaI ** P) {
	*P=NULL;
}

void pushPI(PilhaI **P, float V) {
	PilhaI *nova = (PilhaI *)malloc(sizeof(PilhaI));
	nova->valor=V;
	nova->prox=*P;
	*P=nova;
}

void popPI (PilhaI **P,float *V) {
	PilhaI *aux = *P;
	*P=(*P)->prox;
	*V=aux->valor;
	free(aux);
}

char isEmptyPI(PilhaI *P) {
	return P==NULL;
}

void topoPI(PilhaI *P, float *V) {
	if(!isEmptyPI(P))
		*V=P->valor;
}
//-----------------------
//DEFINICAO PILHA DE CHAR
struct tpPilhaCH {
	char valor[2];
	struct tpPilhaCH *prox;
};
typedef struct tpPilhaCH PilhaCH;

void initPCH(PilhaCH ** P) {
	*P=NULL;
}

void pushPCH(PilhaCH **P, char *V) {
	PilhaCH *nova = (PilhaCH *)malloc(sizeof(PilhaCH));
	strcpy(nova->valor,V);
	nova->prox=*P;
	*P=nova;
}

void popPCH (PilhaCH **P,char *V) {
	PilhaCH *aux = *P;
	strcpy(V,(*P)->valor);
	*P=(*P)->prox;
	free(aux);
}

char isEmptyPCH(PilhaCH *P) {
	return P==NULL;
}

void topoPCH(PilhaCH *P, char **V) {
	if(!isEmptyPCH(P))
		strcpy(*V,P->valor);
}
//-----------------------

Termo * separa(char * expressao) {
	Termo *T=NULL,*NC,*aux=NULL;
	char auxC[100];
	int i,j;
	for(i=0; i<strlen(expressao); i++) {
		NC=(Termo *)malloc(sizeof(Termo));
		NC->prox=NULL;
		if(i<strlen(expressao)-1 && expressao[i]=='*' && expressao[i+1]=='*') {
			i++;
			strcpy(NC->termo,"**");
		} else {
			if(expressao[i]>=48 && expressao[i]<=57) {
				NC->termo[0]=expressao[i];
				NC->termo[1]='\0';
			} else {
				if(expressao[i] == '+' || expressao[i] == '-' || expressao[i] == '*' || expressao[i]=='/' || expressao[i]== '(' || expressao[i]==')') {
					NC->termo[0]=expressao[i];
					NC->termo[1]='\0';
				} else {
					j=0;
					for(; i<strlen(expressao) && (!(expressao[i]>=48 && expressao[i]<=58) || expressao[i] != '+' || expressao[i] != '-' || expressao[i] != '*' || expressao[i]!='/',i++ );) {
						auxC[j++]=expressao[i];
					}
					auxC[j]='\0';
					strcpy(NC->termo,auxC);
				}
			}
		}
		if(T==NULL) {
			T=NC;
			aux=NC;
		} else {
			aux->prox=NC;
			aux=aux->prox;
		}
	}

	return T;
}

void destroiLista(Termo **L) {
	Termo *aux;
	while(*L!=NULL) {
		aux=*L;
		*L=(*L)->prox;
		free(aux);
	}
}

float calc(float v1,float v2,char *o) {
	char op;
	if(strcmp(o,"**")==0)
		return pow(v1,v2);
	else {
		op=o[0];
		switch(op) {
			case '+':
				return v1+v2;
			case '-':
				return v1-v2;
			case '*':
				return v1*v2;
			case '/':
				return v1/v2;
		}
	}
}

float calcula(ListaGenC *L) {
	ListaGenC *ex;
	PilhaI *PI;
	PilhaCH *PCH;
	initPI(&PI);
	initPCH(&PCH);
	float v1,v2;
	char aux[2];
	while(L!=NULL) {
		ex=L;
		if(L->terminal=='V') {
			pushPI(&PI,L->info.valor);
		} else {
			if(L->terminal=='O') {
				topoPCH(PCH,&aux);
				if(!isEmptyPCH(PCH)) {
					if(strlen(aux)>1) {
						popPI(&PI,&v1);
						popPI(&PI,&v2);
						popPCH(&PCH,aux);
						pushPI(&PI,calc(v1,v2,aux));
					} else {
						if((strcmp(L->info.operador,"-")==0||strcmp(L->info.operador,"+")==0) && (strcmp(aux,"*")==0 || strcmp(aux,"/")==0)) {
							popPI(&PI,&v1);
							popPI(&PI,&v2);
							popPCH(&PCH,aux);
							pushPI(&PI,calc(v1,v2,aux));
						}

					}
				}

				pushPCH(&PCH,L->info.operador);

			} else { //Resolver retorno de função

			}
		}

		L=L->cauda;
		free(ex);
	}

	while(!isEmptyPCH(PCH)) {
		popPI(&PI,&v1);
		popPI(&PI,&v2);
		popPCH(&PCH,aux);
		pushPI(&PI,calc(v2,v1,aux));
	}
	popPI(&PI,&v1);
	return v1;
}

float resolveExpressao (char expressao[100]) {
	ListaGenC *L = NULL, *atual;
	PilhaC *P;
	FilaC *F;
	Termo *lista = separa(expressao);
	initPC(&P);
	initFC(&F);
	float resultado=0;

	while(lista!=NULL) {
		if(L==NULL) {
			L= atual = criaNo(lista->termo);
			lista=lista->prox;
		} else {
			if(strcmp(lista->termo,"(")==0) {
				atual->cauda = criaNo("0");
				atual=atual->cauda;
				pushPC(&P,atual);
				lista=lista->prox;
				atual->cabeca=criaNo(lista->termo);
				atual=atual->cabeca;
				lista=lista->prox;
			} else {
				if(strcmp(lista->termo,")")==0) {
					popPC(&P,&atual);
				} else {
					atual->cauda = criaNo(lista->termo);
					atual=atual->cauda;
				}

				lista=lista->prox;
			}
		}
	}
	pushPC(&P,L);
	enqueueFC(&F,L);

	while(!isEmptyFC(F)) {
		dequeueFC(&F,&atual);
		while(atual!=NULL) {
			if(atual->cabeca!=NULL) {
				pushPC(&P,atual);
				enqueueFC(&F,atual->cabeca);
			}
			atual=atual->cauda;
		}
	}

	while(!isEmptyPC(P)) {
		popPC(&P,&atual);
		if(atual!=L) {
			atual->info.valor = calcula(atual->cabeca);
		} else
			resultado = calcula(atual);
	}
	destroiLista(&lista);
	return resultado;
}
