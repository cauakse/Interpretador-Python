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

void ExibirPrograma(FILE *arq, int *LinhaAtual) {
	char linha[100];
	int l=10, c=34;

	rewind(arq);

	fgets(linha,100,arq);
	while(!feof(arq)) {
		if(l==(*LinhaAtual+9)) {
			textbackground(14);
			textcolor(0);
		}
		gotoxy(c,l);
		printf("%s",linha);
		textbackground(0);
		textcolor(15);
		l++;
		fgets(linha,100,arq);
	}
}

void ConteudoArquivo(FILE *arq) {
	char linha[100];
	int l=10, c=34;

	rewind(arq);
	fgets(linha,100,arq);
	while(!feof(arq)) {
		gotoxy(c,l);
		printf("%s",linha);
		l++;
		fgets(linha,100,arq);
	}
}

int ContLinha (FILE *arq) {
	char linha[100];
	int tam=0;

	rewind(arq);
	fgets(linha,100,arq);
	while(!feof(arq)) {
		tam++;
		fgets(linha,100,arq);
	}
	return tam;
}

void ExecPassos (List *L, FILE *arq) {
	char op;
	int LinhaAtual=1, qtdeL;

	qtdeL = ContLinha(arq);
	LimpaTela();
	ExibirTexto((char*)"EXECUCAO DO PROGRAMA:",34,8,14);

	do {
		ExibirPrograma(arq,&LinhaAtual);
		op = getch();

		switch(op) {
			case 13:
				LinhaAtual++;
				if(LinhaAtual>qtdeL)
					op = 27;
		}
	} while(op!=27);
}

void AbrirArquivo (char arquivo[100], List **L) {
	FILE *arq = fopen(arquivo,"r");
	char op;

	LimpaMsg();
	if(arq == NULL)
		EscrMsg((char*)"ARQUIVO NAO ENCONTRADO");
	else {
		EscrMsg("");
		printf("ARQUIVO %s ABERTO COM SUCESSO, PRESSIONE QUALQUER TECLA", arquivo);
		getch();

		createListOfLines(arq, &(*L));

		LimpaTela();
		gotoxy(34,6);
		textcolor(12);
		printf("%s",arquivo);
		textcolor(15);
		ExibirTexto((char*)"CONTEUDO DO ARQUIVO:",34,8,14);

		ConteudoArquivo(arq);

		LimpaMsg();
		EscrMsg((char*)"PRESSIONE [F8] PARA EXECUTAR PASSO A PASSO OU [ESC] PARA VOLTAR");

		do {
			op = getch();
			if(op == 0) {
				op = getch();
				if(op == 66) {
					ExecPassos((*L),arq); // Executa passo a passo do programa!
					LimpaMsg();
					LimpaTelaInteira();
					EscrMsg("EXECUCAO FINALIZADA, PRESSIONE [ESC] PARA SAIR");
				}
			}
		} while(op != 27);
		fclose(arq);
	}
}

void Menu (void) {
	textcolor(14);
	ExibirTexto((char*)"TRABALHO BIMESTRAL - ESTRUTURAS DE DADOS II",40,3,14);
	LimpaMsg();
	EscrMsg((char*)"PRESSIONE [F7] PARA ABRIR O ARQUIVO .PY OU [ESC] PARA SAIR");
}

int main() {
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
