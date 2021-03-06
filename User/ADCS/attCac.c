/**********************************************/
#include <includes.h>

/*******************************************************/
/* 将格里历时间转换成儒略日历 */
/*由于太阳计算模型是时间的函数，而数据综合系统提供的是通用的格里历时间，转换成方便计算的儒略历*/
void timeFormTrans(double *tTime,int *year,int *month,int *day,int *hour,int *minute,int *second)
{
    double fstItem,secItem,thiItem,tmpx,tmpy;  /* 计算使用的临时变量*/
    tmpy = ((*month) - 14)/12.0;
    modf(tmpy,&tmpx);
    tmpy = 1461*((*year) + 4800 + tmpx)/4.0;
    modf(tmpy,&fstItem);
       					/* 第一项 */
    tmpy = 367*((*month) - 2 - tmpx*12)/12.0;
    modf(tmpy,&secItem);
        				/* 第二项 */
    tmpy = ((*year) + 4900 + tmpx)/100.0;
    modf(tmpy,&tmpx);
    tmpy = 3*tmpx/4.0;
    modf(tmpy,&thiItem);
        			
    *tTime = (*day) - 32075 + fstItem + secItem - thiItem - 0.5;            
    *tTime = *tTime + (*hour)/24.0 + (*minute)/1440.0 + (*second)/86400.0; /*单位为日*/
    
}


