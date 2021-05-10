#include <string.h>
#include "cantus_lib_config.h"
#include "cantus.h"

//#define NAND_DEBUG
#ifdef NAND_DEBUG
#define NAND_DEBUGPRINTF	debugprintf
#else
#define NAND_DEBUGPRINTF(...)
#endif

#if NAND_ECC != 0
#if NAND_ECC == 4
#warning "4bit NAND_ECC Included"

#else
#error "Only 4bit NAND_ECC Support"

#endif
#endif

//==============================================================================
// NAND Flash Variable

struct nand_manufacturers nand_manuf_ids[] =
{
	{NAND_MFR_SAMSUNG, "Samsung"},
	{NAND_MFR_AMD, "AMD/Spansion"},
#if 0
	{NAND_MFR_TOSHIBA, "Toshiba"},
	{NAND_MFR_FUJITSU, "Fujitsu"},
	{NAND_MFR_NATIONAL, "National"},
	{NAND_MFR_RENESAS, "Renesas"},
	{NAND_MFR_STMICRO, "ST Micro"},
	{NAND_MFR_HYNIX, "Hynix"},
	{NAND_MFR_MICRON, "Micron"},
	{NAND_MFR_MACRONIX, "Macronix"},
	{NAND_MFR_EON, "Eon"},
	{NAND_MFR_EON2, "Eon"},
	{NAND_MFR_ATO, "ATO"},
#endif
	{0x0, "Unknown"}
};

struct nand_flash_dev nand_flash_ids[] =
{
#if 0
	{"16MiB 1,8V 8-bit",       0x33, 512, 16, 0x4000},
	{"16MiB 3,3V 8-bit",       0x73, 512, 16, 0x4000},
	{"16MiB 1,8V 16-bit",      0x43, 512, 16, 0x4000},
	{"16MiB 3,3V 16-bit",      0x53, 512, 16, 0x4000},
#endif
	
#if 0
	{"32MiB 1,8V 8-bit",       0x35, 512, 32, 0x4000},
	{"32MiB 3,3V 8-bit",       0x75, 512, 32, 0x4000},
	{"32MiB 1,8V 16-bit",      0x45, 512, 32, 0x4000},
	{"32MiB 3,3V 16-bit",      0x55, 512, 32, 0x4000},
#endif
	
#if 1
	{"64MiB 1,8V 8-bit",       0x36, 512, 64, 0x4000},
	{"64MiB 3,3V 8-bit",       0x76, 512, 64, 0x4000},		// K9F1208
	{"64MiB 1,8V 16-bit",      0x46, 512, 64, 0x4000},
	{"64MiB 3,3V 16-bit",      0x56, 512, 64, 0x4000},
#endif
	
#if 0
	{"128MiB 1,8V 8-bit",      0x78, 512, 128, 0x4000},
	{"128MiB 1,8V 8-bit",      0x39, 512, 128, 0x4000},
	{"128MiB 3,3V 8-bit",      0x79, 512, 128, 0x4000},
	{"128MiB 1,8V 16-bit",     0x72, 512, 128, 0x4000},
	{"128MiB 1,8V 16-bit",     0x49, 512, 128, 0x4000},
	{"128MiB 3,3V 16-bit",     0x74, 512, 128, 0x4000},
	{"128MiB 3,3V 16-bit",     0x59, 512, 128, 0x4000},
#endif
	
#if 0
	{"256MiB 3,3V 8-bit",      0x71, 512, 256, 0x4000},
#endif
	
#if 0
	/*
	* These are the new chips with large page size. The pagesize and the
	* erasesize is determined from the extended id bytes
	*/

	/* 512 Megabit */
	{"64MiB 1,8V 8-bit",       0xA2, 0,  64, 0, },
	{"64MiB 1,8V 8-bit",       0xA0, 0,  64, 0, },
	{"64MiB 3,3V 8-bit",       0xF2, 0,  64, 0, },
	{"64MiB 3,3V 8-bit",       0xD0, 0,  64, 0, },
	{"64MiB 3,3V 8-bit",       0xF0, 0,  64, 0, },
	{"64MiB 1,8V 16-bit",      0xB2, 0,  64, 0, },
	{"64MiB 1,8V 16-bit",      0xB0, 0,  64, 0, },
	{"64MiB 3,3V 16-bit",      0xC2, 0,  64, 0, },
	{"64MiB 3,3V 16-bit",      0xC0, 0,  64, 0, },
#endif
	
#if 0
	/* 1 Gigabit */
	{"128MiB 1,8V 8-bit",      0xA1, 0, 128, 0, },
	{"128MiB 3,3V 8-bit",      0xF1, 0, 128, 0, },		// K9F1G08, S34ML01G, EN27N1G08
	{"128MiB 3,3V 8-bit",      0xD1, 0, 128, 0, },
	{"128MiB 1,8V 16-bit",     0xB1, 0, 128, 0, },
	{"128MiB 3,3V 16-bit",     0xC1, 0, 128, 0, },
	{"128MiB 1,8V 16-bit",     0xAD, 0, 128, 0, },
#endif
	
#if 0
	/* 2 Gigabit */
	{"256MiB 1,8V 8-bit",      0xAA, 0, 256, 0, },
	{"256MiB 3,3V 8-bit",      0xDA, 0, 256, 0, },		// K9F2G08, S34ML02G, EN27N2G08
	{"256MiB 1,8V 16-bit",     0xBA, 0, 256, 0, },
	{"256MiB 3,3V 16-bit",     0xCA, 0, 256, 0, },
#endif
	
#if 0
	/* 4 Gigabit */
	{"512MiB 1,8V 8-bit",      0xAC, 0, 512, 0, },
	{"512MiB 3,3V 8-bit",      0xDC, 0, 512, 0, },		// K9F4G08, S34ML04G, EN27N4G08
	{"512MiB 1,8V 16-bit",     0xBC, 0, 512, 0, },
	{"512MiB 3,3V 16-bit",     0xCC, 0, 512, 0, },
#endif
	
#if 0
	/* 8 Gigabit */
	{"1GiB 1,8V 8-bit",        0xA3, 0, 1024, 0,},
	{"1GiB 3,3V 8-bit",        0xD3, 0, 1024, 0,},		// K9F8G08, S34ML08G
	{"1GiB 1,8V 16-bit",       0xB3, 0, 1024, 0,},
	{"1GiB 3,3V 16-bit",       0xC3, 0, 1024, 0,},
#endif
	
#if 0
	/* 16 Gigabit */
	{"2GiB 1,8V 8-bit",        0xA5, 0, 2048, 0,},
	{"2GiB 3,3V 8-bit",        0xD5, 0, 2048, 0,},
	{"2GiB 1,8V 16-bit",       0xB5, 0, 2048, 0,},
	{"2GiB 3,3V 16-bit",       0xC5, 0, 2048, 0,},
#endif
	
#if 0
	/* 32 Gigabit */
	{"4GiB 1,8V 8-bit",        0xA7, 0, 4096, 0,},
	{"4GiB 3,3V 8-bit",        0xD7, 0, 4096, 0,},
	{"4GiB 1,8V 16-bit",       0xB7, 0, 4096, 0,},
	{"4GiB 3,3V 16-bit",       0xC7, 0, 4096, 0,},
#endif
	
#if 0
	/* 64 Gigabit */
	{"8GiB 1,8V 8-bit",        0xAE, 0, 8192, 0,},
	{"8GiB 3,3V 8-bit",        0xDE, 0, 8192, 0,},
	{"8GiB 1,8V 16-bit",       0xBE, 0, 8192, 0,},
	{"8GiB 3,3V 16-bit",       0xCE, 0, 8192, 0,},
#endif
	
#if 0
	/* 128 Gigabit */
	{"16GiB 1,8V 8-bit",       0x1A, 0, 16384, 0},
	{"16GiB 3,3V 8-bit",       0x3A, 0, 16384, 0},
	{"16GiB 1,8V 16-bit",      0x2A, 0, 16384, 0},
	{"16GiB 3,3V 16-bit",      0x4A, 0, 16384, 0},
#endif
	
#if 0
	/* 256 Gigabit */
	{"32GiB 1,8V 8-bit",       0x1C, 0, 32768, 0},
	{"32GiB 3,3V 8-bit",       0x3C, 0, 32768, 0},
	{"32GiB 1,8V 16-bit",      0x2C, 0, 32768, 0},
	{"32GiB 3,3V 16-bit",      0x4C, 0, 32768, 0},
#endif
	
#if 0
	/* 512 Gigabit */
	{"64GiB 1,8V 8-bit",       0x1E, 0, 65536, 0},
	{"64GiB 3,3V 8-bit",       0x3E, 0, 65536, 0},
	{"64GiB 1,8V 16-bit",      0x2E, 0, 65536, 0},
	{"64GiB 3,3V 16-bit",      0x4E, 0, 65536, 0},
#endif
	{NULL,}
};

static NANDTYPE NandInfo;
struct nand_buffer NandBuffer;

static U8 BlockInfoTable[MAX_BLOCK_CNT];
static U16 PhyBlockTable[MAX_BLOCK_CNT];
static U16 LogBlockTable[MAX_BLOCK_CNT];
static U8 sparebuf_for_lognum[MAX_SPARE_SIZE];

volatile static U32 g_dummy;		//for dummy read
static U32 nandcfg = F_NFCFG_TS(7) | F_NFCFG_TWP(7) | F_NFCFG_TRP(7) | F_NFCFG_TH(7);

//==============================================================================
// NAND Flash Static Function Prototype

static U32 nand_make_bbt(void);
static void WaitBusy(void);

