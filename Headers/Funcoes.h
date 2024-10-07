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

int resolveIf(Token *T, Pilha *P) {
	Pilha *aux = P, *aux2=P;
	int flag;
	Token *prim,*seg,*terc;
	T=T->prox;
	prim=T;
	char op[3],valor[100];

	while(aux!=NULL && strcmp(T->tokenName,aux->conteudo.nomeVar)!=0)
		aux=aux->prox;

	T=T->prox;
	seg=T;
	strcpy(op,T->tokenName);

	while(aux2!=NULL && strcmp(T->tokenName,aux2->conteudo.nomeVar)!=0)
		aux2=aux2->prox;

	T=T->prox;
	terc=T;

	if(aux!=NULL && aux2!=NULL) {
		if(strcmp(op,">")==0) {
			flag = aux->conteudo.val.flag;
			if(flag==0)
				return aux->conteudo.val.variavel.in > aux2->conteudo.val.variavel.in;
			if(flag==1)
				return aux->conteudo.val.variavel.fl > aux2->conteudo.val.variavel.fl;
			if(flag==2)
				return aux->conteudo.val.variavel.db > aux2->conteudo.val.variavel.db;
			if(flag==3)
				return aux->conteudo.val.variavel.ch > aux2->conteudo.val.variavel.ch;
			if(flag==4)
				return strcmp(aux->conteudo.val.variavel.str, aux2->conteudo.val.variavel.str) > 0;
			else if(strcmp(op,"<")==0) {
				flag = aux->conteudo.val.flag;
				if(flag==0)
					return aux->conteudo.val.variavel.in < aux2->conteudo.val.variavel.in;
				if(flag==1)
					return aux->conteudo.val.variavel.fl < aux2->conteudo.val.variavel.fl;
				if(flag==2)
					return aux->conteudo.val.variavel.db < aux2->conteudo.val.variavel.db;
				if(flag==3)
					return aux->conteudo.val.variavel.ch < aux2->conteudo.val.variavel.ch;
				if(flag==4)
					return strcmp(aux->conteudo.val.variavel.str, aux2->conteudo.val.variavel.str) < 0;
			} else if(strcmp(op,">=")==0) {
				flag = aux->conteudo.val.flag;
				if(flag==0)
					return aux->conteudo.val.variavel.in >= aux2->conteudo.val.variavel.in;
				if(flag==1)
					return aux->conteudo.val.variavel.fl >= aux2->conteudo.val.variavel.fl;
				if(flag==2)
					return aux->conteudo.val.variavel.db >= aux2->conteudo.val.variavel.db;
				if(flag==3)
					return aux->conteudo.val.variavel.ch >= aux2->conteudo.val.variavel.ch;
				if(flag==4)
					return strcmp(aux->conteudo.val.variavel.str, aux2->conteudo.val.variavel.str) >= 0;
			} else if(strcmp(op,"<=")==0) {
				flag = aux->conteudo.val.flag;
				if(flag==0)
					return aux->conteudo.val.variavel.in <= aux2->conteudo.val.variavel.in;
				if(flag==1)
					return aux->conteudo.val.variavel.fl <= aux2->conteudo.val.variavel.fl;
				if(flag==2)
					return aux->conteudo.val.variavel.db <= aux2->conteudo.val.variavel.db;
				if(flag==3)
					return aux->conteudo.val.variavel.ch <= aux2->conteudo.val.variavel.ch;
				if(flag==4)
					return strcmp(aux->conteudo.val.variavel.str, aux2->conteudo.val.variavel.str) <= 0;
			} else if(strcmp(op,"==")==0) {
				flag = aux->conteudo.val.flag;
				if(flag==0)
					return aux->conteudo.val.variavel.in == aux2->conteudo.val.variavel.in;
				if(flag==1)
					return aux->conteudo.val.variavel.fl == aux2->conteudo.val.variavel.fl;
				if(flag==2)
					return aux->conteudo.val.variavel.db == aux2->conteudo.val.variavel.db;
				if(flag==3)
					return aux->conteudo.val.variavel.ch == aux2->conteudo.val.variavel.ch;
				if(flag==4)
					return strcmp(aux->conteudo.val.variavel.str, aux2->conteudo.val.variavel.str) == 0;
			}
		}
	} else {
		if(aux==NULL) {
			if(aux2==NULL) {
				if(strcmp(op,"==") == 0) {
					return strcmp(prim->tokenName,terc->tokenName) == 0;
				} else if(strcmp(op,">") == 0) {
					return strcmp(prim->tokenName,terc->tokenName) > 0;
				} else if(strcmp(op,"<") == 0) {
					return strcmp(prim->tokenName,terc->tokenName) < 0;
				} else if(strcmp(op,">=") == 0) {
					return strcmp(prim->tokenName,terc->tokenName) >= 0;
				} else if(strcmp(op,"<=") == 0) {
					return strcmp(prim->tokenName,terc->tokenName) <= 0;
				}

			} else {
				flag = aux2->conteudo.val.flag;
				if(flag == 4) {
					if(strcmp(op,"==") == 0) {
						return strcmp(aux2->conteudo.val.variavel.str,prim->tokenName) == 0;
					} else if(strcmp(op,">") == 0) {
						return strcmp(aux2->conteudo.val.variavel.str,prim->tokenName) > 0;
					} else if(strcmp(op,"<") == 0) {
						return strcmp(aux2->conteudo.val.variavel.str,prim->tokenName) < 0;
					} else if(strcmp(op,">=") == 0) {
						return strcmp(aux2->conteudo.val.variavel.str,prim->tokenName) >= 0;
					} else if(strcmp(op,"<=") == 0) {
						return strcmp(aux2->conteudo.val.variavel.str,prim->tokenName) <= 0;
					}
				} else {
					if(flag==0) {
						sprintf(valor,"%d",aux2->conteudo.val.variavel.in);
						if(strcmp(op,"==") == 0) {
							return strcmp(valor,prim->tokenName) == 0;
						} else if(strcmp(op,">") == 0) {
							return strcmp(valor,prim->tokenName) > 0;
						} else if(strcmp(op,"<") == 0) {
							return strcmp(valor,prim->tokenName) < 0;
						} else if(strcmp(op,">=") == 0) {
							return strcmp(valor,prim->tokenName) >= 0;
						} else if(strcmp(op,"<=") == 0) {
							return strcmp(valor,prim->tokenName) <= 0;
						}
					} else if(flag ==1) {
						sprintf(valor,"%f",aux2->conteudo.val.variavel.fl);
						if(strcmp(op,"==") == 0) {
							return strcmp(valor,prim->tokenName) == 0;
						} else if(strcmp(op,">") == 0) {
							return strcmp(valor,prim->tokenName) > 0;
						} else if(strcmp(op,"<") == 0) {
							return strcmp(valor,prim->tokenName) < 0;
						} else if(strcmp(op,">=") == 0) {
							return strcmp(valor,prim->tokenName) >= 0;
						} else if(strcmp(op,"<=") == 0) {
							return strcmp(valor,prim->tokenName) <= 0;
						}
					} else if(flag==2) {
						sprintf(valor,"%lf",aux2->conteudo.val.variavel.db);
						if(strcmp(op,"==") == 0) {
							return strcmp(valor,prim->tokenName) == 0;
						} else if(strcmp(op,">") == 0) {
							return strcmp(valor,prim->tokenName) > 0;
						} else if(strcmp(op,"<") == 0) {
							return strcmp(valor,prim->tokenName) < 0;
						} else if(strcmp(op,">=") == 0) {
							return strcmp(valor,prim->tokenName) >= 0;
						} else if(strcmp(op,"<=") == 0) {
							return strcmp(valor,prim->tokenName) <= 0;
						}
					} else if(flag==3) {
						sprintf(valor,"%c",aux2->conteudo.val.variavel.ch);
						if(strcmp(op,"==") == 0) {
							return strcmp(valor,prim->tokenName) == 0;
						} else if(strcmp(op,">") == 0) {
							return strcmp(valor,prim->tokenName) > 0;
						} else if(strcmp(op,"<") == 0) {
							return strcmp(valor,prim->tokenName) < 0;
						} else if(strcmp(op,">=") == 0) {
							return strcmp(valor,prim->tokenName) >= 0;
						} else if(strcmp(op,"<=") == 0) {
							return strcmp(valor,prim->tokenName) <= 0;
						}
					}

				}

			}
		} else {
			flag = aux->conteudo.val.flag;
			if(flag == 4) {
				if(strcmp(op,"==") == 0) {
					return strcmp(aux->conteudo.val.variavel.str,terc->tokenName) == 0;
				} else if(strcmp(op,">") == 0) {
					return strcmp(aux->conteudo.val.variavel.str,terc->tokenName) > 0;
				} else if(strcmp(op,"<") == 0) {
					return strcmp(aux->conteudo.val.variavel.str,terc->tokenName) < 0;
				} else if(strcmp(op,">=") == 0) {
					return strcmp(aux->conteudo.val.variavel.str,terc->tokenName) >= 0;
				} else if(strcmp(op,"<=") == 0) {
					return strcmp(aux->conteudo.val.variavel.str,terc->tokenName) <= 0;
				}
			} else {
				if(flag==0) {
					sprintf(valor,"%d",aux->conteudo.val.variavel.in);
					if(strcmp(op,"==") == 0) {
						return strcmp(valor,terc->tokenName) == 0;
					} else if(strcmp(op,">") == 0) {
						return strcmp(valor,terc->tokenName) > 0;
					} else if(strcmp(op,"<") == 0) {
						return strcmp(valor,terc->tokenName) < 0;
					} else if(strcmp(op,">=") == 0) {
						return strcmp(valor,terc->tokenName) >= 0;
					} else if(strcmp(op,"<=") == 0) {
						return strcmp(valor,terc->tokenName) <= 0;
					}
				} else if(flag ==1) {
					sprintf(valor,"%f",aux->conteudo.val.variavel.fl);
					if(strcmp(op,"==") == 0) {
						return strcmp(valor,terc->tokenName) == 0;
					} else if(strcmp(op,">") == 0) {
						return strcmp(valor,terc->tokenName) > 0;
					} else if(strcmp(op,"<") == 0) {
						return strcmp(valor,terc->tokenName) < 0;
					} else if(strcmp(op,">=") == 0) {
						return strcmp(valor,terc->tokenName) >= 0;
					} else if(strcmp(op,"<=") == 0) {
						return strcmp(valor,terc->tokenName) <= 0;
					}
				} else if(flag==2) {
					sprintf(valor,"%lf",aux->conteudo.val.variavel.db);
					if(strcmp(op,"==") == 0) {
						return strcmp(valor,terc->tokenName) == 0;
					} else if(strcmp(op,">") == 0) {
						return strcmp(valor,terc->tokenName) > 0;
					} else if(strcmp(op,"<") == 0) {
						return strcmp(valor,terc->tokenName) < 0;
					} else if(strcmp(op,">=") == 0) {
						return strcmp(valor,terc->tokenName) >= 0;
					} else if(strcmp(op,"<=") == 0) {
						return strcmp(valor,terc->tokenName) <= 0;
					}
				} else if(flag==3) {
					sprintf(valor,"%c",aux->conteudo.val.variavel.ch);
					if(strcmp(op,"==") == 0) {
						return strcmp(valor,terc->tokenName) == 0;
					} else if(strcmp(op,">") == 0) {
						return strcmp(valor,terc->tokenName) > 0;
					} else if(strcmp(op,"<") == 0) {
						return strcmp(valor,terc->tokenName) < 0;
					} else if(strcmp(op,">=") == 0) {
						return strcmp(valor,terc->tokenName) >= 0;
					} else if(strcmp(op,"<=") == 0) {
						return strcmp(valor,terc->tokenName) <= 0;
					}
				}
			}
		}
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
