
#ifndef  APP_CFG_MODULE_PRESENT
#define  APP_CFG_MODULE_PRESENT

/*
*********************************************************************************************************
*                                       MODULE ENABLE / DISABLE
*********************************************************************************************************
*/

#define  APP_CFG_SERIAL_EN                            DEF_DISABLED

/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/

#define  APP_CFG_TASK_START_PRIO                              10u
#define  GPS_STO_TASK_PRIO                                    3u
#define  GPS_REV_TASK_PRIO                                    4u
#define  TEL_SAM_TASK_PRIO                                    5u
#define  TEL_STO_TASK_PRIO                        					  6u
#define  PLOAD_SAM_TASK_PRIO                 									7u
#define  PLOAD_STO_TASK_PRIO                 									8u
#define  STO_UPDATE_TASK_PRIO                 		  					9u
#define  FAULT_TASK_PRIO                 		  				      	2u
#define  TEST_TASK_PRIO                 		  				      	11u
#define  STO_MONITOR_TASK_PRIO                                12u
#define  OUTPUT_TASK_PRIO                 		  				     	2u
#define  ADC_SAMPLE_TASK_PRIO                                 13u

#define  APP_TASK_ORBDET_PRIO                                 15u
#define  APP_TASK_ORBPRO_PRIO                                 16u
#define  APP_TASK_CALCUORB_PRIO                               17u
#define  APP_TASK_SENGET_PRIO                                 18u
#define  APP_TASK_MAGDOTDMP_PRIO                              19u
#define  APP_TASK_PITFLTCOM_PRIO                              20u
#define  APP_TASK_PITFLTPRO_PRIO                              21u
#define  APP_TASK_ATTSTACTL_PRIO                              22u
/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*                             Size of the task stacks (# of OS_STK entries)
*********************************************************************************************************
*/

#define  APP_CFG_TASK_START_STK_SIZE                      128u
#define  GPS_STO_TASK_STK_SIZE                            128u
#define  GPS_REV_TASK_STK_SIZE                            128u
#define  TEL_SAM_TASK_STK_SIZE                            128u
#define  TEL_STO_TASK_STK_SIZE                            512u
#define  PLOAD_SAM_TASK_STK_SIZE                          128u
#define  PLOAD_STO_TASK_STK_SIZE                          128u
#define  STO_UPDATE_TASK_STK_SIZE                 		  	128u
#define  FAULT_TASK_STK_SIZE                      		  	128u
#define  TEST_TASK_STK_SIZE                      		     	128u
#define  OUTPUT_TASK_STK_SIZE                      		   	128u
#define  ADC_SAMPLE_TASK_STK_SIZE                  		   	128u
#define  STO_SOR_MONITOR_TASK_STK_SIZE                    128u

#define  APP_TASK_ORBDET_STK_SIZE                         1024u
#define  APP_TASK_CALCUORB_STK_SIZE                       1024u
#define  APP_TASK_SENGET_STK_SIZE                         1024u
#define  APP_TASK_MAGDOTDMP_STK_SIZE                      1024u
#define  APP_TASK_PITFLTCOM_STK_SIZE                      1024u
#define  APP_TASK_PITFLTPRO_STK_SIZE                      1024u
#define  APP_TASK_ATTSTACTL_STK_SIZE                      1024u


/*
*********************************************************************************************************
*                                                 uC/SERIAL
*
* Note(s) : (1) Configure SERIAL_CFG_MAX_NBR_IF to the number of interfaces (i.e., UARTs) that will be
*               present.
*
*           (2) Configure SERIAL_CFG_RD_BUF_EN to enable/disable read buffer functionality.  The serial
*               core stores received data in the read buffer until the user requests it, providing a
*               reliable guarantee against receive overrun.
*
*           (3) Configure SERIAL_CFG_WR_BUF_EN to enable/disable write buffer functionality.  The serial
*               core stores line driver transmit data in the write buffer while the serial interface is
*               transmitting application data.
*
*           (4) Configure SERIAL_CFG_ARG_CHK_EXT_EN to enable/disable extended argument checking
*               functionality.
*
*           (5) Configure SERIAL_CFG_TX_DESC_NBR to allow multiple transmit operations (i.e., Serial_Wr,
*               Serial_WrAsync) to be queued.
*********************************************************************************************************
*/

#define  SERIAL_CFG_MAX_NBR_IF                          2u              /* See Note #1.                                         */

#define  SERIAL_CFG_RD_BUF_EN                           DEF_ENABLED     /* See Note #2.                                         */

#define  SERIAL_CFG_WR_BUF_EN                           DEF_ENABLED     /* See Note #3.                                         */

#define  SERIAL_CFG_ARG_CHK_EXT_EN                      DEF_DISABLED    /* See Note #4.                                         */

#define  SERIAL_CFG_TX_DESC_NBR                         1               /* See Note #5.                                         */

/*
*********************************************************************************************************
*                                    uC/SERIAL APPLICATION CONFIGURATION
*********************************************************************************************************
*/

#define  APP_SERIAL_CFG_TRACE_EN                        DEF_ENABLED
#define  APP_SERIAL_CFG_TRACE_PORT_NAME                 "USART3"


/*
*********************************************************************************************************
*                                     TRACE / DEBUG CONFIGURATION
*********************************************************************************************************
*/
#if 0
#define  TRACE_LEVEL_OFF                                0
#define  TRACE_LEVEL_INFO                               1
#define  TRACE_LEVEL_DBG                                2
#endif

#include <cpu.h>
void  App_SerPrintf  (CPU_CHAR *format, ...);

#define  APP_TRACE_LEVEL                                TRACE_LEVEL_DBG
#define  APP_TRACE                                      App_SerPrintf

#define  APP_TRACE_INFO(x)               ((APP_TRACE_LEVEL >= TRACE_LEVEL_INFO)  ? (void)(APP_TRACE x) : (void)0)
#define  APP_TRACE_DBG(x)                ((APP_TRACE_LEVEL >= TRACE_LEVEL_DBG)   ? (void)(APP_TRACE x) : (void)0)

#endif