/*******************************************************/
/* 利用时间信息，获得J2000到TEME坐标的转换矩阵 */
void cordMtxJToTEMEGet(double JToTEME[3][3],double *tTime)
{
    double Tu,Td,DA,ZA,TA,KesiMean,OmegaMoon,LongiSun,DeltaKesi,DeltaFai,KesiReal;
    double tmpx,tmpy,tmpz,tmpsin,tmp,tmpcos,DATrans[3][3],ZATrans[3][3],TATrans[3][3];
    double TransKm[3][3],TransKr[3][3],TransDf[3][3];
    double tmpTrans[3][3],TransX[3][3],TransY[3][3];
	
    Tu = (*tTime) - 2451545.0;
    Td = Tu; /*保存变量Tu，以便计算格林尼治平恒星时*/ 
    Tu = Tu/36525.0;
    
    tmpx = 180.0*3600/PI;tmpy = Tu*Tu;tmpz = tmpy*Tu;
    DA = (2306.2181*Tu + 0.30188*tmpy + 0.017998*tmpz)/tmpx; /*赤道面欧拉角之一*/
    ZA = (2306.2181*Tu + 1.09468*tmpy + 0.018203*tmpz)/tmpx; /*赤道面欧拉角之二*/
    TA = (2004.3109*Tu - 0.42665*tmpy - 0.041833*tmpz)/tmpx; /*赤道面欧拉角之三*/
    tmpcos = cos(DA);tmpsin = sin(DA);
    DATrans[0][0] = tmpcos;DATrans[0][1] = -tmpsin;DATrans[0][2] = 0.0; /*赤道面欧拉角DA构成的坐标转换矩阵*/
    DATrans[1][0] = tmpsin;DATrans[1][1] = tmpcos;DATrans[1][2] = 0.0;
    DATrans[2][0] = 0.0;DATrans[2][1] = 0.0;DATrans[2][2] = 1.0;
    tmpcos = cos(ZA);tmpsin = sin(ZA);
    ZATrans[0][0] = tmpcos;ZATrans[0][1] = -tmpsin;ZATrans[0][2] = 0.0;/*赤道面欧拉角ZA构成的坐标转换矩阵*/
    ZATrans[1][0] = tmpsin;ZATrans[1][1] = tmpcos;ZATrans[1][2] = 0.0;
    ZATrans[2][0] = 0.0;ZATrans[2][1] = 0.0;ZATrans[2][2] = 1.0;
    tmpcos = cos(TA);tmpsin = sin(TA);
    TATrans[0][0] = tmpcos;TATrans[0][1] = 0.0;TATrans[0][2] = -tmpsin;/*赤道面欧拉角TA构成的坐标转换矩阵*/
    TATrans[1][0] = 0.0;TATrans[1][1] = 1.0;TATrans[1][2] = 0.0;
    TATrans[2][0] = tmpsin;TATrans[2][1] = 0.0;TATrans[2][2] = tmpcos;
    mtxMtp((double *)tmpTrans,(double *)ZATrans,3,3,(double *)TATrans,3,3);
    mtxMtp((double *)TransX,(double *)tmpTrans,3,3,(double *)DATrans,3,3);
    					/* 从J2000到瞬时平赤道坐标系的转换关系 */
    tmp = 180.0/PI;		/*角度-弧度转换参数*/		
    KesiMean = (84381.448 - 46.815*Tu - 0.00059*tmpy + 0.001813*tmpz)/tmpx; /*平黄赤交角*/
    OmegaMoon = (125.04 - 0.052954*Td)/tmp;/*月球的升交点赤经*/
    LongiSun = (280.47 + 0.98565*Td)/tmp;  /* 太阳的平黄经*/
    DeltaKesi = (9.2025*cos(OmegaMoon) + 0.5736*cos(2*LongiSun))/tmpx;  /*交角章动*/
    DeltaFai = (-17.1996*sin(OmegaMoon) - 1.3187*sin(2*LongiSun))/tmpx; /*黄经章动*/
    KesiReal = KesiMean + DeltaKesi;
    tmpcos = cos(KesiMean);tmpsin = sin(KesiMean);
    TransKm[0][0] = 1.0;TransKm[0][1] = 0.0;TransKm[0][2] = 0.0;
    TransKm[1][0] = 0.0;TransKm[1][1] = tmpcos;TransKm[1][2] = tmpsin;
    TransKm[2][0] = 0.0;TransKm[2][1] = -tmpsin;TransKm[2][2] = tmpcos;
    tmpcos = cos(KesiReal);tmpsin = sin(KesiReal);
    TransKr[0][0] = 1.0;TransKr[0][1] = 0.0;TransKr[0][2] = 0.0;
    TransKr[1][0] = 0.0;TransKr[1][1] = tmpcos;TransKr[1][2] = -tmpsin;
    TransKr[2][0] = 0.0;TransKr[2][1] = tmpsin;TransKr[2][2] = tmpcos;
    tmpcos = cos(DeltaFai);tmpsin = sin(DeltaFai);
    TransDf[0][0] = tmpcos;TransDf[0][1] = -tmpsin;TransDf[0][2] = 0.0;
    TransDf[1][0] = tmpsin;TransDf[1][1] = tmpcos;TransDf[1][2] = 0.0;
    TransDf[2][0] = 0.0;TransDf[2][1] = 0.0;TransDf[2][2] = 1.0;
    mtxMtp((double *)tmpTrans,(double *)TransKr,3,3,(double *)TransDf,3,3);
    mtxMtp((double *)TransY,(double *)tmpTrans,3,3,(double *)TransKm,3,3);
    					/* 从瞬时平赤道坐标系到真赤道坐标的转换关系 */
		mtxMtp((double *)JToTEME,(double *)TransY,3,3,(double *)TransX,3,3);
		
}

