/****************************************Copyright (c)****************************************************

**--------------File Info---------------------------------------------------------------------------------
** File name:               uctsk_Blink.c
** Descriptions:            The uctsk_Blink application function
**
**-------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-9
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <includes.h>            


/* Private variables ---------------------------------------------------------*/
static  OS_STK         App_TaskBlinkStk[APP_TASK_BLINK_STK_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void uctsk_Blink            (void);
void LED_GPIO_Configuration (void);


void  App_BlinkTaskCreate (void)
{
    CPU_INT08U  os_err;

	os_err = os_err; /* prevent warning... */

	os_err = OSTaskCreate((void (*)(void *)) uctsk_Blink,				
                          (void          * ) 0,							
                          (OS_STK        * )&App_TaskBlinkStk[APP_TASK_BLINK_STK_SIZE - 1],		
                          (INT8U           ) APP_TASK_BLINK_PRIO  );							

	#if OS_TASK_NAME_EN > 0
    	OSTaskNameSet(APP_TASK_BLINK_PRIO, "Task LED Blink", &os_err);
	#endif

}

static void uctsk_Blink (void) 
{                
    LED_GPIO_Configuration();

   	for(;;)
   	{   
	    /*====LED-ON=======*/
		GPIO_SetBits(GPIOE , GPIO_Pin_2);
		GPIO_SetBits(GPIOE , GPIO_Pin_3);
		GPIO_SetBits(GPIOE , GPIO_Pin_4);
		GPIO_SetBits(GPIOE , GPIO_Pin_5);
		OSTimeDlyHMSM(0, 0, 0, 100);	 /* 100 MS  */ 
		/*====LED-OFF=======*/
		GPIO_ResetBits(GPIOE , GPIO_Pin_2);
		GPIO_ResetBits(GPIOE , GPIO_Pin_3);
		GPIO_ResetBits(GPIOE , GPIO_Pin_4);
		GPIO_ResetBits(GPIOE , GPIO_Pin_5);
		OSTimeDlyHMSM(0, 0, 0, 100);	 /* 100 MS  */

		Display_IPAddress();
   }
}

/*******************************************************************************
* Function Name  : LED_GPIO_Configuration
* Description    : Configure GPIO Pin
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LED_GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE , ENABLE); 						 
/**
 *  LED1 -> PE2 , LED2 -> PE3 , LED3 -> PE4 , LED4 -> PE5
 */					 
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOE, &GPIO_InitStructure);
}


/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
