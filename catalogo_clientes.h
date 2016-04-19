#ifndef CAT_CLIENTES
#define CAT_CLIENTES
typedef struct catalogoClientes *CATALOGO_CLIENTES;

CATALOGO_CLIENTES catalogoClientesInit( void );

void freeCatalogoClientes( CATALOGO_CLIENTES );

CodigoCliente_st inserirClienteCatalogo( CATALOGO_CLIENTES , CodigoCliente_st );

bool existeClienteCatalogo( CATALOGO_CLIENTES , CodigoCliente_st );

int comparaClienteAlfabeticamente(const void *cliente_a, const void *cliente_b, void *param);

int calcularTotalClientes( CATALOGO_CLIENTES );

Modulo getCatalogoClientesPorLetra( CATALOGO_CLIENTES , char );

int getTotalClientesPorIndice( CATALOGO_CLIENTES , int );

int calculaIndiceCliente( char );

PAGINA_RESULTADOS travessiaClientesPorLetra( CATALOGO_CLIENTES , char );

PAGINA_RESULTADOS testarTravessiaClientes(CATALOGO_CLIENTES catalogo, int (*funcaoComparacao)(), void (*funcaoImpressao)() );

#endif /* CAT_CLIENTES */
