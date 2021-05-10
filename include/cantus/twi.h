#pragma once


/* flags of TWI */
#define TWI_RECEIVE         0x21
#define TWI_TRANSMIT        0x22

#define R_TWICONR		    ((volatile unsigned int*)0x80022000)
#define TWICONR_REPST       (1 << 2)
#define TWICONR_NOACK       (1 << 3)
#define TWICONR_TWITX       (1 << 4)
#define TWICONR_MASTER      (1 << 5)
#define TWICONR_TWIEN       (1 << 7)

#define R_TWISTR	        ((volatile unsigned int*)0x80022004)
#define TWISTR_TWIRAK       (1 << 0)
#define TWISTR_TWIRXF       (1 << 1)
#define TWISTR_TWITXE       (1 << 2)
#define TWISTR_TWISRW       (1 << 3)
#define TWISTR_TWILOST      (1 << 4)
#define TWISTR_TWIBUSY      (1 << 5)
#define TWISTR_TWIAS        (1 << 6)
#define TWISTR_TWIDT        (1 << 7)

#define R_TWIADDR		    ((volatile unsigned int*) 0x80022008)

#define R_TWIDR		    ((volatile unsigned int*) 0x8002200C)

#define R_TWIBR0		    ((volatile unsigned int*) 0x80022010)
#define TWI_DEFAULT_SCALE       2
#define TWI_DEFAULT_MAX_FREQ    1000000
#define TWI_DEFAULT_MIN_FREQ    16000

#define R_TWIBR1		    ((volatile unsigned int*) 0x80022014)


int twi_set_freq (U32 freq);
void twi_trans_complete (void);
void twi_wait_busy(void);
void twi_start (U8 devaddr, U8 *waddr, int len, int flags);
int twi_write(U8 devaddr, U8 *waddr, int len, U8 *buffer, int length);
int twi_read(U8 devaddr, U8 *waddr, int len, U8 *buffer,int length);
int twi_read2(U8 devaddr, U8 *waddr, int len, U8 *buffer,int length);
