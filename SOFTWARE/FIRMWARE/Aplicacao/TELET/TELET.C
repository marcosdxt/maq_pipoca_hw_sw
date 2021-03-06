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
|       Arquivo            :  TELET.C
| 
|       Autor              :  Marcos Aquino
|       Data cria��o       :  21/07/2015
|
|       Revis�es           :  1.0.0.0
|							  1.0.0.1  
|                               (06/07/2017) Corrigido envio dos flags de falha
|                                            motor/dosador e CPU OFFLINE e do
|                                            contador parcial e do cart�o parcial.
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
*       Defini��es locais
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
unsigned char TELET_estadoConexaoTelemetria = 1;
unsigned short TELET_silentTime=0;
xQueueHandle filaEstados;

/***********************************************************************************
*       Fun��es locais
***********************************************************************************/
unsigned char TELET_checksum(unsigned char *pData,unsigned char tamanho);
void TELET_ini(void);
void TELET_vetor_interrupcao(void);
void TELET_main(void*pPar);
unsigned char TELET_escreveBlocoOperacao(unsigned int numeroSerie,
                                         unsigned int contadorVendas,
                                         unsigned int arrecadacaoParcial,
                                         unsigned int arrecadacaoTotal,
                                         unsigned char flags,
                                         unsigned int arrecadaoCartao,
                                         unsigned int contadorVendasParcial,
                                         unsigned int arrecadacaoCartaoParcial,
                                         unsigned int comissaoPonto,
                                         char* versaoCPU,
                                         unsigned char md_locacao,
                                         unsigned int valorPipoca);
unsigned char TELET_leSSID(char *ssid);
unsigned char TELET_escreveSSID(char *ssid);
unsigned char TELET_leSenhaWifi(char* senha);
unsigned char TELET_escreveSenhaWifi(char* senha);
unsigned char TELET_leMACAddress(char *MAC);
unsigned int TELET_leMACTelemetria();
void TELET_enviaEstado(TELET_estados estado_atual);
unsigned char TELET_getEstadoConexaoTelemetria(void);

