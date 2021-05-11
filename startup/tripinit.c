/******************************************************************************
	Copyright (C) 2014	Advanced Digital Chips Inc. 
						http://www.adc.co.kr
						
	Author : Software Team.
******************************************************************************/

#include "cantus.h"
#include "tripinit.h"

#define JTAG_USE

inline void USB_Enable(void)
{	
	*R_PAF6 &= ~PAF6_P64;
	*R_PAF6 |= PAF6_P64;
	
	*R_P6oDIR = TRIP_F_USB_DP;
	*R_P6oLOW = TRIP_F_USB_DP;
	delayms(1);
	
	*R_P6oHIGH = TRIP_F_USB_DP;
	delayms(1);
}

inline void USB_Disable(void)
{
	*R_PAF6 &= ~PAF6_P64;
	*R_PAF6 |= PAF6_P64;
	
	*R_P6oDIR = TRIP_F_USB_DP;
	*R_P6oLOW = TRIP_F_USB_DP;
}

void TRIPinit(void)
{
	*R_PAF0 = PAF0_P00	| PAF0_P01	| PAF0_P02	| PAF0_P03
			| PAF0_P04	| PAF0_P05	| PAF0_P06	| PAF0_P07;
		
	*R_PAF1 = PAF1_P10	| PAF1_P11	| PAF1_P12	| PAF1_P13
			| PAF1_P14	| PAF1_P15	| PAF1_UTX7	| PAF1_URX7;
		
	*R_PAF2 = PAF2_P20	| PAF2_P21	| PAF2_P22	| PAF2_P23
			| PAF2_P24	| PAF2_P25	| PAF2_P26	| PAF2_P27;
		
	*R_PAF3 = PAF3_P30	| PAF3_P31	| PAF3_P32	| PAF3_P33
			| PAF3_P34	| PAF3_P35	| PAF3_P36	| PAF3_P37;
		
#ifdef USE_W5500
	*R_PAF4 = PAF4_P40	| PAF4_P41	| PAF4_SPI_MOSI	| PAF4_SPI_MISO
			| PAF4_SPI_SCK	| PAF4_P45	| PAF4_P46	| PAF4_P47;
#else
	*R_PAF4 = PAF4_P40	| PAF4_P41	| PAF4_P42	| PAF4_P43
			| PAF4_P44	| PAF4_P45	| PAF4_P46	| PAF4_P47;
#endif
		
	*R_PAF5 = PAF5_P50	| PAF5_P51	| PAF5_P52	| PAF5_P53
			| PAF5_P54	| PAF5_P55	| PAF5_P56	| PAF5_P57;
	
#ifdef JTAG_USE
	*R_PAF6 = PAF6_TRST	| PAF6_TCK | PAF6_TMS	| PAF6_TDI
			| PAF6_P64;
#else
	*R_PAF6 = PAF6_TRST	| PAF6_P61	| PAF6_TMS	| PAF6_TDI
			| PAF6_P64;
#endif
	
#ifdef USE_W5500
	*R_P2oDIR = TRIP_F_W5500_RST | TRIP_F_W5500_NCS;
	*R_P2oLOW = TRIP_F_W5500_RST;
	*R_P2oHIGH = TRIP_F_W5500_NCS;
#endif
		
	// LED
	*R_P5oDIR |= TRIP_F_LED;
	*R_P5oHIGH |= TRIP_F_LED;

	// KEY
	*R_P6iDIR = TRIP_F_KEY;
	
#ifndef JTAG_USE
	// USB
	USB_Disable();
#endif
}
