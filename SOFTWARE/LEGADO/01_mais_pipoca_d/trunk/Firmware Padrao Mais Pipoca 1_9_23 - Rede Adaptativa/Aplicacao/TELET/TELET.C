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
|       Arquivo            :  TELET.C
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  21/07/2015
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
#include "..\..\includes.h"
#include "telet.h"

/***********************************************************************************
*       Definições locais
***********************************************************************************/
#define TAM_BUF_TX              128
#define TAM_BUF_RX              128
#define STX                     0x02
#define SILENT_TIME_RELOAD      5
#define RELOAD_TIME_OUT         600

/***********************************************************************************
*       Variaveis locais
***********************************************************************************/
unsigned char TELET_bufferTX[TAM_BUF_TX];
unsigned char TELET_bufferRX[TAM_BUF_RX];
unsigned char TELET_bytesRecebidos;
unsigned char TELET_bytesEnviados;
unsigned char TELET_bytesParaEnviar;
unsigned short TELET_silentTime=0;

/***********************************************************************************
*       Funções locais
***********************************************************************************/
unsigned char TELET_checksum(unsigned char *pData,unsigned char tamanho);

/***********************************************************************************
*       Implementação das funções
***********************************************************************************/

/***********************************************************************************
*       Descrição       :       Inicialização do módulo
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void TELET_ini(void){
     
  UART_init(1,4800,NULL,NULL,NULL);  
}
/***********************************************************************************
*       Descrição       :       Vetor de interrupção da uart1
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void TELET_vetor_interrupcao(void){
  unsigned char dummy;
  
  switch(U1IIR_bit.IID){
    case THRE:
               if(TELET_bytesParaEnviar){
                 U1THR = TELET_bufferTX[TELET_bytesEnviados++];
                 TELET_bytesParaEnviar--;
               }
               break; 
    case RDA : dummy = U1RBR;        
               if(TELET_bytesRecebidos<(TAM_BUF_RX-1)){
                 TELET_bufferRX[TELET_bytesRecebidos++] = dummy;
                 TELET_silentTime = SILENT_TIME_RELOAD;
               }
               break;      
    case RLS : 
               dummy = U1LSR;
               dummy = U1RBR;               
               break;      
    case CTI :
               break;      
  }    
  //Apaga o flag de interrupção pendente
  CLRPEND0 |= (0x01)<<6;    
}
/***********************************************************************************
*       Descrição       :       Envia um pacote pelo canal serial
*       Parametros      :       (unsigned char) tamanho do pacote
*       Retorno         :       nenhum
***********************************************************************************/
void TELET_enviaPacote(unsigned char tamanho){
  
  TELET_bytesRecebidos = 0;
  TELET_bytesParaEnviar = tamanho-1;
  TELET_bytesEnviados = 1;
  U1THR = TELET_bufferTX[0];  
}
/***********************************************************************************
*       Descrição       :      Envia um pacote solicitando a escrita
*                              no bloco de operação da máquina
*       Retorno         :      nenhum
*       Retorno         :      nenhum
***********************************************************************************/
unsigned char TELET_escreveBlocoOperacao(unsigned int numeroSerie,
                                         unsigned int contadorVendas,
                                         unsigned int arrecadacaoParcial,
                                         unsigned int arrecadacaoTotal,
                                         unsigned char flags,
                                         unsigned int arrecadaoCartao){/*
                                         unsigned int contadorVendasParcial,
                                         unsigned int arrecadacaoCartaoParcial,
                                         unsigned int arrecadacaoCartaoTotal,
                                         unsigned int valorPipoca,
                                         unsigned int comissaoPonto){    */                                       
 TELET_bufferRX[1] = 255;  
                                           
  TELET_bufferTX[0] = STX;
  TELET_bufferTX[1] = 25;
  TELET_bufferTX[2] = ESCREVE_BLOCO_OPERACAO;
  
  TELET_bufferTX[3] = numeroSerie>>24;
  TELET_bufferTX[4] = numeroSerie>>16;
  TELET_bufferTX[5] = numeroSerie>>8;
  TELET_bufferTX[6] = numeroSerie;
  
  TELET_bufferTX[7] = contadorVendas>>24;
  TELET_bufferTX[8] = contadorVendas>>16;
  TELET_bufferTX[9] = contadorVendas>>8;
  TELET_bufferTX[10]= contadorVendas;
  
  TELET_bufferTX[11]= arrecadacaoParcial>>24;
  TELET_bufferTX[12]= arrecadacaoParcial>>16;
  TELET_bufferTX[13]= arrecadacaoParcial>>8;
  TELET_bufferTX[14]= arrecadacaoParcial;
  
  TELET_bufferTX[15]= arrecadacaoTotal>>24;
  TELET_bufferTX[16]= arrecadacaoTotal>>16;
  TELET_bufferTX[17]= arrecadacaoTotal>>8;
  TELET_bufferTX[18]= arrecadacaoTotal;
  
  TELET_bufferTX[19] = arrecadaoCartao>>24;
  TELET_bufferTX[20] = arrecadaoCartao>>16;
  TELET_bufferTX[21] = arrecadaoCartao>>8;
  TELET_bufferTX[22] = arrecadaoCartao;
  
  TELET_bufferTX[23]= flags;
  TELET_bufferTX[24]= TELET_checksum(TELET_bufferTX,24);
  
  TELET_enviaPacote(25);
  
  TELET_silentTime = 200;
  while(TELET_silentTime) 
    vTaskDelay(1);
  
  if( TELET_bytesRecebidos == TELET_bufferRX[1]){
    
    if(TELET_bufferRX[0] == 0x02 && 
       TELET_bufferRX[1] == TELET_bytesRecebidos && 
       TELET_bytesRecebidos < TAM_BUF_RX && 
       TELET_bufferRX[TELET_bufferRX[1]-1] == TELET_checksum(TELET_bufferRX,TELET_bufferRX[1]-1)){
     
          
       return 255; 
     }        
  }    
  
  return 0;
}
/***********************************************************************************
*       Descrição       :       Faz o calculo do CRC sobre um pacote
*                               de dados
*       Parametros      :       (unsigned char*) ponteiro para o inicio do pacote
*                               (unsigned char) quantidade de bytes
*                                               significativos presentes no buffer
*       Retorno         :       (unsigned char) checksum
***********************************************************************************/
unsigned char TELET_checksum(unsigned char *pData,unsigned char tamanho){
  unsigned short int soma=256;  
  
  for(unsigned char i=0;i<tamanho;i++)
    soma += pData[i];
  
  return (unsigned char)(256 - soma);
}
/***********************************************************************************
*       Descrição       :       Faz a leitura do SSID configurado para a 
*                               rede wifi
*       Parametros      :       (char*) ssid
*       Retorno         :       (unsigned char) maior do que zero
*                                               se conseguir realizar a operação
***********************************************************************************/
unsigned char TELET_leSSID(char *ssid){              
  
  TELET_bufferRX[1] = 255;
                                           
  TELET_bufferTX[0] = STX;
  TELET_bufferTX[1] = 4;
  TELET_bufferTX[2] = LE_SSID;  
  TELET_bufferTX[3]= TELET_checksum(TELET_bufferTX,3);
  
  TELET_enviaPacote(4);
  
  TELET_silentTime = RELOAD_TIME_OUT;
  while(TELET_silentTime) //TELET_silentTime && TELET_bufferRX[1] != TELET_bytesRecebidos && timeout--)
    vTaskDelay(1);
  
  //vTaskDelay(200);
  
  if( TELET_bytesRecebidos == TELET_bufferRX[1]){
    
    if(TELET_bufferRX[0] == 0x02 && 
       TELET_bufferRX[1] == TELET_bytesRecebidos && 
       TELET_bytesRecebidos < TAM_BUF_RX && 
       TELET_bufferRX[TELET_bufferRX[1]-1] == TELET_checksum(TELET_bufferRX,TELET_bufferRX[1]-1)){
     
       memcpy(ssid,&TELET_bufferRX[3],64);
          
       return 255; 
     }        
  }    
  
  return 0;
}
/***********************************************************************************
*       Descrição       :       Faz a escrita do SSID para acesso a rede wifi
*       Parametros      :       (char*) ssid
*       Retorno         :       (unsigned char) maior do que zero
*                                               se conseguir realizar
*                                               a operação
***********************************************************************************/
unsigned char TELET_escreveSSID(char *ssid){                
  
  TELET_bufferRX[1] = 255;
                                           
  TELET_bufferTX[0] = STX;
  TELET_bufferTX[1] = 68;
  TELET_bufferTX[2] = ESCREVE_SSID;  
  memcpy(&TELET_bufferTX[3],ssid,64);
  TELET_bufferTX[67]= TELET_checksum(TELET_bufferTX,67);
  
  TELET_enviaPacote(68);
  TELET_silentTime = RELOAD_TIME_OUT;
  while(TELET_silentTime)
    vTaskDelay(1);
    
  if( TELET_bytesRecebidos == TELET_bufferRX[1]){
    
    if(TELET_bufferRX[0] == 0x02 && 
       TELET_bufferRX[1] == TELET_bytesRecebidos && 
       TELET_bytesRecebidos < TAM_BUF_RX && 
       TELET_bufferRX[TELET_bufferRX[1]-1] == TELET_checksum(TELET_bufferRX,TELET_bufferRX[1]-1)){
               
       return 255; 
     }        
  }    
  
  return 0;
}
/***********************************************************************************
*       Descrição       :       Faz a leitura da senha da rede wifi
*       Parametros      :       (char*) senha
*       Retorno         :       (unsigned char) maior do que zero
*                                               se conseguir realizar a operação
***********************************************************************************/
unsigned char TELET_leSenhaWifi(char* senha){
  
  TELET_bufferRX[1] = 255;
                                           
  TELET_bufferTX[0] = STX;
  TELET_bufferTX[1] = 4;
  TELET_bufferTX[2] = LE_SENHA_WIFI;  
  TELET_bufferTX[3]= TELET_checksum(TELET_bufferTX,3);
  
  TELET_enviaPacote(4);
  
  TELET_silentTime = RELOAD_TIME_OUT;
  while(TELET_silentTime) //TELET_silentTime && TELET_bufferRX[1] != TELET_bytesRecebidos && timeout--)
    vTaskDelay(1);
  
  //vTaskDelay(200);
  
  if( TELET_bytesRecebidos == TELET_bufferRX[1]){
    
    if(TELET_bufferRX[0] == 0x02 && 
       TELET_bufferRX[1] == TELET_bytesRecebidos && 
       TELET_bytesRecebidos < TAM_BUF_RX && 
       TELET_bufferRX[TELET_bufferRX[1]-1] == TELET_checksum(TELET_bufferRX,TELET_bufferRX[1]-1)){
     
       memcpy(senha,&TELET_bufferRX[3],64);
          
       return 255; 
     }        
  }    
  
  return 0;
}
/***********************************************************************************
*       Descrição       :       Faz a escrita da senha da rede wifi
*       Parametros      :       (char*) senha
*       Retorno         :       (unsigned char) maior do que zero 
*                                               se conseguir realizar
*                                               a operação
***********************************************************************************/
unsigned char TELET_escreveSenhaWifi(char* senha){             
  
  TELET_bufferRX[1] = 255;
                                           
  TELET_bufferTX[0] = STX;
  TELET_bufferTX[1] = 68;
  TELET_bufferTX[2] = ESCREVE_SENHA_WIFI;  
  memcpy(&TELET_bufferTX[3],senha,64);
  TELET_bufferTX[67]= TELET_checksum(TELET_bufferTX,67);
  
  TELET_enviaPacote(68);
  TELET_silentTime = RELOAD_TIME_OUT;
  while(TELET_silentTime)
    vTaskDelay(1);
    
  if( TELET_bytesRecebidos == TELET_bufferRX[1]){
    
    if(TELET_bufferRX[0] == 0x02 && 
       TELET_bufferRX[1] == TELET_bytesRecebidos && 
       TELET_bytesRecebidos < TAM_BUF_RX && 
       TELET_bufferRX[TELET_bufferRX[1]-1] == TELET_checksum(TELET_bufferRX,TELET_bufferRX[1]-1)){
               
       return 255; 
     }        
  }    
  
  return 0;
}
/***********************************************************************************
*       Fim do arquivo
***********************************************************************************/
