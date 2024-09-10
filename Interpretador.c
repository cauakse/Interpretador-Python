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

int ProximaLinha (char token[30], FILE *arq, int Linha) {
	char linha[100], tokenArq[30];
	int i=0,j;

	rewind(arq);

	//Encontrando a linha atual dentro do arquivo
	fgets(linha,100,arq);
	while(!feof(arq) && i<Linha) {
		fgets(linha,100,arq);
		i++;
	}

	//Encontrando a primeira LETRA da linha
	i=0;
	while(i<strlen(linha) && !((linha[i] > 64 && linha[i] < 91) || (linha[i] > 96 && linha[i] < 123)))
		i++;
	j=0;
	//Armazenando o primeiro token para comparacao
	while ((i<strlen(linha)) &&
	        ((linha[i] > 47 && linha[i] < 58) ||
	         (((linha[i] > 64 && linha[i] < 91) ||
	           (linha[i] > 96 && linha[i] < 123)))))
		tokenArq[j++] = linha[i++];
	tokenArq[j] = '\0';

	LimpaMsg();
	EscrMsg("");
	printf("PTR: %s // ARQ: %s",token,tokenArq);

	if(strcmp(token,tokenArq)==0)
		return Linha;
	return ProximaLinha(token,arq,Linha+1);
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
		printf("EXECUTANDO %s", L->pToken->tokenName);*/

		//Executando os tokens
		/*atToken=L->pToken;
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

void AbrirArquivo (char arquivo[100], List **L) {
	FILE *arq = fopen(arquivo,"r");
	char op;
	int i=0;

	LimpaMsg();
	if(arq == NULL)
		EscrMsg((char*)"ARQUIVO NAO ENCONTRADO");
	else {
		EscrMsg("");
		printf("ARQUIVO %s ABERTO COM SUCESSO, PRESSIONE QUALQUER TECLA", arquivo);
		getch();

		createListOfLines(arq, &(*L)); // Cria a lista de linhas do arquivo .py
		fclose(arq);

		if(!L)
			EscrMsg((char*)"ERRO");
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
						ExecPassos((*L)); // Executa passo a passo do programa
						LimpaMsg();
						EscrMsg((char*)"EXECUCAO FINALIZADA PRESSIONE [F8] PARA EXECUTAR NOVAMENTE OU [ESC] PARA SAIR");
					}
				}
			} while(op != 27);
		}
	}
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
	RetiraCursor();
	MolduraCompleta();

	do {
		Menu();
		op = getch();
		if(op == 0) {
			op = getch();
			if(op == 65)
				AbrirArquivo("teste.py",&L);
		}
		LimpaTelaInteira();
	} while(op!=27);

	//Teste de criação de Tokens
	LimpaTela();
	LimpaMsg();
	exibe(L);
	//Teste de criação de Tokens

	getch();

	MoldFim();
	MsgFim();
	getch();

	return 0;
}
