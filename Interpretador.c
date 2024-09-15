#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio2.h>
#include <windows.h>
#include "Headers\\Mensagem.h"
#include "Headers\\ListaToken.h"
#include "Headers\\PilhaVar.h"
#include "Headers\\Moldura.h"
#include "Headers\\FuncoesPy.h"
#include "Headers\\Funcoes.h"
#include "Headers\\TADListaGen.h"

void ExibirPrograma(List *L, int LinhaAtual) {
	int l=10, c=6;

	while(L) {
		if(l==(LinhaAtual+10)) {
			textbackground(14);
			textcolor(0);
		}
		LimpaLinha(l);
		gotoxy(c,l);
		while(!strcmp(L->pToken->tokenName,"fimdef") || !strcmp(L->pToken->tokenName,"fim"))
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
	int l=10, c=6;

	while(L) {
		gotoxy(c,l);

		while(!strcmp(L->pToken->tokenName,"fimdef") || !strcmp(L->pToken->tokenName,"fim"))
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

int PonteiroInicial (List **L) {
	List *aux;
	int Linha=0, contLinha=0;

	aux = *L;
	while(aux) {
		if(!strcmp(aux->pToken->tokenName,"fimdef")) {
			*L = aux;
			Linha += contLinha;
			contLinha = 0;
		}
		if(strcmp(aux->pToken->tokenName,"fimdef") && strcmp(aux->pToken->tokenName,"fim"))
			contLinha++;
		aux = aux->prox;
	}
	*L = (*L)->prox;
	return Linha;
}

void Executa(Token *Token, Pilha **pVar, Funcoes *Funcoes) {

	switch(whatsIt(Token,*pVar,Funcoes)) {
		case 1://caso for variavel ja definida
			EscrMsg("opa");
			break;
		case 2://caso for fun��o do cabra
			EscrMsg("opa2");
			break;
		case 3://caso fun��o do sistema
			EscrMsg("opa3");
			break;
		case 4://caso for print
			EscrMsg("opa4");
			break;
		case 5: //implementa��o futura
			EscrMsg("opa5");
			break;
		case 0://defini��o de fun��o
			break;
		default: //se nao achou nada, ent�o s� pode ser cria��o de uma nova variavel
			createNewVar(Token->tokenName,&(*pVar));
	}
	// Ap�s a cria��o das vari�veis, executa-se a fun��o correspondente armazenando seus resultados na pVar
}

void ExibirPilhaVar (Pilha *P) {
	int c=46, l=38, end = 100;
	
	LimpaTelaMem();
	Moldura(45,12,118,40,9,0);
	Moldura(46,13,117,15,9,0);
	ExibirTexto((char*)"PILHA DE VARIAVEIS",74,14,12);
	for(l=16;l<40;l++)
		ExibirTexto("       |                            |",c,l,9);
	//	MolduraFina(46,l,117,l+2,9,0);
	
	gotoxy(48,16);
	printf("END.");
	gotoxy(56,16);
	printf("VARIAVEL");
	gotoxy(84,16);
	printf("VALOR");
	
	while(P->prox)
		P = P->prox;
	l=39;
	c=48;
	while(P) {
		gotoxy(c,l);
		printf("%d",end);
		gotoxy(c+8,l);
		printf("%s", P->conteudo.nomeVar);
		gotoxy(c+36,l);
		printf("%.d", P->conteudo.val);
		l--;
		end+=4;
		P = P->ant;
	}
}

void ExecPassos (List *L) {
	char op;
	int LinhaAtual = 0;
	List *Linhas = L;
	Pilha *pilhaDeVariaveis = NULL; // Definindo a pilha de variaveis vazia
	Funcoes *functions;

	createFunctionsList(&functions,L);// Listando todas as funcoes da lista
	LinhaAtual = PonteiroInicial(&L);

	LimpaTela();
	ExibirTexto((char*)"EXECUCAO DO PROGRAMA:",6,8,14);

	do {
		LimpaTela();
		ExibirPrograma(Linhas,LinhaAtual); // Exibe o conte�do do arquivo com efeito na linha atual .py
		LimpaMsg();
		EscrMsg((char*)"[ENTER] - PROXIMA LINHA, [F9] - CONTEUDO MEM. RAM, [F10] - EXIBIR RESULTADOS OU [ESC] PARA VOLTAR");

		Executa(L->pToken,&pilhaDeVariaveis,functions);// Executa a linha e atualiza a pilha de vari�veis com resultados.

		// Apenas para testar em qual Token o programa est� passando durante o desenvolvimento
		/*LimpaMsg();
		EscrMsg("");
		printf("%d", LinhaAtual);
		printf("EXECUTANDO %s", L->pToken->tokenName);*/

		op = getch();

		switch(op) {
			case 0:
				op = getch();
				switch(op) {
					case 67:
						LimpaMsg();
						ExibirPilhaVar(pilhaDeVariaveis);
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
					// Tokens que n�o s�o Linhas de C�digo.
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

	/*arq = fopen(arquivo,"r");
	*/arq = fopen("teste.py","r");

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
			gotoxy(6,6);
			textcolor(12);
			printf("%s",arquivo);
			textcolor(15);
			ExibirTexto((char*)"CONTEUDO DO ARQUIVO:",6,8,14);

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

	//Teste de cria��o de Tokens
	LimpaTela();
	LimpaMsg();
	exibe(L);
	getch();

	MoldFim();
	MsgFim();
	getch();

	return 0;
}
