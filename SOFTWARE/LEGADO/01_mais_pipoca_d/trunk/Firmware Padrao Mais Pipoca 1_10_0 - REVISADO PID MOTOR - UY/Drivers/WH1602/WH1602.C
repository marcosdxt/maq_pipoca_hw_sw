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
|       Arquivo            :  WH1602.C
|       Descri��o          :  Driver para o LCD wh1602 da winstar
| 
|       Autor              :  Marcos Aquino
|       Data cria��o       :
|
|       Revis�es           :  1.0.0.0
|
|
| __________________________________________________________________________________
*/

/***********************************************************************************
*		Includes
***********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "WH1602_targetWrapper.h"
#include "WH1602.H"
/***********************************************************************************
*		Constantes
***********************************************************************************/
#define ENDERECO_SEGUNDA_LINHA              0xC0
#define DELAY_CICLES                        1100

#define HD44780_delay_ms(X)                 vTaskDelay(X)
/***********************************************************************************
*		Variaveis locais
***********************************************************************************/
unsigned char controleLcd=0x00;

/***********************************************************************************
*		Fun��es locais
***********************************************************************************/
void HD44780_enviaNibble(unsigned char value);
void HD44780_enviaByte(unsigned char endereco,unsigned char value);

/***********************************************************************************
*		Implementa��o
***********************************************************************************/

