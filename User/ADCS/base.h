#ifndef _BASE_H_
#define _BASE_H_

#include <math.h>
#include <stdlib.h>
#include <sgp4unit.h>
#include <attCac.h>
#include <slfMath.h>
#include <bsp_os.h>
#include <includes.h>
#include  <globalavr_adcs.h>

#define PI 3.141592653589793
enum BOOL {INVALID=0,VALID=1};

void AppTaskSenGet(void);

void AppTaskMagDotDmp(void);

void AppTaskPitFltCom(void);

void AppTaskAttStaCtl(void);

#endif
