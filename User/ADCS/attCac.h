#ifndef _ATTCAC_H_
#define _ATTCAC_H_

#define PI 3.141592653589793

/* 将格里历时间转换成儒略日历 */
void timeFormTrans(double *tTime,int *year,int *month,int *day,int *hour,int *minute,double *second);

/* 利用时间信息，获得J2000到TEME坐标的转换矩阵 */
void cordMtxJToTEMEGet(double JToTEME[3][3],double *tTime);

/* 利用时间信息，获得TEME到WGS84坐标的转换矩阵 */
void cordMtxTEMEToWGSGet(double TEMEToWGS[3][3],double Vel_TEMEToWGS[3][3],double *tTime);

/* 利用时间信息，获得J2000到WGS84坐标的转换矩阵 */
void cordMtxJToWGSGet(double JToWGS[3][3],double *tTime);

/*******************************************************/
/* 利用坐标转换将地固系下的位置速度：GPS测量数据转换得到惯性系下轨道信息*/
void posVelInJ_GPSGet(double orbInfo[6],double PosInWGS[3],double VelInWGS[3],double *tTime);

/* 利用坐标转换将TEME系下的位置速度：SGP4预报轨道数据转换得到惯性系下轨道信息*/
void posVelInJ_SGP4Get(double orbInfo[6],double PosInTEME[3],double VelInTEME[3],double *tTime);

/* 获得卫星在地固系下的位置矢量 */
void posInWGSGet(double PosInWGS[3],double JToWGS[3][3],double orbInfo[6]);

/* 从轨道信息提取惯性到轨道的转换矩阵 */
void MtxJtoOGet(double MtxJtoO[3][3],double orbInfo[6]);

/* 将地固系下的信息，转换成大地系下的表达 */
void geoInfoGet(double GeoCord[2],double PosInWGS[3]);

/* 获得地固坐标系下的磁场分量 */
void ChkMagLst(double MagInFix[3],const double magTable[10682][3],double GeoCord[3]);

/* J2000下的磁场分量 */	
void MagJGet(double MagInJ[3],double JToWGS[3][3],double MagFix[3]);

/* 轨道坐标系下的磁场分量 */
void MagOGet(double MagInO[3],double MtxJtoO[3][3],double MagInJ[3]);

/* 获得俯仰角量测信息 */
void magPitGet(double *PitM,double Sat_MagInO[3],double magnetometer[3]);

/* 俯仰滤波器初始化模块 */
void pitFltInit(double PFSt[2],double PPF[2][2],double *PitM);

/* 俯仰滤波器的量测更新 */
void pitFltMagUpd(double PFSt[2],double PPF[2][2],double *PitM);

/* 俯仰滤波器的时间更新 */
void pitFltTimUpd(double PFSt[2],double PPF[2][2],double TqNomTmp[3]);

#endif
