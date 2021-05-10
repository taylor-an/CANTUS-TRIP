#pragma once

//======================================================================================================================
// SDHC Control Register (SDHCCON)

#define R_SDHCCON					((volatile unsigned int*)0x80001000)
#define R_SDCCON					((volatile unsigned int*)0x80001000)

// [ 5] R/W MMC/SDHC Enable
// 0 : Disable(Controller is initialized)
// 1 : Enable
#define F_SDCCON_MMCSDHC_DIS		( 0<< 5)
#define F_SDCCON_MMCSDHC_EN			( 1<< 5)

// [ 4 : 3] R/W Memory Access Type
// 00 : Byte Align
// 01 : Short Align
// 10 : Word Align
// 11 : Not Use
#define F_SDCCON_MACCESS_BYTE		( 0<< 3)
#define F_SDCCON_MACCESS_SHORT		( 1<< 3)
#define F_SDCCON_MACCESS_WORD		( 2<< 3)
#define F_SDCCON_MACCESS_NOT		( 3<< 3)

// [ 2] R/W DMA Mode Selection
// 0 : Normal Mode(Data Transfer By CPU)
// 1 : DMA Mode(Data Transfer By DMA)
#define F_SDCCON_MODE_NORMAL		( 0<< 2)
#define F_SDCCON_MODE_DMA			( 1<< 2)

// [ 1] R/W Bus Width Selection
// 0 : 1Bit Data Bus
// 1 : 4Bit Data Bus
#define F_SDCCON_BWIDTH_1BIT		( 0<< 1)
#define F_SDCCON_BWIDTH_4BIT		( 1<< 1)

// [ 0] R/W MMC/SDHC Clock Enable
// 0 : Disable
// 1 : Enable
#define F_SDCCON_CLK_DIS			( 0<< 0)
#define F_SDCCON_CLK_EN				( 1<< 0)
//======================================================================================================================

//======================================================================================================================
// SDHC Status Register (SDHCSTAT)

#define R_SDHCSTAT					((volatile unsigned int*)0x80001004)
#define R_SDCSTAT					((volatile unsigned int*)0x80001004)

// [15] RO Card Insertion
// 0 : No Card Insertion Detection
// 1 : Card Insert Detected
#define F_SDCSTAT_CINSRT_NDTT		( 0<<15)
#define F_SDCSTAT_CINSRT_DTT		( 1<<15)

// [14] RO Card Removal
// 0 : No Card Removal Detection
// 1 : Card Remove Detected
#define F_SDCSTAT_CRMVL_NDTT		( 0<<14)
#define F_SDCSTAT_CRMVL_DTT			( 1<<14)

// [13] RO FIFO Full
// 0 : Not Full
// 1 : Full
#define F_SDCSTAT_FIFO_NFULL		( 0<<13)
#define F_SDCSTAT_FIFO_FULL			( 1<<13)

// [12] RO FIFO Half Full
// 0 : Not Half Full
// 1 : Half Full
#define F_SDCSTAT_FIFO_NHFULL		( 0<<12)
#define F_SDCSTAT_FIFO_HFULL		( 1<<12)

// [11] RO FIFO Empty
// 0 : Not Empty
// 1 : Empty
#define F_SDCSTAT_FIFO_NEMPTY		( 0<<11)
#define F_SDCSTAT_FIFO_EMPTY		( 1<<11)

// [10] R/C Command & Response Transaction Done
// 0 : In Progress
// 1 : Done
#define F_SDCSTAT_RSP_NDONE			( 0<<10)
#define F_SDCSTAT_RSP_DONE			( 1<<10)

// [ 9] R/C Data Write Operation DoneDone
// 0 : In Progress or Incomplete
// 1 : Done
#define F_SDCSTAT_WRT_NDONE			( 0<< 9)
#define F_SDCSTAT_WRT_DONE			( 1<< 9)

// [ 8] R/C Data Read Operation Done
// 0 : In Progress or Incomplete
// 1 : Done
#define F_SDCSTAT_RD_NDONE			( 0<< 8)
#define F_SDCSTAT_RD_DONE			( 1<< 8)

// [ 7 : 6] R/C Write CRC Error Codee
// 00 : No CRC Error
// 01 : CRC Error(In Data Block CRC Error)
// 10 : No CRC Response(Ignored Data Block In SDCARD)
#define F_SDCSTAT_WCRCERR_NO		( 0<< 6)
#define F_SDCSTAT_WCRCERR_CRC		( 1<< 6)
#define F_SDCSTAT_WCRCERR_NCRCRSP	( 2<< 6)

