#ifndef  __GLOBALAVR_H__
#define  __GLOBALAVR_H__

#include <includes.h>


void ADCS_Init_VAR(void);
void EPS_Init_VAR(void);
void bsp_Init_VAR(void);
void VarRov(void);
void VarBak(void);

/* AD����ͨ�� */
#define OBC_5V             0                /* �����5V��Դ���� ��ѹ��1:2 */
#define OBC_SUN1           1                /* ��̫��1ͨ�� */
#define OBC_SUN2           2                /* ��̫��2ͨ�� */
#define OBC_SUN3           4                /* ��̫��3ͨ�� */
#define OBC_SUN4           3                /* ��̫��4ͨ�� */
#define OBC_SUN5           6                /* ��̫��5ͨ�� */
#define OBC_SUN6           5                /* ��̫��6ͨ�� */
#define OBC_TEMP1          7                /* �¶ȴ�����1ͨ�� */
#define OBC_TEMP2          8                /* �¶ȴ�����2ͨ�� */
#define OBC_POWER          9                /* �����3.3V��Դ���� ��ѹ��1:1 */          
#define OBC_EXPAND1        10               /* �����AD��չ1 */
#define OBC_EXPAND2        11               /* �����AD��չ2 */
#define OBC_EXPAND3        12               /* �����AD��չ3 */
#define OBC_EXPAND4        13               /* �����AD��չ4 */
#define OBC_EXPAND5        14               /* �����AD��չ5 */
#define OBC_EXPAND6        15               /* �����AD��չ6 */

/* ���ڽ��ջ�������С */ 
#define BUFFER_SIZE                256              /* ָ����ջ�������С */
#define GPS_MAX_REV_SIZE           800              /* GPS���ջ�������С */

/* ң��洢 */
#define TEL_STO_ADDR               NOR_FLASH_SA0 			/* ң�����ݴ洢�׵�ַ */
#define TEL_STO_RAG                0x00100000   		  /* ң�����ݴ洢��ַ��Χ */

/* GPS�洢 */
#define GPS_STO_ADDR               NOR_FLASH_SA32 			/* GPS���ݴ洢�׵�ַ */
#define GPS_STO_RAG                0x00100000 				/* GPS���ݴ洢��ַ��Χ */

/* �غɴ洢 */
#define PLOAD_STO_ADDR             0x8000
#define PLOAD_STO_RAG              0x80000000
														
/* FLASH�¶ȴ洢�׵�ַ */ 
#define WRITE_READ_ADDR            0x8000

