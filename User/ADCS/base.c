#include <includes.h>

/***************************************/
/* ��̬��Ϣ�ɼ�����---ÿ2sִ��һ�� */
void AppTaskSenGet(void)
{
   OS_ERR err;
   int i;
   double PosInWGS[3],VelInWGS[3];
   double tTime,dt,PosInTEME[3],VelInTEME[3];
   int temp = 1440;
	 
   while(1)
   {

		  BSP_OS_SemWait(&SEM_CYC_INFO, 0);
		 
	      /*����ǿ�ƶ���*/
		//BSP_OS_MutexWait(&MUTEX_MAG_CAP, 0);
		  Get_Mag_Result(MagCurOut);
	    //BSP_OS_MutexPost(&MUTEX_MAG_CAP);
	 
		  magnetometer[0] = MagCurOut[0];
		  magnetometer[1] = MagCurOut[1];
		  magnetometer[2] = MagCurOut[2];
	   
		 	if(upXwAdcsTLEFlag == VALID)                         /*ԭORBFlag �����ע��Ч��־λ ��7�����ͷ�һ��,upXwAdcsTLEFlagΪ�����ע��Ч��־*/ 
			{ 
				 upXwAdcsTLEFlag = INVALID;                        /*��־λ��λ*/         
				 upAdcsTLEFlag = VALID;
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
			
			if(updateTimeFlag == VALID)                          /* ������յ�GPS�źŻ��ߵ���ָ����ʱ�ɹ�ʱ��λ���ϵ�������Ϊ��Ч */
			{
			    adcs_timer = OSTimeGet (&err);
			    TinSat = TinSat0 + upDelta_TinSat + ((double)adcs_timer) * 1.0 / 1000.0 / 86400.0;
				
				  /*  ����̬����ģʽ����֮ǰ�ǲ���Ҫ���й���ɼ��͵��Ƶģ����ܻ�ռ��ϵͳ��Դ���ҿɿ��Խ���   */
					if(AdcsGpsUse == VALID)                          /* GPS��Ч */
					{				
						 AdcsOrbFlg = VALID;
						 AdcsGpsUse = INVALID;
						 for (i=0;i<3;i++)
						 {
								 PosInWGS[i] = WGS84PV[i];
								 VelInWGS[i] = WGS84PV[i+3];
						 }
						 posVelInJ_GPSGet(orbInfoGPS,PosInWGS,VelInWGS,&TinSat);
						 AdcsOrbGPSFlag = VALID;
						 CntNoGPS = 0;
					}
					else if(upAdcsTLEFlag == VALID)                   /* ��ע�����Ч */
					{
						 AdcsOrbFlg = VALID;
						 dt = (TinSat - satrec.jdsatepoch) * temp;
						 if (CntSGP4 == 0)
						 {
								sgp4init(&satrec);
						 }
						 CntSGP4++;
						 sgp4(PosInTEME,VelInTEME,&satrec,&dt);
						 posVelInJ_SGP4Get(orbInfo,PosInTEME,VelInTEME,&TinSat);
					}
					else if(AdcsOrbGPSFlag == VALID)                  /* ����GPS������������� */
					{
						 if (CntNoGPS == 0)
						 {
								GetTLEFromGPS(&satrecFromGPS,orbInfoGPS,&TinSat);
								sgp4init(&satrecFromGPS);
						 }
						 CntNoGPS++;
						 dt = CntNoGPS * tInterval;
						 if (dt < 43200)
						 {
								AdcsOrbFlg = VALID;
								sgp4(PosInTEME,VelInTEME,&satrecFromGPS,&dt);
								posVelInJ_SGP4Get(orbInfo,PosInTEME,VelInTEME,&tTime);
						 }
						 else
						 {
							   AdcsOrbFlg = INVALID;
						 }
					}
					else
					{
						  AdcsOrbFlg = INVALID;
					}
			}
			else
			{
				  AdcsOrbFlg = INVALID;
			}
			


			
			
      if(upXwAdcsReDmp == VALID)                          /*����ָ�*/
      {
            upXwAdcsReDmp = INVALID;
            magDotDmpFlg = VALID;
            pitFltComFlg = INVALID;
            attStaFlg = INVALID ;
            cntDmpFlag = 0 ;
            cntPitcomFlag = 0 ;
            cntAttStaFlag = 0;
      }
      /******************************************************************************/
      /*���в����������ۺ�ϵͳ��ú���˿�ϵͳ�������и�ֵ*/
      if(upXwAdcsConPFlag == VALID)                                      /*�����ȶ�������Pϵ����ע��־λ�������ۺ���λ�˿�����*/
        {
           upAdcsConP = upXwAdcsConP;
           upXwAdcsConPFlag = INVALID;
        }
      if(upXwAdcsConDFlag == VALID)                                      /*�����ȶ�������Dϵ����ע��־λ�������ۺ���λ�˿�����*/
        {
          upAdcsConD = upXwAdcsConD;
           upXwAdcsConDFlag = INVALID;
        }
      if(upXwAdcsConZFlag == VALID)                                      /*�½���������Zϵ���������ۺ���λ�˿�����*/
        {
          upAdcsConZ = upXwAdcsConZ;
          upXwAdcsConZFlag = INVALID;
        }
      if( upXwAdcsDmpFlgFlag == VALID)                                   /*�����־λ�������ۺ���λ�˿�����*/
        {
          magDotDmpFlg = upXwAdcsDmpFlg;
          upXwAdcsDmpFlgFlag = INVALID;
        }
      if( upXwAdcsFltFlgFlag == VALID)                                   /* ������־λ�������ۺ���λ�˿�����*/
        {
          pitFltComFlg = upXwAdcsFltFlg;
          upXwAdcsFltFlgFlag = INVALID;
        }
      if( upXwAdcsCtlFlgFlag == VALID)                                   /* ���Ʊ�־λ�������ۺ���λ�˿�����*/
        {
          attStaFlg = upXwAdcsCtlFlg;
          upXwAdcsCtlFlgFlag = INVALID;
        }

      /*****************************************************************************************/

      if(AdcsOrbFlg == INVALID)  /*�˿���Ϣ�ɼ�����ʱ�̹�ע���й����־λ�������־λ�͸����˲���־λ��һ����⵽��Ч�������źŵ�ִ����Ӧ����*/
	  {		 
			pitFltComFlg = INVALID;
			magDotDmpFlg = VALID;
	  }
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
/* �������� */
void AppTaskMagDotDmp(void)
{
   double magTmp[3],magInc[3];
   double tmp1,tmp2,tmp3;
   double Vout[3];
   int i,t0 = 2999;
   tmp1 =1/1; tmp2 =1/1; tmp3 =1/1;
   while(1)
   {
	   BSP_OS_SemWait(&SEM_MAG_DOT_DMP, 0);

		if(cntDmpFlag < 0.1)
		{
         mtxCpy(magTmp,magnetometer,1,3);
         for(i=0;i<3;i=i+1)
              MTQOut[i] = 0;                                    /*�״�ִ��ʱ�����������Ϊ0*/
			}
      else
      {
      	mtxSub(magInc,magnetometer,magTmp,1,3);    
      	mtxCpy(magTmp,magnetometer,1,3);
      	for(i=0;i<3;i=i+1)
        {
           MTQOut[i] = (-1)*(1e-3)*magInc[i]/tInterval;          /*���������*/
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
      
     	mtxCpy(downAdcsMtqV,Vout,1,3);                                /*���д���������ѹֵ*/
		  mtxCpy(downAdcsMagnetometer,magnetometer,1,3);                /*���д�ǿ�Ʋ���ֵ*/

//			daTran2(1,&Vout[0]); daTran2(2,&Vout[1]);daTran2(3,&Vout[2]); /*��DAͨ��1��2��3�ֱ�����������������ѹֵ*/
      
      if(pitFltComFlg == INVALID)
      {
         cntDmpFlag++;
         if((AdcsOrbFlg == VALID) && (cntDmpFlag > t0)) /*t0>2999 ����ʱ��*/ 
         {
						 pitFltComFlg = VALID;
      	 }
      }				            /* ����2999s,���������˲��� */
   }
}


/***************************************/
/* �����˲����� */
void AppTaskPitFltCom(void)
{
   double JToWGS[3][3],PosInWGS[3],GeoCord[2];
   double MagInFix[3],MagInJ[3],MtxJtoO[3][3],MagInO[3];
   double PitM,RadToDeg;
   int i;
	 
	 OS_ERR err;
   RadToDeg = 180.0/PI;
   while(1)
   {
	  	BSP_OS_SemWait(&SEM_PIT_FLT_COM, 0);

	    adcs_timer = OSTimeGet (&err);
			TinSat=TinSat0 + upDelta_TinSat + ((double)adcs_timer)*1.0/1000.0/86400.0;

     if(AdcsGpsUse == VALID)                                              /*GPS��������*/
     {
            for(i=0;i<6;i++)
                orbInfo[i]=orbInfoGPS[i];
     }
     /*printf(" orbInfo= %f  %f %f\n",orbInfo[0] ,orbInfo[1] ,orbInfo[2] );*/
      
      mtxCpy(downAdcsOrb,orbInfo,1,6);                                       /*����λ�ú��ٶ�*/
      cordMtxJToWGSGet(JToWGS,&TinSat);
      posInWGSGet(PosInWGS,JToWGS,orbInfo);
      geoInfoGet(GeoCord,PosInWGS);

      ChkMagLst(MagInFix,(const double (*)[3])magTable,GeoCord);

      
      MagJGet(MagInJ,JToWGS,MagInFix);
      MtxJtoOGet(MtxJtoO,orbInfo);
      MagOGet(MagInO,MtxJtoO,MagInJ);

      MagInO_YInc = MagInO[1] - MagInO_last[1] ;                       /*MagInO_YIncΪ���ϵ��Y��ų��仯����ȥ��Ϊ��̬��������*/
      MagInO_last[0] = MagInO[0];                               /*��MagInO��ֵ��MagInO_last*/   
      MagInO_last[1] = MagInO[1];
      MagInO_last[2] = MagInO[2];
              
      magPitGet(&PitM,MagInO,magnetometer);                     /*�����ǲ���ֵ*/   
      /*printf(" MagInO= %f  %f %f\n",MagInO[0] ,MagInO[1] ,MagInO[2] );*/

      if(cntPitcomFlag == INVALID)
			{
         pitFltInit(PFSt,PPF,&PitM);                          /*�״�ִ�У��˲���״̬������Э��������ʼ��*/  
			}				
      else
      {
          pitFltMagUpd(PFSt,PPF,&PitM);                        /*�˲����������*/   
          /*printf(" PitM_Mag= %f \n",PitM*180/PI );*/
      }

       downAdcsMeasAng = PitM*RadToDeg;                     /*���и����ǲ���ֵ*/        
       mtxCpy(downAdcsFltAngRate,PFSt,1,2);
       downAdcsFltAngRate[0] = downAdcsFltAngRate[0]*RadToDeg; /*���и������˲���Ƕ�ֵ*/   
       downAdcsFltAngRate[1] = downAdcsFltAngRate[1]*RadToDeg; /*���и������˲�����ٶ�ֵ*/      
       mtxCpy(downAdcsMagTable,MagInFix,1,3);                  /*����WGS84ϵ�´ų�ֵ*/   
       mtxCpy(downAdcsMagnetometer,magnetometer,1,3);          /*���д�ǿ�Ʋ���ֵ*/
			 
      cntPitcomFlag++;
      if(attStaFlg == INVALID) 
         if(cntPitcomFlag >= 30)
         {
            attStaFlg  = VALID;
            magDotDmpFlg = INVALID;
            cntDmpFlag = 0;                      /* �ر�������������������� */
         }
      if(attStaFlg == VALID)
				 BSP_OS_SemPost(&SEM_ATT_STA_CTL);
   }   
}


/***************************************/
/* �����ȶ��������� */
void AppTaskAttStaCtl(void)
{
   double magTmp[3],magInc[3],theta,dtheta,Tq,MagInB[3],tmpx = 1e-9;
   double tmp1,tmp2,tmp3,tmp,err[3];
   double Vout[3];
	 double TqNom[3];
   int i,j;   
   tmp1 =1/1; tmp2 =1/1; tmp3 =1/1;
   	
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

        Tq = -upAdcsConP*theta - upAdcsConD*dtheta;                   /*�����������*/


         err[0] = 0;
         err[1] = Tq;
         err[2] = 0;
      	 mtxMtp(MagInB,magnetometer,1,3,&tmpx,1,1);				
         tmp = 1.0/norm(MagInB,3);
         tmp = tmp*tmp;
         VecCross(MTQOut,MagInB,err);
         mtxMtp(MTQOut,MTQOut,1,3,&tmp,1,1);                         /*�ɸ��������ؼ��㹫ʽ����X��Z��ž�*/

         MTQOut[1] = -upAdcsConZ * (magInc[1] - MagInO_YInc);               /*�½��������ʣ����Y��ž�*/
         

      for(i=0;i<3;i++)
      {
         if(MTQOut[i] < -0.2)
            MTQOut[i] = -0.2;
         if(MTQOut[i] > 2)
            MTQOut[i] = 0.2;
      }
      
       Vout[0] = MTQOut[0]*tmp1;                                     /*����������������ѹ���*/
       Vout[1] = MTQOut[1]*tmp2;
       Vout[2] = MTQOut[2]*tmp3;
      
      /* printf("mtq: %f %f %f\n",Vout[0],Vout[1],Vout[2]); */
      mtxCpy(downAdcsMtqV,Vout,1,3);                                /*���д���������ѹֵ*/

//      daTran2(1,&Vout[0]); daTran2(2,&Vout[1]);daTran2(3,&Vout[2]); /*��DAͨ��1��2��3�ֱ�����������������ѹֵ*/
     
      cntAttStaFlag++; 
			
			for(j=0;j<3;j=j+1)
      {
          TqNom[i] = mtqTq[i];
					mtqTq[i] = 0.0;
      }
       pitFltTimUpd(PFSt,PPF,TqNom);
   }   
}
