#pragma once

#define R_VOICECON				((volatile unsigned int*)0x80022400)
#define R_VOICEPWD				((volatile unsigned int*)0x80022404)
#define R_VOICEDAC				((volatile unsigned int*)0x80022408)
#define R_VOICEADC				((volatile unsigned int*)0x8002240c)

#define VOICECON_I2S			(1<<2)
#define VOICECON_IEN			(1<<1)
#define VOICECON_EN				(1<<0)

#define VOICEPWD_DAC_MUTE_DIS	(1<<4)
#define VOICEPWD_REF_ON			(1<<3)
#define VOICEPWD_DAC_ON			(1<<2)
#define VOICEPWD_ADC_ON			(1<<1)
#define VOICEPWD_RESET			(1<<0)

#define VOICEPWD_AIN3   (3 << 5)
#define VOICEPWD_AIN2   (2 << 5)
#define VOICEPWD_AIN1   (1 << 5)
#define VOICEPWD_VGA   (0 << 5)

#define AMP_OFF()	 			*R_P6oHIGH = (1<<4) //High : shutdown
#define AMP_ON()	 			*R_P6oLOW = (1<<4)

void set_voice_codec(U32 mode);
void ADC_channel(U32 nch);
