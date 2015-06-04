#ifndef _GLOBALAVR_ADCS_H
#define _GLOBALAVR_ADCS_H
#include "includes.h"

/* �˿��źŵ� */
extern OS_SEM   SEM_CYC_INFO;
extern OS_SEM   SEM_MAG_DOT_DMP;
extern OS_SEM   SEM_PIT_FLT_COM;
extern OS_SEM   SEM_ATT_STA_CTL;

/* �˿ر��� */
/*---------------------------------------���й���ң��-------------------------------------------*/
extern double     downAdcsOrb[6];                  /* ���й������ */           
extern double     downAdcsMtqV[3];                 /* ���д���������ѹ */    
extern double     downAdcsMeasAng;                 /* ���и����ǲ���ֵ */   
extern double     downAdcsFltAngRate[2];           /* �����˲���ĸ����Ǽ����ٶ� */   
extern double     downAdcsMagTable[3];             /* ����WGS84ϵ�´ų�ֵ */   
extern double     downAdcsMagnetometer[3];         /* ���д�ǿ�Ʋ���ֵ */

extern enum BOOL  downAdcsmagDotDmpFlg;            /* ���������־λ */
extern int        downAdcscntDmpFlag;              /* ����������� */
extern enum BOOL  downAdcspitFltComFlg;            /* ���в�����־λ */
extern int      	downAdcscntPitcomFlag;           /* ���в������� */
extern enum BOOL  downAdcsattStaFlg;               /* ���������ȶ����Ʊ�־λ */
extern int      	downAdcscntAttStaFlag;           /* ���������ȶ����ƴ��� */

/*--------------------------------------����ָ��----------------------------------------------- */
extern double     upDelta_TinSat;               /* ���е�����ʱʱ�� */
extern enum BOOL  AdcsOrbFlg;                   /*�����Ч��־λ*/
extern enum BOOL  upXwAdcsTLEFlag;              /*TLE�����ע��־λ�������ۺ���λ*/
extern enum BOOL  upAdcsTLEFlag;
extern double     upXwAdcsTLEBstar;
extern double     upXwAdcsTLEEcco;
extern double     upXwAdcsTLEInclo;
extern double     upXwAdcsTLEArgpo;
extern double     upXwAdcsTLEJdsatepoch;
extern double     upXwAdcsTLEMo;
extern double     upXwAdcsTLENo;
extern double     upXwAdcsTLENodeo;

extern double     upAdcsConP;                   /*�����ȶ�������Pϵ�����˿�ʹ��*/
extern double     upXwAdcsConP;                 /*�����ȶ�������Pϵ���������ۺϷ�����ָ��*/
extern enum BOOL  upXwAdcsConPFlag;             /*�����ȶ�������Pϵ����ע��־λ�������ۺ���λ*/

extern double     upAdcsConD;                   /*�����ȶ�������Dϵ�����˿�ʹ��*/
extern double     upXwAdcsConD;                 /*�����ȶ�������Dϵ���������ۺϷ�����ָ��*/
extern enum BOOL  upXwAdcsConDFlag;             /*�����ȶ�������Dϵ����ע��־λ�������ۺ���λ*/

extern double     upAdcsConZ;                   /*�½���������Zϵ�����˿�ʹ��*/
extern double     upXwAdcsConZ;                 /*�½���������Zϵ���������ۺϷ�����ָ��*/
extern enum BOOL  upXwAdcsConZFlag;             /*�½���������Zϵ���������ۺ���λ*/

extern enum BOOL  upXwAdcsReDmp;                /*���������־���˿�����*/

extern enum BOOL  upXwAdcsDmpFlg;               /*���������־λ����Ӧ�˿�magDotDmpFlg*/
extern enum BOOL  upXwAdcsDmpFlgFlag;           /*�����־λ�������ۺ���λ*/
 
extern enum BOOL  upXwAdcsFltFlg;               /*���в�����־λ����Ӧ�˿�pitFltComFlg*/
extern enum BOOL  upXwAdcsFltFlgFlag;           /*������־λ�������ۺ���λ*/

extern enum BOOL  upXwAdcsCtlFlg;               /*���п��Ʊ�־λ����Ӧ�˿�attStaFlg*/
extern enum BOOL  upXwAdcsCtlFlgFlag;           /*���Ʊ�־λ�������ۺ���λ*/

/*------------------------------------------------------------------------------------- */
extern double tInterval;			                    /* �����̬�����Ĳ��� */
extern double MagInO_YInc;
extern double MagInO_last[3];

extern double WGS84PV[6];                         /* ��Դ��GPS���ջ� */
extern double orbInfo[6],orbInc[3],orbInfoGPS[6];	/* �����Ϣ�����������Ϣ��GPS�����Ϣ*/
extern double magnetometer[3],MTQOut[3];          /* ��ǿ������ֵ��������������ž� */
extern double mtqTq[3];			                      /* ���������������� */

extern enum BOOL  magDotDmpFlg;                   /* �����־λ */
extern enum BOOL  pitFltComFlg;                   /* �����˲���־λ */
extern enum BOOL  attStaFlg;                      /* �����ȶ����Ʊ�־λ */
extern int cntDmpFlag;                            /* ������� */
extern int cntPitcomFlag;                         /* �����˲����� */
extern int cntAttStaFlag;                         /* �����ȶ����ƴ��� */

extern const double ae;			                      /* ����İ뾶 */
extern const double GM;		                        /* ������������ */
extern const double J2; 	    	                  /* J2�㶯�� */
#if debug_mag_enable
extern const double magTable[10682][3];     	    /* �洢�����ϵĴų��� */	
#else
extern double (*magTable)[3];
#endif
extern double PFSt[2];		                        /* �����˲���״̬��Э������ */
extern double PPF[2][2];
extern double PFB[2]; 			                      /* �����˲������õĲ��� */
extern double RPF;                                /* ��������Э����*/
extern double QPF;                                /* ϵͳ��������Э����*/

extern elsetrec satrec;
extern int CntSGP4;
extern enum BOOL  AdcsGpsUse;
extern enum BOOL  AdcsOrbGPSFlag;
extern elsetrec satrecFromGPS;
extern int CntNoGPS;

extern enum BOOL  updateTimeFlag;                 /* ��ʱ��־λ */
extern double TinSat0;                            /* ͳһ��ʼʱ�䣬���Ը�ʽ */
extern double TinSat;			                       /* ����ʵʱʱ�䣬���Ը�ʽ */
extern uint32_t adcs_timer;


void ADCS_Init_VAR(void);
#endif
