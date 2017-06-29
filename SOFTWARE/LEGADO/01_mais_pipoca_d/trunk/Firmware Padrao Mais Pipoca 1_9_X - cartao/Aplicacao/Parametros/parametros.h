/*__________________________________________________________________________________
|	Chave Digital Tecnologia Eletronica Ltda. 
|       
|       Balenário Camboriú - SC
|       www.chavedigital.com.br
| __________________________________________________________________________________
|
|       This source code was developed by Chave Digital and cannot be copied, in part 
|       or in whole, or used, except when legally licensed by Chave Digital
|       or its distributors.
|
|       Este código é propriedade da Chave Digital e não pode ser copiado, em parte 
|       ou em todo, ou utilizado, exceto quando for legalmente licenciado pela 
|       Chave Digital ou por um de seus distribuidores.
| __________________________________________________________________________________
|
|       Arquivo            :  Parametros.h
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  11/03/2014
|
|       Revisões           :  1.0.0.0
|
|
| __________________________________________________________________________________
*/

#ifndef _PARAMETROS_H_
#define _PARAMETROS_H_

typedef enum{
  TEMPO_PREPARO,
  TEMPERATURA_PREPARO,
  VELOCIDADE_PREPARO,
  VELOCIDADE_EXPULSAO,
  ATRASO_PACOTE,
  VALOR_PIPOCA  
}eINDICE_PARAMETROS;

#define QTD_TIPOS_NOTEIRO                               2
typedef enum{
  BV20,
  TRILOGY  
}eTIPO_NOTEIRO;

typedef enum{
  CHINESA,
  EXTECH
}eTIPO_IMPRESSORA;

typedef enum{
  RM5,
  UCA1,
  SOMYC
}eTIPO_MOEDEIRO;

#define TIPO_NOTEIRO_TO_STRING(X)               switch(X){case BV20: return "BV20     "; case TRILOGY: return "TRILOGY";}

void PARAMETROS_alteraTabelaIndiceMusicas(unsigned char musica,
                                          unsigned int enderecoInicial,
                                          unsigned int tamanho);

unsigned char PARAMETROS_carregaDadosIndiceMusicas(unsigned char musica,
                                                   unsigned int* enderecoInicial,
                                                   unsigned int* tamanho);

void PARAMETROS_escreveParametro(unsigned char indice,unsigned short int valor);

unsigned short int PARAMETROS_leParametro(unsigned char indice);

void PARAMETROS_salvaBetaTermistor(unsigned int beta);

unsigned int PARAMETROS_carregaBetaTermistor(void);

void PARAMETROS_salvaRTtermistor(unsigned int rt);

unsigned int PARAMETROS_carregaRTtermistor(void);

void PARAMETROS_gravaTipoCedulas(unsigned char f2,unsigned char f5,unsigned char f10,
                                 unsigned char f20,unsigned char f50,unsigned char f100);

void PARAMETROS_carregaTipoCedulas(unsigned char *f2,unsigned char *f5,unsigned char *f10,
                                   unsigned char *f20,unsigned char *f50,unsigned char *f100);

unsigned int PARAMETROS_leContadorVendas(void);

void PARAMETROS_salvaContadorVendas(unsigned int contador);

unsigned int PARAMETROS_leContadorArrecadacao(void);

void PARAMETROS_salvaContadorArrecadacao(unsigned int arrecadacao);

unsigned int PARAMETROS_leTotalizadorPermanente(void);
                                   
void PARAMETROS_gravaTotalizadorPermanente(unsigned int totalizador);

unsigned short int PARAMETROS_leIntervaloPropaganda(void);

void PARAMETROS_gravaIntervaloPropaganda(unsigned short int intervalo);

unsigned short int PARAMETROS_leFlagPiscadaPropaganda(void);

void PARAMETROS_gravaFlagPiscadaPropaganda(unsigned short int flag);

unsigned short int PARAMETROS_leTemperaturaPreProcesso(void);

void PARAMETROS_salvaTemperaturaPreProcesso(unsigned short int temperatura);

void PARAMETROS_salvaRotacaoInicialExpulsao(unsigned short int rpm);

unsigned short int PARAMETROS_leRotacaoInicialExpulsao(void);

void PARAMETROS_salvaVelocidadeFinalExpulsao(unsigned short int rpm);

unsigned short int PARAMETROS_leVelocidadeFinalExpulsao(void);

unsigned short int PARAMETROS_leVolumeSom(void);

void PARAMETROS_gravaVolumeSom(unsigned short int volume);

unsigned short int PARAMETROS_leFatorTrabalho(void);

void PARAMETROS_gravaFatorTrabalho(unsigned short int fator);

unsigned short int PARAMETROS_leVelocidadeLimpeza(void);

void PARAMETROS_gravaVelocidadeLimpeza(unsigned short int velocidade);

