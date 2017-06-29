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
|       Arquivo            :  ControleEmbalagem.c
|       Descrição          :  Biblioteca para controle do liberador
|                             de embalagem
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  17/02/2015
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
#include "controleEmbalagem.h"
#include "..\..\includes.h"

/***********************************************************************************
*       Definições locais
***********************************************************************************/
#define MOTOR_EMBALAGEM_PIN                     (0x01)<<1
#define SENSOR_PAPEL_PIN                        (0x01)<<25
#define GET_SENSOR_PAPEL()                      (FIO0PIN&SENSOR_PAPEL_PIN)?0:1

#define INI_PINS_EMBALAGEM()                    FIO1DIR |= MOTOR_EMBALAGEM_PIN; 
#define INI_SENSOR_PIN()                        
#define SET_MOTOR_EMBALAGEM(X)                  X?(FIO1SET|=MOTOR_EMBALAGEM_PIN):(FIO1CLR|=MOTOR_EMBALAGEM_PIN)

#define THRESHOLD_INFERIOR                      2000//1600
#define THRESHOLD_SUPERIOR                      2800//2850

/***********************************************************************************
*       Variaveis locais
***********************************************************************************/
unsigned char CE_bufferAmostragem[8];
unsigned char CE_ponteiroAmostragem=0;

/***********************************************************************************
*       Funções locais
***********************************************************************************/


/***********************************************************************************
*       Implementação das funções
***********************************************************************************/

/***********************************************************************************
*       Descrição       :       Inicialização da biblioteca
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void CE_ini(void){
    
  INI_PINS_EMBALAGEM();   
}
/***********************************************************************************
*       Descrição       :       Empurra o pacote para o usuário
*       Parametros      :       (unsigned int) tempo para detectar o pacote
*       Retorno         :       (unsigned char) maior do que zero se 
*                                               conseguir empurrar o pacote
*                                               para o usuário
***********************************************************************************/
unsigned char CE_empurraEmbalagem(unsigned int tempo){
  unsigned int timeOut=tempo;
    
  SET_MOTOR_EMBALAGEM(1);  
  
  tempo = tempo;
  
  for(unsigned char i=0;i<tempo;i++){
    vTaskDelay(100);
    AL_tick();
  }
   
  timeOut=300;
  do{
    
    if(CE_sensorEmbalagem()){
      SET_MOTOR_EMBALAGEM(0);    
      return 255;
    }
    vTaskDelay(10);
    AL_tick();
  }
  while(timeOut--);
  
  SET_MOTOR_EMBALAGEM(0);
  
  return 0;
}
/***********************************************************************************
*       Descrição       :       Média móvel para o filtro do sinal do 
*                               sensor do papel
*       Parametros      :       nenhum
*       Retorno         :       (unsigned char) maior do que zero se o 
*                               sinal estiver presente em mais do que 50% do buffer
***********************************************************************************/
unsigned char CE_sensorEmbalagem(void){
  unsigned short int media = AA_mediaMovelPapel();
  
  if(media<THRESHOLD_INFERIOR)
    return 1;
  
  return 0;  
}
/***********************************************************************************
*       Descrição       :       Setter para o motor da embalagem
*       Parametros      :       (unsigned char) flag
*       Retorno         :       nenhum
***********************************************************************************/
void CE_setterMotor(unsigned char flag){
  
  SET_MOTOR_EMBALAGEM(flag);  
}
/***********************************************************************************
*       Fim do arquivo
***********************************************************************************/
