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
|       Arquivo            :  Horimetro.c
|       Descrição          :  Funções para implementação do bloqueio
|                             por horas trabalhadas
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  14/07/2015
|
|       Revisões           :  1.0.0.0
|
|
| __________________________________________________________________________________
*/

/***********************************************************************************
*       Includes
***********************************************************************************/
#include "..\..\includes.h"
#include "horimetro.h"

/***********************************************************************************
*       Definições
***********************************************************************************/
#define LIMITE_AVISO                            6000
#define LIMITE_MAX_HORAS                        90000
#define CONTADOR_HORIMETOR                      60000

/***********************************************************************************
*      Constantes
***********************************************************************************/


/***********************************************************************************
*       Estruturas
***********************************************************************************/
typedef struct{
  unsigned int horas;
  unsigned short int crc;  
}sHorimetro;

typedef union{
  sHorimetro bytes;
  unsigned char bits[6];
}uHorimetro;


const unsigned int decodificaHorimetro[]={
  ['0'] = 0,
  ['1'] = 1,
  ['2'] = 2,
  ['3'] = 3,
  ['4'] = 4,
  ['5'] = 5,
  ['6'] = 6,
  ['7'] = 7,
  ['8'] = 8,
  ['9'] = 9,
  ['A'] = 10,
  ['B'] = 11,
  ['C'] = 12,
  ['D'] = 13,
  ['E'] = 14,
  ['F'] = 15  
};
/***********************************************************************************
*       Variaveis locais
***********************************************************************************/
unsigned int HORIMETRO_tickCounter=1;

/***********************************************************************************
*       Funções locais
***********************************************************************************/


/***********************************************************************************
*       Implementação das funções
***********************************************************************************/

/***********************************************************************************
*       Descrição       :       Verifica o total de horas que ainda estão
*                               liberadas para uma determinada máquina
*       Parametros      :       nenhum
*       Retorno         :       (unsigned int) horas liberadas
***********************************************************************************/
eHORIMETRO HORIMETRO_verificaSaldoHoras(void){
  unsigned int totalizador;

  totalizador = HORIMETRO_carrega();  
  
  if(totalizador>LIMITE_AVISO)
    return LIBERADO;
  else
    if(totalizador && totalizador<LIMITE_AVISO)
      return RENOVAR;          
  
  return BLOQUEADO;
}
/***********************************************************************************
*       Descrição       :       Tick que faz o desconto das horas trabalhadas
*                               pela máquina
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void HORIMETRO_tick(void){
  unsigned int totalizador;
  
  if(HORIMETRO_tickCounter)
    return;
  
  HORIMETRO_tickCounter = CONTADOR_HORIMETOR;

  totalizador = HORIMETRO_carrega();
  
  if(totalizador)
    totalizador--;
  
  HORIMETRO_grava(totalizador);
}
/***********************************************************************************
*       Descrição       :       Carrega o horimetro para a memória
*       Parametros      :       nenhum
*       Retorno         :       (unsigned int) horimetro
***********************************************************************************/
unsigned int HORIMETRO_carrega(void){
  unsigned char buffer[6];
  unsigned short int crc;
  
  MEMORYWRAPPER_readBytes(AREA_HORIMETRO,buffer,6);
  crc = CCTALK_calculaCRC(buffer,0,4);
  
  if(crc==(buffer[4]<<8 | buffer[5])){
    
    crc = buffer[0]<<24 | buffer[1]<<16 | buffer[2]<<8 | buffer[3];        
    return crc;
  }
  
  return 0x00; 
}
/***********************************************************************************
*       Descrição       :       Salva o horímetro na memória
*       Parametros      :       (unsigned int) total de horas
*       Retorno         :       void
***********************************************************************************/
void HORIMETRO_grava(unsigned int horas){
  unsigned short int crc;
  unsigned char buffer[6];
  
  buffer[0] = horas>>24;
  buffer[1] = horas>>16;
  buffer[2] = horas>>8;
  buffer[3] = horas;
  
  crc = CCTALK_calculaCRC(buffer,0,4);
  buffer[4] = crc>>8;
  buffer[5] = crc;
  
  MEMORYWRAPPER_writeBytes(AREA_HORIMETRO,buffer,6);  
}
/***********************************************************************************
*       Descrição       :       Gera a contrasenha para o horímetro
*       Parametros      :       (unsigned short int) senha local
*                               (unsigned int) contador de vendas
***********************************************************************************/
void HORIMETRO_geraSenha(unsigned short int senhaLocal,
                         unsigned int contadorVendas,
                         unsigned char *bits){
                           
  bits[0]  = senhaLocal+5;
  bits[1]  = senhaLocal>>8;
  bits[2]  = senhaLocal>>4;
  bits[3]  = senhaLocal;
  
  bits[4]  = contadorVendas>>28;
  bits[5]  = contadorVendas>>24;
  bits[6]  = contadorVendas>>20;
  bits[7]  = contadorVendas>>16;
  bits[8]  = contadorVendas>>12;
  bits[9]  = contadorVendas>>8;
  bits[10] = contadorVendas>>4;
  bits[11] = contadorVendas;
  
  bits[12] = 0;
  for(unsigned char i=0;i<12;i++)
    bits[12] ^= bits[i];
                           
  for(unsigned char i=0;i<13;i++){   
    bits[i]^= (0x0A+i);    
    bits[i]&=  0x0F;
  }  
}
/***********************************************************************************
*       Descrição       :       Converte os caracteres digitados
*                               para o valor em binário
*       Parametros      :       (char*) string padrão c contendo os valores
*       Retorno         :       (unsigned short int) valor que foi digitado
*                                                    pelo usuário em decimal
***********************************************************************************/
unsigned char HORIMETRO_decodificaSenha(char* buffer,unsigned short int *senha,
                                        unsigned short int *horas){
                                            
  for(unsigned char i=0;i<7;i++){
    buffer[i]  = decodificaHorimetro[buffer[i]];   
    buffer[i] ^= (0x0B+i);
    buffer[i] &= 0x0F;
  }
  
  *senha = (buffer[1]<<8 | buffer[2]<<4 | buffer[3]);
  *horas = (buffer[4]<<4 | buffer[5])*100;
  
  unsigned char soma=0;
  for(unsigned char i=0;i<6;i++)
    soma^= buffer[i];
  
  if(soma==buffer[6])
    return 255;

  return 0;
}
/***********************************************************************************
*       Fim do arquivo
***********************************************************************************/
