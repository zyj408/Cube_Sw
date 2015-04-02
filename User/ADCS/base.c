#include <includes.h>

/***************************************/
/* GPS量测更新任务
接收GPS数据接收模块发出的信号灯进行工作
根据GPS结合轨道动力学模型，利用信息融合算法估计卫星的位置和速度*/
void AppTaskOrbDet(void)                           /* 优先级：114  本任务接收GPS数据接收模块发出的信号灯进行工作，根据GPS结合动力学模型，利用信息融合算法估计卫星的位置和速度*/
{  
	 OS_ERR err;
	 int i;
   double PosInWGS[3],VelInWGS[3];
   while(1)
   {
      BSP_OS_SemWait(&SEM_ORB_DET, 0);
	    adcs_timer = OSTimeGet (&err);
			TinSat=adcs_time0 + ((double)adcs_timer) * 1.0 / 1000.0 / 86400.0;
		  for (i=0;i<3;i++)
			{
					PosInWGS[i] = WGS84PV[i];
					VelInWGS[i] = WGS84PV[i+3];
			}
			posVelInJ_GPSGet(orbInfoGPS,PosInWGS,VelInWGS,&TinSat);
   }
}

/***************************************/
/* SGP4轨道计算任务 */
void AppTaskCalcuOrb(void)                           /*开普勒轨道计算，利用轨道力学模型计算卫星当前的位置和速度*/
{
	 OS_ERR err;
   double tTime,dt,PosInTEME[3],VelInTEME[3];
	 int temp = 1440;
	 while (1)
	 {
			 BSP_OS_SemWait(&SEM_ORB_CALCU, 0);
		 
	     adcs_timer = OSTimeGet (&err);
			 TinSat=adcs_time0+adcs_timer*1.0/1000.0/86400.0;
			 dt = (TinSat - satrec.jdsatepoch) * temp;
		 
       if (CntSGP4 == 0)
					sgp4init(&satrec);
       CntSGP4++;
	     sgp4(PosInTEME,VelInTEME,&satrec,&dt);
	     posVelInJ_SGP4Get(orbInfo,PosInTEME,VelInTEME,&tTime);
   }
}



