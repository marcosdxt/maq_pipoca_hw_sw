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
|       Arquivo            :  MonitorPagamentos.c
|       Descri��o          :  Biblioteca de gerenciamento dos pagamentos
| 
|       Autor              :  Marcos Aquino
|       Data cria��o       :  17/02/2015
|
|       Revis�es           :  1.0.0.0
|
|                              15/03/2016 - CORRIGIDO BUG NO NOTEIRO QUE FAZIA
|                                           CREDITAR NOTA REJEITADA.
|                                           FALTAVA LER OS EVENTOS DE FRAUDE
|
|                              23/06/2016 - Criado configura��o para 3 tipos
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

//#define _DEBUG_MENSAGEMS_
/***********************************************************************************
*       Defini��es
***********************************************************************************/
#define RELOAD_TIMEOUT                  10000
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

#ifdef _DEBUG_MENSAGEMS_
  #define DEBUG_PRINT(X)                        printf(X)                
#else
  #define DEBUG_PRINT(X)                        
#endif
/***********************************************************************************
*       Constantes
***********************************************************************************/
#ifdef _BRASIL_ 
  const unsigned char MP_valorCanais[]={
    0,2,5,10,20,50,100
  };
#endif

#ifdef _URUGUAI_
  const unsigned char MP_valorCanais[]={
    0,2,5,10,20,50,100
  };
#endif
  
#ifdef _PARAGUAI_
  const unsigned short int MP_valorCanais[]={
    0,2000,5000,10000,20,50,100
  };  
#endif
  
#ifdef _PARAGUAI_60HZ_
  const unsigned short int MP_valorCanais[]={
    0,2000,5000,10000,20,50,100
  };  
#endif 
  
#ifdef _URUGUAI_60HZ_
  const unsigned short int MP_valorCanais[]={
    0,2000,5000,10000,20,50,100
  };  
#endif 
  


/***********************************************************************************
*       Defini��es locais
***********************************************************************************/
#define FILTRO_MOEDEIRO                 5
#define FILTRO_STEP_CREDITO             50
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
unsigned short int MP_contadorBloqueio=0;
unsigned int valorPipocaFicha=0;

/***********************************************************************************
*       Fun��es locais
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

unsigned short int MP_log_debug;
unsigned char MP_decodifica_evento_cctalk(unsigned short int eventos);

/***********************************************************************************
*       Implementa��o das fun��es
***********************************************************************************/

