#include <includes.h>
#include "globalavr.h"

/************************************************* ��������ȫ�ֱ��� ***************************************************************/
/* ����Flash�洢�� */
const CPU_INT32U NorFlashStroeMap[]={
										NOR_FLASH_SA0 ,NOR_FLASH_SA1 ,NOR_FLASH_SA2 ,NOR_FLASH_SA3 ,NOR_FLASH_SA4 ,
										NOR_FLASH_SA5 ,NOR_FLASH_SA6 ,NOR_FLASH_SA7 ,NOR_FLASH_SA8 ,NOR_FLASH_SA9 ,
										NOR_FLASH_SA10,NOR_FLASH_SA11,NOR_FLASH_SA12,NOR_FLASH_SA13,NOR_FLASH_SA14,
										NOR_FLASH_SA15,NOR_FLASH_SA16,NOR_FLASH_SA17,NOR_FLASH_SA18,NOR_FLASH_SA19,
										NOR_FLASH_SA20,NOR_FLASH_SA21,NOR_FLASH_SA22,NOR_FLASH_SA23,NOR_FLASH_SA24,
										NOR_FLASH_SA25,NOR_FLASH_SA26,NOR_FLASH_SA27,NOR_FLASH_SA28,NOR_FLASH_SA29,
										NOR_FLASH_SA30,NOR_FLASH_SA31,NOR_FLASH_SA32,NOR_FLASH_SA33,NOR_FLASH_SA34,
										NOR_FLASH_SA35,NOR_FLASH_SA36,NOR_FLASH_SA37,NOR_FLASH_SA38,NOR_FLASH_SA39,
										NOR_FLASH_SA40,NOR_FLASH_SA41,NOR_FLASH_SA42,NOR_FLASH_SA43,NOR_FLASH_SA44,
										NOR_FLASH_SA45,NOR_FLASH_SA46,NOR_FLASH_SA47,NOR_FLASH_SA48,NOR_FLASH_SA49,
										NOR_FLASH_SA50,NOR_FLASH_SA51,NOR_FLASH_SA52,NOR_FLASH_SA53,NOR_FLASH_SA54,
										NOR_FLASH_SA55,NOR_FLASH_SA56,NOR_FLASH_SA57,NOR_FLASH_SA58,NOR_FLASH_SA59,
										NOR_FLASH_SA60,NOR_FLASH_SA61,NOR_FLASH_SA62
									};
																		
/* ����ָ�����״̬�� */
CPU_INT08U InsBuf[BUFFER_SIZE];            /* ָ����ջ������ */
CPU_INT16U InsCnt;                         /* ָ����ռ���ֵ */
CPU_INT08U InsBuf_p;                       /* ָ��ȫ��ָ�� */								
CPU_INT08U ID_CommandBuf[BUFFER_SIZE];
CPU_INT08U ID_CommandCnt;				

/* �������ģת��״̬�� */																		
CPU_INT16U ObcAdValue[16][5];              /* ��Դ��ȡ�� */
CPU_INT16U ObcAdValueAver[16];

CPU_INT16U TempAdValue[16][5];              /* �¶Ȼ�ȡ�� */
CPU_INT16U TempAdValueAver[16];

								
/* ���ϴ洢ָ��״̬�� */
CPU_INT32U TelCurPtr;                      /* ��ǰң��洢ָ�� */
CPU_INT32U GpsCurPtr;                      /* ��ǰGPS�洢ָ�� */
CPU_INT32U PldCurPtr;                      /* ��ǰ�غɴ洢ָ�� */
CPU_INT08U TelOverRangeCnt;                /* ң��ָ��Խ����� */
CPU_INT08U GpsOverRangeCnt;                /* GPSָ��Խ����� */
CPU_INT08U PldOverRangeCnt;                /* �غ�ָ��Խ����� */
CPU_INT16U TelCurStrCnt;                   /* ��ǰң��洢�� */
CPU_INT16U GpsCurStrCnt;                   /* ��ǰGPS�洢�� */
CPU_INT16U PldCurStrCnt;                   /* ��ǰ�غɴ洢�� */
struct OBCStoreInfoStr GpsStrInfo;
struct OBCStoreInfoStr TelStrInfo;

