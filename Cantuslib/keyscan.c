#include "cantus_lib_config.h"
#include "cantus.h"

//#undef CONFIG_KEY_SUSTAIN_SUPPORT

#ifdef CONFIG_KEY_SUSTAIN_SUPPORT
#define KEY_SUSTAIN_SUPPORT
#endif
#ifdef CONFIG_KEY_SUSTAIN_TIMER
#define SUSTAIN_TIMER	CONFIG_KEY_SUSTAIN_TIMER
#else
#define SUSTAIN_TIMER 6
#endif 
#define SUSTAIN_CHECK_TIME 1000//ms

#if SUSTAIN_TIMER==7
#error "Can not timer 7 as key sustain checking"
#endif

#define KEY_BUF_SIZE 4

#define KEYCON_PRESS_MODE	0
#define KEYCON_BOTH_MODE	(1<<2)
#define KEYCON_ENABLE		1

static struct _tagkeybuf
{
	KEY* InPtr;
	KEY* OutPtr;
	int  Ctr;
	KEY key[KEY_BUF_SIZE];
}keybuf;
static U8 oldkeydata=0;//for the both mode
void KeyScanBufClear()
{
	CRITICAL_ENTER();
	keybuf.InPtr = keybuf.OutPtr = &keybuf.key[0];
	keybuf.Ctr=0;
	CRITICAL_EXIT();
}

#ifdef KEY_SUSTAIN_SUPPORT
static BOOL bsustain=FALSE;
void suschecktimerisr()
{
	bsustain = TRUE;
}
#endif

void KeyScan_ISR()
{
	U8 data;
	data = *R_KSDATA2;
	if(*R_KSCTRL & KEYCON_BOTH_MODE)
	{
		KEYTYPE type;
		if(oldkeydata==0 && data) //pressed
		{
			type = KEYTYPE_PRESSED;
#ifdef KEY_SUSTAIN_SUPPORT
			settimer(SUSTAIN_TIMER,SUSTAIN_CHECK_TIME);
#endif
			oldkeydata = data;
		}
		else if(oldkeydata!=0 && data==0) //released
		{
			type = KEYTYPE_RELEASED;
#ifdef KEY_SUSTAIN_SUPPORT
			stoptimer(SUSTAIN_TIMER);
			bsustain = FALSE;
#endif
			U8 temp;
			temp = oldkeydata;
			oldkeydata = data;
			data = temp;
		}
		else
		{
			oldkeydata = data;
			return;
		}
		if(keybuf.Ctr<KEY_BUF_SIZE)
		{
			keybuf.InPtr->type = type;
			keybuf.InPtr->val = data;
			keybuf.InPtr++;
			if(keybuf.InPtr == &keybuf.key[KEY_BUF_SIZE])
			{
				keybuf.InPtr = &keybuf.key[0];
			}
			keybuf.Ctr++;
		}
	}
	else
	{
		if(keybuf.Ctr<KEY_BUF_SIZE)
		{
			keybuf.InPtr->type = KEYTYPE_PRESSED;
			keybuf.InPtr->val = data;
			keybuf.InPtr++;
			if(keybuf.InPtr == &keybuf.key[KEY_BUF_SIZE])
			{
				keybuf.InPtr = &keybuf.key[0];
			}
			keybuf.Ctr++;
		}
	}
}
void keyscaninit(KEYSCAN_MODE mode)
{
	*R_PAF1 = (*R_PAF1 & ~0x3ff)|PAF1_KEYO0|PAF1_KEYI0|PAF1_KEYO1|PAF1_KEYI1|PAF1_KEYO2 ;
	*R_KSCNT = 0xffff;//default
#ifdef KEY_SUSTAIN_SUPPORT
	if(mode == KEYMODE_BOTH)
	{
		INTERRUPT_TYPE intnum = SUSTAIN_TIMER*4+1;
		setinterrupt(intnum,suschecktimerisr);
	}
#endif
	KeyScanBufClear();
	setinterrupt(INTNUM_TIMER7_KEYSCAN,KeyScan_ISR);
	EnableInterrupt(INTNUM_TIMER7_KEYSCAN,TRUE);
	*R_KSCTRL = mode|KEYCON_ENABLE;

}
BOOL getkeyscan(KEY* key)
{
#ifdef KEY_SUSTAIN_SUPPORT
	if(*R_KSCTRL & KEYCON_BOTH_MODE)
	{
		CRITICAL_ENTER();
		if(bsustain)
		{
			if(oldkeydata)
			{
				key->type = KEYTYPE_SUSTAIN;
				key->val = oldkeydata;
				CRITICAL_EXIT();
				return TRUE;
			}
			bsustain = FALSE;
		}
	}
#endif
	CRITICAL_ENTER();
	if(keybuf.Ctr>0)
	{
		key->type = keybuf.OutPtr->type;
		key->val = keybuf.OutPtr->val;
		keybuf.OutPtr++;
		if(keybuf.OutPtr == &keybuf.key[KEY_BUF_SIZE])
		{
			keybuf.OutPtr = &keybuf.key[0];
		}
		keybuf.Ctr--;
		CRITICAL_EXIT();
		return TRUE;
	}
	CRITICAL_EXIT();
	return FALSE;
}
