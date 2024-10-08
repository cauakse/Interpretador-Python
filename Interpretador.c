#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <conio2.h>
#include <windows.h>
#include "Headers\\Mensagem.h"
#include "Headers\\ListaToken.h"
#include "Headers\\PilhaVar.h"
#include "Headers\\Moldura.h"
#include "Headers\\Funcoes.h"
#include "Headers\\TADListaGen.h"
#include "Headers\\TadListaGenContas.h"

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

void Executa(Token *Tok, Pilha **pVar, Funcoes *Funcoes) {
	Pilha *P = *pVar;
	Token *T;
	char expressao[100], string[100];
	expressao[0]='\0';
	switch(whatsIt(Tok,*pVar,Funcoes)) {
		case 1://caso for variavel ja definida
			P=*pVar;
			if(Tok->prox)
				if(!strcmp(Tok->prox->tokenName,"=")) {
					if(Tok->prox->prox->tokenName[0]>='0' && Tok->prox->prox->tokenName[0]<='9') {
						while(P && strcmp(P->conteudo.nomeVar,Tok->tokenName))
							P = P->prox;
						T = Tok->prox->prox;
						while(T) {
							strcat(expressao,T->tokenName);
							T = T->prox;
						}

						P->conteudo.val.flag = 1;
						P->conteudo.val.variavel.fl=resolveExpressao(expressao);
					} else {
						P->conteudo.val.flag = 4;
						strcpy(string,"");
						Tok = Tok->prox->prox->prox;
						strcat(string,Tok->tokenName);
						Tok = Tok->prox;
						while(Tok && (Tok->tokenName[0]!=39 && Tok->tokenName[0]!=34)) {
							strcat(string," ");
							strcat(string,Tok->tokenName);
							Tok = Tok->prox;
						}
						strcpy(P->conteudo.val.variavel.str,string);
					}
				}
			break;
		case 2://caso for função do cabra
			break;
		case 3://caso função do sistema
			if(strcmp(Tok->tokenName,"if")==0)
				resolveIf(Tok,pVar);
			if(strcmp(Tok->tokenName,"while")==0)
				break;
			if(strcmp(Tok->tokenName,"for")==0)
				break;
			if(strcmp(Tok->tokenName,"do")==0)
				break;
			break;
		case 5: //implementação futura
			break;
		case 0://definição de função
			break;
		default: //se nao achou nada, então só pode ser criação de uma nova variavel
			createNewVar(Tok->tokenName,&(*pVar));
			P=*pVar;
			if(Tok->prox!=NULL)
				if(strcmp(Tok->prox->tokenName,"=")==0) {
					if(Tok->prox->prox->tokenName[0]>='0' && Tok->prox->prox->tokenName[0]<='9') {
						while(P && strcmp(P->conteudo.nomeVar,Tok->tokenName))
							P = P->prox;
						T = Tok->prox->prox;
						while(T) {
							strcat(expressao,T->tokenName);
							T = T->prox;
						}

						P->conteudo.val.flag=1;
						P->conteudo.val.variavel.fl=resolveExpressao(expressao);
					} else {
						P->conteudo.val.flag = 4;
						strcpy(string,"");
						Tok = Tok->prox->prox->prox;
						strcat(string,Tok->tokenName);
						Tok = Tok->prox;
						while(Tok && (Tok->tokenName[0]!=39 && Tok->tokenName[0]!=34)) {
							strcat(string," ");
							strcat(string,Tok->tokenName);
							Tok = Tok->prox;
						}
						strcpy(P->conteudo.val.variavel.str,string);
					}
				}
	}
	// Após a criação das variáveis, executa-se a função correspondente armazenando seus resultados na pVar
}

