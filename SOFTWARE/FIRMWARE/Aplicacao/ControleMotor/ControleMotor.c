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
|       Arquivo            :  ControleMotor.c
|       Descrição          :  Biblioteca para controle da velocidade do
|                             motor universal
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  14/02/2015
|                            
|       Revisões           :  1.0.0.0
|                             1.1(01/05/2015) alterado para controle proporcional da velocidade do motor
|                             1.2(02/05/2015) alterado para rampa de temperatura no controle 
|                                             da resistência.
|                             1.3(20/05/2017) Para disparar o gate do TRIAC era gerado um 
|                                             pulso, para diminuir o overhead causado por
|                                             essa interrupção o desligamento do sinal 
|                                             foi movido para o cruzamento por zero.
|
| __________________________________________________________________________________
*/

/***********************************************************************************
*       Includes
***********************************************************************************/
#include <nxp\iolpc1768.h>
#include "controleMotor.h"
#include "..\..\includes.h"
#include "PIDtemperatura.h"


//#define FQ_REDE_60_HZ
/***********************************************************************************
*       Definições
***********************************************************************************/
#ifdef FQ_REDE_60_HZ
  #define ATRASO_MAXIMO                           2500
  #define ATRASO_MINIMO                           150
#endif

#ifdef FQ_REDE_50_HZ
  #define ATRASO_MAXIMO                           3100
  #define ATRASO_MINIMO                           300
#endif


/***********************************************************************************
*       Ganhos do PID
***********************************************************************************/
#ifdef FQ_REDE_60_HZ
  #define KP                      10.0
  #define KI                      1.0
  #define KD                      1.0
#else
  #define KP                      10.0
  #define KI                      1.0
  #define KD                      1.0
#endif


/***********************************************************************************
*       Variaveis locais
***********************************************************************************/
unsigned short int MU_ciclos=0;
unsigned short int MU_frequencia;
unsigned char MU_ligaSSR=0;
unsigned char contadorSSR=0;
unsigned int periodoRotacaoMotor=0xFFFF;
unsigned char ligarResistencia;
unsigned int bufferRPM[TAMANHO_BUFFER_RPM]={0};
unsigned int CM_periodoCapturadoMotor;
unsigned short int CM_timeOutCounter=RELOAD_TIMEOUT_RRPM;
unsigned long int CM_temp;
unsigned char CM_ligaMotor=0;
unsigned short int CM_bufferRotacao[TAM_BUF_VELOCIDADE]={0};
unsigned char CM_cntCtrlMotor=CICLOS_INICIO_MOTOR;
unsigned int CM_atrasoGateMotor=500;
unsigned short int CM_set_point=0;
extern unsigned int BOARD_lock_timer;
unsigned int kp_const = 0;
unsigned int kd_const = 0;
unsigned int ki_const = 0;
unsigned short int CM_correcao_erro = 0;

/***********************************************************************************
*       Funções locais
***********************************************************************************/
unsigned int MU_calculaAtrasoGate(int rotacao_rpm);


/***********************************************************************************
*       Implementação das funções
***********************************************************************************/

