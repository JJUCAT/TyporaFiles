 #include "Include.h"

double Math_Absolute(double d)
{
	return ((d < 0) ? (d * (-1)) : d);
}

int32_t Math_GetMax(int32_t a,int32_t b)
{
	return ((a > b) ? a : b);
}

int32_t Math_Diff_int(int32_t a, int32_t b)
{
	return ((a > b) ? (a - b) : (b - a));
}

int32_t Math_Abs_int(int32_t d)
{
	return ((d < 0) ? (-d) : d);
}

int32_t Math_RoundAngle(int32_t angle)
{
	if (angle >= 1800) 
	{
		angle -= 3600;
	} 
	else if (angle <= -1800) 
	{
		angle += 3600;
	}
	return angle; 
}

uint16_t Math_NormalAngle(int16_t angle)
{
	while(angle >= 3600)angle -= 3600;
	while(angle < 0)angle += 3600;

	return angle; 
}

double Math_Deg2Rad(double deg, int8_t scale)
{
	return (deg * PI / (180 * scale));
}

void Math_Rotate(double * x, double * y, double theta)
{
	double d, e;

	theta = Math_Deg2Rad(theta, 10);

	d = (*x) * cos(theta) - (*y) * sin(theta);
	e = (*x) * sin(theta) + (*y) * cos(theta);

	*x = d;
	*y = e;
}

int32_t Math_LimitingValue(int32_t input,int32_t limit)
{
	if(input > limit)return limit;
	if(input < 0)return 0;
	return input;
}

double Math_Rad2Deg(double rad, int8_t scale)
{
	return (scale * rad * 180 / PI);
}

uint16_t Math_Course2Dest(int32_t startx, int32_t starty, int32_t destx, int32_t desty)
{
	int16_t alpha = 0;

	if (startx == destx)
	{
		if (desty > starty) 
		{
			alpha = 900;
		} 
		else if (desty < starty)
		{
			alpha = 2700;
		} 
		else
		{
			alpha = 0;
		}
	} 
	else 
	{
		alpha = round(Math_Rad2Deg(atan(((double)(desty - starty) / (destx - startx))), 10));

		if (destx < startx) 
		{
			alpha += 1800;
		}

		if (alpha < 0) 
		{
			alpha += 3600;
		}
	}

	return (uint16_t)alpha;
}


uint8_t Math_CalArraySum(char *data_buf,uint16_t length)
{
	uint16_t idx = 0;
	uint8_t sum = 0 ;
	
	for(idx = 0;idx < length; idx++)
	{
		sum += *(data_buf +idx);
	}
	return sum;
}

uint8_t Math_IsEven(int16_t value)
{
	uint8_t reval = 0;

	if(value < 0)
	{
		value = -value;
	}

	if(value%2 == 0)
	{
		reval = 1;
	}

	return reval;
}









