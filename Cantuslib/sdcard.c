/*

	SD CARD Driver
	
	kkh
*/

#include "cantus.h"

typedef struct 
{
	int CSD_STRUCTURE:2;
	int RESERVED:6;
	int TAAC:8;
	int TRAN_SPEED:8;
	int CCC:12;
	int READ_BL_LEN:4;
	int READ_BL_PARTIAL:1;
	int WRITE_BLK_MISALIGN:1;
	int READ_BLK_MISALIGN:1;
	int SDR_IMP:1;
	int RESERVED2:2;
	int C_SIZE:12;
	int VDD_R_CURR_MIN:3;
	int VDD_R_CURR_MAX:3;
	int VDD_W_CURR_MIN:3;
	int VDD_W_CURR_MAZ:3;
	int C_SIZE_MULT:3;
	int ERASE_BLK_EN:1;
	int SECTOR_SIZE:7;
	int WP_GRP_SIZE:7;
	int WP_GRP_ENABLE:1;
	int RESERVED3:5;
	int FILE_FORMAT_GRP:1;
	int COPY:1;
	int PERM_WRITE_PROTECT:1;
	int TMP_WRITE_PROTECT:1;
	int FILE_FORMAT:2;
	int RESERVED4:2;
	int CRC:7;
	int ALWAYS1:1;
	
}CSD_STRUCT;

enum SDC_ERROR
{
	SDC_ERR_NONE=0,
	SDC_ERR_READ_TIMEOUT,
	SDC_ERR_RESPONSE_TIMEOUT,
	SDC_ERR_WRITE_CRC,
	SDC_ERR_READ_CRC,
	SDC_ERR_RESPONSE_CRC,
};
#define SDC_NOERROR	0
#define SDC_RESPONSE_TIMEOUT 1
#define SDC_READ_TIMEOUT 2
	
static U8 g_bHCMemory = 0;
	
BOOL SDC_Command(U32 cmd, U32 arg);

//#define SDC_DEBUG_ENABLE
#ifdef SDC_DEBUG_ENABLE
	#define SDC_DEBUG debugprintf
#else
	#define SDC_DEBUG(...)
#endif

static U16 s_sdc_resposebuf[8];//max 136bit
static U32 s_sdc_lasterrorno = SDC_ERR_NONE;
static U32 s_sdc_laststat = 0;
static U32 s_BlockSize = 512;
static U32 s_MemSize = 0;

//Sets the address of the first/last write block to be erase
BOOL SDC_Erase(U32 addr, U32 endaddr)
{
	SDC_Command(F_SDCCMDCON_CTYPE_WRSP|SD_MODE_ERASE_WR_BLK_START, addr); 
	SDC_Command(F_SDCCMDCON_CTYPE_WRSP|SD_MODE_ERASE_WR_BLK_END, endaddr); 
	SDC_Command(F_SDCCMDCON_CTYPE_WRSP|F_SDCCMDCON_RTYPE_SBUSY|SD_MODE_ERASE, 0); 
	
	return TRUE;
}

