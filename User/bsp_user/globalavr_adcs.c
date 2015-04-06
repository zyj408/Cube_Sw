#include <includes.h>

/****************************** �˿������к��еı��� ******************************/
/* �˿��źŵ� */
OS_SEM   SEM_ORB_DET;	
OS_SEM   SEM_ORB_CALCU;
OS_SEM   SEM_CYC_INFO;
OS_SEM   SEM_MAG_DOT_DMP;
OS_SEM   SEM_PIT_FLT_COM;
OS_SEM   SEM_PIT_FLT_PRO;
OS_SEM   SEM_ATT_STA_CTL;

/* �˿ر��� */
/*---------------------------------------���й���ң��-------------------------------------------*/
double     downAdcsOrb[6];                  /* ���й������ */         
double     downAdcsWhlV;                    /* ���ж����ֵ�ѹ */   
double     downAdcsMtqV[3];                 /* ���д���������ѹ */    
double     downAdcsMeasAng;                 /* ���и����ǲ���ֵ */   
double     downAdcsFltAngRate[2];           /* �����˲���ĸ����Ǽ����ٶ� */   
double     downAdcsMagTable[3];             /* ����WGS84ϵ�´ų�ֵ */   
double     downAdcsMagnetometer[3];         /* ���д�ǿ�Ʋ���ֵ */

enum BOOL  downAdcsmagDotDmpFlg;            /* ���������־λ */
int        downAdcscntDmpFlag;              /* ����������� */
enum BOOL  downAdcspitFltComFlg;            /* ���в�����־λ */
int      	downAdcscntPitcomFlag;           /* ���в������� */
enum BOOL  downAdcsattStaFlg;               /* ���������ȶ����Ʊ�־λ */
int      	downAdcscntAttStaFlag;           /* ���������ȶ����ƴ��� */

/*--------------------------------------����ָ��----------------------------------------------- */
enum BOOL  AdcsOrbFlg;                   /*�����Ч��־λ*/
enum BOOL  upXwAdcsTLEFlag;              /*TLE�����ע��־λ�������ۺ���λ*/
double     upXwAdcsTLEBstar;
double     upXwAdcsTLEEcco;
double     upXwAdcsTLEInclo;
double     upXwAdcsTLEArgpo;
double     upXwAdcsTLEJdsatepoch;
double     upXwAdcsTLEMo;
double     upXwAdcsTLENo;
double     upXwAdcsTLENodeo;

double     upAdcsConP;                   /*�����ȶ�������Pϵ�����˿�ʹ��*/
double     upXwAdcsConP;                 /*�����ȶ�������Pϵ���������ۺϷ�����ָ��*/
enum BOOL  upXwAdcsConPFlag;             /*�����ȶ�������Pϵ����ע��־λ�������ۺ���λ*/

double     upAdcsConD;                   /*�����ȶ�������Dϵ�����˿�ʹ��*/
double     upXwAdcsConD;                 /*�����ȶ�������Dϵ���������ۺϷ�����ָ��*/
enum BOOL  upXwAdcsConDFlag;             /*�����ȶ�������Dϵ����ע��־λ�������ۺ���λ*/

double     upAdcsWhlV;                   /*�����ֽǶ������Ƶ�ѹ���˿�ʹ��*/
double     upXwAdcsWhlV;                 /*�����ֽǶ������Ƶ�ѹ�������ۺϷ�����ָ��*/
enum BOOL  upXwAdcsWhlVFlag;             /*�����ֽǶ������Ƶ�ѹ�������ۺ���λ*/

double     upAdcsConZ;                   /*�½���������Zϵ�����˿�ʹ��*/
double     upXwAdcsConZ;                 /*�½���������Zϵ���������ۺϷ�����ָ��*/
enum BOOL  upXwAdcsConZFlag;             /*�½���������Zϵ���������ۺ���λ*/

enum BOOL  upXwAdcsReDmp;                /*���������־���˿�����*/
enum BOOL  upXwAdcsDmpForever;           /*���������־��������ָ������*/

enum BOOL  upXwAdcsDmpFlg;               /*���������־λ����Ӧ�˿�magDotDmpFlg*/
enum BOOL  upXwAdcsDmpFlgFlag;           /*�����־λ�������ۺ���λ*/
 
enum BOOL  upXwAdcsFltFlg;               /*���в�����־λ����Ӧ�˿�pitFltComFlg*/
enum BOOL  upXwAdcsFltFlgFlag;           /*������־λ�������ۺ���λ*/

enum BOOL  upXwAdcsCtlFlg;               /*���п��Ʊ�־λ����Ӧ�˿�attStaFlg*/
enum BOOL  upXwAdcsCtlFlgFlag;           /*���Ʊ�־λ�������ۺ���λ*/

/*------------------------------------------------------------------------------------- */
double tInterval;			                    /* �����̬�����Ĳ��� */
double MagInO_YInc;
double MagInO_last[3];

double TinSat0;                            /* ���ϳ�ʼʱ��ʱ�� */
double TinSat;			                        /* ����ʱ�� */
double WGS84PV[6];                         /* ��Դ��GPS���ջ� */
double orbInfo[6],orbInc[3],orbInfoGPS[6];	/* �����Ϣ�����������Ϣ��GPS�����Ϣ*/
double magnetometer[3],MTQOut[3];          /* ��ǿ������ֵ��������������ž� */
double mtqTq[3];			                      /* ���������������� */

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

double adcs_time0;
uint32_t adcs_timer;
/*------------------------------------------------------------------------------------- */
void ADCS_Init_VAR(void)
{
	int i;
	
  for(i=0; i<3; i++)
		orbInc[i] = 0.0;
	
	for(i=0; i<3; i++)
    mtqTq[i] = 0.0;   /* ���������Ŀ��������� */
 
	
   PFB[0] = 0;
	 PFB[1] = 1.0/3.353;   /* �����˲���������ʼ�� */
   RPF = 1.0;    /*  */
   QPF = 1.0;    /*  */

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
   upAdcsConP = 0.0003;                        /*�����ȶ�������Pϵ��*/
   upAdcsConD = 0.006;                         /*�����ȶ�������Dϵ��  */
   upAdcsWhlV = 3.0;                            /*�����ֽǶ������Ƶ�ѹ */
   upAdcsConZ = 0.08;                          /*�½���������Zϵ��    */

   upXwAdcsConPFlag = INVALID;        
   upXwAdcsConDFlag = INVALID;         
   upXwAdcsWhlVFlag = INVALID;         
   upXwAdcsConZFlag = INVALID;         
                     
   upXwAdcsReDmp = INVALID;             
   upXwAdcsDmpForever = INVALID;    
       
   upXwAdcsDmpFlgFlag =INVALID;
   upXwAdcsFltFlgFlag =INVALID;
   upXwAdcsCtlFlgFlag =INVALID;

   /***************************************/                 
   /* ��ʼ��ʱ����㣬��Դ��GPS��ʱ�������ʱģ��*/
	 adcs_time0 = 2456762.5;
}