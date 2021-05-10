
#include "cantus_lib_config.h"
#include "cantus.h"

#ifdef CONFIG_UART_RX_INTERRUPT
#warning "UART RX interrupt mode Build"
#endif
#ifdef CONFIG_UART_TX_INTERRUPT
#warning "UART TX interrupt mode Build"
#endif

/*****************************************************************************
*    UART RING BUF STRUCTURE
*****************************************************************************/
typedef struct _UART_RING_BUF {

#ifdef CONFIG_UART_RX_INTERRUPT
	U16   RingBufRxCtr;     // Number of characters in the Rx ring buffer
	U8    *RingBufRxInPtr;  // Pointer to where next character will be inserted
	U8    *RingBufRxOutPtr; // Pointer from where next character will be extracted
	U8    RingBufRx[UART_BUF_SIZE]; // Ring buffer character storage (Rx)
#endif

#ifdef CONFIG_UART_TX_INTERRUPT
	U16   RingBufTxCtr;     // Number of characters in the Tx ring buffer
	U8    *RingBufTxInPtr;    // Pointer to where next character will be inserted
	U8    *RingBufTxOutPtr; // Pointer from where next character will be extracted
	U8    RingBufTx[UART_BUF_SIZE];  // Ring buffer character storage (Tx)
#endif
} UART_RING_BUF;

#ifdef UART0_ENABLE
static UART_RING_BUF ubuf0;
#endif 
#ifdef UART1_ENABLE
static UART_RING_BUF ubuf1;
#endif 
#ifdef UART2_ENABLE
static UART_RING_BUF ubuf2;
#endif 
#ifdef UART3_ENABLE
static UART_RING_BUF ubuf3;
#endif 
#ifdef UART4_ENABLE
static UART_RING_BUF ubuf4;
#endif 
#ifdef UART5_ENABLE
static UART_RING_BUF ubuf5;
#endif 
#ifdef UART6_ENABLE
static UART_RING_BUF ubuf6;
#endif 
#ifdef UART7_ENABLE
static UART_RING_BUF ubuf7;
#endif 

static void InitRingBuf(UART_RING_BUF* pRing)
{
#ifdef CONFIG_UART_RX_INTERRUPT
	pRing->RingBufRxCtr    = 0;
	pRing->RingBufRxInPtr  = &pRing->RingBufRx[0];
	pRing->RingBufRxOutPtr = &pRing->RingBufRx[0];
#endif
#ifdef CONFIG_UART_TX_INTERRUPT
	pRing->RingBufTxCtr    = 0;
	pRing->RingBufTxInPtr  = &pRing->RingBufTx[0];
	pRing->RingBufTxOutPtr = &pRing->RingBufTx[0];
#endif
}
static void SetUartPAF(int nCh)
{
	switch (nCh)
	{
#ifdef UART0_ENABLE
	case 0:
		*R_PAF4 = ((*R_PAF4)&0xfff0)|PAF4_URX0|PAF4_UTX0;
		break;
#endif
#ifdef UART1_ENABLE
	case 1:
		*R_PAF4 = ((*R_PAF4)&0xff0f)|PAF4_URX1|PAF4_UTX1;
		break;
#endif
#ifdef UART2_ENABLE
	case 2:
		*R_PAF4 = ((*R_PAF4)&0xf0ff)|PAF4_URX2|PAF4_UTX2;
		break;
#endif
#ifdef UART3_ENABLE
	case 3:
		*R_PAF4 = ((*R_PAF4)&0x0fff)|PAF4_URX3|PAF4_UTX3;
		break;
#endif
#ifdef UART4_ENABLE
	case 4:
		*R_PAF1 = ((*R_PAF1)&0xfff0)|PAF1_URX4|PAF1_UTX4;
		break;
#endif
#ifdef UART5_ENABLE
	case 5:
		*R_PAF1 = ((*R_PAF1)&0xff0f)|PAF1_URX5|PAF1_UTX5;
		break;
#endif
#ifdef UART6_ENABLE
	case 6:
		*R_PAF1 = ((*R_PAF1)&0xf0ff)|PAF1_URX6|PAF1_UTX6;
		break;
#endif
#ifdef UART7_ENABLE
	case 7:
		*R_PAF1 = ((*R_PAF1)&0x0fff)|PAF1_URX7|PAF1_UTX7;
		break;
#endif
	default:
		break;
	}
}
static __inline__ U32 getbaseaddress(int ch)
{
	U32 uart_base;
	switch (ch)
	{
#ifdef UART0_ENABLE
	case 0:
		uart_base = (U32)R_UART0_BASE;
		break;
#endif
#ifdef UART1_ENABLE
	case 1:
		uart_base = (U32)R_UART1_BASE;
		break;
#endif
#ifdef UART2_ENABLE
	case 2:
		uart_base = (U32)R_UART2_BASE;
		break;
#endif
#ifdef UART3_ENABLE
	case 3:
		uart_base = (U32)R_UART3_BASE;
		break;
#endif
#ifdef UART4_ENABLE
	case 4:
		uart_base = (U32)R_UART4_BASE;
		break;
#endif
#ifdef UART5_ENABLE
	case 5:
		uart_base = (U32)R_UART5_BASE;
		break;
#endif
#ifdef UART6_ENABLE
	case 6:
		uart_base = (U32)R_UART6_BASE;
		break;
#endif
#ifdef UART7_ENABLE
	case 7:
		uart_base = (U32)R_UART7_BASE;
		break;
#endif
	default:
		return 0;
	}
	return uart_base;
}
static inline UART_RING_BUF* getringbuf(int ch)
{
	UART_RING_BUF* pubuf;
	switch (ch)
	{
#ifdef UART0_ENABLE
	case 0:
		pubuf = &ubuf0;
		break;
#endif
#ifdef UART1_ENABLE
	case 1:
		pubuf = &ubuf1;
		break;
#endif
#ifdef UART2_ENABLE
	case 2:
		pubuf = &ubuf2;
		break;
#endif
#ifdef UART3_ENABLE
	case 3:
		pubuf = &ubuf3;
		break;
#endif
#ifdef UART4_ENABLE
	case 4:
		pubuf = &ubuf4;
		break;
#endif
#ifdef UART5_ENABLE
	case 5:
		pubuf = &ubuf5;
		break;
#endif
#ifdef UART6_ENABLE
	case 6:
		pubuf = &ubuf6;
		break;
#endif
#ifdef UART7_ENABLE
	case 7:
		pubuf = &ubuf7;
		break;
#endif
	default:
		return 0;
	}
	return pubuf;
}

