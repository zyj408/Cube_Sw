/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                             (c) Copyright 2013; Micrium, Inc.; Weston, FL
*
*                   All rights reserved.  Protected by international copyright laws.
*                   Knowledge of the source code may not be used to write a similar
*                   product.  This file may only be used in accordance with a license
*                   and should not be redistributed in any way.
*********************************************************************************************************
*/
#ifndef  INCLUDES_PRESENT
#define  INCLUDES_PRESENT


/*
*********************************************************************************************************
*                                         STANDARD LIBRARIES
*********************************************************************************************************
*/

#include  <stdarg.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <math.h>
#include  <absacc.h>
#include  <string.h>
#include  <time.h>
/*
*********************************************************************************************************
*                                              LIBRARIES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <lib_def.h>
#include  <lib_ascii.h>
#include  <lib_math.h>
#include  <lib_mem.h>
#include  <lib_str.h>

#include  <diskio.h>
#include  <ff.h>

/*
*********************************************************************************************************
*                                                 OS
*********************************************************************************************************
*/

#include  <os.h>


/*
*********************************************************************************************************
*                                              APP / BSP
*********************************************************************************************************
*/

#define debug_enable 1
#define debug_adcs 0

#include  <bsp.h>
#include  "app_cfg.h"
#include  "bsp_os.h"

#include  "base.h"
#include  "globalavr.h"
#include  "globalavr_adcs.h"
/*
*********************************************************************************************************
*                                               SERIAL
*********************************************************************************************************
*/

#if (APP_CFG_SERIAL_EN == DEF_ENABLED)
#include  <app_serial.h>
#endif


/*
*********************************************************************************************************
*                                            INCLUDES END
*********************************************************************************************************
*/


#endif

