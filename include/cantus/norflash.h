
#pragma once


#define __fcode  __attribute__ ((unused,section (".flash_code")))

#define FLASH_Read_ID       (0x90)
#define FLASH_Reset         (0xF0)
#define FLASH_Program       (0xA0)
#define FLASH_Setup_Erase   (0x80)
#define FLASH_Sector_Erase  (0x30)

#define FLASH_Setup_Addr1        (0x555)
#define FLASH_Setup_Addr2        (0x2AA)

#define FLASH_Setup_Code1    (0xAA)
#define FLASH_Setup_Code2    (0x55)

int __fcode norflash_readid (unsigned char *ids);
int __fcode norflash_erase (unsigned int saddr, unsigned int esize);
int __fcode norflash_write (unsigned char *buf, unsigned int saddr,
        unsigned int wsize);
int __fcode norflash_get_size();
int __fcode norflash_get_sector_size();
