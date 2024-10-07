/*
*********************************************************************************************************
*
*                                       MES1 Embedded Software (RTOS)
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : Lukas Buchmayer es24m030
                  
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <includes.h>
#include <cyapicallbacks.h>

/*
*********************************************************************************************************
*                                             LOCAL DEFINES
*********************************************************************************************************
*/

#define  APP_USER_IF_SIGN_ON                        0u
#define  APP_USER_IF_VER_TICK_RATE                  1u
#define  APP_USER_IF_CPU                            2u
#define  APP_USER_IF_CTXSW                          3u
#define  APP_USER_IF_STATE_MAX                      4u

#define  USE_BSP_TOGGLE                             1u

/*
*********************************************************************************************************
*                                            LOCAL VARIABLES
*********************************************************************************************************
*/

//----- semaphores ------
OS_SEM button_sem;


//----- tasks ------
static  OS_TCB   App_TaskStartTCB;
static  CPU_STK  App_TaskStartStk[APP_CFG_TASK_START_STK_SIZE];

static  OS_TCB   App_TaskLED_TCB;
static  CPU_STK  App_TaskLEDStk[APP_CFG_TASK_LED_STK_SIZE];

static  OS_TCB   App_TaskButton_TCB;
static  CPU_STK  App_TaskButtonStk[APP_CFG_TASK_BUTTON_STK_SIZE];

static  OS_TCB   App_TaskCOM_TCB;
static  CPU_STK  App_TaskCOMStk[APP_CFG_TASK_COM_STK_SIZE];



static  OS_TCB   App_TaskLED_BLUE_TCB;
static  CPU_STK  App_TaskLED_BLUE_Stk[APP_CFG_TASK_LED_BLUE_STK_SIZE];

static  OS_TCB   App_TaskLED_GREEN_TCB;
static  CPU_STK  App_TaskLED_GREEN_Stk[APP_CFG_TASK_LED_GREEN_STK_SIZE];

static  OS_TCB   App_TaskLED_RED_TCB;
static  CPU_STK  App_TaskLED_RED_Stk[APP_CFG_TASK_LED_RED_STK_SIZE];


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  App_TaskStart  (void *p_arg);

static  void  App_TaskLED (void *p_arg);
static  void  App_TaskBUTTON (void *p_arg);
static  void  App_TaskCOM (void *p_arg);

static  void  App_TaskLED_BLUE (void *p_arg);
static  void  App_TaskLED_GREEN (void *p_arg);
static  void  App_TaskLED_RED (void *p_arg);

static  void  App_TaskCreate (void);
static  void  App_ObjCreate  (void);

/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : Startup Code.
*
* Note(s)     : none.
*********************************************************************************************************
*/

int  main (void)
{
  OS_ERR  os_err;


  BSP_PreInit();                                              /* Perform BSP pre-initialization.                      */

  CPU_Init();                                                 /* Initialize the uC/CPU services                       */

  OSInit(&os_err);                                            /* Init uC/OS-III.                                      */

  OSTaskCreate((OS_TCB      *)&App_TaskStartTCB,              /* Create the start task                                */
               (CPU_CHAR    *)"Start",
               (OS_TASK_PTR  )App_TaskStart, 
               (void        *)0,
               (OS_PRIO      )APP_CFG_TASK_START_PRIO,
               (CPU_STK     *)&App_TaskStartStk[0],
               (CPU_STK_SIZE )APP_CFG_TASK_START_STK_SIZE_LIMIT,
               (CPU_STK_SIZE )APP_CFG_TASK_START_STK_SIZE,
               (OS_MSG_QTY   )0u,
               (OS_TICK      )0u,
               (void        *)0,
               (OS_OPT       )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
               (OS_ERR      *)&os_err);

  OSStart(&os_err);                                            /* Start multitasking (i.e. give control to uC/OS-III).  */
}


/*
*********************************************************************************************************
*                                          App_TaskStart()
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskStart()' by 'OSTaskCreate()'.
*
* Return(s)   : none
*
* Note(s)     : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  App_TaskStart (void *p_arg)
{
  OS_ERR       err;
     
 (void)p_arg;

  BSP_PostInit();                                               /* Perform BSP post-initialization functions.       */
  
  BSP_CPU_TickInit();                                           /* Perfrom Tick Initialization                      */

#if (OS_CFG_STAT_TASK_EN > 0u)
  OSStatTaskCPUUsageInit(&err);
#endif    

#ifdef CPU_CFG_INT_DIS_MEAS_EN
  CPU_IntDisMeasMaxCurReset();
