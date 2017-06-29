/*__________________________________________________________________________________
|	Quark Tecnologia Eletrônica Embarcada
|       
|       Itapema - SC
|       www.quarktee.com.br
| __________________________________________________________________________________
|
|       This source code was developed by Quark  and cannot be copied, in part 
|       or in whole, or used, except when legally licensed by Quark 
|       or its distributors.
|
|       Este código é propriedade da Quark  e não pode ser copiado, em parte 
|       ou em todo, ou utilizado, exceto quando for legalmente licenciado pela 
|       Quark  ou por um de seus distribuidores.
| __________________________________________________________________________________
|
|       Arquivo            :  Parametros.c
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  11/03/2014
|
|       Revisões           :  1.0.0.0
|
|
| __________________________________________________________________________________
*/

/***********************************************************************************
*       Includes
***********************************************************************************/
#include <nxp\iolpc1768.h>
#include <string.h>
#include "parametros.h"
#include "..\..\includes.h"

/***********************************************************************************
*       Definições
***********************************************************************************/


/***********************************************************************************
*       Constantes
***********************************************************************************/
const unsigned int PARAMETROS_enderecoTabela[4]={
  ADR_INDICE_MUSICA,
  ADR_INDICE_PROPAGANDA,
  ADR_INCICE_PEGUE_O_PACOTE,
  ADR_INDICE_PEGUE_AS_PIPOCAS
};

/***********************************************************************************
*       Variaveis locais
***********************************************************************************/


/***********************************************************************************
*       Funções locais
***********************************************************************************/
extern unsigned short int CCTALK_calculaCRC(unsigned char *pData,unsigned short int offset,
                                            unsigned short int tamanho);

/***********************************************************************************
*       Implementação das funções
***********************************************************************************/

