 #include "Include.h"

void BackStation(void)
{
	if(!g_station.start_state)return;
	osDelay(300);
	Action_MoveBack(BACK_SPEED,BACK_SPEED,STATION_BACK_DISTANCE);
	while((Mode_GetMode() != MODE_USERINTERFACE) && (Mode_GetMode() != MODE_CHARGE))
	{
		Event_Handler();
		if(Action_WaitForMoveEnded())
		{
			Station_SetMoveState(0);
			break;
		}
	}
	Wheel_ResetCounts();
	Wheel_Stop();
}

