#endif      

  App_TaskCreate();                                             /* Create application tasks.                        */

  App_ObjCreate();                                              /* Create kernel objects                            */

  while (DEF_TRUE) {                                            /* Task body, always written as an infinite loop.   */
    OSTimeDlyHMSM(0, 0, 0, 100, 
                  OS_OPT_TIME_HMSM_STRICT, 
                  &err);
  }
}


/*
*********************************************************************************************************
*                                          App_TaskCreate()
*
* Description : Create application tasks.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_TaskCreate (void)
{
  /* declare and define function local variables */
  OS_ERR  os_err;
  
  /* create LED task */
  OSTaskCreate((OS_TCB      *)&App_TaskLED_TCB,
               (CPU_CHAR    *)"TaskLED",
               (OS_TASK_PTR  )App_TaskLED, 
               (void        *)0,
               (OS_PRIO      )APP_CFG_TASK_LED_PRIO,
               (CPU_STK     *)&App_TaskLEDStk[0],
               (CPU_STK_SIZE )APP_CFG_TASK_LED_STK_SIZE_LIMIT,
               (CPU_STK_SIZE )APP_CFG_TASK_LED_STK_SIZE,
               (OS_MSG_QTY   )0u,
               (OS_TICK      )0u,
               (void        *)0,
               (OS_OPT       )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
               (OS_ERR      *)&os_err);
  /* create COM task */
  OSTaskCreate((OS_TCB      *)&App_TaskCOM_TCB,
               (CPU_CHAR    *)"TaskCOM",
               (OS_TASK_PTR  )App_TaskCOM, 
               (void        *)0,
               (OS_PRIO      )APP_CFG_TASK_COM_PRIO,
               (CPU_STK     *)&App_TaskCOMStk[0],
               (CPU_STK_SIZE )APP_CFG_TASK_COM_STK_SIZE_LIMIT,
               (CPU_STK_SIZE )APP_CFG_TASK_COM_STK_SIZE,
               (OS_MSG_QTY   )0u,
               (OS_TICK      )0u,
               (void        *)0,
               (OS_OPT       )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
               (OS_ERR      *)&os_err);
  /* create Button task */
  OSTaskCreate((OS_TCB      *)&App_TaskButton_TCB,
               (CPU_CHAR    *)"TaskBUT",
               (OS_TASK_PTR  )App_TaskBUTTON, 
               (void        *)0,
               (OS_PRIO      )APP_CFG_TASK_BUTTON_PRIO,
               (CPU_STK     *)&App_TaskButtonStk[0],
               (CPU_STK_SIZE )APP_CFG_TASK_BUTTON_STK_SIZE_LIMIT,
               (CPU_STK_SIZE )APP_CFG_TASK_BUTTON_STK_SIZE,
               (OS_MSG_QTY   )0u,
               (OS_TICK      )0u,
               (void        *)0,
               (OS_OPT       )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
               (OS_ERR      *)&os_err);

/* create LED BLUE task */
  OSTaskCreate((OS_TCB      *)&App_TaskLED_BLUE_TCB,
               (CPU_CHAR    *)"TaskLED_BLUE",
               (OS_TASK_PTR  )App_TaskLED_BLUE, 
               (void        *)0,
               (OS_PRIO      )APP_CFG_TASK_LED_BLUE_PRIO,
               (CPU_STK     *)&App_TaskLED_BLUE_Stk[0],
               (CPU_STK_SIZE )APP_CFG_TASK_LED_BLUE_STK_SIZE_LIMIT,
               (CPU_STK_SIZE )APP_CFG_TASK_LED_BLUE_STK_SIZE,
               (OS_MSG_QTY   )0u,
               (OS_TICK      )0u,
               (void        *)0,
               (OS_OPT       )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
               (OS_ERR      *)&os_err);
/* create LED GREEN task */
  OSTaskCreate((OS_TCB      *)&App_TaskLED_GREEN_TCB,
               (CPU_CHAR    *)"TaskLED_GREEN",
               (OS_TASK_PTR  )App_TaskLED_GREEN, 
               (void        *)0,
               (OS_PRIO      )APP_CFG_TASK_LED_GREEN_PRIO,
               (CPU_STK     *)&App_TaskLED_GREEN_Stk[0],
               (CPU_STK_SIZE )APP_CFG_TASK_LED_GREEN_STK_SIZE_LIMIT,
               (CPU_STK_SIZE )APP_CFG_TASK_LED_GREEN_STK_SIZE,
               (OS_MSG_QTY   )0u,
               (OS_TICK      )0u,
               (void        *)0,
               (OS_OPT       )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
               (OS_ERR      *)&os_err);
