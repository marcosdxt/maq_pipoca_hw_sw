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
|       Arquivo            :
|       Descri��o          :
| 
|       Autor              :
|       Data cria��o       :
|
|       Revis�es           :
|
|
| __________________________________________________________________________________
*/
/********************************************************************************
*   Includes
********************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
/********************************************************************************
*   Defini��es
********************************************************************************/
#define portNVIC_SYSTICK_CTRL		( ( volatile unsigned long *) 0xe000e010 )
#define portNVIC_SYSTICK_LOAD		( ( volatile unsigned long *) 0xe000e014 )
#define portNVIC_INT_CTRL		( ( volatile unsigned long *) 0xe000ed04 )
#define portNVIC_SYSPRI2		( ( volatile unsigned long *) 0xe000ed20 )
#define portNVIC_SYSTICK_CLK		0x00000004
#define portNVIC_SYSTICK_INT		0x00000002
#define portNVIC_SYSTICK_ENABLE		0x00000001
#define portNVIC_PENDSVSET		0x10000000
#define portNVIC_PENDSV_PRI		( ( ( unsigned long ) configKERNEL_INTERRUPT_PRIORITY ) << 16 )
#define portNVIC_SYSTICK_PRI		( ( ( unsigned long ) configKERNEL_INTERRUPT_PRIORITY ) << 24 )
#define portINITIAL_XPSR			( 0x01000000 )
#ifndef configKERNEL_INTERRUPT_PRIORITY
	#define configKERNEL_INTERRUPT_PRIORITY 0
#endif
/********************************************************************************
*   Variaveis locais
********************************************************************************/
 unsigned portBASE_TYPE uxCriticalNesting = 0xaaaaaaaa;

/********************************************************************************
*   Fun��es locais
********************************************************************************/
static void prvSetupTimerInterrupt( void );
void xPortSysTickHandler( void );

/********************************************************************************
*   Fun��es externas ao m�dulo
********************************************************************************/
extern void BOARD_timerHook(void);
extern void MU_timerTick(void);
/********************************************************************************
*   Implementa��o
********************************************************************************/

/********************************************************************************
*   Descri��o     :
*   Parametros    : 
*   Retorno       : 
********************************************************************************/
portSTACK_TYPE *pxPortInitialiseStack( portSTACK_TYPE *pxTopOfStack, pdTASK_CODE pxCode, void *pvParameters )
{
    pxTopOfStack--; /* Offset added to account for the way the MCU uses the stack on entry/exit of interrupts. */
    *pxTopOfStack = portINITIAL_XPSR;	/* xPSR */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) pxCode;	/* PC */
    pxTopOfStack--;
    *pxTopOfStack = 0;	/* LR */
    pxTopOfStack -= 5;	/* R12, R3, R2 and R1. */
    *pxTopOfStack = ( portSTACK_TYPE ) pvParameters;	/* R0 */
    pxTopOfStack -= 8;	/* R11, R10, R9, R8, R7, R6, R5 and R4. */

    return pxTopOfStack;
}
/********************************************************************************
*   Descri��o     :   
*   Parametros    : 
*   Retorno       : 
********************************************************************************/
portBASE_TYPE xPortStartScheduler( void )
{
    *(portNVIC_SYSPRI2) |= portNVIC_PENDSV_PRI;
    *(portNVIC_SYSPRI2) |= portNVIC_SYSTICK_PRI;

    prvSetupTimerInterrupt();	
    uxCriticalNesting = 0;

    vPortStartFirstTask();
    return 0;
}
/********************************************************************************
*   Descri��o     : 
*   Parametros    :
*   Retorno       : 
********************************************************************************/
void vPortEndScheduler( void )
{

  
}
/********************************************************************************
*   Descri��o     : 
*   Parametros    : 
*   Retorno       : 
********************************************************************************/
void vPortYieldFromISR( void )
{

    *(portNVIC_INT_CTRL) = portNVIC_PENDSVSET;
}
/********************************************************************************
*   Descri��o     : 
*   Parametros    : 
*   Retorno       : 
********************************************************************************/
void vPortEnterCritical( void )
{
	portDISABLE_INTERRUPTS();
	uxCriticalNesting++;
}
/********************************************************************************
*   Descri��o     : 
*   Parametros    : 
*   Retorno       : 
********************************************************************************/
void vPortExitCritical( void )
{
	uxCriticalNesting--;
	if( uxCriticalNesting == 0 )
	{
		portENABLE_INTERRUPTS();
	}
}
/********************************************************************************
*   Descri��o     :   
*   Parametros    :   
*   Retorno       : 
********************************************************************************/
extern void BOARD_timer_hook(void);
extern void CE_tick(void);
extern unsigned int IU_propagandaAlmeida;
extern unsigned short int IU_tempoRefreshSegundoLCD;
extern unsigned int IU_contadorResfriamento;
extern unsigned int IU_inatividadeProcesso;
extern unsigned int HORIMETRO_tickCounter;
extern unsigned int IU_tempoInfoHorimetro;
extern void IU_tickCompensador(void);

void xPortSysTickHandler( void )
{
  unsigned long ulDummy;
  
    MU_timerTick(); // Aqui para diminuir o jitter....

  *(portNVIC_INT_CTRL) = portNVIC_PENDSVSET;	

    ulDummy = portSET_INTERRUPT_MASK_FROM_ISR();
    {
	vTaskIncrementTick();
    }
    portCLEAR_INTERRUPT_MASK_FROM_ISR( ulDummy );
        
  if(IU_propagandaAlmeida)
    IU_propagandaAlmeida--;
  
  if(IU_tempoRefreshSegundoLCD)
    IU_tempoRefreshSegundoLCD--;

  if(IU_contadorResfriamento)
    IU_contadorResfriamento--;

  if(IU_inatividadeProcesso)
    IU_inatividadeProcesso--;
  
  //if(HORIMETRO_tickCounter)
    //HORIMETRO_tickCounter--;
  
  //if(IU_tempoInfoHorimetro)
    //IU_tempoInfoHorimetro--;
  
  // Hook da placa....     
  BOARD_timerHook();
  IU_tickCompensador();
}
/********************************************************************************
*   Descri��o     :   
*   Parametros    : 
*   Retorno       : 
********************************************************************************/
void prvSetupTimerInterrupt( void ){
  
  *(portNVIC_SYSTICK_LOAD) = ( configCPU_CLOCK_HZ / configTICK_RATE_HZ ) - 1UL;
  *(portNVIC_SYSTICK_CTRL) = portNVIC_SYSTICK_CLK | portNVIC_SYSTICK_INT | portNVIC_SYSTICK_ENABLE;
}
/********************************************************************************
*   Fim do arquivo
********************************************************************************/