/*******************************************************/
/* 利用时间信息，获得TEME到WGS84坐标的转换矩阵 */
void cordMtxTEMEToWGSGet(double TEMEToWGS[3][3],double Vel_TEMEToWGS[3][3],double *tTime)
{
		double tmp,tmpx,tmpy,tmpz,Tu,Td,DeltaFai,KesiReal;
		double KesiMean,OmegaMoon,LongiSun,DeltaKesi;
		double tmpcos,tmpsin,TransX[3][3],TransY[3][3];
		double thetadot=7.292123518e-5;
	
		Tu = (*tTime) - 2451545.0;
		Td = Tu;
		Tu = Tu/36525.0;
    
    tmpx = 180.0*3600/PI;tmpy = Tu*Tu;tmpz = tmpy*Tu;
    tmp = 180.0/PI;		/*角度-弧度转换参数*/		
    KesiMean = (84381.448 - 46.815*Tu - 0.00059*tmpy + 0.001813*tmpz)/tmpx; /*平黄赤交角*/
    OmegaMoon = (125.04 - 0.052954*Td)/tmp;/*月球的升交点赤经*/
    LongiSun = (280.47 + 0.98565*Td)/tmp;  /* 太阳的平黄经*/
    DeltaKesi = (9.2025*cos(OmegaMoon) + 0.5736*cos(2*LongiSun))/tmpx;  /*交角章动*/
    DeltaFai = (-17.1996*sin(OmegaMoon) - 1.3187*sin(2*LongiSun))/tmpx; /*黄经章动*/
    KesiReal = KesiMean + DeltaKesi;
		
    tmp = 2.0*PI/24;
    tmpx = 18.697374558*tmp;tmpy = 24.06570982441908*tmp;
    tmpz = tmpx + tmpy*Td;  /*获得格林尼治平恒星时*/  
	
    tmp = tmpz + DeltaFai*cos(KesiReal); /*获得格林尼治真恒星时角*/ 
    tmpcos = cos(tmp);tmpsin = sin(tmp);
    TransX[0][0] = tmpcos;TransX[0][1] = tmpsin;TransX[0][2] = 0.0;
    TransX[1][0] = -tmpsin;TransX[1][1] = tmpcos;TransX[1][2] = 0.0;
    TransX[2][0] = 0.0;TransX[2][1] = 0.0;TransX[2][2] = 1.0;
    					/* 从瞬时真赤道坐标系到准地固系的转换关系 */
    TransY[0][0] = -tmpsin*thetadot;TransY[0][1] = tmpcos*thetadot;TransY[0][2] = 0.0;
    TransY[1][0] = -tmpcos*thetadot;TransY[1][1] = -tmpsin*thetadot;TransY[1][2] = 0.0;
    TransY[2][0] = 0.0;TransY[2][1] = 0.0;TransY[2][2] = 0.0;
	
    mtxCpy((double *)TEMEToWGS,(double *)TransX,3,3);
		mtxCpy((double *)Vel_TEMEToWGS,(double *)TransY,3,3);
	
}

/*******************************************************/
/* 利用时间信息，获得J2000到WGS84坐标的转换矩阵 */
void cordMtxJToWGSGet(double JToWGS[3][3],double *tTime)
{
		double JToTEME[3][3],TEMEToWGS[3][3],Vel_TEMEToWGS[3][3];
		cordMtxJToTEMEGet(JToTEME,tTime);
		cordMtxTEMEToWGSGet(TEMEToWGS,Vel_TEMEToWGS,tTime);
		mtxMtp((double *)JToWGS,(double *)TEMEToWGS,3,3,(double *)JToTEME,3,3);
}

/*******************************************************/
/* 利用坐标转换将地固系下的位置速度：GPS测量数据转换得到惯性系下轨道信息*/
void posVelInJ_GPSGet(double orbInfo[6],double PosInWGS[3],double VelInWGS[3],double *tTime)
{
    double JToTEME[3][3],TEMEToJ[3][3];
		double TEMEToWGS[3][3],WGSToTEME[3][3],Vel_TEMEToWGS[3][3];
		double JToWGS[3][3],WGSToJ[3][3];
		double PosInTEME[3],VelInTEME[3];
		double rVec[3],vVec[3];
		double tmpx[3],tmpy[3];
	
		cordMtxJToTEMEGet(JToTEME,tTime);
		mtxInv((double *)TEMEToJ,(double *)JToTEME,3);
		cordMtxTEMEToWGSGet(TEMEToWGS,Vel_TEMEToWGS,tTime);
    mtxMtp((double *)JToWGS,(double *)TEMEToWGS,3,3,(double *)JToTEME,3,3); 
		mtxInv((double *)WGSToJ,(double *)JToWGS,3);
		mtxInv((double *)WGSToTEME,(double *)TEMEToWGS,3);
	
		mtxMtp(rVec,(double *)WGSToJ,3,3,(double *)PosInWGS,3,1); 
		mtxMtp(PosInTEME,(double *)WGSToTEME,3,3,(double *)PosInWGS,3,1); 
		mtxMtp(tmpx,(double *)Vel_TEMEToWGS,3,3,(double *)PosInTEME,3,1); 
		mtxSub(tmpy,VelInWGS,tmpx,3,1);
		mtxMtp(VelInTEME,(double *)WGSToTEME,3,3,(double *)tmpy,3,1); 
		mtxMtp(vVec,(double *)TEMEToJ,3,3,(double *)VelInTEME,3,1); 
	
		orbInfo[0] =  rVec[0];
		orbInfo[1] =  rVec[1];
		orbInfo[2] =  rVec[2];
		orbInfo[3] =  vVec[0];
		orbInfo[4] =  vVec[1];
		orbInfo[5] =  vVec[2];
    return;
}

