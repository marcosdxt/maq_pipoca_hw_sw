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
|       Arquivo            :  Horimetro.h
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

#ifndef _HORIMETRO_H_
#define _HORIMETRO_H_




typedef enum{
 LIBERADO,
 RENOVAR,
 BLOQUEADO
}eHORIMETRO;

// Verifica o saldo de horas disponível para
// a operação da máquina
eHORIMETRO HORIMETRO_verificaSaldoHoras(void);

// Tick para contabilização das 
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