/* ����ȫ��ʱ����� */
RTC_TimeTypeDef   CurTime;                 /* ��ǰ����ʱ�� */
RTC_DateTypeDef   CurDate;                 /* ��ǰ�������� */
CPU_INT32U        CurUTCTime;              /* ��ǰ����ʱ��UTC��ʽ */
/* ���Ͽ���״̬��־λ */
struct OBCBootInfoStr OBCBootInfo;         /* OBC����״̬��Ϣ */
CPU_INT08U LowVolCnt;                      /* ��ԴǷѹ���� */

/* �弶ͨ�ű��� */
CPU_INT08U EpsTranOTCnt;                   /* EPS���䳬ʱ���� */
CPU_INT08U EpsRevOTCnt;                    /* EPS������ռ��� */
CPU_INT08U ComTranOTCnt;                   /* COM���䳬ʱ���� */
CPU_INT08U ComRevOTCnt;                    /* COM���䳬ʱ���� */



/* �˿زɼ����� */

double MagCurOut[4];
/* ������������� */
CPU_INT32U MotorSetOutput;
CPU_INT32U MotorCurOutput;

CPU_INT08U MotorCommErr;
CPU_INT16U PwmSetDuty_1;
CPU_INT16U PwmSetDuty_2;
CPU_INT16U PwmSetDuty_3;
CPU_INT16U PwmCurDuty_1;
CPU_INT16U PwmCurDuty_2;
CPU_INT16U PwmCurDuty_3;

float PWM_Feq1;
float PWM_Feq2;

/* ���������ļ�ϵͳ */
FRESULT f_result;
FATFS f_fs;
FIL f_file;

/********************δ����**************************/
/* �������� */
CPU_INT32U RandNum_32;         /* 32λ������� */
CPU_INT32U CurFlashSetor;
CPU_INT08U FlashTestErrCnt;
CPU_INT08U TFCardTestErrCnt;
char DebugFlag = 1;

/* �����غ��������ݸ�ʽ */
typedef struct pld_frm
{
	CPU_INT32U        PloadCurTime;
	CPU_INT08U        PloadData[32][57];
}pload_frm;


/********************************** GPS������ر��� **********************************/
NMEA_MSG GpsCurInfo;                       	/* GPS��ǰ״̬���� */
CPU_INT08U GpsRevBuf[GPS_MAX_REV_SIZE];    	/* GPS�������ݽṹ�� */
volatile CPU_INT16U GpsRevCnt = 0;			/* GPS�������ݼ��� */
//NMEA_MSG GpsCurInfo;                  	/* GPS��ǰ״̬���� */
/********************************** ��Դ��ر��� **********************************/
/* ��Դ���� */
CPU_INT16U EpsAdValue[32][5];   /* ��Դ��ȡ�� */
CPU_INT16U EpsAdValueAver[32];


