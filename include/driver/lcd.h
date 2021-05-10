#pragma once

#define LCD_BASE_ADDR (0x60000000 | (1<<17)|(1<<15)|(1<<14))

void LCDRamSet(unsigned short start_x, unsigned short start_y,unsigned short end_x,unsigned short end_y);
void LCDInit();
void LCDdrawbox(int sx, int sy, int endx, int endy,U8 r , U8 g, U8 b);
void LCDputpixel(int x, int y, U8 r, U8 g, U8 b);
void LCDcls(U8 r, U8 g, U8 b);