static BOOL nand_allocate_block(U32 phynum, U32 lognum);
static int nand_get_phy_freeblock(U32 logblocknum);

static void make_logical_data(int logblocknum);
static int nand_id_has_period(u8 *id_data, int arrlen, int period);
static int nand_id_len(U8* id_data, int arrlen);
static void decode_ext_id(U8 id_data[8]);
static void decode_id(U8 id_data[8], struct nand_flash_dev* type);
#if NAND_ECC != 0
static U16 EccCodeForLSN(U8 first, U8 second, U8 third);
#endif

__inline__ static void nand_reset(void);
__inline__ static void setpageaddress(U32 phypageaddr, U32 offset);
__inline__ static void seteraseblockaddr(U32 phypageaddr);
__inline__ static void setspareaddr(U32 phypageaddr);
__inline__ static void setspareaddr_offset(U32 phypageaddr, U32 offset);
__inline__ static U32 nand_get_log_block(U32 phyblocknum);

//==============================================================================
// NAND Physical Function

BOOL nand_phy_eraseblock(U32 phyblocknum)
{
	U32 phypageaddr;
	
	phypageaddr = BLOCK2PAGE(phyblocknum);
	g_dummy = *R_NFSTAT;	//clear busy
	
	*R_NFCMD = NAND_COMM_BERS1;
	
	seteraseblockaddr(phypageaddr);
	
	*R_NFCMD = NAND_COMM_BERS2;
	
	WaitBusy();

	*R_NFCMD = NAND_COMM_STATUS;
	
	if(*(volatile U8*)R_NFCPUDATA & NAND_RESULT_FAIL)
	{
		return FALSE;
	}
	
	return TRUE;
}

BOOL nand_phy_readpage(U32 phypageaddr, void* pagebuf)
{
	U32 i, j;
	U8* ppagebuf = (U8*)pagebuf;
	
#if NAND_ECC == 4	
	
	U32 k;
	U32 error_cnt;
	U8 spare_buffer[MAX_SPARE_SIZE];
	U8 ECC_set;
#endif
	U8 page_buffer[512];
	
#if NAND_ECC == 4
	
	nand_phy_readspare(phypageaddr, spare_buffer);
	ECC_set = spare_buffer[NandInfo.sparesize-1];
	
	if(NandInfo.pagesize == 512)
	{
		k = 6;
	}
	else
	{
		k = 16;
	}
#endif
	
	for(i=0; i<NandInfo.pagesize/512; i++)
	{
		g_dummy = *R_NFSTAT;
		
		*R_NFCMD = NAND_COMM_READ1;
		setpageaddress(phypageaddr, i*512);
		
		if(NandInfo.pagesize == 2048)
		{
			*R_NFCMD = NAND_COMM_READ2;
		}
		WaitBusy();
		
#if NAND_ECC == 4
		
		g_dummy = *R_NFECC1;
		*R_NFCTRL |= F_NFCTRL_4ECCGEN;
#endif
		if((U32)(&page_buffer[0]) & 3)
		{
			for(j=0; j<512;)
			{
				page_buffer[j] = *(volatile U8*)R_NFCPUDATA;
				page_buffer[j+1] = *(volatile U8*)R_NFCPUDATA;
				page_buffer[j+2] = *(volatile U8*)R_NFCPUDATA;
				page_buffer[j+3] = *(volatile U8*)R_NFCPUDATA;
				j+=4;
			}
		}
		else
		{
			for(j=0; j<512;)
			{
				*(U32*)(page_buffer+j) = *R_NFCPUDATA;
				*(U32*)(page_buffer+j+4) = *R_NFCPUDATA;
				*(U32*)(page_buffer+j+8) = *R_NFCPUDATA;
				*(U32*)(page_buffer+j+12) = *R_NFCPUDATA;
				j+=16;
			}		
		}
		
#if NAND_ECC == 4
		
		g_dummy = *R_NFSTAT;
		
		if(NandInfo.pagesize == 512)
		{
			*R_NFCMD = NAND_COMM_SPAREREAD;
		}
		else
		{			
			*R_NFCMD = NAND_COMM_READ1;
		}
		
		setspareaddr_offset(phypageaddr, k);
		
		if(NandInfo.pagesize == 2048)
		{
			*R_NFCMD = NAND_COMM_READ2;
		}
		WaitBusy();
		
		for(j=k; j<k+7; j++)
		{
			spare_buffer[j] = *(volatile U8*)R_NFCPUDATA;
		}
		
		do
		{
			g_dummy = *R_NFSTAT;
		}	
		while(!(F_NFSTAT_BCH_DDS == (g_dummy&F_NFSTAT_BCH_DDS)));
		
		*R_NFCTRL &= ~(F_NFCTRL_4ECCGEN);
		
		k+=7;
		
		if(ECC_set != 0xFF && ECC_set != 0x0)
		{
			if((ECC_set & 0x8) == 0x8)
			{
				if(F_NFSTAT_BCH_DR == (g_dummy&F_NFSTAT_BCH_DR) )
				{
					error_cnt = ((g_dummy&F_NFSTAT_EBC)>>4);
					if(error_cnt > 4)
					{
						// it's impossible
						debugprintf("Ecnt %d \r\n",error_cnt);
						while(1);
					}
					
					if(0 != error_cnt)
					{
						for(j=0;j<4;j++)
						{
							debugprintf("\r\nIn %xth Byte Error",*(R_NFERRLOC0+j));
							debugprintf("(0x%02x -->",page_buffer[*(R_NFERRLOC0+j)]);
							
							page_buffer[*(R_NFERRLOC0+j)]=page_buffer[*(R_NFERRLOC0+j)]^*(R_NFERRPTN0+j);
													
							debugprintf(" 0x%02x)\r\n",page_buffer[*(R_NFERRLOC0+j)]);
							
							if(*(R_NFERRLOC0+j+1)==0)
							{
								break;
							}
						}
					}
				}
				else
				{
					error_cnt = ((g_dummy&F_NFSTAT_EBC)>>4);
					debugprintf("BCH Decoding Fail / error_cnt = %d\r\n",error_cnt);
					// printmem(spare_buffer, NandInfo.sparesize);
				}					
			}
			ECC_set<<=1;
		}		
#endif
		memcpy(ppagebuf+(i*512), page_buffer, 512);
	}
	
	while (!(*(volatile U8*)R_NFSTAT & F_NFSTAT_NBUSY_LEV)); // for "trr" timing,see Nand-Flash data sheet
	
	return TRUE;
}

BOOL nand_phy_writepage(U32 phypageaddr, void* pagebuf)
{
	U32 i;
	U32 phynum = phypageaddr / NandInfo.pageperblock;
	
	for(i=0; i<MAX_SPARE_SIZE; i++)
	{
		sparebuf_for_lognum[i] = 0xFF;
	}
	
	if((phynum >= NandInfo.datastartblock) && ((phypageaddr % NandInfo.pageperblock) == 0))
	{
		U32 lognum = nand_get_log_block(phynum);
		
		if(lognum == 0xFFFF)
		{
			return nand_phy_writepage_spare(phypageaddr, pagebuf, FALSE);
		}
		
		make_logical_data(lognum);
		
		return nand_phy_writepage_spare(phypageaddr, pagebuf, TRUE);
	}
	
	return nand_phy_writepage_spare(phypageaddr, pagebuf, FALSE);
}

BOOL nand_phy_writepage_spare(U32 phypageaddr, void* pagebuf, BOOL blsn_write)
{
	U32 i, j;
	U8* ppagebuf = (U8*)pagebuf;
	
#if NAND_ECC == 4
	
	U32 k;
	U32 temp_bch[2*4];
	U8* ECC_set;
#endif
	
	g_dummy = *R_NFSTAT; //clear busy
	
	if(NandInfo.pagesize == 512)
	{
		*R_NFCMD = 0x0;
	}
	*R_NFCMD = NAND_COMM_PPRO1;

	setpageaddress(phypageaddr, 0);
	
#if NAND_ECC == 4
	
	k=0;
	ECC_set = &sparebuf_for_lognum[NandInfo.sparesize-1];
	*ECC_set = 0;
#endif
	
	for(i=0; i<NandInfo.pagesize/512; i++)
	{
#if NAND_ECC == 4
		
		temp_bch[k+1] = *R_NFECC1;
		*R_NFCTRL |= F_NFCTRL_4ECCGEN;
		*ECC_set <<= 1;
#endif
		if((U32)(&ppagebuf[0])&3)
		{
			for(j=0; j<512; )
			{
				*(volatile U8*)R_NFCPUDATA = ppagebuf[(i*512)+j];
				*(volatile U8*)R_NFCPUDATA = ppagebuf[(i*512)+j+1];
				*(volatile U8*)R_NFCPUDATA = ppagebuf[(i*512)+j+2];
				*(volatile U8*)R_NFCPUDATA = ppagebuf[(i*512)+j+3];
				j+=4;
			}
		}
		else
		{
			U32* lppagebuf = (U32*)&ppagebuf[0];
			for(j=0; j<512/4; )
			{
				*R_NFCPUDATA = lppagebuf[(i*512/4)+j];
				*R_NFCPUDATA = lppagebuf[(i*512/4)+j+1];
				*R_NFCPUDATA = lppagebuf[(i*512/4)+j+2];
				*R_NFCPUDATA = lppagebuf[(i*512/4)+j+3];
				j+=4;
			}
		}
#if NAND_ECC == 4
		
		temp_bch[k] = *R_NFECC0;
		temp_bch[k+1] = *R_NFECC1;
		
		if(temp_bch[k] != 0x9ab9d49d && temp_bch[k+1] != 0x97387)
		{
			*ECC_set += 1;
		}
		
		k+=2;
		*R_NFCTRL &= 0xFFF;
#endif		
	}
	
#if NAND_ECC == 4
	
	if(NandInfo.pagesize == 512)
	{
		k = 6;
		j = 2;
		*ECC_set <<= 3;
	}
	else
	{
		k = 16;
		j = 2*4;
	}
	
	for(i=0; i<j; i+=2)
	{
		sparebuf_for_lognum[k] = (U8)temp_bch[i];
		sparebuf_for_lognum[k+1] = (U8)(temp_bch[i]>>8);
		sparebuf_for_lognum[k+2] = (U8)(temp_bch[i]>>16);
		sparebuf_for_lognum[k+3] = (U8)(temp_bch[i]>>24);
		
		sparebuf_for_lognum[k+4] = (U8)temp_bch[i+1];
		sparebuf_for_lognum[k+5] = (U8)(temp_bch[i+1]>>8);
		sparebuf_for_lognum[k+6] = (U8)(temp_bch[i+1]>>16);
		k+=7;
	}
#endif
	
#if NAND_ECC == 4
	for(i=0; i<NandInfo.sparesize; i++)
	{
		*(volatile U8*)R_NFCPUDATA = sparebuf_for_lognum[i];
	}
#else
	if(blsn_write)
	{
		for(i = 0; i < NandInfo.sparesize; i++)
		{
			*(volatile U8*)R_NFCPUDATA = sparebuf_for_lognum[i];
		}
	}
#endif
	
	*R_NFCMD = NAND_COMM_PPRO2;
	
	WaitBusy();

	*R_NFCMD = NAND_COMM_STATUS;
	
	if(*(volatile U8*)R_NFCPUDATA & NAND_RESULT_FAIL)
	{
		return FALSE;
	}
	
	return TRUE;
}