// [ 5] R/C Response CRC Error
// 0 : No Error
// 1 : Reponse CRC Error Occurred
#define F_SDCSTAT_RCRCERR_NCRC		( 0<< 5)
#define F_SDCSTAT_RCRCERR_CRC		( 1<< 5)

// [ 4] R/C Read Data CRC Error
// 0 : No Error
// 1 : Read Data CRC Error Occurred
#define F_SDCSTAT_RDCRCERR_NCRC		( 0<< 4)
#define F_SDCSTAT_RDCRCERR_CRC		( 1<< 4)

// [ 3] R/C Write Data CRC Error
// 0 : No Error
// 1 : Write Data CRC Error Occurred
#define F_SDCSTAT_WDCRCERR_NCRC		( 0<< 3)
#define F_SDCSTAT_WDCRCERR_CRC		( 1<< 3)

// [ 2] R/C Response Time Out Error
// 0 : No Error
// 1 : Command Response Was Not Received In Time Specified
#define F_SDCSTAT_RTOERR_NTO		( 0<< 2)
#define F_SDCSTAT_RTOERR_TO			( 1<< 2)

// [ 1] R/C Read Data Time Out Error
// 0 : No Error
// 1 : The Expected Data From Card Was Not Received In Time Specified
#define F_SDCSTAT_RDTOERR_NTO		( 0<< 1)
#define F_SDCSTAT_RDTOERR_TO		( 1<< 1)

// [ 0] RO Momory Busy State
// 0 : Memory Is Ready
// 1 : Memory Is Busy
#define F_SDCSTAT_MBUSY_READY		( 0<< 0)
#define F_SDCSTAT_MBUSY_BUSY		( 1<< 0)
//======================================================================================================================

//======================================================================================================================
// SDHC Clock Divide Register (SDHCCD)

#define R_SDCCD						((volatile unsigned int*)0x80001008)

// [ 9 : 0] R/W MMC/SD Clock Divide Register
//              fahb_clock
// fsdclk = -------------------
//          2 + Divide[ 9 : 0]
#define F_SDCCD(x)					(x&0x3FF)
//======================================================================================================================

//======================================================================================================================
// SDHC Response Time Out Register (SDHCRTO)

#define R_SDCRTO					((volatile unsigned int*)0x8000100C)

// [ 7 : 0] R/W Response Time Out
#define F_SDCRTO(x)					(x&0xFF)
//======================================================================================================================

//======================================================================================================================
// SDHC Read Data Time Out Register (SDHCRDTO)

#define R_SDCDTO					((volatile unsigned int*)0x80001010)

// [15 : 8] R/W Data Read Time Out
#define F_SDCDTO(x)					((x&0xFF)<< 8)
//======================================================================================================================

//======================================================================================================================
// SDHC Block Length Register (SDHCBL)
#define R_SDCBL						((volatile unsigned int*)0x80001014)

// [11 : 0] R/W Block Lenth
#define F_SDCBL(x)					(x&0xFFF)
//======================================================================================================================

//======================================================================================================================
// SDHC Number of Block Register (SDHCNOB)
#define R_SDCNOB					((volatile unsigned int*)0x80001018)

// [15 : 0] R/W In Multi-block Command, Transfered Block Numner
#define F_SDCNOB(x)					(x&0xFFFF)
//======================================================================================================================

//======================================================================================================================
// SDHC Interrupt Enable Register (SDHCIE)
#define R_SDCIE						((volatile unsigned int*)0x8000101c)

// [ 7] R/W Card Insert Detection Interrupt Enable
// 0 : Disable
// 1 : Enable
#define F_SDCIE_CINSRT_INT_DIS		( 0<< 7)
#define F_SDCIE_CINSRT_INT_EN		( 1<< 7)

// [ 6] R/W Card Remove Detection Interrupt Enable
// 0 : Disable
// 1 : Enable
#define F_SDCIE_CRMVL_INT_DIS		( 0<< 6)
#define F_SDCIE_CRMVL_INT_EN		( 1<< 6)

// [ 5] R/W FIFO Full Interrupt Enable
// 0 : Disable
// 1 : Enable
#define F_SDCIE_FIFO_FULL_INT_DIS	( 0<< 5)
#define F_SDCIE_FIFO_FULL_INT_EN	( 1<< 5)

