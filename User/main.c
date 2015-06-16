#include  <includes.h>

//
//              .======.
//              | INRI |
//              |      |
//              |      |
//     .========'      '========.
//     |   _      xxxx      _   |
//     |  /_;-.__ / _\  _.-;_\  |
//     |     `-._`'`_/'`.-'     |
//     '========.`\   /`========'
//              | |  / |
//              |/-.(  |
//              |\_._\ |
//              | \ \`;|
//              |  > |/|
//              | / // |
//              | |//  |
//              | \(\  |
//              |  ``  |
//              |      |
//              |      |
//              |      |
//              |      |
//  \\    _  _\\| \//  |//_   _ \// _
// ^ `^`^ ^`` `^ ^` ``^^`  `^^` `^ `^
//     Ү�ձ���           ����BUG

/*
*********************************************************************************************************
*                                       GLOBAL TIMER 
*********************************************************************************************************
*/
OS_TMR  SPI1_OT_TIMER;
OS_TMR  COM_OT_TIMER;
OS_TMR  GPS_OT_TIMER;
OS_TMR  MTQ_SW_TIMER;
/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/
                                                               
static  OS_TCB   AppTaskStartTCB;
static  CPU_STK  AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];

static  OS_TCB   GPS_STO_TASK_TCB;
static  CPU_STK  GPS_STO_TASK_STK[GPS_STO_TASK_STK_SIZE];

static  OS_TCB   GPS_REV_TASK_TCB;
static  CPU_STK  GPS_REV_TASK_STK[GPS_REV_TASK_STK_SIZE];

static  OS_TCB   TEL_SAM_TASK_TCB;
static  CPU_STK  TEL_SAM_TASK_STK[TEL_SAM_TASK_STK_SIZE];

static  OS_TCB   TEL_STO_TASK_TCB;
static  CPU_STK  TEL_STO_TASK_STK[TEL_STO_TASK_STK_SIZE];

static  OS_TCB   PLOAD_SAM_TASK_TCB;
static  CPU_STK  PLOAD_SAM_TASK_STK[PLOAD_SAM_TASK_STK_SIZE];

static  OS_TCB   PLOAD_STO_TASK_TCB;
static  CPU_STK  PLOAD_STO_TASK_STK[PLOAD_STO_TASK_STK_SIZE];

static  OS_TCB   STO_UPDATE_TASK_TCB;
static  CPU_STK  STO_UPDATE_TASK_STK[STO_UPDATE_TASK_STK_SIZE];

static  OS_TCB   STO_STO_MONITOR_TASK_TCB;
static  CPU_STK  STO_SOR_MONITORTASK_STK[STO_SOR_MONITOR_TASK_STK_SIZE];

static  OS_TCB   FAULT_TASK_TCB;
static  CPU_STK  FAULT_TASK_STK[FAULT_TASK_STK_SIZE];

static  OS_TCB   TEST_TASK_TCB;
static  CPU_STK  TEST_TASK_STK[TEST_TASK_STK_SIZE];

static  OS_TCB   OUTPUT_TASK_TCB;
static  CPU_STK  OUTPUT_TASK_STK[OUTPUT_TASK_STK_SIZE];

static  OS_TCB   ADC_SAMPLE_TASK_TCB;
static  CPU_STK  ADC_SAMPLE_TASK_STK[ADC_SAMPLE_TASK_STK_SIZE];

static  OS_TCB   AppTaskSenGetTCB;
static  CPU_STK  AppTaskSenGetStk[2048];

static  OS_TCB   AppTaskMagDotDmpTCB;
static  CPU_STK  AppTaskMagDotDmpStk[1024];

static  OS_TCB   AppTaskPitFltComTCB;
static  CPU_STK  AppTaskPitFltComStk[1024];

static  OS_TCB   AppTaskAttStaCtlTCB;
static  CPU_STK  AppTaskAttStaCtlStk[1024];
 
OS_SEM   		SEM_GPS_STO;	   
OS_SEM   		SEM_TEL_STO;	   
OS_SEM   		SEM_PLOAD_STO;	  
BSP_OS_MUTEX 	MUTEX_MAG_CAP;
/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

/* �������� */
static  void  AppTaskStart          (void *p_arg);
static  void  AppTaskCreate         (void);
static  void  AppObjCreate          (void);
static  void  AppTimerCreate          (void);