/*******************************************************/
/* 利用坐标转换将TEME系下的位置速度：SGP4预报轨道数据转换得到惯性系下轨道信息*/
void posVelInJ_SGP4Get(double orbInfo[6],double PosInTEME[3],double VelInTEME[3],double *tTime)
{
		double JToTEME[3][3],TEMEToJ[3][3],rVec[3],vVec[3];
		cordMtxJToTEMEGet(JToTEME,tTime);
		mtxInv((double *)TEMEToJ,(double *)JToTEME,3);
		mtxMtp((double *)rVec,(double *)TEMEToJ,3,3,(double *)PosInTEME,3,1); 
		mtxMtp((double *)vVec,(double *)TEMEToJ,3,3,(double *)VelInTEME,3,1); 
	
		orbInfo[0] =  rVec[0];
		orbInfo[1] =  rVec[1];
		orbInfo[2] =  rVec[2];
		orbInfo[3] =  vVec[0];
		orbInfo[4] =  vVec[1];
		orbInfo[5] =  vVec[2];
}

/*******************************************************/
/* 利用坐标转换，获得卫星在地固系下的位置矢量 */
void posInWGSGet(double PosInWGS[3],double JToWGS[3][3],double orbInfo[6])
{
    mtxMtp(PosInWGS,(double *)JToWGS,3,3,orbInfo,3,1); /*orbit的前三个参数为位置参数*/
    return;
}

/*******************************************************/
/* 根据轨道信息获得惯性系到轨道系的姿态矩阵 */
void MtxJtoOGet(double MtxJtoO[3][3],double orbInfo[6])
{
    double XVec[3],YVec[3],ZVec[3],VVec[3];
    double tmp,tmpM[3][3];
    
    tmp = -1.0/norm(orbInfo,3);
    mtxMtp(ZVec,orbInfo,3,1,&tmp,1,1);
        				/* 获得轨道系Z在惯性系的单位矢量 */
    tmp = 1.0/norm(&orbInfo[3],3);
    mtxMtp(VVec,&orbInfo[3],3,1,&tmp,1,1);  			
    tmpM[0][0] = 0.0;tmpM[0][1] = -ZVec[2];tmpM[0][2] = ZVec[1];
    tmpM[1][0] = ZVec[2];tmpM[1][1] = 0.0;tmpM[1][2] = -ZVec[0];
    tmpM[2][0] = -ZVec[1];tmpM[2][1] = ZVec[0];tmpM[2][2] =0.0;
    mtxMtp(YVec,(double*)tmpM,3,3,VVec,3,1);
    tmp = 1.0/norm(YVec,3);
    mtxMtp(YVec,YVec,3,1,&tmp,1,1);  
      					/* 获得轨道系Y在惯性系的单位矢量 */
    tmpM[0][0] = 0.0;tmpM[0][1] = -YVec[2];tmpM[0][2] = YVec[1];
    tmpM[1][0] = YVec[2];tmpM[1][1] = 0.0;tmpM[1][2] = -YVec[0];
    tmpM[2][0] = -YVec[1];tmpM[2][1] = YVec[0];tmpM[2][2] =0.0;
    mtxMtp(XVec,(double*)tmpM,3,3,ZVec,3,1);
    					/* 获得轨道系Z在惯性系的单位矢量 */
    mtxCpy(MtxJtoO[0],XVec,1,3);
    mtxCpy(MtxJtoO[1],YVec,1,3);
    mtxCpy(MtxJtoO[2],ZVec,1,3);
      					/* 利用XYZ矢量构成姿态矩阵 */
    return;
}

