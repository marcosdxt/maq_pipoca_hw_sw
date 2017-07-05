/*__________________________________________________________________________________
|	Marcos Aquino Projetos Exclusivos
|       
|       Balneário Camboriú - SC
|       www.aquino.eng.br
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
|       Arquivo            :  mapeamento_gpios.h
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  09/02/2015
|
|       Revisões           :  1.0.0.0
|
|
| __________________________________________________________________________________
*/
#ifndef _MAPEAMENTO_GPIOS_H_
#define _MAPEAMENTO_GPIOS_H_

// Definição e macro para o pino do led de status
#define LED_STATUS_PIN                                      (0x01)<<17
#define SET_LED_STATUS(X)                                   X?(FIO1SET|=LED_STATUS_PIN):(FIO1CLR|=LED_STATUS_PIN)
#define INI_LED_STATUS_DIR()                                FIO1DIR|=LED_STATUS_PIN

// definição para os pinos do lcd
#define LCD_D4                                             (0x01)<<7
#define LCD_D5                                             (0x01)<<6
#define LCD_D6                                             (0x01)<<5
#define LCD_D7                                             (0x01)<<4
#define LCD_ENABLE                                         (0x01)<<8
#define LCD_RS                                             (0x01)<<9

#define LCD2_D4                                            (0x01)<<6
#define LCD2_D5                                            (0x01)<<7
#define LCD2_D6                                            (0x01)<<8
#define LCD2_D7                                            (0x01)<<9
#define LCD2_ENABLE                                        (0x01)<<4
#define LCD2_RS                                            (0x01)<<5

/***********************************************************************************
*       Fim do arquivo
***********************************************************************************/
#endif// _MAPEAMENTO_GPIOS_H_