#include <base.h>


/*     ----------------------------------------------------------------
 ----------- local functions - only ever used internally by sgp4 ---------- */
static void initl(double *ainv,double *ao,double *con41,double *con42,double *cosio,
									double *cosio2,double *einv,double *eccsq,double *omeosq,double *posq,
									double *rp,double *rteosq,double *sinio,double *po,double *ecco,
									double *inclo,double *no);

/*-----------------------------------------------------------------------------
*                           procedure initl
  ----------------------------------------------------------------------------*/
static void initl(double *ainv,double *ao,double *con41,double *con42,double *cosio,
									double *cosio2,double *einv,double *eccsq,double *omeosq,double *posq,
									double *rp,double *rteosq,double *sinio,double *po,double *ecco,
									double *inclo,double *no)
{
		 double tumin,mu,radiusearthkm,xke,j2,j3,j4,j3oj2,x2o3,ak,d1,del,adel;
     /* ----------------------- earth constants ---------------------- */
     // sgp4fix identify constants and allow alternate values
     getgravconst(&tumin,&mu,&radiusearthkm,&xke,&j2,&j3,&j4,&j3oj2);
     x2o3   = 2.0 / 3.0;

     /* ------------- calculate auxillary epoch quantities ---------- */
     *eccsq  = *(ecco) * (*ecco);
     *omeosq = 1.0 - *eccsq;
     *rteosq = sqrt(*omeosq);
     *cosio  = cos(*inclo);
     *cosio2 = (*cosio) * (*cosio);

     /* ------------------ un-kozai the mean motion ----------------- */
     ak    = pow(xke / (*no), x2o3);
     d1    = 0.75 * j2 * (3.0 * (*cosio2) - 1.0) / ((*rteosq) * (*omeosq));
     del   = d1 / (ak * ak);
     adel  = ak * (1.0 - del * del - del * (1.0 / 3.0 + 134.0 * del * del / 81.0));
     del   = d1/(adel * adel);
     *no    = (*no) / (1.0 + del);

     *ao    = pow(xke / (*no), x2o3);
     *sinio = sin(*inclo);
     *po    = (*ao) * (*omeosq);
     *con42 = 1.0 - 5.0 * (*cosio2);
     *con41 = -(*con42)-(*cosio2)-(*cosio2);
     *ainv  = 1.0 / (*ao);
		 *einv  = 1.0 / (*ecco);
     *posq  = (*po) * (*po);
     *rp    = (*ao) * (1.0 - *ecco);
}

/*-----------------------------------------------------------------------------
*                             procedure sgp4init
*              this procedure initializes variables for sgp4.
  ----------------------------------------------------------------------------*/
