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
|       Arquivo            :  MonitorPagamentos.c
|       Descrição          :  Biblioteca de gerenciamento dos pagamentos
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  17/02/2015
|
|       Revisões           :  1.0.0.0
|
|                              15/03/2016 - CORRIGIDO BUG NO NOTEIRO QUE FAZIA
|                                           CREDITAR NOTA REJEITADA.
|                                           FALTAVA LER OS EVENTOS DE FRAUDE
|
|                              23/06/2016 - Criado configuração para 3 tipos
|                                           de moedeiro
|
| __________________________________________________________________________________
*/
/***********************************************************************************
*       Includes
***********************************************************************************/
#include <nxp\iolpc1768.h>
#include <stdio.h>
#include <stdlib.h>
#include "monitorPagamentos.h"
#include "..\..\includes.h"

/***********************************************************************************
*       Definições
***********************************************************************************/
#define RELOAD_TIMEOUT                  1000
#define SINAL_MOEDA                     (0x01<<29)

#define SINAL_MOEDA_5_CENTAVOS          (0x01)<<10
#define SINAL_MOEDA_10_CENTAVOS         (0x01)<<11
#define SINAL_MOEADA_25_CENTAVOS        (0x01)<<21
#define SINAL_MOEDA_50_CENTAVOS         (0x01)<<22
#define SINAL_MOEADA_1_REAL             (0x01)<<29        
#define SINAL_FICHA                     (0x01)<<30

#define GET_MOEDA_5_CENTAVOS()          (FIO0PIN&SINAL_MOEDA_5_CENTAVOS)?0:1
#define GET_MOEDA_10_CENTAVOS()         (FIO0PIN&SINAL_MOEDA_10_CENTAVOS)?0:1
#define GET_MOEADA_25_CENTAVOS()        (FIO0PIN&SINAL_MOEADA_25_CENTAVOS)?0:1
#define GET_MOEADA_50_CENTAVOS()        (FIO0PIN&SINAL_MOEDA_50_CENTAVOS)?0:1
#define GET_MOEADA_1_REAL()             (FIO0PIN&SINAL_MOEADA_1_REAL)?0:1
#define GET_SINAL_FICHA()               (FIO0PIN&SINAL_FICHA)?0:1

#define INHIBIT_MOEDEIRO                (0x01)<<25
#define INI_INHIBIT()                   FIO3DIR |=INHIBIT_MOEDEIRO
#define SET_INHIBIT(X)                  X?(FIO3SET|=INHIBIT_MOEDEIRO):(FIO3CLR|=INHIBIT_MOEDEIRO)

#define END_NOTEIRO                     40
/***********************************************************************************
*       Constantes
***********************************************************************************/
const unsigned char MP_valorCanais[]={
  0,2,5,10,20,50,100
};


/***********************************************************************************
*       Definições locais
***********************************************************************************/
#define FILTRO_MOEDEIRO                 5

/***********************************************************************************
*       Variaveis locais
***********************************************************************************/
xSemaphoreHandle semAcumuladorValores;
xQueueHandle filaSinais;
float MP_valorAcumulado=0;
unsigned short int MP_timeoutCounter=0;
float MP_valorMoedas=0;
unsigned char MP_sinalMaquinaCartao=0;
void(*MP_funcaoDriverModeiros)(void);

/***********************************************************************************
*       Funções locais
***********************************************************************************/
void MP_verificaFilaPagamentos(void);
unsigned char MP_inicializNoteiro(void);
void MP_bv20FSM(void);

eBV20_state MP_inicializaBV20(void);
eBV20_state MP_monitoraPagamentos(void);
eBV20_state MP_bloqueado(void);
eBV20_state MP_offLine(void);

eBV20_state(*const funcFSM[])(void)={
  MP_inicializaBV20,
  MP_monitoraPagamentos,
  MP_bloqueado,
  MP_offLine
};

unsigned char MP_realizaBloqueio(unsigned char flag);
extern void AL_bloqueioTrilogy(unsigned char flag);
void MP_gpioIntIni(void);

void MP_trataMoedeiroSOMYC(void);
void MP_trataMoedeiroICT(void);
void MP_trataMoedeiroCommestero(void);

void(*const MP_funcMoedeiros[3])(void)={
  MP_trataMoedeiroCommestero,
  MP_trataMoedeiroICT,
  MP_trataMoedeiroSOMYC
};
/***********************************************************************************
*       Implementação das funções
***********************************************************************************/

