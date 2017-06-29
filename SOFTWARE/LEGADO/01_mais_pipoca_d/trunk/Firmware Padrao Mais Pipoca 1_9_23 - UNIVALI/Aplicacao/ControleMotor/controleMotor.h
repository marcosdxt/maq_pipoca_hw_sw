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
|       Arquivo            :  ControleMotor.h
|       Descrição          :  Biblioteca para controle da velocidade do
|                             motor universal
| 
|       Autor              :  Marcos Aquino
|       Data criação       :  14/02/2015
|
|       Revisões           :  1.0.0.0
|
|
| __________________________________________________________________________________
*/

#ifndef _CONTROLE_MOTOR_H_
#define _CONTROLE_MOTOR_H_

/***********************************************************************************
*       Definições locais
***********************************************************************************/
#define CONTROLE_RESISTENCIA    (0x01)<<3
#define SSR                     (0x01)<<2
#define GERAL_NEUTRO            (0x01)<<24

#define INI_PINS                 FIO2DIR |= SSR | CONTROLE_RESISTENCIA; FIO0DIR |= GERAL_NEUTRO

#define SET_SSR(X)              X?(FIO2SET|=SSR):(FIO2CLR|=SSR)
#define SET_GERAL(X)            X?(FIO0SET|=GERAL_NEUTRO):(FIO0CLR|=GERAL_NEUTRO)
#define SET_RESISTENCIA(X)      X?(FIO2SET|=CONTROLE_RESISTENCIA):(FIO2CLR|=CONTROLE_RESISTENCIA)

#define CLK                     96000000
#define PR_TIMER                300
#define PERIODO_BASICO          1.0/(float)((unsigned int)CLK/(unsigned int)PR_TIMER)

#define TAMANHO_BUFFER_RPM      8

#define SET_ATRASO(X)          {T2MR0 = T2TC+X; T2MCR_bit.MR0I = 1;}
#define SET_ATRASO_RES(X)      {T2MR1 = T2TC+X; T2MCR_bit.MR1I = 1;}

#define RELOAD_TIMEOUT_RRPM     1000    

#define TAM_BUF_VELOCIDADE      4
#define DIV_MEDIA_VEL           2       

#define KP_MOTOR                92
#define KD_MOTOR                250

#define CICLOS_INICIO_MOTOR     1


void MU_ini(void);
void MU_timerTick(void);
unsigned short int MU_getFrequenciaRede(void);
void MU_setMotor(unsigned short int rpm);
void MU_captureInterrupt(void);
unsigned int MU_getRPMmedido(void);
void MU_setRPM(unsigned int rpm);
void MU_setTemperatura(unsigned int temperatura,unsigned short int ciclos);
void MU_setRPMDireto(unsigned int rpm);
void MU_setterGeralNeutro(unsigned char estado);
unsigned int MU_getRPMmedido(void);

/***********************************************************************************
*       Fim do arquivo
***********************************************************************************/
#endif//_CONTROLE_MOTOR_H_