/***********************************************************************************
*       Descrição       :      Altera os dados de uma das posições da tabela
*                              de indexação dos arquivos de audio
*       Parametros      :      (unsigned char) indice da música
*                              (unsigned long int) endereço inicial da musica
*                              (unsigned long int) tamanho da musica
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_alteraTabelaIndiceMusicas(unsigned char musica,
                                          unsigned int enderecoInicial,
                                          unsigned int tamanho){
  unsigned char buffer[8];  
  
  buffer[0] = enderecoInicial>>16;
  buffer[1] = enderecoInicial>>8;
  buffer[2] = enderecoInicial;
  buffer[3] = tamanho>>16;
  buffer[4] = tamanho>>8;
  buffer[5] = tamanho;
  unsigned short int crc = CCTALK_calculaCRC(buffer,0,6);
  buffer[6] = crc>>8;
  buffer[7] = crc;
  
  MEMORYWRAPPER_writeBytes(PARAMETROS_enderecoTabela[musica],buffer,8);                                                                                                                                     
}
/***********************************************************************************
*       Descrição       :       Lê os dados de uma das posições da tabela
*                               de indexação dos arquivos de audio
*       Parametros      :       (unsigned char) indice da música
*                               (unsigned long int*) endereço inicial da música
*                               (unsigned long int*) tamanho da música
*       Retorno         :       (unsigned char) maior do que zero se os
*                                               dados forem consistentes
***********************************************************************************/
unsigned char PARAMETROS_carregaDadosIndiceMusicas(unsigned char musica,
                                                   unsigned int* enderecoInicial,
                                                   unsigned int* tamanho){
  unsigned char buffer[8];
  
  MEMORYWRAPPER_readBytes(PARAMETROS_enderecoTabela[musica],buffer,8);
  
  unsigned short int crc = CCTALK_calculaCRC(buffer,0,6);
  unsigned short int tmp = buffer[6]<<8 | buffer[7];
  
  if(tmp==crc){
  
    *enderecoInicial = buffer[0]<<16 | buffer[1]<<8 | buffer[2];
    *tamanho = buffer[3]<<16 | buffer[4]<<8 | buffer[5];
    
    return 255;
  }  
  
  return 0;
}
/***********************************************************************************
*       Descrição       :       Escreve na tabela de parametros do sistema
*       Parametros      :       (unsigned char) indice do parametro
*                               (unsigned short int) valor do parametro
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_escreveParametro(unsigned char indice,unsigned short int valor){
  unsigned char buffer[4];  
  unsigned short int crc;  
  
  buffer[0] = valor>>8;
  buffer[1] = valor;
  crc = CCTALK_calculaCRC(buffer,0,2);
  buffer[2] = crc>>8;
  buffer[3] = crc;
  
  switch(indice){    
    case TEMPO_PREPARO:
         MEMORYWRAPPER_writeBytes(ADR_TEMPO_PREPARO,buffer,4);
         break;
    case TEMPERATURA_PREPARO:
         MEMORYWRAPPER_writeBytes(ADR_TEMPERATURA_PREPARO,buffer,4);      
         break;      
    case VELOCIDADE_PREPARO:
         MEMORYWRAPPER_writeBytes(ADR_VELOCIDADE_PREPARO,buffer,4);      
         break;      
    case VELOCIDADE_EXPULSAO:
         MEMORYWRAPPER_writeBytes(ADR_VELOCIDADE_EXPULSAO,buffer,4);      
         break;      
    case ATRASO_PACOTE:
         MEMORYWRAPPER_writeBytes(ADR_ATRASO_PACOTE,buffer,4);      
         break;      
    case VALOR_PIPOCA:
         MEMORYWRAPPER_writeBytes(ADR_VALOR_PIPOCA,buffer,4);      
         break;      
  }
}
/***********************************************************************************
*       Descrição       :       Lê um parametro a partir da tabela de parametros
*                               do sistema
*       Parametros      :       (unsigned char) indice do parametro
*                               (unsigned short int) valor do parametro
*       Retorno         :       nenhum
***********************************************************************************/
unsigned short int PARAMETROS_leParametro(unsigned char indice){
  unsigned char buffer[4];
  unsigned short int parametro;
  unsigned short int limiteInferior,limiteSuperior;
  unsigned short int crc;
               
  switch(indice){    
    case TEMPO_PREPARO:
         MEMORYWRAPPER_readBytes(ADR_TEMPO_PREPARO,buffer,4);
         limiteInferior = 30;
         limiteSuperior = 180;
         break;
    case TEMPERATURA_PREPARO:
         MEMORYWRAPPER_readBytes(ADR_TEMPERATURA_PREPARO,buffer,4);      
         limiteInferior = 60;
         limiteSuperior = 200;
         break;      
    case VELOCIDADE_PREPARO:
         MEMORYWRAPPER_readBytes(ADR_VELOCIDADE_PREPARO,buffer,4);      
         limiteInferior = 2000;
         limiteSuperior = 18000;
         break;      
    case VELOCIDADE_EXPULSAO:
         MEMORYWRAPPER_readBytes(ADR_VELOCIDADE_EXPULSAO,buffer,4);      
         limiteInferior = 2000;
         limiteSuperior = 18000;
         break;      
    case ATRASO_PACOTE:
         MEMORYWRAPPER_readBytes(ADR_ATRASO_PACOTE,buffer,4);      
         limiteInferior = 1;
         limiteSuperior = 200;
         break;      
    case VALOR_PIPOCA:
         MEMORYWRAPPER_readBytes(ADR_VALOR_PIPOCA,buffer,4);      
         limiteInferior = 1;
         limiteSuperior = 1000;
         break;      
  }  
  crc = CCTALK_calculaCRC(buffer,0,2);
  
  if(crc==(buffer[2]<<8 | buffer[3])){
    
    parametro = buffer[0]<<8 | buffer[1];
    if(parametro<limiteInferior)
      parametro = limiteInferior;
    if(parametro>limiteSuperior)
      parametro = limiteSuperior;
    
    return parametro;    
  }
  else
    return limiteInferior;   
}
/***********************************************************************************
*       Descrição       :       Grava o beta do termistor
*       Parametros      :       (unsigned int) beta
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_salvaBetaTermistor(unsigned int beta){
  unsigned char buffer[6];
  unsigned short int crc;
  
  buffer[0] = beta>>24;
  buffer[1] = beta>>16;
  buffer[2] = beta>>8;
  buffer[3] = beta;
  
  crc = CCTALK_calculaCRC(buffer,0,4);
  
  buffer[4] = crc>>8;
  buffer[5] = crc;
  
  MEMORYWRAPPER_writeBytes(ADR_BETA,buffer,6);    
}
/***********************************************************************************
*       Descrição       :       Carrega o beta do transistor 
*       Parametros      :       nenhum
*       Retorno         :       (unsigned int) beta
***********************************************************************************/
unsigned int PARAMETROS_carregaBetaTermistor(void){
  unsigned char buffer[6];
  unsigned short int crc;

  MEMORYWRAPPER_readBytes(ADR_BETA,buffer,6);

  crc = CCTALK_calculaCRC(buffer,0,4);

  if(crc == buffer[4]<<8 | buffer[5])       
    return buffer[0]<<24 | buffer[1]<<16 | buffer[2]<<8 | buffer[3];
  
  return 4750; // valor padrão para o termistor utilizado no projeto
}
/***********************************************************************************
*       Descrição       :       Grava o parametro RT na flash
*       Parametros      :       (unsigned int) RT do termistor 
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_salvaRTtermistor(unsigned int rt){
  unsigned char buffer[6];
  unsigned short int crc;

  buffer[0] = rt>>24;
  buffer[1] = rt>>16;
  buffer[2] = rt>>8;
  buffer[3] = rt;

  crc = CCTALK_calculaCRC(buffer,0,4);

  buffer[4] = crc>>8;
  buffer[5] = crc;

  MEMORYWRAPPER_writeBytes(ADR_RT,buffer,6);    
}
/***********************************************************************************
*       Descrição       :       Lê o parametro RT a partir da flash
*       Parametros      :       nenhum
*       Retorno         :       (unsigned int) valor do rt
***********************************************************************************/
unsigned int PARAMETROS_carregaRTtermistor(void){
  unsigned char buffer[6];
  unsigned short int crc;
 
  MEMORYWRAPPER_readBytes(ADR_RT,buffer,6);

  crc = CCTALK_calculaCRC(buffer,0,4);
  
  if(crc== buffer[4]<<8 | buffer[5])
    return buffer[0]<<24 | buffer[1]<<16 | buffer[2]<<8 | buffer[3];
  
  return 120505; // Valor padrão para o termistor escolhido no projeto                   
}
/***********************************************************************************
*       Descrição       :       Configura quais cédulas serão aceitas pelo 
*                               noteiro
*       Parametros      :       (unsigned char) R$ 2,00
*                               (unsigned char) R$ 5,00
*                               (unsigned char) R$ 10,00
*                               (unsigned char) R$ 20,00
*                               (unsigned char) R$ 50,00
*                               (unsigned char) R$ 100,00
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_gravaTipoCedulas(unsigned char f2,unsigned char f5,unsigned char f10,
                                 unsigned char f20,unsigned char f50,unsigned char f100){
  unsigned char buffer[3];
  unsigned short int crc;
  
  buffer[0] = f2?1:0;
  buffer[0]|= f5?2:0;
  buffer[0]|= f10?4:0;
  buffer[0]|= f20?8:0;
  buffer[0]|= f50?16:0;
  buffer[0]|= f100?32:0;
                        
  crc = CCTALK_calculaCRC(buffer,0,1);
  buffer[1] = crc>>8;
  buffer[2] = crc;
  
  MEMORYWRAPPER_writeBytes(ADR_TIPO_CEDULAS,buffer,3);                                                                      
}
/***********************************************************************************
*       Descrição       :       Verifica quais cédulas estão habilitadas
*                               para uso na máquina
*       Parametros      :       (unsigned char*) R$ 2,00
*                               (unsigned char*) R$ 5,00
*                               (unsigned char*) R$ 10,00
*                               (unsigned char*) R$ 20,00
*                               (unsigned char*) R$ 50,00
*                               (unsigned char*) R$ 100,00
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_carregaTipoCedulas(unsigned char *f2,unsigned char *f5,unsigned char *f10,
                                   unsigned char *f20,unsigned char *f50,unsigned char *f100){
  unsigned char buffer[3];
  
  MEMORYWRAPPER_readBytes(ADR_TIPO_CEDULAS,buffer,3);
  
  unsigned short int crc = CCTALK_calculaCRC(buffer,0,1);
  
  if(crc==buffer[1]<<8 | buffer[2]){    
    *f2  = (buffer[0]&0x01)?255:0;
    *f5  = (buffer[0]&0x02)?255:0;
    *f10 = (buffer[0]&0x04)?255:0;
    *f20 = (buffer[0]&0x08)?255:0;
    *f50 = (buffer[0]&0x10)?255:0;
    *f100= (buffer[0]&0x20)?255:0;    
  }
  else{    
    *f2   = 255;
    *f5   = 255;
    *f10  = 255;
    *f20  = 255;
    *f50  = 255;
    *f100 = 255;    
  }                                                                         
}
/***********************************************************************************
*       Descrição       :       Lê o valor do contador de vendas
*       Parametros      :       nenhum
*       Retorno         :       (unsigned int) contador de vendas
***********************************************************************************/
unsigned int PARAMETROS_leContadorVendas(void){
  unsigned char buffer[6];
  unsigned short int crc;
  
  MEMORYWRAPPER_readBytes(ADR_CONTADOR_VENDAS,buffer,6);
  
  crc = CCTALK_calculaCRC(buffer,0,4);
  
  if(crc == (buffer[4]<<8 | buffer[5]))    
     return buffer[0]<<24 | buffer[1]<<16 | buffer[2]<<8 | buffer[3]; 
      
  return 0; 
}
/***********************************************************************************
*       Descrição       :       Salva o valor do contador de vendas
*       Parametros      :       (unsigned int) contador de vendas
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_salvaContadorVendas(unsigned int contador){
  unsigned char buffer[6];
  unsigned short int crc;
  
  buffer[0] = contador>>24;
  buffer[1] = contador>>16;
  buffer[2] = contador>>8;
  buffer[3] = contador;
  crc = CCTALK_calculaCRC(buffer,0,4);
  buffer[4] = crc>>8;
  buffer[5] = crc;
  
  MEMORYWRAPPER_writeBytes(ADR_CONTADOR_VENDAS,buffer,6);
}
/***********************************************************************************
*       Descrição       :      Lê o contador de arrecadação
*       Parametros      :       nenhum
*       Retorno         :       (unsigned int) contador de arrecadação financeira
***********************************************************************************/
unsigned int PARAMETROS_leContadorArrecadacao(void){
  unsigned char buffer[6];
  unsigned short int crc;
  
  MEMORYWRAPPER_readBytes(ADR_CONTADOR_DINHEIRO,buffer,6);
  
  crc = CCTALK_calculaCRC(buffer,0,4);
  
  if(crc == (buffer[4]<<8 | buffer[5]))    
     return buffer[0]<<24 | buffer[1]<<16 | buffer[2]<<8 | buffer[3]; 
      
  return 0; 
}
/***********************************************************************************
*       Descrição       :       Salva o contador de arrecadacao
*       Parametros      :       (unsigned int) valores do contador de arrecadacao
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_salvaContadorArrecadacao(unsigned int arrecadacao){
  unsigned char buffer[6];
  unsigned short int crc;
  
  buffer[0] = arrecadacao>>24;
  buffer[1] = arrecadacao>>16;
  buffer[2] = arrecadacao>>8;
  buffer[3] = arrecadacao;
  crc = CCTALK_calculaCRC(buffer,0,4);
  buffer[4] = crc>>8;
  buffer[5] = crc;
  
  MEMORYWRAPPER_writeBytes(ADR_CONTADOR_DINHEIRO,buffer,6);
}
/***********************************************************************************
*       Descrição       :       Lê o contador de dinheiro permanente
*       Parametros      :       nenhum
*       Retorno         :       (unsigned int) totalizador
***********************************************************************************/
unsigned int PARAMETROS_leTotalizadorPermanente(void){
  unsigned char buffer[6];
  unsigned short int crc;
  
  MEMORYWRAPPER_readBytes(ADR_CONTADOR_PERMANENTE,buffer,6);
  
  crc = CCTALK_calculaCRC(buffer,0,4);
  
  if(crc == (buffer[4]<<8 | buffer[5]))    
     return buffer[0]<<24 | buffer[1]<<16 | buffer[2]<<8 | buffer[3]; 
      
  return 0; 
}
/***********************************************************************************
*       Descrição       :       Escreve no contador de dinheiro permanente
*       Parametros      :       (unsigned int) totalizador
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_gravaTotalizadorPermanente(unsigned int totalizador){
  unsigned char buffer[6];
  unsigned short int crc;
  
  buffer[0] = totalizador>>24;
  buffer[1] = totalizador>>16;
  buffer[2] = totalizador>>8;
  buffer[3] = totalizador;
  crc = CCTALK_calculaCRC(buffer,0,4);
  buffer[4] = crc>>8;
  buffer[5] = crc;
  
  MEMORYWRAPPER_writeBytes(ADR_CONTADOR_PERMANENTE,buffer,6);
}
/***********************************************************************************
*       Descrição       :       Lê o parâmetro tempo de propaganda
*       Parametros      :       nenhum
*       Retorno         :       (unsigned short int) intervalo 
*                                                    das propagandas
***********************************************************************************/
unsigned short int PARAMETROS_leIntervaloPropaganda(void){
  unsigned char buffer[4];
  unsigned short int crc;
  
  MEMORYWRAPPER_readBytes(ADR_TEMPO_PROPAGANDA,buffer,4);
  crc = CCTALK_calculaCRC(buffer,0,2);
  
  if(crc == (buffer[2]<<8 | buffer[3])){
        
    return buffer[0]<<8 | buffer[1];  
  }
  
  return 0;  
}
/***********************************************************************************
*       Descrição       :       Escreve no parâmetro tempo de propaganda
*       Parametros      :       (unsigned short int) tempo para propaganda
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_gravaIntervaloPropaganda(unsigned short int intervalo){
  unsigned char buffer[4];
  unsigned short int crc;

  buffer[0] = intervalo>>8;
  buffer[1] = intervalo;
  crc = CCTALK_calculaCRC(buffer,0,2);
  buffer[2] = crc>>8;
  buffer[3] = crc;

  MEMORYWRAPPER_writeBytes(ADR_TEMPO_PROPAGANDA,buffer,4);       
}
/***********************************************************************************
*       Descrição       :       Lê o parâmetro que indica se as fitas de led
*                               frontais ficam piscando ou não
*       Parametros      :       nenhum
*       Retorno         :       (unsigned short int) maior do que zero sim
*                                                    igual a zero não
***********************************************************************************/
unsigned short int PARAMETROS_leFlagPiscadaPropaganda(void){
  unsigned char buffer[4];
  unsigned short int crc;
  
  MEMORYWRAPPER_readBytes(ADR_FITA_LED,buffer,4);
  crc = CCTALK_calculaCRC(buffer,0,2);
  
  if(crc==(buffer[2]<<8 | buffer[3])){    
    return buffer[0]<<8 | buffer[1];
  }    
  
  return 0;
}
/***********************************************************************************
*       Descrição       :       Escreve o parâmetro que indica se as fitas de led
*                               devem piscar ou não
*       Parametros      :       (unsigned short int) maior do que zero sim
*                                                    igual a zero não
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_gravaFlagPiscadaPropaganda(unsigned short int flag){
  unsigned char buffer[4];
  unsigned short int crc;
  
  buffer[0] = flag>>8;
  buffer[1] = flag;
  crc = CCTALK_calculaCRC(buffer,0,2);
  buffer[2] = crc>>8;
  buffer[3] = crc;
  
  MEMORYWRAPPER_writeBytes(ADR_FITA_LED,buffer,4);      
}
/***********************************************************************************
*       Descrição       :       Lê o parâmetro temperatura de pré-processo
*       Parametros      :       nenhum
*       Retorno         :       (unsigned short int) temperatura de pré-processo
***********************************************************************************/
unsigned short int PARAMETROS_leTemperaturaPreProcesso(void){
  unsigned char buffer[4];
  unsigned short int crc;
  
  MEMORYWRAPPER_readBytes(ADR_TEMPERATURA_PRE_PROCESSO,buffer,4);
  crc = CCTALK_calculaCRC(buffer,0,2);
  
  if(crc==(buffer[2]<<8 | buffer[3])){
    
    crc = buffer[0]<<8 | buffer[1];
    if(crc<30)
      crc=30;
    if(crc>150)
      crc=150;
    
    return crc;
  }
  
  return 60;
}
/***********************************************************************************
*       Descrição       :       Salva a temperatura de pré-processo
*       Parametros      :       (unsigned short int) temperatura
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_salvaTemperaturaPreProcesso(unsigned short int temperatura){
  unsigned short int crc;
  unsigned char buffer[4];
  
  buffer[0] = temperatura>>8;
  buffer[1] = temperatura;
  crc = CCTALK_calculaCRC(buffer,0,2);
  buffer[2] = crc>>8;
  buffer[3] = crc;
  
  MEMORYWRAPPER_writeBytes(ADR_TEMPERATURA_PRE_PROCESSO,buffer,4);    
}
/***********************************************************************************
*       Descrição       :       Salva o parâmetro velocidade inicial
*                               da expulsão da pipoca
*       Parametros      :       (unsigned short int) rotação inicial
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_salvaRotacaoInicialExpulsao(unsigned short int rpm){
  unsigned short int crc;
  unsigned char buffer[4];
  
  buffer[0] = rpm>>8;
  buffer[1] = rpm;
  crc = CCTALK_calculaCRC(buffer,0,2);
  buffer[2] = crc>>8;
  buffer[3] = crc;
  
  MEMORYWRAPPER_writeBytes(ADR_VELOCIDADE_INICIAL_EXP,buffer,4);      
}
/***********************************************************************************
*       Descrição       :       Lê o parâmetro velocidade inicial
*       Parametros      :       nenhum
*       Retorno         :       (unsigned short int) rotação inicial
***********************************************************************************/
unsigned short int PARAMETROS_leRotacaoInicialExpulsao(void){
  unsigned char buffer[4];
  unsigned short int crc;
  
  MEMORYWRAPPER_readBytes(ADR_VELOCIDADE_INICIAL_EXP,buffer,4);
  crc = CCTALK_calculaCRC(buffer,0,2);
  
  if(crc==(buffer[2]<<8 | buffer[3])){
    
    crc = buffer[0]<<8 | buffer[1];
    if(crc<4000)
      crc=4000;
    if(crc>18000)
      crc=18000;
    
    return crc;
  }
  
  return 5000;
}
/***********************************************************************************
*       Descrição       :       Salva o parâmetro velocidade final
*                               de expulsão
*       Parametros      :       (unsigned short int) velocidade
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_salvaVelocidadeFinalExpulsao(unsigned short int rpm){
  unsigned short int crc;
  unsigned char buffer[4];
  
  buffer[0] = rpm>>8;
  buffer[1] = rpm;
  crc = CCTALK_calculaCRC(buffer,0,2);
  buffer[2] = crc>>8;
  buffer[3] = crc;
  
  MEMORYWRAPPER_writeBytes(ADR_VELOCIDADE_FINAL_EXP,buffer,4);   
}
/***********************************************************************************
*       Descrição       :       Lê o parâmetro velocidade final de expulsão
*       Parametros      :       nenhum
*       Retorno         :       (unsigned short int) velocidade
***********************************************************************************/
unsigned short int PARAMETROS_leVelocidadeFinalExpulsao(void){
  unsigned char buffer[4];
  unsigned short int crc;
  
  MEMORYWRAPPER_readBytes(ADR_VELOCIDADE_FINAL_EXP,buffer,4);
  crc = CCTALK_calculaCRC(buffer,0,2);
  
  if(crc==(buffer[2]<<8 | buffer[3])){
    
    crc = buffer[0]<<8 | buffer[1];
    if(crc<4000)
      crc=4000;
    if(crc>18000)
      crc=18000;
    
    return crc;
  }
  
  return 5000;
}
/***********************************************************************************
*       Descrição       :       Lê o parametro do volume do som
*       Parametros      :       nenhum
*       Retorno         :       (unsigned short int) volume do som
***********************************************************************************/
unsigned short int PARAMETROS_leVolumeSom(void){
  unsigned char buffer[4];
  unsigned short int crc;
  
  MEMORYWRAPPER_readBytes(ADR_VOLUME_SOM,buffer,4);
  crc = CCTALK_calculaCRC(buffer,0,2);
  
  if(crc==(buffer[2]<<8 | buffer[3])){
    
    crc = buffer[0]<<8 | buffer[1];
    if(crc>100)
      crc=100;
    
    return crc;
  }
  
  return 100;
}
/***********************************************************************************
*       Descrição       :       Escreve no parametro volume do som
*       Parametros      :       (unsigned short int) volume do som
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_gravaVolumeSom(unsigned short int volume){
  unsigned short int crc;
  unsigned char buffer[4];
  
  buffer[0] = volume>>8;
  buffer[1] = volume;
  crc = CCTALK_calculaCRC(buffer,0,2);
  buffer[2] = crc>>8;
  buffer[3] = crc;
  
  MEMORYWRAPPER_writeBytes(ADR_VOLUME_SOM,buffer,4);  
}
/***********************************************************************************
*       Descrição       :       Lê o fato de trabalho do ajuste da temperatura
*       Parametros      :       nenhum
*       Retorno         :       (unsigned short int) fator de trabaho
***********************************************************************************/
unsigned short int PARAMETROS_leFatorTrabalho(void){
  unsigned char buffer[4];
  unsigned short int crc;
  
  MEMORYWRAPPER_readBytes(ADR_FATOR_TEMP,buffer,4);
  crc = CCTALK_calculaCRC(buffer,0,2);
  
  if(crc==(buffer[2]<<8 | buffer[3])){
    
    crc = buffer[0]<<8 | buffer[1];
    if(crc<1)
      crc=1;
    if(crc>2400)
      crc=2400;
    
    return crc;
  }
  
  return 1; 
}
/***********************************************************************************
*       Descrição       :       Salva o fator de trabalho da temperatura
*       Parametros      :       (unsigned short int) fator de trabalho
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_gravaFatorTrabalho(unsigned short int fator){
  unsigned short int crc;
  unsigned char buffer[4];
  
  buffer[0] = fator>>8;
  buffer[1] = fator;
  crc = CCTALK_calculaCRC(buffer,0,2);
  buffer[2] = crc>>8;
  buffer[3] = crc;
  
  MEMORYWRAPPER_writeBytes(ADR_FATOR_TEMP,buffer,4);  
}
/***********************************************************************************
*       Descrição       :       Lê o parâmetro velocidade da limpeza
*       Parametros:     :       nenhum
*       Retorno         :       (unsigned short int) velocidade da limpeza
***********************************************************************************/
unsigned short int PARAMETROS_leVelocidadeLimpeza(void){
  unsigned char buffer[4];
  unsigned short int crc;
  
  MEMORYWRAPPER_readBytes(ADR_VELOCIDADE_LIMPEZA,buffer,4);
  crc = CCTALK_calculaCRC(buffer,0,2);
  
  if(crc==(buffer[2]<<8 | buffer[3])){
    
    crc = buffer[0]<<8 | buffer[1];
    if(crc<4000)
      crc=4000;
    if(crc>20000)
      crc=20000;
    
    return crc;
  }
  
  return 1; 
}
/***********************************************************************************
*       Descrição       :       Grava a velocidade da limpeza
*       Parametros      :       (unsigned short int) velocidade da limpeza
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_gravaVelocidadeLimpeza(unsigned short int velocidade){
  unsigned short int crc;
  unsigned char buffer[4];
  
  buffer[0] = velocidade>>8;
  buffer[1] = velocidade;
  crc = CCTALK_calculaCRC(buffer,0,2);
  buffer[2] = crc>>8;
  buffer[3] = crc;
  
  MEMORYWRAPPER_writeBytes(ADR_VELOCIDADE_LIMPEZA,buffer,4);        
}
/***********************************************************************************
*       Descrição       :       Lê a velocidade do pré-aquecimento
*       Parametros      :       nenhum
*       Retorno         :       (unsigned short int) velocidade pré-aquecimento
***********************************************************************************/
unsigned short int PARAMETROS_leVelocidadeAquecimento(void){
  unsigned char buffer[4];
  unsigned short int crc;
  
  MEMORYWRAPPER_readBytes(ADR_VELOCIDADE_AQUECIMENTO,buffer,4);
  crc = CCTALK_calculaCRC(buffer,0,2);
  
  if(crc==(buffer[2]<<8 | buffer[3])){
    
    crc = buffer[0]<<8 | buffer[1];
    if(crc<1000)
      crc=1000;
    if(crc>15000)
      crc=15000;
    
    return crc;
  }
  
  return 1;   
}
/***********************************************************************************
*       Descrição       :       Grava a velocidade de pré-aquecimento
*       Parametros      :       (unsigned short int) velocidade
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_gravaVelocidadeAquecimento(unsigned short int velocidade){
  unsigned short int crc;
  unsigned char buffer[4];
  
  buffer[0] = velocidade>>8;
  buffer[1] = velocidade;
  crc = CCTALK_calculaCRC(buffer,0,2);
  buffer[2] = crc>>8;
  buffer[3] = crc;
  
  MEMORYWRAPPER_writeBytes(ADR_VELOCIDADE_AQUECIMENTO,buffer,4);              
}
/***********************************************************************************
*       Descrição       :       Lê o volume da voz na flash
*       Parametros      :       nenhum
*       Retorno         :       (unsigned short int) volume para as faixas de voz
***********************************************************************************/
unsigned short int PARAMETROS_leVolumeVoz(void){
  unsigned char buffer[4];
  unsigned short int crc;
  
  MEMORYWRAPPER_readBytes(ADR_VOLUME_VOZ,buffer,4);
  crc = CCTALK_calculaCRC(buffer,0,2);
  
  if(crc==(buffer[2]<<8 | buffer[3])){
    
    crc = buffer[0]<<8 | buffer[1];
    if(crc>100)
      crc=100;
    
    return crc;
  }  
  return 100;   
}
/***********************************************************************************
*       Descrição       :      Grava o volume da voz na flash 
*       Parametros      :       (unsigned short int) volume
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_gravaVolumeVoz(unsigned short int volume){  
  unsigned short int crc;
  unsigned char buffer[4];
  
  buffer[0] = volume>>8;
  buffer[1] = volume;
  crc = CCTALK_calculaCRC(buffer,0,2);
  buffer[2] = crc>>8;
  buffer[3] = crc;
  
  MEMORYWRAPPER_writeBytes(ADR_VOLUME_VOZ,buffer,4);       
}
/***********************************************************************************
*       Descrição       :       Lê o parâmetro que indica se a fita deve
*                               ser ligada durante o preparo da pipoca
*       Parametros      :       nenhum
*       Retorno         :       (unsigned short int) valor 
***********************************************************************************/
unsigned short int PARAMETROS_leFitaPreparo(void){
  unsigned char buffer[4];
  unsigned short int crc;
  
  MEMORYWRAPPER_readBytes(ADR_FITA_PREPARO,buffer,4);
  crc = CCTALK_calculaCRC(buffer,0,2);
  
  if(crc==(buffer[2]<<8 | buffer[3])){
    
    crc = buffer[0]<<8 | buffer[1];
    if(crc>100)
      crc=100;
    
    return crc;
  }  
  return 1;    
}
/***********************************************************************************
*       Descrição       :       Grava o parâmetro que indica se a fita
*                               deve ser ligada durante o preparo da pipoca
*       Parametros      :       (unsigned short int) valor
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_gravaFitaPreparo(unsigned short int fita){
  unsigned short int crc;
  unsigned char buffer[4];
  
  buffer[0] = fita>>8;
  buffer[1] = fita;
  crc = CCTALK_calculaCRC(buffer,0,2);
  buffer[2] = crc>>8;
  buffer[3] = crc;
  
  MEMORYWRAPPER_writeBytes(ADR_FITA_PREPARO,buffer,4);  
}
/***********************************************************************************
*       Descrição       :       Lê o parâmetro que diz qual a comissão para
*                               o ponto
*       Parametros      :       (unsigned short int) valor
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_gravaComissaoPonto(unsigned short int valor){
  unsigned short int crc;
  unsigned char buffer[4];
  
  buffer[0] = valor>>8;
  buffer[1] = valor;
  crc = CCTALK_calculaCRC(buffer,0,2);
  buffer[2] = crc>>8;
  buffer[3] = crc;
  
  MEMORYWRAPPER_writeBytes(ADR_COMISSAO_PONTO,buffer,4);    
}
/***********************************************************************************
*       Descrição       :       Lê o parâmetro que indica a comissão 
*                               do ponto
*       Parametros      :       void
*       Retorno         :       (unsigned short int) valor 
***********************************************************************************/
unsigned short int PARAMETROS_leComissaoPonto(void){
  unsigned char buffer[4];
  unsigned short int crc;
  
  MEMORYWRAPPER_readBytes(ADR_COMISSAO_PONTO,buffer,4);
  crc = CCTALK_calculaCRC(buffer,0,2);
  
  if(crc==(buffer[2]<<8 | buffer[3])){
    
    crc = buffer[0]<<8 | buffer[1];
    if(crc>500)
      crc=500;
    
    return crc;
  }  
  return 1;   
}
/***********************************************************************************
*       Descrição       :       Grava um valor no contador de vendas
*                               parcial
*       Parametros      :       (unsigned int) valor
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_gravaContadorVendasParcial(unsigned int contador){
  unsigned char buffer[6];
  unsigned short int crc;
  
  buffer[0] = contador>>24;
  buffer[1] = contador>>16;
  buffer[2] = contador>>8;
  buffer[3] = contador;
  crc = CCTALK_calculaCRC(buffer,0,4);
  buffer[4] = crc>>8;
  buffer[5] = crc;
  
  MEMORYWRAPPER_writeBytes(ADR_CONTADOR_VENDAS_PARCIAL,buffer,6);
}
/***********************************************************************************
*       Descrição       :       Lê o valor do contador de vendas parcial
*       Parametros      :       nenhum
*       Retorno         :       (unsigned int) valor
***********************************************************************************/
unsigned int PARAMETROS_leContadorVendasParcial(void){
  unsigned char buffer[6];
  unsigned short int crc;
  
  MEMORYWRAPPER_readBytes(ADR_CONTADOR_VENDAS_PARCIAL,buffer,6);
  
  crc = CCTALK_calculaCRC(buffer,0,4);
  
  if(crc == (buffer[4]<<8 | buffer[5]))    
     return buffer[0]<<24 | buffer[1]<<16 | buffer[2]<<8 | buffer[3]; 
      
  return 0; 
}
/***********************************************************************************
*       Descrição       :       Grava os parametros padrão de fábrica na memória
*       Parametros      :       (unsigned short int) tempo e preparo
*                               (unsigned short int) temperatura preparo
*                               (unsigned short int) velocidade preparo
*                               (unsigned short int) temperaturaPreProcesso
*                               (unsigned short int) atraso do pacote
*                               (unsigned short int) velocidade inicial expulsao
*                               (unsigned short int) velocidade final expulsão
*                               (unsigned short int) fator rampa temperatura
*                               (unsigned short int) velocidade de limpeza
*                               (unsigned short int) velocidade aquecimento
*                               (unsigned short int) volume do som
*                               (unsigned short int) volume da voz
*       Retorno         :       nenhum
***********************************************************************************/
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
                                            unsigned short int volumeVoz){
  unsigned char buffer[26];
  unsigned short int crc;
  
  buffer[0] = tempoPreparo>>8;
  buffer[1] = tempoPreparo;
  
  buffer[2] = temperaturaPreparo>>8;
  buffer[3] = temperaturaPreparo;
  
  buffer[4] = velocidadePreparo>>8;
  buffer[5] = velocidadePreparo;
  
  buffer[6] = temperaturaPreProcesso>>8;
  buffer[7] = temperaturaPreProcesso;
  
  buffer[8] = atrasoPacote>>8;
  buffer[9] = atrasoPacote;
  
  buffer[10]= velocidadeInicialExpulsao>>8;
  buffer[11]= velocidadeInicialExpulsao;
  
  buffer[12]= velocidadeFinalExpulsao>>8;
  buffer[13]= velocidadeFinalExpulsao;
  
  buffer[14]= fatorRampaTemperatura>>8;
  buffer[15]= fatorRampaTemperatura;
  
  buffer[16]= velocidadeLimpeza>>8;
  buffer[17]= velocidadeLimpeza;
  
  buffer[18]= velocidadeAquecimento>>8;
  buffer[19]= velocidadeAquecimento;
  
  buffer[20]= volumeSom>>8;
  buffer[21]= volumeSom;
  
  buffer[22]= volumeVoz>>8;
  buffer[23]= volumeVoz;
  
  crc = CCTALK_calculaCRC(buffer,0,24);
   
  buffer[24] = crc>>8;
  buffer[25] = crc;
                                              
  MEMORYWRAPPER_writeBytes(ADR_COPIA_PARAMETROS,buffer,26);
  
  return 0;
}
/***********************************************************************************
*       Descrição       :       Faz a leitura do parametros de configuração
*                               de fabrica que foram capturados no menu
*       Parametros      :       (unsigned short int*) tempo de prepraro
*                               (unsigned short int*) temperatura de preparo
*                               (unsigned short int*) velocidade de preparo
*                               (unsigned short int*) temperatura de pré-processo
*                               (unsigned short int*) atraso do pacote
*                               (unsigned short int*) rotação inicio da expulsão
*                               (unsigned short int*) rotação final da expulsão
*                               (unsigned short int*) fator de rampa de temperatura
*                               (unsigned short int*) velocidade de limpeza
*                               (unsigned short int*) velocidade pre aquecimento
*                               (unsigned short int*) volume do som
*                               (unsigned short int*) volume da voz
*       Retorno         :       (unsigned char ) maior do que zero se o CRC
*                                                estiver correto
***********************************************************************************/
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
                                                   ){
  unsigned char buffer[26];
  unsigned short int crc;
  
  MEMORYWRAPPER_readBytes(ADR_COPIA_PARAMETROS,buffer,26);  
  
  *tempoPreparo             = buffer[0]<<8  | buffer[1];
  *temperaturaPreparo       = buffer[2]<<8  | buffer[3];
  *velocidadePreparo        = buffer[4]<<8  | buffer[5];
  *temperaturaPreProcesso   = buffer[6]<<8  | buffer[7];
  *atrasoPacote             = buffer[8]<<8  | buffer[9];
  *rotacaoInicioExpulsao    = buffer[10]<<8 | buffer[11];
  *rotacaoFimExpulsao       = buffer[12]<<8 | buffer[13];
  *rampaTemperatura         = buffer[14]<<8 | buffer[15];
  *velocidadeLimpeza        = buffer[16]<<8 | buffer[17];
  *velocidadePreAquecimento = buffer[18]<<8 | buffer[19];
  *volumeSom                = buffer[20]<<8 | buffer[21];
  *volumeVoz                = buffer[22]<<8 | buffer[23];
   
  crc = CCTALK_calculaCRC(buffer,0,24);
  
  if(crc == (buffer[24]<<8 | buffer[25]))    
    return 1;                          
  else{
    
    *tempoPreparo = 80;
    *temperaturaPreparo = 100;
    *velocidadePreparo = 4500;
    *temperaturaPreProcesso = 80;
    *atrasoPacote = 25;
    *rotacaoInicioExpulsao = 11000;
    *rotacaoFimExpulsao = 15000;
    *rampaTemperatura = 30;
    *velocidadeLimpeza = 18000;
    *velocidadePreAquecimento = 3000;
    *volumeSom = 30;
    *volumeVoz = 50;    
  }
  
  return 0;
}
/***********************************************************************************
*       Descrição       :      Grava o flag locação na flash
*       Parametros      :       (unsigned char) flag locacao
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_gravaFlagLocacao(unsigned char flag){
  unsigned char buffer[3];
  unsigned short int crc;

  if(flag)
    flag = 255;
  
  buffer[0] = flag;
  crc = CCTALK_calculaCRC(buffer,0,1);
  buffer[1] = crc>>8;
  buffer[2] = crc;
  MEMORYWRAPPER_writeBytes(ADR_FLAG_LOCACAO,buffer,3);      
}
/***********************************************************************************
*       Descrição       :       Lê o flag locação da placa
*       Parametros      :       nenhum
*       Retorno         :       (unsigned char) flag locacao
***********************************************************************************/
unsigned char PARAMETROS_leFlagLocacao(void){
  unsigned char buffer[3];
  unsigned short int crc;

  MEMORYWRAPPER_readBytes(ADR_FLAG_LOCACAO,buffer,3);      
  crc = CCTALK_calculaCRC(buffer,0,1);    
  
  if(crc==(buffer[1]<<8 | buffer[2])){
    return buffer[0];
  }  
  return 0;
}
/***********************************************************************************
*       Descrição       :       Grava o parâmetro de temperatura para
*                               início do preparo
*       Parametros      :       (unsigned short int) temperatura de inicio do 
*                                                    preparo
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_gravaTemperaturaInicio(unsigned short int temperatura){
  unsigned char buffer[4];
  unsigned short int crc;
  
  buffer[0] = temperatura>>8;
  buffer[1] = (unsigned char)temperatura;
  crc = CCTALK_calculaCRC(buffer,0,2);
  buffer[2] = crc>>8;
  buffer[3] = crc;
  MEMORYWRAPPER_writeBytes(ADR_TEMP_INICIAL,buffer,4);    
}
/***********************************************************************************
*       Descrição       :       Lê o parâmetro de temperatura para fim
*                               do preparo
*       Parametros      :       nenhum
*       Retorno         :       (unsigned short int) temperatura
***********************************************************************************/
unsigned short int PARAMETROS_leTemperaturaInicio(void){
  unsigned char buffer[4];
  unsigned short int crc;

  MEMORYWRAPPER_readBytes(ADR_TEMP_INICIAL,buffer,4);      
  crc = CCTALK_calculaCRC(buffer,0,2);    
  
  if(crc==(buffer[2]<<8 | buffer[3])){
    return buffer[0]<<8 | buffer[1];
  }  
  return 65;      
}
/***********************************************************************************
*       Descrição       :       grava o parâmetro tempo de preaquecimento
*       Parametros      :       (unsigned shor int) tempo de pre aquecimento
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_gravaTempoPreAquecimento(unsigned short int tempo){
  unsigned char buffer[4];
  unsigned short int crc;
  
  buffer[0] = tempo>>8;
  buffer[1] = (unsigned char)tempo;
  crc = CCTALK_calculaCRC(buffer,0,2);
  buffer[2] = crc>>8;
  buffer[3] = crc;
  MEMORYWRAPPER_writeBytes(ADR_TEMPO_PRE_AQUECIMENTO,buffer,4);    
}
/***********************************************************************************
*       Descrição       :       Lê o parâmetro tempo de preaquecimento
*       Parametros      :       nenhum
*       Retorno         :       (unsigned short int) tempo de pre aquecimento
***********************************************************************************/
unsigned short int PARAMETROS_leTempoPreaquecimento(void){
  unsigned char buffer[4];
  unsigned short int crc;

  MEMORYWRAPPER_readBytes(ADR_TEMPO_PRE_AQUECIMENTO,buffer,4);      
  crc = CCTALK_calculaCRC(buffer,0,2);    
  
  if(crc==(buffer[2]<<8 | buffer[3])){
    return buffer[0]<<8 | buffer[1];
  }  
  return 5;      
}
/***********************************************************************************
*       Descrição       :       Grava uma das mensagens de cliente
*       Parametros      :       (unsigned char) indice
*                               (char*) mensagem
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_gravaMensagemCliente(unsigned char indice,char *mensagem){
  unsigned char bufferMensagem[19];
  unsigned short int crc;

  memcpy(bufferMensagem,mensagem,17);
  crc = CCTALK_calculaCRC(bufferMensagem,0,17);
  bufferMensagem[17] = crc>>8; 
  bufferMensagem[18] = crc;    
  MEMORYWRAPPER_writeBytes(ADR_MENSAGENS_TEXTO + 19*indice,bufferMensagem,19);
}
/***********************************************************************************
*       Descrição       :       Lê uma das mensagens de cliente
*       Parametros      :       (unsigned char) indice
*                               (char*) mensagem
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_leMensagemCliente(unsigned char indice,char *mensagem){
  const char mensagens[4][17]={"  Mais Pipoca   ",
                               "  Grupo Altech  ",
                               "  Experimente!  ",
                               "                "};
  unsigned char bufferMensagem[19];  
  unsigned short int crc;
  
  MEMORYWRAPPER_readBytes(ADR_MENSAGENS_TEXTO + 19*indice,bufferMensagem,19);
  crc = CCTALK_calculaCRC(bufferMensagem,0,17);
  
  if(crc == (bufferMensagem[17]<<8 | bufferMensagem[18]))    
    memcpy(mensagem,bufferMensagem,17);
  else        
    memcpy(mensagem,mensagens[indice],17);  
}
/***********************************************************************************
*       Descrição       :       Grava o parâmetro ciclo de desumificação
*       Parametros      :       (unsigned char) maior do que zero para ativa
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_gravaCicloDesumificacao(unsigned char ciclo){
  unsigned char buffer[3];
  unsigned short int crc;

  if(ciclo)
    ciclo = 255;
  
  buffer[0] = ciclo;
  crc = CCTALK_calculaCRC(buffer,0,1);
  buffer[1] = crc>>8;
  buffer[2] = crc;
  MEMORYWRAPPER_writeBytes(ADR_FLAG_DESUMIDIFICACAO,buffer,3);  
}
/***********************************************************************************
*       Descrição       :       Lê o parâmetro ciclo de desumificacação 
*       Parametros      :       nenhum
*       Retorno         :       (unsigned char) maior do que zero se for 
*                               para ativar o ciclo
***********************************************************************************/
unsigned char PARAMETROS_leCicloDesumidificacao(void){
  unsigned char buffer[3];
  unsigned short int crc;

  MEMORYWRAPPER_readBytes(ADR_FLAG_DESUMIDIFICACAO,buffer,3);      
  crc = CCTALK_calculaCRC(buffer,0,1);    
  
  if(crc==(buffer[1]<<8 | buffer[2])){
    return buffer[0];
  }  
  return 0;
}
/***********************************************************************************
*       Descrição       :       Salva o parâmetro velocidade inicial
*                               da expulsão da pipoca
*       Parametros      :       (unsigned short int) rotação inicial
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_salvaVelocidadeDesumidificacao(unsigned short int rpm){
  unsigned short int crc;
  unsigned char buffer[4];
  
  buffer[0] = rpm>>8;
  buffer[1] = rpm;
  crc = CCTALK_calculaCRC(buffer,0,2);
  buffer[2] = crc>>8;
  buffer[3] = crc;
  
  MEMORYWRAPPER_writeBytes(ADR_VEL_DESUMIDIFICACAO,buffer,4);      
}
/***********************************************************************************
*       Descrição       :       Lê o parâmetro velocidade inicial
*       Parametros      :       nenhum
*       Retorno         :       (unsigned short int) rotação inicial
***********************************************************************************/
unsigned short int PARAMETROS_leVelocidadeDesumidificacao(void){
  unsigned char buffer[4];
  unsigned short int crc;
  
  MEMORYWRAPPER_readBytes(ADR_VEL_DESUMIDIFICACAO,buffer,4);
  crc = CCTALK_calculaCRC(buffer,0,2);
  
  if(crc==(buffer[2]<<8 | buffer[3])){
    
    crc = buffer[0]<<8 | buffer[1];
    if(crc<4000)
      crc=4000;
    if(crc>18000)
      crc=18000;
    
    return crc;
  }
  
  return 6000;
}
/***********************************************************************************
*       Descrição       :       Lê a temperatura de desumidificação
*       Parametros      :       (unsigned short int) temperatura
*       Retorno         :       nenhum
***********************************************************************************/
unsigned short int PARAMETROS_leTemperaturaDesumidificacao(void){
  unsigned char buffer[4];
  unsigned short int crc;
  
  MEMORYWRAPPER_readBytes(ADR_TEMP_DESUMIDIFICACAO,buffer,4);
  crc = CCTALK_calculaCRC(buffer,0,2);
  
  if(crc==(buffer[2]<<8 | buffer[3])){
    
    crc = buffer[0]<<8 | buffer[1];
    if(crc<50)
      crc=50;
    if(crc>140)
      crc=140;
    
    return crc;
  }
  
  return 80;
}
/***********************************************************************************
*       Descrição       :       Salva a temperatura de desumidificação
*       Parametros      :       (unsigned short int) temperatura
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_salvaTemperaturaDesumidificacao(unsigned short int temperatura){
  unsigned short int crc;
  unsigned char buffer[4];
  
  buffer[0] = temperatura>>8;
  buffer[1] = temperatura;
  crc = CCTALK_calculaCRC(buffer,0,2);
  buffer[2] = crc>>8;
  buffer[3] = crc;
  
  MEMORYWRAPPER_writeBytes(ADR_TEMP_DESUMIDIFICACAO,buffer,4);    
}
/***********************************************************************************
*       Descrição       :       Salva o contador de pipoca feitas pelo menu
*                               de teste
*       Parametros      :       (unsigned short int) total
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_salvaContadorPipocaTeste(unsigned short int pipocaTeste){
  unsigned short int crc;
  unsigned char buffer[4];
  
  buffer[0] = pipocaTeste>>8;
  buffer[1] = pipocaTeste;
  crc = CCTALK_calculaCRC(buffer,0,2);
  buffer[2] = crc>>8;
  buffer[3] = crc;
  
  MEMORYWRAPPER_writeBytes(ADR_CONTADOR_PIPOCAS_TESTE,buffer,4);     
}
/***********************************************************************************
*       Descrição       :       Lê o contador de pipocas feitas no menu
*                               de teste
*       Parametros      :       nenhum
*       Retorno         :       (unsigned short int) quantidade de pipocas
*                               feitas no menu de teste da maquina
***********************************************************************************/
unsigned short int PARAMETROS_leContadorPipocasTeste(void){
  unsigned char buffer[4];
  unsigned short int crc;
  
  MEMORYWRAPPER_readBytes(ADR_CONTADOR_PIPOCAS_TESTE,buffer,4);
  crc = CCTALK_calculaCRC(buffer,0,2);
  
  if(crc==(buffer[2]<<8 | buffer[3])){
    
    crc = buffer[0]<<8 | buffer[1];        
    return crc;
  }
  
  return 0;
}
/***********************************************************************************
*       Descrição       :       Salva o tipo do noteiro na memória flash
*       Parametros      :       (eTIPO_NOTEIRO) tipo do noteiro
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_salvaTipoNoteiro(eTIPO_NOTEIRO tipo){
  unsigned char buffer[4];
  unsigned short int crc;
  
  buffer[0] = (unsigned char)tipo>>8;
  buffer[1] = (unsigned char)tipo;
  crc = CCTALK_calculaCRC(buffer,0,2);
  buffer[2] = crc>>8;
  buffer[3] = crc;
  MEMORYWRAPPER_writeBytes(ADR_TIPO_NOTEIRO,buffer,4);         
}
/***********************************************************************************
*       Descrição       :       Lê o tipo do noteiro configurado para a 
*                               aplicação
*       Parametros      :       nenhum
*       Retorno         :       (eTIPO_NOTEIRO) tipo do noteiro
***********************************************************************************/
eTIPO_NOTEIRO PARAMETROS_leTipoNoteiro(void){
  unsigned char buffer[4];
  unsigned short int crc;
  
  MEMORYWRAPPER_readBytes(ADR_TIPO_NOTEIRO,buffer,4);
  crc = CCTALK_calculaCRC(buffer,0,2);
  
  if(crc==(buffer[2]<<8 | buffer[3])){        
    return (eTIPO_NOTEIRO)(buffer[0]<<8 | buffer[1]);
  }
  
  return BV20;
}
/***********************************************************************************
*       Descrição       :       Salva o tipo da impressora na memória
*       Parametros      :       (eTIPO_IMPRESSORA) tipo da impressora
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_salvaTipoImpressora(eTIPO_IMPRESSORA tipo){
  unsigned char buffer[4];
  unsigned short int crc;
  
  buffer[0] = (unsigned char)tipo>>8;
  buffer[1] = (unsigned char)tipo;
  crc = CCTALK_calculaCRC(buffer,0,2);
  buffer[2] = crc>>8;
  buffer[3] = crc;
  MEMORYWRAPPER_writeBytes(ADR_TIPO_IMPRESSORA,buffer,4);           
}
/***********************************************************************************
*       Descrição       :       Carrega o tipo da impressora
*       Parametros      :       nenhum
*       Retorno         :       (eTIPO_IMPRESSORA) tipo da impressora
***********************************************************************************/
eTIPO_IMPRESSORA PARAMETROS_carregaTipoImpressora(void){
  unsigned char buffer[4];
  unsigned short int crc;
  
  MEMORYWRAPPER_readBytes(ADR_TIPO_IMPRESSORA,buffer,4);
  crc = CCTALK_calculaCRC(buffer,0,2);
  
  if(crc==(buffer[2]<<8 | buffer[3])){        
    return (eTIPO_IMPRESSORA)(buffer[0]<<8 | buffer[1]);
  }
  
  return EXTECH;
}
/***********************************************************************************
*       Descrição       :       Salva o contador de operação realizadas
*                               com o cartão de crédito
*       Parametros      :       (unsigned int) transações
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_salvaContadorOperacoesCartao(unsigned int operacoes){
  unsigned char buffer[6];
  unsigned short int crc;
  
  buffer[0] = operacoes>>24;
  buffer[1] = operacoes>>16;
  buffer[2] = operacoes>>8;
  buffer[3] = operacoes;
  
  crc = CCTALK_calculaCRC(buffer,0,4);
  
  buffer[4] = crc>>8;
  buffer[5] = crc;
  
  MEMORYWRAPPER_writeBytes(ADR_OPERACOES_CARTAO,buffer,6); 
}
/***********************************************************************************
*       Descrição       :       Carrega o contador de operações realizadas
*                               com o cartão de crédito
*       Parametros      :       nenhum
*       Retorno         :       (unsigned int) operações realizada com o 
*                               cartão de crédito
***********************************************************************************/
unsigned int PARAMETROS_carregaOperacoesCartao(void){
  unsigned char buffer[6];
  unsigned short int crc;

  MEMORYWRAPPER_readBytes(ADR_OPERACOES_CARTAO,buffer,6);

  crc = CCTALK_calculaCRC(buffer,0,4);

  if(crc == buffer[4]<<8 | buffer[5])       
    return buffer[0]<<24 | buffer[1]<<16 | buffer[2]<<8 | buffer[3];
  
  return 0; // valor de operações 
}
/***********************************************************************************
*       Descrição       :       Lê o tipo do moedeiro configurado para
*                               a máquina
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
eTIPO_MOEDEIRO PARAMETROS_carregaTipoMoedeiro(void){
  unsigned char buffer[4];
  unsigned short int crc;
  
  MEMORYWRAPPER_readBytes(ADR_TIPO_MOEDEIRO,buffer,4);
  crc = CCTALK_calculaCRC(buffer,0,2);
  
  if(crc==(buffer[2]<<8 | buffer[3])){        
    return (eTIPO_NOTEIRO)(buffer[0]<<8 | buffer[1]);
  }
  
  return UCA1;    
}
/***********************************************************************************
*       Descrição       :       Salva o tipo do moedeiro na memória
*       Parametros      :       (eTIPO_MOEDEIRO) tipo do moedeiro
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_salvaTipoMoedeiro(eTIPO_MOEDEIRO tipo){
  unsigned char buffer[4];
  unsigned short int crc;
  
  buffer[0] = (unsigned char)tipo>>8;
  buffer[1] = (unsigned char)tipo;
  crc = CCTALK_calculaCRC(buffer,0,2);
  buffer[2] = crc>>8;
  buffer[3] = crc;
  MEMORYWRAPPER_writeBytes(ADR_TIPO_MOEDEIRO,buffer,4);    
}
/***********************************************************************************
*       Descrição       :       Lê o valor salvo para o número de série da 
*                               máquina
*       Parametros      :       nenhum
*       Retorno         :       (unsigned int) número de série
***********************************************************************************/
unsigned int PARAMETROS_carregaNumeroSerie(void){
  unsigned char buffer[6];
  unsigned short int crc;
  
  MEMORYWRAPPER_readBytes(ADR_NUMERO_SERIE,buffer,6);
  crc = CCTALK_calculaCRC(buffer,0,4);
  
  if(crc==(buffer[4]<<8 | buffer[5])){
    return (unsigned int)(buffer[0]<<24 | buffer[1]<<16 | buffer[2]<<8 | buffer[3]);
  }
  
  return 100;  
}
/***********************************************************************************
*       Descrição       :       Salva o valr para o número de série
*                               da máquina
*       Parametros      :       (unsigned int) número de série da máquina
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_setNumeroSerie(unsigned int numeroSerie){
  unsigned char buffer[6];
  unsigned short int crc;
  
  buffer[0] = (unsigned char)(numeroSerie>>24);
  buffer[1] = (unsigned char)(numeroSerie>>16);
  buffer[2] = (unsigned char)(numeroSerie>>8);
  buffer[3] = (unsigned char)numeroSerie;
  crc = CCTALK_calculaCRC(buffer,0,4);
  buffer[4] = crc>>8;
  buffer[5] = crc;
  MEMORYWRAPPER_writeBytes(ADR_NUMERO_SERIE,buffer,6);     
}
/***********************************************************************************
*       Descrição       :       Carrega o faturamento parcial do cartão
*                               a partir da memória flash de dados
*       Parametros      :       nenhum
*       Retorno         :      (unsigned int) faturamento parcial do cartão
***********************************************************************************/
unsigned int PARAMETROS_carregaFaturamentoParcialCartao(void){
  unsigned char buffer[6];
  unsigned short int crc;
  
  MEMORYWRAPPER_readBytes(ADR_FAT_CARTAO_PARCIAL,buffer,6);
  crc = CCTALK_calculaCRC(buffer,0,4);
  
  if(crc==(buffer[4]<<8 | buffer[5])){
    return (unsigned int)(buffer[0]<<24 | buffer[1]<<16 | buffer[2]<<8 | buffer[3]);
  }  
  
  return 0;
}
/***********************************************************************************
*       Descrição       :       Salva o faturamento parcial do cartão
*       Parametros      :       (unsigned int) faturamento parcial
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_salvaFaturamentoParcialCartao(unsigned int faturamento){
  unsigned char buffer[6];
  unsigned short int crc;
  
  buffer[0] = faturamento>>24;
  buffer[1] = faturamento>>16;
  buffer[2] = faturamento>>8;
  buffer[3] = faturamento;
  
  crc = CCTALK_calculaCRC(buffer,0,4);
  
  buffer[4] = crc>>8;
  buffer[5] = crc;
  
  MEMORYWRAPPER_writeBytes(ADR_FAT_CARTAO_PARCIAL,buffer,6);    
}
/***********************************************************************************
*       Descrição       :       Carrega o faturamento 
*       Parametros      :       nenhum
*       Retorno         :       (unsigned int) faturamento total com o cartão
***********************************************************************************/
unsigned int PARAMETROS_carregaFaturamentoTotalCartao(void){  
  unsigned char buffer[6];
  unsigned short int crc;
  
  MEMORYWRAPPER_readBytes(ADR_FAT_CARTAO_TOTAL,buffer,6);
  crc = CCTALK_calculaCRC(buffer,0,4);
  
  if(crc==(buffer[4]<<8 | buffer[5])){
    return (unsigned int)(buffer[0]<<24 | buffer[1]<<16 | buffer[2]<<8 | buffer[3]);
  }
   
  return 0;
}
/***********************************************************************************
*       Descrição       :       Salva o faturamento total
*                               com o cartão
*       Parametros      :       (unsigned int) faturamento
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_salvaFaturamentoTotalCartao(unsigned int faturamento){
  unsigned char buffer[6];
  unsigned short int crc;
  
  buffer[0] = faturamento>>24;
  buffer[1] = faturamento>>16;
  buffer[2] = faturamento>>8;
  buffer[3] = faturamento;
  
  crc = CCTALK_calculaCRC(buffer,0,4);
  
  buffer[4] = crc>>8;
  buffer[5] = crc;
  
  MEMORYWRAPPER_writeBytes(ADR_FAT_CARTAO_TOTAL,buffer,6); 
}
/***********************************************************************************
*       Descrição       :       Grava a senha de acesso ao modo root
*       Parametros      :       (unsigned short int ) senha de root
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_gravaSenhaRoot(unsigned short int senha){
  unsigned char buffer[4];
  unsigned short int crc;
   
  buffer[0] = senha>>8;
  buffer[1] = senha;
  
  
  crc = CCTALK_calculaCRC(buffer,0,2);
  
  buffer[2] = crc>>8;
  buffer[3] = crc;
  
  MEMORYWRAPPER_writeBytes(ADR_SENHA_ROOT,buffer,4); 
}
/***********************************************************************************
*       Descrição       :       Lê a senha de acesso ao modo root
*       Parametros      :       nenhum
*       Retorno         :       (unsigned short int) senha de root
***********************************************************************************/
unsigned short int PARAMETROS_leSenhaRoot(void){
  unsigned char buffer[4];
  unsigned short int crc;
  
  MEMORYWRAPPER_readBytes(ADR_SENHA_ROOT,buffer,4);
  crc = CCTALK_calculaCRC(buffer,0,2);
  
  if(crc==(buffer[2]<<8 | buffer[3])){
    return (unsigned int)(buffer[0]<<8 | buffer[1]);
  }
   
  return 0;
}
/***********************************************************************************
*       Fim do arquivo
***********************************************************************************/
