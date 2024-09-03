#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Headers\\ListaToken.h"

int main() {
    List *l = NULL;

    createListOfLines("teste.py", &l);
    exibe(l);
}