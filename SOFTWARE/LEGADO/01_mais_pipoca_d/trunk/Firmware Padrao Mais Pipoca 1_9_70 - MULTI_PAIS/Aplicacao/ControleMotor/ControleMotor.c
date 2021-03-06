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
|       Arquivo            :  ControleMotor.c
|       Descri��o          :  Biblioteca para controle da velocidade do
|                             motor universal
| 
|       Autor              :  Marcos Aquino
|       Data cria��o       :  14/02/2015
|                            
|       Revis�es           :  1.0.0.0
|                             1.1(01/05/2015) alterado para controle proporcional da velocidade do motor
|                             1.2(02/05/2015) alterado para rampa de temperatura no controle 
|                                             da resist�ncia.
|
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
*       Defini��es
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
  #define KP                      4.0
  #define KI                      0.1
  #define KD                      0.1
#else
  #define KP                      4.0
  #define KI                      0.1
  #define KD                      0.1
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

/***********************************************************************************
*       Fun��es locais
***********************************************************************************/
unsigned int MU_calculaAtrasoGate(int rotacao_rpm);

/***********************************************************************************
*       Implementa��o das fun��es
***********************************************************************************/

/***********************************************************************************
*       Descri��o       :       Inicializa��o da biblioteca de controle
*                               do motor universal
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void MU_ini(void){
  
  INI_PINS;

  //--------------------------------------------------------------------------//
  //                        Habilita a interrup��o                            //
  //                        no NVIC                                           //
  //--------------------------------------------------------------------------//
  
  PCONP_bit.PCTIM1 = 1;  
  IP0_bit.PRI_2 = 0;  
  SETENA0_bit.SETENA2 = 1; // Habilita��o da interrup��o do timer 1
  
  T1TCR_bit.CE = 1;   // Contador Habilitado
  T1TCR_bit.CR = 1;   // Realiza o reset
  T1TCR_bit.CR = 0;   // do contador
  
  T1CTCR_bit.CTM = 0x00;
  T1PR = PR_TIMER;
   
  PINSEL3_bit.P1_19 = 3;
  
  T1CCR_bit.CAP1RE = 1; // Borda de subida
  T1CCR_bit.CAP1FE = 0; // Borda de descida  
  T1CCR_bit.CAP1I = 1;  // Interrup��o por evento no canal  
  //-------------------------------------------------------------------------//
  //                    Inicializa��o do timer 2                             //
  //-------------------------------------------------------------------------//
  PCONP_bit.PCTIM2 = 1; // Energiza o perif�rico
  PCLKSEL1_bit.PCLK_TIMER2 = 1; // Sele��o do clock
  
  IP0_bit.PRI_3 = 0;  
  SETENA0_bit.SETENA3 = 1; // Habilita��o da interrup��o do timer 1
  
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
  T2EMR_bit.EM0  = 0; // N�o altera nenhum pino    
  T2EMR_bit.EMC0 = 0;
  
  T2MR0 = T2TC+1000;
  //-------------------------------------------------------------------------//
  //               Math 1 para o dimmer da resist�ncia                       //
  //-------------------------------------------------------------------------//
  T2MCR_bit.MR1I = 0;
  T2MCR_bit.MR1R = 0;
  T2EMR_bit.EM1  = 0; // N�o altera nenhum pino    
  T2EMR_bit.EMC1 = 0;
  
  T2MR1 = T2TC+1000;    
  //-------------------------------------------------------------------------//
  //              Fim da inicializa��o do match                              //
  //-------------------------------------------------------------------------//
  
  // --------------------------
  // Interrup��o externa
  // --------------------------
  PINSEL4_bit.P2_11 = 1;
  EXTMODE_bit.EXTMODE1 = 1;
  EXTPOLAR_bit.EXTPOLAR1 = 1;    
  SETENA0_bit.SETENA19 = 1;   
}
/***********************************************************************************
*       Descri��o       :       Interrup��o do timer 2
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void MU_mathTimer2(void){
  static unsigned char pino=0;
  //static unsigned char resistencia=0;
  
  //-----------------------------------------------//
  //               Controle do motor               //
  //-----------------------------------------------//
  if(T2IR_bit.MR0INT){
    if(!pino){
      SET_SSR(1);
      pino = 1;
      SET_ATRASO(10);//+ ou - 50uS
    }
    else{
      SET_SSR(0);
      pino = 0;
      T2MCR_bit.MR0I = 0;
    }  
    T2IR_bit.MR0INT = 1;
    CLRPEND0_bit.CLRPEND3 = 1;
  }
  
}
/***********************************************************************************
*       Descri��o       :       Soma satura de 32 bits
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
*       Descri��o       :       Calcula o erro integral 
*       Parametros      :       (int) erro atual
*       Retorno         :       (int) erro integral
***********************************************************************************/
#pragma inline
int MU_calcula_integral_erro(int erro,unsigned flush){
  static int y=0;
      
  if(flush)
    y = 0;
  
  if(erro>0)
    if(y <  3600000)
      y+= erro;
        
  if(erro<0)
    if(y > -3600000)
      y+= erro;
  
  return y;
}
/***********************************************************************************
*       Descri��o       :       Calcula a derivada do erro
*       Parametros      :       (int) erro
*       Retorno         :       (int) derivada do erro
***********************************************************************************/
#pragma inline
int CM_calcula_derivada_erro(int erro){
  static int ultimo_erro = 0;
  //int y;
  
  erro = erro - ultimo_erro;
  
  if(erro>3600000)
    erro = 3600000;
  
  if(erro<-3600000)
    erro = -3600000;
  
  ultimo_erro = erro;  
  
  return 0;
}
/***********************************************************************************
*       Descri��o       :       Fun��o que faz o controle da velocidade do 
*                               motor dentro do timer
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
const unsigned int kp_const = KP*256;//(1<<8);
const unsigned int kd_const = KD*32768;//(1<<15);
const unsigned int ki_const = KI*32768;//(1<<15);

#pragma inline
void MU_controleVelocidade(void){
  int erro;
  long long int erro_i;
  long long int erro_d;
  static unsigned short int referencia_rampa=0;
  static unsigned short int ciclos=180;
    
  if(CM_ligaMotor){   
      
        //----------------------------------------
        // Faz a refer�ncia crescer lentamente
        //----------------------------------------
        if(referencia_rampa!=CM_set_point){
          if(referencia_rampa>CM_set_point)
            referencia_rampa-=50;//
          else
            referencia_rampa+=50;//
        }      
        
        if(ciclos){
          ciclos--;
          SET_ATRASO(MU_calculaAtrasoGate(referencia_rampa));
        }
        else{
          // Calcula o erro entre a refer�ncia e a rota��o medida pelo sensor
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
          erro += erro_i + erro_d;
                         
          SET_ATRASO(MU_calculaAtrasoGate(erro));
        }
  }
  else{
    MU_calcula_integral_erro(0,1);
    referencia_rampa = ATRASO_MAXIMO;
    ciclos = 30;
  }  
}
/***********************************************************************************
*       Descri��o       :       Interrup��o do EINT1
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void MU_eintInterrupt(void){  
  
  MU_controleVelocidade();
  
  MU_ciclos++;  
  PID_temp();

  EXTINT_bit.EINT1 = 1;
  CLRPEND0_bit.CLRPEND19 = 1;
}
/***********************************************************************************
*       Descri��o       :       Tick de timer para a biblioteca de controle
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
  // da rota��o do motor
  if(CM_timeOutCounter){
    if(CM_timeOutCounter==1){
      for(unsigned char i=0;i<TAM_BUF_VELOCIDADE;i++)
        CM_bufferRotacao[i] = 0;
    }
    CM_timeOutCounter--;
  }    
  
}
/***********************************************************************************
*       Descri��o       :       Getter para a frequ�ncia da rede el�trica
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
unsigned short int MU_getFrequenciaRede(void){
  
  return MU_frequencia;
}
/***********************************************************************************
*       Descri��o       :       Interrup��o do capture
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
*       Descri��o       :       Setter para a rota��o do motor
*       Parametros      :       (unsigned int) rpm do motor
*       Retorno         :       nenhum
***********************************************************************************/
void MU_setRPM(unsigned int rpm){
  
  if(!rpm){
    CM_ligaMotor = 0;
    SET_GERAL(0);
  }
  else{
    SET_GERAL(1);
    vTaskDelay(100);
    CM_set_point = rpm;
    vTaskDelay(2);// Pra n�o subir antes de ligar o motor....
    //CM_atrasoGateMotor = MU_calculaAtrasoGate(rpm);
    CM_ligaMotor = 1;
  }
}
/***********************************************************************************
*       Descri��o       :       Setter para o controle da resistencia
*       Parametros      :       (unsigned int) temperatura
*       Retorno         :       nenhum
***********************************************************************************/
void MU_setTemperatura(unsigned int temperatura,unsigned short int ciclos){
  
  ligarResistencia = temperatura;  
}
/***********************************************************************************
*       Descri��o       :       L� os RPMs do motor
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
  
  return media;
}
/***********************************************************************************
*       Descri��o       :       Setter para o rel� do geral do neutro
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
void MU_setterGeralNeutro(unsigned char estado){
  
 SET_GERAL(estado);
}
/***********************************************************************************
*       Descri��o       :       Calcula o atraso para o gate para uma determinada
*                               rota��o
*       Parametros      :       (unsigned int) rota��o alvo
*       Retorno         :       (unsigned int) atraso
***********************************************************************************/
unsigned int MU_calculaAtrasoGate(int rotacao_rpm){  
  
#ifdef FQ_REDE_60_HZ  
  long long int valor;
  
  valor = rotacao_rpm*4404;
  valor>>= 15;
  valor = 2499 - valor; 
  
  // trunca os limites
  // do it que gera a interru��o
  // para controlar o gate
  // esses valores foram levantado com o scope, ent�o n�o
  // mudar sem crit�rios(13/11/2016)
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
  // do it que gera a interru��o
  // para controlar o gate
  // esses valores foram levantado com o scope, ent�o n�o
  // mudar sem crit�rios(13/11/2016)
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
|       Arquivo            :  ControleMotor.c
|       Descri��o          :  Biblioteca para controle da velocidade do
|                             motor universal
| 
|       Autor              :  Marcos Aquino
|       Data cria��o       :  14/02/2015
|                            
|       Revis�es           :  1.0.0.0
|                             1.1(01/05/2015) alterado para controle proporcional da velocidade do motor
|                             1.2(02/05/2015) alterado para rampa de temperatura no controle 
|                                             da resist�ncia.
|
|
| __________________________________________________________________________________
*/

/***********************************************************************************
*       Includes
***********************************************************************************/
/*#include <nxp\iolpc1768.h>
#include "controleMotor.h"
#include "..\..\includes.h"
#include "PIDtemperatura.h"
*/
/***********************************************************************************
*       Defini��es
***********************************************************************************/
/*
#ifdef FQ_REDE_60_HZ
  #define ATRASO_MAXIMO                           2500
  #define ATRASO_MINIMO                           150
#endif

#ifdef FQ_REDE_50_HZ
  #define ATRASO_MAXIMO                           3100
  #define ATRASO_MINIMO                           300
#endif

*/

/***********************************************************************************
*       Ganhos do PID
***********************************************************************************/
/*
#ifdef FQ_REDE_60_HZ
  #define KP                      2.5
  #define KI                      0.01
  #define KD                      0.005
#else
  #define KP                      2.5
  #define KI                      0.01
  #define KD                      0.005
#endif
*/

/***********************************************************************************
*       Variaveis locais
***********************************************************************************/
/*
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
*/

/***********************************************************************************
*       Fun��es locais
***********************************************************************************/
//unsigned int MU_calculaAtrasoGate(int rotacao_rpm);

/***********************************************************************************
*       Implementa��o das fun��es
***********************************************************************************/

/***********************************************************************************
*       Descri��o       :       Inicializa��o da biblioteca de controle
*                               do motor universal
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
/*
void MU_ini(void){
  
  INI_PINS;

  //--------------------------------------------------------------------------//
  //                        Habilita a interrup��o                            //
  //                        no NVIC                                           //
  //--------------------------------------------------------------------------//
  
  PCONP_bit.PCTIM1 = 1;  
  IP0_bit.PRI_2 = 0;  
  SETENA0_bit.SETENA2 = 1; // Habilita��o da interrup��o do timer 1
  
  T1TCR_bit.CE = 1;   // Contador Habilitado
  T1TCR_bit.CR = 1;   // Realiza o reset
  T1TCR_bit.CR = 0;   // do contador
  
  T1CTCR_bit.CTM = 0x00;
  T1PR = PR_TIMER;
   
  PINSEL3_bit.P1_19 = 3;
  
  T1CCR_bit.CAP1RE = 1; // Borda de subida
  T1CCR_bit.CAP1FE = 0; // Borda de descida  
  T1CCR_bit.CAP1I = 1;  // Interrup��o por evento no canal  
  //-------------------------------------------------------------------------//
  //                    Inicializa��o do timer 2                             //
  //-------------------------------------------------------------------------//
  PCONP_bit.PCTIM2 = 1; // Energiza o perif�rico
  PCLKSEL1_bit.PCLK_TIMER2 = 1; // Sele��o do clock
  
  IP0_bit.PRI_3 = 0;  
  SETENA0_bit.SETENA3 = 1; // Habilita��o da interrup��o do timer 1
  
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
  T2EMR_bit.EM0  = 0; // N�o altera nenhum pino    
  T2EMR_bit.EMC0 = 0;
  
  T2MR0 = T2TC+1000;
  //-------------------------------------------------------------------------//
  //               Math 1 para o dimmer da resist�ncia                       //
  //-------------------------------------------------------------------------//
  T2MCR_bit.MR1I = 0;
  T2MCR_bit.MR1R = 0;
  T2EMR_bit.EM1  = 0; // N�o altera nenhum pino    
  T2EMR_bit.EMC1 = 0;
  
  T2MR1 = T2TC+1000;    
  //-------------------------------------------------------------------------//
  //              Fim da inicializa��o do match                              //
  //-------------------------------------------------------------------------//
  
  // --------------------------
  // Interrup��o externa
  // --------------------------
  PINSEL4_bit.P2_11 = 1;
  EXTMODE_bit.EXTMODE1 = 1;
  EXTPOLAR_bit.EXTPOLAR1 = 1;    
  SETENA0_bit.SETENA19 = 1;   
}
*/
/***********************************************************************************
*       Descri��o       :       Interrup��o do timer 2
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
/*
void MU_mathTimer2(void){
  static unsigned char pino=0;
  //static unsigned char resistencia=0;
  
  //-----------------------------------------------//
  //               Controle do motor               //
  //-----------------------------------------------//
  if(T2IR_bit.MR0INT){
    if(!pino){
      SET_SSR(1);
      pino = 1;
      SET_ATRASO(10);//+ ou - 50uS
    }
    else{
      SET_SSR(0);
      pino = 0;
      T2MCR_bit.MR0I = 0;
    }  
    T2IR_bit.MR0INT = 1;
    CLRPEND0_bit.CLRPEND3 = 1;
  }
  
}

*/
/***********************************************************************************
*       Descri��o       :       Soma satura de 32 bits
*       Parametros      :       (int) valor 1
*                               (int) valor2
*       Retorno         :       (int) soma satura dos dois valores
***********************************************************************************/
/*
int sadd(int a, int b){
  int sum = a+b;
  int overflow = ((a^sum)&(b^sum))>>31;
  return (overflow<<31)^(sum>>overflow); 
}
*/
/***********************************************************************************
*       Descri��o       :       Calcula o erro integral 
*       Parametros      :       (int) erro atual
*       Retorno         :       (int) erro integral
***********************************************************************************/
/*
#pragma inline
int MU_calcula_integral_erro(int erro,unsigned flush){
  static int buffer_z[4];
  static unsigned short int indice=0;
  static int acumulado=0;
  int y=0;
  
  
  if(flush){
    acumulado = 0;
    buffer_z[0] = buffer_z[1] = buffer_z[2] = buffer_z[3] = 0;
  }
  else{
    
    buffer_z[indice] = erro;
  
    y = sadd(y,9*erro); //y +=  9 * erro;
    y = sadd(y,19 * buffer_z[(indice+1)%4]); //y +=  19 * buffer_z[(indice+1)%4];
    y = sadd(y,-5 * buffer_z[(indice+2)%4]);  //y -=  5 * buffer_z[(indice+2)%4];
    y = sadd(y,buffer_z[(indice+3)%4]);  //y +=  buffer_z[(indice+3)%4];
        
    y *=  170;
    y >>= 12;
    
    y = sadd(y,acumulado); // Soma com satura��o, 32 bits com sinal           
    acumulado = y;
  
    indice = (indice+1)%4;    
  }
  
  return y;
}
*/
/***********************************************************************************
*       Descri��o       :       Calcula a derivada do erro
*       Parametros      :       (int) erro
*       Retorno         :       (int) derivada do erro
***********************************************************************************/
/*
#pragma inline
int CM_calcula_derivada_erro(int erro){
  static int buffer_z[4];
  static unsigned short int indice=0;
  int y=0;
  
  buffer_z[indice] = erro;
  
  y+= 11*erro;
  y-= 18*buffer_z[(indice+1)%4];
  y+=  9*buffer_z[(indice+2)%4];
  y-=  2*buffer_z[(indice+3)%4];  
  indice = (indice+1) % 4;  
  y/=6;
  
  return y;
}
*/
/***********************************************************************************
*       Descri��o       :       Fun��o que faz o controle da velocidade do 
*                               motor dentro do timer
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
/*
#pragma inline
void MU_controleVelocidade(void){
  int erro;
  long long int erro_i;
  long long int erro_d;
  const unsigned int kp_const = KP*(1<<8);
  const unsigned int kd_const = (unsigned int)KD*(1<<15);
  const unsigned int ki_const = (unsigned int)KI*(1<<15);
  static unsigned short int referencia_rampa=0;
  static unsigned short int ciclos=180;
    
  if(CM_ligaMotor){   
      
        //----------------------------------------
        // Faz a refer�ncia crescer lentamente
        //----------------------------------------
        if(referencia_rampa!=CM_set_point){
          if(referencia_rampa>CM_set_point)
            referencia_rampa-=50;//
          else
            referencia_rampa+=50;//
        }      
        
        if(ciclos){
          ciclos--;
          SET_ATRASO(MU_calculaAtrasoGate(referencia_rampa));
        }
        else{
          // Calcula o erro entre a refer�ncia e a rota��o medida pelo sensor
          erro = referencia_rampa - MU_getRPMmedido();
          // Calcula a integral do erro
          erro_i = MU_calcula_integral_erro(erro,0);                     
          erro_i *= ki_const;
          erro_i >>= 15;
          
          erro_d = CM_calcula_derivada_erro(erro);
          erro_d *= kd_const;
          erro_d >>= 15;
          
          erro = sadd(erro,erro_i);
          erro = sadd(erro,erro_d);
          erro*= kp_const;
          erro>>= 8;          
          erro+= referencia_rampa;
                         
          SET_ATRASO(MU_calculaAtrasoGate(erro));
        }
  }
  else{
    MU_calcula_integral_erro(0,1);
    referencia_rampa = ATRASO_MAXIMO;
    ciclos = 30;//90;
  }  
}
*/
/***********************************************************************************
*       Descri��o       :       Interrup��o do EINT1
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
/*
void MU_eintInterrupt(void){  
  
  MU_controleVelocidade();
  
  MU_ciclos++;  
  PID_temp();

  EXTINT_bit.EINT1 = 1;
  CLRPEND0_bit.CLRPEND19 = 1;
}
*/
/***********************************************************************************
*       Descri��o       :       Tick de timer para a biblioteca de controle
*                               da velocidade do motor
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
/*
void MU_timerTick(void){  
  static unsigned short int contador=1;  
  
  if(!--contador){
    MU_frequencia = MU_ciclos;
    MU_ciclos = 0;
    contador = 500;
  }    
  
  // Timeout na captura
  // da rota��o do motor
  if(CM_timeOutCounter){
    if(CM_timeOutCounter==1){
      for(unsigned char i=0;i<TAM_BUF_VELOCIDADE;i++)
        CM_bufferRotacao[i] = 0;
    }
    CM_timeOutCounter--;
  }    
  
}
*/
/***********************************************************************************
*       Descri��o       :       Getter para a frequ�ncia da rede el�trica
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
/*
unsigned short int MU_getFrequenciaRede(void){
  
  return MU_frequencia;
}
*/
/***********************************************************************************
*       Descri��o       :       Interrup��o do capture
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
/*
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
*/
/***********************************************************************************
*       Descri��o       :       Setter para a rota��o do motor
*       Parametros      :       (unsigned int) rpm do motor
*       Retorno         :       nenhum
***********************************************************************************/
/*
void MU_setRPM(unsigned int rpm){
  
  if(!rpm){
    CM_ligaMotor = 0;
    SET_GERAL(0);
  }
  else{
    SET_GERAL(1);
    vTaskDelay(100);
    CM_set_point = rpm;
    vTaskDelay(2);// Pra n�o subir antes de ligar o motor....
    //CM_atrasoGateMotor = MU_calculaAtrasoGate(rpm);
    CM_ligaMotor = 1;
  }

}
*/
/***********************************************************************************
*       Descri��o       :       Setter para o controle da resistencia
*       Parametros      :       (unsigned int) temperatura
*       Retorno         :       nenhum
***********************************************************************************/
/*
void MU_setTemperatura(unsigned int temperatura,unsigned short int ciclos){
  
  ligarResistencia = temperatura;  
}
*/
/***********************************************************************************
*       Descri��o       :       L� os RPMs do motor
*       Parametros      :       nenhum
*       Retorno         :       (unsigned int) RPM atual do motor
***********************************************************************************/
/*
unsigned int MU_getRPMmedido(void){  
  unsigned int media=0; 
  
  if(!CM_periodoCapturadoMotor)
    return 0;
  
  for(unsigned char i=0;i<TAM_BUF_VELOCIDADE;i++)
    media += CM_bufferRotacao[i];
  
  media>>=DIV_MEDIA_VEL;
  media = 19962072/media;
  
  return media;
}
*/
/***********************************************************************************
*       Descri��o       :       Setter para o rel� do geral do neutro
*       Parametros      :       nenhum
*       Retorno         :       nenhum
***********************************************************************************/
/*
void MU_setterGeralNeutro(unsigned char estado){
  
 SET_GERAL(estado);
}
*/
/***********************************************************************************
*       Descri��o       :       Calcula o atraso para o gate para uma determinada
*                               rota��o
*       Parametros      :       (unsigned int) rota��o alvo
*       Retorno         :       (unsigned int) atraso
***********************************************************************************/
/*
unsigned int MU_calculaAtrasoGate(int rotacao_rpm){  
  
#ifdef FQ_REDE_60_HZ  
  long long int valor;
  
  valor = rotacao_rpm*4404;
  valor>>= 15;
  valor = 2499 - valor; 
  
  // trunca os limites
  // do it que gera a interru��o
  // para controlar o gate
  // esses valores foram levantado com o scope, ent�o n�o
  // mudar sem crit�rios(13/11/2016)
  if(valor<ATRASO_MINIMO)
    valor = ATRASO_MINIMO;
  if(valor>ATRASO_MAXIMO)
    valor = ATRASO_MAXIMO;
  
  return valor;
#endif
  
#ifdef FQ_REDE_50_HZ
  long long int valor;
  */

//  valor = rotacao_rpm * 5757;//2201;//7707;//4404;//5505;//3302;/*4404*/;

/*
  valor>>= 15;
  valor = ATRASO_MAXIMO - valor; 
  
  // trunca os limites
  // do it que gera a interru��o
  // para controlar o gate
  // esses valores foram levantado com o scope, ent�o n�o
  // mudar sem crit�rios(13/11/2016)
  if(valor<ATRASO_MINIMO)
    valor = ATRASO_MINIMO;
  if(valor>ATRASO_MAXIMO)
    valor = ATRASO_MAXIMO;
  
  return valor;
#endif  
}*/
/***********************************************************************************
*       Fim do arquivo
***********************************************************************************/