void sgp4init(elsetrec *satrec)
{
     /* --------------------- local variables ------------------------ */
			double tumin,mu,radiusearthkm,xke,j2,j3,j4,j3oj2,ss,qzms2ttemp,
						qzms2t,x2o3,ainv,ao,con42,cosio,cosio2,einv,eccsq,omeosq,posq,
						rp,rteosq,sinio,po,sfour,qzms24,perige,qzms24temp,pinvsq,tsi,etasq,
						eeta,psisq,coef,coef1,cc2,cc3,cosio4,temp1,temp2,temp3,xhdot1,xpidot,
						delmotemp,cc1sq,temp;
     double temp4 = 1.5e-12;

     /* ----------- set all near earth variables to zero ------------ */
     satrec->isimp   = 0;   satrec->aycof  = 0.0;
     satrec->con41   = 0.0; satrec->cc1    = 0.0; satrec->cc4      = 0.0;
     satrec->cc5     = 0.0; satrec->d2     = 0.0; satrec->d3       = 0.0;
     satrec->d4      = 0.0; satrec->delmo  = 0.0; satrec->eta      = 0.0;
     satrec->argpdot = 0.0; satrec->omgcof = 0.0; satrec->sinmao   = 0.0;
     satrec->t       = 0.0; satrec->t2cof  = 0.0; satrec->t3cof    = 0.0;
     satrec->t4cof   = 0.0; satrec->t5cof  = 0.0; satrec->x1mth2   = 0.0;
     satrec->x7thm1  = 0.0; satrec->mdot   = 0.0; satrec->nodedot  = 0.0;
     satrec->xlcof   = 0.0; satrec->xmcof  = 0.0; satrec->nodecf   = 0.0;

     /* ------------------------ earth constants ----------------------- */
     // sgp4fix identify constants and allow alternate values
     getgravconst(&tumin,&mu,&radiusearthkm,&xke,&j2,&j3,&j4,&j3oj2);
     ss = 78.0 / radiusearthkm + 1.0;
     // sgp4fix use multiply for speed instead of pow
     qzms2ttemp = (120.0 - 78.0) / radiusearthkm;
     qzms2t = qzms2ttemp * qzms2ttemp * qzms2ttemp * qzms2ttemp;
     x2o3   =  2.0 / 3.0;

     satrec->t	 = 0.0;

		 initl(&ainv,&ao,&satrec->con41,&con42,&cosio,&cosio2,&einv,&eccsq,&omeosq,&posq,
					&rp,&rteosq,&sinio,&po,&satrec->ecco, &satrec->inclo, &satrec->no);

     if ((omeosq >= 0.0 ) || ( satrec->no >= 0.0))
       {
         satrec->isimp = 0;
         if (rp < (220.0 / radiusearthkm + 1.0))
             satrec->isimp = 1;
         sfour  = ss;
         qzms24 = qzms2t;
         perige = (rp - 1.0) * radiusearthkm;

         /* - for perigees below 156 km, s and qoms2t are altered - */
         if (perige < 156.0)
           {
             sfour = perige - 78.0;
             if (perige < 98.0)
                 sfour = 20.0;
             // sgp4fix use multiply for speed instead of pow
             qzms24temp =  (120.0 - sfour) / radiusearthkm;
             qzms24 = qzms24temp * qzms24temp * qzms24temp * qzms24temp;
             sfour  = sfour / radiusearthkm + 1.0;
           }
         pinvsq = 1.0 / posq;

         tsi  = 1.0 / (ao - sfour);
         satrec->eta  = ao * satrec->ecco * tsi;
         etasq = satrec->eta * satrec->eta;
         eeta  = satrec->ecco * satrec->eta;
         psisq = fabs(1.0 - etasq);
         coef  = qzms24 * pow(tsi, 4.0);
         coef1 = coef / pow(psisq, 3.5);
         cc2   = coef1 * satrec->no * (ao * (1.0 + 1.5 * etasq + eeta *
                        (4.0 + etasq)) + 0.375 * j2 * tsi / psisq * satrec->con41 *
                        (8.0 + 3.0 * etasq * (8.0 + etasq)));
         satrec->cc1   = satrec->bstar * cc2;
         cc3   = 0.0;
         if (satrec->ecco > 1.0e-4)
             cc3 = -2.0 * coef * tsi * j3oj2 * satrec->no * sinio / satrec->ecco;
         satrec->x1mth2 = 1.0 - cosio2;
         satrec->cc4    = 2.0* satrec->no * coef1 * ao * omeosq *
                           (satrec->eta * (2.0 + 0.5 * etasq) + satrec->ecco *
                           (0.5 + 2.0 * etasq) - j2 * tsi / (ao * psisq) *
                           (-3.0 * satrec->con41 * (1.0 - 2.0 * eeta + etasq *
                           (1.5 - 0.5 * eeta)) + 0.75 * satrec->x1mth2 *
                           (2.0 * etasq - eeta * (1.0 + etasq)) * cos(2.0 * satrec->argpo)));
         satrec->cc5 = 2.0 * coef1 * ao * omeosq * (1.0 + 2.75 *
                        (etasq + eeta) + eeta * etasq);
         cosio4 = cosio2 * cosio2;
         temp1  = 1.5 * j2 * pinvsq * satrec->no;
         temp2  = 0.5 * temp1 * j2 * pinvsq;
         temp3  = -0.46875 * j4 * pinvsq * pinvsq * satrec->no;
         satrec->mdot     = satrec->no + 0.5 * temp1 * rteosq * satrec->con41 + 0.0625 *
                            temp2 * rteosq * (13.0 - 78.0 * cosio2 + 137.0 * cosio4);
         satrec->argpdot  = -0.5 * temp1 * con42 + 0.0625 * temp2 *
                             (7.0 - 114.0 * cosio2 + 395.0 * cosio4) +
                             temp3 * (3.0 - 36.0 * cosio2 + 49.0 * cosio4);
         xhdot1            = -temp1 * cosio;
         satrec->nodedot = xhdot1 + (0.5 * temp2 * (4.0 - 19.0 * cosio2) +
                              2.0 * temp3 * (3.0 - 7.0 * cosio2)) * cosio;
         xpidot            =  satrec->argpdot+ satrec->nodedot;
         satrec->omgcof   = satrec->bstar * cc3 * cos(satrec->argpo);
         satrec->xmcof    = 0.0;
         if (satrec->ecco > 1.0e-4)
             satrec->xmcof = -x2o3 * coef * satrec->bstar / eeta;
         satrec->nodecf = 3.5 * omeosq * xhdot1 * satrec->cc1;
         satrec->t2cof   = 1.5 * satrec->cc1;
         // sgp4fix for divide by zero with xinco = 180 deg
         if (fabs(cosio+1.0) > 1.5e-12)
             satrec->xlcof = -0.25 * j3oj2 * sinio * (3.0 + 5.0 * cosio) / (1.0 + cosio);
           else
             satrec->xlcof = -0.25 * j3oj2 * sinio * (3.0 + 5.0 * cosio) / temp4;
         satrec->aycof   = -0.5 * j3oj2 * sinio;
         // sgp4fix use multiply for speed instead of pow
         delmotemp = 1.0 + satrec->eta * cos(satrec->mo);
         satrec->delmo   = delmotemp * delmotemp * delmotemp;
         satrec->sinmao  = sin(satrec->mo);
         satrec->x7thm1  = 7.0 * cosio2 - 1.0;

       /* ----------- set variables if not deep space ----------- */
       if (satrec->isimp != 1)
         {
           cc1sq          = satrec->cc1 * satrec->cc1;
           satrec->d2    = 4.0 * ao * tsi * cc1sq;
           temp           = satrec->d2 * tsi * satrec->cc1 / 3.0;
           satrec->d3    = (17.0 * ao + sfour) * temp;
           satrec->d4    = 0.5 * temp * ao * tsi * (221.0 * ao + 31.0 * sfour) *
                            satrec->cc1;
           satrec->t3cof = satrec->d2 + 2.0 * cc1sq;
           satrec->t4cof = 0.25 * (3.0 * satrec->d3 + satrec->cc1 *
                            (12.0 * satrec->d2 + 10.0 * cc1sq));
           satrec->t5cof = 0.2 * (3.0 * satrec->d4 +
                            12.0 * satrec->cc1 * satrec->d3 +
                            6.0 * satrec->d2 * satrec->d2 +
                            15.0 * cc1sq * (2.0 * satrec->d2 + cc1sq));
         }
       } 
}

