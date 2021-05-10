#include "cantus_lib_config.h"
#include "cantus.h"

#define I2S_BUF_SIZE (1152) //in 4byte size,for MAD(1152*4),
#ifndef CONFIG_I2S_BUF_LIST_MAX
#define CONFIG_I2S_BUF_LIST_MAX 3
#endif

#if CONFIG_I2S_BUF_LIST_MAX < 3
#define CONFIG_I2S_BUF_LIST_MAX 3
#endif

#ifdef CONFIG_I2S_USE_DMA
#define I2S_DMA_TRANSFER
#endif

#define WAVELIST_MAX CONFIG_I2S_BUF_LIST_MAX

#define I2S_MEMCPY	memcpy
void WaveData_transfer();
__inline__ void I2S_TxEnable(U8 b)
{
    if (b)
    {
        // at least, Tx buffer must have 16Word
        
        if ((*R_I2SCON & I2SCON_I2S_START)==0)
        {
            if ((*R_I2SCON & I2SCON_TRANSMIT_PAUSEEN)==0)			//enabled and not paused
            {
#ifdef I2S_DMA_TRANSFER
                *R_I2SCON = I2SCON_TRANSMIT_DMA|I2SCON_TRANSMIT_INT|I2SCON_MCLK_EN|I2SCON_I2S_START;//tx enable
                WaveData_transfer();
#else
                *R_I2SCON = I2SCON_TRANSMIT_INT|I2SCON_MCLK_EN|I2SCON_I2S_START;//tx enable
#endif
				// debugstring("I2S Enabled\r\n");
            }
        }
    }
    else
    {
		// debugstring("I2S_Disabled\r\n");
        *R_I2SCON = 0;//Transmit Disable
        *R_I2SMOD = 0;
#ifdef I2S_DMA_TRANSFER
        *R_GDMACON0=0;
#endif
    }
}

#ifdef I2S_DMA_TRANSFER

typedef struct sWaveData
{
    U32 buf[I2S_BUF_SIZE];
    U32 bytesize;
    struct sWaveData *next;
}WAVEDATALIST;

static WAVEDATALIST wavedatalist[WAVELIST_MAX];
static U32 total_data_listcount=0;//to play

WAVEDATALIST* pHeadList = &wavedatalist[0]; //ready bufffer to play
WAVEDATALIST* pTailList = &wavedatalist[0]; //ready bufffer to insert

void WaveData_transfer()
{
    U32* pbuf;
    dcache_invalidate_way();
    pbuf = pHeadList->buf;
    *R_GDMAD0 = (U32)R_I2SDATA;
    *R_GDMAS0 = (U32)pbuf;
    if ( (U32)pHeadList->bytesize % 32)
    {
        *R_GDMAT0 = (U32)pHeadList->bytesize/4;//4byte, I2S request at half empty
        *R_GDMACON0 = DMA_SIZE_32BIT | DMA_DESTADDR_FIX | DMA_SRCADDR_INC | DMA_BURST_NO |DMA_START_BY_SW |DMA0_REQ_SRC_I2S;
    }
    else
    {
        *R_GDMAT0 = (U32)pHeadList->bytesize/32;//4byte*8beat burst, I2S request at half empty
        *R_GDMACON0 = DMA_SIZE_32BIT | DMA_DESTADDR_FIX | DMA_SRCADDR_INC | DMA_BURST_8BEAT |DMA_START_BY_SW |DMA0_REQ_SRC_I2S;
    }
}

static WAVEDATALIST* InsertWaveBuf(void* buf, U32 wordsize)
{
    U32 bytesize = wordsize*4;
    U32 writelen=0;
    
    if ( (WAVELIST_MAX - total_data_listcount) < (wordsize/I2S_BUF_SIZE + 1))
    {
        return 0;
    }
    
    while (bytesize>0)
    {
        writelen = wordsize>I2S_BUF_SIZE ? I2S_BUF_SIZE : wordsize;
        writelen *=4;//word to byte
        pTailList->bytesize = writelen;
        I2S_MEMCPY(pTailList->buf,buf,writelen);
        bytesize -= writelen;
        pTailList = pTailList->next;
        CRITICAL_ENTER();
        total_data_listcount ++;
        CRITICAL_EXIT();
        buf = ((U8*)buf)+writelen;
    }
    
    return pTailList;
}
void I2S_BufClearAll()
{
    int i;
    for (i=0;i<WAVELIST_MAX-1;i++)
    {
        wavedatalist[i].next = &wavedatalist[i+1];
    }
    wavedatalist[i].next = &wavedatalist[0];
    pHeadList = pTailList = &wavedatalist[0];
    
    total_data_listcount=0;
}
static int (*I2S_Tx_ISRHook)(U32* buf,int readbytesize)=0;
static int (*I2S_Tx_EndHook)()=0;
void I2S_Tx_ISR()
{
    pHeadList = pHeadList->next;
    if (pHeadList == pTailList)
    {
        I2S_TxEnable(FALSE);
		if(I2S_Tx_EndHook)
			I2S_Tx_EndHook();
    }
    else
    {
        WaveData_transfer();
		total_data_listcount --;
		if(I2S_Tx_ISRHook)
		{
			int readlen;
			readlen = I2S_Tx_ISRHook(pTailList->buf,I2S_BUF_SIZE*4);
			if(readlen)
			{
				pTailList->bytesize = readlen;
				pTailList = pTailList->next;
				total_data_listcount ++;
			}
		}
    }
	
}