#if ECC_TEST == 1
#warning "ECC_TEST Included"
BOOL nand_phy_writepage_wrong(U32 phypageaddr, void* pagebuf, void* sparebuf)
{
	U32 i;
	
	for(i=0; i<MAX_SPARE_SIZE; i++)
	{
		sparebuf_for_lognum[i] = 0xFF;
	}
	
	return nand_phy_writepage_spare_wrong(phypageaddr, pagebuf, sparebuf);
}
BOOL nand_phy_writepage_spare_wrong(U32 phypageaddr, void* pagebuf, void* sparebuf)
{
	U32 i, j;
	U8* ppagebuf = (U8*)pagebuf;
	U8* psparebuf = (U8*)sparebuf;
	
#if NAND_ECC == 4
	
	U32 k;
	U32 temp_bch[2*4];
	U8* ECC_set;
#endif
	
	g_dummy = *R_NFSTAT; //clear busy
	
	if(NandInfo.pagesize == 512)
	{
		*R_NFCMD = 0x0;
	}
	*R_NFCMD = NAND_COMM_PPRO1;

	setpageaddress(phypageaddr, 0);
	
#if NAND_ECC == 4
	
	k=0;
	ECC_set = &sparebuf_for_lognum[NandInfo.sparesize-1];
	*ECC_set = 0;
#endif
	
	for(i=0; i<NandInfo.pagesize/512; i++)
	{
#if NAND_ECC == 4
		
		temp_bch[k+1] = *R_NFECC1;
		*R_NFCTRL |= F_NFCTRL_4ECCGEN;
		*ECC_set <<= 1;		
#endif
		if((U32)(&ppagebuf[0])&3)
		{
			for(j=0; j<512; )
			{
				*(volatile U8*)R_NFCPUDATA = ppagebuf[(i*512)+j];
				*(volatile U8*)R_NFCPUDATA = ppagebuf[(i*512)+j+1];
				*(volatile U8*)R_NFCPUDATA = ppagebuf[(i*512)+j+2];
				*(volatile U8*)R_NFCPUDATA = ppagebuf[(i*512)+j+3];
				j+=4;
			}
		}
		else
		{
			U32* lppagebuf = (U32*)&ppagebuf[0];
			for(j=0; j<512/4; )
			{
				*R_NFCPUDATA = lppagebuf[(i*512/4)+j];
				*R_NFCPUDATA = lppagebuf[(i*512/4)+j+1];
				*R_NFCPUDATA = lppagebuf[(i*512/4)+j+2];
				*R_NFCPUDATA = lppagebuf[(i*512/4)+j+3];
				j+=4;
			}
		}
#if NAND_ECC == 4
		
		temp_bch[k] = *R_NFECC0;
		temp_bch[k+1] = *R_NFECC1;
		
		// debugprintf("temp_bch[%d] = 0x%x\r\n", k, temp_bch[k]);
		// debugprintf("temp_bch[%d] = 0x%x\r\n", k+1, temp_bch[k+1]);		
		if(temp_bch[k] != 0x9ab9d49d && temp_bch[k+1] != 0x97387)
		{
			*ECC_set += 1;
		}
		
		k+=2;
		*R_NFCTRL &= 0xFFF;
#endif		
	}
	
#if NAND_ECC == 4
	
#if 1
	for(i=0; i<NandInfo.sparesize; i++)
	{
		sparebuf_for_lognum[i] = psparebuf[i];
	}
#else
	k=16;
	for(i=0; i<2*4; i+=2)
	{
		sparebuf_for_lognum[k] = (U8)temp_bch[i];
		sparebuf_for_lognum[k+1] = (U8)(temp_bch[i]>>8);
		sparebuf_for_lognum[k+2] = (U8)(temp_bch[i]>>16);
		sparebuf_for_lognum[k+3] = (U8)(temp_bch[i]>>24);
		
		sparebuf_for_lognum[k+4] = (U8)temp_bch[i+1];
		sparebuf_for_lognum[k+5] = (U8)(temp_bch[i+1]>>8);
		sparebuf_for_lognum[k+6] = (U8)(temp_bch[i+1]>>16);
		k+=7;
	}
#endif
#endif
	
#if NAND_ECC == 4
	for(i=0; i<NandInfo.sparesize; i++)
	{
		*(volatile U8*)R_NFCPUDATA = sparebuf_for_lognum[i];
	}
#endif
	
	*R_NFCMD = NAND_COMM_PPRO2;
	
	WaitBusy();

	*R_NFCMD = NAND_COMM_STATUS;
	
	if(*(volatile U8*)R_NFCPUDATA & NAND_RESULT_FAIL)
	{
		return FALSE;
	}
	
	return TRUE;
}
#endif

BOOL nand_phy_readspare(U32 phypageaddr, void *sparebuf)
{
	U32 phynum = phypageaddr / NandInfo.pageperblock;
	
	if((phynum >= NandInfo.datastartblock) && ((phypageaddr % NandInfo.pageperblock) == 0))
	{
		return nand_phy_readspare_secc(phypageaddr, sparebuf, TRUE);
	}
	
	return nand_phy_readspare_secc(phypageaddr, sparebuf, FALSE);
}

