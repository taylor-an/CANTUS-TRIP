
#pragma once

#define R_PAF0 ((volatile unsigned int*)0x80020020)
#define R_PAF1 ((volatile unsigned int*)0x80020024)
#define R_PAF2 ((volatile unsigned int*)0x80020028)
#define R_PAF3 ((volatile unsigned int*)0x8002002c)
#define R_PAF4 ((volatile unsigned int*)0x80020030)
#define R_PAF5 ((volatile unsigned int*)0x80020034)
#define R_PAF6 ((volatile unsigned int*)0x80020038)

/* Define PIN Functions of P0AF */
#define PAF0_D0A0A8		(1 << 1)
#define PAF0_P00			(3 << 0)

#define PAF0_D1A1A9		(1 << 3)
#define PAF0_P01			(3 << 2)

#define PAF0_D2A2A10		(1 << 5)
#define PAF0_P02			(3 << 4)

#define PAF0_D3A3A11		(1 << 7)
#define PAF0_P03			(3 << 6)

#define PAF0_D4A4A12		(1 << 9)
#define PAF0_P04			(3 << 8)

#define PAF0_D5A5A13		(1 << 11)
#define PAF0_P05			(3 << 10)

#define PAF0_D6A6A14		(1 << 13)
#define PAF0_P06			(3 << 12)

#define PAF0_D7A7A15		(1 << 15)
#define PAF0_P07			(3 << 14)

/* Define PIN Functions of P1AF */
#define PAF1_UTX4		0
#define PAF1_KEYO0		(1 << 0)
#define PAF1_D8A8		(1 << 1)
#define PAF1_P10			(3 << 0)

#define PAF1_URX4		0
#define PAF1_KEYI0		(1 << 2)
#define PAF1_D9A9		(1 << 3)
#define PAF1_P11			(3 << 2)

#define PAF1_UTX5		0
#define PAF1_KEYO1		(1 << 4)
#define PAF1_D10A10		(1 << 5)
#define PAF1_P12			(3 << 4)

#define PAF1_URX5		0
#define PAF1_KEYI1		(1 << 6)
#define PAF1_D11A11		(1 << 7)
#define PAF1_P13			(3 << 6)

#define PAF1_UTX6		0
#define PAF1_KEYO2		(1 << 8)
#define PAF1_D12A12		(1 << 9)
#define PAF1_P14			(3 << 8)

#define PAF1_URX6		0
#define PAF1_KEYI2		(1 << 10)
#define PAF1_D13A13		(1 << 11)
#define PAF1_P15			(3 << 10)

#define PAF1_UTX7		0
#define PAF1_KEYO3		(1 << 12)
#define PAF1_D14A14		(1 << 13)
#define PAF1_P16			(3 << 12)

#define PAF1_URX7		0
#define PAF1_KEYI3		(1 << 14)
#define PAF1_D15A15		(1 << 15)
#define PAF1_P17			(3 << 14)

/* Define PIN Functions of P2AF */
#define PAF2_ALE0		(1 << 1)
#define PAF2_P20			(3 << 0)

#define PAF2_ALE1		(1 << 3)
#define PAF2_P21			(3 << 2)

#define PAF2_nRE			(1 << 5)
#define PAF2_P22			(3 << 4)

#define PAF2_nWE			(1 << 7)
#define PAF2_P23			(3 << 6)

#define PAF2_nCS0		(1 << 9)
#define PAF2_P24			(3 << 8)

#define PAF2_nCS1		(1 << 11)
#define PAF2_P25			(3 << 10)

#define PAF2_nCS2		(1 << 13)
#define PAF2_P26			(3 << 12)

#define PAF2_SPI_nSS		(1 << 14)
#define PAF2_nCS3		(1 << 15)
#define PAF2_P27			(3 << 14)

/* Define PIN Functions of P3AF */
#define PAF3_EIRQ0		0
#define PAF3_nWAIT		(1 << 1)
#define PAF3_P30			(3 << 0)