/***************************************/
/* 姿态信息采集任务---每2s执行一次 */
void AppTaskSenGet(void)
{

   while(1)
   {
		 	/*读磁强计读数*/
	
			BSP_OS_SemWait(&SEM_CYC_INFO, 0);
      
      if(upXwAdcsReDmp == VALID)                          /*阻尼恢复*/
      {
            upXwAdcsReDmp = INVALID;
            magDotDmpFlg = VALID;
            pitFltComFlg = INVALID;
            attStaFlg = INVALID ;
            cntDmpFlag = 0 ;
            cntPitcomFlag = 0 ;
            cntAttStaFlag = 0;
      }
      if(AdcsGpsUse == VALID)   
				 AdcsOrbFlg = VALID;
			if(upXwAdcsTLEFlag == VALID)                         /*原ORBFlag 轨道上注有效标志位 现7分钟释放一次,upXwAdcsTLEFlag为轨道上注有效标志*/ 
			{ 
				 AdcsOrbFlg = VALID;
				 upXwAdcsTLEFlag = INVALID;                        /*标志位复位*/         
				 satrec.bstar = upXwAdcsTLEBstar;
				 satrec.ecco = upXwAdcsTLEEcco;
				 satrec.inclo = upXwAdcsTLEInclo;
				 satrec.argpo = upXwAdcsTLEArgpo;
				 satrec.jdsatepoch = upXwAdcsTLEJdsatepoch;
				 satrec.mo = upXwAdcsTLEMo;
				 satrec.no = upXwAdcsTLENo;
				 satrec.nodeo = upXwAdcsTLENodeo;
				 CntSGP4 = 0;
			}

      /******************************************************************************/
      /*上行参数由数据综合系统获得后给姿控系统参数进行赋值*/
      if(upXwAdcsConPFlag == VALID)                                      /*三轴稳定控制律P系数上注标志位，数据综合置位姿控清零*/
        {
           upAdcsConP = upXwAdcsConP;
           upXwAdcsConPFlag = INVALID;
        }
      if(upXwAdcsConDFlag == VALID)                                      /*三轴稳定控制律D系数上注标志位，数据综合置位姿控清零*/
        {
          upAdcsConD = upXwAdcsConD;
           upXwAdcsConDFlag = INVALID;
        }
      if(upXwAdcsWhlVFlag == VALID)                                      /*动量轮角动量控制电压，数据综合置位姿控清零*/
        {
          upAdcsWhlV = upXwAdcsWhlV;
          upXwAdcsWhlVFlag = INVALID;
        }
      if(upXwAdcsConZFlag == VALID)                                      /*章进动控制律Z系数，数据综合置位姿控清零*/
        {
          upAdcsConZ = upXwAdcsConZ;
          upXwAdcsConZFlag = INVALID;
        }
      if( upXwAdcsDmpFlgFlag == VALID)                                   /*阻尼标志位，数据综合置位姿控清零*/
        {
          magDotDmpFlg = upXwAdcsDmpFlg;
          upXwAdcsDmpFlgFlag = INVALID;
        }
      if( upXwAdcsFltFlgFlag == VALID)                                   /* 测量标志位，数据综合置位姿控清零*/
        {
          pitFltComFlg = upXwAdcsFltFlg;
          upXwAdcsFltFlgFlag = INVALID;
        }
      if( upXwAdcsCtlFlgFlag == VALID)                                   /* 控制标志位，数据综合置位姿控清零*/
        {
          attStaFlg = upXwAdcsCtlFlg;
          upXwAdcsCtlFlgFlag = INVALID;
        }

      /*****************************************************************************************/

      if(AdcsOrbFlg == VALID)              /*姿控信息采集任务时刻关注上行轨道标志位、阻尼标志位和俯仰滤波标志位，一旦检测到有效即发出信号灯执行相应任务*/
				 BSP_OS_SemPost(&SEM_ORB_CALCU);
      if(magDotDmpFlg == VALID)	
				 BSP_OS_SemPost(&SEM_MAG_DOT_DMP);
      if(pitFltComFlg == VALID)
				 BSP_OS_SemPost(&SEM_PIT_FLT_COM);

      downAdcsmagDotDmpFlg   = magDotDmpFlg;
      downAdcscntDmpFlag     = cntDmpFlag;
      downAdcspitFltComFlg   = pitFltComFlg;
      downAdcscntPitcomFlag  = cntPitcomFlag;
      downAdcsattStaFlg      = attStaFlg;
      downAdcscntAttStaFlag  = cntAttStaFlag;

    }
}

/***************************************/
/* 阻尼任务 */
void AppTaskMagDotDmp(void)
{
   double magTmp[3],magInc[3];
   double tmp1,tmp2,tmp3;
   double Vout[3],Voutmw;
   int i,t0 = 2999;
   tmp1 =1/2.2565; tmp2 =1/2.2061; tmp3 =1/2.1068;
   while(1)
   {
	  	BSP_OS_SemWait(&SEM_MAG_DOT_DMP, 0);

      if(cntDmpFlag < 0.1)
			{
         mtxCpy(magTmp,magnetometer,1,3);
         for(i=0;i<3;i=i+1)
              MTQOut[i] = 0;                                    /*首次执行时磁力矩器输出为0*/
			}
      else
      {
      	mtxSub(magInc,magnetometer,magTmp,1,3);    
      	mtxCpy(magTmp,magnetometer,1,3);
      	for(i=0;i<3;i=i+1)
        {
           MTQOut[i] = (-1)*(1e6)*magInc[i]/tInterval;          /*阻尼控制率*/
           if(MTQOut[i] > 0.2)
              MTQOut[i] = 0.2;
           else if(MTQOut[i] < -0.2)
              MTQOut[i] = -0.2;
           else
              MTQOut[i] = 0;
        }
			}
			
      Vout[0] = MTQOut[0]*tmp1;
      Vout[1] = MTQOut[1]*tmp2;
      Vout[2] = MTQOut[2]*tmp3;
      /* printf("mtq: %f %f %f\n",Vout[0],Vout[1],Vout[2]); */
      
			Voutmw=upAdcsWhlV;                                            /*动量轮输入电压为上注电压值*/
     	downAdcsWhlV = Voutmw;                                        /*下行动量轮电压值*/
     	mtxCpy(downAdcsMtqV,Vout,1,3);                                /*下行磁力矩器电压值*/
		  mtxCpy(downAdcsMagnetometer,magnetometer,1,3);                /*下行磁强计测量值*/

//			daTran2(0, &Voutmw);                                          /*向DA通道0输出动量轮电压值*/
//			daTran2(1,&Vout[0]); daTran2(2,&Vout[1]);daTran2(3,&Vout[2]); /*向DA通道1、2、3分别输出三轴磁力矩器电压值*/
      
      if(pitFltComFlg == INVALID)
      {
         cntDmpFlag++;
         if((AdcsOrbFlg == VALID) && (cntDmpFlag > t0)&&(upXwAdcsDmpForever == INVALID)) /*t0>2999 阻尼时间*/ 
         {
						 pitFltComFlg = VALID;
      	 }
      }				            /* 阻尼2999s,开启俯仰滤波器 */
   }
}