/*******************************************************/
/* 将地固系下的信息，转换成大地系下的表达 */
void geoInfoGet(double GeoCord[2],double PosInWGS[3])
{
   GeoCord[1] = PI-acos(PosInWGS[2]/norm(PosInWGS,3));/*获得纬度，单位为弧度*/
   if(fabs(PosInWGS[0]) < 1.0) /*如果直角坐标系的x接近与零*/
   {
       if(PosInWGS[1] > 0.0)  
           GeoCord[0] = PI/2.0;/*如果直角坐标系的x为正，则经度值在第一象限*/
       else
           GeoCord[0] = PI*3/2.0; /*如果直角坐标系的x为正，则经度值在第三象限*/
   }
   else /*如果直角坐标系的x不接近与零*/
   {
       GeoCord[0] = atan(PosInWGS[1]/PosInWGS[0]); /*获得调整象限前的经度值*/
       if(PosInWGS[0] < 0.0)
           GeoCord[0] = GeoCord[0] + PI;
       if((PosInWGS[0] > 0.0)&&(PosInWGS[1] < 0.0))
           GeoCord[0] = GeoCord[0] + 2.0*PI;
   }
   
   return;
}

/*******************************************************/
/* 获得地固坐标系下的磁场分量 */
/*按照经度间隔3度，纬度间隔2度划分网格，两极点处收缩为单点*/
void ChkMagLst(double MagInFix[3],const double magTable[10682][3],double GeoCord[2])
{
    int cx,cy,cx_right,cy_right,k;
    double cxw,cyw,tmpx,tmpy,left,right,tmp;
    
    tmp = 180.0/PI;/*角度-弧度转换参数*/
    GeoCord[0] *= tmp;GeoCord[1] *= tmp; 
    
    tmpx = GeoCord[0]/3.0;tmpy = GeoCord[1]/2.0;
    cx = (int)(tmpx);cy = (int)(tmpy);
	  cx_right=cx+1;cy_right=cy+1;
    cxw = tmpx - (double)cx ;cyw = tmpy - (double)cy;

    if (cx_right==120)
           cx_right=0;

    for(k=0;k<3;k=k+1)
           if (cy==0)
           {
                MagInFix[k] = *((double *)magTable+k);
           }
           else if (cy==89)
					 {
						 
						    MagInFix[k] = *((double *)magTable+cy*120*3+3+k);
					 }
					 else
           {
						    left = *((double *)magTable+(cy-1)*120*3+cx*3+3+k);
						    right = *((double *)magTable+(cy-1)*120*3+cx_right*3+3+k) - *((double *)magTable+(cy-1)*120*3+cx*3+3+k);
                tmpx =  left+ cxw*right;
						    left = *((double *)magTable+(cy_right-1)*120*3+cx*3+3+k);
						    right = *((double *)magTable+(cy_right-1)*120*3+cx_right*3+3+k) - *((double *)magTable+(cy_right-1)*120*3+cx*3+3+k);
       	        tmpy =  left+ cxw*right;
		   
       	        tmp = tmpx + cyw*(tmpy - tmpx);
       	        MagInFix[k] = tmp;
           }
		   

    return;
}


/*******************************************************/
/* J2000坐标系下的磁场分量 */
void MagJGet(double MagInJ[3],double JToWGS[3][3],double MagInFix[3])
{
   double TransX[3][3];
      
   mtxT((double *)TransX,(double *)JToWGS,3,3); /*获得84到J2000坐标的转换矩阵*/
   mtxMtp(MagInJ,(double *)TransX,3,3,MagInFix,3,1);
   
   return;
}

/*******************************************************/
/* 轨道坐标系下的磁场分量 */
void MagOGet(double MagInO[3],double MtxJtoO[3][3],double MagInJ[3])
{

   mtxMtp(MagInO,(double *)MtxJtoO,3,3,MagInJ,3,1);
   
   return;
}

