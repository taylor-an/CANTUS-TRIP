
#pragma once

#define CRITICAL_ENTER()	asm("set 8");asm("sync") // back up interrupt status
#define CRITICAL_EXIT()		asm("clr 8");asm("sync") // restor interrupt status
#define INTERRUPT_ENABLE	asm("set 13");
#define INTERRUPT_DISABLE	asm("clr 13");
#define NMI_ENABLE			asm("set 14");
#define NMI_DISABLE			asm("clr 14");
#define SETVECTORED			asm("set 12");


/*========================================================
* Interrupt Controller
========================================================*/
#define R_PENDCLR	((volatile unsigned int*)0x80020800)
#define R_EINTMOD	((volatile unsigned int*)0x80020804)
#define R_IINTMOD	((volatile unsigned int*)0x80020808)
#define R_INTPEND	((volatile unsigned int*)0x8002080c)
#define R_INTEN		((volatile unsigned int*)0x80020810)
#define R_INTMASK	((volatile unsigned int*)0x80020814)
#define R_INTMASKSET	R_INTMASK
#define R_INTMASKCLR	((volatile unsigned int*)0x80020818)

#define EINTMOD_INT0_ACTIVE_MASK	(0xfffffff8)
#define EINTMOD_INT0_LOWLEVEL		(0)
#define EINTMOD_INT0_HIGHLEVEL		(1)
#define EINTMOD_INT0_FALLING_EDGE	(2)
#define EINTMOD_INT0_RISING_EDGE	(3)
#define EINTMOD_INT0_ANY_EDGE		(4)

#define EINTMOD_INT1_LOWLEVEL		(0)
#define EINTMOD_INT1_HIGHLEVEL		(1<<4)
#define EINTMOD_INT1_FALLING_EDGE	(2<<4)
#define EINTMOD_INT1_RISING_EDGE	(3<<4)
#define EINTMOD_INT1_ANY_EDGE		(4<<4)

typedef enum 
{
	INTNUM_EIRQ0=0,
	INTNUM_TIMER0,
	INTNUM_I2S,
	INTNUM_UART0,

	INTNUM_EIRQ1,//4
	INTNUM_TIMER1,
	INTNUM_DMA0,
	INTNUM_UART1,

	INTNUM_GPIO0,//8
	INTNUM_TIMER2,
	INTNUM_USB,
	INTNUM_UART2,

	INTNUM_GPIO1,//0xc
	INTNUM_TIMER3,
	INTNUM_SPI,
	INTNUM_UART3,

	INTNUM_GPIO2,//0x10
	INTNUM_TIMER4,
	INTNUM_TWI_RTC,
	INTNUM_UART4_OCR2A,

	INTNUM_GPIO3,//0x14
	INTNUM_TIMER5,
	INTNUM_VOICE,
	INTNUM_UART5_OCR2B,

	INTNUM_GPIO4,//0x18
	INTNUM_TIMER6,
	INTNUM_NFCTRL_SDHC,
	INTNUM_UART6_OCR3A,

	INTNUM_GPIO5,//0x1c
	INTNUM_TIMER7_KEYSCAN,
	INTNUM_DMA1,
	INTNUM_UART7_OCR3B,

	INTNUM_MAX
}INTERRUPT_TYPE;

void InitInterrupt();
BOOL setinterrupt(INTERRUPT_TYPE intnum,void (*fp)());
void EnableInterrupt(INTERRUPT_TYPE num,BOOL b);

