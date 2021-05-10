#include "cantus.h"
#include "../include/driver/w25xxx_flash.h"

#define SRAM_SECTOR_BUFFER

BOOL SPIFLASH_init()
{
	int baud;
	int ret;
	
	*R_PAF2 |= ( 3<<12);//PIO2.6 for SPI nSS
	*R_PAF4 = (*R_PAF4 & ~0x3f0)|(0x150); // SPI_SCK, SPI_MISO, SPI_MOSI
	
	//------------------------------------------------------------
	// EVM Board 2.0	
	// *R_P2oDIR = (1<<6);//PIO2.6 output mode , SPI Chip Select
	//------------------------------------------------------------
	// EVM Board 3.0	
	*R_P3oDIR = (1<<0);//PIO3.0 output mode , SPI Chip Select
	//------------------------------------------------------------
	
	spi_master_init();
	baud = spi_set_freq(SPI_MASTER, W25Xxx_SPI_FREQ);
	debugprintf("SPI SCK : %d KHz\n\r", SPI_SCK(baud)/1000);
	
	ret = w25xxx_check_id();
    if (ret != 0)
	{
        debugstring("W25Xxx ID Check Fail\r\n!\n");
		return FALSE;
    }
	else
	{
		debugstring("W25Xxx ID Check!\r\n");
	}
	
	return TRUE;
}
U32 SPI_GetSectorCount()
{
#ifdef SRAM_SECTOR_BUFFER
	return (W25X32_TOTAL_SIZE)/512;	
#else
	// FLASH_SECTOR_BUFFER
	return (W25X32_TOTAL_SIZE - W25XXX_SECTOR_SIZE)/512;
#endif
}
U32 SPI_GetSectorSize()
{
	return 0;
}
U32 SPI_MemSize() //return MegaByte Size
{
	return 0;
}

int spi_readpage(U32 pagenum, void* buf)
{
#ifdef SRAM_SECTOR_BUFFER
	return w25xxx_read_buffer(pagenum,buf,512);
#else
	return w25xxx_read_buffer(pagenum+W25XXX_SECTOR_SIZE,buf,512);
#endif
}

#ifdef SRAM_SECTOR_BUFFER

BOOL spi_readpage_block(unsigned long baddr,void *buf, U32 blkcnt)
{
	U8* buffer = (U8*)buf;
	int ret = 0;
	U8 wbuf[4];

	baddr = (baddr * 512);

	wbuf[0] = W25Xxx_READ;
	wbuf[1] = (baddr >> 16);
	wbuf[2] = (baddr >> 8);
	wbuf[3] = baddr;
	
	asm("clr 13");
	spi_master_xfer(wbuf, 4, buffer, 512*blkcnt, 0);

	ret = w25xxx_wait_ready();
	asm("set 13");
	
	return TRUE;
}

BOOL spi_readpage_block_wusb(unsigned long baddr,U32 blkcnt)
{
	ALIGN4 U8 buf[W25XXX_SECTOR_SIZE];	// 512 * 8
	int ret = 0;
	U8 wbuf[4];
	U32 i,j;
	U32 k;
	baddr = (baddr * 512);

	for(j=0; j<blkcnt;)
	{
		wbuf[0] = W25Xxx_READ;
		wbuf[1] = (baddr >> 16);
		wbuf[2] = (baddr >> 8);
		wbuf[3] = baddr;
		
		asm("clr 13");
		if(blkcnt-j > 8)
		{
			spi_master_xfer(wbuf, 4, buf, 512*8, 0);
			k = 8;
		}
		else
		{
			spi_master_xfer(wbuf, 4, buf, 512*(blkcnt-j), 0);
			k = (blkcnt-j);
		}
		
		ret = w25xxx_wait_ready();
		asm("set 13");
		
		for(i=0;i<512*k;)
		{
			write_usb_full(&buf[i]);
			i+=64;
		}
		
		baddr += 512*k;
		j+=k;
	}
	
	return TRUE;
}
#endif

int spi_writepage(U32 pagenum, void* buf)
{
#ifdef SRAM_SECTOR_BUFFER
	return w25xxx_write_buffer(pagenum,buf,256);
#else
	return w25xxx_write_buffer(pagenum+W25XXX_SECTOR_SIZE,buf,256);
#endif
}

int spi_eraseblock(U32 pagenum)
{
#ifdef SRAM_SECTOR_BUFFER
	return w25xxx_erase(pagenum,W25XXX_SECTOR_SIZE);
#else
	return w25xxx_erase(pagenum+W25XXX_SECTOR_SIZE,W25XXX_SECTOR_SIZE);
#endif
}

#ifdef SRAM_SECTOR_BUFFER

int update_spisectors(unsigned long sector,void *buffer, U32 sectorcnt)
{
	ALIGN4 U8 tempbuf[W25XXX_SECTOR_SIZE];
	
	unsigned int sectorperblock = (W25XXX_SECTOR_SIZE)/512; // 8
	
	int blocknum;
	int pagenum;
	U8* buf = (U8*)buffer;
	unsigned int startblock = (sector / sectorperblock);
	unsigned int endblock = (sector+sectorcnt) / sectorperblock;
	
	unsigned int startsector = sector;
	unsigned int endsector = sector + sectorcnt-1;
	
	unsigned int startpage = startsector * 512;
	unsigned int endpage = (endsector * 512);

	U32 buff_ptr=0;
	U32 i,k;
	
	for(blocknum = startblock; blocknum<endblock+1; blocknum ++)
	{
		k = 0;
		
		for(pagenum = 0; pagenum < 0xF00; pagenum+=512)
		{
			if(pagenum+(blocknum * 0x1000) >= startpage && pagenum+(blocknum * 0x1000) <= endpage)
			{
				for(i=0; i<512; i++)
				{
					tempbuf[k] = buf[buff_ptr];
					k++;
					buff_ptr++;
				}
			}
			else
			{
				spi_readpage((blocknum*0x1000)+pagenum,&tempbuf[k]);
				k+=512;
			}
		}
		
		if(spi_eraseblock(blocknum*0x1000) == -1)
		{
			debugprintf("block number %x adrress = %xerase failed\r\n",blocknum,blocknum*0x1000);
			return -1;
		}
		
		k = 0;
		for(pagenum = 0;pagenum < 0xF00 ; pagenum+=512)
		{
			spi_writepage((blocknum*0x1000)+pagenum,&tempbuf[k]);
			spi_writepage((blocknum*0x1000)+pagenum+256,&tempbuf[k+256]);
			k+=512;
		}
	}
	
	return 0;
}
#endif