/*******************************************************/
/* 获得俯仰角量测信息 */
void magPitGet(double *PitM,double MagInO[3],double magnetometer[3])
{   /* 俯仰角取值范围为：-180~180度 本函数计算结果为弧度*/
    double magb[3],mago[3],psin,pcos;
    
    mtxCpy(magb,magnetometer,1,3); /*本体系磁场强度*/
    mtxCpy(mago,MagInO,1,3);/*轨道系磁场强度*/
    
    psin = magb[2]*mago[0] - magb[0]*mago[2];
    pcos = magb[2]*mago[2] + magb[0]*mago[0];
    if(fabs(pcos) < (1e-5))/*象限规约参见软件详细设计报告*/
    {
       if(psin > 0.0)
          *PitM = PI/2.0;
       else
          *PitM = (-1)*PI/2.0;
    }
    else
    {
       *PitM = atan(psin/pcos);
       if(pcos < 0.0)
          *PitM  = *PitM +PI;
    }
    
    return;
}

/*******************************************************/
/* 俯仰滤波器初始化模块 */
void pitFltInit(double PFSt[2],double PPF[2][2],double *PitM)
{
    double tmp,tmpx; 
	
    tmp = PI/180.0; /* 角度-弧度转换参数 */
    PFSt[0] = (*PitM);PFSt[1] = 0.0; /* 初始角度为量测角度 初始角速度为�  */
    tmpx = 2.0*tmp;
    PPF[0][0] = tmpx*tmpx;        
    PPF[0][1] = 0.0;
    PPF[1][0] = 0.0;
    tmpx = 0.1*tmp;
    PPF[1][1] = tmpx*tmpx;
    
    return;
}
/*******************************************************/
/* 俯仰滤波器的量测更新 */
void pitFltMagUpd(double PFSt[2],double PPF[2][2],double *PitM)
{  
    double H[2],err,HTmp[2];
    double Pall,K[2],stx[2][2],sty[2][2],st[2][2];
    
    if(((*PitM) - PFSt[0]) > PI)
        PFSt[0]  = PFSt[0] +2.0*PI; 
    if(((*PitM) - PFSt[0]) < (-PI))
        PFSt[0]  = PFSt[0] -2.0*PI;
    err = (*PitM) - PFSt[0];
    					 
    H[0] = 1.0;H[1] = 0.0;    
    mtxMtp(HTmp,H,1,2,(double *)PPF,2,2);
    mtxMtp(&Pall,HTmp,1,2,H,2,1);
    Pall  =  Pall +RPF;
    Pall = 1.0/Pall;  
    
    mtxMtp(K,(double *)PPF,2,2,H,2,1);
    mtxMtp(K,K,2,1,&Pall,1,1); 
    
    mtxMtp(HTmp,K,2,1,&err,1,1);
    mtxAdd(PFSt,PFSt,HTmp,1,2);  
    if(PFSt[0] > PI)
       PFSt[0]  = PFSt[0] - 2.0*PI;
    if(PFSt[0] < (-PI))
       PFSt[0]  = PFSt[0] + 2.0*PI;
    					 
    err = -1.0;
    mtxMtp((double *)stx,K,2,1,H,1,2);
    mtxMtp((double *)stx,(double *)stx,2,2,&err,1,1);
    stx[0][0] += 1.0;stx[1][1] += 1.0;
    mtxMtp((double *)sty,(double *)stx,2,2,(double *)PPF,2,2);
    mtxT((double *)st,(double *)stx,2,2);
    mtxMtp((double *)stx,(double *)sty,2,2,(double *)st,2,2);	
    mtxMtp(H,K,2,1,&RPF,1,1);
    mtxMtp((double *)sty,H,2,1,K,1,2);
    mtxAdd((double *)PPF,(double *)stx,(double *)sty,2,2);				   
    					 
    return;
}