/* create LED RED task */
  OSTaskCreate((OS_TCB      *)&App_TaskLED_RED_TCB,
               (CPU_CHAR    *)"TaskLED_RED",
               (OS_TASK_PTR  )App_TaskLED_RED, 
               (void        *)0,
               (OS_PRIO      )APP_CFG_TASK_LED_RED_PRIO,
               (CPU_STK     *)&App_TaskLED_RED_Stk[0],
               (CPU_STK_SIZE )APP_CFG_TASK_LED_RED_STK_SIZE_LIMIT,
               (CPU_STK_SIZE )APP_CFG_TASK_LED_RED_STK_SIZE,
               (OS_MSG_QTY   )0u,
               (OS_TICK      )0u,
               (void        *)0,
               (OS_OPT       )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
               (OS_ERR      *)&os_err);


}


/*
*********************************************************************************************************
*                                          App_ObjCreate()
*
* Description : Create application kernel objects tasks.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_ObjCreate (void)
{
  /* declare and define function local variables */
  OS_ERR  os_err;
  
  /* create button semaphore to synchronize button press events */
  OSSemCreate(&button_sem,"SEM_BUT",0,&os_err);
}


/*
*********************************************************************************************************
*                                          App_TaskLED()
*
* Description : LED Task pends on a semaphore which is controlled by App_TaskButton(). If the button is
*               pressed the semaphore is constantly posted and this task can toggle the LED (P2_1, J1 Pin 2).
*               If the button is not pressed and the semaphore is not posted, this task waits to inifinity
*               until the button is pressed again. Via the define USE_BSP_TOGGLE both BSP functions can
*               be tested. If USE_BSP_TOGGLE is TRUE the BSP_LED_Toggle() function will be used, otherwise
*               BSP_LED_Off() and BSP_LED_On() will be used.
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskLED()' by 'OSTaskCreate()'.
*
* Return(s)   : none
*
* Note(s)     : none
*********************************************************************************************************
*/


static  void  App_TaskLED (void *p_arg)
{
  /* declare and define task local variables */
  OS_ERR       os_err;
  CPU_TS         ts;
  
  /* prevent compiler warnings */
  (void)p_arg;
  
  /* start of the endless loop */
  while (DEF_TRUE) {
    /* pend on semaphore - blocking */
    OSSemPend(&button_sem,0,OS_OPT_PEND_BLOCKING,&ts,&os_err);
#if USE_BSP_TOGGLE
    /* toggle LED (P2_1, J1 Pin 2) */
    BSP_LED_Toggle(P2_1);
    /* initiate scheduler */
    OSTimeDlyHMSM(0, 0, 1, 0, 
                  OS_OPT_TIME_HMSM_STRICT, 
                  &os_err);
#else
    /* turn LED (P2_1, J1 Pin 2) off */ 
    BSP_LED_Off(P2_1);
    /* initiate scheduler */
    OSTimeDlyHMSM(0, 0, 1, 0, 
                  OS_OPT_TIME_HMSM_STRICT, 
                  &os_err);
    /* turn LED (P2_1, J1 Pin 2) on */
    BSP_LED_On(P2_1);
    /* initiate scheduler */
    OSTimeDlyHMSM(0, 0, 1, 0, 
                  OS_OPT_TIME_HMSM_STRICT, 
                  &os_err);
#endif
  }
}

/*
*********************************************************************************************************
*                                          App_TaskButton()
*
* Description : Button Task checks the state of a GND-connected button. On button press post
*               a semaphore. Otherwise, do not post a semaphore and check the button state again.
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskButton()' by 'OSTaskCreate()'.
*
* Return(s)   : none
*
* Note(s)     : none
*********************************************************************************************************
*/


static  void  App_TaskBUTTON (void *p_arg)
{
  /* declare and define task local variables */
  OS_ERR       os_err;
  CPU_INT08S    button_status = 0;
  static CPU_BOOLEAN button_pressed = 0;
  
  /* prevent compiler warnings */
  (void)p_arg;
  
  /* start of the endless loop */ 
  while (DEF_TRUE) {
    /* check is button is pressed */
    button_status = BSP_PB_StatusGet(P2_2);
    /* check the button status (pressed ... 0 or not pressed ... 1) */
    if(button_status){ //not pressed
      /* if button not pressed reset button_pressed variable (Button State)*/
      PWM_rgb_red_Stop();
      button_pressed = 0;
    }
    else if(!button_status){ //pressed
      button_pressed = 1;
      //broadcast sem to all waiting tasks
      OSSemPost(&button_sem, OS_OPT_POST_ALL, &os_err);
    }
    else{
      /* error */
    }
    /* initiate scheduler */
    OSTimeDlyHMSM(0, 0, 0, 20, 
                  OS_OPT_TIME_HMSM_STRICT, 
                  &os_err);
  }
}

