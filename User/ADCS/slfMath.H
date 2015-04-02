
#ifndef _SLFMATH_H_
#define _SLFMATH_H_

int mtxCpy(double *pMtx1,double *pMtx2,int row,int col);
/* 将矩阵pMtx2拷贝给pMtx1, row表示矩阵的行数 */
/* col表示矩阵的列数，若列数为0，则表示矢量拷贝 */

int mtxMtp(double *pMtx0,double *pMtx1,int row1,int col1,double *pMtx2,int row2,int col2);
/* 矩阵pMtx1乘pMtx2, 运算的结果放在pMtx0，该函数也适合矩阵和矢量，标量和矢量、矩阵乘法 */

double norm(double *pMtx0,int cnt);
/* 求取向量的模 */

void mtxAdd(double *pMtx0,double *pMtx1,double *pMtx2,int row,int col);
/* 矩阵相加 */

void VecCross(double *pMtx0,double *pMtx1,double *pMtx2);
/* 矢量乘法 */

void mtxT(double *pMtx0,double *pMtx1,int row,int col);
/* 矩阵转置 */

void mtxSub(double *pMtx0,double *pMtx1,double *pMtx2,int row,int col);
/* 矩阵相减 */

double mtxDet(double *pMtx0,int n);
/* 矩阵行列式 */

void mtxInv(double *pMtx0,double *pMtx1,int n);
/* 矩阵逆 */

#endif