BOOL nand_phy_readspare_secc(U32 phypageaddr, void *sparebuf, BOOL secc_set)
{
	U32 i;
	U8* psparebuf = (U8*)sparebuf;
	
#if NAND_ECC != 0
	
	U32 SEcc1;
	U32 SEcc2;	
	U32 result;	
	U32 parity_count;
	U32 position_erra;
#endif	
	
 	g_dummy = *R_NFSTAT; //clear busy

	if(NandInfo.pagesize == 512)
	{
		*R_NFCMD = NAND_COMM_SPAREREAD;
		setspareaddr(phypageaddr);
	}
	else
	{
		*R_NFCMD = NAND_COMM_READ1;
		setspareaddr(phypageaddr);
		
		*R_NFCMD = NAND_COMM_READ2;
	}
	
	WaitBusy();
	
#if NAND_ECC == 0
	
	for(i=0; i<NandInfo.sparesize; i++)
	{
		psparebuf[i] = *(volatile U8*)R_NFCPUDATA;
	}
	
	while (!(*(volatile U8*)R_NFSTAT & F_NFSTAT_NBUSY_LEV)); // for "trr" timing,see Nand-Flash data sheet
	
	return TRUE;
#else
	
	if(secc_set == FALSE)
	{
		for(i=0; i<NandInfo.sparesize; i++)
		{
			psparebuf[i] = *(volatile U8*)R_NFCPUDATA;
		}
		
		while (!(*(volatile U8*)R_NFSTAT & F_NFSTAT_NBUSY_LEV)); // for "trr" timing,see Nand-Flash data sheet
		
		return TRUE;
	}
#endif
	
#if NAND_ECC != 0
	
	if(NandInfo.pagesize == 512)
	{
#if 0
		// Manual ECC
		
		for(i=0; i<NandInfo.sparesize; i++)
		{
			psparebuf[i] = *(volatile U8*)R_NFCPUDATA;
		}
		
		SPAREDATA* sparedata = (SPAREDATA*)buf;
		SEcc1 = EccCodeForLSN(sparedata->LSN[0],sparedata->LSN[1],sparedata->LSN[2]);
#else
		SEcc1 = *R_NFECC;	// ECC Clear

		i=0;
		psparebuf[i++] = *(volatile U8*)R_NFCPUDATA;
		psparebuf[i++] = *(volatile U8*)R_NFCPUDATA;
		psparebuf[i++] = *(volatile U8*)R_NFCPUDATA;
		
		SEcc1 = (*R_NFECCL&0xFFFF);
		//PRINTVAR(SEcc1);
		
		for(;i<NandInfo.sparesize;i++)
		{
			psparebuf[i] = *(volatile U8*)R_NFCPUDATA;
		}

		SPAREDATA* sparedata = (SPAREDATA*)psparebuf;	
#endif
		if(sparedata->LSN[0] != 0xff && sparedata->LSN[1] != 0xff && sparedata->LSN[2] != 0xff)
		{
			SEcc2 = (U16)(sparedata->SEcc[0]|((sparedata->SEcc[1])<<8));
			result = SEcc1 ^ SEcc2;
			
			if(result != 0)
			{
				parity_count=0;
				
				for(i=0;i<10;i++)
				{
					if(result & (1<<i))
					{
						parity_count++;
					}
				}
				
				if(parity_count==1)
				{
					debugprintf("'1' is %d\r\n",parity_count);
					debugstring("Original ECC Parity 1bit Error\r\n");
				}
				else if(parity_count==5)
				{	
					debugstring("1 bit Error \r\n");
					
					position_erra = SErr_Position(result);
					debugprintf("0x%x ==> ",sparedata->LSN[position_erra/8]);
					
					sparedata->LSN[position_erra/8] = sparedata->LSN[position_erra/8] ^ (1<<(position_erra%8));
					
					debugprintf("0x%x\r\n",sparedata->LSN[position_erra/8]);
					
					// printmem(buf,NandInfo.sparesize);
				}
				else
				{
					debugprintf("'1' is %d\r\n",parity_count);
					debugstring("Multiple error\r\n");
				}
			}
			else
			{
				// debugstring("No error\r\n");
				// debugprintf("Page %d is valid Data\r\n",phypageaddr);
			}
		}
	}
	else
	{
#if 0
		
		for(i=0;i<NandInfo.sparesize;i++)
		{
			buf[i] = *(volatile U8*)R_NFCPUDATA;
		}
		
		SPAREDATA2* sparedata = (SPAREDATA2*)buf; 
		SEcc1 = EccCodeForLSN(sparedata->LSN[0],sparedata->LSN[1],sparedata->LSN[2]);
#else
		
		i=0;
		psparebuf[i++] = *(volatile U8*)R_NFCPUDATA;
		psparebuf[i++] = *(volatile U8*)R_NFCPUDATA;

		SEcc1 = *R_NFECC; // ECC Clear
		
		psparebuf[i++] = *(volatile U8*)R_NFCPUDATA;
		psparebuf[i++] = *(volatile U8*)R_NFCPUDATA;
		psparebuf[i++] = *(volatile U8*)R_NFCPUDATA;
		
		SEcc1 = (*R_NFECCL&0xFFFF);
		
		for(; i<NandInfo.sparesize; i++)
		{
			psparebuf[i] = *(volatile U8*)R_NFCPUDATA;
		}
		
		SPAREDATA2* sparedata = (SPAREDATA2*)psparebuf; 
#endif
		if(sparedata->LSN[0] != 0xff && sparedata->LSN[1] != 0xff && sparedata->LSN[2] != 0xff)
		{
			SEcc2 = (U16)(sparedata->SEcc[0]|((sparedata->SEcc[1])<<8));
			result = SEcc1 ^ SEcc2;
			
			if(result != 0)
			{
				parity_count=0;
				
				for(i=0;i<10;i++)
				{
					if(result & (1<<i))
					{
						parity_count++;
					}
				}
				
				if(parity_count==1)
				{
					debugprintf("'1' is %d\r\n",parity_count);
					debugstring("Original ECC Parity 1bit Error\r\n");
				}
				else if(parity_count==5)
				{	
					debugstring("1 bit Error \r\n");
					
					position_erra = SErr_Position(result);
					debugprintf("0x%x ==> ",sparedata->LSN[position_erra/8]);
					
					sparedata->LSN[position_erra/8] = sparedata->LSN[position_erra/8] ^ (1<<(position_erra%8));
					
					debugprintf("0x%x\r\n",sparedata->LSN[position_erra/8]);
					
					// printmem(buf,NandInfo.sparesize);
				}
				else
				{
					debugprintf("'1' is %d\r\n",parity_count);
					debugstring("Multiple error\r\n");
				}
			}
			else
			{
				// debugstring("No error\r\n");
				// debugprintf("Page %d is valid Data\r\n",phypageaddr);
			}
		}
	}
	
	while (!(*(volatile U8*)R_NFSTAT & F_NFSTAT_NBUSY_LEV)); // for "trr" timing,see Nand-Flash data sheet
	
	return TRUE;
#endif
}

BOOL nand_phy_writespare(U32 phypageaddr, void* sparebuf)
{
	U32 i;
	U8* ppagebuf = (U8*)sparebuf;
	
#if NAND_ECC != 0
	
	U16 SEcc;
#endif
	
 	g_dummy = *R_NFSTAT; //clear busy

	if(NandInfo.pagesize == 512)
	{
		*R_NFCMD = NAND_COMM_SPAREREAD;
		*R_NFCMD = NAND_COMM_PPRO1;
		setspareaddr(phypageaddr);
		
#if NAND_ECC != 0
		
		SPAREDATA* sparedata = ( SPAREDATA* )ppagebuf;
		SEcc = EccCodeForLSN(sparedata->LSN[0], sparedata->LSN[1], sparedata->LSN[2]);
		sparedata->SEcc[0] = (U8)(SEcc&0xff);
		sparedata->SEcc[1] = (U8)((SEcc>>8)&0xff);
#endif
	}
	else
	{
		*R_NFCMD = NAND_COMM_PPRO1;
		setspareaddr(phypageaddr);
		
#if NAND_ECC != 0
		
		SPAREDATA2* sparedata = ( SPAREDATA2* )ppagebuf;
		SEcc = EccCodeForLSN(sparedata->LSN[0], sparedata->LSN[1], sparedata->LSN[2]);
		sparedata->SEcc[0] = (U8)(SEcc&0xff);
		sparedata->SEcc[1] = (U8)((SEcc>>8)&0xff);
#endif
	}
	
	for(i=0; i<NandInfo.sparesize; i++)
	{
		*(volatile U8*)R_NFCPUDATA = ppagebuf[i];
	}
	
	*R_NFCMD = NAND_COMM_PPRO2;

    WaitBusy();
	
	*R_NFCMD = NAND_COMM_STATUS;
	
	if(*(volatile U8*)R_NFCPUDATA & NAND_RESULT_FAIL)
	{
		return FALSE;
	}
	
	return TRUE;
}

#if ECC_TEST == 1
BOOL nand_phy_writespare_wrong(U32 phypageaddr, void* sparebuf)
{
	U32 i;
	U8* psparebuf = (U8*)sparebuf;
	
 	g_dummy = *R_NFSTAT; //clear busy
	
	if(NandInfo.pagesize == 512)
	{
		*R_NFCMD = NAND_COMM_SPAREREAD;
	}
	
	*R_NFCMD = NAND_COMM_PPRO1;
	setspareaddr(phypageaddr);
	
	for(i=0; i<NandInfo.sparesize; i++)
	{
		*(volatile U8*)R_NFCPUDATA = psparebuf[i];
	}
	
	*R_NFCMD = NAND_COMM_PPRO2;

    WaitBusy();
	
	*R_NFCMD = NAND_COMM_STATUS;
	
	if(*(volatile U8*)R_NFCPUDATA & NAND_RESULT_FAIL)
	{
		return FALSE;
	}
	
	return TRUE;
}
#endif

BOOL nand_phy_readsect(U32 sectnum, void* sectbuf)
{
	U8* ppagebuf = (U8*)sectbuf;
	
	if(NandInfo.pagesize==512)
	{
		return nand_phy_readpage(sectnum, ppagebuf);
	}
	else
	{
		U32 i,k;
		U32 pagenum;
		U32 offset;
		U8 spare_offset[4]={16,23,30,37};

#if NAND_ECC == 4
		
		U8 spare_buffer[MAX_SPARE_SIZE];
		U32 error_cnt;
		U8 ECC_set;
#endif
		pagenum = sectnum/4;
		offset = (sectnum%4)*512;
		k = spare_offset[sectnum%4];
		
#if NAND_ECC == 4
		
		nand_phy_readspare(pagenum, spare_buffer);
		ECC_set = spare_buffer[NandInfo.sparesize-1];
#endif
		
		g_dummy = *R_NFSTAT;
		
		*R_NFCMD = NAND_COMM_READ1;
		setpageaddress(pagenum, offset);
		
		*R_NFCMD = NAND_COMM_READ2;
		WaitBusy();
		
#if NAND_ECC == 4
		
		g_dummy = *R_NFECC1;
		*R_NFCTRL |= F_NFCTRL_4ECCGEN;
#endif
		
		if((U32)(&ppagebuf[0])&3)
		{
			for(i=0;i<512;)
			{
				ppagebuf[i] = *(volatile U8*)R_NFCPUDATA;
				ppagebuf[i+1] = *(volatile U8*)R_NFCPUDATA;
				ppagebuf[i+2] = *(volatile U8*)R_NFCPUDATA;
				ppagebuf[i+3] = *(volatile U8*)R_NFCPUDATA;
				i+=4;
			}
		}
		else
		{
			for(i=0;i<512;)
			{
				*(U32*)(ppagebuf+i) = *R_NFCPUDATA;
				*(U32*)(ppagebuf+i+4) = *R_NFCPUDATA;
				*(U32*)(ppagebuf+i+8) = *R_NFCPUDATA;
				*(U32*)(ppagebuf+i+12) = *R_NFCPUDATA;
				i+=16;
			}		
		}
		
#if NAND_ECC == 4
		
		g_dummy = *R_NFSTAT;
		
		*R_NFCMD = NAND_COMM_READ1;
		setspareaddr_offset(pagenum, k);
		
		*R_NFCMD = NAND_COMM_READ2;
		WaitBusy();
		
		for(i=k;i<k+7;i++)
		{
			spare_buffer[i] = *(volatile U8*)R_NFCPUDATA;
		}
		
		do
		{
			g_dummy = *R_NFSTAT;
		}	
		while( !(F_NFSTAT_BCH_DDS == (g_dummy&F_NFSTAT_BCH_DDS)) );
		
		*R_NFCTRL&= ~(F_NFCTRL_4ECCGEN);	// ECC Decoding Disable
		
		if(ECC_set != 0xFF && ECC_set != 0x0)
		{
			if((ECC_set & 0x80) == 0x80)
			{
				if(F_NFSTAT_BCH_DR == (g_dummy & F_NFSTAT_BCH_DR))
				{
					error_cnt = ((g_dummy & F_NFSTAT_EBC)>>4);
					if(error_cnt > 4)
					{
						// it's impossible
						debugprintf("Ecnt %d \r\n",error_cnt);
						while(1);
					}
					
					if(0 != error_cnt)
					{
						PRINTVAR(error_cnt);
						for(i=0; i<4; i++)
						{
							debugprintf("\r\nIn %xth Byte Error\r\n",*(R_NFERRLOC0+i));
							debugprintf("\r\nError Data %02x\r\n",ppagebuf[*(R_NFERRLOC0+i)]);
							
							ppagebuf[*(R_NFERRLOC0+i)]=ppagebuf[*(R_NFERRLOC0+i)]^*(R_NFERRPTN0+i);
													
							debugprintf("Modify Data %02x\r\n",ppagebuf[*(R_NFERRLOC0+i)]);
							if(*(R_NFERRLOC0+i+1)==0)
							{
								break;
							}				
						}
					}
					
				}
				else
				{
					error_cnt = ((g_dummy&F_NFSTAT_EBC)>>4);
					debugprintf("BCH Decoding Fail / error_cnt = %d\r\n",error_cnt);
					// printmem(spare_buffer, NandInfo.sparesize);
				}
			}
			ECC_set<<=1;
		}
#endif
		return TRUE;
	}
}

