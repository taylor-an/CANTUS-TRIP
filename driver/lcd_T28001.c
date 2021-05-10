#include "cantus.h"
#include "driver/lcd.h"

#define SUPPORT_CRITICAL_SECTION

#ifndef SUPPORT_CRITICAL_SECTION
#define CRITICAL_ENTER();	
#define CRITICAL_EXIT();
#endif

#define MAKE_RGB565(r,g,b)	(U16)((((U16)r&0x1f)<<11)|(((U16)g&0x3f)<<5)|((U16)b&0x1f))  ///< make RGB565 from r,g,b

// LKD-T28001-D2
	
#define SOFTWARE_RESET				0x01
#define READ_DISPLAY_PIXEL_FORMAT   0x0C
#define SLEEP_OUT     				0x11
#define GAMMA_SET     				0x26
#define DISPLAY_OFF     			0x28
#define DISPLAY_ON     				0x29
#define SET_COLUMN_ADDRESS   		0x2A
#define SET_PAGE_ADDRESS   			0x2B
#define WRITE_MEMORY    			0x2C
#define READ_MEMORY     			0x2E
#define MEMORY_ACCESS_CONTROL  		0x36
#define WRITE_MEMORY_CONTINUE  		0x3C
#define READ_MEMORY_CONTINUE  		0x3E
#define PIXEL_FORMAT_SET   			0x3A
#define FRAME_RATE_CONTROL   		0xB1
#define DISPLAY_FUNCTION_CONTROL 	0xB6
#define ENTRY_MODE_SET				0xB7
#define POWER_CONTROL_1    			0xC0
#define POWER_CONTROL_2    			0xC1
#define VCOM_CONTROL_1    			0xC5
#define VCOM_CONTROL_2    			0xC7
#define POWER_CONTROL_A    			0xCB
#define POWER_CONTROL_B    			0xCF
#define POSITIVE_GAMMA_CORRECTION 	0xE0
#define NEGATIVE_GAMMA_CORRECTION 	0xE1
#define DRIVER_TIMING_CONTROL_A  	0xE8
#define DRIVER_TIMING_CONTROL_B  	0xEA
#define POWER_ON_SEQUENCE_CONTROL 	0xED
#define ENABLE_3G     				0xF2
#define INTERFACE_CONTROL   		0xF6
#define PUMP_RATIO_CONTROL   		0xF7

inline void LCDCommandWrite(unsigned char reg)
{
	*(volatile unsigned char *)(LCD_BASE_ADDR) = reg;
}
inline void LCDDataWrite(unsigned char wdata)
{
	*(volatile unsigned char *)(LCD_BASE_ADDR|1) = wdata;
}

unsigned char* LCDReadReg(unsigned char index,int rcnt)
{
#define LCD_READ_REG_SIZE_MAX 10
	static unsigned char rdata[LCD_READ_REG_SIZE_MAX];
	U32 i;

#if 0
	// Check rcnt
	
	if(rcnt > LCD_READ_REG_SIZE_MAX)
	{
		return 0;
	}
#endif
	
	LCDCommandWrite(index);
	
	for(i=0; i<rcnt; i++)
	{
		rdata[i]=*(volatile unsigned char *)(LCD_BASE_ADDR|1);
	}
	
    return rdata;
}

