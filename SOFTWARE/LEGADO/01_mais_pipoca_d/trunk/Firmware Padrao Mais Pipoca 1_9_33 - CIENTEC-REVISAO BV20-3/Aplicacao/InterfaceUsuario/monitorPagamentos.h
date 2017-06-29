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
|       Arquivo            :  MonitorPagamentos.h
|       Descrição          :  Biblioteca de gerenciamento dos pagamentos
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  17/02/2015
|
|       Revisões           :  1.0.0.0
|
|
| __________________________________________________________________________________
*/

#ifndef _MONITOR_PAGAMENTOS_H_
#define _MONITOR_PAGAMENTOS_H_


typedef enum{
  BV20_INICIALIZACAO,
  BV20_MONITORA_PAGAMENTOS,
  BV20_BLOQUEADO,
  BV20_OFFLINE
}eBV20_state;

typedef struct{
  eBV20_state sinal;  
}sSINAL_PAGAMENTO;


void MP_gpioInt(void);
void MP_main(void*pPar);
float MP_getTotalDinheiro(void);
unsigned char MP_descontaValor(unsigned int valor);
void MP_devolveArrecadacao(unsigned int valor);
void MP_enviaSinal(eBV20_state sinal);
unsigned char MP_timeOutNoteiro(void);
void MP_tickTimer(void);
float MP_getMoedas(void);
void MP_setterInhibitMoedeiro(unsigned char flag);
unsigned char MP_getterMaquinaCartao(void);
void MP_setterMaquinaCartao(unsigned char flag);
void MP_zeraTotalizadoresMoeda(void);




#endif//_MONITOR_PAGAMENTOS_H_