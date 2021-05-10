
#pragma once

/*****************************************
* PLL register
*****************************************/
#define R_PLLCTRL	((volatile U32*)0x80020400)
#define R_CKR		((volatile U32*)0x80020404)
#define R_PWRCTRL	((volatile U32*)0x80020408)
#define R_WKCTRL	((volatile U32*)0x8002040c)
#define R_HCLKGATE	((volatile U32*)0x80020410)
#define R_PCLKGATE	((volatile U32*)0x80020414)
#define R_SCLKGATE	((volatile U32*)0x80020418)
#define R_RSTCTRL	((volatile U32*)0x8002041c)
#define R_RSTSTAT	R_RSTCTRL
#define R_HALTCTRL	((volatile U32*)0x80020420)
#define R_PMCTRLEN		((volatile U32*)0x80020424)
#define R_LDOCTRL	((volatile U32*)0x80020428)

#define PLLCTRL_ENABLE	(1<<16)
#define CKR_MCS_PLL		(1<<1)
#define CKR_MCS_OSC		0
#define CKR_PCS_AHB		(1<<1)
#define CKR_PCS_AHB_2	0
#define CKR_UCS_MAINCLOCK_2	0
#define CKR_UCS_MAINCLOCK	(1<<2)
#define CKR_MCLK_XIN	0
#define CKR_MCLK_CAP	(1<<3)

#define PWRCTRL_LDO_EN	(1<<0)
#define PWRCTRL_MOSCEN	(1<<1)
#define PWRCTRL_UPDATEBIT	(1<<2)

#define WKCTRL_RTC_IRQ	(1<<0)
#define WKCTRL_EIRQ_EN	(1<<1)
#define WKCTRL_EIRQ_FALLEDGE	(1<<2)
#define WKCTRL_UPDATEBIT	(1<<2)

#define HCLK_GATE_AHB_EN	(1<<0)
#define HCLK_GATE_FLASH_EN	(1<<1)
#define HCLK_GATE_SRAM_EN	(1<<2)
#define HCLK_GATE_IRAM_EN	(1<<3)
#define HCLK_GATE_DMA_EN	(1<<4)
#define HCLK_GATE_NAND_EN	(1<<5)
#define HCLK_GATE_SDHC_EN	(1<<6)

#define PCLK_GATE_APB_EN	(1<<0)
#define PCLK_GATE_INTCTRL_EN	(1<<1)
#define PCLK_GATE_WATCHDOG_EN	(1<<2)
#define PCLK_GATE_TIMER_EN	(1<<3)
#define PCLK_GATE_UART_EN	(1<<4)
#define PCLK_GATE_SPI_EN	(1<<5)
#define PCLK_GATE_TWI_EN	(1<<6)
#define PCLK_GATE_GPIO_EN	(1<<7)
#define PCLK_GATE_VOICE_EN	(1<<8)
#define PCLK_GATE_I2S_EN	(1<<9)
#define PCLK_GATE_USB_EN	(1<<10)
#define PCLK_GATE_KEYSCAN_EN	(1<<11)
#define PCLK_GATE_RTC_EN	(1<<12)
#define PCLK_GATE_PAF_EN	(1<<13)

#define SCLK_GATE_USB48_EN	(1<<0)
#define SCLK_GATE_USB12_EN	(1<<1)
#define SCLK_GATE_MCLK_EN	(1<<2)

#define RSTCTRL_RESET	1

#define RSTSTAT_WATCHDOG_BIT	(1<<0)
#define RSTSTAT_SOFTWARE_BIT	(1<<1)
#define RSTSTAT_HALT0_BIT		(1<<2)
#define RSTSTAT_POR_BIT			(1<<3)

#define HALTCTRL_HALT0	(0x10)
#define HALTCTRL_HALT1	(0x11)
#define HALTCTRL_HALT2	(0x12)

#define PMCTRL_PLLCTRL_EN	(1<<0)
#define PMCTRL_CLKSEL_EN	(1<<1)
#define PMCTRL_PWRCTRL_EN	(1<<2)
#define PMCTRL_WKCTRL_EN	(1<<3)
#define PMCTRL_HCLK_GATE_EN	(1<<4)
#define PMCTRL_PCLK_GATE_EN	(1<<5)
#define PMCTRL_SCLK_GATE_EN	(1<<6)
#define PMCTRL_RSTCTRL_EN	(1<<7)
#define PMCTRL_HALTCTRL_EN	(1<<8)
#define PMCTRL_LDOCTRL_EN	(1<<9)


U32 GetAHBclock();
U32 GetAPBclock();

