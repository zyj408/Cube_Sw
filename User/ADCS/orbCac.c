/**********************************************/
/* 功能:                                      */
/*      与轨道计算有关的函数                        */
/* 作者:                                      */
/*      郁丰                                  */
/**********************************************/
#include <base.h>

extern double tInterval;			/* 轨道姿态传播的步长 */
extern const double ae;			/* 地球的半径 */
extern const double GM;			/* 地球引力常数 */
extern const double J2; 			/* J2摄动项 */


/* 轨道信息融合 */
void orbInfoCom(double orbInfo[6],double J2000PV[6])
{/*融合方法为取量测值与估计值的均值*/

 orbInfo[0] = 0.5*(orbInfo[0] +J2000PV[0]);
 orbInfo[1] = 0.5*(orbInfo[1] +J2000PV[1]);
 orbInfo[2] = 0.5*(orbInfo[2] +J2000PV[2]);
 orbInfo[3] = 0.5*(orbInfo[3] +J2000PV[3]);
 orbInfo[4] = 0.5*(orbInfo[4] +J2000PV[4]);
 orbInfo[5] = 0.5*(orbInfo[5] +J2000PV[5]);
     
   return;
}


/*--------------------------------------*/
/* 轨道传播 */
void orbPro(double orbInfo[6],double orbInc[3])
{ /*算法设计参见软件详细设计报告*/
    double orbTmp[6],orbit[6];
    double orbk[4][6],orb[6];
    double radius,tmp,a;
    
    mtxCpy(orbit,orbInfo,1,6);
    
    mtxCpy(orbTmp,orbit,1,6);
    mtxMtp(orbk[0],&orbTmp[3],3,1,&tInterval,1,1);

    radius = norm(orbTmp,3);
    tmp = GM/radius/radius*ae/radius*ae/radius*J2*(7.5*(orbTmp[2]/radius)*(orbTmp[2]/radius) - 1.5)/radius;

    orbk[0][3] = -GM*orbTmp[0]/(radius*radius*radius) + tmp*orbTmp[0];
    orbk[0][4] = -GM*orbTmp[1]/(radius*radius*radius) + tmp*orbTmp[1];	
    tmp = GM/radius/radius*ae/radius*ae/radius*J2*(7.5*(orbTmp[2]/radius)*(orbTmp[2]/radius) - 4.5)/radius;
    orbk[0][5] = -GM*orbTmp[2]/(radius*radius*radius) + tmp*orbTmp[2];
    
    mtxMtp(&orbk[0][3],&orbk[0][3],1,3,&tInterval,1,1);
       		               		/* RK 第一步 */
    a = 0.5;
    mtxMtp(orb,orbk[0],1,6,&a,1,1);
    mtxAdd(orbTmp,orbit,orb,1,6);
    mtxMtp(orbk[1],&orbTmp[3],3,1,&tInterval,1,1);

    radius = norm(orbTmp,3);
    tmp = GM/radius/radius*ae/radius*ae/radius*J2*(7.5*(orbTmp[2]/radius)*(orbTmp[2]/radius) - 1.5)/radius;
    orbk[1][3] = -GM*orbTmp[0]/(radius*radius*radius) + tmp*orbTmp[0];
    orbk[1][4] = -GM*orbTmp[1]/(radius*radius*radius) + tmp*orbTmp[1];	
    tmp = GM/radius/radius*ae/radius*ae/radius*J2*(7.5*(orbTmp[2]/radius)*(orbTmp[2]/radius) - 4.5)/radius;
    orbk[1][5] = -GM*orbTmp[2]/(radius*radius*radius) + tmp*orbTmp[2];
    mtxMtp(&orbk[1][3],&orbk[1][3],1,3,&tInterval,1,1);
       		              		/* RK 第二步 */
    mtxMtp(orb,orbk[1],1,6,&a,1,1);
    mtxAdd(orbTmp,orbit,orb,1,6);
    mtxMtp(orbk[2],&orbTmp[3],3,1,&tInterval,1,1);
    radius = norm(orbTmp,3);
    tmp = GM/radius/radius*ae/radius*ae/radius*J2*(7.5*(orbTmp[2]/radius)*(orbTmp[2]/radius) - 1.5)/radius;
    orbk[2][3] = -GM*orbTmp[0]/(radius*radius*radius) + tmp*orbTmp[0];
    orbk[2][4] = -GM*orbTmp[1]/(radius*radius*radius) + tmp*orbTmp[1];	
    tmp = GM/radius/radius*ae/radius*ae/radius*J2*(7.5*(orbTmp[2]/radius)*(orbTmp[2]/radius) - 4.5)/radius;
    orbk[2][5] = -GM*orbTmp[2]/(radius*radius*radius) + tmp*orbTmp[2];
    mtxMtp(&orbk[2][3],&orbk[2][3],1,3,&tInterval,1,1);
       		              		/* RK 第三步 */
       
    mtxAdd(orbTmp,orbit,orbk[2],1,6);
    mtxMtp(orbk[3],&orbTmp[3],3,1,&tInterval,1,1);
    radius = norm(orbTmp,3);
    tmp = GM/radius/radius*ae/radius*ae/radius*J2*(7.5*(orbTmp[2]/radius)*(orbTmp[2]/radius) - 1.5)/radius;

    orbk[3][3] = -GM*orbTmp[0]/(radius*radius*radius) + tmp*orbTmp[0];
    orbk[3][4] = -GM*orbTmp[1]/(radius*radius*radius) + tmp*orbTmp[1];	
    tmp = GM/radius/radius*ae/radius*ae/radius*J2*(7.5*(orbTmp[2]/radius)*(orbTmp[2]/radius) - 4.5)/radius;
    orbk[3][5] = -GM*orbTmp[2]/(radius*radius*radius) + tmp*orbTmp[2];
    mtxMtp(&orbk[3][3],&orbk[3][3],1,3,&tInterval,1,1);

       		           		/* RK 第四步 */
    mtxAdd(orb,orbk[0],orbk[1],1,6);
    mtxAdd(orb,orb,orbk[2],1,6);
    mtxAdd(orb,orb,orbk[3],1,6);
    mtxAdd(orb,orb,orbk[1],1,6);
    mtxAdd(orb,orb,orbk[2],1,6);
    a = 1.0/6.0;
    mtxMtp(orb,orb,1,6,&a,1,1);
    mtxAdd(orbit,orbit,orb,1,6);

    mtxSub(orbInc,orbit,orbInfo,1,3);
    mtxCpy(orbInfo,orbit,1,6);

       
    return;
}


