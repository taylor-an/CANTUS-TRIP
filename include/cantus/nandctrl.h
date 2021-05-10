/******************************************************************************
	Copyright (C) 2013	Advanced Digital Chips Inc. 
						http://www.adc.co.kr
						
	Author : Software Team.
******************************************************************************/

#pragma once

//==============================================================================
// NAND flash Memory Control Register (NFCTRL) - Read / Write

#define R_NFCTRL				((volatile unsigned int*)0x80000C00)

#define F_NFCTRL_4ECCGEN		( 1<<12)		// 4-bit ECC Generation Enable bit - (0:Disable 1:Enable)
#define F_NFCTRL_EDASEL_LITTLE	( 0<<11)		// Endian Select bit - (0:Little 1:Big)
#define F_NFCTRL_EDASEL_BIG		( 1<<11)		// Endian Select bit - (0:Little 1:Big)
#define F_NFCTRL_DSS_8			( 0<<10)		// Data Swap Size - (0:8bit 1:16bit)
#define F_NFCTRL_DSS_16			( 1<<10)		// Data Swap Size - (0:8bit 1:16bit)
#define F_NFCTRL_DMAWR_CLR		( 0<< 9)		// DMA Write Request bit - (0:Clear 1:Request)
#define F_NFCTRL_DMAWR_REQ		( 1<< 9)		// DMA Write Request bit - (0:Clear 1:Request)
#define F_NFCTRL_DMARR_CLR		( 0<< 8)		// DMA Read Request bit - (0:Clear 1:Request)
#define F_NFCTRL_DMARR_REQ		( 1<< 8)		// DMA Read Request bit - (0:Clear 1:Request)
#define F_NFCTRL_INTBUSY_DIS	( 0<< 7)		// Interrupt Enable at Busyx End - (0:Disable 1:Enable)
#define F_NFCTRL_INTBUSY_EN		( 1<< 7)		// Interrupt Enable at Busyx End - (0:Disable 1:Enable)
#define F_NFCTRL_INTIBUFF_DIS	( 0<< 6)		// Interrupt Enable at Internal Buffer Transfer End - (0:Disable 1:Enable)
#define F_NFCTRL_INTIBUFF_EN	( 1<< 6)		// Interrupt Enable at Internal Buffer Transfer End - (0:Disable 1:Enable)
#define F_NFCTRL_INTBCH_DIS		( 0<< 5)		// BCH ECC Decoding Done Interrupt Enable - (0:Disable 1:Enable)
#define F_NFCTRL_INTBCH_EN		( 1<< 5)		// BCH ECC Decoding Done Interrupt Enable - (0:Disable 1:Enable)
#define F_NFCTRL_TDTSIZE_512B	( 0<< 4)		// Transfer Data Size iin Internal Buffer mode - (0:512Bytes 1:1KBytes(at 2Kbytes Page type))
#define F_NFCTRL_TDTSIZE_1KB	( 1<< 4)		// Transfer Data Size iin Internal Buffer mode - (0:512Bytes 1:1KBytes(at 2Kbytes Page type))
#define F_NFCTRL_N2U_RCPLT		( 0<< 3)		// NAND Flash to Upper Buffer - (0:Read Complete 1:Enable NAND Flash to Upper Buffer Transfer)
#define F_NFCTRL_N2U_EN			( 1<< 3)		// NAND Flash to Upper Buffer - (0:Read Complete 1:Enable NAND Flash to Upper Buffer Transfer)
#define F_NFCTRL_N2L_RCPLT		( 0<< 2)		// NAND Flash to Lower Buffer - (0:Read Complete 1:Enable NAND Flash to Lower Buffer Transfer)
#define F_NFCTRL_N2L_EN			( 1<< 2)		// NAND Flash to Lower Buffer - (0:Read Complete 1:Enable NAND Flash to Lower Buffer Transfer)
#define F_NFCTRL_U2N_WCPLT		( 0<< 1)		// Upper Buffer to NAND Flash - (0:Write Complete 1:Enable Upper Buffer to NAND Flash Transfer)
#define F_NFCTRL_U2N_EN			( 1<< 1)		// Upper Buffer to NAND Flash - (0:Write Complete 1:Enable Upper Buffer to NAND Flash Transfer)
#define F_NFCTRL_L2N_WCPLT		( 0<< 0)		// Lower Buffer to NAND Flash - (0:Write Complete 1:Enable Lower Buffer to NAND Flash Transfer)
#define F_NFCTRL_L2N_EN			( 1<< 0)		// Lower Buffer to NAND Flash - (0:Write Complete 1:Enable Lower Buffer to NAND Flash Transfer)
//==============================================================================
// NAND Flash Memory Command Set Register (NFCMD) - Read / Write

