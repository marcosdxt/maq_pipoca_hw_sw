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
|       Arquivo            :  Horimetro.h
|       Descri��o          :  Fun��es para implementa��o do bloqueio
|                             por horas trabalhadas
| 
|       Autor              :  Marcos Aquino
|       Data cria��o       :  14/07/2015
|
|       Revis�es           :  1.0.0.0
|
|
| __________________________________________________________________________________
*/

#ifndef _HORIMETRO_H_
#define _HORIMETRO_H_




typedef enum{
 LIBERADO,
 RENOVAR,
 BLOQUEADO
}eHORIMETRO;

// Verifica o saldo de horas dispon�vel para
// a opera��o da m�quina
eHORIMETRO HORIMETRO_verificaSaldoHoras(void);

// Tick para contabiliza��o das 
// horas trabalhadas
void HORIMETRO_tick(void);

// Carrega o contador de horas
unsigned int HORIMETRO_carrega(void);

// Salva o total de horas na flash
void HORIMETRO_grava(unsigned int horas);

void HORIMETRO_geraSenha(unsigned short int senhaLocal,
                         unsigned int contadorVendas,
                         unsigned char *bits);

unsigned char HORIMETRO_decodificaSenha(char* buffer,unsigned short int *senha,
                                        unsigned short int *horas);

/***********************************************************************************
*       Fim do arquivo
***********************************************************************************/
#endif//_HORIMETRO_H_