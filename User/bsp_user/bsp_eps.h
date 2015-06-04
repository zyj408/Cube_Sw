/*
***********************************************************************************************
**bsp_eps.h
***********************************************************************************************
*/

#ifndef __BSP_EPS_H__
#define __BSP_EPS_H__

#include "stm32f4xx.h"
#include <stdio.h>



/**
	*@defineoutput
	*******************************************************************************************
	*/

#define OUT_REG1	0
#define OUT_REG2  	1
#define OUT_REG3   	2	
#define OUT_REG4   	3
#define OUT_REG5   	4
#define OUT_REG6   	5	
#define OUT_UREG1   6
#define OUT_UREG2   7
#define OUT_UREG3   8
#define OUT_UREG4   9
#define OUT_UREG5   10
#define OUT_DEPLOY  11

//define with usage name
#define OUT_ALL 	  20
#define OUT_GPSA    OUT_REG1
#define OUT_GPSB    OUT_REG2
#define OUT_MTQ     OUT_UREG1
#define OUT_WHEELA 	OUT_UREG2
#define OUT_WHEELB 	OUT_UREG3
#define OUT_USB     OUT_UREG4
#define OUT_FIPEX5V OUT_REG5
#define OUT_FIPEX3V OUT_REG6
#define OUT_HEAT    OUT_UREG5
#define OUT_RES     OUT_REG3


#define REG_NUM          6
#define UREG_NUM         5
#define SV_NUM           6


#define ADC_REF          2500   //ADC参考电压
#define ADC_FullScale    4095  //ADC量程
#define AD7490_REF       2500   //AD7490 参考电压
#define AD7490_FullScale 4095  //ad7490量程

/**
*@EpsAdc
*************************************************************************************
*/
typedef struct{
	uint16_t c_sv[SV_NUM];
	uint16_t v_sv[SV_NUM];
	uint16_t c_com;
	uint16_t c_bus;
	uint16_t v_bus;
	uint16_t c_out[REG_NUM+UREG_NUM];
	int16_t temp_eps[4];
	int16_t temp_bat[2];
} eps_hk_adc_t;

#define hk_adc_num  32

/**
*@battery
********************************************************************************************
*/

//电池电压状态
typedef enum{
	BAT_OV,         //8.35v以上过压
	BAT_NORMAL,     //7.2~8.35 正常
	BAT_UV,         //7.0~7.2欠压
	BAT_CRITICAL,   //6.75~7.0 critical low 
	BAT_HARDOFF,    //<6.75 硬件关断
} bat_state_t;

//电池判断电压电流温度参考值

#define  bat_full_volt       8300 //bat full charged voltage 
#define  bat_normal_volt     7200 //bat safe voltage range,if bat volt lower than this value,satellite switch to low power mode
#define  bat_critical_volt   7000 //bat critical low voltage ,cut off all the loads
#define  bat_hardoff_volt    6750 //bat hard cutting off voltage,the bus switch will turn on till the batvolt higher than the voltage value plus a  hysteretic voltage
#define  bat_oc_curr         2500//mode for batheater [0 =manual , 1 = auto]
#define  bat_heater_low      (-5)//turn heater on at [degc]
#define  bat_heater_high     (10) //turn heater off at [degc]

typedef enum{
	BAT_DISCH = 0,
	BAT_CHARGE,
	BAT_NOCURR
}bat_char_state_t;
typedef enum{NO,YES= !NO}bat_heater_state_t;
//电池所有状态量结构体
typedef struct{ 
	bat_state_t         bat_state;         //电池当前状态
	bat_char_state_t    bat_charge;        //电池充电状态
	bat_heater_state_t  bat_heater_status; //电池加热状态
	uint32_t            bat_fullcap;       //电池初始能量
	uint16_t            bat_percent;       //当前电量百分比
	uint16_t            bat_total_percent; //当前电池容量是初始容量的百分比
	uint32_t            bat_off_count;     //电池硬件关断的次数
	uint8_t             bat_heater_mode;   //电池加热模式，0=manual，1=auto
} eps_bat_t;


/**
*@solararray
*********************************************************************************************
*/

//sv inputs state enum
typedef enum{
	SV_OFF = 0,
	SV_ON,
	SV_ERR,
	SV_HDERR,
}sv_state_t;
//CONVERTER state enum
typedef enum{
	CONV_OFF,
	CONV_ON,
	CONV_ERR,
	CONV_UV,           //converter output voltage too low
	CONV_OV,           //converter output voltage too high
	CONV_OC,           //converter output current too large
}conv_state_t;
//output state enum
typedef enum{
	OUTPUT_OFF = 0, //output off     output is turned off
	OUTPUT_ON,      //output on      output is turned on
	OUTPUT_ERR,     //output err     OUTPUT_TRY 和 OUTPUT_HDTRY 只有在OUT_ERR之后才会出现
	OUTPUT_OFFERR,  //当output 处于off状态时，由于不可知因素导致通道处于开通状态
	OUTPUT_SFTRY,     //mannual autotry
	OUTPUT_HDTRY,    //hardware autotry
	OUTPUT_TRYOK,   //output aytotry ok,之后进入OUTPUT_ON
	OUTPUT_HDERR,   //output hardware err,this channel output will not be opened
}output_state_t;



/**
*@eps sates struct
*********************************************************************************************
*/
typedef struct eps_hk_state_s{
	sv_state_t sv_state[SV_NUM];     //eps solar input state
	conv_state_t conv_state[2];   //eps regulator converter state
	output_state_t out_state[REG_NUM + UREG_NUM];    //eps regulated and unregulated ouputs states
	uint16_t out_faults;      //eps ALL outputs fault count number 
	uint16_t out_ocp[REG_NUM + UREG_NUM];//eps out over current soft detecct count number
	uint16_t out_Ton[REG_NUM + UREG_NUM];      // outputs set n seconds to turn on the output
	uint16_t out_Toff[REG_NUM + UREG_NUM];     // outputs set n second to turn off the output
	uint16_t out_fault[REG_NUM + UREG_NUM];    //eps every regulated and unregulated outputs fault count number
	uint8_t eps_state_reserved; //eps state reserved
} eps_hk_state_t;




/**
*@
*********************************************************************************************
*/
eps_hk_adc_t * adc_to_real(uint16_t *adc_uint,eps_hk_adc_t *adc_dest);
output_state_t out_en(uint8_t chan,FunctionalState NewState);
int8_t outall_en(FunctionalState NewState);
void eps_enter_normal(void);
void eps_enter_safe(void);
void eps_entern_critical(void);
void eps_allin_off(void);
void eps_allin_on(void);
uint8_t bat_heater_on(void);
uint8_t bat_heater_off(void);

void eps_data_Init(void);
void eps_data_handling(void);
static void temp_data_processing(eps_hk_adc_t * eps_adc, eps_hk_state_t * eps_state);
static void output_data_processing(eps_hk_adc_t *eps_adc,eps_hk_state_t *eps_state);
static void bat_data_processing(eps_hk_adc_t *eps_adc,eps_bat_t *eps_bat);
static void conv_data_processing(eps_hk_adc_t *eps_adc,eps_hk_state_t *eps_state);



#endif
/***********************************************************************************************/


