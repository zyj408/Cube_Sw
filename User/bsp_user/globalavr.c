#include <includes.h>
#include "globalavr.h"

/************************************************* 星务计算机全局变量 ***************************************************************/
/* 星上Flash存储表 */
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
																		
/* 星上指令接收状态量 */
CPU_INT08U InsBuf[BUFFER_SIZE];            /* 指令接收缓冲变量 */
CPU_INT16U InsCnt;                         /* 指令接收计数值 */
CPU_INT08U InsBuf_p;                       /* 指令全局指针 */								
CPU_INT08U ID_CommandBuf[BUFFER_SIZE];
CPU_INT08U ID_CommandCnt;				

/* 星务板数模转换状态量 */																		
CPU_INT16U ObcAdValue[16][5];              /* 电源获取量 */
CPU_INT16U ObcAdValueAver[16];

CPU_INT16U TempAdValue[16][5];              /* 温度获取量 */
CPU_INT16U TempAdValueAver[16];

								
/* 星上存储指针状态量 */
CPU_INT32U TelCurPtr;                      /* 当前遥测存储指针 */
CPU_INT32U GpsCurPtr;                      /* 当前GPS存储指针 */
CPU_INT32U PldCurPtr;                      /* 当前载荷存储指针 */
CPU_INT08U TelOverRangeCnt;                /* 遥测指针越界计数 */
CPU_INT08U GpsOverRangeCnt;                /* GPS指针越界计数 */
CPU_INT08U PldOverRangeCnt;                /* 载荷指针越界计数 */
CPU_INT16U TelCurStrCnt;                   /* 当前遥测存储数 */
CPU_INT16U GpsCurStrCnt;                   /* 当前GPS存储数 */
CPU_INT16U PldCurStrCnt;                   /* 当前载荷存储数 */
struct OBCStoreInfoStr GpsStrInfo;
struct OBCStoreInfoStr TelStrInfo;

/* 星上全局时间变量 */
RTC_TimeTypeDef   CurTime;                 /* 当前卫星时间 */
RTC_DateTypeDef   CurDate;                 /* 当前卫星日期 */
CPU_INT32U        CurUTCTime;              /* 当前卫星时间UTC格式 */
/* 星上开机状态标志位 */
struct OBCBootInfoStr OBCBootInfo;         /* OBC启动状态信息 */
CPU_INT08U LowVolCnt;                      /* 电源欠压计数 */

/* 板级通信变量 */
CPU_INT08U EpsTranOTCnt;                   /* EPS传输超时计数 */
CPU_INT08U EpsRevOTCnt;                    /* EPS传输接收计数 */
CPU_INT08U ComTranOTCnt;                   /* COM传输超时计数 */
CPU_INT08U ComRevOTCnt;                    /* COM传输超时计数 */



/* 姿控采集变量 */

double MagCurOut[4];
/* 动量轮输出变量 */
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

/* 卫星星务文件系统 */
FRESULT f_result;
FATFS f_fs;
FIL f_file;

/********************未定义**************************/
/* 其他变量 */
CPU_INT32U RandNum_32;         /* 32位真随机数 */
CPU_INT32U CurFlashSetor;
CPU_INT08U FlashTestErrCnt;
CPU_INT08U TFCardTestErrCnt;
char DebugFlag = 1;

/* 星上载荷下行数据格式 */
typedef struct pld_frm
{
	CPU_INT32U        PloadCurTime;
	CPU_INT08U        PloadData[32][57];
}pload_frm;


/********************************** GPS接收相关变量 **********************************/
NMEA_MSG GpsCurInfo;                       	/* GPS当前状态量集 */
CPU_INT08U GpsRevBuf[GPS_MAX_REV_SIZE];    	/* GPS接收数据结构体 */
volatile CPU_INT16U GpsRevCnt = 0;			/* GPS接收数据计数 */
//NMEA_MSG GpsCurInfo;                  	/* GPS当前状态量集 */
/********************************** 电源相关变量 **********************************/
/* 电源变量 */
CPU_INT16U EpsAdValue[32][5];   /* 电源获取量 */
CPU_INT16U EpsAdValueAver[32];


