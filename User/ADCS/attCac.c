/**********************************************/
#include <includes.h>

/*******************************************************/
/* ½«¸ñÀïÀúÊ±¼ä×ª»»³ÉÈåÂÔÈÕÀú */
/*ÓÉÓÚÌ«Ñô¼ÆËãÄ£ÐÍÊÇÊ±¼äµÄº¯Êý£¬¶øÊý¾Ý×ÛºÏÏµÍ³Ìá¹©µÄÊÇÍ¨ÓÃµÄ¸ñÀïÀúÊ±¼ä£¬×ª»»³É·½±ã¼ÆËãµÄÈåÂÔÀú*/
void timeFormTrans(double *tTime,int *year,int *month,int *day,int *hour,int *minute,double *second)
{
    double fstItem,secItem,thiItem,tmpx,tmpy;  /* ¼ÆËãÊ¹ÓÃµÄÁÙÊ±±äÁ¿*/
    tmpy = ((*month) - 14)/12.0;
    modf(tmpy,&tmpx);
    tmpy = 1461*((*year) + 4800 + tmpx)/4.0;
    modf(tmpy,&fstItem);
       					/* µÚÒ»Ïî */
    tmpy = 367*((*month) - 2 - tmpx*12)/12.0;
    modf(tmpy,&secItem);
        				/* µÚ¶þÏî */
    tmpy = ((*year) + 4900 + tmpx)/100.0;
    modf(tmpy,&tmpx);
    tmpy = 3*tmpx/4.0;
    modf(tmpy,&thiItem);
        			
    *tTime = (*day) - 32075 + fstItem + secItem - thiItem - 0.5;            
    *tTime = *tTime + (*hour)/24.0 + (*minute)/1440.0 + (*second)/86400.0; /*µ¥Î»ÎªÈÕ*/
    
}