/***********************************************************************************
*       Descrição       :       thread 
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void MP_main(void*pPar){
  float temporario;
  eTIPO_NOTEIRO noteiro;
    
  INI_INHIBIT();
  vTaskDelay(2000);
  MP_funcaoDriverModeiros = MP_funcMoedeiros[PARAMETROS_carregaTipoMoedeiro()]; 
  MP_gpioIntIni();  
  vSemaphoreCreateBinary(semAcumuladorValores);
  filaSinais = xQueueCreate(3,sizeof(sSINAL_PAGAMENTO));
  
  CCTALK_ini();  
  MP_valorMoedas = 0;
  
  noteiro = PARAMETROS_leTipoNoteiro();
  
  for(;;){
    
    if(noteiro==BV20)// Só executa a máquina de estados 
      MP_bv20FSM();
    else{
      MP_timeoutCounter = 255;
    }
    
    if(MP_valorMoedas){
      while(!xSemaphoreTake(semAcumuladorValores,portTICK_RATE_MS*1));
      temporario = MP_valorMoedas;
      MP_valorAcumulado+=temporario;
      MP_valorMoedas-=temporario;
      xSemaphoreGive(semAcumuladorValores);  
    }
    
    vTaskDelay(100);
  }  
}
/***********************************************************************************
*       Descrição       :     Faz a monitoração do noteiro paralelo
*       Parametros      :     nenhum
*       Retorno         :     nenhum
***********************************************************************************/
void MP_tickTimer(void){
 

}
/***********************************************************************************
*       Descrição       :       Verifica a fila de eventos de pagamento
*                               do noteiro
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void MP_verificaFilaPagamentos(void){
  static unsigned char quantidadeAnterior=0;
  unsigned char quantidade;
  unsigned short int eventos[5];  
  unsigned char i;
  static unsigned short int valorGolpe=0;
    
    if(CCTALK_leFilaEventos(END_NOTEIRO,&quantidade,eventos)){
      MP_timeoutCounter = RELOAD_TIMEOUT;
      
      while(quantidade!=quantidadeAnterior){
        switch(eventos[0]>>8){
          case BILL_VALIDATED_OK:       
               break;
          case BILL_VALIDATED_OK_AND_SCROW:                         
              {
                  unsigned char tentativas=10;
                  unsigned char flag;
                  i = 0;
                
                  do flag = CCTALK_rotacionaValidador(END_NOTEIRO);
                  while(!flag && tentativas--);
                
                  if(flag){                                                                 
                    while(!xSemaphoreTake(semAcumuladorValores,portTICK_RATE_MS*1));                                 
                    MP_valorAcumulado += MP_valorCanais[eventos[i]&0xFF];    
                    valorGolpe = MP_valorCanais[eventos[i]&0xFF];    
                    xSemaphoreGive(semAcumuladorValores); 
                  }
                  
                  i++;
                  vTaskDelay(200);
               }
               break;            
          case MASTER_INHIBIT_ACTIVE:
          case BILL_RETURNED_FROM_ESCROW:
               break;
          case BILL_JAMMED_IN_TRANSPORT:           
          case BILL_JAMMED_IN_BACKWARDS:      
          case BILL_TAMPER:                      
          case INVALID_BILL_VALIDATION:            
          case BILL_JAMMED_IN_TRANSPORT_SAFE:            
          case OPTO_FRAUD_DETECTED:            
               {
                  while(!xSemaphoreTake(semAcumuladorValores,portTICK_RATE_MS*1));                                 
                  if(MP_valorAcumulado) 
                    MP_valorAcumulado -= valorGolpe;
                  xSemaphoreGive(semAcumuladorValores);                  
                  valorGolpe = 0;                  
               }
               break;
          case INVALID_BILL_TRANSPORTATION:
          case INHIBIT_BILL_SERIAL:
          case INHIBIT_BILL_DIPSWITCH:
          case STACKER_OK: 
          case STACKER_INVERTED:   
          case STACKER_FAULTY:     
          case STACKER_FULL:   
          case STACKER_JAMMED:
          case STRING_FRAUD_DETECTED:
          case ANTI_STRING_MECHANISM_FAULTY: 
          case BARCODE_DETECTED:
          case UNKNOWN_BILL_TYPE_STACKED:
               break;            
        }
        quantidadeAnterior++;
      }
      //quantidadeAnterior = quantidade;
    }    
}
/***********************************************************************************
*       Descrição       :       Getter para o totalizador de sinheiro
*       Parametros      :       nenhum
*       Retorno         :       (unsigned int) totalizador de dinheiro
***********************************************************************************/
float MP_getTotalDinheiro(void){
  float valor;
  
  while(!xSemaphoreTake(semAcumuladorValores,portTICK_RATE_MS*1));
  valor = MP_valorAcumulado;
  xSemaphoreGive(semAcumuladorValores); 
    
  return valor;  
}
/***********************************************************************************
*       Descrição       :       Desconta um valor do acumulador de pagamentos
*       Parametros      :       (unsigned int) valor para ser descontado
*       Retorno         :       (unsigned char) maior do que zero se
*                               conseguir descontar o valor
**********************************************************************************/
unsigned char MP_descontaValor(unsigned int valor){
  unsigned char resultado=0;
  
  while(!xSemaphoreTake(semAcumuladorValores,portTICK_RATE_MS*1));
  if(MP_valorAcumulado>3.97 && MP_valorAcumulado <4.0)
    MP_valorAcumulado = 4;
  
  if(MP_valorAcumulado>=valor){
    MP_valorAcumulado-=valor;
    resultado = 255;
  }  
  
  xSemaphoreGive(semAcumuladorValores);   
  
  return resultado;
}
/***********************************************************************************
*       Descrição       :       Devolve o dinheiro no totalizador de arrecadação
*       Parametros      :       (unsigned int) valor
*       Retorno         :       nenhum
***********************************************************************************/
void MP_devolveArrecadacao(unsigned int valor){
  
  while(!xSemaphoreTake(semAcumuladorValores,portTICK_RATE_MS*1));
  MP_valorAcumulado+=valor;
  xSemaphoreGive(semAcumuladorValores);     
}
/***********************************************************************************
*       Descrição       :       Inicialização do noteiro
*       Parametros      :       nenhum
*       Retorno         :       (unsigned char) maior do que zero
*                                               se conseguir inicializar
*                                               o noteiro
***********************************************************************************/
unsigned char MP_inicializNoteiro(void){
  unsigned char celulas[6];
  unsigned char canais=0;
  unsigned char quantidadeEventos;
  unsigned short int eventos[7];
  
  PARAMETROS_carregaTipoCedulas(&celulas[0],&celulas[1],&celulas[2],
                                &celulas[3],&celulas[4],&celulas[5]);
  
  if(celulas[0])
    canais|= 0x01;
  if(celulas[1])
    canais|= 0x02;
  if(celulas[2])
    canais|= 0x04;
  if(celulas[3])
    canais|= 0x08;
  if(celulas[4])
    canais|= 0x10;
  if(celulas[5])
    canais|= 0x20;
  
  unsigned short int tentativas=3;
  unsigned char flag;
  
  do flag = CCTALK_leFilaEventos(END_NOTEIRO,&quantidadeEventos,eventos);
  while(!flag && tentativas--);
  
  if(!flag)
    return 0;
  
  tentativas=3;
  do flag = CCTALK_setBitsHabilitacaoCanal(END_NOTEIRO,canais);
  while(!flag && tentativas--);
  
  if(!flag)
    return 0;
  
  tentativas=3;
  do flag = CCTALK_setFlagHabilitacaoGeral(END_NOTEIRO,255);
  while(!flag && tentativas--);
  
  if(!flag)
    return 0;
  
  MP_timeoutCounter = RELOAD_TIMEOUT;
  
  return 255;  
}
/***********************************************************************************
*       Descrição       :       Máquina de estados para monitoração do noteiro
*                               BV20
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void MP_bv20FSM(void){
  static eBV20_state estadoAtual = BV20_INICIALIZACAO; 
  sSINAL_PAGAMENTO recebido;
  
  if(xQueueReceive(filaSinais,&recebido,1)){
    estadoAtual = recebido.sinal;
    if(estadoAtual==BV20_BLOQUEADO){
       MP_realizaBloqueio(0);
       SET_INHIBIT(1);
       AL_bloqueioTrilogy(1);
    }
  }
  
  estadoAtual = funcFSM[estadoAtual]();  
}
/***********************************************************************************
*       Descrição       :       Estado de inicialização do noteiro
*       Parametros      :       nenhum
*       Retorno         :       (eBV20_state) próximo estado
***********************************************************************************/
eBV20_state MP_inicializaBV20(void){
  
  SET_INHIBIT(0);
  AL_bloqueioTrilogy(0);
  if(MP_inicializNoteiro())
    return BV20_MONITORA_PAGAMENTOS;
    
  return BV20_OFFLINE;
}
/***********************************************************************************
*       Descrição       :       Estado para monitoração dos pagamentos
*       Parametros      :       nenhum
*       Retorno         :       (eBV20_state) próximo estado
***********************************************************************************/
eBV20_state MP_monitoraPagamentos(void){
  static unsigned char ciclos=25;
  
  if(!ciclos){
    MP_inicializNoteiro();
    ciclos=25;
  }
  MP_verificaFilaPagamentos();
  if(!MP_timeoutCounter)
    return BV20_OFFLINE;
  
  return BV20_MONITORA_PAGAMENTOS;
}
/***********************************************************************************
*       Descrição       :       Estado onde o noteiro está bloqueado
*       Parametros      :       nenhum
*       Retorno         :       (eBV20_state) próximo estado
***********************************************************************************/
eBV20_state MP_bloqueado(void){
  
  MP_verificaFilaPagamentos();    
  return BV20_BLOQUEADO;
}
/***********************************************************************************
*       Descrição       :       Estado offline
*       Parametros      :       nenhum
*       Retorno         :       (eBV20_state)
***********************************************************************************/
eBV20_state MP_offLine(void){
  
  MP_verificaFilaPagamentos();
  if(MP_timeoutCounter)
    return BV20_INICIALIZACAO;
  
  return BV20_OFFLINE;
}
/***********************************************************************************
*       Descrição       :       Faz o bloqueio do noteiro
*       Parametros      :       nenhum
*       Retorno         :       (unsigned char) maior do que zero 
*                                               se conseguir realizar o bloqueio
***********************************************************************************/
unsigned char MP_realizaBloqueio(unsigned char flagGeral){
  unsigned int tentativas=3;
  unsigned char flag;
  
  do flag = CCTALK_setFlagHabilitacaoGeral(END_NOTEIRO,flagGeral);
  while(!flag && tentativas--);

  if(!flag)
    return 0;
  
  return 255;    
}
/***********************************************************************************
*       Descrição       :       Coloca um sinal da fila de 
*                               sinais assincronos da fila de mensagens
*                               do monitor de pagamentos
*       Parametros      :       (eBV20_state) sinal
*       Retorno         :       nenhum
***********************************************************************************/
void MP_enviaSinal(eBV20_state sinal){
  sSINAL_PAGAMENTO comando;
  
  comando.sinal = sinal;
  switch(sinal){
    case BV20_BLOQUEADO:
         SET_INHIBIT(1);
         AL_bloqueioTrilogy(1);
         break;
    case BV20_INICIALIZACAO:    
         SET_INHIBIT(0);
         AL_bloqueioTrilogy(0);
         break;
  }
  xQueueSend(filaSinais,&comando,0);
}
/***********************************************************************************
*       Descrição       :       Verifica a contagem do contador de timeout
*       Parametros      :       nenhum
*       Retorno         :       (unsigned char) maior do que zero
*                               se o contador estiver maior do que zero
***********************************************************************************/
unsigned char MP_timeOutNoteiro(void){
  
  if(MP_timeoutCounter)
    return 255;
  
  return 0;  
}
/***********************************************************************************
*       Descrição       :       Getter para o valor de moedas
*       Parametros      :       nenhum
*       Retorno         :       (float) total de moedas
***********************************************************************************/
float MP_getMoedas(void){
  
  return MP_valorMoedas;
}
/***********************************************************************************
*       Descrição       :       Setter para o inhibit do moedeiro
*       Parametros      :       (unsigned char) flag
*       Retorno         :       nenhum
***********************************************************************************/
void MP_setterInhibitMoedeiro(unsigned char flag){
   
  SET_INHIBIT(flag);
  AL_bloqueioTrilogy(flag);
}
/***********************************************************************************
*       Descrição       :       Getter para flag que indica que a máquina
*                               de cartão enviou sinal para a máquina
*       Parametros      ;       nenhum
*       Retorno         :       (unsigned char) maior do que zero se 
*                               recebeu sinal da máquina de cartão
***********************************************************************************/
unsigned char MP_getterMaquinaCartao(void){
  
  return MP_sinalMaquinaCartao;
}
/***********************************************************************************
*       Descrição       :       Setter para o flag que indica que a máquina
*                               de cartão enviou um sinal para a máquina mais pipoca
*       Parametros      :       (unsigned char) valor 
*       Retorno         :       nenhum
***********************************************************************************/
void MP_setterMaquinaCartao(unsigned char flag){

  MP_sinalMaquinaCartao = flag;
}
/***********************************************************************************
*       Descrição       :       Zera os totalizadores de moeda
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void MP_zeraTotalizadoresMoeda(void){
  
  while(!xSemaphoreTake(semAcumuladorValores,portTICK_RATE_MS*1));  
  MP_valorAcumulado = 0;
  MP_valorMoedas = 0;
  xSemaphoreGive(semAcumuladorValores);     
}
/***********************************************************************************
*       Descrição       :       Inicializa a interrupção
*                               no port 0
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void MP_gpioIntIni(void){
  
 IO0INTENR_bit.P0_21 = 1;
 IO0INTENR_bit.P0_22 = 1;  
 IO0INTENR_bit.P0_29 = 1;
 IO0INTENR_bit.P0_30 = 1;
 
 SETENA0_bit.SETENA21 = 1; 
}
/***********************************************************************************
*       Descrição       :       Interrupção no GPIO
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void MP_gpioInt(void){
  
  MP_funcaoDriverModeiros();
    
  CLRPEND0_bit.CLRPEND21 = 1;  
}
/***********************************************************************************
*       Descrição       :       Função para tratar a interrupção para o 
*                               moedeiro SOMYC
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void MP_trataMoedeiroSOMYC(void){
  
  // Canal 3
  // Canal da moeda de 0,25  
  if(IO0INTSTATR_bit.P0_21){
    IO0INTCLR_bit.P0_21 = 1;
  }  
  
  // Canal 4
  // Canal da moeda de 0,50
  if(IO0INTSTATR_bit.P0_22){
    MP_valorMoedas += 0.25;
    IO0INTCLR_bit.P0_22 = 1;
  }  
  
  // Canal 5
  // Canal da moeda de 1,00
  if(IO0INTSTATR_bit.P0_29){    
    IO0INTCLR_bit.P0_29 = 1;
  }
  
  // Canal 6
  // Canal da ficha
  if(IO0INTSTATR_bit.P0_30){      
    IO0INTCLR_bit.P0_30 = 1;
  }
}
/***********************************************************************************
*       Descrição       :       Função para o tratar a interrupção
*                               para o moedeiro ICT
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void MP_trataMoedeiroICT(void){
  
  // Canal 3
  // Canal da moeda de 0,25  
  if(IO0INTSTATR_bit.P0_21){
    MP_valorMoedas += 0.25;    
    IO0INTCLR_bit.P0_21 = 1;
  }  
  
  // Canal 4
  // Canal da moeda de 0,50
  if(IO0INTSTATR_bit.P0_22){
    MP_valorMoedas += 0.50;    
    IO0INTCLR_bit.P0_22 = 1;
  }  
  
  // Canal 5
  // Canal da moeda de 1,00
  if(IO0INTSTATR_bit.P0_29){
    MP_valorAcumulado += 1;
    IO0INTCLR_bit.P0_29 = 1;
  }
  
  // Canal 6
  // Canal da ficha
  if(IO0INTSTATR_bit.P0_30){      
    IO0INTCLR_bit.P0_30 = 1;
  }
}
/***********************************************************************************
*       Descrição       :       Função para tratar a interrupção
*                               para o moedeiro commestero
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void MP_trataMoedeiroCommestero(void){
  
  // Canal 3
  // Canal da moeda de 0,25  
  if(IO0INTSTATR_bit.P0_21){
    MP_valorMoedas += 0.25;    
    IO0INTCLR_bit.P0_21 = 1;
  }  
  
  // Canal 4
  // Canal da moeda de 0,50
  if(IO0INTSTATR_bit.P0_22){
    MP_valorMoedas += 0.5;
    IO0INTCLR_bit.P0_22 = 1;
  }  
  
  // Canal 5
  // Canal da moeda de 1,00
  if(IO0INTSTATR_bit.P0_29){
    MP_valorMoedas += 1;
    MP_valorAcumulado += 1;
    IO0INTCLR_bit.P0_29 = 1;
  }
  
  // Canal 6
  // Canal da ficha
  if(IO0INTSTATR_bit.P0_30){      
    IO0INTCLR_bit.P0_30 = 1;
  }
}
/***********************************************************************************
*       Fim do arquivo
***********************************************************************************/
