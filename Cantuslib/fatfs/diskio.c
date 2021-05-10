/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2012        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/
#if 0
#include "diskio.h"		/* FatFs lower layer API */
#include "usbdisk.h"	/* Example: USB drive control */
#include "atadrive.h"	/* Example: ATA drive control */
#include "sdcard.h"		/* Example: MMC/SDC contorl */

#else
#include "cantus.h"
#include "fatfs/diskio.h"

#endif

#if CONFIG_FAT_NAND_SUPPORT ==		0
#define nand_init(...)				0
#define nand_readsects(...)			0
#define nand_updatesects(...)		0
#define nand_get_sectorcount()		0
#endif

//avoid SDCARD api linking 
#if CONFIG_FAT_SDCARD_SUPPORT ==	0
#define SDC_Init(...)				0
#define SDC_Read_Sect(...)			0
#define SDC_Write_Sect(...)			0
#define SDC_GetSectorCount()		0
#endif

#if CONFIG_FAT_SPIFLASH_SUPPORT ==	0
#define SPIFLASH_init(...)			0
#define spi_readpage_block(...)		0
#define update_spisectors(...)		0
#define SPI_GetSectorCount()		0
#endif

#if _FS_REENTRANT

static U32 org_int_mask;
static U32 fs_int_disable;

/*
These functions is used for only firmware.
if you use FreeRTOS, Use semaphore
*/

/* Create a sync object */
int  __attribute__((weak)) ff_cre_syncobj  (BYTE b, _SYNC_t* t)
{
	*t = 1;
	return 1;
}

/* Lock sync object */
int __attribute__((weak)) ff_req_grant (_SYNC_t t)
{
	/*register Interrupt Number what used filesystem
	DMA(Sound) ISR is use FS
	*/

	org_int_mask = *R_INTMASK;
	
	*R_INTMASKCLR = fs_int_disable;

	return 1;
}
/* Unlock sync object */
void __attribute__((weak)) ff_rel_grant (_SYNC_t t)		
{
	*R_INTMASK = org_int_mask;
}
/* Delete a sync object */
int __attribute__((weak))  ff_del_syncobj (_SYNC_t t)
{
	return 1;
}
#endif

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE drv				/* Physical drive nmuber (0..) */
)
{
	if(drv == DRIVE_NAND )
	{
		if(nand_init())
			return RES_OK;
	}
	else if(drv == DRIVE_SDCARD)
	{
		if(SDC_Init())
			return RES_OK;
	}
	else if(drv == DRIVE_SPIFLASH)
	{
		if(SPIFLASH_init())
			return RES_OK;
	}
	else
		debugprintf("%d driver not supported\r\n",drv);
	return RES_ERROR;
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE drv		/* Physical drive nmuber (0..) */
)
{
	return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..128) */
)
{
	if(drv == DRIVE_NAND)
	{
		if(nand_readsects(sector,buff,count))
			return RES_OK;
	}
	else if(drv == DRIVE_SDCARD)
	{
		if(SDC_Read_Sect(buff,sector,count))
		return RES_OK;
	}
	else if(drv == DRIVE_SPIFLASH)
	{
		if(spi_readpage_block(sector, buff, count))
		return RES_OK;
	}
	return RES_ERROR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE drv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..128) */
)
{
	if(drv == DRIVE_NAND)
	{
		if(nand_updatesects(sector,(void*)buff,count))
			return RES_OK;
	}
	else if(drv == DRIVE_SDCARD)
	{
		if( SDC_Write_Sect((void*)buff,sector,count) )
			return RES_OK;
	}
	else if(drv == DRIVE_SPIFLASH)
	{
		if(update_spisectors(sector,(void*)buff,count) != -1)
		{
			return RES_OK;
		}
	}
	return RES_ERROR;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res =RES_PARERR ;

	switch (ctrl) {
	case CTRL_SYNC:
		if(drv==DRIVE_NAND)
		{
#if CONFIG_FAT_NAND_SUPPORT == 1			
			nand_flushdata();
#endif		
		}
		res = RES_OK;
		break;

	case GET_SECTOR_COUNT:
		if(drv==DRIVE_NAND)
		{
			*(DWORD*)buff = nand_get_sectorcount();
		}
		else if(drv==DRIVE_SDCARD)
		{
			*(DWORD*)buff = SDC_GetSectorCount();
		}
		else if(drv==DRIVE_SPIFLASH)
		{
			*(DWORD*)buff = SPI_GetSectorCount();
		}
		res = RES_OK;
		break;

	case GET_SECTOR_SIZE:
		*(WORD*)buff = 512;
		res = RES_OK;
		break;

	case GET_BLOCK_SIZE:
		*(DWORD*)buff = 1;
		res = RES_OK;
		break;
	}

	return res;
}
#endif
/*
Currnet time is returned with packed into a DWORD value. The bit field is as follows:

bit31:25 
Year from 1980 (0..127) 
bit24:21 
Month (1..12) 
bit20:16 
Day in month(1..31) 
bit15:11 
Hour (0..23) 
bit10:5 
Minute (0..59) 
bit4:0 
Second / 2 (0..29) 
*/
DWORD get_fattime (void)
{
	
	return 0;
}
