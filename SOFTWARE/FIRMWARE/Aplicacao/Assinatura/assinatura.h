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
|       Arquivo            :  Assinatura.c
|       Descri��o          :  Biblioteca para controle do liberador
|                             de embalagem
| 
|       Autor              :  Marcos Aquino
|       Data cria��o       :  06/11/2015
|
|       Revis�es           :  1.0.0.0
|
|
| __________________________________________________________________________________
*/

#ifndef _ASSINATURA_H_
#define _ASSINATURA_H_

unsigned char ASSINATURA_decodifica(char *assinatura,
                                    unsigned short int *senha,
                                    unsigned int *endereco,
                                    unsigned char*presetValores,
                                    unsigned int * contadorVendasParcial,
                                    unsigned int * contadorVendasTotal,
                                    unsigned int * arrecadacaoParcial,
                                    unsigned int * arrecadacaoTotal);

unsigned char ASSINATURA_verifica(unsigned short int *endereco);

void ASSINATURA_gravaAssinatura(unsigned short int endereco,unsigned char preset,
                                unsigned int contadorParcial,unsigned int contadorTotal,
                                unsigned int arrecadacaoParcial,unsigned int arrecadacaoTotal);

#endif//_ASSINATURA_H_