/*******************************************************/
/* ÀûÓÃÊ±¼äÐÅÏ¢£¬»ñµÃJ2000µ½TEME×ø±êµÄ×ª»»¾ØÕó */
void cordMtxJToTEMEGet(double JToTEME[3][3],double *tTime)
{
    double Tu,Td,DA,ZA,TA,KesiMean,OmegaMoon,LongiSun,DeltaKesi,DeltaFai,KesiReal;
    double tmpx,tmpy,tmpz,tmpsin,tmp,tmpcos,DATrans[3][3],ZATrans[3][3],TATrans[3][3];
    double TransKm[3][3],TransKr[3][3],TransDf[3][3];
    double tmpTrans[3][3],TransX[3][3],TransY[3][3];
	
    Tu = (*tTime) - 2451545.0;
    Td = Tu; /*±£´æ±äÁ¿Tu£¬ÒÔ±ã¼ÆËã¸ñÁÖÄáÖÎÆ½ºãÐÇÊ±*/ 
    Tu = Tu/36525.0;
    
    tmpx = 180.0*3600/PI;tmpy = Tu*Tu;tmpz = tmpy*Tu;
    DA = (2306.2181*Tu + 0.30188*tmpy + 0.017998*tmpz)/tmpx; /*³àµÀÃæÅ·À­½ÇÖ®Ò»*/
    ZA = (2306.2181*Tu + 1.09468*tmpy + 0.018203*tmpz)/tmpx; /*³àµÀÃæÅ·À­½ÇÖ®¶þ*/
    TA = (2004.3109*Tu - 0.42665*tmpy - 0.041833*tmpz)/tmpx; /*³àµÀÃæÅ·À­½ÇÖ®Èý*/
    tmpcos = cos(DA);tmpsin = sin(DA);
    DATrans[0][0] = tmpcos;DATrans[0][1] = -tmpsin;DATrans[0][2] = 0.0; /*³àµÀÃæÅ·À­½ÇDA¹¹³ÉµÄ×ø±ê×ª»»¾ØÕó*/
    DATrans[1][0] = tmpsin;DATrans[1][1] = tmpcos;DATrans[1][2] = 0.0;
    DATrans[2][0] = 0.0;DATrans[2][1] = 0.0;DATrans[2][2] = 1.0;
    tmpcos = cos(ZA);tmpsin = sin(ZA);
    ZATrans[0][0] = tmpcos;ZATrans[0][1] = -tmpsin;ZATrans[0][2] = 0.0;/*³àµÀÃæÅ·À­½ÇZA¹¹³ÉµÄ×ø±ê×ª»»¾ØÕó*/
    ZATrans[1][0] = tmpsin;ZATrans[1][1] = tmpcos;ZATrans[1][2] = 0.0;
    ZATrans[2][0] = 0.0;ZATrans[2][1] = 0.0;ZATrans[2][2] = 1.0;
    tmpcos = cos(TA);tmpsin = sin(TA);
    TATrans[0][0] = tmpcos;TATrans[0][1] = 0.0;TATrans[0][2] = -tmpsin;/*³àµÀÃæÅ·À­½ÇTA¹¹³ÉµÄ×ø±ê×ª»»¾ØÕó*/
    TATrans[1][0] = 0.0;TATrans[1][1] = 1.0;TATrans[1][2] = 0.0;
    TATrans[2][0] = tmpsin;TATrans[2][1] = 0.0;TATrans[2][2] = tmpcos;
    mtxMtp((double *)tmpTrans,(double *)ZATrans,3,3,(double *)TATrans,3,3);
    mtxMtp((double *)TransX,(double *)tmpTrans,3,3,(double *)DATrans,3,3);
    					/* ´ÓJ2000µ½Ë²Ê±Æ½³àµÀ×ø±êÏµµÄ×ª»»¹ØÏµ */
    tmp = 180.0/PI;		/*½Ç¶È-»¡¶È×ª»»²ÎÊý*/		
    KesiMean = (84381.448 - 46.815*Tu - 0.00059*tmpy + 0.001813*tmpz)/tmpx; /*Æ½»Æ³à½»½Ç*/
    OmegaMoon = (125.04 - 0.052954*Td)/tmp;/*ÔÂÇòµÄÉý½»µã³à¾­*/
    LongiSun = (280.47 + 0.98565*Td)/tmp;  /* Ì«ÑôµÄÆ½»Æ¾­*/
    DeltaKesi = (9.2025*cos(OmegaMoon) + 0.5736*cos(2*LongiSun))/tmpx;  /*½»½ÇÕÂ¶¯*/
    DeltaFai = (-17.1996*sin(OmegaMoon) - 1.3187*sin(2*LongiSun))/tmpx; /*»Æ¾­ÕÂ¶¯*/
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
    					/* ´ÓË²Ê±Æ½³àµÀ×ø±êÏµµ½Õæ³àµÀ×ø±êµÄ×ª»»¹ØÏµ */
		mtxMtp((double *)JToTEME,(double *)TransY,3,3,(double *)TransX,3,3);
		
}