void uart_rx_int_enable(int ch,BOOL b)
{
	U32	uart_base = getbaseaddress(ch);
	if(b)
		*(U32*)(uart_base+UART_IER) |= UIER_RDAIEN;
	else
		*(U32*)(uart_base+UART_IER) &= ~UIER_RDAIEN;
}

void uart_tx_int_enable(int ch,BOOL b)
{
	U32	uart_base = getbaseaddress(ch);
	if(b)
		*(U32*)(uart_base+UART_IER) |= UIER_THEIEN;
	else
		*(U32*)(uart_base+UART_IER) &= ~UIER_THEIEN;
}
void uart_status_int_enable(int ch,BOOL b)
{
	U32	uart_base = getbaseaddress(ch);
	if(b)
		*(U32*)(uart_base+UART_IER) |= UIER_RLSIEN;
	else
		*(U32*)(uart_base+UART_IER) &= ~UIER_RLSIEN;
}

#if defined(UART0_ENABLE)|defined(UART1_ENABLE)|defined(UART2_ENABLE)|defined(UART3_ENABLE)|defined(UART4_ENABLE)|defined(UART5_ENABLE)|defined(UART6_ENABLE)|defined(UART7_ENABLE)
static void getuartfifo(U32 uart_base,UART_RING_BUF* pbuf)
{
	U32 iir;
	char ch;
	iir = *(volatile U32*)(uart_base + UART_IIR);
	iir &= 0xf;
	
	switch(iir)
	{
#ifdef CONFIG_UART_TX_INTERRUPT
	case 2:
		if(pbuf->RingBufTxCtr > 0)
		{
			int i;
			int txmax = pbuf->RingBufTxCtr>UART_FIFO_DEPTH?UART_FIFO_DEPTH:pbuf->RingBufTxCtr;
			for(i=0;i<txmax;i++)
			{
				*(volatile U8*)(uart_base+UART_THR) = *pbuf->RingBufTxOutPtr;
				pbuf->RingBufTxOutPtr++;
				if(pbuf->RingBufTxOutPtr == &pbuf->RingBufTx[UART_BUF_SIZE])
				{
					pbuf->RingBufTxOutPtr = &pbuf->RingBufTx[0];
				}
			}
			pbuf->RingBufTxCtr -= i;
		}
		
		if(pbuf->RingBufTxCtr == 0)
		{
			ch = (uart_base&0xff)>>5;
			if(uart_base&0x800)
			{
				ch += 4;
			}			
			uart_tx_int_enable(ch,FALSE);
		}
		
		break;
#endif
#ifdef CONFIG_UART_RX_INTERRUPT
	case 6:
		debugstring("UART Line Status Error(Overrun,Frame,Parity)\r\n");
	case 4:
	case 0xc:
		
		while((*(volatile U32*)(uart_base + UART_LSR)) & ULSR_DRDY)//data ready
		{
			ch = *(volatile U8*)(uart_base+UART_RBR);
			if (pbuf->RingBufRxCtr <UART_BUF_SIZE) 
			{ 
				pbuf->RingBufRxCtr++; 
				*pbuf->RingBufRxInPtr=ch;
				pbuf->RingBufRxInPtr++;
				/* Wrap OUT pointer     */
				if (pbuf->RingBufRxInPtr == &pbuf->RingBufRx[UART_BUF_SIZE]) {
					pbuf->RingBufRxInPtr = &pbuf->RingBufRx[0];
				}
			}
		}
		break;
#endif
	}
}
#endif

