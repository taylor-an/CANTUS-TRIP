#pragma once

#define R_GDMASTAT	((volatile unsigned int*)0x80000800)
#define R_GDMAIM	((volatile unsigned int*)0x80000804)
#define R_GDMAESTAT	((volatile unsigned int*)0x80000808)
#define R_GDMARSS	((volatile unsigned int*)0x8000080c)
#define R_GDMACFG	((volatile unsigned int*)0x80000810)
#define R_GDMALR	((volatile unsigned int*)0x80000818)
#define R_GDMACON0	((volatile unsigned int*)0x80000820)
#define R_GDMAS0	((volatile unsigned int*)0x80000824)
#define R_GDMAD0	((volatile unsigned int*)0x80000828)
#define R_GDMAT0	((volatile unsigned int*)0x8000082c)
#define R_GDMADT0	((volatile unsigned int*)0x80000830)
#define R_GDMACON1	((volatile unsigned int*)0x80000840)
#define R_GDMAS1	((volatile unsigned int*)0x80000844)
#define R_GDMAD1	((volatile unsigned int*)0x80000848)
#define R_GDMAT1	((volatile unsigned int*)0x8000084c)
#define R_GDMADT1	((volatile unsigned int*)0x80000850)


#define DMA_SIZE_8BIT 0
#define DMA_SIZE_16BIT (1<<2)
#define DMA_SIZE_32BIT (2<<2)

#define DMA_DESTADDR_FIX 0
#define DMA_DESTADDR_INC (2<<4)
#define DMA_DESTADDR_DEC (3<<4)

#define DMA_SRCADDR_FIX 0
#define DMA_SRCADDR_INC (2<<6)
#define DMA_SRCADDR_DEC (3<<6)

#define DMA_BURST_NO (0)
#define DMA_BURST_4BEAT (1<<8)
#define DMA_BURST_8BEAT (2<<8)
#define DMA_BURST_16BEAT (3<<8)

#define DMA_CHAIN_MODE (1<<15)

#define DMA0_REQ_SRC_I2S (0)
#define DMA0_REQ_SRC_SW (6<<16)
#define DMA1_REQ_SRC_NANDTX (0<<16)
#define DMA1_REQ_SRC_NANDRX (1<<16)
#define DMA1_REQ_SRC_SDHC (2<<16)
#define DMA1_REQ_SRC_SW (4<<16)

#define DMA_START_BY_SW (1<<24)

