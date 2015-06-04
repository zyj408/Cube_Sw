#include <includes.h>

/****************************** �˿�����к��еı��� ******************************/
/* �˿��źŵ� */
OS_SEM   SEM_CYC_INFO;
OS_SEM   SEM_MAG_DOT_DMP;
OS_SEM   SEM_PIT_FLT_COM;
OS_SEM   SEM_ATT_STA_CTL;

/* �˿ر��� */
/*---------------------------------------���й���ң��-------------------------------------------*/
double     downAdcsOrb[6];                  /* ���й������ */           
double     downAdcsMtqV[3];                 /* ���д���������ѹ */    
double     downAdcsMeasAng;                 /* ���и����ǲ���ֵ */   
double     downAdcsFltAngRate[2];           /* �����˲���ĸ����Ǽ����ٶ� */   
double     downAdcsMagTable[3];             /* ����WGS84ϵ�´ų�ֵ */   
double     downAdcsMagnetometer[3];         /* ���д�ǿ�Ʋ���ֵ */

enum BOOL  downAdcsmagDotDmpFlg;            /* ���������־λ */
int        downAdcscntDmpFlag;              /* ����������� */
enum BOOL  downAdcspitFltComFlg;            /* ���в�����־λ */
int        downAdcscntPitcomFlag;           /* ���в������� */
enum BOOL  downAdcsattStaFlg;               /* ���������ȶ����Ʊ�־λ */
int        downAdcscntAttStaFlag;           /* ���������ȶ����ƴ��� */

/*--------------------------------------����ָ��----------------------------------------------- */
double     upDelta_TinSat;               /* ���е����GPS��ʱʱ�� */
enum BOOL  AdcsOrbFlg;                   /*�����Ч��־λ*/
enum BOOL  upXwAdcsTLEFlag;              /* TLE�����ע��־λ�������ۺ���λ */
enum BOOL  upAdcsTLEFlag;
double     upXwAdcsTLEBstar;
double     upXwAdcsTLEEcco;
double     upXwAdcsTLEInclo;
double     upXwAdcsTLEArgpo;
double     upXwAdcsTLEJdsatepoch;
double     upXwAdcsTLEMo;
double     upXwAdcsTLENo;
double     upXwAdcsTLENodeo;

double     upAdcsConP;                   /* �����ȶ�������Pϵ�����˿�ʹ�� */
double     upXwAdcsConP;                 /* �����ȶ�������Pϵ���������ۺϷ�����ָ�� */
enum BOOL  upXwAdcsConPFlag;             /* �����ȶ�������Pϵ����ע��־λ�������ۺ���λ */

double     upAdcsConD;                   /* �����ȶ�������Dϵ�����˿�ʹ�� */
double     upXwAdcsConD;                 /* �����ȶ�������Dϵ���������ۺϷ�����ָ�� */
enum BOOL  upXwAdcsConDFlag;             /* �����ȶ�������Dϵ����ע��־λ�������ۺ���λ */

double     upAdcsConZ;                   /* �½���������Zϵ�����˿�ʹ�� */
double     upXwAdcsConZ;                 /* �½���������Zϵ���������ۺϷ�����ָ�� */
enum BOOL  upXwAdcsConZFlag;             /* �½���������Zϵ���������ۺ���λ */

enum BOOL  upXwAdcsReDmp;                /* ���������־���˿����� */

enum BOOL  upXwAdcsDmpFlg;               /* ���������־λ����Ӧ�˿�magDotDmpFlg */
enum BOOL  upXwAdcsDmpFlgFlag;           /* �����־λ�������ۺ���λ */
 
enum BOOL  upXwAdcsFltFlg;               /* ���и����˲���־λ����Ӧ�˿�pitFltComFlg */
enum BOOL  upXwAdcsFltFlgFlag;           /* ������־λ�������ۺ���λ */

enum BOOL  upXwAdcsCtlFlg;               /* ���п��Ʊ�־λ����Ӧ�˿�attStaFlg */
enum BOOL  upXwAdcsCtlFlgFlag;           /* ���Ʊ�־λ�������ۺ���λ */

