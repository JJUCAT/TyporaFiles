#include "Include.h"

Align_t g_align;

void Align_Init(void)
{
	memset(&g_align,0,sizeof(g_align));
}

void Align_ResetSmoothAngleCnt(void)
{
	g_align.smooth_angle_cnt = 0;
	g_align.smooth_angle_idx = 0;
}

void Align_CollectSmoothAngle(int16_t angle)
{	
	g_align.smooth_angle_cnt++;
	if(g_align.smooth_angle_cnt >= ALIGN_SMOOTH_ANGLE_MAX)g_align.smooth_angle_cnt = ALIGN_SMOOTH_ANGLE_MAX;
	g_align.smooth_angle_idx++;
	if(g_align.smooth_angle_idx >= ALIGN_SMOOTH_ANGLE_MAX)g_align.smooth_angle_idx = 0;
	g_align.smooth_angle[g_align.smooth_angle_idx] = angle;
}

void Align_CollectCurveAngle(Point32_t point)
{
	static Point32_t pre_point;
	uint16_t angle = 0;
	if(g_align.curve_angle_cnt++ >= ALIGN_CURVE_ANGLE_MAX)	g_align.curve_angle_cnt = ALIGN_CURVE_ANGLE_MAX;

	if(g_align.curve_angle_cnt > 1)
	{
		angle = Math_Course2Dest(pre_point.x,pre_point.y,point.x,point.y);
		g_align.curve_angle[g_align.curve_angle_idx] = angle;
		g_align.curve_angle_idx++;
		if(g_align.curve_angle_idx >= ALIGN_CURVE_ANGLE_CNT)g_align.curve_angle_idx = 0;			
	}
	pre_point = point;		
}

void Align_ResetCurveAngleCnt(void)
{
	g_align.curve_angle_idx = 0;
	g_align.curve_angle_cnt = 0;
}

uint8_t Align_GetCurveAngleCnt(void)
{
	return g_align.curve_angle_cnt;
}

void Align_RotateMap(int16_t angle_offset)
{
	Map_Rotate_Cell(Map_GetRobotCell(),g_map.home_cell ,&g_map.home_cell ,-angle_offset);
	Map_Reset();	
	Gyro_Reset_With_Offset((3600 + angle_offset) % 3600);
	Wifi_ResetStartNav();
	Wifi_NavStart();
	Map_SetCellRobotSizeClean(Map_GetRobotCell().x,Map_GetRobotCell().y);
}


uint8_t Align_CorrectLine(AlignCorrectLine_t type)
{
	uint8_t return_flag = 0;
	switch(type)
	{
		case ALIGN_CORRECT_LINE_SMOOTH:
																if(g_align.smooth_angle_cnt < ALIGN_SMOOTH_ANGLE_MAX)
																{
																	return_flag = 1;
																}
		break;
		case ALIGN_CORRECT_LINE_CURVE:
																if(g_align.curve_angle_cnt < ALIGN_CURVE_ANGLE_MAX)
																{
																	return_flag = 1;
																}																
		break;
		default:break;
	}
	if(return_flag)return 0;

	return Algin_Correct_Map(type);
}


uint8_t Algin_Correct_Map(AlignCorrectLine_t type)
{
	uint8_t is_correct = 0,return_flag = 0;
	int16_t idx = 0;
	int16_t line_length = 0;
	int32_t delta_angle = 0;
	uint16_t robot_heading = 0;
	uint16_t bumper_angle = 0;
	int16_t line_angle = 0;
	switch(type)
	{
		case ALIGN_CORRECT_LINE_CURVE:
																robot_heading = g_align.curve_angle[g_align.curve_angle_idx];
																	
																for(idx=0;idx < ALIGN_CURVE_ANGLE_CNT;idx++)
																{
																	delta_angle = Math_RoundAngle(g_align.curve_angle[idx] - robot_heading);
																	if(Math_Abs_int(delta_angle) > ALIGN_ANGLE_CURVE)break;
																	bumper_angle += g_align.curve_angle[idx]; 
																}		
																if(idx == ALIGN_CURVE_ANGLE_CNT)
																{
																	bumper_angle /=  idx;	
																	line_angle =  bumper_angle;	
																}	
																else
																{
																	return_flag = 1;
																}
		break;	
		case ALIGN_CORRECT_LINE_SMOOTH:								
																robot_heading = g_align.smooth_angle[g_align.smooth_angle_idx];
																for(uint8_t i = 0;i <= ALIGN_SMOOTH_ANGLE_MAX ;i++)
																{
																	delta_angle = Math_RoundAngle(g_align.smooth_angle[i] - robot_heading);
																	
																	if(Math_Abs_int(delta_angle) > ALIGN_ANGLE_DELTA)break;
		
																	line_length++;
																}															
																if(line_length < ALIGN_SMOOTH_ANGLE_MAX)
																{
																	return_flag = 1;
																}	
																else
																{																	
																	line_angle = Align_CalculateSmoothAngle();	
																}		
		break;
		default:break;
	}

	if(return_flag) return 0;
	
	if(!Align_IsAlignMap())
	{
		Align_SetAlignMap();
		delta_angle = Align_GetAngleRight(line_angle);
		if(Math_Abs_int(delta_angle) > ALIGN_ANGLE_CORRECT_MIN)
		{				
			Align_RotateMap(delta_angle);		
		}
		else
		{
			Gyro_SetOffset(delta_angle);
		}		
		is_correct = 1;
	}

	if(is_correct)
	{
		Heading_Reset_Integral(HEADING_INTEGRAL_GLOBAL);
		Heading_Reset_Integral(HEADING_INTEGRAL_PREVIOUS);
		return 2;
	}	
	return 0;
}

int16_t Align_CalculateSmoothAngle(void)
{
	double simsum = 0.0,cossum = 0.0;
	int8_t offset = 0;
	int16_t ret_angle = 0;
	for(offset = 0;offset < ALIGN_SMOOTH_ANGLE_MAX;offset++)
	{
		simsum+= sin((double) g_align.smooth_angle[offset] * PI / 1800);
		cossum+= cos((double) g_align.smooth_angle[offset] * PI / 1800);
	}
		
	ret_angle = (int16_t)(atan2(simsum,cossum) * 1800 / PI);

	return ret_angle;
}

int16_t Align_GetAngleRight(int16_t angle)
{
	return angle - 900;
}

uint8_t Align_IsAlignMap(void)
{
	return g_align.map;
}

void Align_SetAlignMap(void)
{
	g_align.map = 1;
}

void Align_ResetAlginMap(void)
{
	g_align.map = 0;
}