BOOL SDC_WriteBlock(void* buf, U32 blocknum, U32 nob)
{
	register U32 i;
	register U32 stat;
	
	*R_SDCNWR = 0x10;
	*R_SDCNOB = F_SDCNOB(nob);
	
	U32 addr;
	
	if(g_bHCMemory)
		addr = blocknum;
	else
		addr = blocknum*512;
	
	if(nob>1)
		SDC_Command(F_SDCCMDCON_CTYPE_WRSP|F_SDCCMDCON_CMD_WDATA|F_SDCCMDCON_CMD_WRITE|SD_MODE_WRITE_MULTIPLE_BLOCK, addr); //multiblock
	else
		SDC_Command(F_SDCCMDCON_CTYPE_WRSP|F_SDCCMDCON_CMD_WDATA|F_SDCCMDCON_CMD_WRITE|SD_MODE_WRITE_BLOCK, addr); //single block
		
	U32* databuf = (U32*)buf;
	BOOL bok = TRUE;
	
	for(i=0;i<nob*16;i++)
	{
		//fill half for speed
		
		*R_SDCDFA = *(U32*)databuf;	
		*R_SDCDFA = *(U32*)(databuf+1);
		*R_SDCDFA = *(U32*)(databuf+2);
		*R_SDCDFA = *(U32*)(databuf+3);
		*R_SDCDFA = *(U32*)(databuf+4);
		*R_SDCDFA = *(U32*)(databuf+5);
		*R_SDCDFA = *(U32*)(databuf+6);
		*R_SDCDFA = *(U32*)(databuf+7);
		databuf += 8;
		
		//wait for half empty
		while(1)
		{
			stat = *R_SDCSTAT;
			if((stat & F_SDCSTAT_FIFO_HFULL) == 0)
				break;
		}
		
		//check error
		if(stat & F_SDCSTAT_WDCRCERR_CRC)
		{
			bok = FALSE;
			break;
		}
	}
	if(bok)
	{
		while(1)
		{
			stat = *R_SDCSTAT;
			if(stat & F_SDCSTAT_WRT_DONE)
				break;
		}
	}
	if(nob>1)
		SDC_Command(F_SDCCMDCON_CTYPE_WRSP|F_SDCCMDCON_RTYPE_SBUSY|SD_MODE_STOP_TRANSMISSION, 0);	//stop commmand
	
	*R_SDCSTAT = F_SDCSTAT_WRT_DONE;	//write operation done
	
	return bok;
}


static int  SDC_ReadBlock_r(void* buf,U32 blocknum,U32 nob)
{
	register U32 i,j;
	register U32 stat;
	*R_SDCNWR = 0x10;
	*R_SDCNOB = F_SDCNOB(nob);
	
	U32 addr ;
	
	if(g_bHCMemory)
		addr = blocknum;
	else
		addr = blocknum*512;

	if(nob>1)
		SDC_Command(F_SDCCMDCON_CTYPE_WRSP|F_SDCCMDCON_CMD_WDATA|SD_MODE_READ_MULTIPLE_BLOCK, addr);
	else
		SDC_Command(F_SDCCMDCON_CTYPE_WRSP|F_SDCCMDCON_CMD_WDATA|SD_MODE_READ_SINGLE_BLOCK, addr);
	
	U32* databuf = (U32*)buf;
	int re = 0;

	for(i=0; i<nob*8; i++)
	{
		//wait for half full
		while(1)
		{
			stat = *R_SDCSTAT;
//			PRINTVAR(stat);
			if(stat & F_SDCSTAT_FIFO_FULL) //full
			{
				break;
			}
			if(stat & F_SDCSTAT_RDTOERR_TO) //read time out
			{
				s_sdc_lasterrorno = SDC_ERR_READ_TIMEOUT;
				re = 1;
				break;
			}
		}
		if(re != 0)
		{
			break;
		}
		
		if(stat & (1<<SDC_ERR_READ_CRC))
		{
			s_sdc_lasterrorno = F_SDCSTAT_RDTOERR_TO;
			re  = 2;
			break;
		}
		for(j=0; j<2; j++)
		{
			*(U32*)databuf = *R_SDCDFA;
			*(U32*)(databuf+1) = *R_SDCDFA;
			*(U32*)(databuf+2) = *R_SDCDFA;
			*(U32*)(databuf+3) = *R_SDCDFA;
			*(U32*)(databuf+4) = *R_SDCDFA;
			*(U32*)(databuf+5) = *R_SDCDFA;
			*(U32*)(databuf+6) = *R_SDCDFA;
			*(U32*)(databuf+7) = *R_SDCDFA;
			databuf+=8;
		}
		
		if(stat & F_SDCSTAT_RD_DONE)	//read operatoin completed
		{
			break;
		}
	}
	
	if(re==0)
	{
		while(1)
		{
			stat = *R_SDCSTAT;
			
			if(stat & F_SDCSTAT_RD_DONE)
				break;
			
			if(stat & F_SDCSTAT_RDTOERR_TO)
			{
				re = 3;
				break;
			}
		}
	}
	
	if(nob >1)
	{
		SDC_Command(F_SDCCMDCON_CTYPE_WRSP|F_SDCCMDCON_RTYPE_SBUSY|SD_MODE_STOP_TRANSMISSION, 0);	//stop commmand
	}
	*R_SDCSTAT = F_SDCSTAT_RD_DONE;	//write operation done
	
	return re;
}


