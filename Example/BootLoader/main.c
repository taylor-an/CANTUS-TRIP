#include "cantus.h"
#include "tripinit.h"

void download_main(void);
void TRIP_Download(void);
void timer0isr(void);

static volatile U32 mode_wait_time = 0;

int main()
{
	void (*AppFunc)();
	char ch;
	
	dcache_invalidate_way();
	TRIPinit();
	InitInterrupt();
	
	UartConfig(TRIP_UART_DEBUG, 115200, DATABITS_8, STOPBITS_1, UART_PARNONE);
	debugstring("================================================\r\n");
	debugprintf(" BootLoader. System Clock(%dMhz)\r\n", GetAHBclock()/1000000);
	debugstring("================================================\r\n");
	
	setinterrupt(INTNUM_TIMER0, timer0isr);
	settimer(0, 100);
	
	while(mode_wait_time != 10)
	{
		if(UartGetCh(TRIP_UART_DEBUG, &ch))
		{
			if(ch == 'c' || ch == 'C')
			{
				TRIP_Download();
			}
		}
		
		if(TRIP_F_KEY_SET == (*TRIP_R_KEY & TRIP_F_KEY))
		{
			TRIP_Download();			
		}
	}
	
	stoptimer(0);
	
	U8 norid[2];
	int nor_size;
	U32 app_loaded_offset;
	
	norflash_readid(norid);
	nor_size = norflash_get_size();
	debugprintf("Nor-Flash Size : 0x%x\r\n", nor_size);
	
	if(nor_size == 0x20000)
	{
		// CANTUS 128A
		app_loaded_offset = 32*1024;	// Sector Address 0x8000
	}
	else
	{
		// CANTUS 512
		app_loaded_offset = 64*1024;	// Sector Address 0x10000
	}
	
	AppFunc = (void (*)())(*(U32*)(app_loaded_offset));
	
	if( ((U32)AppFunc < app_loaded_offset) || ((U32)AppFunc > nor_size))
	{
		debugprintf("AppFunc(0x%x) is invalid\r\n", AppFunc);
	}
	else
	{
		debugstring("Run Application\r\n");
		AppFunc();
	}
	
	return 0;
}

void TRIP_Download(void)
{
	stoptimer(0);
	
	USB_Enable();
	delayms(10);
	
	debugstring("USB-Communication Mode\r\n");
	download_main();
	
	while(1);	
}

void timer0isr(void)
{
	mode_wait_time++;
}