#define PAF3_EIRQ1		0
#define PAF3_nBE1		(1 << 3)
#define PAF3_P31			(3 << 2)

#define PAF3_NDFL_nWE	(1 << 5)
#define PAF3_P32			(3 << 4)

#define PAF3_NDFL_ALE	(1 << 7)
#define PAF3_P33			(3 << 6)

#define PAF3_NDFL_CLE	(1 << 9)
#define PAF3_P34			(3 << 8)

#define PAF3_NDFL_nCS	(1 << 11)
#define PAF3_P35			(3 << 10)

#define PAF3_NDFL_nRE	(1 << 13)
#define PAF3_P36			(3 << 12)

#define PAF3_nNMI		0
#define PAF3_NDFL_nBUSY	(1 << 15)
#define PAF3_P37			(3 << 14)

/* Define PIN Functions of P4AF */
#define PAF4_UTX0		0
#define PAF4_I2S_MOD		(1 << 1)
#define PAF4_P40			(3 << 0)

#define PAF4_URX0		0
#define PAF4_I2S_SDI		(1 << 3)
#define PAF4_P41			(3 << 2)

#define PAF4_UTX1		0
#define PAF4_SPI_MOSI	(1<<4)
#define PAF4_SDCD_D3		(1 << 5)
#define PAF4_P42			(3 << 4)

#define PAF4_URX1		0
#define PAF4_SPI_MISO	(1<<6)
#define PAF4_SDCD_D2		(1 << 7)
#define PAF4_P43			(3 << 6)

#define PAF4_UTX2		0
#define PAF4_SPI_SCK		(1<<8)
#define PAF4_SDCD_CLK	(1 << 9)
#define PAF4_P44			(3 << 8)

#define PAF4_URX2		0
#define PAF4_SDCD_CMD	(1 << 11)
#define PAF4_P45			(3 << 10)

#define PAF4_UTX3		0
#define PAF4_TWI_SCL		(1 << 13)
#define PAF4_P46			(3 << 12)

#define PAF4_URX3		0
#define PAF4_TWI_SDA		(1 << 15)
#define PAF4_P47			(3 << 14)

/* Define PIN Functions of P5AF */
#define PAF5_TMO0		0
#define PAF5_CAP0		(1 << 0)
#define PAF5_SDCD_D1		(1 << 1)
#define PAF5_P50			(3 << 0)

#define PAF5_TMO1		0
#define PAF5_CAP1		(1 << 2)
#define PAF5_SDCD_D0		(1 << 3)
#define PAF5_P51			(3 << 2)

#define PAF5_TMO2		0
#define PAF5_CAP2		(1 << 4)
#define PAF5_I2S_LRCK	(1 << 5)
#define PAF5_P52			(3 << 4)

#define PAF5_TMO3		0
#define PAF5_CAP3		(1 << 6)
#define PAF5_I2S_SCLK	(1 << 7)
#define PAF5_P53			(3 << 6)

#define PAF5_TMO4		0
#define PAF5_CAP4		(1 << 8)
#define PAF5_I2S_MCLK	(1 << 9)
#define PAF5_P54			(3 << 8)

#define PAF5_TMO5		0
#define PAF5_CAP5		(1 << 10)
#define PAF5_A16			(1 << 11)
#define PAF5_P55			(3 << 10)

#define PAF5_TMO6		0
#define PAF5_CAP6		(1 << 12)
#define PAF5_A17			(1 << 13)
#define PAF5_P56			(3 << 12)

#define PAF5_TMO7		0
#define PAF5_CAP7		(1 << 14)
#define PAF5_A18			(1 << 15)
#define PAF5_P57			(3 << 14)

/* Define PIN Functions of P6AF */
#define PAF6_TRST		0
#define PAF6_P60			(3 << 0)

#define PAF6_TCK			0
#define PAF6_P61			(3 << 2)

#define PAF6_TMS			0
#define PAF6_P62			(3 << 4)

#define PAF6_TDI			0
#define PAF6_P63			(3 << 6)

#define PAF6_POR			0
#define PAF6_P64			(3 << 8)
