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
|       Arquivo            :  PItemperatura.h
|       Descri��o          :  Controle da temperatura
| 
|       Autor              :  Marcos Aquino
|       Data cria��o       :  08/08/2015
|
|       Revis�es           :  1.0.0.0
|
|
| __________________________________________________________________________________
*/
#ifndef _PID_TEMPERATURA_H_
#define _PID_TEMPERATURA_H_



void PID_temp(void);
void PITEMP_setterPreAquecimento(unsigned short int preAquecimento);
void PITEMP_setterCiclosControle(unsigned short int ciclos);













/***********************************************************************************
*       Fim do arquivo
***********************************************************************************/
#endif// _PID_TEMPERATURA_H_