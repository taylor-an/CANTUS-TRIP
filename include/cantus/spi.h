#pragma once

#define SPI_MAX_WFIFO   8
#define SPI_MAX_RFIFO   8

#define R_SPI0CON      ((volatile unsigned int*)0x80021C00)
#define SPICON_SPISIZE8B   (0 << 0)
#define SPICON_SPISIZE16B  (1 << 0)
#define SPICON_SPISIZE32B  (2 << 0)
#define SPICON_MSBF        (1 << 2)
#define SPICON_CPHA        (1 << 3)
#define SPICON_CPOL        (1 << 4)
#define SPICON_MSTR        (1 << 5)
#define SPICON_WOMP        (1 << 6)
#define SPICON_EN          (1 << 7)

#define	R_SPI0BAUD	((volatile unsigned int*)0x80021C04)

#define R_SPI0STAT	((volatile unsigned int*)0x80021C08)
#define SPISTAT_SRXE     (1 << 0)
#define SPISTAT_SRXF     (1 << 1)
#define SPISTAT_STXE     (1 << 2)
#define SPISTAT_STXF     (1 << 3)
#define SPISTAT_SSX      (1 << 4)
#define SPISTAT_MODF     (1 << 5)
#define SPISTAT_WCOL     (1 << 6)
#define SPISTAT_SPIF     (1 << 7)

#define R_SPI0DATA     ((volatile unsigned int*)0x80021C0C)

#define R_SSX0CON      ((volatile unsigned int*)0x80021C10)
#define SSXCR_SSXCON    (1 << 0)

#define R_SPI0INT	    ((volatile unsigned int*)0x80021C14)
#define SIMK_SRXEE     (1 << 0)
#define SIMK_SRXFE     (1 << 1)
#define SIMK_STXEE     (1 << 2)
#define SIMK_STXFE     (1 << 3)
#define SIMK_SSXE      (1 << 4)
#define SIMK_MODFE     (1 << 6)
#define SIMK_SPIFE     (1 << 7)

#define SPI_FIFOSIZE    64

#define SPI_BAUD(clk)    ((GetAPBclock()  / (2*clk))-1)
#define SPI_SCK(baud)    (GetAPBclock()  / (2*(baud+1)))

#define SPI_MASTER      0
#define SPI_SLAVE       1

#define SPI_MASTER_MAXFREQ      (GetAPBclock()/2)
#define SPI_SLAVE_MAXFREQ       (GetAPBclock()/4)

#define USE_W5500

#ifdef USE_W5500
// W5500
#define SPI_CS_LOW()       *R_P2oLOW = (1<<7)
#define SPI_CS_HIGH()      *R_P2oHIGH = (1<<7)
#else
//Board 2.0	
// #define SPI_CS_LOW()       *R_P2oLOW = (1<<6)
// #define SPI_CS_HIGH()      *R_P2oHIGH = (1<<6)
//Board 3.0	
#define SPI_CS_LOW()       *R_P3oLOW = (1<<0)
#define SPI_CS_HIGH()      *R_P3oHIGH = (1<<0)
#endif

void spi_master_init(void);
int spi_set_freq (int mode, U32 freq);
void spi_master_xfer (U8 *wbuf, int wlength, U8 *rbuf, int rlength, int continue_xfer);
void spi_wait_empty_fifo (void);