unsigned short int PARAMETROS_leVelocidadeAquecimento(void);

void PARAMETROS_gravaVelocidadeAquecimento(unsigned short int velocidade);

unsigned short int PARAMETROS_leVolumeVoz(void);
 
void PARAMETROS_gravaVolumeVoz(unsigned short int volume);

unsigned short int PARAMETROS_leFitaPreparo(void);

void PARAMETROS_gravaFitaPreparo(unsigned short int fita);

void PARAMETROS_gravaComissaoPonto(unsigned short int valor);

unsigned short int PARAMETROS_leComissaoPonto(void);

void PARAMETROS_gravaContadorVendasParcial(unsigned int contador);

unsigned int PARAMETROS_leContadorVendasParcial(void);

unsigned char PARAMETROS_gravaConfigFabrica(unsigned short int tempoPreparo,
                                            unsigned short int temperaturaPreparo,
                                            unsigned short int velocidadePreparo,
                                            unsigned short int temperaturaPreProcesso,
                                            unsigned short int atrasoPacote,
                                            unsigned short int velocidadeInicialExpulsao,
                                            unsigned short int velocidadeFinalExpulsao,
                                            unsigned short int fatorRampaTemperatura,
                                            unsigned short int velocidadeLimpeza,
                                            unsigned short int velocidadeAquecimento,
                                            unsigned short int volumeSom,
                                            unsigned short int volumeVoz);

unsigned char PARAMETROS_carregaCopiaConfigFabrica(unsigned short int* tempoPreparo,
                                                   unsigned short int* temperaturaPreparo,
                                                   unsigned short int* velocidadePreparo,
                                                   unsigned short int* temperaturaPreProcesso,
                                                   unsigned short int* atrasoPacote,
                                                   unsigned short int* rotacaoInicioExpulsao,
                                                   unsigned short int* rotacaoFimExpulsao,
                                                   unsigned short int* rampaTemperatura,
                                                   unsigned short int* velocidadeLimpeza,
                                                   unsigned short int* velocidadePreAquecimento,
                                                   unsigned short int* volumeSom,
                                                   unsigned short int* volumeVoz
                                                   );

void PARAMETROS_gravaFlagLocacao(unsigned char flag);

unsigned char PARAMETROS_leFlagLocacao(void);

void PARAMETROS_gravaTemperaturaInicio(unsigned short int temperatura);

unsigned short int PARAMETROS_leTemperaturaInicio(void);

void PARAMETROS_gravaTempoPreAquecimento(unsigned short int tempo);

unsigned short int PARAMETROS_leTempoPreaquecimento(void);

void PARAMETROS_gravaMensagemCliente(unsigned char indice,char *mensagem);

void PARAMETROS_leMensagemCliente(unsigned char indice,char *mensagem);

void PARAMETROS_gravaCicloDesumificacao(unsigned char ciclo);

unsigned char PARAMETROS_leCicloDesumidificacao(void);

void PARAMETROS_salvaVelocidadeDesumidificacao(unsigned short int rpm);

unsigned short int PARAMETROS_leVelocidadeDesumidificacao(void);

unsigned short int PARAMETROS_leTemperaturaDesumidificacao(void);

void PARAMETROS_salvaTemperaturaDesumidificacao(unsigned short int temperatura);

void PARAMETROS_salvaContadorPipocaTeste(unsigned short int pipocaTeste);

unsigned short int PARAMETROS_leContadorPipocasTeste(void);

void PARAMETROS_salvaTipoNoteiro(eTIPO_NOTEIRO tipo);

eTIPO_NOTEIRO PARAMETROS_leTipoNoteiro(void);

void PARAMETROS_salvaTipoImpressora(eTIPO_IMPRESSORA tipo);

eTIPO_IMPRESSORA PARAMETROS_carregaTipoImpressora(void);

void PARAMETROS_salvaContadorOperacoesCartao(unsigned int operacoes);

unsigned int PARAMETROS_carregaOperacoesCartao(void);

eTIPO_MOEDEIRO PARAMETROS_carregaTipoMoedeiro(void);

void PARAMETROS_salvaTipoMoedeiro(eTIPO_MOEDEIRO tipo);

unsigned int PARAMETROS_carregaNumeroSerie(void);

void PARAMETROS_setNumeroSerie(unsigned int numeroSerie);

unsigned int PARAMETROS_carregaFaturamentoParcialCartao(void);

void PARAMETROS_salvaFaturamentoParcialCartao(unsigned int faturamento);

unsigned int PARAMETROS_carregaFaturamentoTotalCartao(void);

void PARAMETROS_salvaFaturamentoTotalCartao(unsigned int faturamento);

/***********************************************************************************
*       Fim do arquivo
***********************************************************************************/
#endif//_PARAMETROS_H_