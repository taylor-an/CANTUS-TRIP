#pragma once

BOOL SPIFLASH_init(void);

U32 SPI_GetSectorSize(void);
U32 SPI_GetSectorCount(void);
U32 SPI_MemSize(void); //return MegaByte Size

int spi_readpage(U32 pagenum, void* buf);
BOOL spi_readpage_block(unsigned long baddr,void *buf, U32 blkcnt);
BOOL spi_readpage_block_wusb(unsigned long baddr,U32 blkcnt);

int spi_writepage(U32 pagenum, void* buf);
int spi_eraseblock(U32 pagenum);
int update_spisectors(unsigned long sector,void *buffer, U32 sectorcnt);