#ifdef UART0_ENABLE
void Uart0ISR()
{
	getuartfifo((U32)R_UART0_BASE,&ubuf0);
}
#else
void Uart0ISR()
{
	
	debugstring("UART0 Interrupt: UART0 Not enabled at Cantus Library\r\n");
	debugprintf("%s, %d\r\n",__FILE__,__LINE__);
}
#endif

#ifdef UART1_ENABLE
void Uart1ISR()
{
	getuartfifo((U32)R_UART1_BASE,&ubuf1);
}
#else
void Uart1ISR()
{
	
	debugstring("UART1 Interrupt: UART1 Not enabled at Cantus Library\r\n");
	debugprintf("%s, %d\r\n",__FILE__,__LINE__);
}
#endif

#ifdef UART2_ENABLE
void Uart2ISR()
{
	getuartfifo((U32)R_UART2_BASE,&ubuf2);
}
#else
void Uart2ISR()
{
	
	debugstring("UART2 Interrupt: UART2 Not enabled at Cantus Library\r\n");
	debugprintf("%s, %d\r\n",__FILE__,__LINE__);
}
#endif

#ifdef UART3_ENABLE
void Uart3ISR()
{
	getuartfifo((U32)R_UART3_BASE,&ubuf3);
}
#else
void Uart3ISR()
{
	
	debugstring("UART3 Interrupt: UART3 Not enabled at Cantus Library\r\n");
	debugprintf("%s, %d\r\n",__FILE__,__LINE__);
}
#endif

#ifdef UART4_ENABLE
void Uart4ISR()
{
	getuartfifo((U32)R_UART4_BASE,&ubuf4);
}
#else
void Uart4ISR()
{
	
	debugstring("UART4 Interrupt: UART4 Not enabled at Cantus Library\r\n");
	debugprintf("%s, %d\r\n",__FILE__,__LINE__);
}
#endif

#ifdef UART5_ENABLE
void Uart5ISR()
{
	getuartfifo((U32)R_UART5_BASE,&ubuf5);
}
#else
void Uart5ISR()
{
	
	debugstring("UART5 Interrupt: UART5 Not enabled at Cantus Library\r\n");
	debugprintf("%s, %d\r\n",__FILE__,__LINE__);
}
#endif

#ifdef UART6_ENABLE
void Uart6ISR()
{
	getuartfifo((U32)R_UART6_BASE,&ubuf6);
}
#else
void Uart6ISR()
{
	
	debugstring("UART6 Interrupt: UART6 Not enabled at Cantus Library\r\n");
	debugprintf("%s, %d\r\n",__FILE__,__LINE__);
}
#endif

#ifdef UART7_ENABLE
void Uart7ISR()
{
	getuartfifo((U32)R_UART7_BASE,&ubuf7);
}
#else
void Uart7ISR()
{
	
	debugstring("UART7 Interrupt: UART7 Not enabled at Cantus Library\r\n");
	debugprintf("%s, %d\r\n",__FILE__,__LINE__);
}
#endif