/* ��ʱ���� */
void ComOT_CallBack (OS_TMR *p_tmr, void *p_arg);
void GPSOT_CallBack (OS_TMR *p_tmr, void *p_arg);
void MtqSwitch_CallBack(OS_TMR *p_tmr, void *p_arg);

/* �������� */
extern void TEL_SAM_TASK(void *p_arg);
extern void TEL_STO_TASK(void *p_arg);
extern void PLOAD_SAM_TASK(void *p_arg);
extern void PLOAD_STO_TASK(void *p_arg);
extern void GPS_STO_TASK(void *p_arg);
extern void GPS_REV_TASK(void *p_arg);
extern void FAULT_TASK(void *p_arg);
extern void STO_UPDATE_TASK(void *p_arg);
extern void TEST_TASK(void *p_arg);
extern void STO_MONITOR_TASK(void *p_arg);
extern void OUTPUT_TASK(void *p_arg);
extern void ADC_SAMPLE_TASK(void *p_arg);



/*
*********************************************************************************************************
*	�� �� ��: main
*	����˵��: ��׼c������ڡ�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/

int main(void)
{
    OS_ERR  err;
    OSInit(&err);                                               /* Init uC/OS-III.                                      */
	/* ʹ����ת���� ����Ĭ�ϵ�ȫ��ʱ��Ƭ��8 */
    OSSchedRoundRobinCfg(DEF_ENABLED,
						 8,
						 &err);	

	OSTaskCreate((OS_TCB       *)&AppTaskStartTCB,              /* Create the start task                                */
                 (CPU_CHAR     *)"App Task Start",
                 (OS_TASK_PTR   )AppTaskStart, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_START_PRIO,
                 (CPU_STK      *)&AppTaskStartStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_START_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_START_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);

    OSStart(&err);                                              /* Start multitasking (i.e. give control to uC/OS-III). */
    
    (void)&err;
    
    return (0);
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskStart
*	����˵��: ����һ�����������ڶ�����ϵͳ�����󣬱����ʼ���δ������(��BSP_Init��ʵ��)
*	��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
	�� �� ����2
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{
	OS_ERR      err;

   (void)p_arg;

	BSP_Tick_Init();
	
 	bsp_Init();
	CPU_Init();
	

	//Mem_Init();  //�ڴ����ģ��                           
//  Math_Init();                         

#if OS_CFG_STAT_TASK_EN > 0u
     OSStatTaskCPUUsageInit(&err);   
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif
	
	AppTimerCreate();
    AppObjCreate();                                            
    AppTaskCreate();                                           
    while (1)
	  {                                         
		//GPIOG->ODR ^= GPIO_Pin_10;     //���LEDָʾ�ƣ�ָʾ������������״̬ 2014/10/18
		BSP_OS_TimeDlyMs(2000);
		//printf("living!\r\n");
		//bsp_LedToggle(1);
		//bsp_LedToggle(2);
		//bsp_LedToggle(3);
    }
}

