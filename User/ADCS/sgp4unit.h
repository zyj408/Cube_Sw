#ifndef _SGP4UNIT_H_
#define _SGP4UNIT_H_

#define PI 3.141592653589793

// -------------------------- structure declarations ----------------------------
typedef struct elsetrec
{
  int    isimp;
  double aycof  , con41  , cc1    , cc4      , cc5    , d2      , d3   , d4    ,
         delmo  , eta    , argpdot, omgcof   , sinmao , t       , t2cof, t3cof ,
         t4cof  , t5cof  , x1mth2 , x7thm1   , mdot   , nodedot, xlcof , xmcof ,
         nodecf;

  double jdsatepoch , bstar , inclo , nodeo , ecco , argpo , mo , no;
} elsetrec;


// --------------------------- function declarations ----------------------------
void sgp4init(elsetrec *satrec);

void sgp4(double r[3],double v[3],elsetrec *satrec,double *tsince);

void getgravconst(double *tumin,double *mu,double *radiusearthkm,double *xke,double *j2,double *j3,double *j4,double *j3oj2);

#endif