#define R_NFCMD					((volatile unsigned char*)0x80000C04)
	
#define F_NFCMD(x)				( x & 0xFF)
//==============================================================================
// NAND Flash Memory Address Register (NFADR) - Read / Write

#define R_NFADR					((volatile unsigned char*)0x80000C08)

#define F_NFADR(x)				( x & 0xFF)
//==============================================================================
// NAND Flash Memory Data Register (NFCPUDATA) - Read / Write

#define R_NFCPUDATA				((volatile unsigned int*)0x80000C0C)

#define F_NFCPUDATA(x)			( x & 0xFFFFFFFF)
//==============================================================================
// NAND Flash Memory DMA Data Register (NFDMADATA) - Read / Write

#define R_NFDMADATA				((volatile unsigned int*)0x80000C10)

#define F_NFDMADATA(x)			( x & 0xFFFFFFFF)
//==============================================================================
// NAND Flash Memory Operation Status Register (NFSTAT)	- Read Only

#define R_NFSTAT				((volatile unsigned int*)0x80000C14)

#define F_NFSTAT_BCH_DDS		( 1<< 7)		// BCH Decoding Done Status
#define F_NFSTAT_EBC			( 7<< 4)		// Error Bits Count
#define F_NFSTAT_BCH_DR			( 1<< 3)		// BCH Decoding Result - (0:Decoding Failed 1:Decoding Success)
#define F_NFSTAT_OCS			( 1<< 2)		// NAND Write/Read Operation Complete Status - (0:Not yet 1:Completion)
#define F_NFSTAT_NBUSY_LEV		( 1<< 1)		// NAND Flash Memory nBusy Level - (0:Busy 1:Ready)
#define F_NFSTAT_BUSY_RES		( 1<< 0)		// NAND Flash Memory Busy Rising Edege Status - (1:Detect)
//==============================================================================
// NAND Flash Memory ECC(Error Correction Code) Register (NFECC) - Read Only

#define R_NFECC					((volatile unsigned int*)0x80000C18)

#define F_NFECC_ECC2			(0xFF<<16)		// ECC2 (~P4, ~P4', ~P2, ~P2', ~P1, ~P1', ~P2048, ~P2048')
#define F_NFECC_ECC1			(0xFF<< 8)		// ECC1 (~P1024, ~P1024', ~P512, ~P512', ~P256, ~P256', ~P128, ~P128')
#define F_NFECC_ECC0			(0xFF<< 0)		// ECC0 (~P64, ~P64', ~P32, ~P32', ~P16, ~P16', ~P8, ~P8')
//==============================================================================
// NAND Flash Memory Configuration Register (NFCFG) - Read / Write

#define R_NFCFG					((volatile unsigned int*)0x80000C1C)

