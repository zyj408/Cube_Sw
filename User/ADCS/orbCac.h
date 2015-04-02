/**********************************************/
/* 功能:                                      */
/*      1 轨道计算的头文件                    */
/* 作者:                                      */
/*      郁丰                                  */
/**********************************************/
#ifndef _ORBCAC_H_
#define _ORBCAC_H_


/* 轨道信息融合 */
void orbInfoCom(double orbInfo[6],double J2000PV[6]);
/* 轨道传播 */
void orbPro(double orbInfo[6],double orbInc[3]);


#endif
