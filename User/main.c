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
//     耶稣保佑           永无BUG

/*
*********************************************************************************************************
*                                       GLOBAL TIMER 
*********************************************************************************************************
*/
OS_TMR  SPI1_OT_TIMER;
OS_TMR  COM_OT_TIMER;
OS_TMR  GPS_OT_TIMER;
OS_TMR  TEST_OT_TIMER;

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

static  OS_TCB   AppTaskOrbDetTCB;
static  CPU_STK  AppTaskOrbDetStk[1024];

static  OS_TCB   AppTaskCalcuOrbTCB;
static  CPU_STK  AppTaskCalcuOrbStk[1024];

static  OS_TCB   AppTaskSenGetTCB;
static  CPU_STK  AppTaskSenGetStk[1024];

static  OS_TCB   AppTaskMagDotDmpTCB;
static  CPU_STK  AppTaskMagDotDmpStk[1024];

static  OS_TCB   AppTaskPitFltComTCB;
static  CPU_STK  AppTaskPitFltComStk[1024];

static  OS_TCB   AppTaskPitFltProTCB;
static  CPU_STK  AppTaskPitFltProStk[1024];

static  OS_TCB   AppTaskAttStaCtlTCB;
static  CPU_STK  AppTaskAttStaCtlStk[1024];
OS_SEM   SEM_TEST_REV;	   
OS_SEM   SEM_GPS_STO;	   
OS_SEM   SEM_TEL_STO;	   
OS_SEM   SEM_PLOAD_STO;	   
/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

/* 建立函数 */
static  void  AppTaskStart          (void *p_arg);
static  void  AppTaskCreate         (void);
static  void  AppObjCreate          (void);
static  void  AppTimerCreate          (void);

/* 超时函数 */
static void ComOT_CallBack (OS_TMR *p_tmr, void *p_arg);
extern void GPSOT_CallBack (OS_TMR *p_tmr, void *p_arg);


/* 声明函数 */
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
*	函 数 名: main
*	功能说明: 标准c程序入口。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/

