#ifndef __MYMATH_H
#define __MYMATH_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

#define PI  3.141592653589793

double Math_Absolute(double d);
int32_t Math_GetMax(int32_t a,int32_t b);
int32_t Math_Abs_int(int32_t d);
int32_t Math_Diff_int(int32_t a, int32_t b);
int32_t Math_RoundAngle(int32_t angle);
uint16_t Math_NormalAngle(int16_t angle);
double Math_Deg2Rad(double deg, int8_t scale);
double Math_Rad2Deg(double rad, int8_t scale);
void Math_Rotate(double * x, double * y, double theta);
int32_t Math_LimitingValue(int32_t input,int32_t limit);
uint16_t Math_Course2Dest(int32_t startx, int32_t starty, int32_t destx, int32_t desty);
uint8_t Math_CalArraySum(char *data_buf,uint16_t length);
uint8_t Math_IsEven(int16_t value);


#endif