//==============================================================================
// NAND Logical Function

BOOL nand_init(void)
{
	U32 i;
	U8 id_buf[8];
	BOOL re = FALSE;
	
	//==========================================================================
	// NAND Controler Port Alternate Function Set
	
	*R_PAF3 = ((*R_PAF3) & 0x000f) | 0xAAA0; //NAND ctrl
	//==========================================================================
	// NAND Controler Configuration Register Set
	
	if(GetAHBclock() > 40000000)
	{
		*R_NFCFG = nandcfg;
	}
	else
	{
		*R_NFCFG = nandcfg | F_NFCFG_RLA(1);
	}
	//==========================================================================
	// Reset NAND Flash Memory
	
	nand_reset();
	//================================================
	// NAND Flash Read ID
	
	*R_NFCMD = NAND_COMM_RID;
	*R_NFADR = 0x0;

	for( i = 0; i < 8; i++ )
	{
		id_buf[i] = *( volatile U8*)R_NFCPUDATA;
		// debugprintf("id_buf[%d] = 0x%x\r\n", i, id_buf[i]);
	}
	//================================================
	// Find Manufacture ID
	
	i = 0;
	while( nand_manuf_ids[i].id != 0 )
	{
		if( id_buf[0] == nand_manuf_ids[i].id )
		{
			debugprintf( "NAND Manufacturer : %s\r\n", nand_manuf_ids[i].name );
			re = TRUE;
			break;
		}
		i++;
	}
	if( re == FALSE )
	{
		debugprintf( "NAND Manufacturer unknown: 0x%02x\r\n", id_buf[0] );
		return FALSE;
	}
	NandInfo.maf_id = id_buf[0];
	//================================================
	// Find Device ID
	
	re = FALSE;
	i = 0;
	while( nand_flash_ids[i].name != NULL )
	{
		if( id_buf[1] == nand_flash_ids[i].id )
		{
			debugprintf( ", %s\r\n", nand_flash_ids[i].name );
			re = TRUE;
			break;
		}
		i++;
	}
	if( re == FALSE )
	{
		debugprintf( "NAND Device unknown: 0x%02x\r\n", id_buf[1] );
		return FALSE;
	}
	NandInfo.dev_id = id_buf[1];
	//================================================
	// The 3rd id byte holds MLC / multichip data
	
	NandInfo.cellinfo = id_buf[2];
	//================================================
	// NAND Flash Set Specification
	
	if( nand_flash_ids[i].pagesize  == 0 )
	{
		decode_ext_id( id_buf );
	}
	else
	{
		decode_id( id_buf, &nand_flash_ids[i] );
	}

	if(NandInfo.sparesize > 64)
	{
		debugprintf("NandInfo.sparesize too big, 0x%x(%d)\r\n", NandInfo.sparesize, NandInfo.sparesize);
		return FALSE;
	}
	
	NandInfo.pageperblock = NandInfo.blocksize / NandInfo.pagesize;
	NandInfo.blockcnt = ( nand_flash_ids[i].chipsize << 10 ) / ( NandInfo.blocksize >> 10 );

	if( NandInfo.cellinfo & NAND_CI_CELLTYPE_MSK )
	{
		// debugstring( "Nand : MLC(Multi Level Cell) type \r\n" );
		if( NandInfo.pagesize >= ( 8 * 1024 ) )
		{
			debugstring("Not Supported 24bit ECC\r\n");
			
			return FALSE;
		}
		else
		{
#if NAND_ECC == 4
			NandInfo.eccbit = 4;
#else
			NandInfo.eccbit = 0;
#endif
		}
	}
	else
	{
#if NAND_ECC == 4
		NandInfo.eccbit = 4;
#else
		NandInfo.eccbit = 0;
#endif
	}
	
	U32 temp = ( nand_flash_ids[i].chipsize << 12 ) / ( NandInfo.pagesize >> 8 );
	//i = __builtin_ffs(temp);//find first set
	i = ffs( temp ); //find first set
	i--;
	NandInfo.rowaddrlen = i;
	//reserve 1Mbyte for buffer
	i = 1;
	while( 1 )
	{
		if( ( NandInfo.blocksize * i ) >= ( 1024 * 1024 ) )
			break;
		i++;
	}
	
	NandInfo.datastartblock = i;
	NandInfo.datablockcnt = NandInfo.blockcnt - NandInfo.datastartblock;
	
	NandBuffer.tail = NandInfo.datastartblock - 1;
	NandBuffer.head = 0;		
	NandBuffer.current = NandBuffer.head;
	
	for(i = 0; i<MAX_BLOCK_CNT; i++)
	{
		PhyBlockTable[i] = 0xFFFF;
		LogBlockTable[i] = 0xFFFF;
	}
	
	U32 badcnt = nand_make_bbt();
	
	NandInfo.datablockcnt -= badcnt;
	
#if 1
	debugprintf("\r\n=============================================\r\n");
	debugprintf("NAND Information\r\n");
	debugprintf("=============================================\r\n");
	debugprintf("maf_id = 0x%x\r\n", NandInfo.maf_id);
	debugprintf("dev_id = 0x%x\r\n", NandInfo.dev_id);
	debugprintf("cellinfo = 0x%x\r\n", NandInfo.cellinfo);
	debugprintf("eccbit = 0x%x\r\n", NandInfo.eccbit);
	debugprintf("pagesize = 0x%x\r\n", NandInfo.pagesize);
	debugprintf("blocksize = 0x%x\r\n", NandInfo.blocksize);
	debugprintf("pageperblock = 0x%x\r\n", NandInfo.pageperblock);
	debugprintf("blockcnt = 0x%x\r\n", NandInfo.blockcnt);
	debugprintf("sparesize = 0x%x\r\n", NandInfo.sparesize);
	debugprintf("maxbadblockcnt = 0x%x\r\n", NandInfo.maxbadblockcnt);
	debugprintf("rowaddrlen = 0x%x\r\n", NandInfo.rowaddrlen);
	debugprintf("datastartblock = 0x%x\r\n", NandInfo.datastartblock);
	debugprintf("datablockcnt = 0x%x\r\n", NandInfo.datablockcnt);
	
	debugprintf("NandBuffer.current = 0x%x\r\n", NandBuffer.current);
	debugprintf("NandBuffer.head = 0x%x\r\n", NandBuffer.head);
	debugprintf("NandBuffer.tail = 0x%x\r\n", NandBuffer.tail);
	debugprintf("=============================================\r\n\r\n");
#endif
	
	return TRUE;
}

BOOL nand_eraseblock(U32 logblocknum)
{
	U32 phyblocknum = nand_get_phy_block(logblocknum);
	
	if(nand_phy_eraseblock(phyblocknum) == FALSE)
	{
		return FALSE;
	}
	
	return TRUE;
}

BOOL nand_readpage(U32 logpageaddr, void* pagebuf)
{
	U32 phypageaddr = nand_get_phy_page(logpageaddr);
	
	return nand_phy_readpage(phypageaddr, pagebuf);
}

BOOL nand_writepage(U32 logpageaddr, void* pagebuf)
{
	U32 phypageaddr = nand_get_phy_page(logpageaddr);
	
	return nand_phy_writepage(phypageaddr, pagebuf);
}

BOOL nand_set_sparedata(U32 logpageaddr)
{
	U32 phypageaddr = nand_get_phy_block(logpageaddr);
	
	return nand_allocate_block(phypageaddr, logpageaddr);
}