/*******************************************************/
/* 俯仰滤波器的时间更新 */
void pitFltTimUpd(double PFSt[2],double PPF[2][2],double TqNomTmp[3])
{
    double TransF[2][2],TransTmp[2][2],tmp;
    double TransB[2],Tq,Stx[2],Sty[2];
    double TransFT[2][2];
    
    TransF[0][0] = 1.0;TransF[0][1] = 2.0;TransF[1][0] = 0.0;TransF[1][1] = 1.0;
    TransTmp[0][0] = 1.0;TransTmp[0][1] = 1.0;TransTmp[1][0] = 0.0;TransTmp[1][1] = 1.0;
    mtxMtp(TransB,(double *)TransTmp,2,2,PFB,2,1);
    tmp = 2.0;
    mtxMtp(TransB,TransB,2,1,&tmp,1,1);

    Tq = TqNomTmp[1]; 
    
    mtxMtp(Stx,(double *)TransF,2,2,PFSt,2,1);
    mtxMtp(Sty,TransB,2,1,&Tq,1,1);
    mtxAdd(PFSt,Stx,Sty,1,2);
    
    mtxT((double *)TransFT,(double *)TransF,2,2);
    mtxMtp((double *)TransTmp,(double *)TransF,2,2,(double *)PPF,2,2);
    mtxMtp((double *)TransF,(double *)TransTmp,2,2,(double *)TransFT,2,2);
    mtxMtp(Stx,TransB,2,1,&QPF,1,1);
    mtxMtp((double *)TransTmp,Stx,2,1,TransB,1,2);
    mtxAdd((double *)PPF,(double *)TransF,(double *)TransTmp,2,2);
    
    if(PFSt[0] > PI)
       PFSt[0]  = PFSt[0] -2.0*PI;
    if(PFSt[0] < (-PI))
       PFSt[0]  = PFSt[0] +2.0*PI;
      
    return;
}


/* 惯性系下轨道位置速度得到开普勒六根数 */
void Get_KplInfo(double Kepler[6],double orbInfo[6])
{
	  double rVector[3],r,vVector[3],v;
	  double x,y,z,xdot,ydot,zdot,hVector[3];
	  double esinE,ecosE,E;
	  double orb_a,orb_e,orb_i,orb_o,orb_o2,orb_u,orb_u2,orb_w,orb_M;
	  double normal_Vector[3],NVector[3],n,MVector[3],m;
	  double half_xE,half_f,f;
	  int i,j,k;

	  for (i=0;i<3;i++)
	  {
	    rVector[i] = orbInfo[i];
			x = rVector[0]; y = rVector[1]; z = rVector[2];
			vVector[i] = orbInfo[i+3];
			xdot = vVector[0];ydot = vVector[1];zdot = vVector[2];
	  }
		r = norm(rVector,3);
		v = norm(vVector,3);
		VecCross(hVector,rVector,vVector);
		//h = norm(hVector,3);
		
		orb_a = GM*r/(2*GM-r*v*v);
		
		esinE = sqrt(1/(GM*orb_a))*(x*xdot+y*ydot+z*zdot);
    ecosE = 1-r/orb_a;
		
		E = atan(esinE/ecosE)*180/PI;
		if ((esinE > 0) && (ecosE < 0))
		{
			E = E+180;
		}
		else if ((esinE < 0) && (ecosE > 0))
		{
			E = E+360;
		}
		else if ((esinE < 0) && (ecosE < 0))
		{
			E = E+180;
		}
		
		orb_e = (1-r/orb_a)/cos(E*PI/180);
		orb_i = acos((x*ydot-y*xdot)/(sqrt(GM*orb_a*(1-orb_e*orb_e))))*180/PI;
		
		normal_Vector[0] = 0;normal_Vector[1] = 0;normal_Vector[2] = 1;
		VecCross(NVector,normal_Vector,hVector);
		n = norm(NVector,3);
		for (j=0;j<3;j++)
		{
			NVector[j] = NVector[j]/n;
		}
		orb_o=acos(NVector[0])*180/PI;
		orb_o2=acos(NVector[1])*180/PI;
		if ((orb_o2 >= 0) && (orb_o2 <= 90))
		{
			orb_o = orb_o;
		}
		else if ((orb_o2 > 90) && (orb_o2 <= 180))
		{
			orb_o = 360-orb_o;
		}
		
		VecCross(MVector,hVector,NVector);
		m = norm(MVector,3);
		for (k=0;k<3;k++)
		{
			MVector[k] = MVector[k]/m;
		}
		orb_u=acos((rVector[0]*NVector[0]+rVector[1]*NVector[1]+rVector[2]*NVector[2])/r)*180/PI;
		orb_u2=acos((rVector[0]*MVector[0]+rVector[1]*MVector[1]+rVector[2]*MVector[2])/r)*180/PI;
		if ((orb_u2 >= 0) && (orb_u2 <= 90))
		{
			orb_u = orb_u;
		}
		else if ((orb_u2 > 90) && (orb_u2 <= 180))
		{
			orb_u = 360-orb_u;
		}
		
		half_xE=E*PI/(2*180);
		half_f=atan(sqrt((1+orb_e)/(1-orb_e))*tan(half_xE));
		if ((half_xE > PI/2) && (half_xE < PI))
			half_f=half_f+PI;
    f=2*half_f*180/PI;
		orb_w=orb_u-f;
		if (orb_w < 0)
			orb_w = orb_w +360;
		orb_M=(E*PI/180-orb_e*sin(E*PI/180))*180/PI;

		
		Kepler[0] = orb_a;
		Kepler[1] = orb_e;
		Kepler[2] = orb_i;
		Kepler[3] = orb_o;
		Kepler[4] = orb_w;
		Kepler[5] = orb_M;
		
    return;	
}



