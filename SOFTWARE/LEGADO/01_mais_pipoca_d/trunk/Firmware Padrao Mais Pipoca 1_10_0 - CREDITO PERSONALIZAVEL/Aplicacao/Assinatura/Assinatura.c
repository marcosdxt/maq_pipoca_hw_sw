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
|       Arquivo            :  Assinatura.c
|       Descrição          :  Biblioteca para controle do liberador
|                             de embalagem
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  06/11/2015
|
|       Revisões           :  1.0.0.0
|
|
| __________________________________________________________________________________
*/
/***********************************************************************************
*       Includes
***********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "..\..\includes.h"

/***********************************************************************************
*       Definições
***********************************************************************************/
#define TAM_VETOR_ASSINATURA            32768

#define IAP_LOCATION                    0x1FFF1FF1

unsigned int IAP_command[5];
unsigned int IAP_output[5];

typedef void(*IAP)(unsigned int[],unsigned int[]);
IAP iap_entry;

extern unsigned char MEMORYWRAPPER_localBuffer[4096];
#define bufferAssinatura                        MEMORYWRAPPER_localBuffer
/***********************************************************************************
*       Constantes
***********************************************************************************/

#pragma location="ConstSection1"
__root const unsigned char vetorAssinatura[TAM_VETOR_ASSINATURA]={
  1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16
};

const char digitos[16]={"0123456789ABCDEF"};

const unsigned char conversor[]={
  ['0'] = 0,
  ['1'] = 0x01,
  ['2'] = 0x02,
  ['3'] = 0x03,
  ['4'] = 0x04,
  ['5'] = 0x05,
  ['6'] = 0x06,
  ['7'] = 0x07,
  ['8'] = 0x08,
  ['9'] = 0x09,
  ['A'] = 0x0A,
  ['B'] = 0x0B,
  ['C'] = 0x0C,
  ['D'] = 0x0D,
  ['E'] = 0x0E,
  ['F'] = 0x0F      
};

const unsigned char mascaraDecodificacao[]={
            7, 3, 1, 6, 4, 2, 5, 0,
            7, 3, 1, 6, 4, 2, 5, 0,
            7, 3, 1, 6, 4, 2, 5, 0,
            7, 3, 1, 6, 4, 2, 5, 0,
            7, 3, 1, 6, 4, 2, 5, 0,
            7, 3, 1, 6, 4, 2, 5, 0,
            7, 3, 1, 6, 4, 2, 5, 0,
            7, 3, 1, 6, 4, 2, 5, 0
};

const unsigned char tabela[]={
  '0', '1', '2', '3',
  '4', '5', '6', '7', 
  '8', '9', 'A', 'B', 
  'C', 'D', 'E', 'F' 
};

unsigned short int ASSINATURA_contraSenhaLocal;
/***********************************************************************************
*       Variaveis 
***********************************************************************************/
unsigned char ASSINATURA_bufferDecode[64];

/***********************************************************************************
*       Funções
***********************************************************************************/
//#define ADR_ASSINATURA                  0x1A00
//#define FIM_ADR_ASSINATURA              0x1B00

/***********************************************************************************
*       Implementação das funções
***********************************************************************************/