/***********************************************************************************
*       Descrição       :       Inicialização da biblioteca de controle
*                               do motor universal
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void MU_ini(void){
  
  INI_PINS;

  //--------------------------------------------------------------------------//
  //                        Habilita a interrupção                            //
  //                        no NVIC                                           //
  //--------------------------------------------------------------------------//
  
  PCONP_bit.PCTIM1 = 1;  
  IP0_bit.PRI_2 = 1;  
  SETENA0_bit.SETENA2 = 1; // Habilitação da interrupção do timer 1
  
  T1TCR_bit.CE = 1;   // Contador Habilitado
  T1TCR_bit.CR = 1;   // Realiza o reset
  T1TCR_bit.CR = 0;   // do contador
  
  T1CTCR_bit.CTM = 0x00;
  T1PR = PR_TIMER;
   
  PINSEL3_bit.P1_19 = 3;
  
  T1CCR_bit.CAP1RE = 1; // Borda de subida
  T1CCR_bit.CAP1FE = 0; // Borda de descida  
  T1CCR_bit.CAP1I = 1;  // Interrupção por evento no canal  
  //-------------------------------------------------------------------------//
  //                    Inicialização do timer 2                             //
  //-------------------------------------------------------------------------//
  PCONP_bit.PCTIM2 = 1; // Energiza o periférico
  PCLKSEL1_bit.PCLK_TIMER2 = 1; // Seleção do clock
  
  IP0_bit.PRI_3 = 1;  
  SETENA0_bit.SETENA3 = 1; // Habilitação da interrupção do timer 1
  
  T2TCR_bit.CE = 1;   // Contador Habilitado
  T2TCR_bit.CR = 1;   // Realiza o reset
  T2TCR_bit.CR = 0;   // do contador
  
  T2CTCR_bit.CTM = 0x00;
  T2PR = PR_TIMER;  
  
  //-------------------------------------------------------------------------//
  //                   Match 0 para o dimmer do motor                        //
  //-------------------------------------------------------------------------//  
  T2MCR_bit.MR0I = 0;
  T2MCR_bit.MR0R = 0;
  T2EMR_bit.EM0  = 0; // Não altera nenhum pino    
  T2EMR_bit.EMC0 = 0;
  
  T2MR0 = T2TC+1000;
  //-------------------------------------------------------------------------//
  //               Math 1 para o dimmer da resistência                       //
  //-------------------------------------------------------------------------//
  T2MCR_bit.MR1I = 0;
  T2MCR_bit.MR1R = 0;
  T2EMR_bit.EM1  = 0; // Não altera nenhum pino    
  T2EMR_bit.EMC1 = 0;
  
  T2MR1 = T2TC+1000;    
  //-------------------------------------------------------------------------//
  //              Fim da inicialização do match                              //
  //-------------------------------------------------------------------------//  
  PINSEL4_bit.P2_11 = 1;
  EXTMODE_bit.EXTMODE1 = 1;
  EXTPOLAR_bit.EXTPOLAR1 = 1;    
  SETENA0_bit.SETENA19 = 1;   
      
  
  IP4_bit.PRI_19 = 0; 
  
  CM_correcao_erro = PARAMETROS_le_correcao_erro();
  kp_const = (unsigned int)( (PARAMETROS_le_ganho_KP()*(KP/100))*256);
  ki_const = (unsigned int)(((PARAMETROS_le_ganho_KI()*(KI-0.01)/100)+0.01)*32768);
  kd_const = (unsigned int)(((PARAMETROS_le_ganho_KD()*(KD-0.01)/100)+0.01)*32768);
}
/***********************************************************************************
*       Descrição       :       Interrupção do timer 2
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void MU_mathTimer2(void){
  
	  if(T2IR_bit.MR0INT){
		SET_SSR(1); // Liga o gate
		T2IR_bit.MR0INT = 1;
		CLRPEND0_bit.CLRPEND3 = 1;
	  }  
}
/***********************************************************************************
*       Descrição       :       Soma satura de 32 bits
*       Parametros      :       (int) valor 1
*                               (int) valor2
*       Retorno         :       (int) soma satura dos dois valores
***********************************************************************************/
int sadd(int a, int b){
  int sum = a+b;
  int overflow = ((a^sum)&(b^sum))>>31;
  return (overflow<<31)^(sum>>overflow); 
}
/***********************************************************************************
*       Descrição       :       Calcula o erro integral 
*       Parametros      :       (int) erro atual
*       Retorno         :       (int) erro integral
***********************************************************************************/
#pragma inline
int MU_calcula_integral_erro(int erro,unsigned flush){
  static long int y=0;
      
  if(flush){    
    y = 0;
    return 0;
  }
  
  y+=erro;
  /*
  if(erro>0)
    if(y <  360000000)
      y+= erro;
        
  if(erro<0)
    if(y > -360000000)
      y+= erro;
  */
  return y;
}
/***********************************************************************************
*       Descrição       :       Calcula a derivada do erro
*       Parametros      :       (int) erro
*       Retorno         :       (int) derivada do erro
***********************************************************************************/
#pragma inline
int CM_calcula_derivada_erro(int erro){
  static int ultimo_erro = 0;
  
  erro = erro - ultimo_erro;   
  
  if(erro>18000)
    erro = 18000;
  
  if(erro<-18000)
    erro = -18000;
  
  ultimo_erro = erro;  
  
  //Trunca os limites  
  return erro;
}
/***********************************************************************************
*       Descrição       :       Função que faz o controle da velocidade do 
*                               motor dentro do timer
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
#pragma inline
void MU_controleVelocidade(void){
  long long int erro;
  long long int erro_i;
  long long int erro_d;
  static unsigned short int referencia_rampa=0;
  static unsigned short int ciclos=180;
  
  if(CM_ligaMotor){
      
        //----------------------------------------
        // Faz a referência crescer lentamente
        //----------------------------------------
        if(referencia_rampa!=CM_set_point){
          if(referencia_rampa>CM_set_point)
            referencia_rampa-=25;//50;
          else
            referencia_rampa+=25;//50;
        }      
        if(ciclos){
          // Nos primeiros ciclos trabalha apenas com o erro
          // proporcional
          if(CM_correcao_erro){
            ciclos--;
            erro = referencia_rampa - MU_getRPMmedido();
            erro*= kp_const;
            erro>>= 8;   
            CM_calcula_derivada_erro(referencia_rampa);
            
            SET_ATRASO(MU_calculaAtrasoGate(erro));
          }
          else{
            SET_ATRASO(MU_calculaAtrasoGate(referencia_rampa));
          }
        }
        else{
          if(CM_correcao_erro){
            // Calcula o erro entre a referência e a rotação medida pelo sensor
            erro = referencia_rampa - MU_getRPMmedido();
            // Calcula a integral do erro
            erro_i = MU_calcula_integral_erro(erro,0);                     
            erro_i *= ki_const;
            erro_i >>= 15;
            
            erro_d = CM_calcula_derivada_erro(erro);
            erro_d *= kd_const;
            erro_d >>= 15;
            
            erro*= kp_const;
            erro>>= 8;          
            erro += erro_i;
            erro += erro_d;
            
            SET_ATRASO(MU_calculaAtrasoGate(erro));
          }
          else{
            SET_ATRASO(MU_calculaAtrasoGate(referencia_rampa));
          }
        }
  }
  else{
    MU_calcula_integral_erro(0,1);
    referencia_rampa = ATRASO_MAXIMO;
    ciclos = 128;
  }  

}
/***********************************************************************************
*       Descrição       :       Interrupção do EINT1
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void MU_eintInterrupt(void){  
  
  // Evita dois disparos
  // pelo glitch de subida
  // do sinal de 60 Hz
  if(!BOARD_lock_timer){
    
    SET_SSR(0);
    BOARD_lock_timer = 6;
    MU_controleVelocidade();
  
    MU_ciclos++;  
    PID_temp();      
  }
  else{
    BOARD_lock_timer = BOARD_lock_timer;    
  }
  EXTINT_bit.EINT1 = 1;
  CLRPEND0_bit.CLRPEND19 = 1;
}
/***********************************************************************************
*       Descrição       :       Tick de timer para a biblioteca de controle
*                               da velocidade do motor
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void MU_timerTick(void){  
  static unsigned short int contador=1;  
  
  if(!--contador){
    MU_frequencia = MU_ciclos;
    MU_ciclos = 0;
    contador = 500;
  }    
  
  // Timeout na captura
  // da rotação do motor
  if(CM_timeOutCounter){
    if(CM_timeOutCounter==1){
      for(unsigned char i=0;i<TAM_BUF_VELOCIDADE;i++)
        CM_bufferRotacao[i] = 0;
    }
    CM_timeOutCounter--;
  }    
  
}
/***********************************************************************************
*       Descrição       :       Getter para a frequência da rede elétrica
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
unsigned short int MU_getFrequenciaRede(void){
  
  return MU_frequencia;
}
/***********************************************************************************
*       Descrição       :       Interrupção do capture
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void MU_captureInterrupt(void){
  static unsigned int ultimaLeitura=0;
  unsigned int larguraAtual;
  unsigned int tmp;
  static unsigned int ponteiroLeitura=0;
  
  larguraAtual = tmp = T1CR1;
  
  if(larguraAtual>ultimaLeitura)         
    larguraAtual-=ultimaLeitura;    
  else
    larguraAtual = (0xFFFFFFFF - ultimaLeitura) + larguraAtual;
    
  ultimaLeitura = tmp;
  
  T1IR_bit.CR1INT = 1;
  CLRPEND0_bit.CLRPEND2 = 1;
  
  CM_periodoCapturadoMotor = larguraAtual;
  CM_timeOutCounter=RELOAD_TIMEOUT_RRPM;      
  CM_bufferRotacao[ponteiroLeitura] = CM_periodoCapturadoMotor;
  ponteiroLeitura= (ponteiroLeitura+1) %TAM_BUF_VELOCIDADE;   
}
/***********************************************************************************
*       Descrição       :       Setter para a rotação do motor
*       Parametros      :       (unsigned int) rpm do motor
*       Retorno         :       nenhum
***********************************************************************************/
void MU_setRPM(unsigned int rpm){
  
  if(rpm>13000)
    rpm = 13000;
  
  if(rpm && rpm<4000)
    rpm = 4000;
  
  if(!rpm){
    CM_ligaMotor = 0;
    vTaskDelay(320);
    SET_GERAL(0);    
  }
  else{
    SET_GERAL(1);
    vTaskDelay(320);
    CM_set_point = rpm;
    vTaskDelay(10);// Pra não subir antes de ligar o motor....
    CM_ligaMotor = 1;
  }
}
/***********************************************************************************
*       Descrição       :       Setter para o controle da resistencia
*       Parametros      :       (unsigned int) temperatura
*       Retorno         :       nenhum
***********************************************************************************/
void MU_setTemperatura(unsigned int temperatura,unsigned short int ciclos){
  
  ligarResistencia = temperatura;  
}
/***********************************************************************************
*       Descrição       :       Lê os RPMs do motor
*       Parametros      :       nenhum
*       Retorno         :       (unsigned int) RPM atual do motor
***********************************************************************************/
unsigned int MU_getRPMmedido(void){  
  unsigned int media=0; 
  
  if(!CM_periodoCapturadoMotor)
    return 0;

  for(unsigned char i=0;i<TAM_BUF_VELOCIDADE;i++)
    media += CM_bufferRotacao[i];  
  media>>=DIV_MEDIA_VEL;
  
  media = 19962072/media;
  
  if(media>13500)
    media = 13500;

  return media;
}
/***********************************************************************************
*       Descrição       :       Setter para o relé do geral do neutro
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void MU_setterGeralNeutro(unsigned char estado){
  
 SET_GERAL(estado);
}
/***********************************************************************************
*       Descrição       :       Calcula o atraso para o gate para uma determinada
*                               rotação
*       Parametros      :       (unsigned int) rotação alvo
*       Retorno         :       (unsigned int) atraso
***********************************************************************************/
unsigned int MU_calculaAtrasoGate(int rotacao_rpm){  
  
#ifdef FQ_REDE_60_HZ  
  long long int valor;
  
  valor = rotacao_rpm*2800;
  valor>>= 15;
  valor = 2499 - valor; 
  
  // trunca os limites
  // do it que gera a interrução
  // para controlar o gate
  // esses valores foram levantado com o scope, então não
  // mudar sem critérios(13/11/2016)
  if(valor<ATRASO_MINIMO)
    valor = ATRASO_MINIMO;
  if(valor>ATRASO_MAXIMO)
    valor = ATRASO_MAXIMO;
  
  return valor;
#endif
  
#ifdef FQ_REDE_50_HZ
  long long int valor;
  
  valor = rotacao_rpm * 5757;//2201;//7707;//4404;//5505;//3302;/*4404*/;
  valor>>= 15;
  valor = ATRASO_MAXIMO - valor; 
  
  // trunca os limites
  // do it que gera a interrução
  // para controlar o gate
  // esses valores foram levantado com o scope, então não
  // mudar sem critérios(13/11/2016)
  if(valor<ATRASO_MINIMO)
    valor = ATRASO_MINIMO;
  if(valor>ATRASO_MAXIMO)
    valor = ATRASO_MAXIMO;
  
  return valor;
#endif  
}
/***********************************************************************************
*       Fim do arquivo
***********************************************************************************/