BOOL UartConfig(int ch,int baud,UART_DATABITS databits,UART_STOPBITS stopbit,UART_PARITY parity)
{
	UART_RING_BUF* pubuf;
	U32 apbclock;
	volatile U32 uart_base;
	pubuf = getringbuf(ch);

	uart_base = getbaseaddress(ch);
	if(uart_base==0)
		return FALSE;

	SetUartPAF(ch);
	InitRingBuf(pubuf);
	
	apbclock = GetAPBclock();
	U16 divisor = apbclock/(baud*16) ;
	if( (apbclock%(baud*16)) > ((baud*16)/2))
		divisor++;
	U32 lcr;//line control register
	lcr = ((stopbit - 1) << 2) + (databits - 5);
	switch (parity) 
	{
		case UART_PARODD:
			lcr |= ULCR_PAR_EN;
			break;
		case UART_PAREVEN:
			lcr |= ULCR_PAR_EN | ULCR_PAR_EVEN;
			break;
		case UART_PARNONE:
		default:
			break;
	}
	
	*(volatile U32*)(uart_base + UART_FCR) = UFCR_FIFOEN|UFCR_RFTL1B;
	lcr |= ULCR_DLAB;
	*(volatile U32*)(uart_base + UART_LCR) = lcr;
	*(volatile U32*)(uart_base + UART_DIV_LO) = divisor&0xff;
	*(volatile U32*)(uart_base + UART_DIV_HI) = (divisor>>8)&0xff;
	lcr &= ~ULCR_DLAB; //divisor latch access disable, fifo control write mode.
	*(volatile U32*)(uart_base + UART_LCR) = lcr;
	*(volatile U32*)(uart_base + UART_IER) = 0;//disable rx/tx interrupts
	Uart_rx_flush(ch);
	Uart_tx_flush(ch);
#if defined(CONFIG_UART_RX_INTERRUPT)||defined(CONFIG_UART_TX_INTERRUPT)
	
	switch(ch)
	{
#ifdef UART0_ENABLE		
	case 0:
		*R_IINTMOD &= ~(1<<INTNUM_UART0);//for the first empty interrupt
		setinterrupt(INTNUM_UART0,Uart0ISR);
		EnableInterrupt(INTNUM_UART0,TRUE);
		break;
#endif		
#ifdef UART1_ENABLE				
	case 1:
		*R_IINTMOD &= ~(1<<INTNUM_UART1);
		setinterrupt(INTNUM_UART1,Uart1ISR);
		EnableInterrupt(INTNUM_UART1,TRUE);
		break;
#endif		
#ifdef UART2_ENABLE				
	case 2:
		*R_IINTMOD &= ~(1<<INTNUM_UART2);
		setinterrupt(INTNUM_UART2,Uart2ISR);
		EnableInterrupt(INTNUM_UART2,TRUE);
		break;
#endif		
#ifdef UART3_ENABLE						
	case 3:
		*R_IINTMOD &= ~(1<<INTNUM_UART3);
		setinterrupt(INTNUM_UART3,Uart3ISR);
		EnableInterrupt(INTNUM_UART3,TRUE);
		break;
#endif		
#ifdef UART4_ENABLE
	case 4:
		*R_IINTMOD &= ~(1<<INTNUM_UART4_OCR2A);
		setinterrupt(INTNUM_UART4_OCR2A,Uart4ISR);
		EnableInterrupt(INTNUM_UART4_OCR2A,TRUE);
		break;
#endif		
#ifdef UART5_ENABLE						
	case 5:
		*R_IINTMOD &= ~(1<<INTNUM_UART5_OCR2B);
		setinterrupt(INTNUM_UART5_OCR2B,Uart5ISR);
		EnableInterrupt(INTNUM_UART5_OCR2B,TRUE);
		break;
#endif		
#ifdef UART6_ENABLE						
	case 6:
		*R_IINTMOD &= ~(1<<INTNUM_UART6_OCR3A);
		setinterrupt(INTNUM_UART6_OCR3A,Uart6ISR);
		EnableInterrupt(INTNUM_UART6_OCR3A,TRUE);
		break;
#endif		
#ifdef UART7_ENABLE						
	case 7:
		*R_IINTMOD &= ~(1<<INTNUM_UART7_OCR3B);
		setinterrupt(INTNUM_UART7_OCR3B,Uart7ISR);
		EnableInterrupt(INTNUM_UART7_OCR3B,TRUE);
		break;
#endif		
	}
	
	
#endif
#ifdef CONFIG_UART_RX_INTERRUPT
	uart_rx_int_enable(ch,TRUE);
#endif
	return TRUE;
}

