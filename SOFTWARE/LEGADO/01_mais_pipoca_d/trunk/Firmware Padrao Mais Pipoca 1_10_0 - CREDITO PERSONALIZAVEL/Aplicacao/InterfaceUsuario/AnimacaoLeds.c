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
|       Arquivo            :  AnimacaoLeds.c
|       Descrição          :  Menus de interface com o usuário
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  04/05/2015
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


/***********************************************************************************
*       Constantes
***********************************************************************************/


/***********************************************************************************
*       Variaveis globais
***********************************************************************************/
unsigned char AL_mascaraLeds;
unsigned char AL_saidaAuxiliar=0xFF;
eANI_STATE AL_estado=LEDS_FORA_SERVICO;

/***********************************************************************************
*       Funções locais
***********************************************************************************/


/***********************************************************************************
*       Implementação das funções
***********************************************************************************/

/***********************************************************************************
*       Descrição       :       Inicialização do módulo que realiza
*                               a animação dos leds do painel frontal
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void AL_tick(void){
  static unsigned char toggle=0;  
  static unsigned char subCiclo=0;
  
  switch(AL_estado){
    case LEDS_NORMAL:
         switch(subCiclo){
           case 0:
                   AL_setterLeds(LED_INSIRA_DINHEIRO,1,0);
                   AL_setterLeds(LED_PEGUE_COPO     ,0,0);
                   AL_setterLeds(LED_MONTE_O_COPO   ,0,0);
                   AL_setterLeds(LED_FORA_SERVICO   ,0,0);
                   AL_setterLeds(LED_INSIRA_COPO    ,0,1);
                   break;
           case 1:
                   AL_setterLeds(LED_INSIRA_DINHEIRO,1,0);
                   AL_setterLeds(LED_PEGUE_COPO     ,1,0);
                   AL_setterLeds(LED_MONTE_O_COPO   ,0,0);
                   AL_setterLeds(LED_FORA_SERVICO   ,0,0);                   
                   AL_setterLeds(LED_INSIRA_COPO    ,0,1);             
                   break;
           case 2:
                   AL_setterLeds(LED_INSIRA_DINHEIRO,1,0);
                   AL_setterLeds(LED_PEGUE_COPO     ,1,0);
                   AL_setterLeds(LED_MONTE_O_COPO   ,1,0);
                   AL_setterLeds(LED_FORA_SERVICO   ,0,0);                   
                   AL_setterLeds(LED_INSIRA_COPO    ,0,1);                          
                   break;
           case 3:
                   AL_setterLeds(LED_INSIRA_DINHEIRO,1,0);
                   AL_setterLeds(LED_PEGUE_COPO     ,1,0);
                   AL_setterLeds(LED_MONTE_O_COPO   ,1,0);
                   AL_setterLeds(LED_FORA_SERVICO   ,0,0);                   
                   AL_setterLeds(LED_INSIRA_COPO    ,1,1);                                       
                   break;
           case 4:
                   AL_setterLeds(LED_INSIRA_DINHEIRO,1,0);
                   AL_setterLeds(LED_PEGUE_COPO     ,1,0);
                   AL_setterLeds(LED_MONTE_O_COPO   ,1,0);
                   AL_setterLeds(LED_FORA_SERVICO   ,0,0);                   
                   AL_setterLeds(LED_INSIRA_COPO    ,0,1);                                                    
                   break;
           case 5:
                   AL_setterLeds(LED_INSIRA_DINHEIRO,1,0);
                   AL_setterLeds(LED_PEGUE_COPO     ,1,0);
                   AL_setterLeds(LED_MONTE_O_COPO   ,0,0);
                   AL_setterLeds(LED_FORA_SERVICO   ,0,0);                   
                   AL_setterLeds(LED_INSIRA_COPO    ,0,1);                                                                 
                   break;
           case 6:
                   AL_setterLeds(LED_INSIRA_DINHEIRO,1,0);
                   AL_setterLeds(LED_PEGUE_COPO     ,0,0);
                   AL_setterLeds(LED_MONTE_O_COPO   ,0,0);
                   AL_setterLeds(LED_FORA_SERVICO   ,0,0);                   
                   AL_setterLeds(LED_INSIRA_COPO    ,0,1);                                                                              
                   break;                               
           case 7:
           case 8:
           case 9:
           case 10:
           case 11:
                  toggle = ~toggle;
                  AL_setterLeds(LED_INSIRA_DINHEIRO,toggle,0);
                  AL_setterLeds(LED_PEGUE_COPO     ,0,0);
                  AL_setterLeds(LED_MONTE_O_COPO   ,0,0);
                  AL_setterLeds(LED_FORA_SERVICO   ,0,0);                   
                  AL_setterLeds(LED_INSIRA_COPO    ,0,1);                                                                                                
                  break;
         }   
         subCiclo = (1+subCiclo)%12;
         break;
    case LEDS_PRE_PROCESSO:         
         AL_setterLeds(LED_INSIRA_DINHEIRO,1,0);
         AL_setterLeds(LED_PEGUE_COPO     ,0,0);
         AL_setterLeds(LED_MONTE_O_COPO   ,0,0);
         AL_setterLeds(LED_FORA_SERVICO   ,0,0);                   
         AL_setterLeds(LED_INSIRA_COPO    ,0,1);        
         break;
    case LEDS_RETIRAR_COPO:
         toggle = ~toggle;
         AL_setterLeds(LED_INSIRA_DINHEIRO,1,0);
         AL_setterLeds(LED_PEGUE_COPO     ,toggle,0);
         AL_setterLeds(LED_MONTE_O_COPO   ,0,0);
         AL_setterLeds(LED_FORA_SERVICO   ,0,0);                   
         AL_setterLeds(LED_INSIRA_COPO    ,0,1);                                                                                                      
         break;
    case LEDS_DOBRA_COPO:
         toggle = ~toggle;
         AL_setterLeds(LED_INSIRA_DINHEIRO,1,0);
         AL_setterLeds(LED_PEGUE_COPO     ,1,0);
         AL_setterLeds(LED_MONTE_O_COPO   ,toggle,0);
         AL_setterLeds(LED_FORA_SERVICO   ,0,0);                   
         AL_setterLeds(LED_INSIRA_COPO    ,0,1);          
         break;
    case LEDS_INSERIR_COPO:
         toggle = ~toggle;
         AL_setterLeds(LED_INSIRA_DINHEIRO,1,0);
         AL_setterLeds(LED_PEGUE_COPO     ,1,0);
         AL_setterLeds(LED_MONTE_O_COPO   ,1,0);
         AL_setterLeds(LED_FORA_SERVICO   ,0,0);                   
         AL_setterLeds(LED_INSIRA_COPO    ,toggle,1);                
         break;
    case LEDS_FORA_SERVICO:
         AL_setterLeds(LED_INSIRA_DINHEIRO,0,0);
         AL_setterLeds(LED_PEGUE_COPO     ,0,0);
         AL_setterLeds(LED_MONTE_O_COPO   ,0,0);
         AL_setterLeds(LED_INSIRA_COPO    ,0,1);   
         toggle = ~toggle;
         AL_setterLeds(LED_FORA_SERVICO,toggle,1);
         break;
  }    
}
/***********************************************************************************
*       Descrição       :       Setter para o estado da animação do led
*       Parametros      :       (eANI_STATE) estado
*       Retorno         :       nenhum
***********************************************************************************/
void AL_setterEstado(eANI_STATE estado){
  
  AL_estado = estado;
}
/***********************************************************************************
*       Descrição       :       Setter para os leds do sistema
*       Parametros      :       (unsigned char) led
*                               (unsigned char) estado
*       Retorno         :       nenhum
***********************************************************************************/
void AL_setterLeds(unsigned char led,unsigned char estado,unsigned char flush){
  static unsigned char auxiliar=0;
  
  switch(led){  
    //--------------------------------------------------------------------------    
    case LED_INSIRA_DINHEIRO:
         if(estado)
           AL_mascaraLeds |= 0x08;
         else
           AL_mascaraLeds &= ~0x08;
         break;
    case LED_PEGUE_COPO:  
         if(estado)
           AL_mascaraLeds |= 0x10;
         else
           AL_mascaraLeds &= ~0x10;
         break;
    case LED_MONTE_O_COPO:
         if(estado)
           AL_mascaraLeds |= 0x20;
         else
           AL_mascaraLeds &= ~0x20;
         break;
    case LED_INSIRA_COPO:
         if(estado)
           AL_mascaraLeds |= 0x40;
         else
           AL_mascaraLeds &= ~0x40;
         break;
    case LED_FORA_SERVICO:
         if(estado)
           AL_mascaraLeds |= 0x80;
         else
           AL_mascaraLeds &= ~0x80;
         break;
    //--------------------------------------------------------------------------         
    case FITA_LED_ESQUERDA:
         if(estado)
           AL_mascaraLeds |= 0x02;
         else
           AL_mascaraLeds &= ~0x02;
         break;
    case FITA_LED_DIREITA:
         if(estado)
           AL_mascaraLeds |= 0x04;
         else
           AL_mascaraLeds &= ~0x04;
         break;
  }
  
  if(flush || auxiliar!=AL_saidaAuxiliar){
     unsigned char buffer[2];    
     
     buffer[1] = AL_mascaraLeds;
     buffer[0] = AL_saidaAuxiliar;    
     LIB74595_enviaByte(buffer);
  }
  
  auxiliar = AL_saidaAuxiliar;
}
/***********************************************************************************
*       Descrição       :       Sette para o sinal de bloqueio do noteiro
*                               trilogy
*       Parametros      :       (unsigned char) maior do que zero
*                               para bloquear o noteiro
*       Retorno         :       nenhum
***********************************************************************************/
void AL_bloqueioTrilogy(unsigned char flag){
  
  if(flag)
    AL_saidaAuxiliar = 0;
  else
    AL_saidaAuxiliar  = 255;
}
/***********************************************************************************
*       Fim do arquivo
***********************************************************************************/