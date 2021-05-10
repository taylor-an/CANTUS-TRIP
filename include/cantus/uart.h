

#pragma once

#include <typedef.h>
/***********************************************
* UART registers
***********************************************/
#define R_UART0_BASE ((volatile unsigned int*)0x80021400)
#define R_UART1_BASE ((volatile unsigned int*)0x80021420)
#define R_UART2_BASE ((volatile unsigned int*)0x80021440)
#define R_UART3_BASE ((volatile unsigned int*)0x80021460)
#define R_UART4_BASE ((volatile unsigned int*)0x80021800)
#define R_UART5_BASE ((volatile unsigned int*)0x80021820)
#define R_UART6_BASE ((volatile unsigned int*)0x80021840)
#define R_UART7_BASE ((volatile unsigned int*)0x80021860)

#define UART_RBR        0 // UART Channel Receiver Buffer Registers
#define UART_THR        0 //UART Channel Transimitter Holding Registers
#define UART_DIV_LO     0 //UART Channel Divisor Latch LSB Registers
#define UART_DIV_HI     (1*4) //UART Channel Divisor Latch MSB Registers
#define UART_IER        (1*4) //UART Channel Interrupt Enable Registers
#define UART_IIR        (2*4) //UART Channel Interrupt Identification Registers
#define UART_FCR        (2*4) //UART Channel FIFO Control Registers
#define UART_LCR        (3*4) //UART Channel Line Control Registers
#define UART_LSR        (5*4) //UART Channel Line Status Registers

typedef enum{
	DATABITS_5=5,
	DATABITS_6=6,
	DATABITS_7=7,
	DATABITS_8=8,
}UART_DATABITS;

typedef enum{
	STOPBITS_1=1,
	STOPBITS_2=2,
}UART_STOPBITS;

typedef enum{
	UART_PARNONE=0,
	UART_PARODD,
	UART_PAREVEN,
}UART_PARITY;

#define	UART_RX_USERBUF_EMPTY	2 /* Rx buffer is empty, no character available */
#define	UART_TX_USERBUF_FULL	3 /* Tx buffer is full, could not deposit char */
#define	UART_TX_USERBUF_EMPTY	4 /* If the Tx buffer is empty. */
#define	UART_RX_USERBUF_TIMEOUT	5 /* If a timeout occurred while waiting for a character*/
#define	UART_TX_USERBUF_TIMEOUT	6 /* If a timeout occurred while waiting to send a char.*/

#define UIER_RLSIEN     (1 << 2) // Recevier Line Status Interrupt Enable
#define UIER_THEIEN     (1 << 1) // Transmitter Holding Empty Interrupt Enable
#define UIER_RDAIEN     (1 << 0) // Recevied Data Available Interrupt Enable

#define UIIR_FIFOMOD    (3 << 6)        // FIFO Status 
#define UIIR_INTID      (7 << 1)        // UART Interrupt ID
#define UIIR_INTP       (1 << 0)        // UART Intterupt Pending

#define UFCR_RFTL1B     (0 << 6)        // Recevier FIFO Trigger level, 1Byte
#define UFCR_RFTL4B     (1 << 6)        // 4Byte
#define UFCR_RFTL8B     (2 << 6)        // 8Byte
#define UFCR_RFTL14B    (3 << 6)        // 14Byte
#define UFCR_XFR        (1 << 2)        // XMIT FIFO Reset
#define UFCR_RFR        (1 << 1)        // RCVR FIFO Reset
#define UFCR_FIFOEN     (1 << 0)        // Enable FIFO

#define ULCR_DLAB       (1 << 7)        // Divisor Latch Register, FIFO Control Register access 
#define ULCR_SB         (1 << 6)        // Set Break, only print to serial output '0'
#define ULCR_SP         (1 << 5)        // Stick Parity
#define ULCR_PAR_EVEN   (1 << 4)        // Select Even Parity
#define ULCR_PAR_EN     (1 << 3)        // Parity Enable
#define ULCR_STB1B      (0 << 2)        // 1 Stop Bit
#define ULCR_STB2B      (1 << 2)        // 2 Stop Bit

#define ULSR_EIRF       (1 << 7)        // Error in RCVR FIFO
#define ULSR_TEMP       (1 << 6)        // Transmitter Empty
#define ULSR_THRE       (1 << 5)        // Transmitter Holding Register Empty
#define ULSR_BINT       (1 << 4)        // Break Interrupt
#define ULSR_FERR       (1 << 3)        // Framing Error
#define ULSR_PERR       (1 << 2)        // Parity Error
#define ULSR_OERR       (1 << 1)        // Overrun Error
#define ULSR_DRDY       (1 << 0)        // Data Ready

#define UART_FIFO_DEPTH 16
#define UART_CHANNEL_MAX 8

BOOL UartConfig(int ch,int baud,UART_DATABITS databits,UART_STOPBITS stopbit,UART_PARITY parity);

BOOL UartGetCh(int n,char* ch);
int UartGetData(int n,U8* buf, int bufmax);//return data byte length
int UartGetDataSize(int n);

BOOL UartPutCh(int n, char ch);
U32 UartPutData(U32 n, U8* tbuf, U32 tsize);

void Uart_rx_flush(int ch);
void Uart_tx_flush(int ch);

void setdebugchannel(int ch);
int getdebugchannel();

void debugprintf(const char * const format, ...);
void debugstring(const char* str);

#define PRINTLINE	debugprintf("%s , %d \r\n",__FILE__,__LINE__)
#define PRINTVAR(A)	debugprintf("%s , %d :%s=0x%x(%d)\r\n",__FILE__,__LINE__,#A,A,A)
