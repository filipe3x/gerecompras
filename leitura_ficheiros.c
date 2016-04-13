#include "gestao_filiais.c"
#include "catalogo_clientes.c"
#include "faturacao.c"

void abrirFicheiroVendas(String nomeFicheiro, MODULO_GESTAO_FILIAIS moduloGestaoFiliais, MODULO_FATURACAO moduloFaturacao){
	const char *modoAbertura = "r";
	FILE *ficheiroVendas = fopen(nomeFicheiro, modoAbertura);

	int i = 0;
	String linhaLida = (String) malloc(128);
	VENDA novaVenda  = linhaVendaInit(); //alocar espaco para novaVenda
	StringVenda structStrVendas;

	while(fgets(linhaLida, MAX_BUFFER_VENDAS, ficheiroVendas) && i < TAMANHO){ //max 39 caract por linha
		linhaLida[strcspn( linhaLida, "\r\n")] = 0; //apagar new lines de cada linha
		lerLinhaVenda(linhaLida,&structStrVendas); //Inserir na estrutura de Strings temporariamente
		inserirLinhaVenda(novaVenda, structStrVendas); //Interpretar valores na estrutura temporaria structStrVendas e inserir valores na estrutura novaVenda  (as strings antigas sao aproveitadas)
		// VENDA clone = linhaVendaInit(); //copia deep das estruturas contidas em novaVenda
		// clonelinhaVenda(novaVenda,clone);
		inserirVendaModuloGestaoFiliais(moduloGestaoFiliais, novaVenda); //inserir struct no modulo
		inserirVendaModuloFacturacao(moduloFaturacao, novaVenda);
		// freeLinhaVenda(clone); // free clone
		i++;
	}

	fclose(ficheiroVendas);
	free(novaVenda); // free novaVenda
	free(linhaLida);
	printf("Total linhas validadas: %d\n", moduloGestaoFiliais->totalVendas);
}

int gestaoFiliaisMain(){
	MODULO_GESTAO_FILIAIS moduloGestao = moduloGestaoFiliaisInit();
	MODULO_FATURACAO moduloFaturacao = moduloFaturacaoInit();

	abrirFicheiroVendas("Vendas_1M.txt", moduloGestao, moduloFaturacao);

	// int f;
	// scanf("%d",&f);
	// travessiaTesteGestaoFilial(moduloGestao->filial[f]);
	// travessiaFaturacao(moduloFaturacao);

	// freeModuloGestaoFiliais(moduloGestao);
	// freeModuloFaturacao(moduloFaturacao);

    return (EXIT_SUCCESS);
}

int abrirFicheiroClientes(String nomeFicheiro){
    CatalogoClientes catalogo = catalogoClientesInit();

    String linhaLida = (String) malloc(64);
    FILE *ficheiroClientes = fopen(nomeFicheiro, "r"); //abrir ficheiro clientes

    int nrlinhasLidas = 0;
    while(fgets(linhaLida, MAX_BUFFER_CATALOGO, ficheiroClientes) && nrlinhasLidas < TAMANHO){ //ler linha ficheiro
        linhaLida[strcspn( linhaLida, "\r\n")] = 0; //apagar new lines de cada linha
        inserirClienteCatalogo(catalogo, linhaLida);
        nrlinhasLidas++;
    }
    free(linhaLida);
    fclose(ficheiroClientes);

    printf("linhas lidas: %d\n",nrlinhasLidas);
    printf("linhas validas: %d\n",calcularTotalClientes(catalogo));

    travessiaTesteClientes(catalogo);
    // travessiaClientesPorLetra(catalogo,'A');

    freeCatalogoClientes(catalogo);

    return(EXIT_SUCCESS);
}

