#include "cantus.h"
#include "driver/lcd.h"

#define SUPPORT_CRITICAL_SECTION

#ifndef  SUPPORT_CRITICAL_SECTION
#define CRITICAL_ENTER();	
#define CRITICAL_EXIT();
#endif

void LCDDataWrite(unsigned char reg, unsigned char wdata)
{

    *(volatile unsigned char *)(LCD_BASE_ADDR)=reg;
    *(volatile unsigned char *)(LCD_BASE_ADDR|1)=wdata;
}
void LCDPowerOnSet(void)
{
    // GAMMA Setting
    LCDDataWrite(0x0046,0x00B7);
    LCDDataWrite(0x0047,0x0045);
    LCDDataWrite(0x0048,0x0004);
    LCDDataWrite(0x0049,0x0025);
    LCDDataWrite(0x004A,0x0000);
    LCDDataWrite(0x004B,0x0006);
    LCDDataWrite(0x004C,0x0001);
    LCDDataWrite(0x004D,0x0044);
    LCDDataWrite(0x004E,0x0008); //
    LCDDataWrite(0x004F,0x001B); //
    LCDDataWrite(0x0050,0x0001); //
    LCDDataWrite(0x0051,0x0006); //

    // Window Setting
    LCDDataWrite(0x02,0x00);
    LCDDataWrite(0x03,0x00);
    LCDDataWrite(0x04,0x00);
    LCDDataWrite(0x05,0xEF);
    LCDDataWrite(0x06,0x00);
    LCDDataWrite(0x07,0x00);
    LCDDataWrite(0x08,0x01);
    LCDDataWrite(0x09,0x3F);

    // Display Setting
    delayms(10);
    LCDDataWrite(0x01,0x06);

    LCDDataWrite(0x16,0xC8);
    LCDDataWrite(0x23,0x95);
    LCDDataWrite(0x24,0x95);
    LCDDataWrite(0x25,0xFF);
    LCDDataWrite(0x27,0x02);
    LCDDataWrite(0x28,0x02);
    LCDDataWrite(0x29,0x02);
    LCDDataWrite(0x2A,0x02);
    LCDDataWrite(0x2C,0x02);
    LCDDataWrite(0x2D,0x02);

    LCDDataWrite(0x3A,0x01);
    LCDDataWrite(0x3B,0x01);
    LCDDataWrite(0x3C,0xF0);
    LCDDataWrite(0x3D,0x00);
    delayms(20);




    LCDDataWrite(0x35,0x38);
    LCDDataWrite(0x36,0x78);
    LCDDataWrite(0x3E,0x38);
    LCDDataWrite(0x40,0x0F);
    LCDDataWrite(0x41,0xF0);

    LCDDataWrite(0x38,0x00);

// Power Setting

    LCDDataWrite(0x19,0x49);
    LCDDataWrite(0x93,0x0A);

    delayms(10);

    LCDDataWrite(0x20,0x50);
    LCDDataWrite(0x1D,0x03);
    LCDDataWrite(0x1E,0x00);

    LCDDataWrite(0x1F,0x09);
    LCDDataWrite(0x44,0x5A);
    LCDDataWrite(0x45,0x10);

    delayms(10);

    LCDDataWrite(0x1C,0x04);

    delayms(20);

    LCDDataWrite(0x43,0x80);

    delayms(5);

    LCDDataWrite(0x1B,0x0A);

    delayms(40);

    LCDDataWrite(0x1B,0x12);

    delayms(40);

//Display On Setting

    LCDDataWrite(0x90,0x7F);
    LCDDataWrite(0x26,0x04);

    delayms(40);

    LCDDataWrite(0x26,0x24);
    LCDDataWrite(0x26,0x2C);

    delayms(40);
    LCDDataWrite(0x70,0x08);
    LCDDataWrite(0x26,0x3C);
    LCDDataWrite(0x57,0x02);
    LCDDataWrite(0x55,0x00);
    LCDDataWrite(0x57,0x00);

    // LCDDataWrite(0x72,0x1);
    // LCDDataWrite(0x40,0x0f);
    // LCDDataWrite(0x41,0xf0);

    // LCDDataWrite(0x57,0x02);
    // LCDDataWrite(0x56,0x84);
    // LCDDataWrite(0x57,0x00);
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
    LCDDataWrite(0x02,(unsigned char)(start_x>>8));
    LCDDataWrite(0x03,(unsigned char)start_x);
    LCDDataWrite(0x04,(unsigned char)(end_x>>8));
    LCDDataWrite(0x05,(unsigned char)end_x);
    LCDDataWrite(0x06,(unsigned char)(start_y>>8));
    LCDDataWrite(0x07,(unsigned char)(start_y));
    LCDDataWrite(0x08,(unsigned char)(end_y>>8));
    LCDDataWrite(0x09,(unsigned char)end_y);
    LCDDataWrite(0x16,0xC8);
    *(volatile unsigned char *)(LCD_BASE_ADDR)=0x22;
    
}
void LCDDrawBlock(unsigned short start_x,  unsigned short start_y,unsigned short end_x, unsigned short end_y, unsigned int rgb)
{
    int x,y;

	LCDRamSet(start_x,start_y,end_x,end_y);
    for (y=start_y; y<end_y; y++)
    {
        for (x=start_x; x<end_x; x++)
        {
            *(volatile unsigned char *)(LCD_BASE_ADDR|1)=(unsigned char)rgb;//red
            *(volatile unsigned char *)(LCD_BASE_ADDR|1)=(unsigned char)(rgb>>8);//green
            *(volatile unsigned char *)(LCD_BASE_ADDR|1)=(unsigned char)(rgb>>16);//blue
        }
    }
}

void LCDcls(U8 r, U8 g, U8 b)
{
    int x,y;
	CRITICAL_ENTER();
	LCDRamSet(0,0,239,319);
    volatile U8* frame=(U8*)(LCD_BASE_ADDR|1);
    for (y=0;y<320;y++)
    {
        for (x=0;x<240;x++)
        {
            *frame=r;
            *frame=g;
            *frame=b;
        }
    }
	CRITICAL_EXIT();
}
void drawbox(int sx, int sy, int endx, int endy,U8 r , U8 g, U8 b)
{
	int x,y;
	CRITICAL_ENTER();
	LCDRamSet(sx,sy,endx,endy);
    volatile U8* frame=(U8*)(LCD_BASE_ADDR|1);
    for (y=0;y<320;y++)
    {
        for (x=0;x<240;x++)
        {
            *frame=r;
            *frame=g;
            *frame=b;
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
	*(volatile unsigned char *)(LCD_BASE_ADDR|1)=r;
    *(volatile unsigned char *)(LCD_BASE_ADDR|1)=g;
    *(volatile unsigned char *)(LCD_BASE_ADDR|1)=b;
	CRITICAL_EXIT();
}


