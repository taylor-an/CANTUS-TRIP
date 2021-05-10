// timer_tick.c

#include "timer_tick.h"

void timer_init(U32 period)
{
	timer_tick_period = period;
    setinterrupt(INTNUM_TIMER0, timer_tick_handler);
	settimer(0, timer_tick_period);
}

U32 get_timer_tick_period(void)
{
    return timer_tick_period;
}

U32 get_timer_tick_cnt(void)
{
    return timer_tick_cnt;
}

U32 get_timer_tick_1ms(void)
{
    return timer_tick_1ms;
}

U32 get_timer_tick_10ms(void)
{
    return timer_tick_10ms;
}

U32 get_timer_tick_100ms(void)
{
    return timer_tick_100ms;
}

U32 get_timer_tick_1000ms(void)
{
    return timer_tick_1000ms;
}

U32* getp_timer_tick_cnt(void)
{
    return (U32*)&timer_tick_cnt;
}

U32* getp_timer_tick_1ms(void)
{
    return (U32*)&timer_tick_1ms;
}

U32* getp_timer_tick_10ms(void)
{
    return (U32*)&timer_tick_10ms;
}

U32* getp_timer_tick_100ms(void)
{
    return (U32*)&timer_tick_100ms;
}

U32* getp_timer_tick_1000ms(void)
{
    return (U32*)&timer_tick_1000ms;
}

bool time_over_check(U32 time_stamp, U32 *tick, U32 cnt)
{
    U32 cur_time;
    
    cur_time = *tick;
    
    if(cur_time < time_stamp)
    {
        cur_time += (0xffffffff - time_stamp) + 1;
        if(cur_time >= cnt)
        {
            return true;
        }
    }
    else
    {
        if((cur_time - time_stamp) >= cnt)
        {
            return true;
        }
    }

    return false;
}

void timer_tick_handler(void)
{
    if (timer_tick_cnt == 1000)
    {
        timer_tick_cnt = 0;
    }
    timer_tick_cnt++;

    if (timer_tick_cnt % 1 == 0)
    {
        timer_tick_1ms++;
    }

    if (timer_tick_cnt % 10 == 0)
    {
        timer_tick_10ms++;
    }

    if (timer_tick_cnt % 100 == 0)
    {
        timer_tick_100ms++;
    }

    if (timer_tick_cnt % 1000 == 0)
    {
        timer_tick_1000ms++;
    }
}


