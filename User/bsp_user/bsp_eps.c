#include <includes.h>

//adc map,对应eps_hk_adc_t 结构体中数据在读取到的原值中的位置，
//其中0--15对应于mcu adc ，16-31对应于ad7490读取到的数据
const uint8_t adc_map[]={29,30,10,7,6,5, /*c_s[6]*/
									28,11,9,8,4,3,/*v_s[6]*/
									16,12,24,/*c_com,c_bus,v_bus*/
									17,18,19,27,15,14,/*c_reg[6]*/
									20,22,23,21,13,/*c_ureg[5]*/
									31,0,26,25,/*temp_eps[4]*/
									1,2};/*temp_bat[2]*/

//定义adc数据比例，电压量为分压比，典型值49.9k/10k,转换关系为 V_real = V*Vdiv
//电流量为1/(R*G),典型值为R=0.051欧，G=25V/V;总线电流采样电阻R=0.051/2，转换关系为C=V*(1/R*G)
//温度量为V = 0.75+0.01*(T-25);转换关系为T = (V-0.75)/0.01+25
const float  adc_div[]={0.7843,0.7843,0.7843,0.7843,0.7843,0.7843,
									5.99,5.99,5.99,5.99,5.99,5.99,
									0.7843,0.7843,6.10,
									0.7843,0.7843,0.7843,0.7843,0.7843,0.7843,
									0.7843,0.7843,0.7843,0.7843,0.7843,
									1,1,1,1,1,1};

	
#define batSeries_num     2   //电池串联个数
#define batParallel       2		//电池并联个数												
#define batC              (2600*batParallel) //电池总容量mAh
#define batResolution     12 //电池电量曲线数组个数
#define batCurve_num      3  //电池电量曲线个数
									
//battery capacity to voltage map array				
//电池电量曲线表，电量计算根据表格数据									
const uint16_t batSOCcurve[batCurve_num + 1][batResolution]={
													4200,4060,3980,3920,3870,3820,3790,3770,3740,3680,3450,3000,  /*0.2c 开路电压下的电池容量*/
													4200,3970,3870,3790,3730,3680,3650,3620,3580,3510,3420,3000,   /*0.5c下电池容量*/
													4200,3800,3750,3650,3625,3580,3550,3520,3480,3400,3300,0000,   /*1c */
													100, 90,  80,  70,  60,  50,  40,  30,  20,  10,  5,   0};     /*电量百分比*/

//regulated and unregulated outputs current limitation array
//保护输出限流值，前6个为3.3v 或5v保护输出，后5个为母线保护输出
uint16_t c_out_limit[REG_NUM+UREG_NUM]={1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000};


/*
	eps_data_Init();     //数据初始化
	eps_adc_data = adc_to_real(,eps_adc_data); //将adc值填入eps_adc_data结构体
	eps_data_handling();//数据处理，eps状态量填入相关结构体
*/



/**********************************************************************************************************************/
/*
**将uint16_t格式的adc值转换为真实值
** 电压量单位 mv
** 电流量单位 ma
** 温度量单位 degc
*/
eps_hk_adc_t * adc_to_real(uint16_t *adc_uint,eps_hk_adc_t *adc_dest)
{
	uint8_t i;
	uint16_t * adc_dest1;
	int16_t * adc_dest2;
	adc_dest1 = (uint16_t*)adc_dest;
	adc_dest2 = (int16_t*)(&(adc_dest->temp_eps[0]));
	for(i=0;i<hk_adc_num-6;i++)
	{
		*(adc_dest1+i) = (uint16_t)(((float)(*(adc_uint+adc_map[i])))*adc_div[i]*ADC_REF/ADC_FullScale);
	}
	for(i=0;i<6;i++)
	{
		*(adc_dest2+i) = (int16_t)(((((float)(*(adc_uint+adc_map[i+26])))*ADC_REF/ADC_FullScale-750)/10)+25);
	}
	
	return adc_dest;
}

