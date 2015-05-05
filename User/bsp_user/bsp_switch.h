#ifndef __BSP_SWITCH_H_
#define __BSP_SWITCH_H_

#define OBC_MTQ_ENABLE    1

/* GPS-A使能 */
#define GPIO_GPSA_EN_PORT	    GPIOE			
#define RCC_GPSA_EN_PORT      RCC_AHB1Periph_GPIOE
#define GPIO_GPSA_EN_PIN		  GPIO_Pin_6
/* GPS-B使能 */
#define GPIO_GPSB_EN_PORT	    GPIOE			
#define RCC_GPSB_EN_PORT      RCC_AHB1Periph_GPIOE
#define GPIO_GPSB_EN_PIN		  GPIO_Pin_2
/* MTQ使能 */
#define GPIO_MTQ_EN_PORT	    GPIOE			
#define RCC_MTQ_EN_PORT       RCC_AHB1Periph_GPIOE
#define GPIO_MTQ_EN_PIN		    GPIO_Pin_1
/* GPIO7 */
#define GPIO_RES_PORT	        GPIOE		
#define RCC_RES_PORT          RCC_AHB1Periph_GPIOE
#define GPIO_RES_PIN		      GPIO_Pin_0
/* 动量轮A使能 */
#define GPIO_WHEEL_A_PORT	    GPIOB			
#define RCC_WHEEL_A_PORT    	RCC_AHB1Periph_GPIOB
#define GPIO_WHEEL_A_PIN		  GPIO_Pin_4
/* 动量轮B使能 */
#define GPIO_WHEEL_B_PORT	    GPIOB			
#define RCC_WHEEL_B_PORT    	RCC_AHB1Periph_GPIOB
#define GPIO_WHEEL_B_PIN		  GPIO_Pin_3	
/* 太阳翻板使能 */
#define GPIO_SOLAR_EN_PORT	  GPIOG			
#define RCC_SOLAR_EN_PORT    	RCC_AHB1Periph_GPIOG
#define GPIO_SOLAR_EN_PIN		  GPIO_Pin_14
/* USB使能 */
#define GPIO_USB_EN_PORT	    GPIOG			
#define RCC_USB_EN_PORT    	  RCC_AHB1Periph_GPIOG
#define GPIO_USB_EN_PIN		    GPIO_Pin_15

#define GPIO_S0_EN_PORT	      GPIOD			
#define RCC_S0_EN_PORT    	  RCC_AHB1Periph_GPIOD
#define GPIO_S0_EN_PIN		    GPIO_Pin_3

#define GPIO_S1_EN_PORT	      GPIOG			
#define RCC_S1_EN_PORT    	  RCC_AHB1Periph_GPIOG
#define GPIO_S1_EN_PIN		    GPIO_Pin_8

#define GPIO_S2_EN_PORT	      GPIOG			
#define RCC_S2_EN_PORT    	  RCC_AHB1Periph_GPIOG
#define GPIO_S2_EN_PIN		    GPIO_Pin_7

#define GPIO_S3_EN_PORT	      GPIOG			
#define RCC_S3_EN_PORT    	  RCC_AHB1Periph_GPIOG
#define GPIO_S3_EN_PIN		    GPIO_Pin_6

#if OBC_MTQ_ENABLE
#define GPIO_MTQ_SLEEP_PORT	  GPIOG			
#define RCC_MTQ_SLEEP_PORT    RCC_AHB1Periph_GPIOG
#define GPIO_MTQ_SLEEP_PIN		GPIO_Pin_11
/* 电机1方向 */
#define GPIO_MTQ1_DIR_PORT	  GPIOB			
#define RCC_MTQ1_DIR_PORT    	RCC_AHB1Periph_GPIOB
#define GPIO_MTQ1_DIR_PIN		  GPIO_Pin_0
/* 电机2方向 */
#define GPIO_MTQ2_DIR_PORT	  GPIOC			
#define RCC_MTQ2_DIR_PORT    	RCC_AHB1Periph_GPIOC
#define GPIO_MTQ2_DIR_PIN		  GPIO_Pin_5
/* 电机3方向 */
#define GPIO_MTQ3_DIR_PORT	  GPIOC			
#define RCC_MTQ3_DIR_PORT    	RCC_AHB1Periph_GPIOC
#define GPIO_MTQ3_DIR_PIN		  GPIO_Pin_4
#endif

