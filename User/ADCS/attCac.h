#ifndef _ATTCAC_H_
#define _ATTCAC_H_

#include <includes.h>
/* ��������ʱ��ת������������ */
void timeFormTrans(double *tTime,int *year,int *month,int *day,int *hour,int *minute,int *second);

/* ����ʱ����Ϣ�����J2000��TEME�����ת������ */
void cordMtxJToTEMEGet(double JToTEME[3][3],double *tTime);

/* ����ʱ����Ϣ�����TEME��WGS84�����ת������ */
void cordMtxTEMEToWGSGet(double TEMEToWGS[3][3],double Vel_TEMEToWGS[3][3],double *tTime);

/* ����ʱ����Ϣ�����J2000��WGS84�����ת������ */
void cordMtxJToWGSGet(double JToWGS[3][3],double *tTime);

/*******************************************************/
/* ��������ת�����ع�ϵ�µ�λ���ٶȣ�GPS��������ת���õ�����ϵ�¹����Ϣ*/
void posVelInJ_GPSGet(double orbInfo[6],double PosInWGS[3],double VelInWGS[3],double *tTime);

/* ��������ת����TEMEϵ�µ�λ���ٶȣ�SGP4Ԥ���������ת���õ�����ϵ�¹����Ϣ*/
void posVelInJ_SGP4Get(double orbInfo[6],double PosInTEME[3],double VelInTEME[3],double *tTime);

/* ��������ڵع�ϵ�µ�λ��ʸ�� */
void posInWGSGet(double PosInWGS[3],double JToWGS[3][3],double orbInfo[6]);

/* �ӹ����Ϣ��ȡ���Ե������ת������ */
void MtxJtoOGet(double MtxJtoO[3][3],double orbInfo[6]);

/* ���ع�ϵ�µ���Ϣ��ת���ɴ��ϵ�µı�� */
void geoInfoGet(double GeoCord[2],double PosInWGS[3]);

/* ��õع�����ϵ�µĴų����� */

void ChkMagLst(double MagInFix[3],const double magTable[10682][3],double GeoCord[3]);

/* J2000�µĴų����� */	
void MagJGet(double MagInJ[3],double JToWGS[3][3],double MagFix[3]);

/* �������ϵ�µĴų����� */
void MagOGet(double MagInO[3],double MtxJtoO[3][3],double MagInJ[3]);

/* ��ø�����������Ϣ */
void magPitGet(double *PitM,double Sat_MagInO[3],double magnetometer[3]);

/* �����˲�����ʼ��ģ�� */
void pitFltInit(double PFSt[2],double PPF[2][2],double *PitM);

/* �����˲������������ */
void pitFltMagUpd(double PFSt[2],double PPF[2][2],double *PitM);

/* �����˲�����ʱ����� */
void pitFltTimUpd(double PFSt[2],double PPF[2][2],double TqNomTmp[3]);

/* ����ϵ�¹��λ���ٶȵõ������������� */
void Get_KplInfo(double Kepler[6],double orbInfo[6]);

/* ��GPS������������и��� */
void GetTLEFromGPS(elsetrec *satrecFromGPS,double orbInfoGPS[6],double *tTime);

#endif
