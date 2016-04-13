typedef char** ListagemStrings;

typedef struct paginaResultados{
    ListagemStrings resultados;
    int tamanhoLista;
    int tamanhoCadaElemento; //em nr carateres
    int indiceAtual; //so para insercoes

    int nrPaginaAtual;
    int nrElementosPorPagina;
    int posicaoInicial;
    int posicaoFinal;
} *PAGINA_RESULTADOS;

int getTamanhoLista(PAGINA_RESULTADOS pagina){
	return pagina->tamanhoLista;
}

int getTamanhoCadaElemento(PAGINA_RESULTADOS pagina){
	return pagina->tamanhoCadaElemento;
}

Codigo getnElemento(PAGINA_RESULTADOS pagina, int n){
	return (String) (pagina->resultados + getTamanhoCadaElemento(pagina) * n);
}

int getIndiceAtual(PAGINA_RESULTADOS pagina){
	return pagina->indiceAtual;
}

int getPaginaAtual(PAGINA_RESULTADOS pagina){
	return pagina->nrPaginaAtual;
}

int getPosicaoInicial(PAGINA_RESULTADOS pagina){
	return pagina->posicaoInicial;
}

int getPosicaoFinal(PAGINA_RESULTADOS pagina){
	return pagina->posicaoFinal;
}

int getNrElementosPorPagina(PAGINA_RESULTADOS pagina){
	return pagina->nrElementosPorPagina;
}

void setNrElementosPorPagina(PAGINA_RESULTADOS pagina, int n){
	int max = getIndiceAtual(pagina);

	if(n > max) pagina->nrElementosPorPagina = max;
	else pagina->nrElementosPorPagina = n;
}

void setPosicaoInicial(PAGINA_RESULTADOS pagina, int pos){
	pagina->posicaoInicial = pos;
}

void setPosicaoFinal(PAGINA_RESULTADOS pagina, int pos){
	pagina->posicaoFinal = pos;
}

void setPaginaAtual(PAGINA_RESULTADOS pagina, int pag){
	pagina->nrPaginaAtual = pag;
}

int incIndiceAtual(PAGINA_RESULTADOS pagina){
	return pagina->indiceAtual++;
}

int incPaginaAtual(PAGINA_RESULTADOS pagina){
	return pagina->nrPaginaAtual++;
}

int calcularIndiceInsercao(PAGINA_RESULTADOS pagina){
	return getIndiceAtual(pagina) * getTamanhoCadaElemento(pagina);
}

PAGINA_RESULTADOS paginaResultadosInit(int tamanhoL, int tamanhoElemento){
	PAGINA_RESULTADOS pagina = (PAGINA_RESULTADOS) malloc(sizeof(struct paginaResultados));
	pagina->resultados = (ListagemStrings) malloc(tamanhoL * (tamanhoElemento + 1));

	pagina->tamanhoLista = tamanhoL;
	pagina->tamanhoCadaElemento = tamanhoElemento;

	pagina->indiceAtual = 0;	
	pagina->nrPaginaAtual = 0;
	pagina->nrElementosPorPagina = 0;
	pagina->posicaoInicial = 0;
	pagina->posicaoFinal = 0;

	return pagina;
}

void estadoPaginacao(PAGINA_RESULTADOS pagina){
    int tamanhoLista = getTamanhoLista(pagina);
    int tamanhoCadaElemento = getTamanhoCadaElemento(pagina);
    int indiceAtual = getIndiceAtual(pagina);

    int nrPaginaAtual = getPaginaAtual(pagina);
    int nrElementosPorPagina = getNrElementosPorPagina(pagina);
    int posicaoInicial = getPosicaoInicial(pagina);
    int posicaoFinal = getPosicaoFinal(pagina);

	printf("- tamanhoLista %d - tamanhoCadaElemento %d - indiceAtual %d -\n", tamanhoLista, tamanhoCadaElemento, indiceAtual);
	printf("nrPaginaAtual %d\n", nrPaginaAtual);
	printf("nrElementosPorPagina %d\n", nrElementosPorPagina);
	printf("posicaoInicial %d\n", posicaoInicial);
	printf("posicaoFinal %d\n", posicaoFinal);
}

Codigo inserirResultadoLista(PAGINA_RESULTADOS pagina, Codigo item){
	Codigo cod = strncpy( getnElemento(pagina,getIndiceAtual(pagina)) , item , getTamanhoCadaElemento(pagina) + 1);
	incIndiceAtual(pagina);
	return cod;
}

