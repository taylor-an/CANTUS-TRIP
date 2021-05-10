#include "cantus.h"
#include "tripinit.h"
#include "trip.h"
#include "timer_tick.h"

int main()
{
	dcache_invalidate_way();
	TRIPinit();
	InitInterrupt();
	
	UartConfig(TRIP_UART_DEBUG, 115200, DATABITS_8, STOPBITS_1, UART_PARNONE);
	debugstring("\r\n================================================\r\n");
	debugprintf(" W5500 TEST. System Clock(%dMhz)\r\n", GetAHBclock()/1000000);
	debugstring("================================================\r\n");
	
	timer_init(1);
	
	while(1)
	{
		standby(2);
	}
	
	return 0;
}