/* 由GPS轨道迭代出两行根数 */
void GetTLEFromGPS(elsetrec *satrecFromGPS,double orbInfoGPS[6],double *tTime)
{
	  double MeanKepler[6];
	  double loops,RMS,PosNorm[3],endwhile;
	  double PosInTEME[3],VelInTEME[3];
	  double error[6],JToTEME[3][3],TEMEToJ[3][3];
	  double PosInJ[3],VelInJ[3],meanorbInfo[6];
	  int i,j,k,m;
	
	  cordMtxJToTEMEGet(JToTEME,tTime);
	  mtxInv((double *)TEMEToJ,(double *)JToTEME,3);
	
	  for (m=0;m<6;m++)
	  {
			meanorbInfo[m] = orbInfoGPS[m];
		}
	  Get_KplInfo(MeanKepler,meanorbInfo);
	  satrecFromGPS->bstar = 0.0002;
	  satrecFromGPS->inclo = MeanKepler[2]*PI/180;
	  satrecFromGPS->nodeo = MeanKepler[3]*PI/180;
	  satrecFromGPS->ecco = MeanKepler[1];
	  satrecFromGPS->argpo = MeanKepler[4]*PI/180;
	  satrecFromGPS->mo = MeanKepler[5]*PI/180;
	  satrecFromGPS->no = 60*sqrt(GM/(MeanKepler[0]*MeanKepler[0]*MeanKepler[0]));
	
	  loops = 0;
	  RMS = 10000;
	  endwhile = 1;
	
	  while (endwhile !=0)
		{
			loops = loops+1;
			sgp4init(satrecFromGPS);
			sgp4(PosInTEME,VelInTEME,satrecFromGPS,0);
			mtxMtp((double *)PosInJ,(double *)TEMEToJ,3,3,(double *)PosInTEME,3,1); 
			mtxMtp((double *)VelInJ,(double *)TEMEToJ,3,3,(double *)VelInTEME,3,1); 
			for (i=0;i<3;i++)
			{
				error[i] = orbInfoGPS[i]-PosInJ[i];
				PosNorm[i] = error[i];
			}
			for (j=0;j<3;j++)
			{
				error[j+3] = orbInfoGPS[j+3]-VelInJ[j];
			}
			RMS = sqrt(norm(PosNorm,3)/3);
			
			if ((RMS >= 0.01) && (loops <= 20))
			{
				for (k=0;k<6;k++)
			  {
					meanorbInfo[k] = meanorbInfo[k]+error[k];
				}
				Get_KplInfo(MeanKepler,meanorbInfo);
				satrecFromGPS->bstar = 0.0002;
				satrecFromGPS->inclo = MeanKepler[2]*PI/180;
				satrecFromGPS->nodeo = MeanKepler[3]*PI/180;
				satrecFromGPS->ecco = MeanKepler[1];
				satrecFromGPS->argpo = MeanKepler[4]*PI/180;
				satrecFromGPS->mo = MeanKepler[5]*PI/180;
				satrecFromGPS->no = 60*sqrt(GM/(MeanKepler[0]*MeanKepler[0]*MeanKepler[0]));
			}
			else
			{
				endwhile = 0;
			}
		}
    return;	
}