volatile CPU_INT08U BatInC;  	  /* 电池充电电流 */
volatile CPU_INT08U BatOutC;    /* 电池放电电流 */
volatile CPU_INT08U BusC;  		  /* 总线电流 */
volatile CPU_INT08U ComC;  		  /* 通信电流 */
volatile CPU_INT08U V3C;  		  /* 3.3V电流 */
volatile CPU_INT08U V5C;  		  /* 5V电流 */
volatile CPU_INT08U EPSC;       /* EPS总体电流 */
volatile CPU_INT08U WhlAC;  	  /* 动量轮A电流 */
volatile CPU_INT08U WhlBC;  	  /* 动量轮B电流 */
volatile CPU_INT08U MtqC;  	 	  /* 磁棒电流 */
volatile CPU_INT08U USBC;  		  /* USB电流 */
volatile CPU_INT08U GPSAC;  	  /* GPSA电流 */
volatile CPU_INT08U GPSBC;  	  /* GPSB电流 */
volatile CPU_INT08U MagC;  		  /* 磁强计电流 */
volatile CPU_INT08U PldC1;      /* 负载1电流 */
volatile CPU_INT08U PldC2;      /* 负载2电流 */
volatile CPU_INT08U PldC3;      /* 负载3电流 */
volatile CPU_INT08U PldC4;      /* 负载4电流 */
volatile CPU_INT08U PldC5;      /* 负载5电流 */
volatile CPU_INT08U PldC6;      /* 负载6电流 */
volatile CPU_INT08U PldC7;      /* 负载7电流 */
volatile CPU_INT08U CellC1;     /* 电池片1电流 */
volatile CPU_INT08U CellC2;     /* 电池片2电流 */
volatile CPU_INT08U CellC3;     /* 电池片3电流 */
volatile CPU_INT08U CellC4;     /* 电池片4电流 */
volatile CPU_INT08U CellC5;     /* 电池片5电流 */
volatile CPU_INT08U CellC6;     /* 电池片6电流 */
volatile CPU_INT08U SplyV1;     /* 母线1电压 */
volatile CPU_INT08U SplyV2;     /* 母线2电压 */
volatile CPU_INT08U BatV1;      /* 电池电压1 */
volatile CPU_INT08U BatV2;		  /* 电池电压2 */
volatile CPU_INT08U V3V1;  		  /* 3.3V电压1 */
volatile CPU_INT08U V5V1;  		  /* 5V电压1 */
volatile CPU_INT08U V3V2;  		  /* 3.3V电压2 */
volatile CPU_INT08U V5V2;  		  /* 5V电压2 */
volatile CPU_INT08U Temp1;  		/* 温度1 */
volatile CPU_INT08U Temp2;  		/* 温度2 */
volatile CPU_INT08U Temp3;  		/* 温度3 */
volatile CPU_INT08U Temp4;  		/* 温度4 */
volatile CPU_INT08U Temp5;  		/* 温度5 */
volatile CPU_INT08U Temp6;  		/* 温度6 */
volatile CPU_INT08U Temp7;  		/* 温度7 */
volatile CPU_INT08U Temp8;  		/* 温度8 */

/* 电源开关量 */
volatile CPU_INT16U SwSubSys;   /* 负载开关量1 */
volatile CPU_INT16U SwPld;		/* 负载开关量2 */
volatile CPU_INT16U FalSubSys;  /* 负载故障状态1 */
volatile CPU_INT16U FalPld;		/* 负载故障状态2 */



eps_hk_adc_t eps_adc_data; 		/* 定义eps 所有可测量结构体 */
eps_bat_t eps_bat;	 			/* 定义全局变量 bat结构体 */
eps_hk_state_t eps_state;		/* define eps hk state struct avariable */


/***************************** 星上用户定义变量初始化函数 *****************************/

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
	//	/* 写Falsh出错，估计Flash寿命命已到 */
	//}

}

void VarRov(void)
{
	unsigned char VarRecoveryBuff[40];
	//if(bsp_ReadCpuFlash(CurFlashSetor, VarRecoveryBuff, 40) == 1)
  //{
		///* 读Flash出错，估计Flash寿命已到 */
	
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

/* 初始化星上所有状态量 */
void bsp_Init_VAR(void)
{
	OS_ERR int_err;
	
	/* 设置操作系统时间基准 */
	OSTimeSet(0, &int_err);    
	
  /* 星上存储指针状态量初始化 */	
	TelCurPtr = TEL_STO_ADDR;      /* 遥测存储地址 */
	GpsCurPtr = GPS_STO_ADDR;      /* GPS存储地址 */
	PldCurPtr = PLOAD_STO_ADDR;    /* 载荷存储地址 */
	TelOverRangeCnt = 0;           /* 遥测指针越界计数 */
	GpsOverRangeCnt = 0;           /* GPS指针越界计数 */
	PldOverRangeCnt = 0;           /* 载荷指针越界计数 */
	TelCurStrCnt = 0;              /* 当前遥测存储数 */
	GpsCurStrCnt = 0;              /* 当前GPS存储数 */
	PldCurStrCnt = 0;              /* 当前载荷存储数 */
	
	Mem_Set(&GpsStrInfo, 0x00, sizeof(struct OBCStoreInfoStr));
	Mem_Set(&TelStrInfo, 0x00, sizeof(struct OBCStoreInfoStr));
	
	/* 遥测指令相关初始化 */
	Mem_Set(InsBuf, 0x00, BUFFER_SIZE);
	Mem_Set(ID_CommandBuf, 0x00, BUFFER_SIZE);
	ID_CommandCnt	= 0x00;
	
	/* 数模转换相关初始化 */
	Mem_Set(&ObcAdValue[0][0], 0x00, sizeof(ObcAdValue));
	Mem_Set(&EpsAdValue[0][0], 0x00, sizeof(EpsAdValue));
	Mem_Set(&TempAdValue[0][0], 0x00, sizeof(TempAdValue));
	
	/* 星上全局时间变量初始化 */
	Mem_Set(&CurTime, 0x00, sizeof(RTC_TimeTypeDef));
	Mem_Set(&CurDate, 0x00, sizeof(RTC_DateTypeDef));

	/* 星上开机状态初始化 */
	Mem_Set(&OBCBootInfo, 0x00, sizeof(struct OBCBootInfoStr));
	LowVolCnt = 0;

	/* 板级通信变量初始化 */
	EpsTranOTCnt = 0;                 
	EpsRevOTCnt = 0;       
	ComTranOTCnt = 0;            
	ComRevOTCnt = 0;
	
	/* 星务输出变量初始化 */
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
	
	eps_data_Init();  	 /* EPS变量初始化 */
	FipexInfomationInit();
	FipexSetDefaultInfo();
}