volatile CPU_INT08U BatInC;  	  /* ��س����� */
volatile CPU_INT08U BatOutC;    /* ��طŵ���� */
volatile CPU_INT08U BusC;  		  /* ���ߵ��� */
volatile CPU_INT08U ComC;  		  /* ͨ�ŵ��� */
volatile CPU_INT08U V3C;  		  /* 3.3V���� */
volatile CPU_INT08U V5C;  		  /* 5V���� */
volatile CPU_INT08U EPSC;       /* EPS������� */
volatile CPU_INT08U WhlAC;  	  /* ������A���� */
volatile CPU_INT08U WhlBC;  	  /* ������B���� */
volatile CPU_INT08U MtqC;  	 	  /* �Ű����� */
volatile CPU_INT08U USBC;  		  /* USB���� */
volatile CPU_INT08U GPSAC;  	  /* GPSA���� */
volatile CPU_INT08U GPSBC;  	  /* GPSB���� */
volatile CPU_INT08U MagC;  		  /* ��ǿ�Ƶ��� */
volatile CPU_INT08U PldC1;      /* ����1���� */
volatile CPU_INT08U PldC2;      /* ����2���� */
volatile CPU_INT08U PldC3;      /* ����3���� */
volatile CPU_INT08U PldC4;      /* ����4���� */
volatile CPU_INT08U PldC5;      /* ����5���� */
volatile CPU_INT08U PldC6;      /* ����6���� */
volatile CPU_INT08U PldC7;      /* ����7���� */
volatile CPU_INT08U CellC1;     /* ���Ƭ1���� */
volatile CPU_INT08U CellC2;     /* ���Ƭ2���� */
volatile CPU_INT08U CellC3;     /* ���Ƭ3���� */
volatile CPU_INT08U CellC4;     /* ���Ƭ4���� */
volatile CPU_INT08U CellC5;     /* ���Ƭ5���� */
volatile CPU_INT08U CellC6;     /* ���Ƭ6���� */
volatile CPU_INT08U SplyV1;     /* ĸ��1��ѹ */
volatile CPU_INT08U SplyV2;     /* ĸ��2��ѹ */
volatile CPU_INT08U BatV1;      /* ��ص�ѹ1 */
volatile CPU_INT08U BatV2;		  /* ��ص�ѹ2 */
volatile CPU_INT08U V3V1;  		  /* 3.3V��ѹ1 */
volatile CPU_INT08U V5V1;  		  /* 5V��ѹ1 */
volatile CPU_INT08U V3V2;  		  /* 3.3V��ѹ2 */
volatile CPU_INT08U V5V2;  		  /* 5V��ѹ2 */
volatile CPU_INT08U Temp1;  		/* �¶�1 */
volatile CPU_INT08U Temp2;  		/* �¶�2 */
volatile CPU_INT08U Temp3;  		/* �¶�3 */
volatile CPU_INT08U Temp4;  		/* �¶�4 */
volatile CPU_INT08U Temp5;  		/* �¶�5 */
volatile CPU_INT08U Temp6;  		/* �¶�6 */
volatile CPU_INT08U Temp7;  		/* �¶�7 */
volatile CPU_INT08U Temp8;  		/* �¶�8 */

/* ��Դ������ */
volatile CPU_INT16U SwSubSys;   /* ���ؿ�����1 */
volatile CPU_INT16U SwPld;		/* ���ؿ�����2 */
volatile CPU_INT16U FalSubSys;  /* ���ع���״̬1 */
volatile CPU_INT16U FalPld;		/* ���ع���״̬2 */



eps_hk_adc_t eps_adc_data; 		/* ����eps ���пɲ����ṹ�� */
eps_bat_t eps_bat;	 			/* ����ȫ�ֱ��� bat�ṹ�� */
eps_hk_state_t eps_state;		/* define eps hk state struct avariable */


/***************************** �����û����������ʼ������ *****************************/

