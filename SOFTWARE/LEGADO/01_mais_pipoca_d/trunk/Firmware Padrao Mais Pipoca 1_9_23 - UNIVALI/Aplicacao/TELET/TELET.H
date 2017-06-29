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
|       Arquivo            :  TELET.H
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  21/07/2015
|
|       Revisões           :  1.0.0.0
|
|
| __________________________________________________________________________________
*/

#ifndef _TELET_H_
#define _TELET_H_

typedef enum{
  ESCREVE_BLOCO_OPERACAO=0x0A,
  LE_BLOCO_OPERACAO,
  ESCREVE_POSICAO_GEOGRAFICA,
  LE_POSICAO_GEOGRAFICA,
  ESCREVE_BUFFER_ABERTURA_PORTA,  
  LE_BUFFER_ABERTURA_PORTA,
  ESCREVE_SSID,
  LE_SSID,
  ESCREVE_SENHA_WIFI,
  LE_SENHA_WIFI    
}TELET_comandos;

void TELET_ini(void);
void TELET_vetor_interrupcao(void);
unsigned char TELET_escreveBlocoOperacao(unsigned int numeroSerie,
                                         unsigned int contadorVendas,
                                         unsigned int arrecadacaoParcial,
                                         unsigned int arrecadacaoTotal,
                                         unsigned char flags,
                                         unsigned int arrecadaoCartao);
unsigned char TELET_leSSID(char *ssid);
unsigned char TELET_escreveSSID(char *ssid);
unsigned char TELET_leSenhaWifi(char* senha);
unsigned char TELET_escreveSenhaWifi(char* senha);

/**********************************************************************************
*       Fim do arquivo
**********************************************************************************/
#endif //_TELET_H_
