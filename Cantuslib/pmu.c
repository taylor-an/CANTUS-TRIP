
#include "cantus_lib_config.h"
#include "cantus.h"

static U32 pllclock()
{
    U32 N,M,P;
	U32 clock;
	
    M = (*R_PLLCTRL)&0x3f;
    P = ((*R_PLLCTRL)&0xc0)>>6;
    N = ((*R_PLLCTRL)&0xff00)>>8;
	
    clock = ((N+2)*OSC_CLOCK)/( (M+2) * ( 1<< P ) );
    return clock;
}

U32 GetAHBclock()
{
    U8 pres;
    U32 ahbclock;
    U32 mainclock;//after prescaler
    if ((*R_CKR) & 0x1)//PLL?
    {
        mainclock = pllclock();
    }
    else
        mainclock = OSC_CLOCK;

    //compute prescaler
    pres = ((*R_CKR)&0x70)>>4;
    switch (pres)
    {
    case 1:
        ahbclock = mainclock/2;
        break;
    case 2:
        ahbclock = mainclock/4;
        break;
    case 3:
        ahbclock = mainclock/8;
        break;
    case 4:
        ahbclock = mainclock/16;
        break;
    case 5:
        ahbclock = mainclock/1024;
        break;
    default:
        ahbclock = mainclock;
    }

    return (U32)ahbclock;

}
U32 GetAPBclock()
{
    U32 ahbclock = GetAHBclock();
    if ((*R_CKR)&0x2)
        return ahbclock;
    else
        return ahbclock/2;
}