/***************************************/
/* 俯仰滤波状态更新任务 */
void AppTaskPitFltCom(void)
{
   double JToWGS[3][3],PosInWGS[3],GeoCord[2];
   double MagInFix[3],MagInJ[3],MtxJtoO[3][3],MagInO[3];
   double PitM,RadToDeg;
   int i;
	 #if 0
	 OS_ERR err;
   RadToDeg = 180.0/PI;
   while(1)
   {
	  	BSP_OS_SemWait(&SEM_PIT_FLT_COM, 0);

	    adcs_timer = OSTimeGet (&err);
			TinSat=adcs_time0+adcs_timer*1.0/1000.0/86400.0;

     if(AdcsGpsUse == VALID)                                              /*GPS正常工作*/
     {
            for(i=0;i<6;i++)
                orbInfo[i]=orbInfoGPS[i];
     }
     /*printf(" orbInfo= %f  %f %f\n",orbInfo[0] ,orbInfo[1] ,orbInfo[2] );*/
      
      mtxCpy(downAdcsOrb,orbInfo,1,6);                                       /*下行位置和速度*/
      cordMtxJToWGSGet(JToWGS,&TinSat);
      posInWGSGet(PosInWGS,JToWGS,orbInfo);
      geoInfoGet(GeoCord,PosInWGS);
      ChkMagLst(MagInFix,magTable,GeoCord);
      
      MagJGet(MagInJ,JToWGS,MagInFix);
      MtxJtoOGet(MtxJtoO,orbInfo);
      MagOGet(MagInO,MtxJtoO,MagInJ);

      MagInO_YInc = MagInO[1] - MagInO_last[1] ;                       /*MagInO_YInc为轨道系内Y轴磁场变化量，去向为姿态控制任务*/
      MagInO_last[0] = MagInO[0];                               /*将MagInO赋值给MagInO_last*/   
      MagInO_last[1] = MagInO[1];
      MagInO_last[2] = MagInO[2];
              
      magPitGet(&PitM,MagInO,magnetometer);                     /*俯仰角测量值*/   
      /*printf(" MagInO= %f  %f %f\n",MagInO[0] ,MagInO[1] ,MagInO[2] );*/

      if(cntPitcomFlag == INVALID)
         pitFltInit(PFSt,PPF,&PitM);                          /*首次执行，滤波器状态向量和协方差矩阵初始化*/   
      else
      {
          pitFltMagUpd(PFSt,PPF,&PitM);                        /*滤波器量测更新*/   
          /*printf(" PitM_Mag= %f \n",PitM*180/PI );*/
       }

       downAdcsMeasAng = PitM*RadToDeg;                     /*下行俯仰角测量值*/        
       mtxCpy(downAdcsFltAngRate,PFSt,1,2);
       downAdcsFltAngRate[0] = downAdcsFltAngRate[0]*RadToDeg; /*下行俯仰角滤波后角度值*/   
       downAdcsFltAngRate[1] = downAdcsFltAngRate[1]*RadToDeg; /*下行俯仰角滤波后角速度值*/      
       mtxCpy(downAdcsMagTable,MagInFix,1,3);                  /*下行WGS84系下磁场值*/   
       mtxCpy(downAdcsMagnetometer,magnetometer,1,3);          /*下行磁强计测量值*/
			 
      cntPitcomFlag++;
      if(attStaFlg == INVALID) 
         if(cntPitcomFlag >= 30)
         {
            attStaFlg  = VALID;
            magDotDmpFlg = INVALID;
            cntDmpFlag = 0;                      /* 关闭阻尼任务与阻尼计数器 */
         }
      if(attStaFlg == VALID)
				 BSP_OS_SemPost(&SEM_ATT_STA_CTL);
			BSP_OS_SemPost(&SEM_PIT_FLT_PRO);
   }   
	 #endif
}


