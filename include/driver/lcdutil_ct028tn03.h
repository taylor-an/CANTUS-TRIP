
#pragma once

void drawstring(int x, int y, char* str,BOOL bBg);
void drawchar(int x, int y, char ch,BOOL bBg);
BOOL drawBMP(char* fname);
BOOL drawBMPRect(char* fname,int startx, int starty,int dx,int dy);