/***************************************************************/
#define SW_GPSA_PIN()         GPIO_ReadOutputDataBit(GPIO_GPSA_EN_PORT, GPIO_GPSA_EN_PIN)
#define SW_GPSB_PIN()         GPIO_ReadOutputDataBit(GPIO_GPSB_EN_PORT, GPIO_GPSB_EN_PIN)
#define SW_WHEELA_PIN()       GPIO_ReadOutputDataBit(GPIO_WHEEL_A_PORT, GPIO_WHEEL_A_PIN)
#define SW_WHEELB_PIN()       GPIO_ReadOutputDataBit(GPIO_WHEEL_B_PORT, GPIO_WHEEL_B_PIN)
#define SW_MTQ_PIN()          GPIO_ReadOutputDataBit(GPIO_MTQ_EN_PORT, GPIO_MTQ_EN_PIN)
#define SW_USB_PIN()          GPIO_ReadOutputDataBit(GPIO_USB_EN_PORT, GPIO_USB_EN_PIN)
#define SW_SOLAR_PIN()        GPIO_ReadOutputDataBit(GPIO_SOLAR_EN_PORT, GPIO_SOLAR_EN_PIN)
#define SW_RES_PIN()        	GPIO_ReadOutputDataBit(GPIO_RES_PORT, GPIO_RES_PIN)

#define SW_S0_PIN()        	GPIO_ReadOutputDataBit(GPIO_S0_EN_PORT, GPIO_S0_EN_PIN)
#define SW_S1_PIN()        	GPIO_ReadOutputDataBit(GPIO_S1_EN_PORT, GPIO_S1_EN_PIN)
#define SW_S2_PIN()        	GPIO_ReadOutputDataBit(GPIO_S2_EN_PORT, GPIO_S2_EN_PIN)
#define SW_S3_PIN()        	GPIO_ReadOutputDataBit(GPIO_S3_EN_PORT, GPIO_S3_EN_PIN)


#if OBC_MTQ_ENABLE 
#define MTQ_POWER_PIN()       GPIO_ReadOutputDataBit(GPIO_MTQ_EN_PORT, GPIO_MTQ_EN_PIN)      
#define MTQ_SLEEP_PIN()       GPIO_ReadOutputDataBit(GPIO_MTQ_SLEEP_PORT, GPIO_MTQ_SLEEP_PIN)  
#define MTQ_DIR1_PIN()        GPIO_ReadOutputDataBit(GPIO_MTQ1_DIR_PORT, GPIO_MTQ1_DIR_PIN)      
#define MTQ_DIR2_PIN()        GPIO_ReadOutputDataBit(GPIO_MTQ2_DIR_PORT, GPIO_MTQ2_DIR_PIN)  
#define MTQ_DIR3_PIN()        GPIO_ReadOutputDataBit(GPIO_MTQ3_DIR_PORT, GPIO_MTQ3_DIR_PIN)  
#endif
/***************************************************************/
/* 开关量定义 */

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

#if OBC_MTQ_ENABLE
#define MTQ_WAKEN             GPIO_SetBits(GPIO_MTQ_SLEEP_PORT, GPIO_MTQ_SLEEP_PIN)
#define MTQ_SLEEP             GPIO_ResetBits(GPIO_MTQ_SLEEP_PORT, GPIO_MTQ_SLEEP_PIN)
#define MTQ1_DIR_POS          GPIO_SetBits(GPIO_MTQ1_DIR_PORT, GPIO_MTQ1_DIR_PIN)
#define MTQ1_DIR_NAG          GPIO_ResetBits(GPIO_MTQ1_DIR_PORT, GPIO_MTQ1_DIR_PIN)
#define MTQ2_DIR_POS          GPIO_SetBits(GPIO_MTQ2_DIR_PORT, GPIO_MTQ2_DIR_PIN)
#define MTQ2_DIR_NAG          GPIO_ResetBits(GPIO_MTQ2_DIR_PORT, GPIO_MTQ2_DIR_PIN)
#define MTQ3_DIR_POS          GPIO_SetBits(GPIO_MTQ3_DIR_PORT, GPIO_MTQ3_DIR_PIN)
#define MTQ3_DIR_NAG          GPIO_ResetBits(GPIO_MTQ3_DIR_PORT, GPIO_MTQ3_DIR_PIN)

#endif


void bsp_InitSwitch(void);
#endif

