#include <includes.h>

void bsp_InitRNG(void)
{
    /* 使能RNG时钟源 */
    RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE);

    /* 使能RNG */
    RNG_Cmd(ENABLE);
}

