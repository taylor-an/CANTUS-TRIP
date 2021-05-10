#pragma once

#define R_KSCTRL	((volatile unsigned int*)0x80023000)
#define R_KSCNT	((volatile unsigned int*)0x80023004)
#define R_KSDATA1	((volatile unsigned int*)0x80023008)
#define R_KSDATA2	((volatile unsigned int*)0x8002300c)

typedef enum{
	KEYMODE_PRESS=0,
	KEYMODE_BOTH=(1<<2)
} KEYSCAN_MODE;

typedef enum{
	KEYTYPE_NONE=0,
	KEYTYPE_PRESSED,
	KEYTYPE_SUSTAIN, // not supported when KEYMODE_PRESS mode
	KEYTYPE_RELEASED,// not supported when KEYMODE_PRESS mode
}KEYTYPE;

typedef struct {
	unsigned char val;
	KEYTYPE type;
}KEY; 

// EVM 4.1
#define EVM_F_KEY_SW3		9
#define EVM_F_KEY_SW4		5
#define EVM_F_KEY_SW5		1
#define EVM_F_KEY_SW6		10
#define EVM_F_KEY_SW7		6
#define EVM_F_KEY_SW8		2

void keyscaninit(KEYSCAN_MODE mode);
BOOL getkeyscan(KEY* key);
