/*__________________________________________________________________________________
|	Marcos Aquino Projetos Exclusivos
|       
|       Balne�rio Cambori� - SC
|       www.aquino.eng.br
| __________________________________________________________________________________
|
|       This source code was developed by Chave Digital and cannot be copied, in part 
|       or in whole, or used, except when legally licensed by Chave Digital
|       or its distributors.
|
|       Este c�digo � propriedade da Chave Digital e n�o pode ser copiado, em parte 
|       ou em todo, ou utilizado, exceto quando for legalmente licenciado pela 
|       Chave Digital ou por um de seus distribuidores.
| __________________________________________________________________________________
|
|       Arquivo            :  AnimacaoLeds.h
|       Descri��o          :  Menus de interface com o usu�rio
| 
|       Autor              :  Marcos Aquino
|       Data cria��o       :  04/05/2015
|
|       Revis�es           :  1.0.0.0
|
|
| __________________________________________________________________________________
*/
#ifndef _ANIMACAO_LEDS_H_
#define _ANIMACAO_LEDS_H_

typedef enum {
  LEDS_NORMAL,
  LEDS_PRE_PROCESSO,
  LEDS_RETIRAR_COPO,
  LEDS_DOBRA_COPO,
  LEDS_INSERIR_COPO,
  LEDS_FORA_SERVICO
}eANI_STATE;

#define LED_INSIRA_DINHEIRO                                     1
#define LED_PEGUE_COPO                                          2
#define LED_MONTE_O_COPO                                        3
#define LED_INSIRA_COPO                                         4
#define LED_FORA_SERVICO                                        5
#define FITA_LED_ESQUERDA                                       6
#define FITA_LED_DIREITA                                        7

void AL_tick(void);
void AL_setterEstado(eANI_STATE estado);
void AL_setterLeds(unsigned char led,unsigned char estado,unsigned char flush);
void AL_bloqueioTrilogy(unsigned char flag);





/************************************************************************************
*       Fim do arquivo
************************************************************************************/
#endif//_ANIMACAO_LEDS_H_