/*
#define SW_MTQ_ENABLE         GPIO_SetBits(GPIO_MTQ_EN_PORT, GPIO_MTQ_EN_PIN)
#define SW_MTQ_DISABLE        GPIO_ResetBits(GPIO_MTQ_EN_PORT, GPIO_MTQ_EN_PIN)
#define SW_WHEELA_ENABLE      GPIO_SetBits(GPIO_WHEEL_A_PORT, GPIO_WHEEL_A_PIN)
#define SW_WHEELA_DISABLE     GPIO_ResetBits(GPIO_WHEEL_A_PORT, GPIO_WHEEL_A_PIN)
#define SW_WHEELB_ENABLE      GPIO_SetBits(GPIO_WHEEL_B_PORT, GPIO_WHEEL_B_PIN)
#define SW_WHEELB_DISABLE     GPIO_ResetBits(GPIO_WHEEL_B_PORT, GPIO_WHEEL_B_PIN)
#define SW_GPSA_ENABLE        GPIO_SetBits(GPIO_GPSA_EN_PORT, GPIO_GPSA_EN_PIN)
#define SW_GPSA_DISABLE       GPIO_ResetBits(GPIO_GPSA_EN_PORT, GPIO_GPSA_EN_PIN)
#define SW_GPSB_ENABLE        GPIO_SetBits(GPIO_GPSB_EN_PORT, GPIO_GPSB_EN_PIN)
#define SW_GPSB_DISABLE       GPIO_ResetBits(GPIO_GPSB_EN_PORT, GPIO_GPSB_EN_PIN)
#define SW_USB_ENABLE         GPIO_SetBits(GPIO_USB_EN_PORT, GPIO_USB_EN_PIN)
#define SW_USB_DISABLE        GPIO_ResetBits(GPIO_USB_EN_PORT, GPIO_USB_EN_PIN)
#define SW_SOLAR_ENABLE       GPIO_SetBits(GPIO_SOLAR_EN_PORT, GPIO_SOLAR_EN_PIN)
#define SW_SOLAR_DISABLE      GPIO_ResetBits(GPIO_SOLAR_EN_PORT, GPIO_SOLAR_EN_PIN)
#define SW_RES_ENABLE         GPIO_SetBits(GPIO_RES_PORT, GPIO_RES_PIN)
#define SW_RES_DISABLE        GPIO_ResetBits(GPIO_RES_PORT, GPIO_RES_PIN)
#define SW_S0_ENABLE         GPIO_SetBits(GPIO_S0_EN_PORT, GPIO_S0_EN_PIN)
#define SW_S0_DISABLE        GPIO_ResetBits(GPIO_S0_EN_PORT, GPIO_S0_EN_PIN)
#define SW_S1_ENABLE         GPIO_SetBits(GPIO_S1_EN_PORT, GPIO_S1_EN_PIN)
#define SW_S1_DISABLE        GPIO_ResetBits(GPIO_S1_EN_PORT, GPIO_S1_EN_PIN)
#define SW_S2_ENABLE         GPIO_SetBits(GPIO_S2_EN_PORT, GPIO_S2_EN_PIN)
#define SW_S2_DISABLE        GPIO_ResetBits(GPIO_S2_EN_PORT, GPIO_S2_EN_PIN)
#define SW_S3_ENABLE         GPIO_SetBits(GPIO_S3_EN_PORT, GPIO_S3_EN_PIN)
#define SW_S3_DISABLE        GPIO_ResetBits(GPIO_S3_EN_PORT, GPIO_S3_EN_PIN)
	#define OUT_REG1   1
	#define OUT_REG2   2
	#define OUT_REG3   3	
	#define OUT_REG4   4
	#define OUT_REG5   5
	#define OUT_REG6   6	
	#define OUT_UREG1   7
	#define OUT_UREG2   8
	#define OUT_UREG3   9
	#define OUT_UREG4   10
	#define OUT_UREG5   11
	#define OUT_DEPLOY  12
//define with usage name
	#define OUT_ALL      0
	#define OUT_GPSA    OUT_REG1
	#define OUT_GPSB    OUT_REG2
	#define OUT_MTQ     OUT_UREG1
	#define OUT_WHEELA  OUT_UREG2
	#define OUT_WHEELB  OUT_UREG3
	#define OUT_USB     OUT_UREG4
	#define OUT_FIPEX5V OUT_REG5
	#define OUT_FIPEX3V OUT_REG6
	#define OUT_HEAT    OUT_UREG5
	#define OUT_RES     OUT_REG3
*/
/**
*@bref 电源板输出控制
*@detail
*@para   chan  对应通道 0表示所有
         NewState  通道状态
*@reteval  none
*/
void out_en(uint8_t chan,FunctionalState NewState)
{
	switch (chan)
	{
		case OUT_ALL:
			if(NewState == ENABLE)
			{
				SW_MTQ_ENABLE;
				SW_WHEELA_ENABLE;
				SW_WHEELB_ENABLE; 	
				SW_GPSA_ENABLE;
				SW_GPSB_ENABLE;
				SW_USB_ENABLE;
				//SW_S2_ENABLE; //fipex 5v
				//SW_S3_ENABLE; //fipex 3v
				
			}
			else
			{
				SW_MTQ_DISABLE;
				SW_WHEELA_DISABLE;
				SW_WHEELB_DISABLE; 	
				SW_GPSA_DISABLE;
				SW_GPSB_DISABLE;
				SW_USB_DISABLE;
				//SW_S2_DISABLE; //fipex 5v
				//SW_S3_DISABLE; //fipex 3v
			}
			break;
		case OUT_GPSA:
			if(NewState == ENABLE) SW_GPSA_ENABLE;
			else SW_GPSA_DISABLE;
			break;
		case OUT_GPSB:
			if(NewState == ENABLE) SW_GPSB_ENABLE;
			else SW_GPSB_DISABLE;
			break;
		case OUT_MTQ:
			if(NewState == ENABLE) SW_MTQ_ENABLE;
			else SW_MTQ_DISABLE;
			break;
		case OUT_WHEELA:
			if(NewState == ENABLE) SW_WHEELA_ENABLE;
			else SW_WHEELA_DISABLE;
			break;
		case OUT_WHEELB:
			if(NewState == ENABLE) SW_WHEELB_ENABLE;
			else SW_WHEELB_DISABLE;
			break;
		case OUT_USB:
			if(NewState == ENABLE) SW_USB_ENABLE;
			else SW_USB_DISABLE;
			break;
		case OUT_FIPEX5V:
			if(NewState == ENABLE) SW_S2_ENABLE;
			else	SW_S2_DISABLE;
			break;
		case OUT_FIPEX3V:
			if(NewState == ENABLE) SW_S3_ENABLE;
			else SW_S3_DISABLE;
			break;
		case OUT_HEAT:
			if(NewState == ENABLE) SW_S0_ENABLE;
			else SW_S0_DISABLE;
			break;
		case OUT_RES:
			if(NewState == ENABLE) SW_RES_ENABLE;
			else SW_RES_DISABLE;
			break;
		case OUT_DEPLOY:
			if(NewState == ENABLE) SW_SOLAR_ENABLE;
			else SW_SOLAR_DISABLE;
			break;
		default:
			break;
	}
}
void eps_enter_normal(void)
{
	if(eps_state.out_state[OUT_GPSA] == OUTPUT_ON)
		out_en(OUT_GPSA,ENABLE);
	if(eps_state.out_state[OUT_GPSB] == OUTPUT_ON)
		out_en(OUT_GPSB,ENABLE);
	if(eps_state.out_state[OUT_MTQ] == OUTPUT_ON)
		out_en(OUT_MTQ,ENABLE);
	if(eps_state.out_state[OUT_WHEELA] == OUTPUT_ON)
		out_en(OUT_WHEELA,ENABLE);
	if(eps_state.out_state[OUT_WHEELB] == OUTPUT_ON)
		out_en(OUT_WHEELB,ENABLE);
	if(eps_state.out_state[OUT_USB] == OUTPUT_ON)
		out_en(OUT_USB,ENABLE);
	if(eps_state.out_state[OUT_FIPEX5V] == OUTPUT_ON)
		out_en(OUT_FIPEX5V,ENABLE);
	if(eps_state.out_state[OUT_FIPEX3V] == OUTPUT_ON)
		out_en(OUT_FIPEX3V,ENABLE);	
}
void eps_enter_safe(void)
{
	out_en(OUT_MTQ,DISABLE);
}
void eps_enter_critical(void)
{
	out_en(OUT_ALL,DISABLE);
	out_en(OUT_HEAT,DISABLE);
}
void eps_allin_off(void)
{
	
}
void eps_allin_on(void)
{	
	
}
void bat_heater_on(void)
{
	out_en(OUT_HEAT,ENABLE);
}
void bat_heater_off(void)
{
	out_en(OUT_HEAT,DISABLE);
}

