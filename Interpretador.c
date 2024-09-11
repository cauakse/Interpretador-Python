#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio2.h>
#include <windows.h>
#include "Headers\\ListaToken.h"
#include "Headers\\PilhaVar.h"
#include "Headers\\Mensagem.h"
#include "Headers\\Moldura.h"
#include "Headers\\FuncoesPy.h"

void ExibirPrograma(List *L, int LinhaAtual) {
	int l=10, c=34;

	while(L) {
		if(l==(LinhaAtual+10)) {
			textbackground(14);
			textcolor(0);
		}
		LimpaLinha(l);
		gotoxy(c,l);
		if(!strcmp(L->pToken->tokenName,"fimdef") || !strcmp(L->pToken->tokenName,"fim"))
			L = L->prox;
		printf("%s",L->pToken->tokenText);
		textbackground(0);
		textcolor(15);
		l++;
		L = L->prox;
	}
}

void ConteudoArquivo(List *L) {
	char linha[100];
	int l=10, c=34;

	while(L) {
		gotoxy(c,l);

		if(!strcmp(L->pToken->tokenName,"fimdef") || !strcmp(L->pToken->tokenName,"fim"))
			L = L->prox;
		printf("%s",L->pToken->tokenText);
		l++;
		L = L->prox;
	}
}

// Algoritmo recursivo para destruir os tokens
void DestroiLinha(Token **Token) {
	if(*Token) {
		DestroiLinha(&(*Token)->prox);
		free(*Token);
		*Token = NULL;
	}
}

// Algoritmo recursivo para destruir os elementos da lista
void DestroiLista(List **L) {
	if(*L) {
		DestroiLinha(&(*L)->pToken);
		DestroiLista(&(*L)->prox);
		free(*L);
		*L = NULL;
	}
}

void createFunctionsList (Funcoes **F, List *L) {
	Funcoes * aux, *ultimo;
	Token *tAux;

	*F = NULL;
	while(L!=NULL) {
		if(strcmp(L->pToken->tokenName,"def")==0) {
			aux=(Funcoes*)malloc(sizeof(Funcoes));
			tAux=L->pToken->prox;
			strcpy(aux->nome,tAux->tokenName);
			aux->inicio=L;
			aux->prox=NULL;
			if(*F==NULL) {
				*F=aux;
				ultimo=*F;
			} else {
				ultimo->prox=aux;
				ultimo=aux;
			}
		}
		L=L->prox;
	}
}

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
	        strcmp(T->tokenName,"switch")==0)
		return 3;// Se for função do sistema

	return 10;// Se for criação de variavel
}

void createNewVar(char nome[45],Pilha ** P) {
	Pilha *aux = (Pilha*)malloc(sizeof(Pilha));
	aux->prox=*P;
	aux->ant=NULL;
	strcpy(aux->conteudo.nomeVar,nome);
	aux->conteudo.val.flag=6;// Deixando a variável com tipo indefinido
	if(*P!=NULL) {
		(*P)->ant=aux;
	} else {
		aux->prox=NULL;
	}
	*P=aux;

	system("cls");
	aux=*P;
	while(aux!=NULL) {
		printf("Nome: %s\n",aux->conteudo.nomeVar);
		aux=aux->prox;
	}
	getche();
}