PAGINA_RESULTADOS travessiaClientesPorLetra(CatalogoClientes catalogo, char letra){
    int i = calculaIndiceCliente(toupper(letra));
    int totalResultados = catalogo->totalClientes[i];

    TravessiaModulo trav = avl_trav_alloc();
    avl_t_init(trav, catalogo->indice[i]);

    CodigoCliente_st cliente = avl_t_next(trav);
    PAGINA_RESULTADOS pagina = paginaResultadosInit(totalResultados,strlen(cliente));
    inserirResultadoLista(pagina, cliente);

    int n = 0;
    // totalResultados = 5;
    while((cliente = avl_t_next(trav)) && n < totalResultados){
        inserirResultadoLista(pagina, cliente);
        n++;
    }

    // printf("Total clientes começados por %c: %d\n", toupper(c), getTotalClientes(catalogo, i));
    avl_trav_free(trav); //free

    return pagina;
}

bool testeUltimaPagina(PAGINA_RESULTADOS pagina){
	return getPosicaoFinal(pagina) == getIndiceAtual(pagina);
}

int calcularNrPaginasInteiras(int nrElementosTotal, int nrElementosPorPagina){
	return nrElementosTotal / nrElementosPorPagina;
}

int calcularNrElementosUltimaPagina(int nrElementosTotal, int nrElementosPorPagina){
	return nrElementosTotal % nrElementosPorPagina;
}

void calcularUltimaPag(PAGINA_RESULTADOS pagina){
	int nrPagina = calcularNrPaginasInteiras(getIndiceAtual(pagina), getNrElementosPorPagina(pagina));
	int nrElementosUltimaPag = calcularNrElementosUltimaPagina(getIndiceAtual(pagina), getNrElementosPorPagina(pagina));

	if(nrElementosUltimaPag == 0)

	setPaginaAtual(pagina, nrPagina);
	setPosicaoInicial(pagina, nrPagina * getNrElementosPorPagina(pagina));
	setPosicaoFinal(pagina, getPosicaoInicial(pagina) +  nrElementosUltimaPag - 1);
}

int calcularNrPaginasTotal(PAGINA_RESULTADOS pagina){
	int nrPaginasInteiras = calcularNrPaginasInteiras(getIndiceAtual(pagina), getNrElementosPorPagina(pagina));
	int nrElementosUltimaPag = calcularNrElementosUltimaPagina(getIndiceAtual(pagina), getNrElementosPorPagina(pagina));
	return nrElementosUltimaPag > 0 ? nrPaginasInteiras + 1 : nrPaginasInteiras;
}

int virarPagina(PAGINA_RESULTADOS pagina){
	int max = getIndiceAtual(pagina);
	int nrElemPorPag = getNrElementosPorPagina(pagina);
	// int posicaoInic = getPosicaoInicial(pagina);
	int posicaoFin = getPosicaoFinal(pagina);

	if (posicaoFin + nrElemPorPag >= max){ //chegamos ao fim
		int nrElementosUltimaPag = calcularNrElementosUltimaPagina(getIndiceAtual(pagina), getNrElementosPorPagina(pagina));
		if(nrElementosUltimaPag > 0) calcularUltimaPag(pagina);
		return 0;
	}else{
		incPaginaAtual(pagina);
		setPosicaoInicial(pagina, posicaoFin + 1);
		setPosicaoFinal(pagina, posicaoFin + nrElemPorPag);
		return getPosicaoInicial(pagina);
	}
}

void posicoesInit(PAGINA_RESULTADOS pagina){
	int nrpag = getPaginaAtual(pagina);
	int elemPorPag = getNrElementosPorPagina(pagina);

	setPosicaoInicial(pagina, nrpag * elemPorPag);
	setPosicaoFinal(pagina, getPosicaoInicial(pagina) + elemPorPag - 1);
}

void imprimirElementos(PAGINA_RESULTADOS pagina){
	int i = getPosicaoInicial(pagina);
	int fim = getPosicaoFinal(pagina);

	Codigo codigo;

	while( i  <= fim ){
		codigo = getnElemento(pagina, i);
		printf("Codigo: %s\n", codigo);
		i++;
	}
}

void percorrerPaginaResultados(PAGINA_RESULTADOS pagina, int nrPag, int elemPorPag){
	setNrElementosPorPagina(pagina, elemPorPag);
	posicoesInit(pagina);

	while(scanf("%d",&nrPag) != 0){
		printf("\tTOTAL PAG: %d\n", calcularNrPaginasTotal(pagina));
		estadoPaginacao(pagina);
		imprimirElementos(pagina);
		virarPagina(pagina);
	}
}

void travessiaTesteClientes(CatalogoClientes catalogo){
	char c = 'A';
	// scanf("%c",&c);

	PAGINA_RESULTADOS pagina = travessiaClientesPorLetra(catalogo, c);

	percorrerPaginaResultados(pagina, 1, 10);

    printf("Total clientes começados por %c: %d\n", toupper(c), getTotalClientes(catalogo, calculaIndiceCliente(c)));
}

void freeTravessiaCatalogoClientes(TravessiaModulo travessia){
    if(travessia != NULL)
        avl_trav_free(travessia);

    free(travessia);
}
