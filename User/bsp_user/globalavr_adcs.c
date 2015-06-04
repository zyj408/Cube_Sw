#include <includes.h>

/****************************** 姿控软件中含有的变量 ******************************/
/* 姿控信号灯 */
OS_SEM   SEM_CYC_INFO;
OS_SEM   SEM_MAG_DOT_DMP;
OS_SEM   SEM_PIT_FLT_COM;
OS_SEM   SEM_ATT_STA_CTL;

/* 姿控变量 */
/*---------------------------------------下行功能遥测-------------------------------------------*/
double     downAdcsOrb[6];                  /* 下行轨道数据 */           
double     downAdcsMtqV[3];                 /* 下行磁力矩器电压 */    
double     downAdcsMeasAng;                 /* 下行俯仰角测量值 */   
double     downAdcsFltAngRate[2];           /* 下行滤波后的俯仰角及角速度 */   
double     downAdcsMagTable[3];             /* 下行WGS84系下磁场值 */   
double     downAdcsMagnetometer[3];         /* 下行磁强计测量值 */

enum BOOL  downAdcsmagDotDmpFlg;            /* 下行阻尼标志位 */
int        downAdcscntDmpFlag;              /* 下行阻尼次数 */
enum BOOL  downAdcspitFltComFlg;            /* 下行测量标志位 */
int        downAdcscntPitcomFlag;           /* 下行测量次数 */
enum BOOL  downAdcsattStaFlg;               /* 下行三轴稳定控制标志位 */
int        downAdcscntAttStaFlag;           /* 下行三轴稳定控制次数 */

/*--------------------------------------上行指令----------------------------------------------- */
double     upDelta_TinSat;               /* 上行地面或GPS授时时间 */
enum BOOL  AdcsOrbFlg;                   /*轨道有效标志位*/
enum BOOL  upXwAdcsTLEFlag;              /* TLE轨道上注标志位，数据综合置位 */
enum BOOL  upAdcsTLEFlag;
double     upXwAdcsTLEBstar;
double     upXwAdcsTLEEcco;
double     upXwAdcsTLEInclo;
double     upXwAdcsTLEArgpo;
double     upXwAdcsTLEJdsatepoch;
double     upXwAdcsTLEMo;
double     upXwAdcsTLENo;
double     upXwAdcsTLENodeo;

double     upAdcsConP;                   /* 三轴稳定控制律P系数，姿控使用 */
double     upXwAdcsConP;                 /* 三轴稳定控制律P系数，数据综合放上行指令 */
enum BOOL  upXwAdcsConPFlag;             /* 三轴稳定控制律P系数上注标志位，数据综合置位 */

double     upAdcsConD;                   /* 三轴稳定控制律D系数，姿控使用 */
double     upXwAdcsConD;                 /* 三轴稳定控制律D系数，数据综合放上行指令 */
enum BOOL  upXwAdcsConDFlag;             /* 三轴稳定控制律D系数上注标志位，数据综合置位 */

double     upAdcsConZ;                   /* 章进动控制律Z系数，姿控使用 */
double     upXwAdcsConZ;                 /* 章进动控制律Z系数，数据综合放上行指令 */
enum BOOL  upXwAdcsConZFlag;             /* 章进动控制律Z系数，数据综合置位 */

enum BOOL  upXwAdcsReDmp;                /* 重新阻尼标志，姿控清零 */

enum BOOL  upXwAdcsDmpFlg;               /* 上行阻尼标志位，对应姿控magDotDmpFlg */
enum BOOL  upXwAdcsDmpFlgFlag;           /* 阻尼标志位，数据综合置位 */
 
enum BOOL  upXwAdcsFltFlg;               /* 上行俯仰滤波标志位，对应姿控pitFltComFlg */
enum BOOL  upXwAdcsFltFlgFlag;           /* 测量标志位，数据综合置位 */

enum BOOL  upXwAdcsCtlFlg;               /* 上行控制标志位，对应姿控attStaFlg */
enum BOOL  upXwAdcsCtlFlgFlag;           /* 控制标志位，数据综合置位 */

