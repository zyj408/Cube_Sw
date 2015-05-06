#ifndef  __GLOBALAVR_H__
#define  __GLOBALAVR_H__

#include <includes.h>


void ADCS_Init_VAR(void);
void EPS_Init_VAR(void);
void bsp_Init_VAR(void);
void VarRov(void);
void VarBak(void);

/* AD采样通道 */
#define OBC_5V             0                /* 星务板5V电源采样 分压比1:2 */
#define OBC_SUN1           1                /* 粗太敏1通道 */
#define OBC_SUN2           2                /* 粗太敏2通道 */
#define OBC_SUN3           4                /* 粗太敏3通道 */
#define OBC_SUN4           3                /* 粗太敏4通道 */
#define OBC_SUN5           6                /* 粗太敏5通道 */
#define OBC_SUN6           5                /* 粗太敏6通道 */
#define OBC_TEMP1          7                /* 温度传感器1通道 */
#define OBC_TEMP2          8                /* 温度传感器2通道 */
#define OBC_POWER          9                /* 星务板3.3V电源采样 分压比1:1 */          
#define OBC_EXPAND1        10               /* 星务板AD扩展1 */
#define OBC_EXPAND2        11               /* 星务板AD扩展2 */
#define OBC_EXPAND3        12               /* 星务板AD扩展3 */
#define OBC_EXPAND4        13               /* 星务板AD扩展4 */
#define OBC_EXPAND5        14               /* 星务板AD扩展5 */
#define OBC_EXPAND6        15               /* 星务板AD扩展6 */

/* 串口接收缓冲区大小 */ 
#define BUFFER_SIZE                256              /* 指令接收缓冲区大小 */
#define GPS_MAX_REV_SIZE           800              /* GPS接收缓冲区大小 */

/* 遥测存储 */
#define TEL_STO_ADDR               NOR_FLASH_SA0 			/* 遥测数据存储首地址 */
#define TEL_STO_RAG                0x00100000   		  /* 遥测数据存储地址范围 */

/* GPS存储 */
#define GPS_STO_ADDR               NOR_FLASH_SA32 			/* GPS数据存储首地址 */
#define GPS_STO_RAG                0x00100000 				/* GPS数据存储地址范围 */

/* 载荷存储 */
#define PLOAD_STO_ADDR             0x8000
#define PLOAD_STO_RAG              0x80000000
														
/* FLASH温度存储首地址 */ 
#define WRITE_READ_ADDR            0x8000

/* 外部FLASH段偏移地址 */
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
	char BootCnt;             /* 星务计算机启动计数 */
	char BootPINRSTCnt;       /* 星务计算机引脚复位计数 */
	char BootPORRSTCnt;       //
	char BootSFTRSTCnt;       /* 星务计算机软件复位计数 */
	char BootIWDGRSTCnt;      /* 星务计算机独立看门狗复位计数 */
	char BootWWDGRSTCnt;      /* 星务计算机窗口看门狗复位计数 */
	char BootLPWRRSTCnt;      
	/*******************/
	char BootRTC_Source;      /* RTC时钟源。0外部晶振，1内部晶振，2没有配置*/
	char BootLSE_Error;       /* 外部时钟错误次数 */
	char BootLSI_Error;       /* 内部时钟错误次数 */
};


struct OBCStoreInfoStr
{
	unsigned char SectorInUse;             /* 正在使用标志位 */
	
	struct
	{
		unsigned char Empty             :1; /* 空闲标志位 */
		unsigned char Full              :1; /* 存满标志位 */
		unsigned char Downed            :1; /* 优先级 */
	  unsigned char Broke             :1; /* 错误数 */
		unsigned char EraserTime        :4; /* 擦除次数 */
	}SectorInfo[32];
	
};


struct OBCTaskStatus
{
		unsigned char CpuUsage;    
		CPU_STK_SIZE StackFree;    
	  CPU_STK_SIZE StackUsed;
		CPU_TS TimeStamp;           
};

/* 声明外部软件超时定时器 */
extern OS_TMR  GPS_OT_TIMER;
extern OS_TMR  COM_OT_TIMER;;

/* GPS存储信号灯 */
extern OS_SEM   SEM_GPS_STO;	
extern OS_SEM   SEM_TEL_STO;
extern OS_SEM   SEM_PLOAD_STO;

/* 声明GPS接收缓冲变量 */
extern CPU_INT08U GpsRevBuf[GPS_MAX_REV_SIZE]; 
extern volatile CPU_INT16U GpsRevCnt;
extern NMEA_MSG GpsCurInfo;            
/* 外部变量声明 */
/* 星上指令接收状态量 */
extern CPU_INT08U InsBuf[BUFFER_SIZE];            /* 指令接收缓冲变量 */
extern CPU_INT16U InsCnt;                         /* 指令接收计数值 */
extern CPU_INT08U InsBuf_p;                       /* 指令全局指针 */								
extern CPU_INT08U ID_CommandBuf[BUFFER_SIZE];
extern CPU_INT08U ID_CommandCnt;				

