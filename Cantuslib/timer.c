#include "cantus_lib_config.h"
#include "cantus.h"
#ifdef CONFIG_TIMER_PRESCALE
#define PRESCALE	CONFIG_TIMER_PRESCALE
#else
#define PRESCALE 2
#endif
/*
PRESCALE 2: 	tick = 41 nsec, 	maxms 176093 ms
PRESCALE 8:		tick = 166 nsec, 	maxms 712964 ms
PRESCALE 32:	tick = 666 nsec, 	maxms 2860448 ms
PRESCALE 128:	tick = 2666 nsec, 	maxms 11450382 ms
PRESCALE 512:	tick = 10666 nsec,	maxms 45810121 ms
PRESCALE 8192:	tick = 170677 nsec, maxms 733052133 ms
PRESCALE 32758:	tick = 683060 nsec, maxms 0xffffffff ms
*/
BOOL settimer(int nCh,U32 ms)
{
	if(nCh > 7)
		return FALSE;
	U32 r_tpcon = 0x80021000 + 0x20*nCh;
	U32 r_tmcon = 0x80021004 + 0x20*nCh;
	U32 r_tmcnt = 0x80021008 + 0x20*nCh;
	int intnum = nCh*4+1;
	int conval;
	
	long long int nanosecpertick = 1000000000/(GetAPBclock()/PRESCALE);
	long long int usec = (long long int)ms*1000;
	long long int cnt = ((long long int)usec*1000) / nanosecpertick;
	if(cnt > 0xffffffff) // ms is too big to set, adjust PRESACLE.
		return FALSE;
	//reset
	*(volatile U32*)r_tpcon = 1<<1;
	*(volatile U32*)r_tpcon = 0;
#if PRESCALE==2
	conval = 0;
#elif PRESCALE==8
	conval = 1<<1;
#elif PRESCALE==32
	conval = 2<<1;
#elif PRESCALE==128
	conval = 3<<1;
#elif PRESCALE==512
	conval = 4<<1;
#elif PRESCALE==2048
	conval = 5<<1;
#elif PRESCALE==8192
	conval = 6<<1;
#elif PRESCALE==32768
	conval = 7<<1;
#else
#error "Invalid PRESCALE value"
#endif
	
	*(volatile U32*)r_tmcnt = cnt;
	*(volatile U32*)r_tmcon = conval|1;

	EnableInterrupt(intnum,TRUE);
	return TRUE;
}

BOOL settimer_us(int nCh,U32 us)
{
	if(nCh > 7)
		return FALSE;
	U32 r_tpcon = 0x80021000 + 0x20*nCh;
	U32 r_tmcon = 0x80021004 + 0x20*nCh;
	U32 r_tmcnt = 0x80021008 + 0x20*nCh;
	int intnum = nCh*4+1;
	int conval;
	
	long long int nanosecpertick = 1000000000/(GetAPBclock()/PRESCALE);
	long long int usec = (long long int)us*1000;
	long long int cnt = ((long long int)usec) / nanosecpertick;
	if(cnt > 0xffffffff) // ms is too big to set, adjust PRESACLE.
		return FALSE;
	//reset
	*(volatile U32*)r_tpcon = 1<<1;
	*(volatile U32*)r_tpcon = 0;
#if PRESCALE==2
	conval = 0;
#elif PRESCALE==8
	conval = 1<<1;
#elif PRESCALE==32
	conval = 2<<1;
#elif PRESCALE==128
	conval = 3<<1;
#elif PRESCALE==512
	conval = 4<<1;
#elif PRESCALE==2048
	conval = 5<<1;
#elif PRESCALE==8192
	conval = 6<<1;
#elif PRESCALE==32768
	conval = 7<<1;
#else
#error "Invalid PRESCALE value"
#endif
	
	*(volatile U32*)r_tmcnt = cnt;
	*(volatile U32*)r_tmcon = conval|1;

	EnableInterrupt(intnum,TRUE);
	return TRUE;
}

BOOL stoptimer(int nCh)
{
	if(nCh>7)
		return FALSE;
	U32 r_tmcon = 0x80021004 + 0x20*nCh;
	*(volatile U32*)r_tmcon = 0;//stop
	return TRUE;
}

U32 gettimermaxms()
{
	U32 Maxms;
	U32 tickpersec  = GetAPBclock()/PRESCALE;
	Maxms = (0xffffffff/tickpersec)*1000;
	return (U32)Maxms;
}

static void loopdelay(U32 ms)
{
	register U32 i;
	register U32 cnt;
	cnt = ms*10000; // estimated value
	for(i=0;i < cnt ;i++)
	{
		asm("nop"); // 1clock inst, 11ns
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
	}
}
void delayms(U32 ms)
{
	int nch;
	U32 r_tmcon;
	U32 r_tmcnt;
	U32 upcnt;
	U32 curcnt;
	for(nch=0;nch<7;nch++)
	{
		r_tmcon = 0x80021004 + 0x20*nch;
		if(((*(volatile U32*)r_tmcon) & 1)==0)
			break;
	}
	if(nch==7)
	{
		loopdelay(ms);
	}
	else
	{
		r_tmcnt = r_tmcon+4;
		long long int nanosecpertick = 1000000000/(GetAPBclock()/PRESCALE);
		upcnt = (U32)((long long int)ms*1000000/nanosecpertick);
		settimer(nch,100000);
		EnableInterrupt(nch*4+1,FALSE);
		while(1)
		{
			curcnt = *(volatile U32*)r_tmcnt;
			if(upcnt < curcnt)
			{
				stoptimer(nch);
				break;
			}
		}
	}
}