void I2S_Set_TxISRHook(int (*readfunc)(U32* buf,int readbytesize))
{
	if(readfunc)
		I2S_Tx_ISRHook = readfunc;
}
void I2S_Set_TxEndHook(int (*func)())
{
	if(func)
		I2S_Tx_EndHook = func;
}

I2S_ERROR I2S_TxData(U32* buf, U32 wordsize)
{
    if (InsertWaveBuf(buf,wordsize))
    {
        I2S_TxEnable(TRUE);
        return I2S_ERR_NO;
    }
    return I2S_ERR_BUF_FULL;
}

U32 I2S_GetMaxBufSize()
{
    return I2S_BUF_SIZE*WAVELIST_MAX;
}

#else


typedef struct _I2S_BUF
{
    volatile int     TxCnt;     // Number of characters in the Tx ring buffer
    U32   *pBufTxIn;    // Pointer to where next word will be inserted
    U32   *pBufTxOut; // Pointer from where next word will be extracted
    U32   BufTx[I2S_BUF_SIZE];  // Ring buffer word storage (Tx)
} I2S_BUF;

static I2S_BUF I2S_Buf;
#define I2S_BUF_END (&I2S_Buf.BufTx[I2S_BUF_SIZE])


void I2S_BufClearAll()
{
    I2S_Buf.TxCnt = 0;
    I2S_Buf.pBufTxIn = I2S_Buf.pBufTxOut = I2S_Buf.BufTx;
    volatile U32 dummy;
    int i;
    for (i=0;i<32;i++)//if not shared mode , max 128Mbyte
        dummy = *R_I2SDATA;
}

void I2S_Tx_ISR (void)
{
    U32 status;
    U8 i;
    U32 emptycount;
    status  = *R_I2SSTAT;
    emptycount = (status>>8)&0xff;
    emptycount = 32-emptycount;

	if (I2S_Buf.TxCnt == 0)
	{
		I2S_TxEnable(FALSE);
	}
	else
	{
		if (emptycount < I2S_Buf.TxCnt)
		{
			if ((I2S_Buf.pBufTxOut + emptycount) < I2S_BUF_END)
			{
				for (i=0;i<emptycount;i++)
				{
					*R_I2SDATA = *I2S_Buf.pBufTxOut;
					I2S_Buf.pBufTxOut++;
				}
			}
			else
			{
				for (i=0;i<emptycount;i++)
				{
					*R_I2SDATA = *I2S_Buf.pBufTxOut;
					I2S_Buf.pBufTxOut++;
					if (I2S_Buf.pBufTxOut == &I2S_Buf.BufTx[I2S_BUF_SIZE])
						I2S_Buf.pBufTxOut = I2S_Buf.BufTx;
				}
			}
			I2S_Buf.TxCnt -= emptycount;
		}
		else
		{
			for (i=0;i<I2S_Buf.TxCnt;i++)
			{
				*R_I2SDATA = *I2S_Buf.pBufTxOut;
				I2S_Buf.pBufTxOut++;
				if (I2S_Buf.pBufTxOut == &I2S_Buf.BufTx[I2S_BUF_SIZE])
					I2S_Buf.pBufTxOut = I2S_Buf.BufTx;
			}
			I2S_Buf.TxCnt = 0;
		}
	}
}