// [ 4] R/W FIFO Half Full Interrupt Enable
// 0 : Disable
// 1 : Enable
#define F_SDCIE_FIFO_HALF_INT_DIS	( 0<< 4)
#define F_SDCIE_FIFO_HALF_INT_EN	( 1<< 4)

// [ 3] R/W FIFO Empty Interrupt Enable
// 0 : Disable
// 1 : Enable
#define F_SDCIE_FIFO_EMPTY_INT_DIS	( 0<< 3)
#define F_SDCIE_FIFO_EMPTY_INT_EN	( 1<< 3)

// [ 2] R/W End Command Response Interrupt Enable
// 0 : Disable
// 1 : Enable
#define F_SDCIE_RSP_INT_DIS			( 0<< 2)
#define F_SDCIE_RSP_INT_EN			( 1<< 2)

// [ 1] R/W Write Operation Done Interrupt Enable
// 0 : Disable
// 1 : Enable
#define F_SDCIE_WRT_INT_DIS			( 0<< 1)
#define F_SDCIE_WRT_INT_EN			( 1<< 1)

// [ 0] R/W Read Operation Done Interrupt Enable
// 0 : Disable
// 1 : Enable
#define F_SDCIE_RD_INT_DIS			( 0<< 0)
#define F_SDCIE_RD_INT_EN			( 1<< 0)
//======================================================================================================================

//======================================================================================================================
// SDHC Command Control Register (SDHCCMDCON)

#define R_SDCCMDCON					((volatile unsigned int*)0x80001020)

// [10] R/W Need Response Command Type?
// 0 : No Response
// 1 : Wait Response
#define F_SDCCMDCON_CTYPE_NWRSP		( 0<<10)
#define F_SDCCMDCON_CTYPE_WRSP		( 1<<10)

// [ 9 : 8] R/W Response Type 
// 00 : Short Response(Response Size : 48bit)
// 01 : Short Response With Busy(Response Size : 48bit)
// 10 : Long Response(Response Size : 136bit)
#define F_SDCCMDCON_RTYPE_SHORT		( 0<< 8)
#define F_SDCCMDCON_RTYPE_SBUSY		( 1<< 8)
#define F_SDCCMDCON_RTYPE_LONG		( 2<< 8)

// [ 7] R/W Using Data Stream Command?
// 0 : Without Data
// 1 : With Data
#define F_SDCCMDCON_CMD_NWDATA		( 0<< 7)
#define F_SDCCMDCON_CMD_WDATA		( 1<< 7)

// [ 6] R/W FIFO Output Direction
// 0 : Read Data(Read Command)
// 1 : Write Data(Write Command)
#define F_SDCCMDCON_CMD_READ		( 0<< 6)
#define F_SDCCMDCON_CMD_WRITE		( 1<< 6)

// [ 5 : 0] R/W Command Number
// 00000 = CMD0
// 00001 = CMD1
// ...
// 11111 = CMD63
#define F_SDCCMDCON_CMD_NUMBER(x)	(x&0x3F)
//======================================================================================================================

//======================================================================================================================
// SDHC Command Argument Register (SDHCCMDA)

#define R_SDCCMDA					((volatile unsigned int*)0x80001024)

// [31 : 0] R/W Command Argument
#define F_SDCCMDA(x)				(x&0xFFFFFFFF)
//======================================================================================================================

//======================================================================================================================
// SDHC Response FIFO Access Register (SDHCRFA)
#define R_SDCRFA					(volatile unsigned int*)0x80001028

// [15 : 0] R/W Response FIFO
// 8 x 16bit
#define F_SDCRFA(x)					(x&0xFFFF)
//======================================================================================================================

//======================================================================================================================
// SDHC Data FIFO Access Register (SDHCDFA)
#define R_SDCDFA					((volatile unsigned int*)0x8000102C)

// [31 : 0] R/W Data FIFO
// 16 x 32bit
#define F_SDCDFA(x)					(x&0xFFFFFFFF)
//======================================================================================================================

//======================================================================================================================
#define R_SDCNWR					(volatile unsigned int*)0x80001030 // interval cmd,data
//======================================================================================================================

//======================================================================================================================
// SD Memory Card Commands

