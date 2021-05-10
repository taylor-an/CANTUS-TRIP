#include "cantus.h"
#include "cantus/voice_codec.h"

void set_voice_codec(U32 mode)
{
	AMP_ON();
	//Voice Codec setting
	*R_VOICEPWD= VOICEPWD_REF_ON ;
	delayms(100);
	
	*R_VOICEPWD |= mode;
	delayms(10);
	*R_VOICEPWD |= VOICEPWD_RESET|VOICEPWD_DAC_MUTE_DIS;
	*R_VOICECON = VOICECON_I2S | VOICECON_EN;
	delayms(100);
}

void ADC_channel(U32 nch)
{
	*R_VOICECON=0;
	if(nch == 0)
	{
		*R_VOICEPWD = VOICEPWD_REF_ON | VOICEPWD_VGA;
	}
	else if(nch == 1)
	{
		*R_VOICEPWD = VOICEPWD_REF_ON | VOICEPWD_AIN1;
	}
	else if(nch == 2)
	{
		*R_VOICEPWD = VOICEPWD_REF_ON | VOICEPWD_AIN2;
	}
	else if(nch == 3)
	{
		*R_VOICEPWD = VOICEPWD_REF_ON | VOICEPWD_AIN3;
	}
	delayms(1);
	
	*R_VOICEPWD |= (VOICEPWD_ADC_ON);
	delayms(1);
	
	*R_VOICEPWD |= VOICEPWD_RESET;
	*R_VOICECON = VOICECON_EN;	
	delayms(1);	
}
