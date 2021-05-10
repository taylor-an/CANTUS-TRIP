
#include "cantus_lib_config.h"
#include "cantus.h"

static int nor_sector_size;
static int nor_size;
static BOOL bReadID=FALSE;
static U8 norid[2]={0,};
/* MACRO for read/write a memory */
#define __raw_readb(a) *((volatile U8 *)(a))

#define __raw_writeb(d, a) *((volatile U8  *)(a))=(U8)(d)


int __fcode norflash_readid (unsigned char *ids)
{
	unsigned int tmp;

	asm("set 8");//disable interrupt
	asm("sync");
    CacheDisable();
    *(volatile U8*)FLASH_Setup_Addr1 = FLASH_Setup_Code1;
    *(volatile U8*)FLASH_Setup_Addr2 = FLASH_Setup_Code2;
    *(volatile U8*)FLASH_Setup_Addr1 = FLASH_Read_ID;

    ids[0] = *(volatile U8*)(0x100);
    ids[1] = *(volatile U8*)(0x1);
	*(volatile U8*)0=FLASH_Reset;
	tmp = *(volatile U8*)(0x0); //don't delete it , this is for waiting reset time.
	CacheEnable();
	asm("clr 8");
	asm("sync");
	
	// Default CANTUS-128 NOR-Flash EN29LV010
	// 16K x 8
	nor_size = (128*1024);
	nor_sector_size = (16*1024);
	
	if(ids[1]==0x4f)
	{
		// CANTUS-512 NOR-Flash EN29LV040
		// 64K x 8
		nor_size = (512*1024);
		nor_sector_size = (64*1024);
	}
	else if(ids[1]==0xd5)
	{
		// CANTUS-128A NOR-Flash EN39LV010
		// 4K x 32
		nor_sector_size = (4*1024);
	}	
	
	norid[0]=ids[0];
	norid[1]=ids[1];
	bReadID = TRUE;
	
    return 0;
}

int __fcode norflash_erase (unsigned int saddr, unsigned int esize)
{
    int ret = 0;
    int timeout;
    unsigned int setup_a1 = FLASH_Setup_Addr1;
    unsigned int setup_a2 = FLASH_Setup_Addr2;

	if(bReadID==FALSE)
		norflash_readid(norid);
    saddr &= ~(nor_sector_size - 1);
    esize &= ~(nor_sector_size - 1);

	asm("set 8");//disable interrupt
	asm("sync");
	CacheDisable();
    while (esize) {
        __raw_writeb(FLASH_Setup_Code1, setup_a1);
        __raw_writeb(FLASH_Setup_Code2, setup_a2);
        __raw_writeb(FLASH_Setup_Erase, setup_a1);

        __raw_writeb(FLASH_Setup_Code1, setup_a1);
        __raw_writeb(FLASH_Setup_Code2, setup_a2);

        __raw_writeb(FLASH_Sector_Erase, saddr);

        timeout = 0;
        while (1) {
            if ((__raw_readb(saddr) & 0x80) == 0x80)
                break;
            if (++timeout > 0x1000000) {
                ret = -1;
                break;
            }
        }

        if (ret == -1)
            break;

        esize -= nor_sector_size;
        saddr += nor_sector_size;
    }
	CacheEnable();
	asm("clr 8");
	asm("sync");

    return ret;
}

int __fcode norflash_write (unsigned char *buf, unsigned int saddr,
        unsigned int wsize)
{
    int i;
    unsigned int setup_a1 = FLASH_Setup_Addr1;
    unsigned int setup_a2 = FLASH_Setup_Addr2;

    unsigned int code1 = FLASH_Setup_Code1;
    unsigned int code2 = FLASH_Setup_Code2;
    int delay = 0;
    int ret = 0;

	if(bReadID==FALSE)
		norflash_readid(norid);

	asm("set 8");//disable interrupt
	asm("sync");
	CacheDisable();
    for (i = 0; i < wsize; i++) {
        __raw_writeb(code1, setup_a1);
        __raw_writeb(code2, setup_a2);
        __raw_writeb(FLASH_Program, setup_a1);

        __raw_writeb(buf[i], saddr);

        delay = 0;
        while (1) {
            if (__raw_readb(saddr) == buf[i])
                break;
            delay++;
            if (delay > 0x100000) {
                ret = -1;
                break;
            }
        }

        if (ret != 0)
            break;

        saddr++;
    }
	CacheEnable();
	asm("clr 8");
	asm("sync");

    return ret;
}

int __fcode norflash_get_sector_size()
{
	if(bReadID==FALSE)
		norflash_readid(norid);
	return nor_sector_size;
}
int __fcode norflash_get_size()
{
	if(bReadID==FALSE)
		norflash_readid(norid);
	return nor_size;
}