/***************************************/
/* 俯仰滤波状态预测任务 */
void AppTaskPitFltPro(void)
{
   int i;
   double TqNom[3];
   while(1)
   {
	  	BSP_OS_SemWait(&SEM_PIT_FLT_PRO, 0);

      for(i=0;i<3;i=i+1)
      {
          TqNom[i] = mtqTq[i];
					mtqTq[i] = 0.0;
      }
       pitFltTimUpd(PFSt,PPF,TqNom);
   }   
}


/***************************************/
/* 三轴稳定控制任务 */
void AppTaskAttStaCtl(void)
{
   double magTmp[3],magInc[3],theta,dtheta,Tq,MagInB[3],tmpx = 1e-9;
   double tmp1,tmp2,tmp3,tmp,err[3];
   double Vout[3],Voutmw;
   int i;   
   tmp1 =1/2.2565; tmp2 =1/2.2061; tmp3 =1/2.1068;
   	
   while(1)
   {
      BSP_OS_SemWait(&SEM_ATT_STA_CTL, 0);

      theta = PFSt[0]*180/PI;
      dtheta = PFSt[1]*180/PI;
      if(cntAttStaFlag < 1)
          mtxCpy(magTmp,magnetometer,1,3);
      else
      {
      	  mtxSub(magInc,magnetometer,magTmp,1,3);
      	  mtxCpy(magTmp,magnetometer,1,3);
      }

        Tq = -upAdcsConP*theta - upAdcsConD*dtheta;                   /*俯仰轴控制率*/


         err[0] = 0;
         err[1] = Tq;
         err[2] = 0;
      	 mtxMtp(MagInB,magnetometer,1,3,&tmpx,1,1);				
         tmp = 1.0/norm(MagInB,3);
         tmp = tmp*tmp;
         VecCross(MTQOut,MagInB,err);
         mtxMtp(MTQOut,MTQOut,1,3,&tmp,1,1);                         /*由俯仰轴力矩计算公式反求X，Z轴磁矩*/

         MTQOut[1] = -upAdcsConZ * (magInc[1] - MagInO_YInc);               /*章进动控制率，求出Y轴磁矩*/
         

      for(i=0;i<3;i++)
      {
         if(MTQOut[i] < -0.2)
            MTQOut[i] = -0.2;
         if(MTQOut[i] > 2)
            MTQOut[i] = 0.2;
      }
      
       Vout[0] = MTQOut[0]*tmp1;                                     /*计算磁力矩器三轴电压输出*/
       Vout[1] = MTQOut[1]*tmp2;
       Vout[2] = MTQOut[2]*tmp3;
      
      /* printf("mtq: %f %f %f\n",Vout[0],Vout[1],Vout[2]); */

      Voutmw = upAdcsWhlV;                                          /*动量轮输入电压为上注电压值*/
      downAdcsWhlV = Voutmw;                                        /*下行动量轮电压值*/
      mtxCpy(downAdcsMtqV,Vout,1,3);                                /*下行磁力矩器电压值*/

//      daTran2(0, &Voutmw);                                          /*向DA通道0输出动量轮电压值*/
//      daTran2(1,&Vout[0]); daTran2(2,&Vout[1]);daTran2(3,&Vout[2]); /*向DA通道1、2、3分别输出三轴磁力矩器电压值*/
     
      cntAttStaFlag++; 
   }   
}