#define F_NFCFG_RLA(n)			( n<<20)		// Read data Latch timing Adjust bit - (0:Maximum ~ 60MHz 1:40MHz ~ Minimum)
#define F_NFCFG_NCS(n)			( n<<16)		// NDFL_nCS Control - (0:Chip Enable 1:Chip Disable)
#define F_NFCFG_TS(x)			( (x&0x7) <<12)	// Ts : NDFL_ALE/NDFL_CLE Set-up Time - (0:1 Clock ~ 7:8 Clock)
#define F_NFCFG_TWP(x)			( (x&0x7) << 8)	// Twp : NDFL_nWE Pulse Width - (0:1 Clock ~ 7:8 Clock)
#define F_NFCFG_TRP(x)			( (x&0x7) << 4)	// Trp : NDFL_nRE Pulse Width - (0:1 Clock ~ 7:8 Clock)
#define F_NFCFG_TH(x)			( (x&0x7) << 0)	// Th : NDFL_ALE/NDFL_CLE/NDFL_nCS Hold Time - (0:1 Clock ~ 7:8 Clock)
//==============================================================================
// NAND Flash Memory ECC Code for LSN data (NFECCL) - Read Only

#define R_NFECCL				((volatile unsigned int*)0x80000C20)

#define F_NFECCL_S_ECC1			(0xFF<< 8)		// S_ECC1 (1, 1, 1, 1, 1, 1, ~P4_s, ~P4'_s)
#define F_NFECCL_S_ECC0			(0xFF<< 0)		// S_ECC0 (~P2_s, ~P2'_s, ~P1_s, ~P1'_s, ~P16_s, ~P16'_s, ~P8_s, ~P8's)
//==============================================================================
// NAND Flash Memory Lower Buffer Start Address Register (NFLBADR)

#define R_NFLBADR				((volatile unsigned int*)0x80000C24)

#define F_NFLBADR(x)			( x & 0x7FF )	// Lower Buffer Start Address
//==============================================================================
// NAND Flash Memory Upper Buffer Start Address Register (NFUBADR)

#define R_NFUBADR				((volatile unsigned int*)0x80000C2C)

#define F_NFUBADR(x)			( x & 0x7FF )	// Upper Buffer Start Address
//==============================================================================
// NAND Flash Memory MLC ECCx Register (NFECCx) - Read Only

#define R_NFECC0				((volatile unsigned int*)0x80000C34)
#define R_NFECC1				((volatile unsigned int*)0x80000C38)

#define F_NFECC0(x)				( x & 0xFFFFFFFF )	// 4-bit ECC Parity Value Low 32bits value of 52-bit parity
#define F_NFECC1(x)				( x & 0xFFFFF )		// 4-bit ECC Parity Value High 20bits value of 52-bit parity
//==============================================================================
// NAND Flash Memory Error Location x Register (NFERRLOCx) - Read Only

#define R_NFERRLOC0				((volatile unsigned int*)0x80000C40)
#define R_NFERRLOC1				((volatile unsigned int*)0x80000C44)
#define R_NFERRLOC2				((volatile unsigned int*)0x80000C48)
#define R_NFERRLOC3				((volatile unsigned int*)0x80000C4C)

#define F_NFERRLOC0(x)			( x & 0x1FFF )		// 1st Error Byte Location
#define F_NFERRLOC1(x)			( x & 0x1FFF )		// 2nd Error Byte Location
#define F_NFERRLOC2(x)			( x & 0x1FFF )		// 3rd Error Byte Location
#define F_NFERRLOC3(x)			( x & 0x1FFF )		// 4th Error Byte Location
//==============================================================================
// NAND Flash Memory Error Pattern x Register (NFERRPTNx)

#define R_NFERRPTN0				((volatile unsigned int*)0x80000C50)
#define R_NFERRPTN1				((volatile unsigned int*)0x80000C54)
#define R_NFERRPTN2				((volatile unsigned int*)0x80000C58)
#define R_NFERRPTN3				((volatile unsigned int*)0x80000C5C)

#define F_NFERRPTN0				( x & 0xFF )		// 1st Error Byte Pattern
#define F_NFERRPTN1				( x & 0xFF )		// 2nd Error Byte Pattern
#define F_NFERRPTN2				( x & 0xFF )		// 3rd Error Byte Pattern
#define F_NFERRPTN3				( x & 0xFF )		// 4th Error Byte Pattern
//==============================================================================
// NAND Command Define