/***********************************************************************************
*       Implementa��o das fun��es 
***********************************************************************************
*       Descri��o       :       main do TELET
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void TELET_main(void*pPar){
  
  unsigned char flags=0;
  
  TELET_estados estadoAtualTelet = ESTADO_BLOQUEADO;
  filaEstados = xQueueCreate(1,sizeof(TELET_estados));
  TELET_ini();  
  
  for(;;){

    if(xQueueReceive(filaEstados,&estadoAtualTelet,500)){
      if(estadoAtualTelet==ESTADO_MONITORACAO){
        flags=0;
        flags |= CE_sensorEmbalagem()?0x80:0x00;
        flags |= TECLADO_getChavePorta()?0x40:0x00;
        flags |= TECLADO_getChaveReservaPapel()?0x20:0x00;
        flags |= AA_verificaConexaoTermistor()?0x10:0x20;
        flags |= MP_timeOutNoteiro()?0x08:0x00;
        flags |= IU_getFalhaMotor()?0x04:0x00;
        flags |= IU_getFalhaDosador()?0x02:0x00;

        if(TELET_escreveBlocoOperacao(PARAMETROS_carregaNumeroSerie(),
                                      PARAMETROS_leContadorVendas(),
                                      PARAMETROS_leContadorArrecadacao()*100,
                                      PARAMETROS_leTotalizadorPermanente()*100,
                                      flags,
                                      PARAMETROS_carregaFaturamentoTotalCartao()*100,
                                      PARAMETROS_leContadorVendasParcial(),
                                      PARAMETROS_carregaFaturamentoParcialCartao()*100,
                                      PARAMETROS_leComissaoPonto(),
                                      PARAMETROS_leVersaoCPU(),
                                      PARAMETROS_leFlagLocacao(),
                                      PARAMETROS_leParametro(VALOR_PIPOCA)*100)){
          TELET_estadoConexaoTelemetria = 1;
        }else{
          TELET_estadoConexaoTelemetria = 0;
        }
      }  
    }  
  }  
}
/***********************************************************************************
*       Descri��o       :       Envio do estado para a pilha
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void TELET_enviaEstado(TELET_estados estado_enviado){
  
  xQueueSend(filaEstados,&estado_enviado,500);
}
/***********************************************************************************
*       Descri��o       :       Inicializa��o do m�dulo
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void TELET_ini(void){
     
  UART_init(1,4800,NULL,NULL,NULL);  
}
/***********************************************************************************
*       Descri��o       :       Vetor de interrup��o da uart1
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
  //Apaga o flag de interrup��o pendente
  CLRPEND0 |= (0x01)<<6;    
}
/***********************************************************************************
*       Descri��o       :       Envia um pacote pelo canal serial
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
*       Descri��o       :      Envia um pacote solicitando a escrita
*                              no bloco de opera��o da m�quina
*       Retorno         :      nenhum
*       Retorno         :      nenhum
***********************************************************************************/
unsigned char TELET_escreveBlocoOperacao(unsigned int numeroSerie,
                                         unsigned int contadorVendas,
                                         unsigned int arrecadacaoParcial,
                                         unsigned int arrecadacaoTotal,
                                         unsigned char flags,
                                         unsigned int arrecadaoCartao,
                                         unsigned int contadorVendasParcial,
                                         unsigned int arrecadacaoCartaoParcial,
                                         unsigned int comissaoPonto,
                                         char* versaoCPU,
                                         unsigned char md_locacao,
                                         unsigned int valorPipoca){                                      
  
  unsigned char tamanho = strlen(versaoCPU);                                         
  
  TELET_bufferRX[1] = 255;  
                                           
  TELET_bufferTX[0] = STX;
  TELET_bufferTX[1] = 43+tamanho;
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
  
  TELET_bufferTX[23] = contadorVendasParcial>>24;
  TELET_bufferTX[24] = contadorVendasParcial>>16;
  TELET_bufferTX[25] = contadorVendasParcial>>8;
  TELET_bufferTX[26] = contadorVendasParcial;
  
  TELET_bufferTX[27] = arrecadacaoCartaoParcial>>24;
  TELET_bufferTX[28] = arrecadacaoCartaoParcial>>16;
  TELET_bufferTX[29] = arrecadacaoCartaoParcial>>8;
  TELET_bufferTX[30] = arrecadacaoCartaoParcial;
  
  TELET_bufferTX[31] = comissaoPonto>>24;
  TELET_bufferTX[32] = comissaoPonto>>16;
  TELET_bufferTX[33] = comissaoPonto>>8;
  TELET_bufferTX[34] = comissaoPonto;
  
  TELET_bufferTX[35] = valorPipoca>>24;
  TELET_bufferTX[36] = valorPipoca>>16;
  TELET_bufferTX[37] = valorPipoca>>8;
  TELET_bufferTX[38] = valorPipoca;
  
  TELET_bufferTX[39] = md_locacao;
  
  TELET_bufferTX[40] = tamanho;
  
  for(unsigned char i=0;i<tamanho;i++)
    TELET_bufferTX[41+i] = versaoCPU[i];
  
  TELET_bufferTX[41+tamanho]= flags;
  TELET_bufferTX[42+tamanho]= TELET_checksum(TELET_bufferTX,42+tamanho);
  
  TELET_enviaPacote(43+tamanho);
  
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
*       Descri��o       :       Faz o calculo do CRC sobre um pacote
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
*       Descri��o       :       Faz o envio do estado da conex�o
*       Parametros      :       Nenhum
*       Retorno         :       (unsigned char) estados
***********************************************************************************/
unsigned char TELET_getEstadoConexaoTelemetria(void){

  return TELET_estadoConexaoTelemetria;
}
/***********************************************************************************
*       Descri��o       :       Faz a leitura do SSID configurado para a 
*                               rede wifi
*       Parametros      :       (char*) ssid
*       Retorno         :       (unsigned char) maior do que zero
*                                               se conseguir realizar a opera��o
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
*       Descri��o       :       Faz a leitura do MAC da TELEMETRIA
*       Parametros      :       nenhum
*       Retorno         :       (unsigned int) MAC da Telemetria, se for zero falhou
***********************************************************************************/
unsigned int TELET_leMACTelemetria(){              
  
  TELET_bufferRX[1] = 255;
                                           
  TELET_bufferTX[0] = STX;
  TELET_bufferTX[1] = 4;
  TELET_bufferTX[2] = LE_MAC_TELEMETRIA;  
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
     
       unsigned int MAC = TELET_bufferRX[3]<<24  | TELET_bufferRX[4]<<16  | TELET_bufferRX[5]<<8  | TELET_bufferRX[6];
          
       return MAC; 
     }        
  }    
  
  return 0;
}
/***********************************************************************************
*       Descri��o       :       Faz a leitura do MAC Address
*       Parametros      :       (char) MAC
*       Retorno         :       (unsigned char) maior do que zero
*                                              se conseguir realizar a opera��o
***********************************************************************************/
unsigned char TELET_leMACAddress(char *MAC){              
  
  TELET_bufferRX[1] = 255;
                                           
  TELET_bufferTX[0] = STX;
  TELET_bufferTX[1] = 4;
  TELET_bufferTX[2] = LE_MAC_ADDRESS;  
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
     
       memcpy(MAC,&TELET_bufferRX[4],TELET_bufferRX[3]);
       return 255;
     }        
  }    
  
  return 0;
}
/***********************************************************************************
*       Descri��o       :       Faz a escrita do SSID para acesso a rede wifi
*       Parametros      :       (char*) ssid
*       Retorno         :       (unsigned char) maior do que zero
*                                               se conseguir realizar
*                                               a opera��o
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
*       Descri��o       :       Faz a leitura da senha da rede wifi
*       Parametros      :       (char*) senha
*       Retorno         :       (unsigned char) maior do que zero
*                                               se conseguir realizar a opera��o
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
*       Descri��o       :       Faz a escrita da senha da rede wifi
*       Parametros      :       (char*) senha
*       Retorno         :       (unsigned char) maior do que zero 
*                                               se conseguir realizar
*                                               a opera��o
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
