#ifndef  __BSP_GPS_H__
#define  __BSP_GPS_H__

//#include <includes.h>

/* GPS NMEA-0183协议重要参数结构体定义 */
/* 卫星信息 */
typedef struct NMEA_SLMSG_STRU
{										    
 	u8 num;								/* 卫星编号 */
	u8 eledeg;						/* 卫星仰角 */
	u16 azideg;						/* 卫星方位角 */
	u8 sn;								/* 信噪比 */		   
}NMEA_SLMSG;  

/* UTC时间信息 */
typedef struct NMEA_UTC_TIME_STRU
{										    
 	u16 year;							/* 年份 */
	u8 month;							/* 月份 */
	u8 date;							/* 日期 */
	u8 hour; 							/* 小时 */
	u8 min; 							/* 分钟 */
	u8 sec; 							/* 秒钟 */
}NMEA_UTC_TIME;   

/* GPS接收信息 */
typedef struct NMEA_MSG_STRU 
{										    
 	u8 svnum;					         /* 可见卫星数 */
	NMEA_SLMSG slmsg[12];			 /* 最多12颗卫星 */
	NMEA_UTC_TIME utc;			   /* UTC时间 */
	u32 latitude;				       /* 纬度 分扩大100000倍,实际要除以100000 */
	u8 nshemi;								 /* 北纬/南纬,N:北纬;S:南纬 */				  
	u32 longitude;			    	 /* 经度 分扩大100000倍,实际要除以100000 */
	u8 ewhemi;								 /* 东经/西经,E:东经;W:西经 */
	u8 gpssta;								 /* GPS状态:0,未定位;1,非差分定位;2,差分定位;6,正在估算 */				  
 	u8 posslnum;							 /* 用于定位的卫星数,0~12 */
 	u8 possl[12];							 /* 用于定位的卫星编号 */
	u8 fixmode;								 /* 定位类型:1,没有定位;2,2D定位;3,3D定位 */
	u16 pdop;									 /* 位置精度因子 0~500,对应实际值0~50.0 */
	u16 hdop;									 /* 水平精度因子 0~500,对应实际值0~50.0 */
	u16 vdop;									 /* 垂直精度因子 0~500,对应实际值0~50.0 */
	int altitude;			 			   /* 海拔高度,放大了10倍,实际除以10.单位:0.1m */
	u16 speed;								 /* 地面速率,放大了1000倍,实际除以10.单位:0.001公里/小时 */ 
}NMEA_MSG;

void GPS_Analysis(NMEA_MSG *gpsx,u8 *buf);
	

#endif
