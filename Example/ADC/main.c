#include "cantus.h"
#include "tripinit.h"

void evmboardinit();

int main()
{
	dcache_invalidate_way();
	TRIPinit();
	InitInterrupt();
	
	UartConfig(TRIP_UART_DEBUG, 115200, DATABITS_8, STOPBITS_1, UART_PARNONE);
	debugstring("================================================\r\n");
	debugprintf("Analog Digital Converter. System Clock(%dMhz)\r\n", GetAHBclock()/1000000);
	debugstring("================================================\r\n");

	*R_PMCTRLEN |= PMCTRL_SCLK_GATE_EN;
	*R_SCLKGATE |= SCLK_GATE_MCLK_EN;
	*R_CKR &= (~CKR_MCLK_CAP);
	*R_PMCTRLEN=0;
	
	ADC_channel(1);
	delayms(1000);	// First power on
	U32 temp = *R_VOICEADC;
	
	/*
		Analog Virtual Ground 1.65 V
	
		ADC Input Range : 0.65 V ~ 2.65 V	
	*/
	
	while(1)
	{
		debugstring("Channel 1 \r\n");
		ADC_channel(1);
		debugprintf("Result = %08x %04d \r\n",*R_VOICEADC^0x2000,*R_VOICEADC^0x2000);
		
		debugstring("Channel 2 \r\n");
		ADC_channel(2);
		debugprintf("Result = %08x %04d \r\n",*R_VOICEADC^0x2000,*R_VOICEADC^0x2000);
		
		debugstring("Channel 3 \r\n");
		ADC_channel(3);
		debugprintf("Result = %08x %04d \r\n",*R_VOICEADC^0x2000,*R_VOICEADC^0x2000);
		
		delayms(500);
		debugstring("\r\n\r\n");		
	}
}