void nand_flushdata(void)
{
	// Nothing to do
}

//==============================================================================
// NAND Flash Fat Function

BOOL nand_readsect(U32 sectnum, void* sectbuf)
{	
	if(NandInfo.pagesize==512)
	{
		sectnum = nand_get_phy_page(sectnum);
	}
	else
	{
		U32 pagesize = nand_get_pagesize();
		U32 sectorperpage = pagesize/512;
		U32 pagenum = sectnum/sectorperpage;//512 is sector size
		pagenum = nand_get_phy_page(pagenum);
		sectnum = (pagenum*sectorperpage)+(sectnum%sectorperpage);
	}
	
	return nand_phy_readsect(sectnum, sectbuf);
}

BOOL nand_readsects(U32 sectnum, void* sectbuf, U32 sectcnt)
{
	int i;
	
	for(i=0; i<sectcnt; i++)
	{
		if(nand_readsect(sectnum+i, sectbuf+(i*512)) == FALSE)
			return FALSE;
	}
	
	return TRUE;
}

BOOL nand_updatesects(U32 sectnum, void* sectbuf, U32 sectcnt)
{
	ALIGN4 U8 tempbuf[2048];
	U8* buf = (U8*)sectbuf;
	U32 pageperblock = nand_get_pageperblock();
	U32 pagesize = nand_get_pagesize();
	U32 blocknum;
	U32 pagenum;
	U32 sectorperpage = pagesize/512;
	U32 startblock = (sectnum/sectorperpage)/pageperblock;
	U32 endblock = (sectnum+sectcnt)/sectorperpage/pageperblock;
	U32 startsector = sectnum;
	U32 endsector = sectnum+sectcnt-1;
	U32 totalwritelen=0;
	U32 i;
	
	for(blocknum=startblock; blocknum<endblock+1; blocknum++)
	{
		// debugprintf("NandBuffer.current = %d\r\n", NandBuffer.current);
		if(nand_phy_eraseblock(NandBuffer.current) == FALSE)
		{
			debugprintf("block %d erase error\r\n", NandBuffer.current);
			return FALSE;
		}
		
		// ===============================================================================================
		// Make Data In Buff Block
		// ===============================================================================================
		
		U32 zeropagenum = 0 + (NandBuffer.current * pageperblock);
		// debugprintf("zeropagenum = 0x%x\r\n", zeropagenum);
		
		if(pagesize==512)
		{
			for(pagenum=blocknum*pageperblock; pagenum<blocknum*pageperblock+pageperblock; pagenum++)
			{
				if((pagenum >= startsector) && (pagenum <= endsector))
				{
					nand_phy_writepage(zeropagenum, buf+totalwritelen);
					totalwritelen += 512;
				}
				else
				{
					nand_readpage(pagenum, tempbuf);
					nand_phy_writepage(zeropagenum, tempbuf);
				}
				zeropagenum++;
			}
		}
		else//2048 page
		{
			U32 startpage = startsector/4;
			U32 endpage = endsector/4;
			for(pagenum=blocknum*pageperblock;pagenum<blocknum*pageperblock+pageperblock;pagenum++)
			{
				if( (pagenum >= startpage) 
					&&  (pagenum <= endpage) )
				{
					U32 sectnum=pagenum*4;
					U32 copysectorcnt;
					U32 offsetsectorinpage;
					if(sectnum <= startsector) // page |--|data|?|
					{
						offsetsectorinpage = startsector-sectnum;
						copysectorcnt = 4-offsetsectorinpage;
						if(startpage == endpage)
						{
							copysectorcnt = endsector+1 -startsector;
						}
					}
					else//page |data|?|
					{
						offsetsectorinpage = 0;
						copysectorcnt = endsector+1-sectnum;
					}
					if(copysectorcnt>=4)
					{
						nand_phy_writepage(zeropagenum,buf+totalwritelen);
						totalwritelen += 2048;
					}
					else
					{
						nand_readpage(pagenum,tempbuf);
						memcpy(tempbuf+offsetsectorinpage*512,buf+totalwritelen,copysectorcnt*512);
						nand_phy_writepage(zeropagenum,tempbuf);
						totalwritelen += (copysectorcnt*512);
					}					
				}
				else
				{
					nand_readpage(pagenum,tempbuf);
					nand_phy_writepage(zeropagenum,tempbuf);
				}
				zeropagenum++;
			}
		}
		
		// ===============================================================================================
		// Copy Buff Block to Data Block
		// ===============================================================================================
		
		if(nand_eraseblock(blocknum) == FALSE)
		{
			return FALSE;
		}
		
		pagenum = NandBuffer.current * pageperblock;
		
		for(i = 0; i < pageperblock; i++)
		{
			nand_phy_readpage(pagenum + i, tempbuf);
			nand_writepage((blocknum*pageperblock) + i, tempbuf);
		}
		
		NandBuffer.current++;
		if(NandBuffer.current > NandBuffer.tail)
		{
			NandBuffer.current = NandBuffer.head;		
		}
	}

	return TRUE;
}

//==============================================================================
// NAND Flash Specific Function

void nand_set_cfg(U32 cfg)
{
	nandcfg = cfg;
}

U32 nand_get_blockcnt(void)
{
	return NandInfo.blockcnt;
}

U32 nand_get_blocksize(void)
{
	return NandInfo.blocksize;
}

U32 nand_get_pageperblock(void)
{
	return NandInfo.blocksize/NandInfo.pagesize;
}

U32 nand_get_pagesize(void)
{
	return NandInfo.pagesize;
}

U32 nand_get_sparesize(void)
{
	return NandInfo.sparesize;
}

U32 nand_get_sectorcount(void)
{
	return NandInfo.blocksize * NandInfo.blockcnt/512;
}

U32 nand_get_memsize_kbyte(void)
{
	return (NandInfo.blocksize/1024) * (NandInfo.datablockcnt-NandInfo.maxbadblockcnt);
}

U32 nand_get_phy_block(U32 logblocknum)
{
	U32 phyblocknum = PhyBlockTable[logblocknum];
	
	if(phyblocknum == 0xffff) //not allocate
	{
		phyblocknum = nand_get_phy_freeblock(logblocknum);
		if(phyblocknum ==-1)
		{
			NAND_DEBUGPRINTF("failed to get free block\r\n");
			return -1;
		}
		NAND_DEBUGPRINTF("new block allocated P-%d:L-%d\r\n", phyblocknum, logblocknum);
		nand_allocate_block(phyblocknum, logblocknum);
	}
	
	return phyblocknum;
}

U32 nand_get_phy_page(U32 logpageaddr)
{
	U32 blocknum = logpageaddr / NandInfo.pageperblock;
	
	blocknum = nand_get_phy_block(blocknum);
	
	return (blocknum*NandInfo.pageperblock + (logpageaddr % NandInfo.pageperblock));
}

//==============================================================================
// NAND Flash Static Function

static U32 nand_make_bbt(void)
{
	U32 i;
	U32 pageperblock = NandInfo.pageperblock;
	U32 logicalBlocknum;
	U32 badblockcnt = 0;
	U8 sparedatabuf[MAX_SPARE_SIZE];
	U8 sparedatabuf2[MAX_SPARE_SIZE];
	
	memset(PhyBlockTable, 0xff, MAX_BLOCK_CNT*2);
	
	for(i = NandInfo.datastartblock; i < NandInfo.blockcnt; i++)
	{
		nand_phy_readspare(i*pageperblock, sparedatabuf);
		nand_phy_readspare(i*pageperblock+1, sparedatabuf2);
			
		if(NandInfo.pagesize==512)
		{
			SPAREDATA* sparedata = (SPAREDATA*)sparedatabuf;
			SPAREDATA* sparedata2 = (SPAREDATA*)sparedatabuf2;
			if( (sparedata->BadInfo != 0xff) || (sparedata2->BadInfo != 0xff) )
			{
				BlockInfoTable[i] = BI_BAD;
				badblockcnt ++;
				debugprintf("%dblock is bad block\r\n",i);
				continue;
			}
			//make logical block number
			logicalBlocknum = (U32)sparedata->LSN[0] + ((U32)sparedata->LSN[1]<<8) + ((U32)sparedata->LSN[2]<<16);
			if(logicalBlocknum==0xffffff)
			{
				BlockInfoTable[i]=BI_GOOD;
			}
			else
			{
				U8 paritybit = 0;
				int k;
				//check parity bit
				for(k=0;k<24;k++)
				{
					if(logicalBlocknum & (1<<k))
					{
						if(paritybit)
							paritybit = 0;
						else
							paritybit = 1;
					}
				}
				if(sparedata->paritybit != paritybit)
				{
					BlockInfoTable[i]=BI_NOTUSED;					
				}
				else
				{
					PhyBlockTable[logicalBlocknum] = i;
					LogBlockTable[i] = logicalBlocknum;
					BlockInfoTable[i] = BI_USED;
					//NAND_DEBUGPRINTF("P-%d : L-%d\r\n",i,logicalBlocknum);
				}
			}
		}
		else //large block
		{
			SPAREDATA2* sparedata = (SPAREDATA2*)sparedatabuf;
			SPAREDATA2* sparedata2 = (SPAREDATA2*)sparedatabuf2;
			if( (sparedata->BadInfo != 0xff) || (sparedata2->BadInfo != 0xff) )
			{
				BlockInfoTable[i]=BI_BAD;
				badblockcnt ++;				
				debugprintf("%d is bad block\r\n",i);
				continue;
			}
			//make logical block number
			logicalBlocknum = (U32)sparedata->LSN[0] + ((U32)sparedata->LSN[1]<<8) + ((U32)sparedata->LSN[2]<<16);
			if(logicalBlocknum==0xffffff)
			{
				BlockInfoTable[i]=BI_GOOD;
			}
			else
			{
				U8 paritybit = 0;
				int k;
				//check parity bit
				for(k=0;k<24;k++)
				{
					if(logicalBlocknum & (1<<k))
					{
						if(paritybit)
							paritybit = 0;
						else
							paritybit = 1;
					}
				}
				if(sparedata->paritybit != paritybit)
				{
					BlockInfoTable[i]=BI_NOTUSED;					
					debugprintf("%d block paritybit error\r\n",i);
				}
				else
				{
					if(PhyBlockTable[logicalBlocknum] != 0xffff )
					{
						debugprintf("Error : logical number was duplicated, it needs Erase all]\r\n");
					}
					PhyBlockTable[logicalBlocknum] = i;
					LogBlockTable[i] = logicalBlocknum;
					BlockInfoTable[i] = BI_USED;
					//NAND_DEBUGPRINTF("P-%d : L-%d\r\n",i,logicalBlocknum);
				}
			}
		}
	}
	
	debugstring("make bad block inforamation done\r\n");
	debugprintf("bad block cnt = %d\r\n", badblockcnt);
	
	return badblockcnt;
}

