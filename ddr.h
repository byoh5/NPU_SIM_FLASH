
#ifndef DDR_H_
#define DDR_H_

#include "en675.h"

char ddr_cal_test();
void DdrTest(void);
void DdrParamBypass(UINT var);
void CaOutDly(int var);
void DqOutDly(UINT var);
void DmOutDly(UINT var);
void DdrInit(BYTE Sel);

#endif /* DDR_H_ */
