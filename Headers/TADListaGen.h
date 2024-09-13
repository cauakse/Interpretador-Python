// Modelo conceitual da aula

union info_lista
{
	char info[8];
	struct reg_lista lista;
};

struct reg_lista {
	struct listagen *cabeca;
	struct listagen *cauda;
};

struct listagen {
	char terminal;
	union info_lista no;
};
typedef structlistagen ListaGen;


