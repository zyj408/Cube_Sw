#ifndef _BSP_IWDG_H
#define _BSP_IWDG_H



/** 描述    : 独立看门狗初始化*/
void bsp_InitIwdg(uint32_t _ulIWDGTime);

/** 描述    : 喂独立看门狗*/
void IWDG_Feed(void);

#endif
