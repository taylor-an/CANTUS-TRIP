#include "cantus.h"

void rtc_gettime(rtcTime* ptm)
{
    int val;
    val = *R_RTCSEC;
    ptm->sec = val & 0x3f;

    val = *R_RTCMIN;
    ptm->min = val & 0x3f;

    val = *R_RTCHOUR;
    ptm->hour = val & 0x1f;

    val = *R_RTCDAY;
    ptm->day = val & 0x1f;

    val = *R_RTCWEEK;
    ptm->week = val&0x7;

    val = *R_RTCMON;
    ptm->mon = val & 0xf;

    val = *R_RTCYEAR;
    ptm->year = (val & 0x7f)+RTC_BASE_YEAR;
}

void rtc_settime(rtcTime* ptm)
{
    *R_RTCSEC  = ptm->sec;
    *R_RTCMIN  = ptm->min;
    *R_RTCHOUR = ptm->hour;
    *R_RTCDAY  = ptm->day;
    *R_RTCWEEK = ptm->week;
    *R_RTCMON  = ptm->mon;
    *R_RTCYEAR = ptm->year-RTC_BASE_YEAR;
	while(*R_RTCSEC & (1<<6));//wait for update
}