void Uart_rx_flush(int ch)
{
	U32 uart_base;
	U32 uart_fcr;
	
	uart_base = getbaseaddress(ch);
	
	CRITICAL_ENTER();

	// For Read UART Channel FIFO Control Register
	*(volatile U32*)(uart_base + UART_LCR) |= ULCR_DLAB;
	uart_fcr = *(volatile U32*)(uart_base + UART_FCR);
	
	// For Write UART Channel FIFO Control Register
	*(volatile U32*)(uart_base + UART_LCR) &= ~ULCR_DLAB;
	*(volatile U32*)(uart_base + UART_FCR) = uart_fcr | UFCR_RFR;

	CRITICAL_EXIT();
}
void Uart_tx_flush(int ch)
{
	U32 uart_base;
	U32 uart_fcr;
	
	uart_base = getbaseaddress(ch);
	
	CRITICAL_ENTER();
	
	// For Read UART Channel FIFO Control Register
	*(volatile U32*)(uart_base + UART_LCR) |= ULCR_DLAB;
	uart_fcr = *(volatile U32*)(uart_base + UART_FCR);
	
	// For Write UART Channel FIFO Control Register
	*(volatile U32*)(uart_base + UART_LCR) &= ~ULCR_DLAB;
	*(volatile U32*)(uart_base + UART_FCR) = uart_fcr | UFCR_XFR;
	
	CRITICAL_EXIT();
}

BOOL UartGetCh(int n,char* ch)
{
#ifdef CONFIG_UART_RX_INTERRUPT
	UART_RING_BUF* pbuf = getringbuf(n);
	if(pbuf==0)
		return FALSE;
	CRITICAL_ENTER();
	if (pbuf->RingBufRxCtr > 0) { /* No, decrement character count */
		pbuf->RingBufRxCtr--; /* No, decrement character count */
		*ch = *pbuf->RingBufRxOutPtr++; /* Get character from buffer */
		/* Wrap OUT pointer     */
		if (pbuf->RingBufRxOutPtr == &pbuf->RingBufRx[UART_BUF_SIZE]) {
			pbuf->RingBufRxOutPtr = &pbuf->RingBufRx[0];
		}
		CRITICAL_EXIT();
		return TRUE;
	}
	else
	{
		CRITICAL_EXIT();
		return FALSE;
	}	

#else
	U32 uart_base = getbaseaddress(n);
	if(uart_base==0)
		return FALSE;
	U32 lsr = *(U32*)(uart_base + UART_LSR);
	if(lsr & ULSR_DRDY)
	{
		*ch = *(u8*)(uart_base+UART_RBR);
		return TRUE;
	}
	else
		return FALSE;
#endif
}
int UartGetData(int n,U8* buf, int bufmax)
{
	int i;
#ifdef CONFIG_UART_RX_INTERRUPT
	UART_RING_BUF* pbuf = getringbuf(n);
	if(pbuf==0)
		return FALSE;
	CRITICAL_ENTER();
	for(i=0;i<bufmax;i++)
	{
		if (pbuf->RingBufRxCtr > 0)
		{
			/* No, decrement character count */
			pbuf->RingBufRxCtr--; /* No, decrement character count */
			buf[i] = *pbuf->RingBufRxOutPtr++; /* Get character from buffer */
			/* Wrap OUT pointer     */
			if (pbuf->RingBufRxOutPtr == &pbuf->RingBufRx[UART_BUF_SIZE])
			{
				pbuf->RingBufRxOutPtr = &pbuf->RingBufRx[0];
			}
		}
		else
		{
			break;
		}	
	}
	CRITICAL_EXIT();

#else
	U32 uart_base = getbaseaddress(n);
	if(uart_base==0)
		return FALSE;
	for(i=0;i<bufmax;i++)
	{
		U32 lsr = *(U32*)(uart_base + UART_LSR);
		if(lsr & ULSR_DRDY)
		{
			buf[i] = *(u8*)(uart_base+UART_RBR);
		}
		else
			break;
	}
#endif
	return i;
}