/*------------------------------------------------------------------------------------- */
double tInterval;			                    /* �����̬�����Ĳ��� */
double MagInO_YInc;
double MagInO_last[3];

double WGS84PV[6];                         /* ��Դ��GPS���ջ� */
double orbInfo[6],orbInc[3],orbInfoGPS[6]; /* �����Ϣ�����������Ϣ��GPS�����Ϣ*/
double magnetometer[3],MTQOut[3];          /* ��ǿ������ֵ��������������ž� */
double mtqTq[3];			                     /* ���������������� */

enum BOOL  magDotDmpFlg;                   /* �����־λ */
enum BOOL  pitFltComFlg;                   /* �����˲���־λ */
enum BOOL  attStaFlg;                      /* �����ȶ����Ʊ�־λ */
int cntDmpFlag;                            /* ������� */
int cntPitcomFlag;                         /* �����˲����� */
int cntAttStaFlag;                         /* �����ȶ����ƴ��� */

double PFSt[2];		                        /* �����˲���״̬��Э������ */
double PPF[2][2];
double PFB[2]; 			                      /* �����˲������õĲ��� */
double RPF;                                /* ��������Э����*/
double QPF;                                /* ϵͳ��������Э����*/

elsetrec satrec;
int CntSGP4;
enum BOOL  AdcsGpsUse;
enum BOOL  AdcsOrbGPSFlag;
elsetrec satrecFromGPS;
int CntNoGPS;

enum BOOL  updateTimeFlag;                 /* ��ʱ��־λ */
double TinSat0;                            /* ͳһ��ʼʱ�䣬���Ը�ʽ */
double TinSat;			                   /* ����ʵʱʱ�䣬���Ը�ʽ */
uint32_t adcs_timer;                       /* ���ϼ�ʱ�� */
/*------------------------------------------------------------------------------------- */


void ADCS_Init_VAR(void)
{
	int i;
	int year,month,day,hour,minute,second;	/* ͳһ��ʼʱ�䣬����������ʱ������ʽ */
	
  for(i=0; i<3; i++)
		orbInc[i] = 0.0;
	
  for(i=0; i<3; i++)
    mtqTq[i] = 0.0;   /* Ŀ��������� */
	
   PFB[0] = 0;
   PFB[1] = 1.0/0.01282;   /* �����˲���������ʼ�� */
   RPF = 0.07;    /*  */
   QPF = 3.68e-8;    /*  */

   magDotDmpFlg = VALID;			
   pitFltComFlg = INVALID;			       
   attStaFlg = INVALID;			              		              
   cntDmpFlag = 0;
   cntPitcomFlag = 0;
   cntAttStaFlag = 0;

   tInterval = 2.0;	                        /*�����̬�����Ĳ���*/		                           

   /***************************************/
   /* ��ʼ������ָ����� */      
	 AdcsOrbFlg = INVALID;
   AdcsGpsUse = INVALID; 
   upXwAdcsTLEFlag = INVALID;  

   /* ��ʼ��ָ����Ϣ���� */
   upAdcsConP = 3.2e-6;                        /*�����ȶ�������Pϵ��*/
   upAdcsConD = 4.05e-5;                         /*�����ȶ�������Dϵ��  */
   upAdcsConZ = 8e-4;                          /*�½���������Zϵ��    */

   upXwAdcsConPFlag = INVALID;        
   upXwAdcsConDFlag = INVALID;                
   upXwAdcsConZFlag = INVALID;         
                     
   upXwAdcsReDmp = INVALID;                
       
   upXwAdcsDmpFlgFlag = INVALID;
   upXwAdcsFltFlgFlag = INVALID;
   upXwAdcsCtlFlgFlag = INVALID;

   /***************************************/                 
   /* ��ʼ��ʱ����� */
	 year = 2014; month = 4; day = 15; hour = 0; minute = 0; second = 0;
	 timeFormTrans(&TinSat0,&year,&month,&day,&hour,&minute,&second);
	 updateTimeFlag = INVALID;
}