#define NAND_COMM_READ1			0x00	// NAND Read 1st Cycle
#define NAND_COMM_READ1_2		0x01	// NAND Read 2nd harf register
#define NAND_COMM_READ2			0x30	// NAND Read 2st Cycle
#define NAND_COMM_SPAREREAD		0x50	// NAND Read spare area
#define NAND_COMM_RID			0x90	// NAND Read ID
#define NAND_COMM_RST			0xff	// NAND Reset
#define NAND_COMM_PPRO1			0x80	// NAND Page Program 1st Cycle
#define NAND_COMM_PPRO2			0x10	// NAND Page Program 2st Cycle
#define NAND_COMM_BERS1			0x60	// NAND Block Erase 1st Cycle
#define NAND_COMM_BERS2			0xd0	// NAND Block Erase 2st Cycle
#define NAND_COMM_IRD			0x85	// NAND Random Data Input
#define NAND_COMM_OWD1			0x05	// NAND Random Data Output 1st Cycle
#define NAND_COMM_OWD2			0xe0	// NAND Random Data Output 2st Cycle
#define NAND_COMM_STATUS		0x70	// NAND Status Read
#define NAND_RESULT_PASS		0		// NAND Result Pass
#define NAND_RESULT_FAIL		1		// NAND Result Fail

#define BI_GOOD					0xff	//not used block
#define BI_USED					0x7f	//allocated block
#define BI_NOTUSED				0x0f	//need for erase to check bad?
#define BI_BAD 					0x00	//can't use

#define MAX_PAGE_SIZE 			512*4
#define MAX_SPARE_SIZE 			16*4
#define MAX_BLOCK_CNT 			4096
// #define MAX_BLOCK_CNT 			8192	// K9F8G08	it's too big

/* Cell info constants */
#define NAND_CI_CHIPNR_MSK      0x03
#define NAND_CI_CELLTYPE_MSK    0x0C

/*
* NAND Flash Manufacturer ID Codes
*/
#define NAND_MFR_TOSHIBA        0x98
#define NAND_MFR_SAMSUNG        0xec
#define NAND_MFR_FUJITSU        0x04
#define NAND_MFR_NATIONAL       0x8f
#define NAND_MFR_RENESAS        0x07
#define NAND_MFR_STMICRO        0x20
#define NAND_MFR_HYNIX          0xad
#define NAND_MFR_MICRON         0x2c
#define NAND_MFR_AMD            0x01
#define NAND_MFR_MACRONIX       0xc2
#define NAND_MFR_EON            0x92
#define NAND_MFR_EON2           0xC8
#define NAND_MFR_ATO            0x9b

// For ECC
#define sP16(val)				((val&(1<<3)) ? (1<<4) : 0 )
#define sP8(val)				((val&(1<<1)) ? (1<<3) : 0 )
#define sP4(val)				((val&(1<<9)) ? (1<<2) : 0 )
#define sP2(val)				((val&(1<<7)) ? (1<<1) : 0 )
#define sP1(val)				((val&(1<<5)) ? (1<<0) : 0 )

#define SErr_Position(val)		(sP16(val)|sP8(val)|sP4(val)|sP2(val)|sP1(val))

struct nand_manufacturers
{
	int id;
	char *name;
};

struct nand_flash_dev
{
	char *name;
	int id;
	unsigned long pagesize;
	unsigned long chipsize;		//Mega-bytes
	unsigned long erasesize;	//block size
};

struct nand_buffer
{
	U32 current;
	U32 head;
	U32 tail;
};

#define BLOCK2PAGE(phyblocknum) (phyblocknum * NandInfo.pageperblock)

typedef struct _tagNand
{
	U8 maf_id;
	U8 dev_id;
	U8 cellinfo;
	U8 eccbit;
	U32 pagesize;
	U32 blocksize;
	U32 pageperblock;
	U32 blockcnt;
	U32 sparesize;
	U32 maxbadblockcnt;
	U32 rowaddrlen;
	U32 datastartblock; //file system area
	U32 datablockcnt;
} NANDTYPE;