/*******************************************************/
/* ÀûÓÃÊ±¼äÐÅÏ¢£¬»ñµÃTEMEµ½WGS84×ø±êµÄ×ª»»¾ØÕó */
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
    tmp = 180.0/PI;		/*½Ç¶È-»¡¶È×ª»»²ÎÊý*/		
    KesiMean = (84381.448 - 46.815*Tu - 0.00059*tmpy + 0.001813*tmpz)/tmpx; /*Æ½»Æ³à½»½Ç*/
    OmegaMoon = (125.04 - 0.052954*Td)/tmp;/*ÔÂÇòµÄÉý½»µã³à¾­*/
    LongiSun = (280.47 + 0.98565*Td)/tmp;  /* Ì«ÑôµÄÆ½»Æ¾­*/
    DeltaKesi = (9.2025*cos(OmegaMoon) + 0.5736*cos(2*LongiSun))/tmpx;  /*½»½ÇÕÂ¶¯*/
    DeltaFai = (-17.1996*sin(OmegaMoon) - 1.3187*sin(2*LongiSun))/tmpx; /*»Æ¾­ÕÂ¶¯*/
    KesiReal = KesiMean + DeltaKesi;
		
    tmp = 2.0*PI/24;
    tmpx = 18.697374558*tmp;tmpy = 24.06570982441908*tmp;
    tmpz = tmpx + tmpy*Td;  /*»ñµÃ¸ñÁÖÄáÖÎÆ½ºãÐÇÊ±*/  
	
    tmp = tmpz + DeltaFai*cos(KesiReal); /*»ñµÃ¸ñÁÖÄáÖÎÕæºãÐÇÊ±½Ç*/ 
    tmpcos = cos(tmp);tmpsin = sin(tmp);
    TransX[0][0] = tmpcos;TransX[0][1] = tmpsin;TransX[0][2] = 0.0;
    TransX[1][0] = -tmpsin;TransX[1][1] = tmpcos;TransX[1][2] = 0.0;
    TransX[2][0] = 0.0;TransX[2][1] = 0.0;TransX[2][2] = 1.0;
    					/* ´ÓË²Ê±Õæ³àµÀ×ø±êÏµµ½×¼µØ¹ÌÏµµÄ×ª»»¹ØÏµ */
    TransY[0][0] = -tmpsin*thetadot;TransY[0][1] = tmpcos*thetadot;TransY[0][2] = 0.0;
    TransY[1][0] = -tmpcos*thetadot;TransY[1][1] = -tmpsin*thetadot;TransY[1][2] = 0.0;
    TransY[2][0] = 0.0;TransY[2][1] = 0.0;TransY[2][2] = 0.0;
	
    mtxCpy((double *)TEMEToWGS,(double *)TransX,3,3);
		mtxCpy((double *)Vel_TEMEToWGS,(double *)TransY,3,3);
	
}

/*******************************************************/
/* ÀûÓÃÊ±¼äÐÅÏ¢£¬»ñµÃJ2000µ½WGS84×ø±êµÄ×ª»»¾ØÕó */
void cordMtxJToWGSGet(double JToWGS[3][3],double *tTime)
{
		double JToTEME[3][3],TEMEToWGS[3][3],Vel_TEMEToWGS[3][3];
		cordMtxJToTEMEGet(JToTEME,tTime);
		cordMtxTEMEToWGSGet(TEMEToWGS,Vel_TEMEToWGS,tTime);
		mtxMtp((double *)JToWGS,(double *)TEMEToWGS,3,3,(double *)JToTEME,3,3);
}

/*******************************************************/
/* ÀûÓÃ×ø±ê×ª»»½«µØ¹ÌÏµÏÂµÄÎ»ÖÃËÙ¶È£ºGPS²âÁ¿Êý¾Ý×ª»»µÃµ½¹ßÐÔÏµÏÂ¹ìµÀÐÅÏ¢*/
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
/* ÀûÓÃ×ø±ê×ª»»½«TEMEÏµÏÂµÄÎ»ÖÃËÙ¶È£ºSGP4Ô¤±¨¹ìµÀÊý¾Ý×ª»»µÃµ½¹ßÐÔÏµÏÂ¹ìµÀÐÅÏ¢*/
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
/* ÀûÓÃ×ø±ê×ª»»£¬»ñµÃÎÀÐÇÔÚµØ¹ÌÏµÏÂµÄÎ»ÖÃÊ¸Á¿ */
void posInWGSGet(double PosInWGS[3],double JToWGS[3][3],double orbInfo[6])
{
    mtxMtp(PosInWGS,(double *)JToWGS,3,3,orbInfo,3,1); /*orbitµÄÇ°Èý¸ö²ÎÊýÎªÎ»ÖÃ²ÎÊý*/
    return;
}

