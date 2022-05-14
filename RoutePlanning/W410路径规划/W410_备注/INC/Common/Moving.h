#ifndef __MOVING_H
#define __MOVING_H

#include <stdint.h>



typedef enum
{
    MOVING_NONE = 0,
    MOVING_MOVO2POINT,
    MOVING_WALLFOLLOW,
    MOVING_DOCKING,
    MOVING_EXCEPTION,
}Moving_t;

extern  Moving_t g_moving;

void Moving_SetType(Moving_t t);
Moving_t Moving_GetType(void);

#endif