int UartGetDataSize(int n)
{
#ifdef CONFIG_UART_RX_INTERRUPT
	UART_RING_BUF* pbuf = getringbuf(n);
	if(pbuf==0)
	{
		return FALSE;
	}
	return pbuf->RingBufRxCtr;
#endif
	return 0;
}
BOOL UartPutCh(int n,char ch)
{
	U32 uart_base = getbaseaddress(n);
	if(uart_base==0)
		return FALSE;
#ifdef CONFIG_UART_TX_INTERRUPT
	UART_RING_BUF* pbuf = getringbuf(n);
	if(pbuf==0)
		return FALSE;
	CRITICAL_ENTER();
	if (pbuf->RingBufTxCtr < UART_BUF_SIZE) 
	{
		pbuf->RingBufTxCtr++;    /* No, increment character count*/
		*pbuf->RingBufTxInPtr = ch;    /* Put character into buffer*/
		pbuf->RingBufTxInPtr++;
		if (pbuf->RingBufTxInPtr == &pbuf->RingBufTx[UART_BUF_SIZE]) 
		{
			pbuf->RingBufTxInPtr = &pbuf->RingBufTx[0];
		}
		if((*(U32*)(uart_base+UART_IER) & UIER_THEIEN)==0)
		{
			uart_tx_int_enable(n,TRUE);
		}
		CRITICAL_EXIT();
		return TRUE;
	}
	else 
	{
		// buffer full, wait until the previous data is out
		*(volatile U32*)(uart_base + UART_IER) &= ~UIER_THEIEN;
		
		if(pbuf->RingBufTxCtr > 0)
		{
			int i;
			int txmax = pbuf->RingBufTxCtr > UART_FIFO_DEPTH ? UART_FIFO_DEPTH : pbuf->RingBufTxCtr;
			
			for(i = 0; i < txmax; i++)
			{
				*(volatile U8 *)(uart_base + UART_THR) = *pbuf->RingBufTxOutPtr;
				pbuf->RingBufTxOutPtr++;
				
				if(pbuf->RingBufTxOutPtr == &pbuf->RingBufTx[UART_BUF_SIZE])
				{
					pbuf->RingBufTxOutPtr = &pbuf->RingBufTx[0];
				}
			}
			pbuf->RingBufTxCtr -= i;
		}
		
		if(pbuf->RingBufTxCtr > 0)
		{
			*(volatile U32*)(uart_base + UART_IER) |= UIER_THEIEN;
		}
		
		CRITICAL_EXIT();
		return FALSE;
	}
#else
	//wait empty
	while(!((*(volatile U32*)(uart_base + UART_LSR))  & ULSR_TEMP));
	*(volatile u8*)(uart_base+UART_THR) = ch;
	return TRUE;
#endif
}

U32 UartPutData(U32 n, U8* tbuf, U32 tsize)
{
	U32 i;
	
#ifdef CONFIG_UART_TX_INTERRUPT
	
	UART_RING_BUF* pbuf = getringbuf(n);	
	U32 emptysize = UART_BUF_SIZE - (pbuf->RingBufTxCtr);
	U32 uart_base = getbaseaddress(n);
	
	if(tsize > emptysize)
	{
		tsize = emptysize;
	}
	
	CRITICAL_ENTER();
	for(i=0; i<tsize; i++)
	{
		pbuf->RingBufTxCtr++;
		*pbuf->RingBufTxInPtr = *tbuf;
		pbuf->RingBufTxInPtr++;
		if(pbuf->RingBufTxInPtr == &pbuf->RingBufTx[UART_BUF_SIZE])
		{
			pbuf->RingBufTxInPtr = &pbuf->RingBufTx[0];
		}
		tbuf++;
	}
	CRITICAL_EXIT();
	
	if((*(U32*)(uart_base+UART_IER) & UIER_THEIEN)==0)
	{
		uart_tx_int_enable(n, TRUE);
	}
	
#else
	
	for(i=0; i<tsize; i++)
	{
		UartPutCh(n, *tbuf);
		tbuf++;
	}
	
#endif
	
	return tsize;
	
}

static int debugch=DEBUG_CHANNEL;
void setdebugchannel(int ch)
{
	if(ch > 7)
		return;
	debugch = ch;
}

int getdebugchannel()
{
	return debugch;
}

void debugstring(const char* str)
{
	U32 uart_base = getbaseaddress(debugch);
	
	while(*str)
	{
		while(!((*(volatile U32*)(uart_base + UART_LSR))  & ULSR_TEMP));
		*(volatile u8*)(uart_base+UART_THR) = *str;
		
		str++;
	}
}
