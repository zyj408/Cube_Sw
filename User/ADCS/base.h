#ifndef _BASE_H_
#define _BASE_H_

#include <math.h>
#include <stdlib.h>
#include <mysys.h>
#include <sgp4unit.h>
#include <attCac.h>
#include <orbCac.h>
#include <slfMath.h>
#include <bsp_os.h>
#include <includes.h>

#define PI 3.141592653589793
enum BOOL {INVALID=0,VALID=1};

void adcsStart(void);

void AppTaskOrbDet(void);

void AppTaskOrbPro(void);

void AppTaskCalcuOrb(void);

void AppTaskSenGet(void);

void AppTaskMagDotDmp(void);

void AppTaskPitFltCom(void);

void AppTaskPitFltPro(void);

void AppTaskAttStaCtl(void);

#endif
