 #include "Include.h"

Heading_Integral_t g_heading_integral;
Heading_GetAngle on_heading_get_angle;

void Heading_Init(void)
{
	memset(&g_heading_integral,0,sizeof(g_heading_integral));
	on_heading_get_angle = Gyro_GetAngle;
}

void Heading_Update_Integral(uint16_t cur_heading)
{
	int16_t heading_diff = 0;
	
	heading_diff = Math_RoundAngle(g_heading_integral.previous - cur_heading);
	
	g_heading_integral.movetoposition += heading_diff;
	g_heading_integral.wallfollow += heading_diff;
	g_heading_integral.global += heading_diff;
	g_heading_integral.head2cource += heading_diff;
	
	g_heading_integral.previous = cur_heading;
}

int32_t Heading_Get_Integral(Heading_Integral_Type_t type)
{
	switch(type)
	{
		case HEADING_INTEGRAL_PREVIOUS:
												return g_heading_integral.previous;
									
		case HEADING_INTEGRAL_WALLFOLLOW:
												return g_heading_integral.wallfollow;
												
		case HEADING_INTEGRAL_GLOBAL:
												return g_heading_integral.global;
												
		case HEADING_INTEGRAL_MOVETOPOSITION:
												return g_heading_integral.movetoposition;
		case HEADING_INTEGRAL_HEAD2COURCE:
												return g_heading_integral.head2cource;
		default:
												return 0;
	}
}

void Heading_Reset_Integral(Heading_Integral_Type_t type)
{
	switch(type)
	{
		case HEADING_INTEGRAL_PREVIOUS:
												g_heading_integral.previous = 0;
												break;
		case HEADING_INTEGRAL_WALLFOLLOW:
												g_heading_integral.wallfollow = 0;
												break;
		case HEADING_INTEGRAL_GLOBAL:
												g_heading_integral.global = 0;
												break;
		case HEADING_INTEGRAL_MOVETOPOSITION:
												g_heading_integral.movetoposition = 0;
												break;
		case HEADING_INTEGRAL_HEAD2COURCE:
												g_heading_integral.head2cource = 0;
		default:
												break;
	}
}

HeadingDir_t Heading_GetHeading4(uint16_t angle)
{
	if((angle <= 450) || (angle > 3150))
	{
		return HEADING_DIR_EAST;
	}
	if((angle > 450) && (angle <= 1350))
	{
		return HEADING_DIR_NORTH;
	}
	if((angle > 1350) && (angle <= 2250))
	{
		return HEADING_DIR_WEST;
	}
	if((angle > 2250) && (angle <= 3150))
	{
		return HEADING_DIR_SOUTH;
	}
	return HEADING_DIR_EAST;
}

uint16_t Heading_GetHeading8(void)
{
	uint16_t temp_angle = on_heading_get_angle(0);
	if((temp_angle > 3375) || (temp_angle <= 225))
	{
		return HEADING_DIR_EAST;
	}
	if((temp_angle > 225) && (temp_angle <= 675))
	{
		return HEADING_DIR_NORTH_EAST;
	}
	if((temp_angle > 675) && (temp_angle <= 1125))
	{
		return HEADING_DIR_NORTH;
	}
	if((temp_angle > 1125) && (temp_angle <= 1575))
	{
		return HEADING_DIR_NORTH_WEST;
	}
	if((temp_angle > 1575) && (temp_angle <= 2025))
	{
		return HEADING_DIR_WEST;
	}
	if((temp_angle > 2025) && (temp_angle <= 2475))
	{
		return HEADING_DIR_SOUTH_WEST;
	}
	if((temp_angle > 2475) && (temp_angle <= 2925))
	{
		return HEADING_DIR_SOUTH;
	}
	if((temp_angle > 2925) && (temp_angle <= 3375))
	{
		return HEADING_DIR_SOUTH_EAST;
	}
	return 0;
}

uint16_t Heading_GetCurrentDir(void)
{
	uint16_t dir;
	uint16_t temp_angle = on_heading_get_angle(0);

	dir = (uint16_t) round(((double)temp_angle) / 100);
	switch(dir) 
	{
		case 0:
		case 36:
			dir = (uint16_t)HEADING_DIR_EAST;
			break;
		case 9:
			dir = (uint16_t)HEADING_DIR_NORTH;
			break;
		case 18:
			dir = (uint16_t)HEADING_DIR_WEST;
			break;
		case 27:
			dir = (uint16_t)HEADING_DIR_SOUTH;
			break;
		default:			
			dir = (uint16_t)Heading_GetHeading4(temp_angle);		
			break;
	}
	return dir;
}






