static U32 I2S_tx_Buf_EmptySize()
{
    CRITICAL_ENTER();
    U32 re = I2S_BUF_SIZE - I2S_Buf.TxCnt;
    CRITICAL_EXIT();
    return re;
}
I2S_ERROR I2S_TxData(U32* buf, U32 len)
{
    int validcnt=0;
    validcnt = I2S_tx_Buf_EmptySize();
    if (validcnt >= len)
    {
        validcnt = (I2S_BUF_END - I2S_Buf.pBufTxIn);//4byte cnt , don't divide by 4, this is pointer.
        if (validcnt > len)
            validcnt = len;
        memcpy(I2S_Buf.pBufTxIn,buf,validcnt*4);
        I2S_Buf.pBufTxIn += validcnt;
        if ( I2S_Buf.pBufTxIn == I2S_BUF_END)
        {
            I2S_Buf.pBufTxIn = I2S_Buf.BufTx;
        }
        buf += validcnt;
        if (validcnt < len)
        {
            validcnt = len - validcnt;
            memcpy(I2S_Buf.pBufTxIn,buf,validcnt*4);
            I2S_Buf.pBufTxIn += validcnt;
            if ( I2S_Buf.pBufTxIn == I2S_BUF_END)
                I2S_Buf.pBufTxIn = I2S_Buf.BufTx;
        }

        CRITICAL_ENTER();
        I2S_Buf.TxCnt+=len;
        CRITICAL_EXIT();
        I2S_TxEnable(TRUE);
        return I2S_ERR_NO;
    }
    return I2S_ERR_BUF_FULL;
}

#endif

U8 vol_i2s=0;
BOOL I2S_TxInit(U32 mode, I2SFreq freq)
{
    I2S_Stop();
    *R_PMCTRLEN |= PMCTRL_CLKSEL_EN|PMCTRL_SCLK_GATE_EN;//SCLK,CKR write enable
    *R_SCLKGATE |= SCLK_GATE_MCLK_EN;//MCLK
    *R_CKR	&= (~CKR_MCLK_CAP);//Xin
    *R_PMCTRLEN = 0;//CKR write disable
    
	// For External Audio Codec
    *R_PAF4 = ((*R_PAF4) & 0xfff0) | PAF4_I2S_MOD|PAF4_I2S_SDI;
    *R_PAF5 = ((*R_PAF5) & 0xfc0f) | PAF5_I2S_LRCK | PAF5_I2S_MCLK | PAF5_I2S_SCLK;
    
    *R_IINTMOD &= ~(1<<2);//I2S level mode
    
    I2S_BufClearAll();
#ifdef I2S_DMA_TRANSFER
    setinterrupt(INTNUM_DMA0,I2S_Tx_ISR);
    EnableInterrupt(INTNUM_DMA0,TRUE);
#else
    setinterrupt(INTNUM_I2S,I2S_Tx_ISR);
    EnableInterrupt(INTNUM_I2S,TRUE);
#endif
    // Set MCLK clock, 256fs
    
    switch (freq)
    {
    case FREQ_11025:
        debugstring("I2S Tx Init, 11.025Khz \r\n");
        *R_I2SPRE = I2SPS_MSTR_EXTCLK|3; //2.8224Mhz
        break;
    case FREQ_22050:
        debugstring("I2S Tx Init, 22.05Khz \r\n");
        *R_I2SPRE = I2SPS_MSTR_EXTCLK|1;//5.6448Mhz
        break;
    case FREQ_44100:
        debugstring("I2S Tx Init, 44.1Khz \r\n");
        *R_I2SPRE = I2SPS_MSTR_EXTCLK;//11.2896Mhz
        break;
    default:
        debugprintf("%s:invalid frequency\r\n",__FUNCTION__);
        return FALSE;
    }
	
#if 0
	// MSB(Left)-Justified Format
    mode |= I2SMOD_FIFO_SHARE | I2SMOD_MODE_TRANSMIT | I2SMOD_MSB_FMT | I2SMOD_MCLK_256 | I2SMOD_SLK_32 | I2SMOD_TX_VOLUME_EN;
	
#else
	// I2S-bus Format
    mode |= I2SMOD_FIFO_SHARE | I2SMOD_MODE_TRANSMIT | I2SMOD_MCLK_256 | I2SMOD_SLK_32 | I2SMOD_TX_VOLUME_EN;
	
#endif
	
    *R_I2SMOD = mode;
	
	*R_I2SVOL = (vol_i2s<<24) | (vol_i2s<<16);
	debugprintf("I2S Vol %d \r\n",vol_i2s);
	
    return TRUE;
}

void I2S_Stop()
{
    I2S_TxEnable(FALSE);
}

void I2S_Pause(BOOL b)
{
    if (b)
	{
        *R_I2SCON |= I2SCON_TRANSMIT_PAUSEEN;
	}
    else
	{
        *R_I2SCON &= ~I2SCON_TRANSMIT_PAUSEEN;
	}
}

BOOL I2S_Running()
{
    if( (*R_I2SSTAT & 0xffff) && (*R_I2SCON & 1))
	{
        return TRUE;
	}
    else
	{
        return FALSE;
	}
}
