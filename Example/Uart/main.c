#include "cantus.h"
#include "tripinit.h"

#define TXBUFFER_SIZE	UART_BUF_SIZE*7
#define RXBUFFER_SIZE	UART_BUF_SIZE*7
int main()
{
	dcache_invalidate_way();
	TRIPinit();
	InitInterrupt();
	
	UartConfig(TRIP_UART_DEBUG, 115200, DATABITS_8, STOPBITS_1, UART_PARNONE);
	debugstring("================================================\r\n");
	debugprintf("Uart Rx/Tx test. System Clock(%dMhz)\r\n", GetAHBclock()/1000000);
	debugstring("================================================\r\n");
	
	U32 r1, r1cnt;
	U8 rxbuffer[RXBUFFER_SIZE];
	U8 ch;
	
	while(1)
	{
#if 0
		if(UartGetCh(TRIP_UART_DEBUG, &ch))
		{
			UartPutCh(TRIP_UART_DEBUG, ch);
		}
		
#else
		U32 rsize = UartGetDataSize(TRIP_UART_DEBUG);
		
		if(rsize != 0)
		{
			debugprintf("\r\n\r\nRead Size = %d\r\n", rsize);
			rsize = UartGetData(TRIP_UART_DEBUG, rxbuffer, rsize);
			
			r1 = 0;
			r1cnt = 0;
			while(r1 != rsize)
			{
				r1 += UartPutData(TRIP_UART_DEBUG, rxbuffer+r1, rsize-r1);
				debugprintf("\r\n\r\nr1[%d] = %d\r\n", r1cnt++, r1);
			}
		}
#endif
	}
	
	return 0;
}
