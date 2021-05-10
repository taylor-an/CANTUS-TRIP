/*
**************************************************************************
*                   SPI EEPROM(W25Xxx) Functions
*                    Advanced Digital Chipes Inc.
**************************************************************************
*/

#ifndef __W25Xxx_H__
#define __W25Xxx_H__


int w25xxx_write_buffer (U32 addr, U8 *buffer, int length);
int w25xxx_read_buffer (U32 addr, U8 *buffer, int length);
int w25xxx_erase (U32 start_addr, int length);
int w25xxx_block_erase (U32 addr, int length);
int w25xxx_wait_ready (void);
int w25xxx_check_id (void);


#define W25XXX_PAGE_SIZE        (256)
#define W25XXX_SECTOR_SIZE      (4096)
#define W25XXX_BLOCK_SIZE       (16*W25XXX_SECTOR_SIZE)

#define W25X16_TOTAL_SIZE       (2*1024*1024)

#define W25X32_TOTAL_SIZE       (64*W25XXX_BLOCK_SIZE)
#define W25X64_TOTAL_SIZE       (128*W25XXX_BLOCK_SIZE)
#define W25X32_FLASH_MAXADDR    W25X32_TOTAL_SIZE

// W25Xxx: 3.3V -> Max ??MHz
#define W25Xxx_SPI_FREQ    23000000    // 23MHz
 
// W25Xxx Flash instruction
#define W25Xxx_WREN         0x06    // Write Enable
#define W25Xxx_WRDI         0x04    // Write Disable
#define W25Xxx_RDSR         0x05    // Read Status Register
//#define W25Xxx_WRSR         0x01    // Write Status Register
#define W25Xxx_READ         0x03    // Read Data Bytes
#define W25Xxx_FAST_READ    0x0B    // Read Data Bytes at Higher Speed
#define W25Xxx_DUAL_READ    0x3B    // Read Data Bytes at Higher Speed
#define W25Xxx_Page_Program       0x02    // Page Program
#define W25Xxx_BERASE       0xD8    // Block Erase
#define W25Xxx_SERASE       0x20    // Sector Erase
#define W25Xxx_CERASE       0xC7    // Sector Erase
#define W25Xxx_READID       0x90    // Read ID /* ID: 0x00, 0xEF, (0x14, 0x15, 0x16) */
#define W25Xxx_READJEDECID  0x9F    // Read ID /* ID: 0x00, 0xEF, (0x14, 0x15, 0x16) */

#define W25XXX_MAKERID      0xEF
#define W25X16_DEVID        0x14
#define W25X32_DEVID        0x15

#define W25Xxx_PDOWN        0xB9    // Power-down
#define W25Xxx_PON          0xAB    // Release Power-down / Device ID


#define NORMAL              W25Xxx_READ
#define FAST                W25Xxx_FAST_READ
#define BLOCK               W25Xxx_BERASE
#define SECTOR              W25Xxx_SERASE
#define ALL                 W25Xxx_CERASE

#define W25Xxx_SR_WIP  (1 << 0)    // Wait In Progress bit
#define W25Xxx_SR_WEL  (1 << 1)    // Wait Enable Latch bit

#define W25Xxx_ID1     0x00 // Manufacture ID
#define W25Xxx_ID2     0xEF // Memory Type
#define W25X16_ID3     0x14 // Memory Capacity
#define W25X32_ID3     0x15 // Memory Capacity
#define W25X64_ID3     0x16 // Memory Capacity

#define W25Xxx_JEDECID1     0xEF // Manufacture ID
#define W25Xxx_JEDECID2     0x30 // Memory Type
#define W25X16_JEDECID3     0x15 // Memory Capacity
#define W25X32_JEDECID3     0x16 // Memory Capacity
#define W25X64_JEDECID3     0x17 // Memory Capacity

#endif //__W25Xxx_H__