/*-----------------------------------------------------------------------------
*                             procedure sgp4
  ----------------------------------------------------------------------------*/
void sgp4(double r[3],double v[3],elsetrec *satrec,double *tsince)
{
			double twopi,x2o3,tumin,mu,radiusearthkm,xke,j2,j3,j4,j3oj2,vkmpersec,
					xmdf,argpdf,nodedf,argpm,mm,t2,nodem,tempa,tempe,templ,delomg,delmtemp,
					delm,temp,t3,t4,nm,em,inclm,am,xlm,emsq,sinim,cosim,ep,xincp,argpp,nodep,
					mp,sinip,cosip,axnl,aynl,xl,u,eo1,tem5,sineo1,coseo1,esine,ecose,el2,pl,
					rl,rdotl,rvdotl,betal,sinu,cosu,su,sin2u,cos2u,temp1,temp2,mrt,xnode,xinc,
					mvt,rvdot,sinsu,cossu,snod,cnod,sini,cosi,xmx,xmy,ux,uy,uz,vx,vy,vz;
			int ktr;
     /* ------------------ set mathematical constants --------------- */
     twopi = 2.0 * PI;
     x2o3  = 2.0 / 3.0;
     // sgp4fix identify constants and allow alternate values
     getgravconst(&tumin,&mu,&radiusearthkm,&xke,&j2,&j3,&j4,&j3oj2);
     vkmpersec = radiusearthkm * xke/60.0;

     /* --------------------- clear sgp4 error flag ----------------- */
     satrec->t = *tsince;

     /* ------- update for secular gravity and atmospheric drag ----- */
     xmdf    = satrec->mo + satrec->mdot * satrec->t;
     argpdf  = satrec->argpo + satrec->argpdot * satrec->t;
     nodedf  = satrec->nodeo + satrec->nodedot * satrec->t;
     argpm   = argpdf;
     mm      = xmdf;
     t2      = satrec->t * satrec->t;
     nodem   = nodedf + satrec->nodecf * t2;
     tempa   = 1.0 - satrec->cc1 * satrec->t;
     tempe   = satrec->bstar * satrec->cc4 * satrec->t;
     templ   = satrec->t2cof * t2;

     if (satrec->isimp != 1)
       {
         delomg = satrec->omgcof * satrec->t;
         // sgp4fix use mutliply for speed instead of pow
         delmtemp =  1.0 + satrec->eta * cos(xmdf);
         delm   = satrec->xmcof *
                  (delmtemp * delmtemp * delmtemp -
                  satrec->delmo);
         temp   = delomg + delm;
         mm     = xmdf + temp;
         argpm  = argpdf - temp;
         t3     = t2 * satrec->t;
         t4     = t3 * satrec->t;
         tempa  = tempa - satrec->d2 * t2 - satrec->d3 * t3 -
                          satrec->d4 * t4;
         tempe  = tempe + satrec->bstar * satrec->cc5 * (sin(mm) -
                          satrec->sinmao);
         templ  = templ + satrec->t3cof * t3 + t4 * (satrec->t4cof +
                          satrec->t * satrec->t5cof);
       }

     nm    = satrec->no;
     em    = satrec->ecco;
     inclm = satrec->inclo;

     am = pow((xke / nm),x2o3) * tempa * tempa;
     nm = xke / pow(am, 1.5);
     em = em - tempe;
			 
     if (em < 1.0e-6)
         em  = 1.0e-6;
     mm     = mm + satrec->no * templ;
     xlm    = mm + argpm + nodem;
     emsq   = em * em;
     temp   = 1.0 - emsq;

     nodem  = fmod(nodem, twopi);
     argpm  = fmod(argpm, twopi);
     xlm    = fmod(xlm, twopi);
     mm     = fmod(xlm - argpm - nodem, twopi);

     /* ----------------- compute extra mean quantities ------------- */
     sinim = sin(inclm);
     cosim = cos(inclm);

     /* -------------------- add lunar-solar periodics -------------- */
     ep     = em;
     xincp  = inclm;
     argpp  = argpm;
     nodep  = nodem;
     mp     = mm;
     sinip  = sinim;
     cosip  = cosim;

     /* -------------------- long period periodics ------------------ */
     axnl = ep * cos(argpp);
     temp = 1.0 / (am * (1.0 - ep * ep));
     aynl = ep* sin(argpp) + temp * satrec->aycof;
     xl   = mp + argpp + nodep + temp * satrec->xlcof * axnl;

     /* --------------------- solve kepler's equation --------------- */
     u    = fmod(xl - nodep, twopi);
     eo1  = u;
     tem5 = 9999.9;
     ktr = 1;
     while (( fabs(tem5) >= 1.0e-12) && (ktr <= 10) )
       {
         sineo1 = sin(eo1);
         coseo1 = cos(eo1);
         tem5   = 1.0 - coseo1 * axnl - sineo1 * aynl;
         tem5   = (u - aynl * coseo1 + axnl * sineo1 - eo1) / tem5;
         if(fabs(tem5) >= 0.95)
             tem5 = tem5 > 0.0 ? 0.95 : -0.95;
         eo1    = eo1 + tem5;
         ktr = ktr + 1;
       }

     /* ------------- short period preliminary quantities ----------- */
     ecose = axnl*coseo1 + aynl*sineo1;
     esine = axnl*sineo1 - aynl*coseo1;
     el2   = axnl*axnl + aynl*aynl;
     pl    = am*(1.0-el2);
     if (pl < 0.0)
       {
				 *r = 0;
         *(r+1) = 0;
         *(r+2) = 0;
         *v = 0;
         *(v+1) = 0;
         *(v+2) = 0;
       }
       else
       {
         rl     = am * (1.0 - ecose);
         rdotl  = sqrt(am) * esine/rl;
         rvdotl = sqrt(pl) / rl;
         betal  = sqrt(1.0 - el2);
         temp   = esine / (1.0 + betal);
         sinu   = am / rl * (sineo1 - aynl - axnl * temp);
         cosu   = am / rl * (coseo1 - axnl + aynl * temp);
         su     = atan2(sinu, cosu);
         sin2u  = (cosu + cosu) * sinu;
         cos2u  = 1.0 - 2.0 * sinu * sinu;
         temp   = 1.0 / pl;
         temp1  = 0.5 * j2 * temp;
         temp2  = temp1 * temp;

         /* -------------- update for short period periodics ------------ */
         mrt   = rl * (1.0 - 1.5 * temp2 * betal * satrec->con41) +
                 0.5 * temp1 * satrec->x1mth2 * cos2u;
         su    = su - 0.25 * temp2 * satrec->x7thm1 * sin2u;
         xnode = nodep + 1.5 * temp2 * cosip * sin2u;
         xinc  = xincp + 1.5 * temp2 * cosip * sinip * cos2u;
         mvt   = rdotl - nm * temp1 * satrec->x1mth2 * sin2u / xke;
         rvdot = rvdotl + nm * temp1 * (satrec->x1mth2 * cos2u +
                 1.5 * satrec->con41) / xke;

         /* --------------------- orientation vectors ------------------- */
         sinsu =  sin(su);
         cossu =  cos(su);
         snod  =  sin(xnode);
         cnod  =  cos(xnode);
         sini  =  sin(xinc);
         cosi  =  cos(xinc);
         xmx   = -snod * cosi;
         xmy   =  cnod * cosi;
         ux    =  xmx * sinsu + cnod * cossu;
         uy    =  xmy * sinsu + snod * cossu;
         uz    =  sini * sinsu;
         vx    =  xmx * cossu - cnod * sinsu;
         vy    =  xmy * cossu - snod * sinsu;
         vz    =  sini * cossu;

         /* --------- position and velocity (in km and km/sec) ---------- */
         *r = (mrt * ux)* radiusearthkm;
         *(r+1) = (mrt * uy)* radiusearthkm;
         *(r+2) = (mrt * uz)* radiusearthkm;
         *v = (mvt * ux + rvdot * vx) * vkmpersec;
         *(v+1) = (mvt * uy + rvdot * vy) * vkmpersec;
         *(v+2) = (mvt * uz + rvdot * vz) * vkmpersec;
       } 

}

/* ------------ wgs-84 constants ------------*/
void getgravconst(double *tumin,double *mu,double *radiusearthkm,double *xke,double *j2,double *j3,double *j4,double *j3oj2)
{   
      *mu     = 398600.5;            // in km3 / s2
      *radiusearthkm = 6378.137;     // km
      *xke    = 60.0 / sqrt((*radiusearthkm)*(*radiusearthkm)*(*radiusearthkm)/(*mu));
      *tumin  = 1.0 / (*xke);
      *j2     =   0.00108262998905;
      *j3     =  -0.00000253215306;
      *j4     =  -0.00000161098761;
      *j3oj2  =  (*j3) / (*j2);
}
