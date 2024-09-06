struct valor {
    char flag;
    union variavel;
};
typedef struct valor Valor;

struct conteudo {
    char id;
    Valor val;
    List *pont;
};
typedef struct conteudo Conteudo;

struct pilha {
    struct pilha *prox, *ant;
    Conteudo conteudo;
};
typedef struct pilha Pilha;


