// trip.c

#include "trip.h"

void standby(U32 ms_100)
{
	static bool set = FALSE;
	static bool led_on = FALSE;
	static U32 time_stamp_100ms;
	
	if(set == FALSE)
	{
		set = TRUE;
		time_stamp_100ms = get_timer_tick_100ms();
	}
	else
	{
		if(TRUE == time_over_check(time_stamp_100ms, getp_timer_tick_100ms(), ms_100))
		{
			set = FALSE;
			if(led_on == TRUE)
			{
				led_on = FALSE;
				*TRIP_R_LED_OFF = TRIP_F_LED;
			}
			else
			{
				led_on = TRUE;
				*TRIP_R_LED_ON = TRIP_F_LED;
			}
		}
	}
}
