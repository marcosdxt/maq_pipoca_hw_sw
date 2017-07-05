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
|       Arquivo            :  GerenciadorEventos.h
|       Descri��o          :  Biblioteca para implementa��o da fila
|                             de eventos de venda
| 
|       Autor              :  Marcos Aquino
|       Data cria��o       :  27/04/2012
|
|       Revis�es           :  1.0.0.0
|
|
| __________________________________________________________________________________
*/
#ifndef _GERENCIADOR_EVENTOS_H_
#define _GERENCIADOR_EVENTOS_H_

typedef enum{
  MOEDA,
  DINHEIRO,
  CARTAO,
  FICHA,
}eMeioPagamento;

typedef struct{
  unsigned short int ultimo
  
}sBlocoControle;

typedef struct{
  eMeioPagamento pagamento;
  unsigned char hora,minuto,segundo,dia,mes,ano;
  unsigned short int valor;
  unsigned char crc[2];
}sEvento;



void GE_ini(void);








/***********************************************************************************
*       Fim do arquivo
***********************************************************************************/
#endif//_GERENCIADOR_EVENTOS_H_