/***********************************************************************************
*   Descri��o   :   Inicializa��o do m�dulo
*   Parametros  :   nenhum
*   Retorno     :   nenhum
***********************************************************************************/
void HD44780_init(unsigned char modo1,unsigned char modo2){
  unsigned char i;
  
  INIT_DIR_PINS; 
  
  HD44780_delay_ms(500);

  for(i=0;i<3;i++){
    HD44780_enviaNibble(0x03);
    HD44780_delay_ms(100);  
  }
  
  HD44780_enviaNibble(0x02);
  HD44780_delay_ms(20);      
  
  
  HD44780_enviaByte(0,0x20 | modo1);
  HD44780_delay_ms(20);        
  HD44780_enviaByte(0,0x08 | modo2);
  HD44780_delay_ms(20);        
  
  controleLcd = 0x08 | modo2;
  
  HD44780_enviaByte(0,0x10);
  HD44780_delay_ms(10);          
  HD44780_enviaByte(0,0x06);   
  HD44780_delay_ms(10);          
}
/***********************************************************************************
*    Descri��o   :   Envia um nibble para o controlador do LCD
*    Parametros  :   (unsigned char) valor que ser� escrito no display
*    Retorno     :   nenhum
***********************************************************************************/
void HD44780_enviaNibble(unsigned char value){
 
  SET_LCD_ENABLE; 
  __no_operation();
  __no_operation();
  __no_operation();
  __no_operation();
  __no_operation();
  __no_operation();  
  LCD_WRITE_NIBBLE(value);      
  __no_operation();
  __no_operation();
  __no_operation();
  __no_operation();
  __no_operation();
  __no_operation();  
                         
  CLR_LCD_ENABLE;   
}
/***********************************************************************************
*    Descri��o    :   Envia um byte para o controlador do LCD
*    Parametros   :   (unsigned char) endere�o
*                     (unsigned char) valor que ser� escrito no controlador
*    Retorno      :   nenhum
***********************************************************************************/
void HD44780_enviaByte(unsigned char endereco,unsigned char value){

  (endereco)?(SET_LCD_RS):(CLR_LCD_RS);                              
  HD44780_enviaNibble(value>>4);
  HD44780_enviaNibble(value);  
  
  HD44780_delay_ms(2);  
}
/***********************************************************************************
*    Descri��o    :   Delay utilizado dentro do m�dulo
*    Parametros   :   (unsigned int) n�mero de itera��es
*    Retorno      :   nenh8um
***********************************************************************************/
/*
void HD44780_delay_ms(unsigned int delayCyCles){
  unsigned int i,count;
  
  for(i=0;i<delayCyCles;i++)
    for(count=DELAY_CICLES;count;count--);           
}
*/
/***********************************************************************************
*     Descri��o   :   Posiciona o cursor em uma determinada linha do display   
*     Parametros  :   (unsigned char) X
*                     (unsigned char) Y
*     Retorno     :   (nenhum)
***********************************************************************************/
void HD44780_posicionaTexto(unsigned char x,unsigned char y){
    
  if(x<16 && y==0)
     HD44780_enviaByte(0,0x80 + x);
  else
    if(y==1)
      HD44780_enviaByte(0,0xC0 + x);
    
  HD44780_delay_ms(4);      
}
/***********************************************************************************
*     Descri��o    :  Escreve um caracter no display
*     Parametrs    :  (char)
*     Retorno      :  nenhum
***********************************************************************************/
void HD44780_writeChar(char c){
  
  switch(c){
    case '\f':  HD44780_enviaByte(0,1);
                HD44780_delay_ms(12);
                break;    
    case '\n':    
    case '\r':  HD44780_enviaByte(1,2);
                break;    
    default  :  HD44780_enviaByte(1,c);
                break;
  }
}
/***********************************************************************************
*     Descri��o   :   Escreve uma string no display
*     Parametros  :   (char*) Ponteiro para a string
*     Retorno     :   nenhum
***********************************************************************************/
void HD44780_writeString(char *string){
  
  while(*string)
    HD44780_writeChar(*string++);      
}
/***********************************************************************************
*     Descri��o   :   Liga o display
*     Parametros  :   (nenhum)
*     Retorno     :   (nenhum)
***********************************************************************************/
void HD44780_ligaDisplay(void){
 
  controleLcd |= 0x04;
  HD44780_enviaByte(0,controleLcd);  
}
/***********************************************************************************
*     Descri��o    :    Desliga o display
*     Parametros   :    (nenhum)
*     Retorno      :    (nenhum)
***********************************************************************************/
void HD44780_desligaDisplay(void){
 
  controleLcd &= 0xFB;
  HD44780_enviaByte(0,controleLcd);
}
/************************************************************************************
*     Descri��o     :   Liga o cursor
*     Parametros    :   nenhum
*     Retorno       :   nenhum
***********************************************************************************/
void HD44780_ligaCursor(void){
  
  controleLcd |= 0x02;
  HD44780_enviaByte(0,controleLcd);  
}
/***********************************************************************************
*     Descri��o     :   Desliga o cursor
*     Parametros    :   nenhum
*     Retorno       :   nenhum
***********************************************************************************/
void HD44780_desligaCursor(void){
  
  controleLcd &= 0xFD;
  HD44780_enviaByte(0,controleLcd);  
}
/***********************************************************************************
*     Descri��o     :   Liga o cursor piscante
*     Parametros    :   nenhum
*     Retorno       :   nenhum
***********************************************************************************/
void HD44780_ligaCursorPiscante(void){
 
  controleLcd |= 0x01;
  HD44780_enviaByte(0,controleLcd); 
}
/***********************************************************************************
*     Descri��o     :   Desliga o cursor piscante
*     Parametros    :   nenhum
*     Retorno       :   nenhum
***********************************************************************************/
void HD44780_desligaCursorPiscante(void){
 
   controleLcd &= 0xFE;
   HD44780_enviaByte(0,controleLcd);
}                                   
/***********************************************************************************
*		Fim do arquivo
***********************************************************************************/

void teste_lcd(void){
  
  HD44780_init(LCD_DISPLAY_8X5 | LCD_2_LINHAS,
               LCD_DISPLAY_LIGADO | LCD_CURSOR_DESLIGADO | LCD_CURSOR_FIXO);
  
  FIO1DIR |= (0X01)<<14;
  FIO1SET |= (0X01)<<14;

  HD44780_writeChar('\f');  

  //                   0123456789012345
  HD44780_writeString("::  Apus REP  ::");
  HD44780_posicionaTexto(0,1);
  HD44780_writeString("     v1.0       ");
  
}