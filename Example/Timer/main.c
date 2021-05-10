#include "cantus.h"
#include "tripinit.h"

void timer0isr(void);

volatile U32 led_state = 0;

int main()
{
	dcache_invalidate_way();
	TRIPinit();
	InitInterrupt();
	
	UartConfig(TRIP_UART_DEBUG, 115200, DATABITS_8, STOPBITS_1, UART_PARNONE);
	debugstring("================================================\r\n");
	debugprintf(" Timer. System Clock(%dMhz)\r\n", GetAHBclock()/1000000);
	debugstring("================================================\r\n");
	
	setinterrupt(INTNUM_TIMER0, timer0isr);
	settimer(0, 100);
	
	while(1);
	
	return 0;
}

void timer0isr(void)
{
	if(led_state == 1)
	{
		*TRIP_R_LED_ON = TRIP_F_LED;
	}
	else
	{
		*TRIP_R_LED_OFF = TRIP_F_LED;
	}
	
	led_state ^= 1;
}