void VarBak(void)
{
	unsigned char VarBackupBuff[40];
	VarBackupBuff[0] = 0;
	VarBackupBuff[1] = 0;
	VarBackupBuff[2] = 0;
	VarBackupBuff[3] = 0;
	VarBackupBuff[4] = 0;
	VarBackupBuff[5] = 0;
	VarBackupBuff[6] = 0;
	VarBackupBuff[7] = 0;
	VarBackupBuff[8] = 0;
  VarBackupBuff[9] = 0;
  VarBackupBuff[10] = OBCBootInfo.BootCnt;
	VarBackupBuff[11] = OBCBootInfo.BootPINRSTCnt;
	VarBackupBuff[12] = OBCBootInfo.BootPORRSTCnt;
	VarBackupBuff[13] = OBCBootInfo.BootSFTRSTCnt;
	VarBackupBuff[14] = OBCBootInfo.BootIWDGRSTCnt;
	VarBackupBuff[15] = OBCBootInfo.BootWWDGRSTCnt;
	VarBackupBuff[16] = OBCBootInfo.BootLPWRRSTCnt;
	VarBackupBuff[17] = OBCBootInfo.BootRTC_Source;
	VarBackupBuff[18] = OBCBootInfo.BootLSE_Error;
	VarBackupBuff[19] = OBCBootInfo.BootLSI_Error;
  VarBackupBuff[20] = EpsTranOTCnt;
	VarBackupBuff[21] = EpsRevOTCnt;
	
	VarBackupBuff[22] = (unsigned char)(TelCurPtr & 0xFF);
	VarBackupBuff[23] = (unsigned char)(TelCurPtr>>8 & 0xFF);
	VarBackupBuff[24] = (unsigned char)(TelCurPtr>>16 & 0xFF);
	VarBackupBuff[25] = (unsigned char)(TelCurPtr>>24 & 0xFF);
	VarBackupBuff[26] = (unsigned char)(GpsCurPtr & 0xFF);
	VarBackupBuff[27] = (unsigned char)(GpsCurPtr>>8 & 0xFF);
	VarBackupBuff[28] = (unsigned char)(GpsCurPtr>>16 & 0xFF);
	VarBackupBuff[29] = (unsigned char)(GpsCurPtr>>24 & 0xFF);
  VarBackupBuff[30] = (unsigned char)(PldCurPtr & 0xFF);
	VarBackupBuff[31] = (unsigned char)(PldCurPtr>>8 & 0xFF);
	VarBackupBuff[32] = (unsigned char)(PldCurPtr>>16 & 0xFF);
	VarBackupBuff[33] = (unsigned char)(PldCurPtr>>24 & 0xFF);
	VarBackupBuff[34] = TelOverRangeCnt;
	VarBackupBuff[35] = GpsOverRangeCnt;
	VarBackupBuff[36] = PldOverRangeCnt;
	
	VarBackupBuff[37] = ComTranOTCnt;
	VarBackupBuff[38] = ComRevOTCnt;
	VarBackupBuff[39] = LowVolCnt;
	
	//if(bsp_WriteCpuFlash(CurFlashSetor, VarBackupBuff, 40)==2)
	//{
	//	/* дFalsh��������Flash�������ѵ� */
	//}

}

void VarRov(void)
{
	unsigned char VarRecoveryBuff[40];
	//if(bsp_ReadCpuFlash(CurFlashSetor, VarRecoveryBuff, 40) == 1)
  //{
		///* ��Flash��������Flash�����ѵ� */
	
	//}
	OBCBootInfo.BootCnt = VarRecoveryBuff[10];
	OBCBootInfo.BootPINRSTCnt = VarRecoveryBuff[11];
	OBCBootInfo.BootPORRSTCnt = VarRecoveryBuff[12];
	OBCBootInfo.BootSFTRSTCnt = VarRecoveryBuff[13];
	OBCBootInfo.BootIWDGRSTCnt = VarRecoveryBuff[14];
	OBCBootInfo.BootWWDGRSTCnt = VarRecoveryBuff[15];
	OBCBootInfo.BootLPWRRSTCnt = VarRecoveryBuff[16];
	OBCBootInfo.BootRTC_Source = VarRecoveryBuff[17];
	OBCBootInfo.BootLSE_Error = VarRecoveryBuff[18];
	OBCBootInfo.BootLSI_Error = VarRecoveryBuff[19];
  EpsTranOTCnt = VarRecoveryBuff[20];
	EpsRevOTCnt = VarRecoveryBuff[21];
	TelCurPtr = (VarRecoveryBuff[25]<<24) | (VarRecoveryBuff[24]<<16) | (VarRecoveryBuff[23]<<8) | (VarRecoveryBuff[22]);
	GpsCurPtr = (VarRecoveryBuff[29]<<24) | (VarRecoveryBuff[28]<<16) | (VarRecoveryBuff[27]<<8) | (VarRecoveryBuff[26]);
  PldCurPtr = (VarRecoveryBuff[33]<<24) | (VarRecoveryBuff[32]<<16) | (VarRecoveryBuff[31]<<8) | (VarRecoveryBuff[30]);
	TelOverRangeCnt = VarRecoveryBuff[34];
	GpsOverRangeCnt = VarRecoveryBuff[35];
	PldOverRangeCnt = VarRecoveryBuff[36];
	
	ComTranOTCnt = VarRecoveryBuff[37];
	ComRevOTCnt = VarRecoveryBuff[38];
	LowVolCnt = VarRecoveryBuff[39];
	
}