void ExecPassos (List *L) {
	char op;
	int LinhaAtual=0;
	List *Linhas = L;
	Token *atToken;
	Pilha *pilhaDeVariaveis=NULL; // Definindo a pilha de variaveis vazia
	Funcoes *functions;

	createFunctionsList(&functions,L);// Listando todas as funcoes da lista

	LimpaTela();
	ExibirTexto((char*)"EXECUCAO DO PROGRAMA:",34,8,14);

	do {
		ExibirPrograma(Linhas,LinhaAtual); // Exibe o conteúdo do arquivo com efeito na linha atual .py
		LimpaMsg();
		EscrMsg((char*)"[ENTER] - PROXIMA LINHA, [F9] - CONTEUDO MEM. RAM, [F10] - EXIBIR RESULTADOS OU [ESC] PARA VOLTAR");

		// Apenas para testar em qual Token o programa está passando durante o desenvolvimento
		/*LimpaMsg();
		EscrMsg("");
		printf("%d", LinhaAtual);
		printf("EXECUTANDO %s", L->pToken->tokenName);

		//Executando os tokens
		atToken=L->pToken;
		switch(whatsIt(atToken,pilhaDeVariaveis,functions)) {
			case 1://caso for variavel ja definida
				EscrMsg("opa");
				break;
			case 2://caso for função do cabra
				EscrMsg("opa2");
				break;
			case 3://caso função do sistema
				EscrMsg("opa3");
				break;
			case 4://caso for print
				EscrMsg("opa4");
				break;
			case 5: //implementação futura
				EscrMsg("opa5");
				break;
			case 0://definição de função
				break;
			default: //se nao achou nada, então só pode ser criação de uma nova variavel
				createNewVar(atToken->tokenName,&pilhaDeVariaveis);
		}*/

		op = getch();

		switch(op) {
			case 0:
				op = getch();
				switch(op) {
					case 67:
						LimpaMsg();
						EscrMsg((char*)"CONTEUDO DA MEMORIA RAM");
						getch();
						break;
					case 68:
						LimpaMsg();
						EscrMsg((char*)"EXIBICAO DOS RESULTADOS");
						getch();
						break;
				}
				break;
			case 13: // Andando nas linhas do programa
				if(L->prox) {
					L = L->prox;
					LinhaAtual++;
					// Tokens que não são Linhas de Código.
					if(!strcmp(L->pToken->tokenName,"fimdef") || !strcmp(L->pToken->tokenName,"fim"))
						L = L->prox;
					//	if(L)
					//		LinhaAtual = ProximaLinha(L->pToken->tokenName,arq,LinhaAtual+1);
				} else
					op=27;
		}
	} while(op!=27);
}

void AbrirArquivo (List **L) {
	FILE *arq;
	char op, arquivo[100];
	int i=0;

	/*LigaCursor();
	Moldura(40,10,90,16,9,0);
	Moldura(41,11,89,13,9,0);
	ExibirTexto((char*)"DIGITE O NOME DO ARQUIVO",53,12,14);
	ExibirTexto((char*)"~ ",42,14,12);
	textcolor(15);
	gets(arquivo);
	RetiraCursor();

	arq = fopen(arquivo,"r");*/
	arq = fopen("teste2.py","r");

	LimpaMsg();
	if(!arq) {
		EscrMsg((char*)"ARQUIVO NAO ENCONTRADO");
		getch();
	} else {
		EscrMsg("");
		printf("ARQUIVO %s ABERTO COM SUCESSO, PRESSIONE QUALQUER TECLA", arquivo);
		getch();

		createListOfLines(arq, &(*L)); // Cria a lista de linhas do arquivo .py
		fclose(arq); //Fecha o arquivo e a partir daqui trabalha apenas com ponteiros

		if(!L)
			EscrMsg((char*)"ERRO! ARQUIVO VAZIO");
		else {
			LimpaTela();
			gotoxy(34,6);
			textcolor(12);
			printf("%s",arquivo);
			textcolor(15);
			ExibirTexto((char*)"CONTEUDO DO ARQUIVO:",34,8,14);

			ConteudoArquivo(*L);

			LimpaMsg();
			EscrMsg((char*)"PRESSIONE [F8] PARA EXECUTAR PASSO A PASSO OU [ESC] PARA VOLTAR");

			do {
				op = getch();
				if(op == 0) {
					op = getch();
					if(op == 66) {
						ExecPassos(*L); // Executa passo a passo do programa
						LimpaMsg();
						EscrMsg((char*)"EXECUCAO FINALIZADA PRESSIONE [F8] PARA EXECUTAR NOVAMENTE OU [ESC] PARA SAIR");
					}
				} else if (op == 'x') {
					clrscr();
					exibe(*L);
					getch();
				}
			} while(op != 27);
		}
	}
	if(*L)
		DestroiLista(&(*L));
}

void Menu (void) {
	textcolor(14);
	ExibirTexto((char*)"TRABALHO BIMESTRAL - ESTRUTURAS DE DADOS II",40,3,14);
	LimpaMsg();
	EscrMsg((char*)"PRESSIONE [F7] PARA ABRIR O ARQUIVO .PY OU [ESC] PARA SAIR");
}

int main(void) {
	List *L = NULL;
	char op;

	SetConsoleTitle("Interpretador Python");
	Dimensao();
	MolduraCompleta();
	RetiraCursor();

	do {
		LimpaTelaInteira();
		Menu();
		op = getch();
		if(op == 0) {
			op = getch();
			if(op == 65)
				AbrirArquivo(&L);
		}
		LimpaTelaInteira();
	} while(op!=27);

	//Teste de criação de Tokens
	LimpaTela();
	LimpaMsg();
	exibe(L);
	getch();

	MoldFim();
	MsgFim();
	getch();

	return 0;
}
