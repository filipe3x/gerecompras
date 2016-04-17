#ifndef CAT_CLIENTES
#define CAT_CLIENTES
typedef struct catalogoClientes *CatalogoClientes;

CatalogoClientes catalogoClientesInit( void );

void freeCatalogoClientes( CatalogoClientes );

CodigoCliente_st inserirClienteCatalogo( CatalogoClientes , CodigoCliente_st );

bool existeClienteCatalogo( CatalogoClientes , CodigoCliente_st );

int calcularTotalClientes( CatalogoClientes );

Modulo getCatalogoClientesPorLetra( CatalogoClientes , char );

int getTotalClientesPorIndice( CatalogoClientes , int );

int calculaIndiceCliente( char );

PAGINA_RESULTADOS travessiaClientesPorLetra( CatalogoClientes , char );

// void travessiaTesteClientes(CatalogoClientes catalogo);

int comparaCliente(const void *cliente_a, const void *cliente_b, void *param);

#endif /* CAT_CLIENTES */