void ExibirPilhaVar (Pilha *P) {
	int c=46, l=38, end = 100;

	LimpaTelaMem();
	Moldura(45,12,118,40,9,0);
	Moldura(46,13,117,15,9,0);
	ExibirTexto((char*)"PILHA DE VARIAVEIS",74,14,12);
	for(l=16; l<40; l++)
		ExibirTexto("       |                            |",c,l,9);
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
		switch(P->conteudo.val.flag) {
			case 0:
				printf("%d", P->conteudo.val.variavel.in);
				break;
			case 1:
				printf("%f", P->conteudo.val.variavel.fl);
				break;
			case 2:
				printf("%lf", P->conteudo.val.variavel.db);
				break;
			case 3:
				printf("%c", P->conteudo.val.variavel.ch);
				break;
			case 4:
				printf("%s", P->conteudo.val.variavel.str);
				break;
			case 5:
				printf("%u", P->conteudo.val.variavel.pont);
				break;
			case 6:
				printf("UNDEFINED");
				break;
		}
		l--;
		end += 4;
		P = P->ant;
	}
}

Valor BuscaVariavel (Token *Var, Pilha *Pilha) {
	Valor ValorVar;
	ValorVar.flag = -1;
	while(Pilha && ValorVar.flag == -1) {
		if(!strcmp(Var->tokenName,Pilha->conteudo.nomeVar)) {
			ValorVar.flag = Pilha->conteudo.val.flag;
			ValorVar.variavel = Pilha->conteudo.val.variavel;
		}
		Pilha = Pilha->prox;
	}
	return ValorVar;
}

void ExibirPrint(Token *token, Pilha *pilhaVar) {
	Valor val;
	Token *var;
	char string[100], aux[100], valor[10], mascara=0;

	val.flag = -1;
	strcpy(string,"");

	if(strcmp(token->tokenName,"print"))
		EscrMsg("A LINHA NAO CONTEM FUNCAO PRINT");
	else {
		token = token->prox;
		if(token->tokenName[0] == '(') {
			token = token->prox;
			if(token->tokenName[0] == 34 || token->tokenName[0] == 39) {
				token = token->prox;
				while(token && (token->tokenName[0]!=34 && token->tokenName[0]!=39)) {
					if(strcmp(token->tokenName,"%d") && strcmp(token->tokenName,"%f") && strcmp(token->tokenName,"%c") && strcmp(token->tokenName,"%s")) {
						// Se não for uma máscara ele deve concatenar com a string a ser apresentada
						strcpy(aux," ");
						strcat(aux,token->tokenName);
						strcat(string,aux);
					} else {
						// Caso encontre uma máscara ele deve procurar a variável e verificar sua existência.
						mascara = 1;
						if(val.flag == -1) {
							var = token;
							while(var && strcmp(var->tokenName,"%"))
								var = var->prox;
							if(var->prox->tokenName[0] == '(')
								var = var->prox;
							var = var->prox;
							val = BuscaVariavel(var,pilhaVar);
						} else {
							var = var->prox;
							val = BuscaVariavel(var,pilhaVar);
						}
						if(val.flag != -1) {
							switch(token->tokenName[1]) {
								case 'd': // Int
									sprintf(valor," %.0f",val.variavel.fl);
									break;
								case 'f': // Float
									sprintf(valor," %.6f",val.variavel.fl);
									break;
								case 2: // Double
									sprintf(valor," %.6f",val.variavel.fl);
									break;
								case 'c': // Char
									sprintf(valor," %d",val.variavel.fl);
									break;
								case 's': // String
									strcpy(valor," ");
									strcat(valor,val.variavel.str);
							}
							strcat(string,valor);
						}
					}
					token = token->prox;
				}
				if(!mascara) {
					var = token;
					while(var && (var->tokenName[0] != 34 && var->tokenName[0] != 39))
						var = var->prox;
					while(var && var->tokenName[0] != ')') {
						if(var->tokenName[0] != ',' || var->tokenName[0] != '+') {
							val = BuscaVariavel(var,pilhaVar);
							if(val.flag != -1) {
								if(val.flag == 1)
									sprintf(valor," %.0f",val.variavel.fl);
								else {
									strcpy(valor," ");
									strcat(valor,val.variavel.str);
								}
								strcat(string,valor);
							}
						}
						var = var->prox;
					}
				}
			}
		}
		EscrMsg("");
		LimpaPrint();
		Moldura(20,10,110,16,9,0);
		Moldura(21,11,109,13,9,0);
		ExibirTexto((char*)"RESULTADO DO PRINT",53,12,14);
		gotoxy(22,14);
		printf("%s",string);
		textcolor(15);
		getch();
	}
}