/* �ⲿFLASH��ƫ�Ƶ�ַ */
#define NOR_FLASH_SA0              0x00000000
#define NOR_FLASH_SA1              0x00008000
#define NOR_FLASH_SA2              0x00010000
#define NOR_FLASH_SA3              0x00018000
#define NOR_FLASH_SA4              0x00020000
#define NOR_FLASH_SA5              0x00028000
#define NOR_FLASH_SA6              0x00030000
#define NOR_FLASH_SA7              0x00038000
#define NOR_FLASH_SA8              0x00040000
#define NOR_FLASH_SA9              0x00048000
#define NOR_FLASH_SA10             0x00050000
#define NOR_FLASH_SA11             0x00058000
#define NOR_FLASH_SA12             0x00060000
#define NOR_FLASH_SA13             0x00068000
#define NOR_FLASH_SA14             0x00070000
#define NOR_FLASH_SA15             0x00078000
#define NOR_FLASH_SA16             0x00080000
#define NOR_FLASH_SA17             0x00088000
#define NOR_FLASH_SA18             0x00090000
#define NOR_FLASH_SA19             0x00098000
#define NOR_FLASH_SA20             0x000A0000
#define NOR_FLASH_SA21             0x000A8000
#define NOR_FLASH_SA22             0x000B0000
#define NOR_FLASH_SA23             0x000B8000
#define NOR_FLASH_SA24             0x000C0000
#define NOR_FLASH_SA25             0x000C8000
#define NOR_FLASH_SA26             0x000D0000
#define NOR_FLASH_SA27             0x000D8000
#define NOR_FLASH_SA28             0x000E0000
#define NOR_FLASH_SA29             0x000E8000
#define NOR_FLASH_SA30             0x000F0000
#define NOR_FLASH_SA31             0x000F8000
#define NOR_FLASH_SA32             0x00100000
#define NOR_FLASH_SA33             0x00108000
#define NOR_FLASH_SA34             0x00110000
#define NOR_FLASH_SA35             0x00118000
#define NOR_FLASH_SA36             0x00120000
#define NOR_FLASH_SA37             0x00128000
#define NOR_FLASH_SA38             0x00130000
#define NOR_FLASH_SA39             0x00138000
#define NOR_FLASH_SA40             0x00140000
#define NOR_FLASH_SA41             0x00148000
#define NOR_FLASH_SA42             0x00150000
#define NOR_FLASH_SA43             0x00158000
#define NOR_FLASH_SA44             0x00160000
#define NOR_FLASH_SA45             0x00168000
#define NOR_FLASH_SA46             0x00170000
#define NOR_FLASH_SA47             0x00178000
#define NOR_FLASH_SA48             0x00180000
#define NOR_FLASH_SA49             0x00188000
#define NOR_FLASH_SA50             0x00190000
#define NOR_FLASH_SA51             0x00198000
#define NOR_FLASH_SA52             0x001A0000
#define NOR_FLASH_SA53             0x001A8000
#define NOR_FLASH_SA54             0x001B0000
#define NOR_FLASH_SA55             0x001B8000
#define NOR_FLASH_SA56             0x001C0000
#define NOR_FLASH_SA57             0x001C8000
#define NOR_FLASH_SA58             0x001D0000
#define NOR_FLASH_SA59             0x001D8000
#define NOR_FLASH_SA60             0x001E0000
#define NOR_FLASH_SA61             0x001E8000
#define NOR_FLASH_SA62             0x001F0000


						
struct OBCBootInfoStr
{
	char BootCnt;             /* ���������������� */
	char BootPINRSTCnt;       /* �����������Ÿ�λ���� */
	char BootPORRSTCnt;       //
	char BootSFTRSTCnt;       /* �������������λ���� */
	char BootIWDGRSTCnt;      /* ���������������Ź���λ���� */
	char BootWWDGRSTCnt;      /* �����������ڿ��Ź���λ���� */
	char BootLPWRRSTCnt;      
	/*******************/
	char BootRTC_Source;      /* RTCʱ��Դ��0�ⲿ����1�ڲ�����2û������*/
	char BootLSE_Error;       /* �ⲿʱ�Ӵ������ */
	char BootLSI_Error;       /* �ڲ�ʱ�Ӵ������ */
};

struct OBCPWM_OutPutStr
{
	unsigned short 	PwmSetDuty;
	unsigned short 	PwmCurDuty;
	unsigned short 	PwmSetDutyOld;
	unsigned char	PwmSetDir;
	unsigned char	PwmCurDir;
};

struct OBCStoreInfoStr
{
	unsigned char SectorInUse;             /* ����ʹ�ñ�־λ */
	
	struct
	{
		unsigned char Empty             :1; /* ���б�־λ */
		unsigned char Full              :1; /* ������־λ */
		unsigned char Downed            :1; /* ���ȼ� */
		unsigned char Broke             :1; /* ������ */
		unsigned char EraserTime        :4; /* �������� */
	}SectorInfo[32];
	
};


struct OBCTaskStatus
{
	unsigned char CpuUsage;    
	CPU_STK_SIZE StackFree;    
	CPU_STK_SIZE StackUsed;
	CPU_TS TimeStamp;           
};

/* �����ⲿ�����ʱ��ʱ�� */
extern OS_TMR  GPS_OT_TIMER;
extern OS_TMR  COM_OT_TIMER;;

