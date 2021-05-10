#pragma once

void drawstring(int x, int y, int w, int h, char* str,BOOL bBg);
void drawchar(int x, int y, int w, int h, char ch,BOOL bBg);
void drawcharKor(int x, int y, int w, int h,unsigned int Kcode, BOOL bBg);
BOOL drawBMP(char* fname);
BOOL drawBMPRect(char* fname,int startx, int starty,int dx,int dy);