/*******************************************************/
/* ¸ù¾Ý¹ìµÀÐÅÏ¢»ñµÃ¹ßÐÔÏµµ½¹ìµÀÏµµÄ×ËÌ¬¾ØÕó */
void MtxJtoOGet(double MtxJtoO[3][3],double orbInfo[6])
{
    double XVec[3],YVec[3],ZVec[3],VVec[3];
    double tmp,tmpM[3][3];
    
    tmp = -1.0/norm(orbInfo,3);
    mtxMtp(ZVec,orbInfo,3,1,&tmp,1,1);
        				/* »ñµÃ¹ìµÀÏµZÔÚ¹ßÐÔÏµµÄµ¥Î»Ê¸Á¿ */
    tmp = 1.0/norm(&orbInfo[3],3);
    mtxMtp(VVec,&orbInfo[3],3,1,&tmp,1,1);  			
    tmpM[0][0] = 0.0;tmpM[0][1] = -ZVec[2];tmpM[0][2] = ZVec[1];
    tmpM[1][0] = ZVec[2];tmpM[1][1] = 0.0;tmpM[1][2] = -ZVec[0];
    tmpM[2][0] = -ZVec[1];tmpM[2][1] = ZVec[0];tmpM[2][2] =0.0;
    mtxMtp(YVec,(double*)tmpM,3,3,VVec,3,1);
    tmp = 1.0/norm(YVec,3);
    mtxMtp(YVec,YVec,3,1,&tmp,1,1);  
      					/* »ñµÃ¹ìµÀÏµYÔÚ¹ßÐÔÏµµÄµ¥Î»Ê¸Á¿ */
    tmpM[0][0] = 0.0;tmpM[0][1] = -YVec[2];tmpM[0][2] = YVec[1];
    tmpM[1][0] = YVec[2];tmpM[1][1] = 0.0;tmpM[1][2] = -YVec[0];
    tmpM[2][0] = -YVec[1];tmpM[2][1] = YVec[0];tmpM[2][2] =0.0;
    mtxMtp(XVec,(double*)tmpM,3,3,ZVec,3,1);
    					/* »ñµÃ¹ìµÀÏµZÔÚ¹ßÐÔÏµµÄµ¥Î»Ê¸Á¿ */
    mtxCpy(MtxJtoO[0],XVec,1,3);
    mtxCpy(MtxJtoO[1],YVec,1,3);
    mtxCpy(MtxJtoO[2],ZVec,1,3);
      					/* ÀûÓÃXYZÊ¸Á¿¹¹³É×ËÌ¬¾ØÕó */
    return;
}

/*******************************************************/
/* ½«µØ¹ÌÏµÏÂµÄÐÅÏ¢£¬×ª»»³É´óµØÏµÏÂµÄ±í´ï */
void geoInfoGet(double GeoCord[2],double PosInWGS[3])
{
   GeoCord[1] = PI-acos(PosInWGS[2]/norm(PosInWGS,3));/*»ñµÃÎ³¶È£¬µ¥Î»Îª»¡¶È*/
   if(fabs(PosInWGS[0]) < 1.0) /*Èç¹ûÖ±½Ç×ø±êÏµµÄx½Ó½üÓëÁã*/
   {
       if(PosInWGS[1] > 0.0)  
           GeoCord[0] = PI/2.0;/*Èç¹ûÖ±½Ç×ø±êÏµµÄxÎªÕý£¬Ôò¾­¶ÈÖµÔÚµÚÒ»ÏóÏÞ*/
       else
           GeoCord[0] = PI*3/2.0; /*Èç¹ûÖ±½Ç×ø±êÏµµÄxÎªÕý£¬Ôò¾­¶ÈÖµÔÚµÚÈýÏóÏÞ*/
   }
   else /*Èç¹ûÖ±½Ç×ø±êÏµµÄx²»½Ó½üÓëÁã*/
   {
       GeoCord[0] = atan(PosInWGS[1]/PosInWGS[0]); /*»ñµÃµ÷ÕûÏóÏÞÇ°µÄ¾­¶ÈÖµ*/
       if(PosInWGS[0] < 0.0)
           GeoCord[0] = GeoCord[0] + PI;
       if((PosInWGS[0] > 0.0)&&(PosInWGS[1] < 0.0))
           GeoCord[0] = GeoCord[0] + 2.0*PI;
   }
   
   return;
}

