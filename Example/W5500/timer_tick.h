// timer_tick.h

#ifndef _TIMER_TICK_H_
#define _TIMER_TICK_H_

#include "cantus.h"

static U32 timer_tick_period;
static volatile U32 timer_tick_cnt = 0;
static volatile U32 timer_tick_1ms = 0;
static volatile U32 timer_tick_10ms = 0;
static volatile U32 timer_tick_100ms = 0;
static volatile U32 timer_tick_1000ms = 0;

void timer_init(U32 period);
U32 get_timer_tick_period(void);
U32 get_timer_tick_cnt(void);
U32 get_timer_tick_1ms(void);
U32 get_timer_tick_10ms(void);
U32 get_timer_tick_100ms(void);
U32 get_timer_tick_1000ms(void);
U32* getp_timer_tick_cnt(void);
U32* getp_timer_tick_1ms(void);
U32* getp_timer_tick_10ms(void);
U32* getp_timer_tick_100ms(void);
U32* getp_timer_tick_1000ms(void);
bool time_over_check(U32 time_stamp, U32 *tick, U32 cnt);
void timer_tick_handler(void);

#endif