/*
*********************************************************************************************************
*                                          App_TaskCOM()
*
* Description : COM Task checks for available bytes within the UART receive buffer. If correct string is
*               available (e.g. PC -> uC: #abc$ or #Hellor World$), process the message and output a
*               pre-defined string via UART and append the user-defined string via the UART interface.
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskCOM()' by 'OSTaskCreate()'.
*
* Return(s)   : none
*
* Note(s)     : none
*********************************************************************************************************
*/

static  void  App_TaskCOM (void *p_arg)
{
  /* declare and define task local variables */
  OS_ERR       os_err;
  CPU_INT08U   rec_byte = 0x00;
  CPU_INT08U   rx_msg[UART_1_RX_BUFFER_SIZE] = {0};
  CPU_INT08U   idx = 0x00;
  CPU_INT08U   rec_byte_cnt = 0x00;
  CPU_BOOLEAN  str_available = DEF_FALSE;
  
  /* prevent compiler warnings */
  (void)p_arg;
  (void)Start_of_Packet;
  (void)End_of_Packet;
  
  /* start of the endless loop */
  while (DEF_TRUE) {
    /* check if a byte is available */
    rec_byte = uart_get_byte();
    /* check if the received byte is '#'*/
    if(rec_byte == Start_of_Packet){
      /* if received byte was correct */
      while(DEF_TRUE){
        /* receive byte by byte */
        rec_byte = uart_get_byte();
        /* check is byte is something meaningful */
        if(rec_byte){
          /* save byte into software receive buffer and increment idx */
          rx_msg[idx++] = rec_byte;
        }
        /* initiate scheduler */
        OSTimeDlyHMSM(0, 0, 0, 20, 
                      OS_OPT_TIME_HMSM_STRICT, 
                      &os_err);
        /* check if received byte is '$' */
        if(rx_msg[idx-1]==End_of_Packet){
          /* if end of packet is reached -> break */
          break;
        }
      }
      /* message received, calculate received bytes, -2 because of '#' & '$' */
      rec_byte_cnt = idx-2;
      /* signal that a string is available */
      str_available = DEF_TRUE;
    }
    /* if received byte wasn't start of packet */
    else{
      /* reset software receive buffer */
      memset(&rx_msg[0],0,sizeof(rx_msg));
      /* reset string available signal */
      str_available = DEF_FALSE;
      /* reset received byte variable */
      rec_byte = 0x00;
      /* reset idx */
      idx = 0x00;
    }
    /* check if message is available */
    if(str_available){
      /* send pre-defined bytes via UART interface */
      uart_send_byte('C');
      uart_send_byte('Y');
      uart_send_byte('8');
      uart_send_byte('K');
      uart_send_byte('I');
      uart_send_byte('T');
      uart_send_byte(':');
      /* send received message without '#' and '$' */
      for(idx=0;idx<=rec_byte_cnt;idx++){
        uart_send_byte(rx_msg[idx]);
      }
      /* reset software receive buffer */
      memset(&rx_msg[0],0,sizeof(rx_msg));
      /* reset string available signal */
      str_available = DEF_FALSE;
      /* reset received byte variable */
      rec_byte = 0x00;
      /* reset idx */
      idx = 0x00;
    }
    /* initiate scheduler */
    OSTimeDlyHMSM(0, 0, 0, 100, 
                  OS_OPT_TIME_HMSM_STRICT, 
                  &os_err);
  }
}

/*
*********************************************************************************************************
*                                          App_TaskLED_BLUE()
*
* Description : Task for blue rgb led. Controlled by PWM_rgb_blue. Waits for semaphore button_sem and then
*               proceeds according to assignment description. Compare value starts at 255 and decreases by
*               1 every 20ms. If 0 is reached then jump back to 255 and repeat. If the button is not
*               pressed then keep compare value and wait for button aka semaphore.
*
*               Made in collaboration with Florian Gotschim.
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskLED_BLUE()' by 'OSTaskCreate()'.
*
* Return(s)   : none
*
* Note(s)     : none
*********************************************************************************************************
*/