BOOL SDC_ReadBlock(void* buf, U32 blocknum, U32 nob)
{
	if(SDC_ReadBlock_r(buf, blocknum, nob) != 0)
	{
		int i;
		int sysclock = GetAHBclock();
		
		for(i=0; i<100; i++)
		{
			if((sysclock/(2+i)) <= (1*1000*1000))
				break;
		}
		
		U32 orgdiv = *R_SDCCD;
		
		*R_SDCCD = i;
		int re = SDC_ReadBlock_r(buf, blocknum, nob);
		*R_SDCCD = orgdiv;
		
		if(re == 0)
			return TRUE;
		else if(re == 1)
			debugprintf("READ TIME OUT ERROR(0x%x block)\r\n", blocknum);
		else if (re == 2)
			debugstring("READ CRC ERROR\r\n");
		return FALSE;
	}
	
	return TRUE;
}

BOOL SDC_Command(U32 cmd, U32 arg)
{
	U8 i; 

	// PRINTVAR(F_SDCCMDCON_CMD_NUMBER(cmd));
	
	*R_SDCCMDA = F_SDCCMDA(arg);
	*R_SDCCMDCON = cmd;
	U8 cmdnum = F_SDCCMDCON_CMD_NUMBER(cmd);
	//debugprintf("CMD%d ,ARG:%#x \r\n", cmdnum,arg);
	while(1)
	{
		s_sdc_laststat = *R_SDCSTAT;
		if(s_sdc_laststat & F_SDCSTAT_RSP_DONE)
		{
		//	PRINTVAR(s_sdc_laststat);
			break;
		}
		if(s_sdc_laststat & F_SDCSTAT_RTOERR_TO)
		{
			*R_SDCSTAT =	F_SDCSTAT_RSP_DONE		| F_SDCSTAT_WRT_DONE		| F_SDCSTAT_RD_DONE		|
							F_SDCSTAT_WCRCERR_CRC	| F_SDCSTAT_WCRCERR_NCRCRSP	| F_SDCSTAT_RCRCERR_CRC	| F_SDCSTAT_RDCRCERR_CRC |
							F_SDCSTAT_WDCRCERR_CRC	| F_SDCSTAT_RTOERR_TO		| F_SDCSTAT_RDTOERR_TO	| F_SDCSTAT_MBUSY_BUSY; // do not clear read done / write done
							
			//PRINTVAR(s_sdc_laststat);
			s_sdc_lasterrorno = F_SDCSTAT_RTOERR_TO;
			SDC_DEBUG("SDC_ERR_RESPONSE_TIMEOUT\r\n");
			return FALSE;
		}
	}
	
    *R_SDCSTAT =	F_SDCSTAT_RSP_DONE		| F_SDCSTAT_WRT_DONE		| F_SDCSTAT_RD_DONE		|
					F_SDCSTAT_WCRCERR_CRC	| F_SDCSTAT_WCRCERR_NCRCRSP	| F_SDCSTAT_RCRCERR_CRC	| F_SDCSTAT_RDCRCERR_CRC |
					F_SDCSTAT_WDCRCERR_CRC	| F_SDCSTAT_RTOERR_TO		| F_SDCSTAT_RDTOERR_TO	| F_SDCSTAT_MBUSY_BUSY; // do not clear read done / write done
	
	if(cmd & F_SDCCMDCON_CTYPE_WRSP)
	{
		if(s_sdc_laststat & F_SDCSTAT_RTOERR_TO)
		{
			//PRINTVAR(s_sdc_laststat);
			s_sdc_lasterrorno = F_SDCSTAT_RTOERR_TO;
			SDC_DEBUG("SDC_ERR_RESPONSE_TIMEOUT\r\n");
			return FALSE;
		}
		
		if(!(cmdnum == 41|| cmdnum == 1))
		{
			if(s_sdc_laststat & F_SDCSTAT_RCRCERR_CRC)
			{
				s_sdc_lasterrorno = F_SDCSTAT_RCRCERR_CRC;
				//PRINTVAR(s_sdc_lasterrorno );
				return FALSE;
			}
		}

		if(cmd & F_SDCCMDCON_RTYPE_LONG)
		{
			SDC_DEBUG("Long Respose\r\n");
			for(i=0; i<8; i++)
				s_sdc_resposebuf[i] = *(volatile U16*)R_SDCRFA;
		}
		else
		{
			SDC_DEBUG("Short Respose\r\n");
			for(i=0; i<3; i++)
			{
				s_sdc_resposebuf[i] = *(volatile U16*)R_SDCRFA;
				SDC_DEBUG(s_sdc_resposebuf[i]);
			}
		}
	}
	
    return TRUE;
}