static  void  AppTaskCreate (void)
{
	OS_ERR      err;
	
	/**************����UPDATE����*********************/
	/**************����GPS�洢����******************/
	OSTaskCreate((OS_TCB       *)&GPS_STO_TASK_TCB,             
                 (CPU_CHAR     *)"GPS_STO_TASK",
                 (OS_TASK_PTR   )GPS_STO_TASK, 
                 (void         *)0,
                 (OS_PRIO       )GPS_STO_TASK_PRIO,
                 (CPU_STK      *)&GPS_STO_TASK_STK[0],
                 (CPU_STK_SIZE  )GPS_STO_TASK_STK_SIZE / 10,
                 (CPU_STK_SIZE  )GPS_STO_TASK_STK_SIZE,
                 (OS_MSG_QTY    )1,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);

	/**************����GPS��������*********************/
	OSTaskCreate((OS_TCB       *)&GPS_REV_TASK_TCB,            
                 (CPU_CHAR     *)"GPS_REV_TASK",
                 (OS_TASK_PTR   )GPS_REV_TASK, 
                 (void         *)0,
                 (OS_PRIO       )GPS_REV_TASK_PRIO,
                 (CPU_STK      *)&GPS_REV_TASK_STK[0],
                 (CPU_STK_SIZE  )GPS_REV_TASK_STK_SIZE / 10,
                 (CPU_STK_SIZE  )GPS_REV_TASK_STK_SIZE,
                 (OS_MSG_QTY    )2,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);
	
	/**************����ң��ɼ�����*********************/
	OSTaskCreate((OS_TCB       *)&TEL_SAM_TASK_TCB,             
                 (CPU_CHAR     *)"TEL_SAM_TASK",
                 (OS_TASK_PTR   )TEL_SAM_TASK, 
                 (void         *)0,
                 (OS_PRIO       )TEL_SAM_TASK_PRIO,
                 (CPU_STK      *)&TEL_SAM_TASK_STK[0],
                 (CPU_STK_SIZE  )TEL_SAM_TASK_STK_SIZE / 10,
                 (CPU_STK_SIZE  )TEL_SAM_TASK_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);

	/**************����ң��洢����*********************/			 
	OSTaskCreate((OS_TCB       *)&TEL_STO_TASK_TCB,              
                 (CPU_CHAR     *)"TEL_STO_TASK",
                 (OS_TASK_PTR   )TEL_STO_TASK, 
                 (void         *)0,
                 (OS_PRIO       )TEL_STO_TASK_PRIO,
                 (CPU_STK      *)&TEL_STO_TASK_STK[0],
                 (CPU_STK_SIZE  )TEL_STO_TASK_STK_SIZE / 10,
                 (CPU_STK_SIZE  )TEL_STO_TASK_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )8,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);
	
	/**************�����غɲɼ�����*********************/			 
	OSTaskCreate((OS_TCB       *)&PLOAD_SAM_TASK_TCB,              
                 (CPU_CHAR     *)"PLOAD_SAM_TASK",
                 (OS_TASK_PTR   )PLOAD_SAM_TASK, 
                 (void         *)0,
                 (OS_PRIO       )PLOAD_SAM_TASK_PRIO,
                 (CPU_STK      *)&PLOAD_SAM_TASK_STK[0],
                 (CPU_STK_SIZE  )PLOAD_SAM_TASK_STK_SIZE / 10,
                 (CPU_STK_SIZE  )PLOAD_SAM_TASK_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )8,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);
				 
	/**************�����غɴ洢����*********************/			 
	OSTaskCreate((OS_TCB       *)&PLOAD_STO_TASK_TCB,              
                 (CPU_CHAR     *)"PLOAD_STO_TASK",
                 (OS_TASK_PTR   )PLOAD_STO_TASK, 
                 (void         *)0,
                 (OS_PRIO       )PLOAD_STO_TASK_PRIO,
                 (CPU_STK      *)&PLOAD_STO_TASK_STK[0],
                 (CPU_STK_SIZE  )PLOAD_STO_TASK_STK_SIZE / 10,
                 (CPU_STK_SIZE  )PLOAD_STO_TASK_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )8,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);
								 
	/**************�����洢��ά������*******************/									 
		OSTaskCreate((OS_TCB       *)&STO_UPDATE_TASK_TCB,              
                 (CPU_CHAR     *)"STO_UPDATE_TASK",
                 (OS_TASK_PTR   )STO_UPDATE_TASK, 
                 (void         *)0,
                 (OS_PRIO       )STO_UPDATE_TASK_PRIO,
                 (CPU_STK      *)&STO_UPDATE_TASK_STK[0],
                 (CPU_STK_SIZE  )STO_UPDATE_TASK_STK_SIZE / 10,
                 (CPU_STK_SIZE  )STO_UPDATE_TASK_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )8,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);		
								 
	/**************����NORFLASH�������***************/									 
		OSTaskCreate((OS_TCB       *)&STO_STO_MONITOR_TASK_TCB,              
                 (CPU_CHAR     *)"STO_MONITOR_TASK",
                 (OS_TASK_PTR   )STO_MONITOR_TASK,
                 (void         *)0,
                 (OS_PRIO       )STO_MONITOR_TASK_PRIO,
                 (CPU_STK      *)&STO_SOR_MONITORTASK_STK[0],
                 (CPU_STK_SIZE  )STO_SOR_MONITOR_TASK_STK_SIZE / 10,
                 (CPU_STK_SIZE  )STO_SOR_MONITOR_TASK_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )8,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);	
								 
	/**************�������ϼ������*******************/									 
		OSTaskCreate((OS_TCB       *)&FAULT_TASK_TCB,              
                 (CPU_CHAR     *)"FAULT_TASK",
                 (OS_TASK_PTR   )FAULT_TASK, 
                 (void         *)0,
                 (OS_PRIO       )FAULT_TASK_PRIO,
                 (CPU_STK      *)&FAULT_TASK_STK[0],
                 (CPU_STK_SIZE  )FAULT_TASK_STK_SIZE / 10,
                 (CPU_STK_SIZE  )FAULT_TASK_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )8,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);		
								 
	/**************�����������*******************/		
		OSTaskCreate((OS_TCB       *)&OUTPUT_TASK_TCB,              
                 (CPU_CHAR     *)"OUTPUT_TASK",
                 (OS_TASK_PTR   )OUTPUT_TASK, 
                 (void         *)0,
                 (OS_PRIO       )OUTPUT_TASK_PRIO,
                 (CPU_STK      *)&OUTPUT_TASK_STK[0],
                 (CPU_STK_SIZE  )OUTPUT_TASK_STK_SIZE / 10,
                 (CPU_STK_SIZE  )OUTPUT_TASK_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )8,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);		

	/**************������Դ��������*******************/		
		OSTaskCreate((OS_TCB       *)&ADC_SAMPLE_TASK_TCB,              
                 (CPU_CHAR     *)"ADC_SAMPLE_TASK",
                 (OS_TASK_PTR   )ADC_SAMPLE_TASK, 
                 (void         *)0,
                 (OS_PRIO       )ADC_SAMPLE_TASK_PRIO,
                 (CPU_STK      *)&ADC_SAMPLE_TASK_STK[0],
                 (CPU_STK_SIZE  )ADC_SAMPLE_TASK_STK_SIZE / 10,
                 (CPU_STK_SIZE  )ADC_SAMPLE_TASK_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )8,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);		
								 
	/**************��������������*******************/									 
		OSTaskCreate((OS_TCB       *)&TEST_TASK_TCB,              
                 (CPU_CHAR     *)"TEST_TASK",
                 (OS_TASK_PTR   )TEST_TASK, 
                 (void         *)0,
                 (OS_PRIO       )TEST_TASK_PRIO,
                 (CPU_STK      *)&TEST_TASK_STK[0],
                 (CPU_STK_SIZE  )TEST_TASK_STK_SIZE / 10,
                 (CPU_STK_SIZE  )TEST_TASK_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )8,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);	
	
	
  /*****************************************************************/
							 
	OSTaskCreate((OS_TCB       *)&AppTaskSenGetTCB,
               (CPU_CHAR     *)"App Task SenGet",
               (OS_TASK_PTR   )AppTaskSenGet,
               (void         *)0,
               (OS_PRIO       )APP_TASK_SENGET_PRIO,
               (CPU_STK      *)&AppTaskSenGetStk[0],
               (CPU_STK_SIZE  )1024 / 10,
               (CPU_STK_SIZE  )APP_TASK_SENGET_STK_SIZE,
               (OS_MSG_QTY    )0,
               (OS_TICK       )0,
               (void         *)0,
               (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
               (OS_ERR       *)&err);
							 
	OSTaskCreate((OS_TCB       *)&AppTaskMagDotDmpTCB,
               (CPU_CHAR     *)"App Task MagDotDmp",
               (OS_TASK_PTR   )AppTaskMagDotDmp,
               (void         *)0,
               (OS_PRIO       )APP_TASK_MAGDOTDMP_PRIO,
               (CPU_STK      *)&AppTaskMagDotDmpStk[0],
               (CPU_STK_SIZE  )1024 / 10,
               (CPU_STK_SIZE  )APP_TASK_MAGDOTDMP_STK_SIZE,
               (OS_MSG_QTY    )0, 
               (OS_TICK       )0,
               (void         *)0,
               (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
               (OS_ERR       *)&err);
							 
	OSTaskCreate((OS_TCB       *)&AppTaskPitFltComTCB,
               (CPU_CHAR     *)"App Task PitFltCom",
               (OS_TASK_PTR   )AppTaskPitFltCom,
               (void         *)0,
               (OS_PRIO       )APP_TASK_PITFLTCOM_PRIO,
               (CPU_STK      *)&AppTaskPitFltComStk[0], 
               (CPU_STK_SIZE  )1024 / 10,                  
               (CPU_STK_SIZE  )APP_TASK_PITFLTCOM_STK_SIZE, 
               (OS_MSG_QTY    )0,  //0
               (OS_TICK       )0,  //0
               (void         *)0,  //0
               (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
               (OS_ERR       *)&err);
							 
	OSTaskCreate((OS_TCB       *)&AppTaskAttStaCtlTCB,
               (CPU_CHAR     *)"App Task AttStaCtl",
               (OS_TASK_PTR   )AppTaskAttStaCtl,
               (void         *)0,
               (OS_PRIO       )APP_TASK_ATTSTACTL_PRIO,
               (CPU_STK      *)&AppTaskAttStaCtlStk[0],
               (CPU_STK_SIZE  )1024 / 10,
               (CPU_STK_SIZE  )APP_TASK_ATTSTACTL_STK_SIZE,
               (OS_MSG_QTY    )0,
               (OS_TICK       )0,
               (void         *)0,
               (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
               (OS_ERR       *)&err);						 
}

/*
*********************************************************************************************************
*                                          AppObjCreate()
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
static  void  AppObjCreate (void)
{
	/* ���������ź��� */
	
	BSP_OS_SemCreate(&SEM_GPS_STO,
					0,	
					(CPU_CHAR *)"SEM_GPS_STO");     /* GPS�洢�źŵ� */
	
	BSP_OS_SemCreate(&SEM_TEL_STO,
					0,	
					(CPU_CHAR *)"SEM_TEL_STO");     /* ң��洢�źŵ� */
	
	BSP_OS_SemCreate(&SEM_PLOAD_STO,
					0,	
					(CPU_CHAR *)"SEM_PLOAD_STO");   /* �غɴ洢�źŵ� */
	
	//BSP_OS_MutexCreate (&MUTEX_MAG_CAP,
      //                  (CPU_CHAR *)"MUTEX_MAG_CAP");
/********************�˿��źŵ�*********************/
	
	BSP_OS_SemCreate(&SEM_CYC_INFO,
					0,	
					(CPU_CHAR *)"SEM_CYC_INFO");    /* �źŲɼ��źŵ� */
	
	BSP_OS_SemCreate(&SEM_MAG_DOT_DMP,
					0,	
					(CPU_CHAR *)"SEM_MAG_DOT_DMP");  /* �����źŵ� */
	
	BSP_OS_SemCreate(&SEM_PIT_FLT_COM,
					0,	
					(CPU_CHAR *)"SEM_PIT_FLT_COM");  /* �˲�״̬�����źŵ� */
	
	
	BSP_OS_SemCreate(&SEM_ATT_STA_CTL,
					0,	
					(CPU_CHAR *)"SEM_ATT_STA_CTL");  /* �����ȶ��źŵ� */
}

static  void  AppTimerCreate (void)
{
	OS_ERR err;
	/* ����SPI1��ʱ��ʱ�� */
	OSTmrCreate((OS_TMR            *) &SPI1_OT_TIMER,
							(CPU_CHAR          *) "EPS Over Timer",
							(OS_TICK            ) 0,
							(OS_TICK            ) 1,
							(OS_OPT             ) OS_OPT_TMR_ONE_SHOT,
							(OS_TMR_CALLBACK_PTR) 0,
							(void              *) 0,
							(OS_ERR            *) &err);
							
	/* ����ͨ��1��ʱ��ʱ�� */						
	OSTmrCreate((OS_TMR            *) &COM_OT_TIMER,
							(CPU_CHAR          *) "COM Over Timer",
							(OS_TICK            ) 2*10,
							(OS_TICK            ) 0,
							(OS_OPT             ) OS_OPT_TMR_ONE_SHOT,
							(OS_TMR_CALLBACK_PTR) ComOT_CallBack,
							(void              *) 0,
							(OS_ERR            *) &err);
							
	OSTmrCreate((OS_TMR            *) &MTQ_SW_TIMER,
							(CPU_CHAR          *) "Mtq Switch Timer",
							(OS_TICK            ) 18,
							(OS_TICK            ) 0,
							(OS_OPT             ) OS_OPT_TMR_ONE_SHOT,
							(OS_TMR_CALLBACK_PTR) MtqSwitch_CallBack,
							(void              *) 0,
							(OS_ERR            *) &err);			
	/* ����GPS���ճ�ʱ��ʱ�� */								
	OSTmrCreate((OS_TMR            *) &GPS_OT_TIMER,
							(CPU_CHAR          *) "GPS Over Timer",
							(OS_TICK            ) 2*10,      /* 2s�ӳ� */
							(OS_TICK            ) 0,
							(OS_OPT             ) OS_OPT_TMR_ONE_SHOT,
							(OS_TMR_CALLBACK_PTR) GPSOT_CallBack,
							(void              *) 0,
							(OS_ERR            *) &err);				
							
}