/* 星务板数模转换状态量 */																		
extern CPU_INT16U ObcAdValue[16][6];              /* 电源获取量 */
extern CPU_INT08U ObcAdErr[16];
extern CPU_INT08U ObcCommErr;
																		
/* 星上存储指针状态量 */
extern CPU_INT32U TelCurPtr;                      /* 当前遥测存储指针 */
extern CPU_INT32U GpsCurPtr;                      /* 当前GPS存储指针 */
extern CPU_INT32U PldCurPtr;                      /* 当前载荷存储指针 */
extern CPU_INT08U TelOverRangeCnt;                /* 遥测指针越界计数 */
extern CPU_INT08U GpsOverRangeCnt;                /* GPS指针越界计数 */
extern CPU_INT08U PldOverRangeCnt;                /* 载荷指针越界计数 */
extern CPU_INT16U TelCurStrCnt;                   /* 当前遥测存储数 */
extern CPU_INT16U GpsCurStrCnt;                   /* 当前GPS存储数 */
extern CPU_INT16U PldCurStrCnt;                   /* 当前载荷存储数 */
extern struct OBCStoreInfoStr GpsStrInfo;
extern struct OBCStoreInfoStr TelStrInfo;

/* 星上全局时间变量 */
extern RTC_TimeTypeDef   CurTime;                 /* 当前卫星时间 */
extern RTC_DateTypeDef   CurDate;                 /* 当前卫星日期 */
extern CPU_INT32U        CurUTCTime;              /* 当前卫星时间UTC格式 */
/* 星上开机状态标志位 */
extern struct OBCBootInfoStr OBCBootInfo;         /* OBC启动状态信息 */
extern CPU_INT08U LowVolCnt;                      /* 电源欠压计数 */

/* 板级通信变量 */
extern CPU_INT08U EpsTranOTCnt;                   /* EPS传输超时计数 */
extern CPU_INT08U EpsRevOTCnt;                    /* EPS传输接收计数 */
extern CPU_INT08U ComTranOTCnt;                   /* COM传输超时计数 */
extern CPU_INT08U ComRevOTCnt;                    /* COM传输超时计数 */

/* 动量轮输出变量 */
extern CPU_INT32U MotorSetOutput;
extern CPU_INT32U MotorCurOutput;

extern CPU_INT08U MotorCommErr;
extern CPU_INT16U PwmSetDuty_1;
extern CPU_INT16U PwmSetDuty_2;
extern CPU_INT16U PwmSetDuty_3;
extern CPU_INT16U PwmCurDuty_1;
extern CPU_INT16U PwmCurDuty_2;
extern CPU_INT16U PwmCurDuty_3;
/* 卫星星务文件系统 */
extern FRESULT f_result;
extern FATFS f_fs;
extern FIL f_file;

/********************未定义**************************/
/* 其他变量 */
extern CPU_INT32U RandNum_32;         /* 32位真随机数 */
extern CPU_INT32U CurFlashSetor;
extern CPU_INT08U FlashTestErrCnt;
extern CPU_INT08U TFCardTestErrCnt;
extern char DebugFlag;


extern CPU_INT16U EpsAdValue[32][6];
extern CPU_INT08U EpsAdErr[32];
extern CPU_INT08U EpsCommErr;
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
/* μ??′?a1?á? */
extern volatile CPU_INT16U SwSubSys;
extern volatile CPU_INT16U SwPld;
extern volatile CPU_INT16U FalSubSys;
extern volatile CPU_INT16U FalPld;

/* 电源相关宏 */

/* 开关宏 */
#define SW_WHL_A                   0x8000 
#define SW_WHL_B  								 0x4000
#define SW_MTQ  									 0x2000
#define SW_USB  									 0x1000
#define SW_GPS_A  								 0x0800
#define SW_GPS_B  								 0x0400
#define SW_MAG  									 0x0200
#define SW_BAT_A 								   0x0100
#define SW_BAT_B  						  	 0x0080

#define SW_PLD1                    0x8000 
#define SW_PLD2                    0x4000 
#define SW_PLD3                    0x2000 
#define SW_PLD4                    0x1000 
#define SW_PLD5                    0x0800 
#define SW_PLD6                    0x0400 
#define SW_PLD7                    0x0200 
/* 故障宏 */
#define FAL_WHL_A                  0x8000 
#define FAL_WHL_B  								 0x4000
#define FAL_MTQ  									 0x2000
#define FAL_USB  									 0x1000
#define FAL_GPS_A  								 0x0800
#define FAL_GPS_B  								 0x0400
#define FAL_MAG  									 0x0200
#define FAL_BAT_A 								 0x0100
#define FAL_BAT_B  						  	 0x0080
#define FAL_COM  									 0x0040
#define FAL_3V3_1 								 0x0020
#define FAL_5V_1  						  	 0x0010