/***********************************************************************************
*       Descri��o       :       thread 
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void MP_main(void*pPar){
  float temporario;
  eTIPO_NOTEIRO noteiro;
    
  INI_INHIBIT();
  SET_INHIBIT(0);
  AL_bloqueioTrilogy(0);  
  vTaskDelay(2000);
  MP_funcaoDriverModeiros = MP_funcMoedeiros[PARAMETROS_carregaTipoMoedeiro()]; 
  MP_gpioIntIni();  
  vSemaphoreCreateBinary(semAcumuladorValores);
  filaSinais = xQueueCreate(3,sizeof(sSINAL_PAGAMENTO));
  
  CCTALK_ini();  
  MP_valorMoedas = 0;
  
  noteiro = PARAMETROS_leTipoNoteiro();
  valorPipocaFicha = PARAMETROS_leParametro(VALOR_PIPOCA);
  for(;;){
    
    if(noteiro==BV20)// S� executa a m�quina de estados 
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
*       Descri��o       :     Faz a monitora��o do noteiro paralelo
*       Parametros      :     nenhum
*       Retorno         :     nenhum
***********************************************************************************/
void MP_tickTimer(void){
 

}
/***********************************************************************************
*       Descri��o       :       Verifica a fila de eventos de pagamento
*                               do noteiro
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void MP_verificaFilaPagamentos(void){
  static unsigned short int quantidadeAnterior=0x00;
  unsigned short int eventos[5];  
  unsigned char quantidade;
    
    if(CCTALK_leFilaEventos(END_NOTEIRO,&quantidade,eventos)){
      MP_timeoutCounter = RELOAD_TIMEOUT;      
      
      unsigned short int diferenca;
      if(quantidade>=quantidadeAnterior)
        diferenca = quantidade - quantidadeAnterior;
      else
        diferenca = (255 - quantidadeAnterior) + quantidade;
      
      if(diferenca){
        for(unsigned char i=0;i<diferenca && i<5;i++)
          MP_decodifica_evento_cctalk(eventos[i]);
      }
      
      // Atualiza a quantidade atual para a anterior
      quantidadeAnterior = quantidade;      
    }            
}
/***********************************************************************************
*       Descri��o       :       Decodifica as mensagens do cctalk
*       Parametros      :       (unsigned short int) evento
*       Retorno         :       (unsigned char) pendencia
***********************************************************************************/
unsigned char MP_decodifica_evento_cctalk(unsigned short int eventos){
  unsigned char descritor = eventos>>8;
  unsigned char parametro = eventos&0xFF;
  static unsigned short int canal_evento=0;
  
  if((descritor==BILL_VALIDATED_OK || descritor==BILL_VALIDATED_OK_AND_SCROW) && parametro){
    
    switch(descritor){
      case BILL_VALIDATED_OK:
          //ultimo_credito = 0;
          if(canal_evento!=255){
            while(!xSemaphoreTake(semAcumuladorValores,portTICK_RATE_MS*1));    
            MP_valorAcumulado += canal_evento;          
            xSemaphoreGive(semAcumuladorValores);   
          }          
          canal_evento = 255;
          break;
      case BILL_VALIDATED_OK_AND_SCROW:        
          {
            unsigned short int tentativas=60000;
            unsigned char flag;
            
            do {
              flag = CCTALK_rotacionaValidador(END_NOTEIRO);            
              if(!flag)
                vTaskDelay(100);
            }
            while(!flag && tentativas--);

            //vTaskDelay(2000);
            
            if(flag){
              canal_evento = MP_valorCanais[parametro];         
            }                        
          }
          break;   
    }//Fim do switch
  }
  else{  
    // Decodifica as 
    // mensagens referentes aos problemas
    switch(descritor){      
      case MASTER_INHIBIT_ACTIVE:
           MP_log_debug = descritor;
           break;        
      case BILL_RETURNED_FROM_ESCROW:
           MP_log_debug = descritor;
           break;             
      case BILL_TAMPER:                                          
      case BILL_JAMMED_IN_TRANSPORT_SAFE:  
           break;
      case INVALID_BILL_VALIDATION:                  
      case BILL_JAMMED_IN_TRANSPORT:           
      case BILL_JAMMED_IN_BACKWARDS:              
           break;                    
      case OPTO_FRAUD_DETECTED:     
           /*
              while(!xSemaphoreTake(semAcumuladorValores,portTICK_RATE_MS*1));    
              // Desconta o valor do �ltimo cr�dito do valor
              // acumulado
              if(MP_valorAcumulado>=ultimo_credito)
                MP_valorAcumulado-=ultimo_credito;
              else
                MP_valorAcumulado = 0;
              
              xSemaphoreGive(semAcumuladorValores);   
           */
           break;
      case INVALID_BILL_TRANSPORTATION:
           MP_log_debug = descritor;
           break;
      case INHIBIT_BILL_SERIAL:
      case INHIBIT_BILL_DIPSWITCH:
      case STACKER_OK: 
      case STACKER_INVERTED:   
      case STACKER_FAULTY:     
      case STACKER_FULL:   
      case STACKER_JAMMED:
      case STRING_FRAUD_DETECTED:
           MP_log_debug = descritor;
           break;        
      case ANTI_STRING_MECHANISM_FAULTY: 
           MP_log_debug = descritor;
           break;        
      case BARCODE_DETECTED:
           MP_log_debug = descritor;
           break;        
      case UNKNOWN_BILL_TYPE_STACKED:
           MP_log_debug = descritor;
           break;
    }
  }
        
  return 0;
}
/***********************************************************************************
*       Descri��o       :       Getter para o totalizador de sinheiro
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
*       Descri��o       :       Desconta um valor do acumulador de pagamentos
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
*       Descri��o       :       Devolve o dinheiro no totalizador de arrecada��o
*       Parametros      :       (unsigned int) valor
*       Retorno         :       nenhum
***********************************************************************************/
void MP_devolveArrecadacao(unsigned int valor){
  
  while(!xSemaphoreTake(semAcumuladorValores,portTICK_RATE_MS*1));
  MP_valorAcumulado+=valor;
  xSemaphoreGive(semAcumuladorValores);     
}
/***********************************************************************************
*       Descri��o       :       Inicializa��o do noteiro
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
*       Descri��o       :       M�quina de estados para monitora��o do noteiro
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
*       Descri��o       :       Estado de inicializa��o do noteiro
*       Parametros      :       nenhum
*       Retorno         :       (eBV20_state) pr�ximo estado
***********************************************************************************/
eBV20_state MP_inicializaBV20(void){
  
  SET_INHIBIT(0);
  AL_bloqueioTrilogy(0);
  if(MP_inicializNoteiro())
    return BV20_MONITORA_PAGAMENTOS;
    
  return BV20_OFFLINE;
}
/***********************************************************************************
*       Descri��o       :       Estado para monitora��o dos pagamentos
*       Parametros      :       nenhum
*       Retorno         :       (eBV20_state) pr�ximo estado
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
*       Descri��o       :       Estado onde o noteiro est� bloqueado
*       Parametros      :       nenhum
*       Retorno         :       (eBV20_state) pr�ximo estado
***********************************************************************************/
eBV20_state MP_bloqueado(void){
  
  MP_verificaFilaPagamentos();    
  return BV20_BLOQUEADO;
}
/***********************************************************************************
*       Descri��o       :       Estado offline
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
*       Descri��o       :       Faz o bloqueio do noteiro
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
*       Descri��o       :       Coloca um sinal da fila de 
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
         AL_setterLeds(0,0,1);
         break;
    case BV20_INICIALIZACAO:    
         SET_INHIBIT(0);
         AL_bloqueioTrilogy(0);
         break;
  }
  xQueueSend(filaSinais,&comando,0);
}
/***********************************************************************************
*       Descri��o       :       Verifica a contagem do contador de timeout
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
*       Descri��o       :       Getter para o valor de moedas
*       Parametros      :       nenhum
*       Retorno         :       (float) total de moedas
***********************************************************************************/
float MP_getMoedas(void){
  
  return MP_valorMoedas;
}
/***********************************************************************************
*       Descri��o       :       Setter para o inhibit do moedeiro
*       Parametros      :       (unsigned char) flag
*       Retorno         :       nenhum
***********************************************************************************/
void MP_setterInhibitMoedeiro(unsigned char flag){
   
  SET_INHIBIT(flag);
  AL_bloqueioTrilogy(flag);
}
/***********************************************************************************
*       Descri��o       :       Getter para flag que indica que a m�quina
*                               de cart�o enviou sinal para a m�quina
*       Parametros      ;       nenhum
*       Retorno         :       (unsigned char) maior do que zero se 
*                               recebeu sinal da m�quina de cart�o
***********************************************************************************/
unsigned char MP_getterMaquinaCartao(void){
  
  return MP_sinalMaquinaCartao;
}
/***********************************************************************************
*       Descri��o       :       Setter para o flag que indica que a m�quina
*                               de cart�o enviou um sinal para a m�quina mais pipoca
*       Parametros      :       (unsigned char) valor 
*       Retorno         :       nenhum
***********************************************************************************/
void MP_setterMaquinaCartao(unsigned char flag){

  MP_sinalMaquinaCartao = flag;
}
/***********************************************************************************
*       Descri��o       :       Zera os totalizadores de moeda
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
*       Descri��o       :       Inicializa a interrup��o
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
*       Descri��o       :       Interrup��o no GPIO
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void MP_gpioInt(void){
  
  MP_funcaoDriverModeiros();
    
  CLRPEND0_bit.CLRPEND21 = 1;  
}
/***********************************************************************************
*       Descri��o       :       Fun��o para tratar a interrup��o para o 
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
*       Descri��o       :       Fun��o para o tratar a interrup��o
*                               para o moedeiro ICT
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void MP_trataMoedeiroICT(void){

#ifdef PORTUGUES  
  // Canal 3
  // Canal da moeda de 0,25  
  if(IO0INTSTATR_bit.P0_21){
    if(!MP_contadorBloqueio){
      MP_valorMoedas += 0.25;    
    }
    MP_contadorBloqueio = FILTRO_STEP_CREDITO;
    IO0INTCLR_bit.P0_21 = 1;    
  }  
  
  // Canal 4
  // Canal da moeda de 0,50
  if(IO0INTSTATR_bit.P0_22){
    if(!MP_contadorBloqueio){
      MP_valorMoedas += 0.50;      
    }
    IO0INTCLR_bit.P0_22 = 1;    
    MP_contadorBloqueio = FILTRO_STEP_CREDITO;
  }  
  
  // Canal 5
  // Canal da moeda de 1,00
  if(IO0INTSTATR_bit.P0_29){
    if(!MP_contadorBloqueio){
      MP_valorAcumulado += 1;
      MP_sinalMaquinaCartao = 255;
    }
    IO0INTCLR_bit.P0_29 = 1;    
    MP_contadorBloqueio = FILTRO_STEP_CREDITO;
  }
  
  // Canal 6
  // Canal da ficha
  if(IO0INTSTATR_bit.P0_30){      
    IO0INTCLR_bit.P0_30 = 1;
  }
  
#endif
  
#ifdef ESPANHOL
  
  // Canal 3
  // Canal da moeda de 5 pesos
  if(IO0INTSTATR_bit.P0_21){
    if(!MP_contadorBloqueio){
      MP_valorMoedas += 5;    
    }
    MP_contadorBloqueio = FILTRO_STEP_CREDITO;
    IO0INTCLR_bit.P0_21 = 1;    
  }  
  
  // Canal 4
  // Canal da moeda de 10 pesos
  if(IO0INTSTATR_bit.P0_22){
    if(!MP_contadorBloqueio){
      MP_valorMoedas += 10;      
    }
    IO0INTCLR_bit.P0_22 = 1;    
    MP_contadorBloqueio = FILTRO_STEP_CREDITO;
  }  
  
  // Canal 5
  // Canal da moeda de 50 pesos
  if(IO0INTSTATR_bit.P0_29){
    if(!MP_contadorBloqueio){
      MP_valorAcumulado += 50;
      MP_sinalMaquinaCartao = 255;
    }
    IO0INTCLR_bit.P0_29 = 1;    
    MP_contadorBloqueio = FILTRO_STEP_CREDITO;
  }
  
  // Canal 6
  // Canal da ficha
  if(IO0INTSTATR_bit.P0_30){      
    IO0INTCLR_bit.P0_30 = 1;
  }  
  
#endif
}
/***********************************************************************************
*       Descri��o       :       Fun��o para tratar a interrup��o
*                               para o moedeiro commestero
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void MP_trataMoedeiroCommestero(void){
  
#ifdef _BRASIL_
  
  // Canal 3
  // Canal da moeda de 0,25  
  if(IO0INTSTATR_bit.P0_21){
    if(!MP_contadorBloqueio){
      MP_valorMoedas += 0.25;        
    }
    MP_contadorBloqueio = FILTRO_STEP_CREDITO;
    IO0INTCLR_bit.P0_21 = 1;
  }  
  
  // Canal 4
  // Canal da moeda de 0,50
  if(IO0INTSTATR_bit.P0_22){
    if(!MP_contadorBloqueio){
      MP_valorMoedas += 0.5;
    }
    MP_contadorBloqueio = FILTRO_STEP_CREDITO;
    IO0INTCLR_bit.P0_22 = 1;
  }  
  
  // Canal 5
  // Canal da moeda de 1,00
  if(IO0INTSTATR_bit.P0_29){
    if(!MP_contadorBloqueio){
      MP_valorAcumulado += 1;
    }
    MP_contadorBloqueio = FILTRO_STEP_CREDITO;
    IO0INTCLR_bit.P0_29 = 1;
  }
  
  // Canal 6
  // Canal da ficha
  if(IO0INTSTATR_bit.P0_30){  
    if(!MP_contadorBloqueio){
      MP_valorAcumulado+=valorPipocaFicha;  
    }
    MP_contadorBloqueio = FILTRO_STEP_CREDITO;
    IO0INTCLR_bit.P0_30 = 1;
  }
  
#endif
  
#ifdef _URUGUAI_
  
  // Canal 3
  // Canal da moeda de 5 pesos  
  if(IO0INTSTATR_bit.P0_21){
    if(!MP_contadorBloqueio){
      MP_valorMoedas += 5;        
    }
    MP_contadorBloqueio = FILTRO_STEP_CREDITO;
    IO0INTCLR_bit.P0_21 = 1;
  }  
  
  // Canal 4
  // Canal da moeda de 10 pesos
  if(IO0INTSTATR_bit.P0_22){
    if(!MP_contadorBloqueio){
      MP_valorMoedas += 10;
    }
    MP_contadorBloqueio = FILTRO_STEP_CREDITO;
    IO0INTCLR_bit.P0_22 = 1;
  }  
  
  // Canal 5
  // Canal da moeda de 40 pesos
  if(IO0INTSTATR_bit.P0_29){
    if(!MP_contadorBloqueio){
      MP_valorAcumulado += 50;
    }
    MP_contadorBloqueio = FILTRO_STEP_CREDITO;
    IO0INTCLR_bit.P0_29 = 1;
  }
  
  // Canal 6
  // Canal da ficha
  if(IO0INTSTATR_bit.P0_30){  
    if(!MP_contadorBloqueio){
      MP_valorAcumulado+=valorPipocaFicha;  
    }
    MP_contadorBloqueio = FILTRO_STEP_CREDITO;
    IO0INTCLR_bit.P0_30 = 1;
  }  
  
#endif
  
#ifdef _PARAGUAI_
  // Canal 3
  // Canal da moeda de 0,25  
  if(IO0INTSTATR_bit.P0_21){
    if(!MP_contadorBloqueio){
      MP_valorMoedas += 500;//0.25;        
    }
    MP_contadorBloqueio = FILTRO_STEP_CREDITO;
    IO0INTCLR_bit.P0_21 = 1;
  }  
  
  // Canal 4
  // Canal da moeda de 0,50
  if(IO0INTSTATR_bit.P0_22){
    if(!MP_contadorBloqueio){
      MP_valorMoedas += 1000;//0.5;
    }
    MP_contadorBloqueio = FILTRO_STEP_CREDITO;
    IO0INTCLR_bit.P0_22 = 1;
  }  
  
  // Canal 5
  // Canal da moeda de 1,00
  if(IO0INTSTATR_bit.P0_29){
    if(!MP_contadorBloqueio){
      //MP_valorAcumulado += 1;
    }
    MP_contadorBloqueio = FILTRO_STEP_CREDITO;
    IO0INTCLR_bit.P0_29 = 1;
  }
  
  // Canal 6
  // Canal da ficha
  if(IO0INTSTATR_bit.P0_30){  
    if(!MP_contadorBloqueio){
      MP_valorAcumulado+=valorPipocaFicha;  
    }
    MP_contadorBloqueio = FILTRO_STEP_CREDITO;
    IO0INTCLR_bit.P0_30 = 1;
  }

#endif  
  
#ifdef _PARAGUAI_60HZ_
  // Canal 3
  // Canal da moeda de 0,25  
  if(IO0INTSTATR_bit.P0_21){
    if(!MP_contadorBloqueio){
      MP_valorMoedas += 500;//0.25;        
    }
    MP_contadorBloqueio = FILTRO_STEP_CREDITO;
    IO0INTCLR_bit.P0_21 = 1;
  }  
  
  // Canal 4
  // Canal da moeda de 0,50
  if(IO0INTSTATR_bit.P0_22){
    if(!MP_contadorBloqueio){
      MP_valorMoedas += 1000;//0.5;
    }
    MP_contadorBloqueio = FILTRO_STEP_CREDITO;
    IO0INTCLR_bit.P0_22 = 1;
  }  
  
  // Canal 5
  // Canal da moeda de 1,00
  if(IO0INTSTATR_bit.P0_29){
    if(!MP_contadorBloqueio){
      //MP_valorAcumulado += 1;
    }
    MP_contadorBloqueio = FILTRO_STEP_CREDITO;
    IO0INTCLR_bit.P0_29 = 1;
  }
  
  // Canal 6
  // Canal da ficha
  if(IO0INTSTATR_bit.P0_30){  
    if(!MP_contadorBloqueio){
      MP_valorAcumulado+=valorPipocaFicha;  
    }
    MP_contadorBloqueio = FILTRO_STEP_CREDITO;
    IO0INTCLR_bit.P0_30 = 1;
  }

#endif    
}
/***********************************************************************************
*       Fim do arquivo
***********************************************************************************/