#define SD_MODE_CMD0				0	// GO_IDLE_STATE
#define SD_MODE_CMD2				2	// ALL_SEND_CID
#define SD_MODE_CMD3				3	// SEND_RELATIVE_ADDR
#define SD_MODE_CMD4				4	// SET_DSR
#define SD_MODE_CMD5				5	// IO_SEND_OP_COND
#define SD_MODE_CMD7				7	// SELECT/DESELECT_CARD
#define SD_MODE_CMD8				8	// SEND_IF_COND
#define SD_MODE_CMD9				9	// SEND_CSD
#define SD_MODE_CMD10				10	// SEND_CID
#define SD_MODE_CMD11				11	// VOLTAGE_SWITCH
#define SD_MODE_CMD12				12	// STOP_TRANSMISSION
#define SD_MODE_CMD13				13	// SEND_STATUS
#define SD_MODE_CMD15				15	// GO_INACTIVE_STATE
#define SD_MODE_CMD16				16	// SET_BLOCKLEN
#define SD_MODE_CMD17				17	// READ_SINGLE_BLOCK
#define SD_MODE_CMD18				18	// READ_MULTIPLE_BLOCK
#define SD_MODE_CMD19				19	// SEND_TUNING_BLOCK
#define SD_MODE_CMD20				20	// SPEED_CLASS_CONTROL
#define SD_MODE_CMD23				23	// SET_BLOCK_COUNT
#define SD_MODE_CMD24				24	// WRITE_BLOCK
#define SD_MODE_CMD25				25	// WRITE_MULTIPLE_BLOCK
#define SD_MODE_CMD27				27	// PROGRAM_CSD
#define SD_MODE_CMD28				28	// SET_WRITE_PROT
#define SD_MODE_CMD29				29	// CLR_WRITE_PROT
#define SD_MODE_CMD30				30	// SEND_WRITE_PROT
#define SD_MODE_CMD32				32	// ERASE_WR_BLK_START
#define SD_MODE_CMD33				33	// ERASE_WR_BLK_END
#define SD_MODE_CMD38				38	// ERASE
#define SD_MODE_CMD42				42	// LOCK_UNLOCK
#define SD_MODE_CMD52				52	// IO_RW_DIRECT
#define SD_MODE_CMD53				53	// IO_RW_EXTENDED
#define SD_MODE_CMD55				55	// APP_CMD
#define SD_MODE_CMD56				56	// GEN_CMD
#define SD_MODE_ACMD6				6	// SET_BUS_WIDTH
#define SD_MODE_ACMD13				13	// SD_STATUS
#define SD_MODE_ACMD22				22	// SEND_NUM_WR_BLOCKS
#define SD_MODE_ACMD23				23	// SET_WR_BLK_ERASE_COUNT
#define SD_MODE_ACMD41				41	// SD_APP_OP_COND
#define SD_MODE_ACMD42				42	// SET_CLR_CARD_DETECT
#define SD_MODE_ACMD51				51	// SEND_SCR

#define SD_MODE_GO_IDLE_STATE			SD_MODE_CMD0
#define SD_MODE_ALL_SEND_CID			SD_MODE_CMD2
#define SD_MODE_SEND_RELATIVE_ADDR		SD_MODE_CMD3
#define SD_MODE_SELECT_DESELECT_CARD	SD_MODE_CMD7
#define SD_MODE_SEND_CSD				SD_MODE_CMD9
#define SD_MODE_SEND_CID				SD_MODE_CMD10

#define SD_MODE_STOP_TRANSMISSION		SD_MODE_CMD12
#define SD_MODE_SET_BLOCKLEN			SD_MODE_CMD16
#define SD_MODE_READ_SINGLE_BLOCK		SD_MODE_CMD17
#define SD_MODE_READ_MULTIPLE_BLOCK		SD_MODE_CMD18

#define SD_MODE_WRITE_BLOCK				SD_MODE_CMD24
#define SD_MODE_WRITE_MULTIPLE_BLOCK	SD_MODE_CMD25

#define SD_MODE_ERASE_WR_BLK_START		SD_MODE_CMD32
#define SD_MODE_ERASE_WR_BLK_END		SD_MODE_CMD33
#define SD_MODE_ERASE					SD_MODE_CMD38

#define SD_MODE_APP_CMD					SD_MODE_CMD55

#define SD_MODE_SET_BUS_WIDTH			SD_MODE_ACMD6
#define SD_MODE_SD_APP_OP_COND			SD_MODE_ACMD41
//======================================================================================================================

BOOL SDC_Init();
U32 SDC_GetSectorSize();
U32 SDC_GetSectorCount();
U32 SDC_MemSize(); //return MegaByte Size
BOOL SDC_Read_Sect(void* buf,U32 sector,U32 sectorcnt);
BOOL SDC_Write_Sect(void* buf,U32 sector,U32 sectorcnt);