void eps_data_Init(void)
{
	//init eps_adc
	memset(&eps_adc_data,0,sizeof(eps_hk_adc_t));
	//init eps_state structure 
	memset(&eps_state,0,sizeof(eps_hk_state_t));
	eps_state.conv_state[0] = CONV_ON;
	eps_state.conv_state[1] = CONV_ON;
	//init eps_bat structure
	memset(&eps_bat,0,sizeof(eps_bat_t));
	eps_bat.bat_heater_mode = 0;
	eps_bat.bat_fullcap = 38480000;
	eps_bat.bat_percent = 100;
	eps_bat.bat_total_percent = 100;
	eps_bat.bat_state = BAT_NORMAL;
	
}
void eps_data_handling()
{
	//
	bat_data_processing(&eps_adc_data,&eps_bat);
	//
	output_data_processing(&eps_adc_data,&eps_state);
	//
//	conv_data_processing(&eps_adc_data,&eps_state);
	//
	temp_data_processing(&eps_adc_data,&eps_state);
}
/**
*@bref    eps tempture manager funtion
*@detail
*@para    eps_adc   
*         eps_state
*@reteval none
*/
static void temp_data_processing(eps_hk_adc_t * eps_adc, eps_hk_state_t * eps_state)
{
	
}
/**
*@bref converter state 
*@para volt : converter voltage
*      ....
*@reteval    state
*/
static conv_state_t conv_state(uint16_t volt,uint16_t max,uint16_t min,uint16_t errmax,uint16_t errmin)
{
	if(volt > errmax || volt < errmin) return CONV_ERR;
	else if(volt > max) return CONV_OV;
	else if(volt < min) return CONV_UV;
	
		return 0;
}
/**
*@bref 
*@detail
//CONVERTER state enum
typedef enum{
	CONV_OFF,
	CONV_ON,
	CONV_ERR,
	CONV_UV,           //converter output voltage too low
	CONV_OV,           //converter output voltage too high
	CONV_OC,           //converter output current too large
}conv_state_t;
*@para
*@reteval
*/
/*
static void conv_data_processing(eps_hk_adc_t *eps_adc,eps_hk_state_t *eps_state)
{
	uint8_t i;
	uint16_t conv1_max = 5250;
	uint16_t conv1_min = 4750;
	uint16_t conv1_errmax = 6000;
	uint16_t conv1_errmin = 4000;
	uint16_t conv2_max = 3450;
	uint16_t conv2_min = 3150;
	uint16_t conv2_errmax = 4000;
	uint16_t conv2_errmin = 2000;
	uint16_t conv_volt = 0;

	conv_volt = eps_adc->v_5v;
	conv_state(conv_volt,conv1_max,conv1_min,conv1_errmax,conv1_errmin);
	conv_volt = eps_adc->v_3v;
	conv_state(conv_volt,conv2_max,conv2_min,conv2_errmax,conv2_errmin);
}
*/
/**
*@bref    output protection function
*@para
*@reteval
typedef enum{
	OUTPUT_OFF = 0, //output off
	OUTPUT_ON,      //output on
	OUTPUT_ERR,     //output err     OUTPUT_TRY 和 OUTPUT_HDTRY 只有在OUT_ERR之后才会出现
	OUTPUT_SFTRY,   //mannual soft autotry
	OUTPUT_HDTRY,    //hardware autotry
	OUTPUT_TRYOK,   //output aytotry ok,之后进入OUTPUT_ON 或 OUTPUT_OFF状态
	OUTPUT_HDERR,   //output hardware err,this channel output will not be opened
}output_state_t;
*/
static void output_data_processing(eps_hk_adc_t *eps_adc,eps_hk_state_t *eps_state)
{
	uint8_t i;
	// set turn on time and turn off time
	// if the eps_state.out_Ton > ton ,then the function is not active	
	// if the eps_state.out_Toff > toff ,then the function is not active	
	uint16_t ton = 10;
	uint16_t toff = 10; 
	static output_state_t pre_state[REG_NUM + UREG_NUM] = {0};//define output pre state enum variable

	for(i=0;i<(REG_NUM + UREG_NUM);i++)
	{
		switch (eps_state->out_state[i])
		{
			case OUTPUT_OFF:
				out_en(i,DISABLE);
				break;
			case OUTPUT_ON:
				out_en(i,ENABLE);
				if(eps_adc->c_out[i] > c_out_limit[i])
				{
					eps_state->out_state[i] = OUTPUT_ERR;
					eps_state->out_ocp[i]++;
					//用于在OUTPUT_ERR中的判断
					pre_state[i] = OUTPUT_ON;//
				}
				else
				{
					pre_state[i] = OUTPUT_OFF;//
					break;
				}
//				break;
			case OUTPUT_ERR:
				out_en(i,DISABLE);
				//表明是由于软件设置电流限制导致的error
				if(pre_state[i] == OUTPUT_ON)//the error was caused by over current case
				{
					eps_state->out_state[i] = OUTPUT_SFTRY;
					eps_state->out_Ton[i] = ton; 
					pre_state[i] = OUTPUT_OFF; //SET as default value
				}
				else//表明是由于硬件fault中断导致的error
				{
					eps_state->out_state[i] = OUTPUT_HDTRY;			
				}
				eps_state->out_fault[i]++; //单个通道对应的错误加1
				eps_state->out_faults++;//所有的错误量加1

				//注释掉break；程序继续往下走
//				break;
				//软件设定等待一段时间开启故障通道，设定时间在上一个case
			case OUTPUT_SFTRY:
				if((eps_state->out_Ton[i] > 0) && (eps_state->out_Ton[i] < ton+1)) eps_state->out_Ton[i]--;
				//设定时间到，打开故障通道
				if(eps_state->out_Ton == 0)
				{
					out_en(i,ENABLE);
					//设定故障通道关断时间，
					eps_state->out_Toff[i] = toff;
				}
				//判断是否在Toff时间内
				//在此时间内没有出现电流超出限制事件，则保持通道开通
				//否则关闭通道 进入OUTPUT_HDERR 模式
				//此模式可能是输出通道硬件故障也可能是负载故障
				if((eps_state->out_Toff[i] > 0) && (eps_state->out_Toff[i] < toff+1)) 
				{
					eps_state->out_Toff[i]--;
					if(eps_adc->c_out[i] > c_out_limit[i])
					{
						eps_state->out_state[i] = OUTPUT_HDERR;
						eps_state->out_Toff[i] = 0;
						out_en(i,DISABLE);
					}
					else
					{
						if(eps_state->out_Toff[i] == 0) 
						{
							eps_state->out_state[i] = OUTPUT_TRYOK;
						}
					}
				}
//				break;
			case OUTPUT_HDTRY:
				//首次进入HRTRY模式，进行设定
				if(pre_state[i] != OUTPUT_ERR)
				{ 
					//打开故障通道，设定一定时间
					//在规定时间内跳出故障模式则表明通道正常
					out_en(i,ENABLE);
					//通道状态设为打开，在1S内若再次发生，则中断故障比软件限流故障要快，会再次进入该模式
					eps_state->out_state[i] = OUTPUT_TRYOK;
					eps_state->out_Toff[i] = toff;
					pre_state[i] = OUTPUT_ERR;
				}
				//后面再次进入
				else
				{ //在规定时间内没能跳出，则关断当前通道
					if(eps_state->out_Toff[i]-- == 0)
					{
						out_en(i,DISABLE);
						//设定钙通道HARD ERROR
						eps_state->out_state[i] = OUTPUT_HDERR;
					}
				}
//				break;
			case OUTPUT_TRYOK:
				eps_state->out_state[i] = OUTPUT_ON;
				pre_state[i] = OUTPUT_OFF;
				out_en(i,ENABLE);
				break;
			case OUTPUT_HDERR:
				//lock the output state in HARDWARE ERROE STATE,
				eps_state->out_state[i] = OUTPUT_HDERR;
				//lock the output in disable state
				out_en(i,DISABLE);
				pre_state[i] = OUTPUT_OFF;
				break;
			default:
			break;
		}
	}

}

