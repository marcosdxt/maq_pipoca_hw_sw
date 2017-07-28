/*__________________________________________________________________________________
|	Quark Tecnologia Eletr�nica Embarcada
|       
|       Itapema - SC
|       www.quarktee.com.br
| __________________________________________________________________________________
|
|       This source code was developed by Quark  and cannot be copied, in part 
|       or in whole, or used, except when legally licensed by Quark 
|       or its distributors.
|
|       Este c�digo � propriedade da Quark  e n�o pode ser copiado, em parte 
|       ou em todo, ou utilizado, exceto quando for legalmente licenciado pela 
|       Quark  ou por um de seus distribuidores.
| __________________________________________________________________________________
|
|       Arquivo            :  Parametros.c
| 
|       Autor              :  Marcos Aquino
|       Data cria��o       :  11/03/2014
|
|       Revis�es           :  1.0.0.0
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
*       Defini��es
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
*       Fun��es locais
***********************************************************************************/
extern unsigned short int CCTALK_calculaCRC(unsigned char *pData,unsigned short int offset,
                                            unsigned short int tamanho);

/***********************************************************************************
*       Implementa��o das fun��es
***********************************************************************************/

/***********************************************************************************
*       Descri��o       :      Altera os dados de uma das posi��es da tabela
*                              de indexa��o dos arquivos de audio
*       Parametros      :      (unsigned char) indice da m�sica
*                              (unsigned long int) endere�o inicial da musica
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
*       Descri��o       :       L� os dados de uma das posi��es da tabela
*                               de indexa��o dos arquivos de audio
*       Parametros      :       (unsigned char) indice da m�sica
*                               (unsigned long int*) endere�o inicial da m�sica
*                               (unsigned long int*) tamanho da m�sica
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
*       Descri��o       :       Escreve na tabela de parametros do sistema
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
*       Descri��o       :       L� um parametro a partir da tabela de parametros
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
         limiteSuperior = 65000;
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
*       Descri��o       :       Grava o beta do termistor
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
*       Descri��o       :       Carrega o beta do transistor 
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
  
  return 4750; // valor padr�o para o termistor utilizado no projeto
}
/***********************************************************************************
*       Descri��o       :       Grava o parametro RT na flash
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
*       Descri��o       :       L� o parametro RT a partir da flash
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
  
  return 120505; // Valor padr�o para o termistor escolhido no projeto                   
}
/***********************************************************************************
*       Descri��o       :       Configura quais c�dulas ser�o aceitas pelo 
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
*       Descri��o       :       Verifica quais c�dulas est�o habilitadas
*                               para uso na m�quina
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
*       Descri��o       :       L� o valor do contador de vendas
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
*       Descri��o       :       Salva o valor do contador de vendas
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
*       Descri��o       :      L� o contador de arrecada��o
*       Parametros      :       nenhum
*       Retorno         :       (unsigned int) contador de arrecada��o financeira
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
*       Descri��o       :       Salva o contador de arrecadacao
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
*       Descri��o       :       L� o contador de dinheiro permanente
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
*       Descri��o       :       Escreve no contador de dinheiro permanente
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
*       Descri��o       :       L� o par�metro tempo de propaganda
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
*       Descri��o       :       Escreve no par�metro tempo de propaganda
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
*       Descri��o       :       L� o par�metro que indica se as fitas de led
*                               frontais ficam piscando ou n�o
*       Parametros      :       nenhum
*       Retorno         :       (unsigned short int) maior do que zero sim
*                                                    igual a zero n�o
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
*       Descri��o       :       Escreve o par�metro que indica se as fitas de led
*                               devem piscar ou n�o
*       Parametros      :       (unsigned short int) maior do que zero sim
*                                                    igual a zero n�o
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
*       Descri��o       :       L� o par�metro temperatura de pr�-processo
*       Parametros      :       nenhum
*       Retorno         :       (unsigned short int) temperatura de pr�-processo
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
*       Descri��o       :       Salva a temperatura de pr�-processo
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
*       Descri��o       :       Salva o par�metro velocidade inicial
*                               da expuls�o da pipoca
*       Parametros      :       (unsigned short int) rota��o inicial
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
*       Descri��o       :       L� o par�metro velocidade inicial
*       Parametros      :       nenhum
*       Retorno         :       (unsigned short int) rota��o inicial
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
*       Descri��o       :       Salva o par�metro velocidade final
*                               de expuls�o
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
*       Descri��o       :       L� o par�metro velocidade final de expuls�o
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
*       Descri��o       :       L� o parametro do volume do som
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
*       Descri��o       :       Escreve no parametro volume do som
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
*       Descri��o       :       L� o fato de trabalho do ajuste da temperatura
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
*       Descri��o       :       Salva o fator de trabalho da temperatura
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
*       Descri��o       :       L� o par�metro velocidade da limpeza
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
*       Descri��o       :       Grava a velocidade da limpeza
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
*       Descri��o       :       L� a velocidade do pr�-aquecimento
*       Parametros      :       nenhum
*       Retorno         :       (unsigned short int) velocidade pr�-aquecimento
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
*       Descri��o       :       Grava a velocidade de pr�-aquecimento
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
*       Descri��o       :       L� o volume da voz na flash
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
*       Descri��o       :      Grava o volume da voz na flash 
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
*       Descri��o       :       L� o par�metro que indica se a fita deve
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
*       Descri��o       :       Grava o par�metro que indica se a fita
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
*       Descri��o       :       L� o par�metro que diz qual a comiss�o para
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
*       Descri��o       :       L� o par�metro que indica a comiss�o 
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
*       Descri��o       :       Grava um valor no contador de vendas
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
*       Descri��o       :       L� o valor do contador de vendas parcial
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
*       Descri��o       :       Grava os parametros padr�o de f�brica na mem�ria
*       Parametros      :       (unsigned short int) tempo e preparo
*                               (unsigned short int) temperatura preparo
*                               (unsigned short int) velocidade preparo
*                               (unsigned short int) temperaturaPreProcesso
*                               (unsigned short int) atraso do pacote
*                               (unsigned short int) velocidade inicial expulsao
*                               (unsigned short int) velocidade final expuls�o
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
*       Descri��o       :       Faz a leitura do parametros de configura��o
*                               de fabrica que foram capturados no menu
*       Parametros      :       (unsigned short int*) tempo de prepraro
*                               (unsigned short int*) temperatura de preparo
*                               (unsigned short int*) velocidade de preparo
*                               (unsigned short int*) temperatura de pr�-processo
*                               (unsigned short int*) atraso do pacote
*                               (unsigned short int*) rota��o inicio da expuls�o
*                               (unsigned short int*) rota��o final da expuls�o
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
*       Descri��o       :      Grava o flag loca��o na flash
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
*       Descri��o       :       L� o flag loca��o da placa
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
*       Descri��o       :       Grava o par�metro de temperatura para
*                               in�cio do preparo
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
*       Descri��o       :       L� o par�metro de temperatura para fim
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
*       Descri��o       :       grava o par�metro tempo de preaquecimento
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
*       Descri��o       :       L� o par�metro tempo de preaquecimento
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
*       Descri��o       :       Grava uma das mensagens de cliente
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
*       Descri��o       :       L� uma das mensagens de cliente
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
*       Descri��o       :       Grava o par�metro ciclo de desumifica��o
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
*       Descri��o       :       L� o par�metro ciclo de desumificaca��o 
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
*       Descri��o       :       Salva o par�metro velocidade inicial
*                               da expuls�o da pipoca
*       Parametros      :       (unsigned short int) rota��o inicial
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
*       Descri��o       :       L� o par�metro velocidade inicial
*       Parametros      :       nenhum
*       Retorno         :       (unsigned short int) rota��o inicial
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
*       Descri��o       :       L� a temperatura de desumidifica��o
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
*       Descri��o       :       Salva a temperatura de desumidifica��o
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
*       Descri��o       :       Salva o contador de pipoca feitas pelo menu
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
*       Descri��o       :       L� o contador de pipocas feitas no menu
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
*       Descri��o       :       Salva o tipo do noteiro na mem�ria flash
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
*       Descri��o       :       L� o tipo do noteiro configurado para a 
*                               aplica��o
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
*       Descri��o       :       Salva o tipo da impressora na mem�ria
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
*       Descri��o       :       Carrega o tipo da impressora
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
*       Descri��o       :       Salva o contador de opera��o realizadas
*                               com o cart�o de cr�dito
*       Parametros      :       (unsigned int) transa��es
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
*       Descri��o       :       Carrega o contador de opera��es realizadas
*                               com o cart�o de cr�dito
*       Parametros      :       nenhum
*       Retorno         :       (unsigned int) opera��es realizada com o 
*                               cart�o de cr�dito
***********************************************************************************/
unsigned int PARAMETROS_carregaOperacoesCartao(void){
  unsigned char buffer[6];
  unsigned short int crc;

  MEMORYWRAPPER_readBytes(ADR_OPERACOES_CARTAO,buffer,6);

  crc = CCTALK_calculaCRC(buffer,0,4);

  if(crc == buffer[4]<<8 | buffer[5])       
    return buffer[0]<<24 | buffer[1]<<16 | buffer[2]<<8 | buffer[3];
  
  return 0; // valor de opera��es 
}
/***********************************************************************************
*       Descri��o       :       L� o tipo do moedeiro configurado para
*                               a m�quina
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
*       Descri��o       :       Salva o tipo do moedeiro na mem�ria
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
*       Descri��o       :       L� o valor salvo para o n�mero de s�rie da 
*                               m�quina
*       Parametros      :       nenhum
*       Retorno         :       (unsigned int) n�mero de s�rie
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
*       Descri��o       :       Salva o valr para o n�mero de s�rie
*                               da m�quina
*       Parametros      :       (unsigned int) n�mero de s�rie da m�quina
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
*       Descri��o       :       Carrega o faturamento parcial do cart�o
*                               a partir da mem�ria flash de dados
*       Parametros      :       nenhum
*       Retorno         :      (unsigned int) faturamento parcial do cart�o
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
*       Descri��o       :       Salva o faturamento parcial do cart�o
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
*       Descri��o       :       Carrega o faturamento 
*       Parametros      :       nenhum
*       Retorno         :       (unsigned int) faturamento total com o cart�o
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
*       Descri��o       :       Salva o faturamento total
*                               com o cart�o
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
*       Descri��o       :       Grava a senha de acesso ao modo root
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
*       Descri��o       :       L� a senha de acesso ao modo root
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
*       Descri��o       :       Grava o valor do cr�dito salvo para o modeiro
*                               UCA-1
*       Parametros      :       (unsigned short int) valor configurado
*                                                    para o pulso do UCA1
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_gravaValorCredito(unsigned short int credito){
  unsigned char buffer[4];
  unsigned short int crc;
   
  buffer[0] = credito>>8;
  buffer[1] = credito;
  
  
  crc = CCTALK_calculaCRC(buffer,0,2);
  
  buffer[2] = crc>>8;
  buffer[3] = crc;
  
  MEMORYWRAPPER_writeBytes(ADR_VALOR_CREDITO_UCA,buffer,4);  
}
/***********************************************************************************
*       Descri��o       :       L� o valor do cr�dito salvo para o moedeiro
*                               UCA-1
*       Parametros      :       nenhum
*       Retorno         :       (unsigned short int) valor do pulso
***********************************************************************************/
unsigned short int PARAMETROS_leValorCredito(void){
  unsigned char buffer[4];
  unsigned short int crc;
  
  MEMORYWRAPPER_readBytes(ADR_VALOR_CREDITO_UCA,buffer,4);
  crc = CCTALK_calculaCRC(buffer,0,2);
  
  if(crc==(buffer[2]<<8 | buffer[3])){
    return (unsigned int)(buffer[0]<<8 | buffer[1]);
  }
   
  return 25;
}
/***********************************************************************************
*       Descri��o       :      Grava o par�metro ganho Proporcional
*       Parametros      :      (unsigned short int) ganho KP
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_grava_ganho_KP(unsigned short int kp){
  unsigned char buffer[4];
  unsigned short int crc;
  
  buffer[0] = kp>>8;
  buffer[1] = kp;
  crc = CCTALK_calculaCRC(buffer,0,2);   
  buffer[2] = crc>>8;
  buffer[3] = crc;
  
  MEMORYWRAPPER_writeBytes(ADR_GANHO_KP,buffer,4);
}
/***********************************************************************************
*       Descri��o       :       L� o ganho proporcional
*       parametros      :       nnehum
*       Retorno         :       (unsigned short int) ganho KP
***********************************************************************************/
unsigned short int PARAMETROS_le_ganho_KP(void){
  unsigned char buffer[4];
  unsigned short int crc;
  
  MEMORYWRAPPER_readBytes(ADR_GANHO_KP,buffer,4);
  crc = CCTALK_calculaCRC(buffer,0,2);
  
  if(crc==(buffer[2]<<8 | buffer[3])){
    return (unsigned int)(buffer[0]<<8 | buffer[1]);
  }
   
  return 20;  
}
/***********************************************************************************
*       Descri��o       :       Grava o ganho integral
*       Parametros      :       (unsigned short int) ganho Ki
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_grava_ganho_KI(unsigned short int ki){
  unsigned char buffer[4];
  unsigned short int crc;
  
  buffer[0] = ki>>8;
  buffer[1] = ki;
  crc = CCTALK_calculaCRC(buffer,0,2);   
  buffer[2] = crc>>8;
  buffer[3] = crc;
  
  MEMORYWRAPPER_writeBytes(ADR_GANHO_KI,buffer,4);
}
/***********************************************************************************
*       Descri��o       :       Le o ganho KI
*       Parametros      :       nenhum
*       Retorno         :       (unsigned short int) ganho Ki
***********************************************************************************/
unsigned short int PARAMETROS_le_ganho_KI(void){
  unsigned char buffer[4];
  unsigned short int crc;
  
  MEMORYWRAPPER_readBytes(ADR_GANHO_KI,buffer,4);
  crc = CCTALK_calculaCRC(buffer,0,2);
  
  if(crc==(buffer[2]<<8 | buffer[3])){
    return (unsigned int)(buffer[0]<<8 | buffer[1]);
  }
   
  return 1;  
}
/***********************************************************************************
*       Descri��o       :       Grava o ganho KD
*       Parametros      :       (unsigned short int) grava o ganho derivativo
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_grava_ganho_KD(unsigned short int kd){
  unsigned char buffer[4];
  unsigned short int crc;
  
  buffer[0] = kd>>8;
  buffer[1] = kd;
  crc = CCTALK_calculaCRC(buffer,0,2);   
  buffer[2] = crc>>8;
  buffer[3] = crc;
  
  MEMORYWRAPPER_writeBytes(ADR_GANHO_KD,buffer,4);
}
/***********************************************************************************
*       Descri��o       :       Le o ganho KD
*       Parametros      :       nenhum
*       Retorno         :       (unsigned short int) ganho KD
***********************************************************************************/
unsigned short int PARAMETROS_le_ganho_KD(void){
  unsigned char buffer[4];
  unsigned short int crc;
  
  MEMORYWRAPPER_readBytes(ADR_GANHO_KD,buffer,4);
  crc = CCTALK_calculaCRC(buffer,0,2);
  
  if(crc==(buffer[2]<<8 | buffer[3])){
    return (unsigned int)(buffer[0]<<8 | buffer[1]);
  }
   
  return 50;  
}
/***********************************************************************************
*       Descri��o       :       Le versao da CPU
*       Parametros      :       nenhum
*       Retorno         :       vsCPU
***********************************************************************************/
char* PARAMETROS_leVersaoCPU(void){
  static const char versao[]={STR_VERSAO_BOARD_TELEMETRIA};
  
  return (char*)versao;
} 
/***********************************************************************************
*       Descri��o       :       Grava correcao de erro
*       Parametros      :       (unsigned short int) correcao_erro
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_grava_correcao_erro(unsigned short int correcao_erro){
  unsigned char buffer[3];
  unsigned short int crc;
  
  buffer[0] = correcao_erro;
  crc = CCTALK_calculaCRC(buffer,0,1);   
  buffer[1] = crc>>8;
  buffer[2] = crc;
  
  MEMORYWRAPPER_writeBytes(ADR_CORRECAO_ERRO,buffer,3);
}
/***********************************************************************************
*       Descri��o       :       le correcao de erro
*       Parametros      :       nenhum
*       Retorno         :       (unsigned short int) erro
***********************************************************************************/
unsigned short int PARAMETROS_le_correcao_erro(void){
  unsigned char buffer[3];
  unsigned short int crc;
  
  MEMORYWRAPPER_readBytes(ADR_CORRECAO_ERRO,buffer,3);
  crc = CCTALK_calculaCRC(buffer,0,1);
  
  if(crc==(buffer[1]<<8 | buffer[2])){
    return (unsigned int)buffer[0];
  }
   
  return 1;  
}
/***********************************************************************************
*       Descri��o       :       Grava constante de temperatura
*       Parametros      :       (unsigned short int) valor
*       Retorno         :       nenhum
***********************************************************************************/
void PARAMETROS_grava_constante_temperatura(unsigned short int valor){
  unsigned char buffer[3];
  unsigned short int crc;
 
  buffer[0] = valor;
  crc = CCTALK_calculaCRC(buffer,0,1);   
  buffer[1] = crc>>8;
  buffer[2] = crc;
  
  MEMORYWRAPPER_writeBytes(ADR_COSTANTE_TEMPERATURA,buffer,3);
}
/***********************************************************************************
*       Descri��o       :       le constante de temperatura
*       Parametros      :       nenhum
*       Retorno         :       (unsigned short int) valor
***********************************************************************************/
unsigned short int PARAMETROS_le_constante_temperatura(void){
  unsigned char buffer[3];
  unsigned short int crc;
  
  MEMORYWRAPPER_readBytes(ADR_COSTANTE_TEMPERATURA,buffer,3);
  crc = CCTALK_calculaCRC(buffer,0,1);
  
  if(crc==(buffer[1]<<8 | buffer[2])){
    return (unsigned int)buffer[0];
  }
  return 1;  
}
/***********************************************************************************
*       Fim do arquivo
***********************************************************************************/
