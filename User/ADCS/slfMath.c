/* 引用头文件 */
#include <base.h>

/* 矩阵复制函数 */
int mtxCpy(double *pMtx1,double *pMtx2,int row,int col)
{ 
    int i,j;
	
    for(i=0;i<row;i=i+1)
    	for(j=0;j<col;j=j+1)
    	    *(pMtx1 + i*col + j) = *(pMtx2 + i*col + j);

    return 1;
}

/* 矩阵乘法函数 */
int mtxMtp(double *pMtx0,double *pMtx1,int row1,int col1,double *pMtx2,int row2,int col2)
{
    int i,j,k;
    
    if((row1 < 2)&&(col1 < 2))
    {
    	for(i=0;i<row2;i=i+1)
    	   for(j=0;j<col2;j=j+1)
    	       *(pMtx0 + i*col2 + j) = (*(pMtx1)) * (*(pMtx2 + i*col2 + j));
    	return 1;
    }
    if((row2 < 2)&&(col2 < 2))
    {
    	for(i=0;i<row1;i=i+1)
    	   for(j=0;j<col1;j=j+1)
    	       *(pMtx0 + i*col1 + j) = (*(pMtx1 + i*col1 + j)) * (*(pMtx2));
    	return 1;
    }
    if(fabs(col1 - row2) < 1)
    {
    	for(i=0;i<row1;i=i+1)
    	   for(j=0;j<col2;j=j+1)
    	   {
    	      *(pMtx0 + i*col2 + j) = 0;
    	      for(k=0;k<row2;k=k+1)
    	         *(pMtx0 + i*col2 + j) += (*(pMtx1 + i*col1 + k)) * (*(pMtx2 + k*col2 + j));
    	   }
        return 1;
    }
    
    return 0;
}

/* 向量求模 */
double norm(double *pMtx0,int cnt)
{
    int i;
    double tmp;
    
    tmp = 0;
    for(i=0;i<cnt;i=i+1)
       tmp = tmp + (*(pMtx0+i))*(*(pMtx0+i));
    tmp = sqrt(tmp); 
    
    return tmp;
}

/* 矩阵相加 */
void mtxAdd(double *pMtx0,double *pMtx1,double *pMtx2,int row,int col)
{
    int i,j;
    
    for(i=0;i<row;i=i+1)
       for(j=0;j<col;j=j+1)
           *(pMtx0 + i*col + j) = (*(pMtx1 + i*col + j)) + (*(pMtx2 + i*col + j));
    
    return;
}

/* 矢量叉乘法 */
void VecCross(double *pMtx0,double *pMtx1,double *pMtx2)
{
    double Mtx[3][3];
    
    Mtx[0][0] = 0;Mtx[0][1] = -(*(pMtx1 + 2));Mtx[0][2] = (*(pMtx1 + 1));
    Mtx[1][0] = (*(pMtx1 + 2));Mtx[1][1] = 0;Mtx[1][2] = -(*pMtx1);
    Mtx[2][0] = -(*(pMtx1 + 1));Mtx[2][1] = (*pMtx1);Mtx[2][2] = 0;
    
    mtxMtp(pMtx0,(double*)Mtx,3,3,pMtx2,3,1);
    
    return;
}

/* 矩阵转置 */
void mtxT(double *pMtx0,double *pMtx1,int row,int col)
{
    int i,j;
   
    for(i=0;i<row;i=i+1)
    	for(j=0;j<col;j=j+1)
    	    *(pMtx0 + i*col + j) = *(pMtx1 + j*row + i);
    
    return;
}

/* 矩阵相减 */
void mtxSub(double *pMtx0,double *pMtx1,double *pMtx2,int row,int col)
{
    int i,j;
    
    for(i=0;i<row;i=i+1)
       for(j=0;j<col;j=j+1)
           *(pMtx0 + i*col + j) = (*(pMtx1 + i*col + j)) - (*(pMtx2 + i*col + j));
    
    return;
}

/* 矩阵行列式 */
double mtxDet(double *pMtx0,int n)
{
    int r, c, m;
    int lop = 0;
    double result = 0;
    double mid = 1;

    if (n != 1)
    {
        lop = (n == 2) ? 1 : n;       
        for (m = 0; m < lop; m++)
        {
            mid = 1;            
            for (r = 0, c = m; r < n; r++, c++)
            {
                mid = mid * (*(pMtx0+r*n+c%n));
            }
            result += mid;
        }
        for (m = 0; m < lop; m++)
        {
            mid = 1;          
            for (r = 0, c = n-1-m+n; r < n; r++, c--)
            {
                mid = mid * (*(pMtx0+r*n+c%n));
            }
            result -= mid;
        }
    }
    else 
        result = *pMtx0;
    return result;
}

/* 矩阵逆*/
void mtxInv(double *pMtx0,double *pMtx1,int n)
{
		int i,j,k,t;
		double det;
		double temp[9];              //只能计算3X3矩阵的逆矩阵，当定义*temp时分配地址出错
		det = mtxDet((double *)pMtx1,n);
	  for(i=0;i<n;i++)  
    {  
        for(j=0;j<n;j++)  
        {  
            for(k=0;k<n-1;k++)  
            {  
                for(t=0;t<n-1;t++)  
                {  
										if (k>=i)
												if (t>=j)
														*(temp+k*(n-1)+t) = *(pMtx1+n*(k+1)+t+1);  
												else
														*(temp+k*(n-1)+t) = *(pMtx1+n*(k+1)+t);  
										else
												if (t>=j)
														*(temp+k*(n-1)+t) = *(pMtx1+n*k+t+1);  
												else
														*(temp+k*(n-1)+t) = *(pMtx1+n*k+t);
                }  
            }  
            if((i+j)%2 == 1)  
                *(pMtx0+j*n+i) = - mtxDet((double *)temp,n-1) / det;  
						else
								*(pMtx0+j*n+i) = mtxDet((double *)temp,n-1) / det;  
        }
    }
}