static  void  App_TaskLED_BLUE (void *p_arg)
{
  /* declare and define task local variables */
  OS_ERR       os_err;
  CPU_TS         ts;
    
  CPU_INT08U   duty_cycle = 0;
  static CPU_BOOLEAN end_duty_cycle = 0; //start at false
  PWM_rgb_blue_WriteCompare(PWM_CMP_MAX_VAL); //defined in bsp_pwm_led.h
  
  /* prevent compiler warnings */
  (void)p_arg;
  
  /* start of the endless loop */
  while (DEF_TRUE) {
    OSSemPend(&button_sem, 0, OS_OPT_PEND_BLOCKING, &ts, &os_err);
    PWM_rgb_blue_Start();
    
    duty_cycle = PWM_rgb_blue_ReadCompare();
    if(duty_cycle == PWM_CMP_MAX_VAL){
        end_duty_cycle = 1;
    } else if(duty_cycle == PWM_CMP_MIN_VAL){
        end_duty_cycle = 0;
    }
    
    if(end_duty_cycle){ //reached max value
        PWM_rgb_blue_WriteCompare(duty_cycle -= 1);
    } else { //reset CMP
        PWM_rgb_blue_WriteCompare(PWM_CMP_MAX_VAL);
    }

    /* initiate scheduler */
    //every 20ms
    OSTimeDlyHMSM(0, 0, 0, 20, 
                  OS_OPT_TIME_HMSM_STRICT, 
                  &os_err);

  }
}

/*
*********************************************************************************************************
*                                          App_TaskLED_GREEN()
*
* Description : Task for green rgb led. Controlled by PWM_rgb_blue. Waits for semaphore button_sem and then
*               proceeds according to assignment description. Compare value starts at 0 and increases by
*               1 every 20ms. If 255 is reached then jump back to 0 and repeat. If the button is not
*               pressed then keep compare value and wait for button aka semaphore.
*
*               Made in collaboration with Florian Gotschim.
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskLED_GREEN()' by 'OSTaskCreate()'.
*
* Return(s)   : none
*
* Note(s)     : none
*********************************************************************************************************
*/


static  void  App_TaskLED_GREEN (void *p_arg)
{
  /* declare and define task local variables */
  OS_ERR       os_err;
  CPU_TS         ts;
  
  CPU_INT08U   duty_cycle = 0;
  static CPU_BOOLEAN end_duty_cycle = 0; //start at false
  PWM_rgb_green_WriteCompare(PWM_CMP_MIN_VAL); //defined in bsp_pwm_led.h
    
  /* prevent compiler warnings */
  (void)p_arg;
  
  /* start of the endless loop */
  while (DEF_TRUE) {
    OSSemPend(&button_sem, 0, OS_OPT_PEND_BLOCKING, &ts, &os_err);
    
    PWM_rgb_green_Start();
    duty_cycle = PWM_rgb_green_ReadCompare();
    if(duty_cycle == PWM_CMP_MAX_VAL){
        end_duty_cycle = 1;
    } else if(duty_cycle == PWM_CMP_MIN_VAL){
        end_duty_cycle = 0;
    }
    
    if(end_duty_cycle){ //reset CMP
        PWM_rgb_green_WriteCompare(PWM_CMP_MIN_VAL);
    } else { //reached min value
        PWM_rgb_green_WriteCompare(duty_cycle += 1);
    }
    
    /* initiate scheduler */
    OSTimeDlyHMSM(0, 0, 0, 20, 
                  OS_OPT_TIME_HMSM_STRICT, 
                  &os_err);

  }
}

/*
*********************************************************************************************************
*                                          App_TaskLED_RED()
*
* Description : Task for blue rgb led. Controlled by PWM_rgb_blue. Waits for semaphore button_sem and then
*               proceeds according to assignment description. Compare value starts at 1 per PWM TopDesign
*               configuration. If the button is pressed and the semaphore is available then set the compare
*               to 255 aka max value for full brightness. If the button is released and the semaphore is
*               hereby not available then the compare value does not get increased and neither does the PWM
*               get started.
*
*               Made in collaboration with Florian Gotschim.
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskLED_RED()' by 'OSTaskCreate()'.
*
* Return(s)   : none
*
* Note(s)     : none
*********************************************************************************************************
*/


static  void  App_TaskLED_RED (void *p_arg)
{
  /* declare and define task local variables */
  OS_ERR       os_err;
  CPU_TS         ts;
  
  /* prevent compiler warnings */
  (void)p_arg;
  
  /* start of the endless loop */
  while (DEF_TRUE) {
    OSSemPend(&button_sem, 0, OS_OPT_PEND_BLOCKING, &ts, &os_err);
    
    PWM_rgb_red_WriteCompare(PWM_CMP_MAX_VAL); //defined in PWM_rgb_led.h
    PWM_rgb_red_Start();
    
    /* initiate scheduler */
    OSTimeDlyHMSM(0, 0, 0, 20, 
                  OS_OPT_TIME_HMSM_STRICT, 
                  &os_err);

  }
}

/* END OF FILE */