/* ��ʼ����������״̬�� */
void bsp_Init_VAR(void)
{
	OS_ERR int_err;
	
	/* ���ò���ϵͳʱ���׼ */
	OSTimeSet(0, &int_err);    
	
  /* ���ϴ洢ָ��״̬����ʼ�� */	
	TelCurPtr = TEL_STO_ADDR;      /* ң��洢��ַ */
	GpsCurPtr = GPS_STO_ADDR;      /* GPS�洢��ַ */
	PldCurPtr = PLOAD_STO_ADDR;    /* �غɴ洢��ַ */
	TelOverRangeCnt = 0;           /* ң��ָ��Խ����� */
	GpsOverRangeCnt = 0;           /* GPSָ��Խ����� */
	PldOverRangeCnt = 0;           /* �غ�ָ��Խ����� */
	TelCurStrCnt = 0;              /* ��ǰң��洢�� */
	GpsCurStrCnt = 0;              /* ��ǰGPS�洢�� */
	PldCurStrCnt = 0;              /* ��ǰ�غɴ洢�� */
	
	Mem_Set(&GpsStrInfo, 0x00, sizeof(struct OBCStoreInfoStr));
	Mem_Set(&TelStrInfo, 0x00, sizeof(struct OBCStoreInfoStr));
	
	/* ң��ָ����س�ʼ�� */
	Mem_Set(InsBuf, 0x00, BUFFER_SIZE);
	Mem_Set(ID_CommandBuf, 0x00, BUFFER_SIZE);
	ID_CommandCnt	= 0x00;
	
	/* ��ģת����س�ʼ�� */
	Mem_Set(&ObcAdValue[0][0], 0x00, sizeof(ObcAdValue));
	Mem_Set(&EpsAdValue[0][0], 0x00, sizeof(EpsAdValue));
	Mem_Set(&TempAdValue[0][0], 0x00, sizeof(TempAdValue));
	
	/* ����ȫ��ʱ�������ʼ�� */
	Mem_Set(&CurTime, 0x00, sizeof(RTC_TimeTypeDef));
	Mem_Set(&CurDate, 0x00, sizeof(RTC_DateTypeDef));

	/* ���Ͽ���״̬��ʼ�� */
	Mem_Set(&OBCBootInfo, 0x00, sizeof(struct OBCBootInfoStr));
	LowVolCnt = 0;

	/* �弶ͨ�ű�����ʼ�� */
	EpsTranOTCnt = 0;                 
	EpsRevOTCnt = 0;       
	ComTranOTCnt = 0;            
	ComRevOTCnt = 0;
	
	/* �������������ʼ�� */
	PwmSetDuty_1 = 50;
	PwmSetDuty_2 = 50;
	PwmSetDuty_3 = 50;
	PwmCurDuty_1 = 0;
	PwmCurDuty_2 = 0;
	PwmCurDuty_3 = 0;
	MotorSetOutput = 40000;
	MotorCurOutput = 0;	
	
	CurFlashSetor = ADDR_FLASH_SECTOR_1;
	ADCS_Init_VAR();
	
	eps_data_Init();  	 /* EPS������ʼ�� */
	FipexInfomationInit();
	FipexSetDefaultInfo();
}
