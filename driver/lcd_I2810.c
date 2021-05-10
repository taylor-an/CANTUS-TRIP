#include "cantus.h"
#include "driver/lcd.h"

#define SUPPORT_CRITICAL_SECTION

#ifndef  SUPPORT_CRITICAL_SECTION
#define CRITICAL_ENTER();	
#define CRITICAL_EXIT();
#endif

#define MAKE_RGB565(r,g,b)	(U16)((((U16)r&0xf8)<<8)|(((U16)g&0xfc)<<3)|(((U16)b&0xf8)>>3))  ///< make RGB565 from r,g,b

// void LCDDataWrite(unsigned char reg, unsigned short wdata_rcv)
void LCDDataWrite(unsigned short reg, unsigned short wdata)
{
    *(volatile unsigned char *)(LCD_BASE_ADDR)=0;
    *(volatile unsigned char *)(LCD_BASE_ADDR)=(reg & 0xff);
	
	*(volatile unsigned char *)(LCD_BASE_ADDR|1) = ((wdata >> 8) & 0xff);
    *(volatile unsigned char *)(LCD_BASE_ADDR|1) = (wdata & 0xff);
}

void LCDPowerOnSet(void)
{
	// Start Initial Sequence
    LCDDataWrite(0x0001,0x0100);
    LCDDataWrite(0x0002,0x0700);
    LCDDataWrite(0x0003,0x1030);
    LCDDataWrite(0x0004,0x0000);
    LCDDataWrite(0x0008,0x0207);
    LCDDataWrite(0x0009,0x0000); //
    LCDDataWrite(0x000A,0x0000); //
    LCDDataWrite(0x000C,0x0000); //
    LCDDataWrite(0x000D,0x0000); //
    LCDDataWrite(0x000F,0x0000); //

    // Power On sequence
    LCDDataWrite(0x0010,0x0000);
    LCDDataWrite(0x0011,0x0007);
    LCDDataWrite(0x0012,0x0000);
    LCDDataWrite(0x0013,0x0000);
	delayms(200); //
	LCDDataWrite(0x0010,0x1290);
	LCDDataWrite(0x0011,0x0227);
	delayms(50); //Delay 50 ms
	LCDDataWrite(0x0012,0x001A);
	delayms(50); //Delay 50 ms
	LCDDataWrite(0x0013,0x1800);
	LCDDataWrite(0x0029,0x0028);
	LCDDataWrite(0x002B,0x000C);
	
	delayms(50); //Delay 50 ms
	LCDDataWrite(0x0020,0x0000);
	LCDDataWrite(0x0021,0x0000);
	
	// Adjust the Gamma Curve
	// // **************************** ori CPT 2.8"
	LCDDataWrite(0x0030,0x0000);
	LCDDataWrite(0x0031,0x0305);
	LCDDataWrite(0x0032,0x0003);
	LCDDataWrite(0x0035,0x0304);
	LCDDataWrite(0x0036,0x000F);
	LCDDataWrite(0x0037,0x0407);
	LCDDataWrite(0x0038,0x0204);
	LCDDataWrite(0x0039,0x0707);
	LCDDataWrite(0x003C,0x0403);
	LCDDataWrite(0x003D,0x1604);

	// Set GRAM area
	LCDDataWrite(0x0050,0x0000);
	LCDDataWrite(0x0051,0x00EF);
	LCDDataWrite(0x0052,0x0000);
	LCDDataWrite(0x0053,0x013F);
	LCDDataWrite(0x0060,0xA700);
	LCDDataWrite(0x0061,0x0001);
	LCDDataWrite(0x006A,0x0000);
	
	// // Partial Display Control
	LCDDataWrite(0x0080,0x0000);
	LCDDataWrite(0x0081,0x0000);
	LCDDataWrite(0x0082,0x0000);
	LCDDataWrite(0x0083,0x0000);
	LCDDataWrite(0x0084,0x0000);
	LCDDataWrite(0x0085,0x0000);
	
	// Panel Control
	LCDDataWrite(0x0090,0x0010);
	LCDDataWrite(0x0092,0x0600);
	
	LCDDataWrite(0x0007,0x0133);

}

unsigned char LCDReadReg(unsigned int index)
{
    unsigned char rdata;

    *(volatile unsigned char *)(LCD_BASE_ADDR)=index;
    rdata=*(volatile unsigned char *)(LCD_BASE_ADDR|1);
    return rdata;
}

void LCDRamSet(unsigned short start_x, unsigned short start_y,unsigned short end_x,unsigned short end_y)
{
	LCDDataWrite(0x0050,start_x);
	LCDDataWrite(0x0051,end_x);
	LCDDataWrite(0x0052,start_y);	
	LCDDataWrite(0x0053,end_y);
	
	LCDDataWrite(0x0020, (unsigned char)start_x );
	LCDDataWrite(0x0021,  start_y );	
	
	*(volatile unsigned char *)(LCD_BASE_ADDR)=0x0;
	*(volatile unsigned char *)(LCD_BASE_ADDR)=0x22;
}

void LCDcls(U8 r, U8 g, U8 b)
{
    int x,y;
	CRITICAL_ENTER();
	LCDRamSet(0,0,239,319);
    volatile U8* frame=(U8*)(LCD_BASE_ADDR|1);
	U16 rgb565 = MAKE_RGB565(r,g,b);
    for (y=0;y<320;y++)
    {
        for (x=0;x<240;x++)
        {
            *frame=rgb565>>8;
            *frame=rgb565;
        }
    }
	CRITICAL_EXIT();
}

void drawbox(int sx, int sy, int endx, int endy,U8 r , U8 g, U8 b)
{
	int x,y;
	CRITICAL_ENTER();
	LCDRamSet(sx,sy,endx-1,endy-1);
    volatile U8* frame=(U8*)(LCD_BASE_ADDR|1);
    U16 rgb565 = MAKE_RGB565(r,g,b);
    for (y=0;y<endy ;y++)
    {
        for (x=0;x<endx ;x++)
        {
            *frame=rgb565 >>8;
            *frame=rgb565;
        }
    }
	CRITICAL_EXIT();
}


void LCDInit()
{
    LCDPowerOnSet();
}


void LCDputpixel(int x, int y, U8 r, U8 g, U8 b)
{
	CRITICAL_ENTER();
	LCDRamSet(x,y,x,y);
	U16 rgb565 = MAKE_RGB565(r,g,b);
	*(volatile unsigned char *)(LCD_BASE_ADDR|1)= rgb565>>8;
    *(volatile unsigned char *)(LCD_BASE_ADDR|1)= rgb565;
	
	
	CRITICAL_EXIT();
}