/*------------------------------------------------------------------------------------- */
double tInterval;			                    /* 轨道姿态传播的步长 */
double MagInO_YInc;
double MagInO_last[3];

double WGS84PV[6];                         /* 来源于GPS接收机 */
double orbInfo[6],orbInc[3],orbInfoGPS[6]; /* 轨道信息，轨道增量信息，GPS轨道信息*/
double magnetometer[3],MTQOut[3];          /* 磁强计量测值，磁力矩器输出磁矩 */
double mtqTq[3];			                     /* 磁力矩器控制力矩 */

enum BOOL  magDotDmpFlg;                   /* 阻尼标志位 */
enum BOOL  pitFltComFlg;                   /* 俯仰滤波标志位 */
enum BOOL  attStaFlg;                      /* 三轴稳定控制标志位 */
int cntDmpFlag;                            /* 阻尼次数 */
int cntPitcomFlag;                         /* 俯仰滤波次数 */
int cntAttStaFlag;                         /* 三轴稳定控制次数 */

double PFSt[2];		                        /* 俯仰滤波器状态与协方差阵 */
double PPF[2][2];
double PFB[2]; 			                      /* 俯仰滤波器中用的参量 */
double RPF;                                /* 量测噪声协方差*/
double QPF;                                /* 系统过程噪声协方差*/

elsetrec satrec;
int CntSGP4;
enum BOOL  AdcsGpsUse;
enum BOOL  AdcsOrbGPSFlag;
elsetrec satrecFromGPS;
int CntNoGPS;

enum BOOL  updateTimeFlag;                 /* 授时标志位 */
double TinSat0;                            /* 统一初始时间，儒略格式 */
double TinSat;			                   /* 星上实时时间，儒略格式 */
uint32_t adcs_timer;                       /* 星上计时器 */
/*------------------------------------------------------------------------------------- */


void ADCS_Init_VAR(void)
{
	int i;
	int year,month,day,hour,minute,second;	/* 统一初始时间，采用年月日时分秒形式 */
	
  for(i=0; i<3; i++)
		orbInc[i] = 0.0;
	
  for(i=0; i<3; i++)
    mtqTq[i] = 0.0;   /* 目标控制力矩 */
	
   PFB[0] = 0;
   PFB[1] = 1.0/0.01282;   /* 俯仰滤波器参数初始化 */
   RPF = 0.07;    /*  */
   QPF = 3.68e-8;    /*  */

   magDotDmpFlg = VALID;			
   pitFltComFlg = INVALID;			       
   attStaFlg = INVALID;			              		              
   cntDmpFlag = 0;
   cntPitcomFlag = 0;
   cntAttStaFlag = 0;

   tInterval = 2.0;	                        /*轨道姿态传播的步长*/		                           

   /***************************************/
   /* 初始化上行指令参数 */      
	 AdcsOrbFlg = INVALID;
   AdcsGpsUse = INVALID; 
   upXwAdcsTLEFlag = INVALID;  

   /* 初始化指令信息参数 */
   upAdcsConP = 3.2e-6;                        /*三轴稳定控制律P系数*/
   upAdcsConD = 4.05e-5;                         /*三轴稳定控制律D系数  */
   upAdcsConZ = 8e-4;                          /*章进动控制律Z系数    */

   upXwAdcsConPFlag = INVALID;        
   upXwAdcsConDFlag = INVALID;                
   upXwAdcsConZFlag = INVALID;         
                     
   upXwAdcsReDmp = INVALID;                
       
   upXwAdcsDmpFlgFlag = INVALID;
   upXwAdcsFltFlgFlag = INVALID;
   upXwAdcsCtlFlgFlag = INVALID;

   /***************************************/                 
   /* 初始化时间起点 */
	 year = 2014; month = 4; day = 15; hour = 0; minute = 0; second = 0;
	 timeFormTrans(&TinSat0,&year,&month,&day,&hour,&minute,&second);
	 updateTimeFlag = INVALID;
}