static void WaitBusy(void)
{
	U32 i;
	
	// Check Busy End
	while (!(*(volatile U8*)R_NFSTAT & F_NFSTAT_BUSY_RES));
	while (!(*(volatile U8*)R_NFSTAT & F_NFSTAT_NBUSY_LEV)); // for "trr" timing,see Nand-Flash data sheet
	
	for( i=0;i<1;i++);
}

static BOOL nand_allocate_block(U32 phyblocknum, U32 logblocknum)
{
	if(phyblocknum > NandInfo.blockcnt || logblocknum > NandInfo.blockcnt)
		return FALSE;
	
	U16 oldphyblocknum = PhyBlockTable[logblocknum];
	
	if((oldphyblocknum  != phyblocknum) && (oldphyblocknum  < NandInfo.blockcnt))
	{
		BlockInfoTable[oldphyblocknum ] = BI_NOTUSED;
	}
	
	BlockInfoTable[phyblocknum] = BI_USED;
	PhyBlockTable[logblocknum] = phyblocknum;
	LogBlockTable[phyblocknum] = logblocknum;

	return TRUE;
}

static int nand_get_phy_freeblock(U32 logblocknum)
{
	int i;
	int k = logblocknum;
	
	for(i = NandInfo.datastartblock; i < NandInfo.blockcnt; i++)
	{
		if(BlockInfoTable[k] == BI_GOOD)
		{
			// if(nand_phy_eraseblock(k)==TRUE)
			return k;
		}
		else if(BlockInfoTable[i] == BI_NOTUSED)
		{
			if(nand_phy_eraseblock(k) == TRUE)
			{
				return k;
			}
			BlockInfoTable[k] = BI_BAD;
		}
		
		k++;
		
		if(k >= NandInfo.blockcnt)
		{
			k = NandInfo.datastartblock;
		}
	}
	
	return -1;
}

static void make_logical_data(int logblocknum)
{
#if NAND_ECC != 0
	
	U16 SEcc;
#endif
	
	if( NandInfo.pagesize == 512 )
	{
		SPAREDATA* sparedata = ( SPAREDATA* )sparebuf_for_lognum;
		sparedata->BadInfo = 0xff;
		sparedata->LSN[0] = logblocknum;
		sparedata->LSN[1] = logblocknum >> 8;
		sparedata->LSN[2] = logblocknum >> 16;
		
#if NAND_ECC != 0
		
		SEcc = EccCodeForLSN(sparedata->LSN[0], sparedata->LSN[1], sparedata->LSN[2]);
		sparedata->SEcc[0] = (U8)(SEcc&0xff);
		sparedata->SEcc[1] = (U8)((SEcc>>8)&0xff);
#endif
		
		U8 paritybit = 0;
		int k;
		
		//check parity bit
		for( k = 0; k < 24; k++ )
		{
			if( logblocknum & ( 1 << k ) )
			{
				if( paritybit )
					paritybit = 0;
				else
					paritybit = 1;
			}
		}
		sparedata->paritybit = paritybit;
	}
	else//2048
	{
		SPAREDATA2* sparedata = ( SPAREDATA2* )sparebuf_for_lognum;
		sparedata->BadInfo = 0xff;
		sparedata->LSN[0] = logblocknum;
		sparedata->LSN[1] = logblocknum >> 8;
		sparedata->LSN[2] = logblocknum >> 16;
		
#if NAND_ECC != 0
		
		SEcc = EccCodeForLSN(sparedata->LSN[0], sparedata->LSN[1], sparedata->LSN[2]);
		sparedata->SEcc[0] = (U8)(SEcc&0xff);
		sparedata->SEcc[1] = (U8)((SEcc>>8)&0xff);
#endif
		
		U8 paritybit = 0;
		int k;
		
		//check parity bit
		for( k = 0; k < 24; k++ )
		{
			if( logblocknum & ( 1 << k ) )
			{
				if( paritybit )
					paritybit = 0;
				else
					paritybit = 1;
			}
		}
		sparedata->paritybit = paritybit;
	}
}

/*
* nand_id_has_period - Check if an ID string has a given wraparound period
* @id_data: the ID string
* @arrlen: the length of the @id_data array
* @period: the period of repitition
*
* Check if an ID string is repeated within a given sequence of bytes at
* specific repetition interval period (e.g., {0x20,0x01,0x7F,0x20} has a
* period of 2). This is a helper function for nand_id_len(). Returns non-zero
* if the repetition has a period of @period; otherwise, returns zero.
*/
static int nand_id_has_period(u8 *id_data, int arrlen, int period)
{
	int i, j;
	
	for ( i = 0; i < period; i++ )
		for ( j = i + period; j < arrlen; j += period )
			if ( id_data[i] != id_data[j] )
				return 0;
	
	return 1;
}

/* nand_id_len - Get the length of an ID string returned by CMD_READID
* @id_data: the ID string
* @arrlen: the length of the @id_data array

* Returns the length of the ID string, according to known wraparound/trailing
* zero patterns. If no pattern exists, returns the length of the array.
*/
static int nand_id_len(u8 *id_data, int arrlen)
{
	int last_nonzero, period;

	/* Find last non-zero byte */
	for ( last_nonzero = arrlen - 1; last_nonzero >= 0; last_nonzero-- )
		if ( id_data[last_nonzero] )
			break;

	/* All zeros */
	if ( last_nonzero < 0 )
		return 0;

	/* Calculate wraparound period */
	for ( period = 1; period < arrlen; period++ )
		if ( nand_id_has_period( id_data, arrlen, period ) )
			break;

	/* There's a repeated pattern */
	if ( period < arrlen )
		return period;

	/* There are trailing zeros */
	if ( last_nonzero < arrlen - 1 )
		return last_nonzero + 1;

	/* No pattern detected */
	return arrlen;
}

static void decode_ext_id(U8 id_data[8])
{
	int extid, id_len;
	/* The 4th id byte is the important one */
	extid = id_data[3];

	id_len = nand_id_len( id_data, 8 );

	/*
	* Field definitions are in the following datasheets:
	* Old style (4,5 byte ID): Samsung K9GAG08U0M (p.32)
	* New Samsung (6 byte ID): Samsung K9GAG08U0F (p.44)
	* Hynix MLC   (6 byte ID): Hynix H27UBG8T2B (p.22)
	*
	* Check for ID length, non-zero 6th byte, cell type, and Hynix/Samsung
	* ID to decide what to do.
	*/
	if ( id_len == 6 && id_data[0] == NAND_MFR_SAMSUNG &&
		( NandInfo.cellinfo & NAND_CI_CELLTYPE_MSK ) &&
		id_data[5] != 0x00 )
	{
		/* Calc pagesize */
		NandInfo.pagesize = 2048 << ( extid & 0x03 );
		extid >>= 2;
		/* Calc oobsize */
		switch ( ( ( extid >> 2 ) & 0x04 ) | ( extid & 0x03 ) )
		{
		case 1:
			NandInfo.sparesize = 128;
			break;
		case 2:
			NandInfo.sparesize = 218;
			break;
		case 3:
			NandInfo.sparesize = 400;
			break;
		case 4:
			NandInfo.sparesize = 436;
			break;
		case 5:
			NandInfo.sparesize = 512;
			break;
		case 6:
		default: /* Other cases are "reserved" (unknown) */
			NandInfo.sparesize = 640;
			break;
		}
		extid >>= 2;
		/* Calc blocksize */
		NandInfo.blocksize = ( 128 * 1024 ) <<
			( ( ( extid >> 1 ) & 0x04 ) | ( extid & 0x03 ) );
	}
	else if ( id_len == 6 && id_data[0] == NAND_MFR_HYNIX &&
		( NandInfo.cellinfo & NAND_CI_CELLTYPE_MSK ) )
	{
		unsigned int tmp;

		/* Calc pagesize */
		NandInfo.pagesize = 2048 << ( extid & 0x03 );
		extid >>= 2;
		/* Calc oobsize */
		switch ( ( ( extid >> 2 ) & 0x04 ) | ( extid & 0x03 ) )
		{
		case 0:
			NandInfo.sparesize = 128;
			break;
		case 1:
			NandInfo.sparesize = 224;
			break;
		case 2:
			NandInfo.sparesize = 448;
			break;
		case 3:
			NandInfo.sparesize = 64;
			break;
		case 4:
			NandInfo.sparesize = 32;
			break;
		case 5:
			NandInfo.sparesize = 16;
			break;
		default:
			NandInfo.sparesize = 640;
			break;
		}
		extid >>= 2;
		/* Calc blocksize */
		tmp = ( ( extid >> 1 ) & 0x04 ) | ( extid & 0x03 );
		if ( tmp < 0x03 )
			NandInfo.sparesize = ( 128 * 1024 ) << tmp;
		else if ( tmp == 0x03 )
			NandInfo.sparesize = 768 * 1024;
		else
			NandInfo.sparesize = ( 64 * 1024 ) << tmp;
	}
	else
	{
		/* Calc pagesize */
		NandInfo.pagesize = 1024 << ( extid & 0x03 );
		extid >>= 2;
		/* Calc oobsize */
		NandInfo.sparesize = ( 8 << ( extid & 0x01 ) ) *
			( NandInfo.pagesize >> 9 );
		extid >>= 2;
		/* Calc blocksize. Blocksize is multiples of 64KiB */
		NandInfo.blocksize = ( 64 * 1024 ) << ( extid & 0x03 );
	}
}