#define FAL_PLD1                   0x8000 
#define FAL_PLD2                   0x4000 
#define FAL_PLD3                   0x2000 
#define FAL_PLD4                   0x1000 
#define FAL_PLD5                   0x0800 
#define FAL_PLD6                   0x0400 
#define FAL_PLD7                   0x0200 



/* 入站指令 */
#define INS_CONN_TST           0x01    //通信链路测试
#define INS_COMM_SWITCH_CLR    0x02    //星载计算机12小时重置
#define INS_DOWN_TEL           0x03    //下行星上状态遥测数据
#define INS_DOWN_PLD           0x04    //下行QB50载荷数据
#define INS_DOWN_CMD           0x05    //下行星上指令数据
#define INS_SD_CLR             0x06    //SD卡清空
#define INS_FLASH_RST          0x07    //FLASH指针还原
#define INS_OBC_RST            0x08    //星务计算机重启

/* 供电开关指令 */
#define INS_MTQ_ON             0x10		 //磁棒开
#define INS_MTQ_OFF            0x11		 //磁棒关
#define INS_GPS_A_ON           0x12		 //GPSA开
#define INS_GPS_A_OFF          0x13		 //GPSA关
#define INS_GPS_B_ON           0x14		 //GPSB开
#define INS_GPS_B_OFF          0x15		 //GPSB关
#define INS_RSV_ON             0x16		 //保留开关1开
#define INS_RSV_OFF            0x17		 //保留开关1关
#define INS_MW_A_ON            0x18		 //动量轮A开
#define INS_MW_A_OFF           0x19		 //动量轮A关
#define INS_MW_B_ON            0x1A		 //动量轮B开
#define INS_MW_B_OFF           0x1B		 //动量轮B关
#define INS_SLBRD_ON           0x1C		 //帆板开
#define INS_SLBRD_OFF          0x1D		 //帆板关
#define INS_USB_ON             0x1E		 //USB开
#define INS_USB_OFF            0x1F		 //USB关
#define INS_S1_ON              0x20		 //S1开
#define INS_S1_OFF             0x21		 //S1关
#define INS_S2_ON              0x22		 //S2开
#define INS_S2_OFF             0x23		 //S2关
#define INS_S3_ON              0x24		 //S3开
#define INS_S3_OFF             0x25		 //S3关
#define INS_S4_ON              0x26		 //S4开
#define INS_S4_OFF             0x27		 //S4关
#define INS_MTQ1_DIR_POS       0x28		 //磁棒1正方向
#define INS_MTQ1_DIR_NAG       0x29		 //磁棒1反方向
#define INS_MTQ2_DIR_POS       0x2A		 //磁棒2正方向
#define INS_MTQ2_DIR_NAG       0x2B		 //磁棒2反方向
#define INS_MTQ3_DIR_POS       0x2C		 //磁棒3正方向
#define INS_MTQ3_DIR_NAG       0x2D		 //磁棒3反方向
/* 姿控系统指令 */
#define INS_DET                0x31    //重新阻尼
#define INS_STA                0x32		 //永久阻尼使能
#define INS_DUMP_FOEV_DIS      0x33    //永久阻尼禁止
#define INS_SW_MAG_A           0x34    //切换至磁强计A
#define INS_SW_MAG_B           0x35    //切换至磁强计B
#define INS_SW_MW_A            0x36    //切换至动量轮A
#define INS_SW_MW_B            0x37    //切换至动量轮B
/* 通信系统指令 */
#define INS_SW_1200            0x41    //BPSK1200切换
#define INS_SW_9600            0x42		 //BPSK9600开
#define INS_CW_ON              0x43    //CW开
#define INS_COM_TRAN_OFF       0x44    //通信机发射机关机


//******数据注入指令*****//
/* 姿控系统指令 */
#define INS_CTL_P_PRA          0x51    //三轴稳定控制律注入
#define INS_CTL_D_PRA          0x52    //三轴稳定控制律注入
#define INS_ZJD_CTL            0x53    //章进动控制系数
//**************************
#define INS_DMP_FLAG           0x54    //阻尼标志位
#define INS_FLT_FLAG           0x55    //测量标志位
#define INS_CTL_FLAG           0x56    //控制标志位
#define INS_CNT_DMP_FLAG       0x57    //阻尼计数器
#define INS_CNT_FLT_FLAG       0x58    //测量计数器
#define INS_CNT_CTL_FLAG       0x59    //控制计数器

#define INS_ORB_TLE_FLAG       0x5A    //TLE轨道上注 modify by ZYJ 2015.05.06
/**************************/
#define INS_EQUP_INPUT         0x61    //器件健康状态变更指令
#define INS_TEL_ADRS           0x62    //遥测存储指针变更
#define INS_GPS_ADRS           0x63    //GPS存储指针变更
#define INS_TIME_IN            0x64    //时间注入
#define INS_FIPEX_SCRIPT_IN    0x65    //Fipex指令注入
#endif