void ExecPassos (List *L) {
	char op;
	int LinhaAtual = 0;
	Token *ant = NULL;
	List *Linhas = L;
	Pilha *pilhaDeVariaveis = NULL; // Definindo a pilha de variaveis vazia
	Funcoes *functions;

	createFunctionsList(&functions,L);// Listando todas as funcoes da lista
	LinhaAtual = PonteiroInicial(&L);

	LimpaTela();
	ExibirTexto((char*)"EXECUCAO DO PROGRAMA:",6,8,14);

	do {
		LimpaTela();
		ExibirPrograma(Linhas,LinhaAtual); // Exibe o conteúdo do arquivo com efeito na linha atual .py
		LimpaMsg();
		EscrMsg((char*)"[ENTER] - PROXIMA LINHA, [F9] - CONTEUDO MEM. RAM, [F10] - EXIBIR RESULTADOS OU [ESC] PARA VOLTAR");

		Executa(L->pToken,&pilhaDeVariaveis,functions);// Executa a linha e atualiza a pilha de variáveis com resultados.

		op = getch();

		switch(op) {
			case 0:
				op = getch();
				switch(op) {
					case 67:
						LimpaMsg();
						ExibirPilhaVar(pilhaDeVariaveis);
						EscrMsg((char*)"CONTEUDO DA MEMORIA RAM");
						break;
					case 68:
						LimpaMsg();
						ExibirPrint(L->pToken,pilhaDeVariaveis);
						EscrMsg((char*)"EXIBICAO DOS RESULTADOS");
				}
				getch();
				break;
			case 13: // Andando nas linhas do programa
				if(L->prox) {
					L = L->prox;
					LinhaAtual++;
					// Tokens que não são Linhas de Código.
					if(!strcmp(L->pToken->tokenName,"fimdef") || !strcmp(L->pToken->tokenName,"fim"))
						L = L->prox;
					if(!strcmp(L->pToken->tokenName,"if")) {
						if(resolveIf(L->pToken,pilhaDeVariaveis) == 1) {
							ant = L->pToken;
							L = L->prox;
							LinhaAtual++;
						} else {
							while(L && strcmp(L->pToken->tokenName,"fim")) {
								L = L->prox;
								LinhaAtual++;
							}
							if(!strcmp(L->pToken->tokenName,"fim")) {
								L = L->prox;
								if(!strcmp(L->pToken->tokenName,"else")) {
									L = L->prox;
									LinhaAtual++;
								}
							}
						}
					}
					if(!strcmp(L->pToken->tokenName,"else")) {
						printf("else");
						getch();
						if(ant) {
							ant = NULL;
							while(L && strcmp(L->pToken->tokenName,"fim")) {
								L = L->prox;
								LinhaAtual++;
							}
							if(!strcmp(L->pToken->tokenName,"fim"))
								L = L->prox;
						}
					}
				} else
					op=27;
		}
	} while(op!=27);
}

void AbrirArquivo (List **L) {
	FILE *arq;
	char op, arquivo[100];
	int i=0;

	LigaCursor();
	Moldura(40,10,90,16,9,0);
	Moldura(41,11,89,13,9,0);
	ExibirTexto((char*)"DIGITE O NOME DO ARQUIVO",53,12,14);
	ExibirTexto((char*)"~ ",42,14,12);
	textcolor(15);
	gets(arquivo);
	RetiraCursor();

	arq = fopen(arquivo,"r");

	LimpaMsg();
	if(!arq) {
		EscrMsg((char*)"ARQUIVO NAO ENCONTRADO");
		getch();
	} else {
		EscrMsg("");
		printf("ARQUIVO %s ABERTO COM SUCESSO, PRESSIONE QUALQUER TECLA", arquivo);
		getch();

		createListOfLines(arq, &(*L)); // Cria a lista de linhas do arquivo .py.
		fclose(arq); //Fecha o arquivo e a partir daqui trabalha apenas com ponteiros.

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