/***********************************************************************************
*       Descrição       :       Descodifica a assinatura utilizada
*                               nas mensagens
*       Parametros      :       (unsigned short int*) senha
*                               (unsigned char*) preset de parâmetros
*                               (unsigned int*) contador de vendas parcial
*                               (unsigned int*) contador de vendas total
*                               (unsigned int*) arrecadacao parcial
*                               (unsigned int*) arrecadacao total
*       Retorno         :       (unsigned char) maior do que zero se a 
*                                               assinatura for válida
***********************************************************************************/
unsigned char ASSINATURA_decodifica(char *assinatura,
                                    unsigned short int *senha,
                                    unsigned int *endereco,
                                    unsigned char*presetValores,
                                    unsigned int * contadorVendasParcial,
                                    unsigned int * contadorVendasTotal,
                                    unsigned int * arrecadacaoParcial,
                                    unsigned int * arrecadacaoTotal){  
    
  for(unsigned char i=0;i<64;i++){
    ASSINATURA_bufferDecode[i] = conversor[assinatura[i]];
    ASSINATURA_bufferDecode[i] = (ASSINATURA_bufferDecode[i]^mascaraDecodificacao[i])&0x0F;
  }
    
  *senha = ASSINATURA_bufferDecode[0]<<28 | ASSINATURA_bufferDecode[1]<<24 | ASSINATURA_bufferDecode[2]<<20 |
           ASSINATURA_bufferDecode[3]<<16 | ASSINATURA_bufferDecode[4]<<12 | ASSINATURA_bufferDecode[5]<<8  |
           ASSINATURA_bufferDecode[6]<<4  | ASSINATURA_bufferDecode[7];
  
  *endereco = ASSINATURA_bufferDecode[8]<<28 | ASSINATURA_bufferDecode[9]<<24 | ASSINATURA_bufferDecode[10]<<20 |
              ASSINATURA_bufferDecode[11]<<16| ASSINATURA_bufferDecode[12]<<12| ASSINATURA_bufferDecode[13]<<8  |
              ASSINATURA_bufferDecode[14]<<4 | ASSINATURA_bufferDecode[15];
  
  unsigned short int preset = ASSINATURA_bufferDecode[16]<<12 | ASSINATURA_bufferDecode[17]<<8 | ASSINATURA_bufferDecode[18]<<4 | ASSINATURA_bufferDecode[19];
  if(preset==50500)
    *presetValores = 255;
  else
    if(preset==1020)
      *presetValores = 0;
    else
      return 0;
  
  *contadorVendasParcial = ASSINATURA_bufferDecode[20]<<28 | ASSINATURA_bufferDecode[21]<<24 | ASSINATURA_bufferDecode[22]<<20 |
                           ASSINATURA_bufferDecode[23]<<16 | ASSINATURA_bufferDecode[24]<<12 | ASSINATURA_bufferDecode[25]<<8  |
                           ASSINATURA_bufferDecode[26]<<4  | ASSINATURA_bufferDecode[27];
  
  *contadorVendasTotal = ASSINATURA_bufferDecode[28]<<28 | ASSINATURA_bufferDecode[29]<<24 | ASSINATURA_bufferDecode[30]<<20 |
                         ASSINATURA_bufferDecode[31]<<16 | ASSINATURA_bufferDecode[32]<<12 | ASSINATURA_bufferDecode[33]<<8  |
                         ASSINATURA_bufferDecode[34]<<4  | ASSINATURA_bufferDecode[35];
  
  *arrecadacaoParcial = ASSINATURA_bufferDecode[36]<<28 | ASSINATURA_bufferDecode[37]<<24 | ASSINATURA_bufferDecode[38]<<20 | 
                        ASSINATURA_bufferDecode[39]<<16 | ASSINATURA_bufferDecode[40]<<12 | ASSINATURA_bufferDecode[41]<<8  |
                        ASSINATURA_bufferDecode[42]<<4  | ASSINATURA_bufferDecode[43];
  
  *arrecadacaoTotal = ASSINATURA_bufferDecode[44]<<28 | ASSINATURA_bufferDecode[45]<<24 | ASSINATURA_bufferDecode[46]<<20 |
                      ASSINATURA_bufferDecode[47]<<16 | ASSINATURA_bufferDecode[48]<<12 | ASSINATURA_bufferDecode[49]<<8  |
                      ASSINATURA_bufferDecode[50]<<4  | ASSINATURA_bufferDecode[51];
  
  unsigned short int soma=0;
  for(unsigned char i=0;i<60;i++)
    soma+=ASSINATURA_bufferDecode[i];
  
  unsigned short int checkSum;  
  checkSum = ASSINATURA_bufferDecode[60]<<12 | ASSINATURA_bufferDecode[61]<<8 | ASSINATURA_bufferDecode[62]<<4 | ASSINATURA_bufferDecode[63];      
  
  if(soma==checkSum)
    return 0xFF;
  
  return 0x00;
}
/***********************************************************************************
*       Descrição       :       Faz a verificação se o módulo já esta
*                               assinado
*       Parametros      :       nenhum
*       Retorno         :       (unsigned char) maior do que zero
*                                              se o módulo estiver assinado
***********************************************************************************/
unsigned char ASSINATURA_verifica(unsigned short int *endereco){                                  
  unsigned char *ptr;
  
  ptr = (unsigned char*)0x78000;
  
  for(unsigned char i=0;i<22;i++)
    bufferAssinatura[i] = *ptr++;
  
  iap_entry = (IAP)IAP_LOCATION;  
  IAP_command[0] = 58;
  iap_entry(IAP_command,IAP_output);  
  
  unsigned short int crc =  CCTALK_calculaCRC(bufferAssinatura,0,20);
      
  
  for(unsigned char i=0;i<16;i++)
    bufferAssinatura[i+4]^= (0xC1+i);
  
  
  if( (IAP_output[1] == (bufferAssinatura[4]<<24  | bufferAssinatura[5]<<16  | bufferAssinatura[6]<<8  | bufferAssinatura[7]))  &&
      (IAP_output[2] == (bufferAssinatura[8]<<24  | bufferAssinatura[9]<<16  | bufferAssinatura[10]<<8 | bufferAssinatura[11])) &&
      (IAP_output[3] == (bufferAssinatura[12]<<24 | bufferAssinatura[13]<<16 | bufferAssinatura[14]<<8 | bufferAssinatura[15])) &&
      (IAP_output[4] == (bufferAssinatura[16]<<24 | bufferAssinatura[17]<<16 | bufferAssinatura[18]<<8 | bufferAssinatura[19])) ){
          
      if( (bufferAssinatura[20]<<8 | bufferAssinatura[21]) == crc){        
        *endereco = bufferAssinatura[0]<<8 | bufferAssinatura[1];
        return 0xFF;
      }         
  }
  
  return 0;    
}
/***********************************************************************************
*       Descrição       :       Grava os valores da assinatura na área 
*                               da flash que é designada para a assinatura
*                               da placa
*       Parametros      :       (unsigned short int) endereço
*                               (unsigned char) preset
*                               (unsigned int) contador de venda parcial
*                               (unsigned int) contador de venda total
*                               (unsigned int) arrecadacao parcial
*                               (unsigned int) arrecadacao total
*       Retorno         :       nenhum
***********************************************************************************/
void ASSINATURA_gravaAssinatura(unsigned short int endereco,unsigned char preset,
                                unsigned int contadorParcial,unsigned int contadorTotal,
                                unsigned int arrecadacaoParcial,unsigned int arrecadacaoTotal){                                                                                              
  
  iap_entry = (IAP)IAP_LOCATION;  
  
  IAP_command[0] = 50;
  IAP_command[1] = 0x1D;
  IAP_command[2] = 0x1D;  
  iap_entry(IAP_command,IAP_output);
  
  IAP_command[0] = 52;
  IAP_command[1] = 0x1D;
  IAP_command[2] = 0x1D;
  IAP_command[3] = 96000;  
  iap_entry(IAP_command,IAP_output);
  
  IAP_command[0] = 58;
  iap_entry(IAP_command,IAP_output);  
  
  bufferAssinatura[0] = endereco>>8;
  bufferAssinatura[1] = endereco;
  bufferAssinatura[2] = bufferAssinatura[0]^0xFe;
  bufferAssinatura[3] = bufferAssinatura[1]^0xFe;  
  
  //Identificação do chip
  bufferAssinatura[4] = ((IAP_output[1]>>24)&0xFF);//^0xCa;
  bufferAssinatura[5] = ((IAP_output[1]>>16)&0xFF);//^0xCa;
  bufferAssinatura[6] = ((IAP_output[1]>>8) &0xFF);//^0xCa;
  bufferAssinatura[7] = ((IAP_output[1])    &0xFF);//^0xCa;
  
  bufferAssinatura[8] = ((IAP_output[2]>>24)&0xFF);//^0xCa;
  bufferAssinatura[9] = ((IAP_output[2]>>16)&0xFF);//^0xCa;
  bufferAssinatura[10]= ((IAP_output[2]>>8) &0xFF);//^0xCa;
  bufferAssinatura[11]= ((IAP_output[2])    &0xFF);//^0xCa;
  
  bufferAssinatura[12]= ((IAP_output[3]>>24)&0xFF);//^0xCa;
  bufferAssinatura[13]= ((IAP_output[3]>>16)&0xFF);//^0xCa;
  bufferAssinatura[14]= ((IAP_output[3]>>8) &0xFF);///^0xCa;
  bufferAssinatura[15]= ((IAP_output[3])    &0xFF);//^0xCa;
  
  bufferAssinatura[16]= ((IAP_output[4]>>24)&0xFF);//^0xCa;
  bufferAssinatura[17]= ((IAP_output[4]>>16)&0xFF);//^0xCa;
  bufferAssinatura[18]= ((IAP_output[4]>>8) &0xFF);//^0xCa;
  bufferAssinatura[19]= ((IAP_output[4])    &0xFF);//^0xCa;
  
  for(unsigned char i=0;i<16;i++)
    bufferAssinatura[4+i]^=(0xC1+i);
  
  unsigned short int crc = CCTALK_calculaCRC(bufferAssinatura,0,20);
  
  bufferAssinatura[20] = crc>>8;
  bufferAssinatura[21] = crc;
  
  IAP_command[0] = 50;
  IAP_command[1] = 0x1D;
  IAP_command[2] = 0x1D;  
  iap_entry(IAP_command,IAP_output);  
                              
  vTaskDelay(5);
  
  IAP_command[0] = 51;
  IAP_command[1] = 0x78000;
  IAP_command[2] = (unsigned int)bufferAssinatura;
  IAP_command[3] = 256;
  IAP_command[4] = 96000;
  iap_entry(IAP_command,IAP_output);
  
  vTaskDelay(5);
  
  if(preset){
    
    PARAMETROS_salvaContadorVendas(contadorTotal);
          
    PARAMETROS_gravaContadorVendasParcial(contadorParcial);    
    
    PARAMETROS_salvaContadorArrecadacao(arrecadacaoParcial);
        
    PARAMETROS_gravaTotalizadorPermanente(arrecadacaoTotal);               
  }
}
/***********************************************************************************
*       Fim do arquivo
***********************************************************************************/
