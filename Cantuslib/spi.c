#include "cantus.h"

static void inline spi_wait_finish (void)
{
	U8 stat;
	do {
		stat = *R_SPI0STAT;
	} while (!(stat & SPISTAT_SPIF));
}

void spi_master_init(void)
{
	*R_SPI0CON = SPICON_MSTR |SPICON_CPOL |SPICON_CPHA| SPICON_MSBF | SPICON_EN;
}

/* 
* mode: SPI_MASTER, SPI_SLAVE
* freq: Frequency
*/
int spi_set_freq (int mode, U32 freq)
{
	U8 baudrate;

	if ((mode == SPI_MASTER) && (freq > SPI_MASTER_MAXFREQ))
	{
		return SPI_MASTER_MAXFREQ;
	}
	else if ((mode == SPI_SLAVE) && (freq > SPI_SLAVE_MAXFREQ))
	{
		return SPI_SLAVE_MAXFREQ;
	}

	baudrate = (GetAPBclock() / (freq<<1)) - 1;
	*R_SPI0BAUD=baudrate;

	return baudrate;
}

void spi_master_xfer(U8 *wbuf, int wlength, U8 *rbuf,
					 int rlength, int continue_xfer)
{
	int i;
	int rcnt = 0, rlen;
	int wcnt = 0, wlen;
	U8 flush;

	if (wlength > SPI_MAX_WFIFO)
		wlen = SPI_MAX_WFIFO;
	else
		wlen = wlength;

	if (rlength > SPI_MAX_RFIFO)
		rlen = SPI_MAX_RFIFO;
	else
		rlen = rlength;

#ifndef USE_W5500
	SPI_CS_LOW();
#endif

	while (wcnt < wlength) {
		for (i = 0; i < wlen; ) {
			*R_SPI0DATA = wbuf[i];
			i++;
		}
		spi_wait_finish();

		for (i = 0; i < wlen; i++)
			flush = *R_SPI0DATA; // flush garbage

		wbuf += wlen;
		wcnt += wlen;
		if ((wlength - wcnt) < SPI_MAX_WFIFO)
			wlen = wlength - wcnt;
	}

	while (rcnt < rlength) {
		for (i = 0; i < rlen; ) {
			*R_SPI0DATA = 0xaa;
			spi_wait_finish();
			flush = *R_SPI0DATA;
			rbuf[i] = flush;
			i++;
		}

		rbuf += rlen;
		rcnt += rlen;
		if ((rlength - rcnt) < SPI_MAX_RFIFO)
			rlen = rlength - rcnt;
	}

#ifndef USE_W5500
	if (continue_xfer == 0)
		SPI_CS_HIGH();
#endif
}


void spi_wait_empty_fifo (void)
{
	U8 stat;
	do {
		stat = *R_SPI0STAT;
	} while (!(stat & SPISTAT_STXE));
}