BOOL SDC_Init()
{
	U32 i;
	g_bHCMemory = 0;
	*R_PAF4 = ((*R_PAF4) & 0xf00f)|PAF4_SDCD_CLK|PAF4_SDCD_CMD|PAF4_SDCD_D2|PAF4_SDCD_D3;
	*R_PAF5 = ((*R_PAF5) & 0xfff0)|PAF5_SDCD_D0|PAF5_SDCD_D1;
	
	*R_SDCCON = 0;//disable
	*R_SDCCON = F_SDCCON_MMCSDHC_EN | F_SDCCON_CLK_EN;
	delayms(10);
	
	// initialize
	*R_SDCCD = F_SDCCD(0x200);	// fsdclk = 185KHz @ AHB 96MHz
	*R_SDCRTO = F_SDCRTO(0xFF);	// 256 clock counts
	*R_SDCDTO = F_SDCDTO(0xF0);
	*R_SDCBL = F_SDCBL(512);	// Block 512 Byte
	
	i= 512;//512 times
	do
	{
		if(!SDC_Command(F_SDCCMDCON_CTYPE_NWRSP|SD_MODE_GO_IDLE_STATE, 0)) //reset
			continue;
			
		if(SDC_Command(F_SDCCMDCON_CTYPE_WRSP|8, 0x1AA))
			break;//cmd55
	}while(--i);
		
	i= 1024;//512 times
	do
	{
		SDC_Command(F_SDCCMDCON_CTYPE_WRSP|SD_MODE_APP_CMD, 0); //ACMDs shall be preceded with APP_CMD command (CMD55).
		if(!SDC_Command(F_SDCCMDCON_CTYPE_WRSP|F_SDCCMDCON_RTYPE_SBUSY|SD_MODE_SD_APP_OP_COND, 0x40FF8000))
		{
			continue;
		}
		else
		{
			if(s_sdc_resposebuf[0] & 0x80)
			{
				if(s_sdc_resposebuf[0] & (1<<6))
				{
					debugstring("HCSD Memory Card\r\n");
					g_bHCMemory = 1;
				}
				else
				{
					debugstring("SD Memory Card\r\n"); 
				}
				break;
			}
		}
		
	}while(--i);
	
	if(i==0)
	{
		debugstring("TIME OUT 1 \r\n");
		return FALSE;
	}


	debugstring("Identification Mode Start\r\n");
	
	i= 512;//512 times
	do
	{
		if(SDC_Command(F_SDCCMDCON_CTYPE_WRSP|F_SDCCMDCON_RTYPE_LONG|SD_MODE_ALL_SEND_CID, 0) == TRUE)
		{
			break;
		}
	}while(i--);
	
	if(i==0)
	{
		debugstring("COMMAND SEND ERROR \r\n");
		return FALSE;
	}
	
	if(SDC_Command(F_SDCCMDCON_CTYPE_WRSP|SD_MODE_SEND_RELATIVE_ADDR, 0) == FALSE) //Get RCA
	{

		return FALSE;
	}
	
	U32 rca;
	rca = ((s_sdc_resposebuf[0] & 0x00ff)<<8) | (s_sdc_resposebuf[1]>>8);
	SDC_Command(F_SDCCMDCON_CTYPE_WRSP|F_SDCCMDCON_RTYPE_LONG|SD_MODE_SEND_CSD, rca<<16);

#ifdef SDC_DEBUG_ENABLE
	debugstring("==== CSD ===== \r\n");
	for(i=0; i<8; i++)
	{
		debugprintf("%04X [%d:%d]\r\n", s_sdc_resposebuf[i], 16*(8-i)-1, 16*(8-i)-16);
		PRINTVAR(s_sdc_resposebuf[i]);
	}
#endif
	
	U8 sd_ver = 0;//0:1.0, 1:2.0
	U8 TAAC,NSAC,TRAN_SPEED;
	sd_ver = s_sdc_resposebuf[0]>>14;
	TAAC = s_sdc_resposebuf[0]&0xff;
	NSAC = s_sdc_resposebuf[1]>>8;
	TRAN_SPEED = s_sdc_resposebuf[1]&0xff;
	U32 maxfreq;
	
	switch(TRAN_SPEED & 0x7)
	{
		case 0:
			maxfreq = 1*10000;
			break;
		case 1:
			maxfreq = 1*100000;
			break;
		case 2:
			maxfreq = 10*100000;
			break;
		case 3:
			maxfreq = 100*100000;
			break;
		default:
			debugstring("unsupported operating frequency\r\n");
			maxfreq = 1*10000;
			break;
	}
	U32 freqtype[16]={0,10,12,13,15,20,25,30,35,40,45,50,55,60,70,80};
	maxfreq *= freqtype[TRAN_SPEED>>3];	
	
	debugprintf("the mandatory maximum operating frequency of SD Memory Card : %dMhz \r\n", maxfreq/1000000);
	
	int READ_BL_LEN = s_sdc_resposebuf[2]&0xf;
	int C_SIZE;
	
	if(sd_ver == 0)
		C_SIZE = ((s_sdc_resposebuf[3]&0x03ff)<<2) + ((s_sdc_resposebuf[4]&0xc000)>>14);
	else
		C_SIZE = ((s_sdc_resposebuf[3]&0x3f)<<16) + s_sdc_resposebuf[4];
	
	s_BlockSize = 1<<READ_BL_LEN;;
	
	if(sd_ver == 0)
	{
		int C_SIZE_MULT = ((s_sdc_resposebuf[4]&0x3)<<1) + ((s_sdc_resposebuf[5]&0x8000)>>15);
		//U32 BLOCKNR = (C_SIZE+1)*(pow(2,C_SIZE_MULT+2));
		U32 BLOCKNR = (C_SIZE+1)*(1<<(C_SIZE_MULT+2));
		s_MemSize = (BLOCKNR * s_BlockSize)/(1024*1024);
	}
	else
	{
		s_MemSize = (C_SIZE+1)/2;
		debugprintf("SD Memory Size : %ldMbyte \r\n", s_MemSize);
	}
	debugprintf("SD Memory Size : %dMbyte ,Blocksize:%dbyte\r\n", s_MemSize, s_BlockSize);

	SDC_Command(F_SDCCMDCON_CTYPE_WRSP|F_SDCCMDCON_RTYPE_LONG|SD_MODE_SEND_CID, rca<<16);

#ifdef SDC_DEBUG_ENABLE	
	debugstring("==== CID ===== \r\n");
	for(i=0; i<8; i++)
	{
		debugprintf("%04X [%d:%d]\r\n", s_sdc_resposebuf[i], 16*(8-i)-1, 16*(8-i)-16);
		PRINTVAR(s_sdc_resposebuf[i]);
	}
#endif

	SDC_Command(F_SDCCMDCON_CTYPE_WRSP|F_SDCCMDCON_RTYPE_SBUSY|SD_MODE_SELECT_DESELECT_CARD, rca<<16);
	debugstring("SD CARD Initialize Completed\r\n");

	//after initialize
	//if not 512
	if(READ_BL_LEN != 9)
	{
		SDC_Command(F_SDCCMDCON_CTYPE_WRSP|SD_MODE_SET_BLOCKLEN, 512);	//set block size
		debugstring("set block size as 512byte\r\n");
	}
	SDC_Command(F_SDCCMDCON_CTYPE_WRSP|SD_MODE_APP_CMD, rca<<16);	//APPcmd
	SDC_Command(F_SDCCMDCON_CTYPE_WRSP|SD_MODE_SET_BUS_WIDTH, 2);	//4bit mode
	*R_SDCCON = F_SDCCON_MMCSDHC_EN|F_SDCCON_BWIDTH_4BIT|F_SDCCON_CLK_EN;	//4bit mode
	
	U32 sysclock = GetAHBclock();

	for(i=0; i<100; i++)
	{
		if((sysclock/(2+i)) <= maxfreq)
			break;
	}
	debugprintf("SDCard operating frequency:%d \r\n", sysclock/(2+i));
	*R_SDCCD = F_SDCCD(i);
	
	return TRUE;
}

