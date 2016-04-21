#ifndef _FATURACAO_H
#define _FATURACAO_H
typedef struct contas *Contas;
typedef struct faturacao *MODULO_FATURACAO;
typedef double Resultados;

Quantidade_st getQuantidadeTotalNormal(MODULO_FATURACAO );
Quantidade_st getQuantidadeTotalPromocional(MODULO_FATURACAO );
Preco_st getTotalFaturadoNormal(MODULO_FATURACAO );
Preco_st getTotalFaturadoPromocional(MODULO_FATURACAO );
Quantidade_st getContas_QuantidadeTotal(MODULO_FATURACAO , Filial_st , Mes_st , TipoVenda_st );
Preco_st getContas_TotalFaturado(MODULO_FATURACAO , Filial_st , Mes_st , TipoVenda_st );

MODULO_FATURACAO moduloFaturacaoInit( void );
void freeModuloFaturacao( MODULO_FATURACAO );

void inserirVendaModuloFacturacao( MODULO_FATURACAO , VENDA );

Resultados consultaFaturacaoMes(MODULO_FATURACAO modulo, Mes_st mes);
Resultados consultaFaturacaoIntervaloMeses(MODULO_FATURACAO modulo, Mes_st mes_a, Mes_st mes_b);
Resultados consultaFaturacaoFilial(MODULO_FATURACAO modulo, Filial_st filial);
Resultados consultaFaturacaoMesFilial(MODULO_FATURACAO modulo, Mes_st mes, Filial_st filial);

Resultados consultaQuantidadesMes(MODULO_FATURACAO modulo, Mes_st mes);
Resultados consultaQuantidadesIntervaloMeses(MODULO_FATURACAO modulo, Mes_st mes_a, Mes_st mes_b);
Resultados consultaQuantidadesFilial(MODULO_FATURACAO modulo, Filial_st filial);
Resultados consultaQuantidadesMesFilial(MODULO_FATURACAO modulo, Mes_st mes, Filial_st filial);

void travessiaFaturacao(MODULO_FATURACAO modulo); //FAZER MELHOR

#endif /* _FATURACAO_H */