typedef struct _tagSpare
{
	// Small Page Spare 16bytes
	
	U8 LSN[3];			// [ 0: 2]
	U8 paritybit;		// [ 3]
	U8 reserve;			// [ 4]
	U8 BadInfo;			// [ 5]
#if NAND_ECC == 1
	U8 Ecc[3];			// [ 6: 8]
	U8 SEcc[2];			// [ 9:10]
	U8 reserve2[4];		// [11:14]
	U8 ECC_set;			// [15]
#else
	U8 Ecc[7];			// [ 6:12]
	U8 SEcc[2];			// [13:14]
	U8 ECC_set;			// [15]
#endif
	
}SPAREDATA;

typedef struct _tagSpare2
{
	// Large Page Spare 64bytes
	
	U8 BadInfo;			// [ 0]
	U8 paritybit;		// [ 1] reserve is used parity
	U8 LSN[3];			// [ 2: 4] Logical Number
	U8 reserve[3];		// [ 5: 7]
	U8 Ecc[3];			// [ 8:10]
	U8 SEcc[2];			// [11:12]
	U8 reserve2[3];		// [13:15]
		
	U8 Ecc1[7];			// [16:22] 1 - 512bytes
	U8 Ecc2[7];			// [23:29] 2 - 512bytes
	U8 Ecc3[7];			// [30:36] 3 - 512bytes
	U8 Ecc4[7];			// [37:43] 4 - 512bytes
	U8 reserve3[19];	// [44:62]
	U8 ECC_set;			// [63]
}SPAREDATA2;

//==============================================================================
// NAND Physical Function

BOOL nand_phy_eraseblock(U32 phyblocknum);

BOOL nand_phy_readpage(U32 phypageaddr, void* pagebuf);
BOOL nand_phy_writepage(U32 phypageaddr, void* pagebuf);
BOOL nand_phy_writepage_spare(U32 phypageaddr, void* pagebuf, BOOL blsn_write);
#if ECC_TEST == 1
BOOL nand_phy_writepage_wrong(U32 phypageaddr, void* pagebuf, void* sparebuf);
BOOL nand_phy_writepage_spare_wrong(U32 phypageaddr, void* pagebuf, void* sparebuf);
#endif
BOOL nand_phy_readspare(U32 phypageaddr, void* sparebuf);
BOOL nand_phy_readspare_secc(U32 phypageaddr, void* sparebuf, BOOL secc_set);
BOOL nand_phy_writespare(U32 phypageaddr, void* sparebuf);
#if ECC_TEST == 1
BOOL nand_phy_writespare_wrong(U32 phypageaddr, void* sparebuf);
#endif

BOOL nand_phy_readsect(U32 sectnum, void* sectbuf);	// 512 Bytes For FAT

//==============================================================================
// NAND Logical Function
	
BOOL nand_init(void);

U32 nand_eraseblock(U32 logblocknum);

BOOL nand_readpage(U32 logpageaddr, void* pagebuf);
BOOL nand_writepage(U32 logpageaddr, void* pagebuf);

BOOL nand_set_sparedata(U32 logpageaddr);
void nand_flushdata(void);

//==============================================================================
// NAND Flash Fat Function

BOOL nand_readsect(U32 sectnum, void* sectbuf);
BOOL nand_readsects(U32 sectnum, void* sectbuf, U32 sectcnt);
BOOL nand_updatesects(U32 sectnum, void* sectbuf, U32 sectcnt);

//==============================================================================
// NAND Flash Specification Function

void nand_set_cfg(U32 cfg);

U32 nand_get_blockcnt(void);
U32 nand_get_blocksize(void);
U32 nand_get_pageperblock(void);

U32 nand_get_pagesize(void);
U32 nand_get_sparesize(void);
U32 nand_get_sectorcount(void);//sector is 512
U32 nand_get_memsize_kbyte(void);
U32 nand_get_phy_block(U32 logblocknum);
U32 nand_get_phy_page(U32 logpageaddr);

//==============================================================================
