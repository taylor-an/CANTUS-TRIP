#include "cantus.h"

#define AK4183_ADDR		0x92

#define	CMD_SLEEP		0x70
#define CMD_Driver_X	0x80
#define CMD_Driver_Y	0x90
#define CMD_Driver_YP	0xA0
#define CMD_Driver_XM	0xB0
#define	CMD_READ_X		0xC0
#define	CMD_READ_Y		0xD0
#define CMD_Z1			0xE0
#define CMD_Z2			0xF0

#define REG_TWICON	(volatile U8*)0x80022000
#define	REG_TWISTAT	(volatile U16*)0x80022004
#define REG_TWIADR	(volatile U8*)0x80022008
#define REG_TWIDATA	(volatile U8*)0x8002200c
#define REG_TWIBR0	(volatile U8*)0x80022010
#define REG_TWIBR1	(volatile U8*)0x80022014


#define ERR_TWI_ACK_TIME_OVER	0xff// ACK Time Over
#define ERR_TWI_TC_TIME_OVER	0xfe// Transfrer complete Time over
#define ERR_TWI_RX_TIME_OVER	0xfd// RX full check time over

U8 TwiCompliteCheck(U32 waite_time)
{// 0->OK, 1->Error
	U8 state =1,aa;
	
	while(waite_time){
		aa= *REG_TWISTAT;
		if(aa & 0x80){
			state =0;
			break;
		}
		waite_time--;
	}
	return state;
}
U8 TwiBusBusyCheck(U32 waite_time)
{// 0->OK, 1->Error
	U8 state =1,aa;
	
	while(waite_time){
		aa= *REG_TWISTAT;
		if((aa & 0x20)==0){
			state =0;
			break;
		}
		waite_time--;
	}
	return state;
}
U8 TwiStop(void)
{
	volatile U32 dtime;
	*REG_TWICON =0;
	*REG_TWIDATA = 0xfe;
	for(dtime=0;dtime<1000; dtime++);
	*REG_TWICON = TWICONR_TWIEN | TWICONR_MASTER | TWICONR_TWITX | TWICONR_NOACK ;
	for(dtime=0;dtime<1000; dtime++);
	
	return 0;
}

char AK4183Write(U8 twi_addr, U8 twi_data)
{
	if(TwiBusBusyCheck(1000)){
		TwiStop();
		if(TwiBusBusyCheck(1000)){
			TwiStop();
			return -1;
		}
	}
	*REG_TWIDATA = twi_addr;// Divice ID Write
	*REG_TWICON = TWICONR_TWIEN | TWICONR_MASTER | TWICONR_TWITX;// start condiction
	if(TwiCompliteCheck(1000)){
		TwiStop();
		return -1;
	}

	*REG_TWIDATA = twi_data;// data write
	if(TwiCompliteCheck(1000)){
		TwiStop();
		return -1;
	}
	
	// stop condiction
	*REG_TWICON &= ~(TWICONR_MASTER);
	if(TwiBusBusyCheck(1000)){
		TwiStop();
		return -1;
	}

	return 0;
}

int AK4183Read(U8 twi_addr)
{
	U16 twi_data=0;

	if(TwiBusBusyCheck(1000)){
		TwiStop();
		if(TwiBusBusyCheck(1000)){
			TwiStop();
			return -1;
		}
	}

	*REG_TWIDATA = twi_addr|0x01;
	*REG_TWICON = TWICONR_TWIEN | TWICONR_MASTER;
	
	if(TwiCompliteCheck(1000)){
		TwiStop();
		return -1;
	}

	twi_data=*REG_TWIDATA;
	if(TwiCompliteCheck(1000)){
		TwiStop();
		return -1;
	}
	
	twi_data=*REG_TWIDATA;
	if(TwiCompliteCheck(1000)){
		TwiStop();
		return -1;
	}
	
	*REG_TWICON |= TWICONR_NOACK ;
	twi_data<<=8;
	twi_data|=*REG_TWIDATA;
	if(TwiCompliteCheck(1000)){
		TwiStop();
		return -1;
	}

	if(TwiCompliteCheck(1000)){
		TwiStop();
		return -1;
	}
	return twi_data>>=4;
}
U16 TouchReadX(void)
{
	if(AK4183Write(AK4183_ADDR,CMD_Driver_X)==0)
		return AK4183Read(AK4183_ADDR);
	else return -1;
}

U16 TouchReadY(void)
{
	if(AK4183Write(AK4183_ADDR,CMD_Driver_Y)==0)
		return AK4183Read(AK4183_ADDR);
	else return -1;
}
void TouchSleep(void)
{
	if(AK4183Write(AK4183_ADDR,CMD_SLEEP)==0)
		debugstring("AK4183 Sleep Mode\r\n");
}
