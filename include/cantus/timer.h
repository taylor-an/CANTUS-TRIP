#pragma once

#define R_TP0CON	((volatile unsigned int*)0x80021000)
#define R_TP1CON	((volatile unsigned int*)0x80021020)
#define R_TP2CON	((volatile unsigned int*)0x80021040)
#define R_TP3CON	((volatile unsigned int*)0x80021060)
#define R_TP4CON	((volatile unsigned int*)0x80021080)
#define R_TP5CON	((volatile unsigned int*)0x800210a0)
#define R_TP6CON	((volatile unsigned int*)0x800210c0)
#define R_TP7CON	((volatile unsigned int*)0x800210e0)

#define R_TM0CON	((volatile unsigned int*)0x80021004)
#define R_TM1CON	((volatile unsigned int*)0x80021024)
#define R_TM2CON	((volatile unsigned int*)0x80021044)
#define R_TM3CON	((volatile unsigned int*)0x80021064)
#define R_TM4CON	((volatile unsigned int*)0x80021084)
#define R_TM5CON	((volatile unsigned int*)0x800210a4)
#define R_TM6CON	((volatile unsigned int*)0x800210c4)
#define R_TM7CON	((volatile unsigned int*)0x800210e4)

#define R_TM0CNT	((volatile unsigned int*)0x80021008)
#define R_TM1CNT	((volatile unsigned int*)0x80021028)
#define R_TM2CNT	((volatile unsigned int*)0x80021048)
#define R_TM3CNT	((volatile unsigned int*)0x80021068)
#define R_TM4CNT	((volatile unsigned int*)0x80021088)
#define R_TM5CNT	((volatile unsigned int*)0x800210a8)
#define R_TM6CNT	((volatile unsigned int*)0x800210c8)
#define R_TM7CNT	((volatile unsigned int*)0x800210e8)

#define R_TM0DUT	((volatile unsigned int*)0x8002100c)
#define R_TM1DUT	((volatile unsigned int*)0x8002102c)
#define R_TM2DUT	((volatile unsigned int*)0x8002104c)
#define R_TM3DUT	((volatile unsigned int*)0x8002106c)
#define R_TM4DUT	((volatile unsigned int*)0x8002108c)
#define R_TM5DUT	((volatile unsigned int*)0x800210ac)
#define R_TM6DUT	((volatile unsigned int*)0x800210cc)
#define R_TM7DUT	((volatile unsigned int*)0x800210ec)

#define R_TM0PUL	((volatile unsigned int*)0x80021010)
#define R_TM1PUL	((volatile unsigned int*)0x80021030)
#define R_TM2PUL	((volatile unsigned int*)0x80021050)
#define R_TM3PUL	((volatile unsigned int*)0x80021070)
#define R_TM4PUL	((volatile unsigned int*)0x80021090)
#define R_TM5PUL	((volatile unsigned int*)0x800210b0)
#define R_TM6PUL	((volatile unsigned int*)0x800210d0)
#define R_TM7PUL	((volatile unsigned int*)0x800210f0)

#define TMCTRL_TMOD_TIMER	0<<14
#define TMCTRL_TMOD_PWM		1<<14
#define TMCTRL_TMOD_CAPTURE	2<<14

#define TMCTRL_CAPMOD_LOWHIGH	0<<8
#define TMCTRL_CAPMOD_LOW		2<<8
#define TMCTRL_CAPMOD_HIGH		3<<8
#define TMCTRL_CAPMOD_FALLING	4<<8
#define TMCTRL_CAPMOD_RISING	6<<8

#define TMCTRL_PFSEL_2		0<<1
#define TMCTRL_PFSEL_8		1<<1
#define TMCTRL_PFSEL_32		2<<1
#define TMCTRL_PFSEL_128	3<<1
#define TMCTRL_PFSEL_512	4<<1
#define TMCTRL_PFSEL_2048	5<<1
#define TMCTRL_PFSEL_8192	6<<1
#define TMCTRL_PFSEL_32768	7<<1

#define TMCTRL_TMEN	1<<0

BOOL settimer(int nCh,U32 ms);
BOOL settimer_us(int nCh,U32 us);
BOOL stoptimer(int nCh);
U32 gettimermaxms();
void delayms(U32 ms);
