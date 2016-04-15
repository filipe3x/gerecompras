#ifndef _FATURACAO_H
#define _FATURACAO_H
typedef struct contas *Contas;
typedef struct faturacao *MODULO_FATURACAO;
typedef double Resultados;

Quantidade_st getQuantidadeTotalNormal(MODULO_FATURACAO );
Quantidade_st getQuantidadeTotalPromocional(MODULO_FATURACAO );
Preco_st getTotalFaturadoNormal(MODULO_FATURACAO );
Preco_st getTotalFaturadoPromocional(MODULO_FATURACAO );

////

void setQuantidadeTotalNormal(MODULO_FATURACAO , Quantidade_st );
void setQuantidadeTotalPromocional(MODULO_FATURACAO , Quantidade_st );
void setTotalFaturadoNormal(MODULO_FATURACAO , Preco_st valor );
void setTotalFaturadoPromocional(MODULO_FATURACAO , Preco_st valor );

////

void incQuantidadeTotalNormal(MODULO_FATURACAO , Quantidade_st );
void incQuantidadeTotalPromocional(MODULO_FATURACAO , Quantidade_st );
void incTotalFaturadoNormal(MODULO_FATURACAO , Preco_st valor );
void incTotalFaturadoPromocional(MODULO_FATURACAO , Preco_st valor );

////

Quantidade_st getContas_QuantidadeTotal(MODULO_FATURACAO , Filial_st , Mes_st , TipoVenda_st );
Preco_st getContas_TotalFaturado(MODULO_FATURACAO , Filial_st , Mes_st , TipoVenda_st );
void setContas_QuantidadeTotal(MODULO_FATURACAO , Filial_st , Mes_st , TipoVenda_st , Quantidade_st );
void setContas_TotalFaturado(MODULO_FATURACAO , Filial_st , Mes_st , TipoVenda_st , Preco_st valor );
void incContas_QuantidadeTotal(MODULO_FATURACAO , Filial_st , Mes_st , TipoVenda_st , Quantidade_st );
void incContas_TotalFaturado(MODULO_FATURACAO , Filial_st , Mes_st , TipoVenda_st , Preco_st valor );

MODULO_FATURACAO moduloFaturacaoInit( void );
void freeModuloFaturacao( MODULO_FATURACAO );

void inserirVendaModuloFacturacao( MODULO_FATURACAO , VENDA );

#endif /* _FATURACAO_H */
