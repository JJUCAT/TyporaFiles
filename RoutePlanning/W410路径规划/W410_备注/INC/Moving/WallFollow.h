#ifndef __WALLFOLLOW_H
#define __WALLFOLLOW_H

#include <stdint.h>
#include "Hw_SysInitialize.h"


typedef enum
{
	WALLFOLLOW_NONE = 0,
	WALLFOLLOW_MODE,
	WALLFOLLOW_TRAPPED,
	WALLFOLLOW_SPOT,
	WALLFOLLOW_FIND,
	WALLFOLLOW_NEXTLANE,
	WALLFOLLOW_CHARGE,
}WallFollow_t;





TouringResult_t WallFollow(Point32_t start_cnt, WallDir_t wall_dir, WallFollow_t wallfollow_state,Touring_t touring);








#endif