void LCDRamSet(unsigned short start_x, unsigned short start_y,unsigned short end_x,unsigned short end_y)
{
	LCDCommandWrite(SET_COLUMN_ADDRESS);
	LCDDataWrite(start_x>>8);
	LCDDataWrite(start_x);	
	LCDDataWrite(end_x>>8);
	LCDDataWrite(end_x);
	
	LCDCommandWrite(SET_PAGE_ADDRESS);
	LCDDataWrite(start_y>>8);
	LCDDataWrite(start_y);	
	LCDDataWrite(end_y>>8);
	LCDDataWrite(end_y);
	
	LCDCommandWrite(0x2C);
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
    LCDCommandWrite(SOFTWARE_RESET);
	delayms(100);
	LCDCommandWrite(DISPLAY_OFF);

	LCDCommandWrite(POWER_CONTROL_B);
	LCDDataWrite(0x00);
	LCDDataWrite(0x83);
	LCDDataWrite(0x30);

	LCDCommandWrite(POWER_ON_SEQUENCE_CONTROL);
	LCDDataWrite(0x64);
	LCDDataWrite(0x03);
	LCDDataWrite(0x12);
	LCDDataWrite(0x81);

	LCDCommandWrite(DRIVER_TIMING_CONTROL_A);
	LCDDataWrite(0x85);
	LCDDataWrite(0x01);
	LCDDataWrite(0x79);

	LCDCommandWrite(POWER_CONTROL_A);
	LCDDataWrite(0x39);
	LCDDataWrite(0x2c);
	LCDDataWrite(0x00);
	LCDDataWrite(0x34);
	LCDDataWrite(0x02);

	LCDCommandWrite(PUMP_RATIO_CONTROL);
	LCDDataWrite(0x20);

	LCDCommandWrite(DRIVER_TIMING_CONTROL_B);
	LCDDataWrite(0x00);
	LCDDataWrite(0x00);

	LCDCommandWrite(POWER_CONTROL_1);
	LCDDataWrite(0x26);

	LCDCommandWrite(POWER_CONTROL_2);
	LCDDataWrite(0x11);

	LCDCommandWrite(VCOM_CONTROL_1);
	LCDDataWrite(0x35);
	LCDDataWrite(0x3e);

	LCDCommandWrite(VCOM_CONTROL_2);
	LCDDataWrite(0xbe);

	LCDCommandWrite(MEMORY_ACCESS_CONTROL);
	LCDDataWrite(0x8);

	LCDCommandWrite(PIXEL_FORMAT_SET);
	LCDDataWrite(0x55);	// 65K color 16bit/pixel RGB 565

	LCDCommandWrite(FRAME_RATE_CONTROL);
	LCDDataWrite(0x00);
	LCDDataWrite(0x1B);

	LCDCommandWrite(ENABLE_3G);
	LCDDataWrite(0x08);

	LCDCommandWrite(GAMMA_SET);
	LCDDataWrite(0x01);

	LCDCommandWrite(POSITIVE_GAMMA_CORRECTION);
	LCDDataWrite(0x1f);
	LCDDataWrite(0x1a);
	LCDDataWrite(0x18);
	LCDDataWrite(0x0a);
	LCDDataWrite(0x0f);
	LCDDataWrite(0x06);
	LCDDataWrite(0x45);
	LCDDataWrite(0x87);
	LCDDataWrite(0x32);
	LCDDataWrite(0x0a);
	LCDDataWrite(0x07);
	LCDDataWrite(0x02);
	LCDDataWrite(0x07);
	LCDDataWrite(0x05);
	LCDDataWrite(0x00);

	LCDCommandWrite(NEGATIVE_GAMMA_CORRECTION);
	LCDDataWrite(0x00);
	LCDDataWrite(0x25);
	LCDDataWrite(0x27);
	LCDDataWrite(0x05);
	LCDDataWrite(0x10);
	LCDDataWrite(0x09);
	LCDDataWrite(0x3a);
	LCDDataWrite(0x78);
	LCDDataWrite(0x4d);
	LCDDataWrite(0x05);
	LCDDataWrite(0x18);
	LCDDataWrite(0x0d);
	LCDDataWrite(0x38);
	LCDDataWrite(0x3a);
	LCDDataWrite(0x1f);

	LCDCommandWrite(SET_COLUMN_ADDRESS);
	LCDDataWrite(0x00);
	LCDDataWrite(0x00);
	LCDDataWrite(0x00);
	LCDDataWrite(0xEF);

	LCDCommandWrite(SET_PAGE_ADDRESS);
	LCDDataWrite(0x00);
	LCDDataWrite(0x00);
	LCDDataWrite(0x01);
	LCDDataWrite(0x3F);

	LCDCommandWrite(ENTRY_MODE_SET);
	LCDDataWrite(0x07);

	LCDCommandWrite(DISPLAY_FUNCTION_CONTROL);
	LCDDataWrite(0x0a);
	LCDDataWrite(0xA2);
	LCDDataWrite(0x27);
	LCDDataWrite(0x00);

	LCDCommandWrite(SLEEP_OUT);
	delayms(200);
	LCDCommandWrite(DISPLAY_ON);
	delayms(200);
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