/*
* Old devices have chip data hardcoded in the device ID table. nand_decode_id
* decodes a matching ID table entry and assigns the MTD size parameters for
* the chip.
*/
static void decode_id(U8 id_data[8], struct nand_flash_dev* type)
{
	NandInfo.blocksize = type->erasesize;
	NandInfo.pagesize = type->pagesize;
	NandInfo.sparesize = type->pagesize / 32;

	/*
	* Check for Spansion/AMD ID + repeating 5th, 6th byte since
	* some Spansion chips have erasesize that conflicts with size
	* listed in nand_ids table.
	* Data sheet (5 byte ID): Spansion S30ML-P ORNAND (p.39)
	*/
	if ( NandInfo.maf_id == NAND_MFR_AMD && id_data[4] != 0x00 && id_data[5] == 0x00
		&& id_data[6] == 0x00 && id_data[7] == 0x00
		&& NandInfo.pagesize == 512 )
	{
		NandInfo.blocksize = 128 * 1024;
		NandInfo.blocksize <<= ( ( id_data[3] & 0x03 ) << 1 );
	}
}

#if NAND_ECC != 0
static U16 EccCodeForLSN(U8 first, U8 second, U8 third)
{

	U8 P1=0, P2=0, P4=0, P8=0, P16=0;
	U8 P1p=0, P2p=0, P4p=0, P8p=0, P16p=0;
	U16 S_ECC=0xfc00;

	P1 = ((first>>7)&1)^((first>>5)&1)^((first>>3)&1)^((first>>1)&1)^P1;
	P1 = ((second>>7)&1)^((second>>5)&1)^((second>>3)&1)^((second>>1)&1)^P1;
	P1 = ((third>>7)&1)^((third>>5)&1)^((third>>3)&1)^((third>>1)&1)^P1;

	P1p = ((first>>6)&1)^((first>>4)&1)^((first>>2)&1)^(first&1)^P1p;
	P1p = ((second>>6)&1)^((second>>4)&1)^((second>>2)&1)^(second&1)^P1p;
	P1p = ((third>>6)&1)^((third>>4)&1)^((third>>2)&1)^(third&1)^P1p;	

	P2 = ((first>>7)&1)^((first>>6)&1)^((first>>3)&1)^((first>>2)&1)^P2;
	P2 = ((second>>7)&1)^((second>>6)&1)^((second>>3)&1)^((second>>2)&1)^P2;
	P2 = ((third>>7)&1)^((third>>6)&1)^((third>>3)&1)^((third>>2)&1)^P2;

	P2p = ((first>>5)&1)^((first>>4)&1)^((first>>1)&1)^(first&1)^P2p;
	P2p = ((second>>5)&1)^((second>>4)&1)^((second>>1)&1)^(second&1)^P2p;
	P2p = ((third>>5)&1)^((third>>4)&1)^((third>>1)&1)^(third&1)^P2p;	

	P4 = ((first>>7)&1)^((first>>6)&1)^((first>>5)&1)^((first>>4)&1)^P4;
	P4 = ((second>>7)&1)^((second>>6)&1)^((second>>5)&1)^((second>>4)&1)^P4;
	P4 = ((third>>7)&1)^((third>>6)&1)^((third>>5)&1)^((third>>4)&1)^P4;

	P4p = ((first>>3)&1)^((first>>2)&1)^((first>>1)&1)^(first&1)^P4p;
	P4p = ((second>>3)&1)^((second>>2)&1)^((second>>1)&1)^(second&1)^P4p;
	P4p = ((third>>3)&1)^((third>>2)&1)^((third>>1)&1)^(third&1)^P4p;

	P8 = ((second>>7)&1)^((second>>6)&1)^((second>>5)&1)^((second>>4)&1)^((second>>3)&1)^((second>>2)&1)^((second>>1)&1)^(second&1)^P8;
	
	P8p = ((first>>7)&1)^((first>>6)&1)^((first>>5)&1)^((first>>4)&1)^((first>>3)&1)^((first>>2)&1)^((first>>1)&1)^(first&1)^P8p;
	P8p = ((third>>7)&1)^((third>>6)&1)^((third>>5)&1)^((third>>4)&1)^((third>>3)&1)^((third>>2)&1)^((third>>1)&1)^(third&1)^P8p;
	
	P16 = ((third>>7)&1)^((third>>6)&1)^((third>>5)&1)^((third>>4)&1)^((third>>3)&1)^((third>>2)&1)^((third>>1)&1)^(third&1)^P16;
	
	P16p = ((first>>7)&1)^((first>>6)&1)^((first>>5)&1)^((first>>4)&1)^((first>>3)&1)^((first>>2)&1)^((first>>1)&1)^(first&1)^P16p;
	P16p = ((second>>7)&1)^((second>>6)&1)^((second>>5)&1)^((second>>4)&1)^((second>>3)&1)^((second>>2)&1)^((second>>1)&1)^(second&1)^P16p;
	
	S_ECC |= ((~P4&1)<<9) | ((~P4p&1)<<8) | ((~P2&1)<<7) | ((~P2p&1)<<6) | ((~P1&1)<<5) | ((~P1p&1)<<4) | ((~P16&1)<<3) | ((~P16p&1)<<2) | ((~P8&1)<<1) | (~P8p&1);

	return S_ECC;
}
#endif

__inline__ static void nand_reset(void)
{
	*R_NFCMD = NAND_COMM_RST;
	WaitBusy();
}

__inline__ static void setpageaddress(U32 phypageaddr, U32 offset)
{
	int i;
	int k;
	U8 mask;
	U8 validbit;
	
	*R_NFADR = offset & 0xff;  // A0-A7
	
	if(NandInfo.pagesize == 2048)
	{
		*R_NFADR = offset>>8;
	}

	for(i=0; i < NandInfo.rowaddrlen; i += 8)
	{
		mask=0;
		validbit = (NandInfo.rowaddrlen-i)>8?8:(NandInfo.rowaddrlen-i);
		
		for(k=0;k<validbit;k++)
		{
			mask |= (1<<k);
		}
		
		*R_NFADR = (U8)(phypageaddr & mask) ;  // Row Address
		phypageaddr = phypageaddr>>8;
	}
}

__inline__ static void seteraseblockaddr(U32 phypageaddr)
{
	int i;
	int k;
	U8 mask;
	U8 validbit;
	
	for(i=0;i<NandInfo.rowaddrlen;i+=8)
	{
		mask=0;
		validbit = (NandInfo.rowaddrlen-i)>8?8:(NandInfo.rowaddrlen-i);
		
		for(k=0;k<validbit;k++)
		{
			mask |= (1<<k);
		}
		
		*R_NFADR = (U8)(phypageaddr & mask);  // Row Address
		phypageaddr = phypageaddr>>8;
	}
}

__inline__ static void setspareaddr(U32 phypageaddr)
{
	if(NandInfo.pagesize==512)
	{
		setpageaddress(phypageaddr, 0);
	}
	else
	{
		int i;
		int k;
		U8 mask;
		U8 validbit;
		
		*R_NFADR = 0;
		*R_NFADR = 0x8;
		
		for(i=0; i<NandInfo.rowaddrlen; i+=8)
		{
			mask=0;
			validbit = (NandInfo.rowaddrlen-i)>8?8:(NandInfo.rowaddrlen-i);
			
			for(k=0;k<validbit;k++)
			{
				mask |= (1<<k);
			}
			
			*R_NFADR = (U8)(phypageaddr  & mask);  // Row Address
			phypageaddr = phypageaddr>>8;
		}
	}
}

__inline__ static void setspareaddr_offset(U32 phypageaddr, U32 offset)
{
	if(NandInfo.pagesize==512)
	{
		setpageaddress(phypageaddr, offset);
	}
	else
	{
		int i;
		int k;
		U8 mask;
		U8 validbit;
		
		*R_NFADR = offset;
		*R_NFADR = 0x8;
		
		for(i=0; i<NandInfo.rowaddrlen; i+=8)
		{
			mask=0;
			validbit = (NandInfo.rowaddrlen-i)>8?8:(NandInfo.rowaddrlen-i);
			
			for(k=0;k<validbit;k++)
			{
				mask |= (1<<k);
			}
			
			*R_NFADR = (U8)(phypageaddr  & mask);  // Row Address
			phypageaddr = phypageaddr>>8;
		}
	}
}

__inline__ static U32 nand_get_log_block(U32 phyblocknum)
{
	return LogBlockTable[phyblocknum];
}

//==============================================================================