int main(void)
{
    OS_ERR  err;
    OSInit(&err);                                               /* Init uC/OS-III.                                      */
	/* 使能轮转调度 设置默认的全局时间片是8 */
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
*	函 数 名: AppTaskStart
*	功能说明: 这是一个启动任务，在多任务系统启动后，必须初始化滴答计数器(在BSP_Init中实现)
*	形    参：p_arg 是在创建该任务时传递的形参
*	返 回 值: 无
	优 先 级：2
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{
	OS_ERR      err;

   (void)p_arg;

 	bsp_Init();
	CPU_Init();
	BSP_Tick_Init();

	//Mem_Init();  //内存管理模块                           
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
		//GPIOG->ODR ^= GPIO_Pin_10;     //添加LED指示灯，指示星务计算机工作状态 2014/10/18
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
	
	/**************创建UPDATE任务*********************/
	/**************创建GPS存储任务******************/
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

	/**************创建GPS接收任务*********************/
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
	
	/**************创建遥测采集任务*********************/
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

	/**************创建遥测存储任务*********************/			 
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
	
	/**************创建载荷采集任务*********************/			 
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
				 
	/**************创建载荷存储任务*********************/			 
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
								 
	/**************创建存储量维护任务*******************/									 
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
								 
	/**************创建NORFLASH清除任务***************/									 
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
								 
	/**************创建故障检测任务*******************/									 
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
								 
	/**************创建输出任务*******************/		
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

	/**************创建电源监视任务*******************/		
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
								 
	/**************创建地面电测任务*******************/									 
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
	OSTaskCreate((OS_TCB       *)&AppTaskOrbDetTCB,
               (CPU_CHAR     *)"App Task OrbDet",    
               (OS_TASK_PTR   )AppTaskOrbDet,     
               (void         *)0,                          
               (OS_PRIO       )APP_TASK_ORBDET_PRIO, 
               (CPU_STK      *)&AppTaskOrbDetStk[0],
               (CPU_STK_SIZE  )1024 / 10,
               (CPU_STK_SIZE  )APP_TASK_ORBDET_STK_SIZE,
               (OS_MSG_QTY    )0,
               (OS_TICK       )0,
               (void         *)0,
               (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
               (OS_ERR       *)&err);	
							 
	OSTaskCreate((OS_TCB       *)&AppTaskCalcuOrbTCB,
               (CPU_CHAR     *)"App Task CalcuOrb",
               (OS_TASK_PTR   )AppTaskCalcuOrb,
               (void         *)0,
               (OS_PRIO       )APP_TASK_CALCUORB_PRIO,
               (CPU_STK      *)&AppTaskCalcuOrbStk[0],
               (CPU_STK_SIZE  )1024 / 10,
               (CPU_STK_SIZE  )APP_TASK_CALCUORB_STK_SIZE,
               (OS_MSG_QTY    )0,
               (OS_TICK       )0, 
               (void         *)0,
               (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
               (OS_ERR       *)&err);
							 
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
							 
	OSTaskCreate((OS_TCB       *)&AppTaskPitFltProTCB, 
               (CPU_CHAR     *)"App Task PitFltPro",    
               (OS_TASK_PTR   )AppTaskPitFltPro,          
               (void         *)0,                        
               (OS_PRIO       )APP_TASK_PITFLTPRO_PRIO, 
               (CPU_STK      *)&AppTaskPitFltProStk[0],
               (CPU_STK_SIZE  )1024 / 10,
               (CPU_STK_SIZE  )APP_TASK_PITFLTPRO_STK_SIZE,
               (OS_MSG_QTY    )0,
               (OS_TICK       )0,
               (void         *)0,
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
	/* 创建互斥信号量 */
	BSP_OS_SemCreate(&SEM_TEST_REV,
					 0,	
					 (CPU_CHAR *)"SEM_TEXT_REV");   /* 地面测试信号灯 */
	
  BSP_OS_SemCreate(&SEM_GPS_STO,
					0,	
					(CPU_CHAR *)"SEM_GPS_STO");     /* GPS存储信号灯 */
	
	BSP_OS_SemCreate(&SEM_TEL_STO,
					0,	
					(CPU_CHAR *)"SEM_TEL_STO");     /* 遥测存储信号灯 */
	
	BSP_OS_SemCreate(&SEM_PLOAD_STO,
					0,	
					(CPU_CHAR *)"SEM_PLOAD_STO");   /* 载荷存储信号灯 */
	
/********************姿控信号灯*********************/
	BSP_OS_SemCreate(&SEM_ORB_DET,
					0,	
					(CPU_CHAR *)"SEM_ORB_DET");     /* GPS测量信号灯 */
	
	BSP_OS_SemCreate(&SEM_ORB_CALCU,
					0,	
					(CPU_CHAR *)"SEM_ORB_CALCU");   /* 轨道预报信号灯 */
	
	BSP_OS_SemCreate(&SEM_CYC_INFO,
					0,	
					(CPU_CHAR *)"SEM_CYC_INFO");    /* 信号采集信号灯 */
	
	BSP_OS_SemCreate(&SEM_MAG_DOT_DMP,
					0,	
					(CPU_CHAR *)"SEM_MAG_DOT_DMP");  /* 阻尼信号灯 */
	
	BSP_OS_SemCreate(&SEM_PIT_FLT_COM,
					0,	
					(CPU_CHAR *)"SEM_PIT_FLT_COM");  /* 滤波状态更新信号灯 */
	
	BSP_OS_SemCreate(&SEM_PIT_FLT_PRO,
					0,	
					(CPU_CHAR *)"SEM_PIT_FLT_PRO");  /* 滤波时间更新信号灯 */
	
	BSP_OS_SemCreate(&SEM_ATT_STA_CTL,
					0,	
					(CPU_CHAR *)"SEM_ATT_STA_CTL");  /* 三轴稳定信号灯 */
}

static  void  AppTimerCreate (void)
{
	OS_ERR err;
	/* 建立SPI1超时定时器 */
	OSTmrCreate((OS_TMR            *) &SPI1_OT_TIMER,
							(CPU_CHAR          *) "EPS Over Timer",
							(OS_TICK            ) 0,
							(OS_TICK            ) 1,
							(OS_OPT             ) OS_OPT_TMR_ONE_SHOT,
							(OS_TMR_CALLBACK_PTR) 0,
							(void              *) 0,
							(OS_ERR            *) &err);
							
	/* 建立通信1超时定时器 */						
	OSTmrCreate((OS_TMR            *) &COM_OT_TIMER,
							(CPU_CHAR          *) "COM Over Timer",
							(OS_TICK            ) 0,
							(OS_TICK            ) 0,
							(OS_OPT             ) OS_OPT_TMR_ONE_SHOT,
							(OS_TMR_CALLBACK_PTR) ComOT_CallBack,
							(void              *) 0,
							(OS_ERR            *) &err);
				
	/* 建立GPS接收超时定时器 */								
	OSTmrCreate((OS_TMR            *) &GPS_OT_TIMER,
							(CPU_CHAR          *) "GPS Over Timer",
							(OS_TICK            ) 2*10,      /* 2s延迟 */
							(OS_TICK            ) 0,
							(OS_OPT             ) OS_OPT_TMR_ONE_SHOT,
							(OS_TMR_CALLBACK_PTR) GPSOT_CallBack,
							(void              *) 0,
							(OS_ERR            *) &err);				
							
}

static void ComOT_CallBack (OS_TMR *p_tmr, void *p_arg)
{
	//ComRevOTCnt++;
}
