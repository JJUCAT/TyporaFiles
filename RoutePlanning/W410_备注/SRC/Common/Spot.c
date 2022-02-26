#include "Include.h"

Spot_t g_spot;

void Spot_Init(void)
{
    memset(&g_spot,0,sizeof(g_spot));
    g_spot.dir_outin = SPOT_DIR_OUT;
    g_spot.dir_cw = SPOT_DIR_CW;
}

void Spot_SetDirCw(SpotDir_t dir)
{
	g_spot.dir_cw = dir;
}

SpotDir_t Spot_GetDirCw(void)
{
	return g_spot.dir_cw;
}

void Spot_SetDirOutin(SpotDir_t dir)
{
	 g_spot.dir_outin = dir;
}

SpotDir_t Spot_GetDirOutin(void)
{
	return g_spot.dir_outin;
}

void Spot_SetLaneIdx(uint8_t idx)
{
	g_spot.lane_idx = idx;
}

uint8_t Spot_GetLaneIdx(void)
{
	return g_spot.lane_idx;
}

Point16_t Spot_GetNextLanePos(Point16_t start_cell,uint8_t dir,uint8_t step)
{
    Point16_t ret_cell = start_cell;

    if(Math_Abs_int(start_cell.x) >= Math_Abs_int(start_cell.y))
    {
        if(start_cell.x > 0)
        {
            if(dir)
            {
                ret_cell.x += 1;	
            }
            else
            {
                ret_cell.x -= 1;
            }
        }
        else 
        {
            if(dir)
            {
                ret_cell.x -= 1;
            }
            else
            {
                ret_cell.x += 1;
            }
        }
    }
    else
    {
        if(start_cell.y > 0)
        {
            if(dir)
            {
                ret_cell.y += 1;
            }
            else
            {
                ret_cell.y -= 1;
            }
        }
        else                
        {
            if(dir)
            {
                ret_cell.y -= 1;
            }
            else
            {
                ret_cell.y += 1;
            }
        }
    }

    return ret_cell;
}

uint8_t Spot_IsOnLane(uint8_t idx,Point16_t robot_cell)   
{
	uint8_t current_idx = 0;
	
	current_idx = Math_GetMax(Math_Absolute(robot_cell.x),Math_Absolute(robot_cell.y));
	
	if(current_idx == idx)
	{		
		return 1;	
	}
	return 0;
}

uint16_t Spot_GetPointLaneIdx(Point16_t point)  
{
    uint16_t 	temp_idx = 0;

    temp_idx = Math_GetMax(Math_Absolute(point.x),Math_Absolute(point.y));

    return temp_idx;
}




















