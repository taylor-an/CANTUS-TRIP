#pragma once

#define R_RTCCON      ((volatile unsigned int*)0x80023800)
#define R_RTCSEC      ((volatile unsigned int*)0x80023804)
#define R_RTCMIN      ((volatile unsigned int*)0x80023808)
#define R_RTCHOUR     ((volatile unsigned int*)0x8002380C)
#define R_RTCDAY      ((volatile unsigned int*)0x80023810)
#define R_RTCWEEK     ((volatile unsigned int*)0x80023814)
#define R_RTCMON      ((volatile unsigned int*)0x80023818)
#define R_RTCYEAR     ((volatile unsigned int*)0x8002381C)

#define RTC_BASE_YEAR	2000

typedef struct 
{
	int	sec;
	int	min;
	int	hour;
	int	day;
	int	week; // 0:Sun,1:Mon,...
	int	mon; // 1: Jan, 2:Feb,...
	int	year;
}rtcTime;

void rtc_gettime(rtcTime* tm);
void rtc_settime(rtcTime* tm);
