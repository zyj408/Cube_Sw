#ifndef _GLOBALAVR_ADCS_H
#define _GLOBALAVR_ADCS_H

/* 姿控信号灯 */
extern OS_SEM   SEM_ORB_DET;	
extern OS_SEM   SEM_ORB_CALCU;
extern OS_SEM   SEM_CYC_INFO;
extern OS_SEM   SEM_MAG_DOT_DMP;
extern OS_SEM   SEM_PIT_FLT_COM;
extern OS_SEM   SEM_PIT_FLT_PRO;
extern OS_SEM   SEM_ATT_STA_CTL;

/* 姿控变量 */
/*---------------------------------------下行功能遥测-------------------------------------------*/
extern double     downAdcsOrb[6];                  /* 下行轨道数据 */         
extern double     downAdcsWhlV;                    /* 下行动量轮电压 */   
extern double     downAdcsMtqV[3];                 /* 下行磁力矩器电压 */    
extern double     downAdcsMeasAng;                 /* 下行俯仰角测量值 */   
extern double     downAdcsFltAngRate[2];           /* 下行滤波后的俯仰角及角速度 */   
extern double     downAdcsMagTable[3];             /* 下行WGS84系下磁场值 */   
extern double     downAdcsMagnetometer[3];         /* 下行磁强计测量值 */

extern enum BOOL  downAdcsmagDotDmpFlg;            /* 下行阻尼标志位 */
extern int        downAdcscntDmpFlag;              /* 下行阻尼次数 */
extern enum BOOL  downAdcspitFltComFlg;            /* 下行测量标志位 */
extern int      	downAdcscntPitcomFlag;           /* 下行测量次数 */
extern enum BOOL  downAdcsattStaFlg;               /* 下行三轴稳定控制标志位 */
extern int      	downAdcscntAttStaFlag;           /* 下行三轴稳定控制次数 */

/*--------------------------------------上行指令----------------------------------------------- */
extern enum BOOL  AdcsOrbFlg;                   /*轨道有效标志位*/
extern enum BOOL  upXwAdcsTLEFlag;              /*TLE轨道上注标志位，数据综合置位*/
extern double     upXwAdcsTLEBstar;
extern double     upXwAdcsTLEEcco;
extern double     upXwAdcsTLEInclo;
extern double     upXwAdcsTLEArgpo;
extern double     upXwAdcsTLEJdsatepoch;
extern double     upXwAdcsTLEMo;
extern double     upXwAdcsTLENo;
extern double     upXwAdcsTLENodeo;

extern double     upAdcsConP;                   /*三轴稳定控制律P系数，姿控使用*/
extern double     upXwAdcsConP;                 /*三轴稳定控制律P系数，数据综合放上行指令*/
extern enum BOOL  upXwAdcsConPFlag;             /*三轴稳定控制律P系数上注标志位，数据综合置位*/

extern double     upAdcsConD;                   /*三轴稳定控制律D系数，姿控使用*/
extern double     upXwAdcsConD;                 /*三轴稳定控制律D系数，数据综合放上行指令*/
extern enum BOOL  upXwAdcsConDFlag;             /*三轴稳定控制律D系数上注标志位，数据综合置位*/

extern double     upAdcsWhlV;                   /*动量轮角动量控制电压，姿控使用*/
extern double     upXwAdcsWhlV;                 /*动量轮角动量控制电压，数据综合放上行指令*/
extern enum BOOL  upXwAdcsWhlVFlag;             /*动量轮角动量控制电压，数据综合置位*/

extern double     upAdcsConZ;                   /*章进动控制律Z系数，姿控使用*/
extern double     upXwAdcsConZ;                 /*章进动控制律Z系数，数据综合放上行指令*/
extern enum BOOL  upXwAdcsConZFlag;             /*章进动控制律Z系数，数据综合置位*/

extern enum BOOL  upXwAdcsReDmp;                /*重新阻尼标志，姿控清零*/
extern enum BOOL  upXwAdcsDmpForever;           /*永久阻尼标志，等上行指令清零*/

extern enum BOOL  upXwAdcsDmpFlg;               /*上行阻尼标志位，对应姿控magDotDmpFlg*/
extern enum BOOL  upXwAdcsDmpFlgFlag;           /*阻尼标志位，数据综合置位*/
 
extern enum BOOL  upXwAdcsFltFlg;               /*上行测量标志位，对应姿控pitFltComFlg*/
extern enum BOOL  upXwAdcsFltFlgFlag;           /*测量标志位，数据综合置位*/

extern enum BOOL  upXwAdcsCtlFlg;               /*上行控制标志位，对应姿控attStaFlg*/
extern enum BOOL  upXwAdcsCtlFlgFlag;           /*控制标志位，数据综合置位*/

/*------------------------------------------------------------------------------------- */
extern double tInterval;			                    /* 轨道姿态传播的步长 */
extern double MagInO_YInc;
extern double MagInO_last[3];

extern double TinSat;			                        /* 星上时间 */
extern double WGS84PV[6];                         /* 来源于GPS接收机 */
extern double orbInfo[6],orbInc[3],orbInfoGPS[6];	/* 轨道信息，轨道增量信息，GPS轨道信息*/
extern double magnetometer[3],MTQOut[3];          /* 磁强计量测值，磁力矩器输出磁矩 */
extern double mtqTq[3];			                      /* 磁力矩器控制力矩 */

extern enum BOOL  magDotDmpFlg;                   /* 阻尼标志位 */
extern enum BOOL  pitFltComFlg;                   /* 俯仰滤波标志位 */
extern enum BOOL  attStaFlg;                      /* 三轴稳定控制标志位 */
extern int cntDmpFlag;                            /* 阻尼次数 */
extern int cntPitcomFlag;                         /* 俯仰滤波次数 */
extern int cntAttStaFlag;                         /* 三轴稳定控制次数 */

extern const double ae;			                      /* 地球的半径 */
extern const double GM;		                        /* 地球引力常数 */
extern const double J2; 	    	                  /* J2摄动项 */
extern const double magTable[10682][3];     	    /* 存储在星上的磁场表 */	
extern double PFSt[2];		                        /* 俯仰滤波器状态与协方差阵 */
extern double PPF[2][2];
extern double PFB[2]; 			                      /* 俯仰滤波器中用的参量 */
extern double RPF;                                /* 量测噪声协方差*/
extern double QPF;                                /* 系统过程噪声协方差*/

extern elsetrec satrec;
extern int CntSGP4;
extern  enum BOOL  AdcsGpsUse;

extern double adcs_time0;
extern uint32_t adcs_timer;


void ADCS_Init_VAR(void);
#endif