/* GPS�洢�źŵ� */
extern OS_SEM			SEM_GPS_STO;	
extern OS_SEM   		SEM_TEL_STO;
extern OS_SEM   		SEM_PLOAD_STO;
extern BSP_OS_MUTEX 	MUTEX_MAG_CAP;
/* ����GPS���ջ������ */
extern CPU_INT08U GpsRevBuf[GPS_MAX_REV_SIZE]; 
extern volatile CPU_INT16U GpsRevCnt;
extern NMEA_MSG GpsCurInfo;            
/* �ⲿ�������� */
/* ����ָ�����״̬�� */
extern CPU_INT08U InsBuf[BUFFER_SIZE];            /* ָ����ջ������ */
extern CPU_INT16U InsCnt;                         /* ָ����ռ���ֵ */
extern CPU_INT08U InsBuf_p;                       /* ָ��ȫ��ָ�� */								
extern CPU_INT08U ID_CommandBuf[BUFFER_SIZE];
extern CPU_INT08U ID_CommandCnt;				

/* �������ģת��״̬�� */																		
extern CPU_INT16U ObcAdValue[16][5];              /* ��Դ��ȡ�� */
extern CPU_INT16U ObcAdValueAver[16];

extern CPU_INT16U TempAdValue[16][5];              /* �¶Ȼ�ȡ�� */
extern CPU_INT16U TempAdValueAver[16];							
/* ���ϴ洢ָ��״̬�� */
extern CPU_INT32U TelCurPtr;                      /* ��ǰң��洢ָ�� */
extern CPU_INT32U GpsCurPtr;                      /* ��ǰGPS�洢ָ�� */
extern CPU_INT32U PldCurPtr;                      /* ��ǰ�غɴ洢ָ�� */
extern CPU_INT08U TelOverRangeCnt;                /* ң��ָ��Խ����� */
extern CPU_INT08U GpsOverRangeCnt;                /* GPSָ��Խ����� */
extern CPU_INT08U PldOverRangeCnt;                /* �غ�ָ��Խ����� */
extern CPU_INT16U TelCurStrCnt;                   /* ��ǰң��洢�� */
extern CPU_INT16U GpsCurStrCnt;                   /* ��ǰGPS�洢�� */
extern CPU_INT16U PldCurStrCnt;                   /* ��ǰ�غɴ洢�� */
extern struct OBCStoreInfoStr GpsStrInfo;
extern struct OBCStoreInfoStr TelStrInfo;

/* ����ȫ��ʱ����� */
extern RTC_TimeTypeDef   CurTime;                 /* ��ǰ����ʱ�� */
extern RTC_DateTypeDef   CurDate;                 /* ��ǰ�������� */
extern CPU_INT32U        CurUTCTime;              /* ��ǰ����ʱ��UTC��ʽ */
/* ���Ͽ���״̬��־λ */
extern struct OBCBootInfoStr OBCBootInfo;         /* OBC����״̬��Ϣ */
extern CPU_INT08U LowVolCnt;                      /* ��ԴǷѹ���� */

/* �弶ͨ�ű��� */
extern CPU_INT08U EpsTranOTCnt;                   /* EPS���䳬ʱ���� */
extern CPU_INT08U EpsRevOTCnt;                    /* EPS������ռ��� */
extern CPU_INT08U ComTranOTCnt;                   /* COM���䳬ʱ���� */
extern CPU_INT08U ComRevOTCnt;                    /* COM���䳬ʱ���� */

/* ������������� */
extern CPU_INT32U MotorSetOutput;
extern CPU_INT32U MotorCurOutput;

extern double MagCurOut[4];

extern struct OBCPWM_OutPutStr PwmOutPut[3];

extern float PWM_Feq1;
extern float PWM_Feq2;
/* ���������ļ�ϵͳ */
extern FRESULT f_result;
extern FATFS f_fs;
extern FIL f_file;