/**
*@bref battery permanent capacity percent calculation function
*@para
*@eteval
*/
static uint16_t batSOCpermanent(eps_bat_t *bat,uint16_t volt,uint16_t curr)
{
	return 0;
}
/**
*@bref      battery remaining capacity function
*@bref      the function is realized in  voltage method
*@para     :
**    bat  :eps_bat struct ,it will be filled with the result
**		volt :battery voltage
**    curr :battery current
*@reteval  : the calculated result of the battery remaining capacity
*/
static uint16_t batSOC(eps_bat_t *bat,uint16_t volt,uint16_t curr)
{
	uint8_t i;
	uint8_t curve_select; 
	uint16_t soc;
	uint16_t volt_step,volt_diff,soc_diff;
	if(curr < batC*0.2)
	{
		curve_select =0;
	}
	else if(curr < batC*0.5)
	{
		curve_select = 1;
	}
	else  //curr > 0.5*batC 
	{
		curve_select = 2;
	}
	
	for(i=0;i<batResolution;i++)
	{
		if(volt > batSeries_num * batSOCcurve[curve_select][i])
		{
			if(i == 0)//full of charged
			{
				soc = 100;
				break;
			}

			volt_diff = volt/batSeries_num - batSOCcurve[curve_select][i];

			volt_step = batSOCcurve[curve_select][i-1] - batSOCcurve[curve_select][i];

			soc_diff = batSOCcurve[batCurve_num][i-1] - batSOCcurve[batCurve_num][i];

			soc = soc_diff * volt_diff/volt_step + batSOCcurve[batCurve_num][i];
			break;
		}
	}
	bat->bat_percent = soc;
	return soc;
}
/**
*@bref     battery process function
*@detail   根据电池电压，判断当前所处状态
*          根据电池电流判断充放电状态
*          根据电池电池板温度判断电池板当前状态，是否需要加热
*          调用batSOC() 完成电池剩余电量的计算
*          调用batSOCpermanent() 完成电池容量的计算
*@para     none
*@reteval  none
*/
static void bat_data_processing(eps_hk_adc_t *eps_adc,eps_bat_t *eps_bat)
{
	uint16_t volt_hyst = 50;//mV
	uint16_t curr_hyst = 2;//mA
	uint16_t bat_volt;
	uint16_t bat_curr,bat_curr_in,bat_curr_out;
	int16_t  bat_temp1,bat_temp2;
	uint16_t bat_pre_state;
	bat_volt = eps_adc->v_bus;
	bat_curr_in = eps_adc->c_sv[0]+eps_adc->c_sv[1]+eps_adc->c_sv[2]
								+eps_adc->c_sv[3]+eps_adc->c_sv[4]+eps_adc->c_sv[5];
	bat_curr_out = eps_adc->c_bus;
	bat_temp1 = eps_adc->temp_bat[0];
	bat_temp2 = eps_adc->temp_bat[1];
	bat_pre_state = eps_bat->bat_state;
/* 根据电压判断当前状态，为避免震荡，设定电压降低时根据设定值判断
	*电压升高时，判断值增加0.05v，即50mV的滞回量
	*************************************************************
	*/	
	switch (bat_pre_state)
	{//由满充降到正常电压范围，打开所有输入通道
		case BAT_OV:
			if(bat_volt < bat_full_volt)
			{
				eps_allin_on();
				eps_enter_normal();
				eps_bat->bat_state = BAT_NORMAL;
			}
			break;
		case BAT_NORMAL:
			//正常模式进入满充状态，关闭所有输入
			if(bat_volt > bat_full_volt + volt_hyst)
			{
				eps_allin_off();
				eps_bat->bat_state = BAT_OV;
			}
			//正常电压降到低电压，进入safe模式
			else if(bat_volt < bat_normal_volt)
			{
				eps_enter_safe();
				eps_bat->bat_state = BAT_UV;
			}
			break;
		case BAT_UV:
			//低电压升高到正常电压范围，进入normal模式
			if(bat_volt > bat_normal_volt + volt_hyst)
			{
				eps_enter_normal();
				eps_bat->bat_state = BAT_NORMAL;
			}
			//低电压降到critical电压以下，进入critical模式
			if(bat_volt < bat_critical_volt)
			{
				eps_enter_critical();
				eps_bat->bat_state = BAT_CRITICAL;
			}
			break;
		case BAT_CRITICAL:
			//critical电压升高到低电压模式
			if(bat_volt > bat_critical_volt + volt_hyst)
			{
				eps_enter_safe();
				eps_bat->bat_state = BAT_UV;
			}
			//critical电压降到接近硬件关断电压
			if(bat_volt < bat_hardoff_volt + volt_hyst)
			{
				eps_bat->bat_state = BAT_HARDOFF;
			}
			break;
		case BAT_HARDOFF:
			//接近硬件关断电压升高到critical电压
			if(bat_volt > bat_hardoff_volt + volt_hyst)
			{
				eps_bat->bat_state = BAT_CRITICAL;
			}
			break;
		default:
			break;
	}
	
	/*根据电流情况判断充放电状态
	*************************************************************
	*/
	if(bat_curr_in > bat_curr_out + curr_hyst)
	{
		eps_bat->bat_charge = BAT_CHARGE;
		bat_curr = bat_curr_in - bat_curr_out;
	}
	else if(bat_curr_in < bat_curr_out - curr_hyst)
	{
		eps_bat->bat_charge = BAT_DISCH;
		bat_curr = bat_curr_out - bat_curr_in;
	}
	else
	{
		eps_bat->bat_charge = BAT_NOCURR;
		bat_curr = 0;
	}
	/*根据温度判断电池加热
	*0 = manual ;1 = auto
	*************************************************************
	*/
	if(eps_bat->bat_heater_mode )
	{
		switch (eps_bat->bat_heater_status)
		{
			case NO:
			if(bat_temp1 < bat_heater_low || bat_temp2 < bat_heater_low)
			{
				if(eps_bat->bat_state > BAT_CRITICAL)
				{
					bat_heater_on();
				  eps_bat->bat_heater_status = YES;
				}
				
			}
				break;
			case YES:
				if(bat_temp1 > bat_heater_high || bat_temp2 > bat_heater_high)
				{
					bat_heater_off();
					eps_bat->bat_heater_status = NO;
				}
				break;
			default:
				break;
		}
	}
	
	/**
	******************************************************************
	*@batSOC  battery SOC function
	*/
	batSOC(eps_bat,bat_volt,bat_curr);
	batSOCpermanent(eps_bat,bat_volt,bat_curr);
}

























/**********************************************************************************/

