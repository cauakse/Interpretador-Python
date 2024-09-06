void EscrMsg (char mensagem[]) {
	gotoxy(3,43);
	textcolor(12);
	printf("Mensagem: ");
	textcolor(15);
	printf("%s",mensagem);
}

void ExibirTexto(char frase[100],int Coluna, int Linha, int Cor) {
	gotoxy(Coluna,Linha);
	textcolor(Cor);
	printf("%s",frase);
	textcolor(15);
}

void LimpaMsg (void) {
	gotoxy(3,43);
	printf("                                                                                                                    ");
}

void MsgFim (void) {
	textcolor(14);
	gotoxy(40,3);
	printf("TRABALHO BIMESTRAL - ESTRUTURAS DE DADOS II");
	gotoxy(32,10);
	printf("INTERPRETADOR DE PROGRAMAS EM PYTHON UTILIZANDO LINGUAGEM C");
	gotoxy(43,14);
	printf("102316546 - CAUA PEREIRA DOMINGUES");
	gotoxy(41,15);
	printf("102421080 - MATHEUS OLIVEIRA DA SILVA");
	gotoxy(45,43);
	printf("PROF. DR. FRANCISCO ASSIS DA SILVA");
}
