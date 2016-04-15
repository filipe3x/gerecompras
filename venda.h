#ifndef _VENDA
#define _VENDA
typedef char* Codigo;
typedef char* String;

typedef char* CodigoProduto_st;
typedef double Preco_st;
typedef int Quantidade_st;
typedef int TipoVenda_st;
typedef char* CodigoCliente_st;
typedef int Mes_st;
typedef int Filial_st;

typedef struct stringLinhaVenda* StringVenda;
typedef struct linhaVenda* VENDA;
typedef VENDA Venda;

#ifndef NRFILIAIS
#define NRFILIAIS 3
#endif
#define VENDA_NORMAL 0
#define VENDA_PROMOCIONAL 1

extern const String Mes[];

Codigo cloneCodigo( Codigo );
void freeCodigo( Codigo );

VENDA linhaVendaInit( void );
void freeLinhaVENDA( VENDA );

StringVenda stringVendaInit( void );
void freeStringVenda( StringVenda );

CodigoProduto_st getCodigoProduto( VENDA );
Preco_st getPreco( VENDA );
Quantidade_st getQuantidade( VENDA );
TipoVenda_st getTipoVenda( VENDA );
CodigoCliente_st getCodigoCliente( VENDA );
Mes_st getMes( VENDA );
Filial_st getFilial( VENDA );

void setCodigoProduto( VENDA , CodigoProduto_st );
void setPreco( VENDA , Preco_st );
void setQuantidade( VENDA , Quantidade_st );
void setTipoVenda( VENDA , TipoVenda_st );
void setCodigoCliente( VENDA , CodigoCliente_st );
void setMes( VENDA , Mes_st );
void setFilial( VENDA , Filial_st );

Filial_st calculaIndicieFilial( VENDA );

VENDA clonelinhaVenda( VENDA , VENDA );
StringVenda lerLinhaVenda( String , StringVenda );
VENDA inserirLinhaVenda( VENDA , StringVenda );
void imprimirLinhaVenda( VENDA );

int verificaCodigoProduto( Codigo );
int verificaCodigoCliente( Codigo );
int verificaQuantidade( int );
int verificaPreco( double );
int verificaFilial( int );
int verificaMes( int );
int verificaTipoCompra( char );
int verificaVenda( String );

#endif
