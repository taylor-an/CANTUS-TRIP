/*
*******************************************************************************
*                   SPI FLASH(W25Xxx) Functions
                      Advanced Digital Chipes Inc.
*******************************************************************************
*/

/*
 * W25X16: Winbond 16Mbit/2Mbyte
 * W25X32: Winbond 32Mbit/4Mbyte
 * W25X64: Winbond 64Mbit/8Mbyte
 */


#include "cantus.h"
#include "driver/w25xxx_flash.h"

int w25xxx_write_buffer (U32 addr, U8 *buffer, int length)
{
    int wlen;
    U8 wbuf[4];
    int ret = 0;
	int i;
	
	U32 startaddr = addr;
	U32 endaddr = addr+length-1;
	U32 startpage;
	U32 endpage;
	
	startpage = startaddr/W25XXX_PAGE_SIZE;
	endpage = endaddr/W25XXX_PAGE_SIZE;

	for(i=startpage;i<endpage+1;i++)
	{
		U32 offset = addr%W25XXX_PAGE_SIZE;
		if(offset + length > W25XXX_PAGE_SIZE)
		{
			wlen = W25XXX_PAGE_SIZE  - offset;
		}
		else
		{
			wlen = length;
		}

		asm("clr 13");
		
		wbuf[0] = W25Xxx_WREN;
		spi_master_xfer(wbuf, 1, NULL, 0, 0);
		ret = w25xxx_wait_ready();
		if (ret != 0)
			break;

		wbuf[0] = W25Xxx_Page_Program;
		
		wbuf[1] = (addr>>16);
		wbuf[2] = (addr>>8);
		wbuf[3] = addr;

		spi_master_xfer(wbuf, 4, NULL, 0, 1); // Write Command & Address
											   // Continue to write data next
		spi_master_xfer(buffer, wlen, NULL, 0, 0); // Write Data

		/*  Write Cycle Time of W25Xxx FLASH: 3.3V -> 11ms, 1.2ms */
		ret = w25xxx_wait_ready();
		if (ret != 0)
			break;

		buffer += wlen;
		addr += wlen;
		length -=wlen;

		wbuf[0] = W25Xxx_WRDI;
		spi_master_xfer(wbuf, 1, NULL, 0, 0);
		ret = w25xxx_wait_ready();
		if (ret != 0)
			break;
		
		asm("set 13");
	}
	return ret;
}

/*
 * mode: NORMAL, FAST
 */
int w25xxx_read_buffer (U32 addr, U8 *buffer, int length)
{
    int ret = 0;
    U8 wbuf[4];
		
	wbuf[0] = W25Xxx_READ;
	wbuf[1] = (addr>>16);
	wbuf[2] = (addr>>8);
	wbuf[3] = addr;
	asm("clr 13");
	spi_master_xfer(wbuf, 4, buffer, length, 0);

	ret = w25xxx_wait_ready();
	asm("set 13");

    return ret;
}

/*
 * SECTOR erase
 */
int w25xxx_erase (U32 addr, int length)
{
    U8 wbuf[4];
    int ret = 0;
    unsigned int esize = 0;
    unsigned int sector_size = W25XXX_SECTOR_SIZE;
    unsigned int block_size = W25XXX_BLOCK_SIZE;

    length += sector_size - 1;
    length &= ~(sector_size - 1);

    addr &= ~(sector_size - 1);

    while (length) {

        if (length >= block_size) {
            esize = block_size;
        }
        else if (length >= sector_size) {
            esize = sector_size;
        }

        wbuf[0] = W25Xxx_WREN;
        spi_master_xfer(wbuf, 1, NULL, 0, 0);
        ret = w25xxx_wait_ready();
        if (ret != 0)
            break;

        if (esize == block_size) {
            wbuf[0] = W25Xxx_BERASE;
        }
        else if (esize == sector_size) {
            wbuf[0] = W25Xxx_SERASE;
        }

        wbuf[1] = (addr>>16);
        wbuf[2] = (addr>>8);
        wbuf[3] = addr;

        spi_master_xfer(wbuf, 4, NULL, 0, 0);

        ret = w25xxx_wait_ready();
        if (ret != 0)
            break;

        addr += esize;
        length -= esize;

        wbuf[0] = W25Xxx_WRDI;
        spi_master_xfer(wbuf, 1, NULL, 0, 0);
        ret = w25xxx_wait_ready();
        if (ret != 0)
            break;
    }

    return 0;
}

int w25xxx_wait_ready (void)
{
    U8 inst = W25Xxx_RDSR;
    U8 status;
    int delay = 0;
    int ret = 0;

    do {
        spi_master_xfer (&inst, 1, &status, 1, 0);
        delay++;
        if (delay > 0x80000) {
            ret = -1;
            break;
        }
    } while (status & W25Xxx_SR_WIP);


    return ret;
}

int w25xxx_check_id (void)
{
    U8 buf[4];

    buf[0] = W25Xxx_READID;
    buf[1] = 0;
    buf[2] = 0;
    buf[3] = 0;
    spi_master_xfer(buf, 4, buf, 2, 0);

    w25xxx_wait_ready();

    if ((buf[0] != W25XXX_MAKERID) || (buf[1] != W25X32_DEVID)) {
        debugprintf(" 0x%x ", buf[0] );
        debugprintf("0x%x \r\n", buf[1] );// W25X32_ID3
       return -1;
    }

    return 0;
}

