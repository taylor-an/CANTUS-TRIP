#pragma once

#define OSC_CLOCK					11289600 //11.2896Mhz

/***********************************************
	Timer
***********************************************/
#define CONFIG_TIMER_PRESCALE		2
/*
APB : 48Mhz
PRESCALE 2: 	tick = 41 nsec, 	maxms 176093 ms
PRESCALE 8:		tick = 166 nsec, 	maxms 712964 ms
PRESCALE 32:	tick = 666 nsec, 	maxms 2860448 ms
PRESCALE 128:	tick = 2666 nsec, 	maxms 11450382 ms
PRESCALE 512:	tick = 10666 nsec,	maxms 45810121 ms
PRESCALE 8192:	tick = 170677 nsec, maxms 733052133 ms
PRESCALE 32758:	tick = 683060 nsec, maxms 0xffffffff ms
*/

/***********************************************
	Uart Config 
***********************************************/
//#define UART0_ENABLE
//#define UART1_ENABLE
//#define UART2_ENABLE
//#define UART3_ENABLE
//#define UART4_ENABLE
//#define UART5_ENABLE
//#define UART6_ENABLE
#define UART7_ENABLE

#define UART_BUF_SIZE				128
#define CONFIG_UART_RX_INTERRUPT
// #define CONFIG_UART_TX_INTERRUPT
#define DEBUG_CHANNEL				7
#define CONFIG_DEBUGPRINTF_ENABLE //debugprintf funcion , if enabled, 1kbyte need

/***********************************************
	I2S
***********************************************/
#define CONFIG_I2S_BUF_LIST_MAX		3 // One List size is fixed fo  (1152*4)byte
#define CONFIG_I2S_USE_DMA
/***********************************************
	Key Scan
***********************************************/
// you should set keyscaninit(KEYMODE_BOTH) to get key-sustain state.
#define CONFIG_KEY_SUSTAIN_SUPPORT
//if key-scan mode is both and CONFIG_KEY_SUSTAIN_SUPPORT is defined, this timer used for key-sustain.
// Don't use timer 7.
#define CONFIG_KEY_SUSTAIN_TIMER	6

/*********************************************
	NAND FLASH
*********************************************/
#define NAND_ECC					0

#if NAND_ECC != 0
#define ECC_TEST					0
#endif

/*********************************************
	FAT support media
*********************************************/
#define DRIVE_NAND					0
#define DRIVE_SDCARD				1
#define DRIVE_SPIFLASH				2

#define CONFIG_FAT_READONLY			0
#define CONFIG_FAT_VOLUMES			3

/*
	아래 설정 값은 include/fatfs/ffconf.h 파일을 참조.
	영문만을 사용 할 경우 
		CONFIG_CODE_PAGE 437 로 설정
		library 프로젝트에 lib_src/fatfs/option/ccsbcs.c 파일을 추가 한 후 rebuild 한다
	한글을 사용 할 경우 
		약 136,408 byte 가 늘어 난다. 
		CONFIG_CODE_PAGE 949 로 설정
		library 프로젝트에 lib_src/fatfs/option/cc949.c 파일을 추가 한 후 rebuild 한다
*/
// #define CONFIG_CODE_PAGE			437
#define CONFIG_CODE_PAGE			949

#define CONFIG_FAT_NAND_SUPPORT		0
#define CONFIG_FAT_SDCARD_SUPPORT	0
#define CONFIG_FAT_SPIFLASH_SUPPORT	0

