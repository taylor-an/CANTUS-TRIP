#include "cantus.h"
#include "tripinit.h"

int main()
{
	dcache_invalidate_way();
	TRIPinit();
	InitInterrupt();
	
	UartConfig(TRIP_UART_DEBUG, 115200, DATABITS_8, STOPBITS_1, UART_PARNONE);
	debugstring("================================================\r\n");
	debugprintf(" LED TEST. System Clock(%dMhz)\r\n", GetAHBclock()/1000000);
	debugstring("================================================\r\n");
	
	while(1)
	{
		*TRIP_R_LED_ON = TRIP_F_LED;
		delayms(200);
		
		*TRIP_R_LED_OFF = TRIP_F_LED;
		delayms(200);
	}
	
	return 0;
}