U32 SDC_GetSectorSize()
{
	return 512;
}

U32 SDC_GetSectorCount()
{
	return ((unsigned long long)s_MemSize*1024*1024/512);
}

//return Megabyte Size
U32 SDC_MemSize()
{
	return s_MemSize;
}

#define SDC_MEMCPY(dest, src, bytesize) memcpy((void*)dest, (void*)src, bytesize)
static BOOL SDC_Readblock_indirect(void* buf, U32 sectoraddr, U32 sectcnt)
{
	U32 tempbuf[512/4];
	U32 bufaddr = (U32)buf;
	int i;
	
	for(i=0; i<sectcnt; i++)
	{
		if(SDC_ReadBlock(tempbuf, (sectoraddr+i), 1) == FALSE)
			return FALSE;
		SDC_MEMCPY(bufaddr, tempbuf, 512);
		bufaddr += 512;
	}
	
	return TRUE;
}

BOOL SDC_Read_Sect(void* buf, unsigned int sector, unsigned int sectorcnt)
{
	
	if((unsigned int)buf & 0x3)
	{
		if(SDC_Readblock_indirect(buf, sector, sectorcnt) == FALSE)
			return FALSE;
	}
	else
	{
		if(SDC_ReadBlock(buf, sector, sectorcnt)==FALSE)
			return FALSE;
	}
	return TRUE;
}

static BOOL SDC_Writeblock_indirect(void* buf, U32 sectoraddr, U32 sectcnt)
{
	U32 tempbuf[512/4];
	U32 bufaddr = (U32)buf;
	int i;
	
	for(i=0; i<sectcnt; i++)
	{
		SDC_MEMCPY(bufaddr, tempbuf, 512);
		if(SDC_WriteBlock(tempbuf, (sectoraddr+i), 1) == FALSE)
			return FALSE;
		bufaddr += 512;
	}
	return TRUE;
}

BOOL SDC_Write_Sect(void* buf, unsigned int sector, unsigned int sectorcnt)
{
	if((unsigned int)buf & 0x3)
	{
		if(SDC_Writeblock_indirect(buf, sector, sectorcnt) == FALSE)
				return FALSE;
	}
	else
	{
		if(SDC_WriteBlock(buf, sector, sectorcnt) == FALSE)
			return FALSE;
	}
	return TRUE;
}
