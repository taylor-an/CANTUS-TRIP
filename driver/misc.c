
#include "cantus.h"
#ifdef CONFIG_TIMER_PRESCALE
#define PRESCALE	CONFIG_TIMER_PRESCALE
#else
#define PRESCALE 2
#endif

int esti_timer = 0;
void StartEstimate()
{
	int nch;
	U32 r_tmcon;
	for(nch=1;nch<7;nch++)
	{
		r_tmcon = 0x80021004 + 0x20*nch;
		if(!(*(volatile U32*)r_tmcon) & 1);
			break;
	}
	if(nch>7)
		return;
	esti_timer = nch;
	settimer(nch,100000);
}
void EndEstimate(char* str)
{
	if(esti_timer==0)
	{
		debugstring("etimate timer not initialized\r\n");
		return;
	}
	stoptimer(esti_timer);

	long long int nanosecpertick = 1000000000/(GetAPBclock()/PRESCALE);
	U32 upcnt =*(volatile U32*)(0x80021008 + 0x20*esti_timer);
	debugprintf("\r\n %s Time : upcnt : %d, us:%d \r\n",str,upcnt,((long long)upcnt*nanosecpertick)/1000);  
	esti_timer = 0;
}

void printmem(void* addr, U32 len)
{
	int i;
	U8* buf = (U8*)addr;
	for(i=0;i<len;i++)
	{
		if( i%16 == 0)
		{
			debugprintf("\r\n%08x:",buf+i);
		}
		debugprintf("%02x ",buf[i]);
	}
	debugprintf("\r\n\r\n");
}