/********************δ����**************************/
/* �������� */
extern CPU_INT32U RandNum_32;         /* 32λ������� */
extern CPU_INT32U CurFlashSetor;
extern CPU_INT08U FlashTestErrCnt;
extern CPU_INT08U TFCardTestErrCnt;
extern char DebugFlag;


extern CPU_INT16U EpsAdValue[32][5];   /* ��Դ��ȡ�� */
extern CPU_INT16U EpsAdValueAver[32];


extern volatile CPU_INT08U BatInC;
extern volatile CPU_INT08U BatOutC;
extern volatile CPU_INT08U BusC;
extern volatile CPU_INT08U ComC;
extern volatile CPU_INT08U V3C;
extern volatile CPU_INT08U V5C; 
extern volatile CPU_INT08U EPSC;
extern volatile CPU_INT08U WhlAC; 
extern volatile CPU_INT08U WhlBC;  
extern volatile CPU_INT08U MtqC;  
extern volatile CPU_INT08U USBC;
extern volatile CPU_INT08U GPSAC; 
extern volatile CPU_INT08U GPSBC; 
extern volatile CPU_INT08U MagC; 
extern volatile CPU_INT08U PldC1;
extern volatile CPU_INT08U PldC2;
extern volatile CPU_INT08U PldC3;
extern volatile CPU_INT08U PldC4;
extern volatile CPU_INT08U PldC5;
extern volatile CPU_INT08U PldC6;
extern volatile CPU_INT08U PldC7;
extern volatile CPU_INT08U CellC1;
extern volatile CPU_INT08U CellC2;
extern volatile CPU_INT08U CellC3;
extern volatile CPU_INT08U CellC4;
extern volatile CPU_INT08U CellC5;
extern volatile CPU_INT08U CellC6;
extern volatile CPU_INT08U SplyV1;
extern volatile CPU_INT08U SplyV2;
extern volatile CPU_INT08U BatV1;
extern volatile CPU_INT08U BatV2;
extern volatile CPU_INT08U V3V1;
extern volatile CPU_INT08U V5V1;
extern volatile CPU_INT08U V3V2;
extern volatile CPU_INT08U V5V2;
extern volatile CPU_INT08U Temp1;
extern volatile CPU_INT08U Temp2;
extern volatile CPU_INT08U Temp3;
extern volatile CPU_INT08U Temp4;
extern volatile CPU_INT08U Temp5;
extern volatile CPU_INT08U Temp6;
extern volatile CPU_INT08U Temp7;
extern volatile CPU_INT08U Temp8;

extern volatile CPU_INT16U SwSubSys;
extern volatile CPU_INT16U SwPld;
extern volatile CPU_INT16U FalSubSys;
extern volatile CPU_INT16U FalPld;

extern eps_hk_adc_t eps_adc_data; 		/* ����eps ���пɲ����ṹ�� */
extern eps_bat_t eps_bat;	 			/* ����ȫ�ֱ��� bat�ṹ�� */
extern eps_hk_state_t eps_state;		/* define eps hk state struct avariable */

/* ��Դ��غ� */


/* ��վָ�� */
#define INS_CONN_TST           0x01    //ͨ����·����
#define INS_COMM_SWITCH_CLR    0x02    //���ؼ����12Сʱ����
#define INS_DOWN_TEL           0x03    //��������״̬ң������
#define INS_DOWN_PLD           0x04    //����QB50�غ�����
#define INS_DOWN_CMD           0x05    //��������ָ������
#define INS_SD_CLR             0x06    //SD�����
#define INS_FLASH_RST          0x07    //FLASHָ�뻹ԭ
#define INS_OBC_RST            0x08    //������������
#define INS_OBC_REV            0x09    //�������

/* ���翪��ָ�� */
#define INS_MTQ_ON             0x10		 //�Ű���
#define INS_MTQ_OFF            0x11		 //�Ű���
#define INS_GPS_A_ON           0x12		 //GPSA��
#define INS_GPS_A_OFF          0x13		 //GPSA��
#define INS_GPS_B_ON           0x14		 //GPSB��
#define INS_GPS_B_OFF          0x15		 //GPSB��
#define INS_RSV_ON             0x16		 //��������1��
#define INS_RSV_OFF            0x17		 //��������1��
#define INS_MW_A_ON            0x18		 //������A��
#define INS_MW_A_OFF           0x19		 //������A��
#define INS_MW_B_ON            0x1A		 //������B��
#define INS_MW_B_OFF           0x1B		 //������B��
#define INS_SLBRD_ON           0x1C		 //���忪
#define INS_SLBRD_OFF          0x1D		 //�����
#define INS_USB_ON             0x1E		 //USB��
#define INS_USB_OFF            0x1F		 //USB��
#define INS_S1_ON              0x20		 //S1��
#define INS_S1_OFF             0x21		 //S1��
#define INS_S2_ON              0x22		 //S2��
#define INS_S2_OFF             0x23		 //S2��
#define INS_S3_ON              0x24		 //S3��
#define INS_S3_OFF             0x25		 //S3��
#define INS_S4_ON              0x26		 //S4��
#define INS_S4_OFF             0x27		 //S4��
#define INS_MTQ1_DIR_POS       0x28		 //�Ű�1������
#define INS_MTQ1_DIR_NAG       0x29		 //�Ű�1������
#define INS_MTQ2_DIR_POS       0x2A		 //�Ű�2������
#define INS_MTQ2_DIR_NAG       0x2B		 //�Ű�2������
#define INS_MTQ3_DIR_POS       0x2C		 //�Ű�3������
#define INS_MTQ3_DIR_NAG       0x2D		 //�Ű�3������
/* �˿�ϵͳָ�� */
#define INS_DET                0x31    //��������
#define INS_STA                0x32		 //��������ʹ��
#define INS_DUMP_FOEV_DIS      0x33    //���������ֹ
#define INS_SW_MAG_A           0x34    //�л�����ǿ��A
#define INS_SW_MAG_B           0x35    //�л�����ǿ��B
#define INS_SW_MW_A            0x36    //�л���������A
#define INS_SW_MW_B            0x37    //�л���������B
/* ͨ��ϵͳָ�� */
#define INS_SW_1200            0x41    //BPSK1200�л�
#define INS_SW_9600            0x42		 //BPSK9600��
#define INS_CW_ON              0x43    //CW��
#define INS_COM_TRAN_OFF       0x44    //ͨ�Ż�������ػ�


//******����ע��ָ��*****//
/* �˿�ϵͳָ�� */
#define INS_CTL_P_PRA          0x51    //�����ȶ�������ע��
#define INS_CTL_D_PRA          0x52    //�����ȶ�������ע��
#define INS_ZJD_CTL            0x53    //�½�������ϵ��
//**************************
#define INS_DMP_FLAG           0x54    //�����־λ
#define INS_FLT_FLAG           0x55    //������־λ
#define INS_CTL_FLAG           0x56    //���Ʊ�־λ
#define INS_CNT_DMP_FLAG       0x57    //���������
#define INS_CNT_FLT_FLAG       0x58    //����������
#define INS_CNT_CTL_FLAG       0x59    //���Ƽ�����

#define INS_ORB_TLE_FLAG       0x5A    //TLE�����ע modify by ZYJ 2015.05.06

#define INS_MTQ1_PWM           0x5B
#define INS_MTQ2_PWM           0x5C
#define INS_MTQ3_PWM           0x5D

#define INS_ADCS_TIME_IN       0x5E

/**************************/
#define INS_EQUP_INPUT         0x61    //��������״̬���ָ��
#define INS_TEL_ADRS           0x62    //ң��洢ָ����
#define INS_GPS_ADRS           0x63    //GPS�洢ָ����
#define INS_TIME_IN            0x64    //ʱ��ע��
#define INS_FIPEX_SCRIPT_IN    0x65    //Fipexָ��ע��
#define INS_MAG_TABLE_IN   	   0x66    //Fipexָ��ע��
#endif