/*******************************************************/
/* »ñµÃµØ¹Ì×ø±êÏµÏÂµÄ´Å³¡·ÖÁ¿ */
/*°´ÕÕ¾­¶È¼ä¸ô3¶È£¬Î³¶È¼ä¸ô2¶È»®·ÖÍø¸ñ£¬Á½¼«µã´¦ÊÕËõÎªµ¥µã*/
void ChkMagLst(double MagInFix[3],const double magTable[10682][3],double GeoCord[2])
{
	#if 0
    int cx,cy,cx_right,cy_right,k;
    double cxw,cyw,tmpx,tmpy,left,right,tmp;
    
    tmp = 180.0/PI;/*½Ç¶È-»¡¶È×ª»»²ÎÊý*/
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
					 #endif
}


/*******************************************************/
/* J2000×ø±êÏµÏÂµÄ´Å³¡·ÖÁ¿ */
void MagJGet(double MagInJ[3],double JToWGS[3][3],double MagInFix[3])
{
   double TransX[3][3];
      
   mtxT((double *)TransX,(double *)JToWGS,3,3); /*»ñµÃ84µ½J2000×ø±êµÄ×ª»»¾ØÕó*/
   mtxMtp(MagInJ,(double *)TransX,3,3,MagInFix,3,1);
   
   return;
}

/*******************************************************/
/* ¹ìµÀ×ø±êÏµÏÂµÄ´Å³¡·ÖÁ¿ */
void MagOGet(double MagInO[3],double MtxJtoO[3][3],double MagInJ[3])
{

   mtxMtp(MagInO,(double *)MtxJtoO,3,3,MagInJ,3,1);
   
   return;
}

/*******************************************************/
/* »ñµÃ¸©Ñö½ÇÁ¿²âÐÅÏ¢ */
void magPitGet(double *PitM,double MagInO[3],double magnetometer[3])
{   /* ¸©Ñö½ÇÈ¡Öµ·¶Î§Îª£º-180~180¶È ±¾º¯Êý¼ÆËã½á¹ûÎª»¡¶È*/
    double magb[3],mago[3],psin,pcos;
    
    mtxCpy(magb,magnetometer,1,3); /*±¾ÌåÏµ´Å³¡Ç¿¶È*/
    mtxCpy(mago,MagInO,1,3);/*¹ìµÀÏµ´Å³¡Ç¿¶È*/
    
    psin = magb[2]*mago[0] - magb[0]*mago[2];
    pcos = magb[2]*mago[2] + magb[0]*mago[0];
    if(fabs(pcos) < (1e-5))/*ÏóÏÞ¹æÔ¼²Î¼ûÈí¼þÏêÏ¸Éè¼Æ±¨¸æ*/
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
/* ¸©ÑöÂË²¨Æ÷³õÊ¼»¯Ä£¿é */
void pitFltInit(double PFSt[2],double PPF[2][2],double *PitM)
{
    double tmp,tmpx; 
	
    tmp = PI/180.0; /* ½Ç¶È-»¡¶È×ª»»²ÎÊý */
    PFSt[0] = (*PitM);PFSt[1] = 0.0; /* ³õÊ¼½Ç¶ÈÎªÁ¿²â½Ç¶È ³õÊ¼½ÇËÙ¶ÈÎªÁ  */
    tmpx = 2.0*tmp;
    PPF[0][0] = tmpx*tmpx;        
    PPF[0][1] = 0.0;
    PPF[1][0] = 0.0;
    tmpx = 0.1*tmp;
    PPF[1][1] = tmpx*tmpx;
    
    return;
}
/*******************************************************/
/* ¸©ÑöÂË²¨Æ÷µÄÁ¿²â¸üÐÂ */
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
/* ¸©ÑöÂË²¨Æ÷µÄÊ±¼ä¸üÐÂ */
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

