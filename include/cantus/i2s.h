
#pragma once

#define R_I2SCON	((volatile unsigned int*)0x80022800)
#define R_I2SMOD	((volatile unsigned int*)0x80022804)
#define R_I2SPRE	((volatile unsigned int*)0x80022808)
#define R_I2SSTAT	((volatile unsigned int*)0x8002280c)
#define R_I2SDATA	((volatile unsigned int*)0x80022810)
#define R_I2SVOL	((volatile unsigned int*)0x80022814)

#define I2SCON_RECEIVE_ADCEN    (1 << 8)
#define I2SCON_TRANSMIT_DMA     (1 << 7)
#define I2SCON_RECEIVE_DMA      (1 << 6)
#define I2SCON_TRANSMIT_INT      (1 << 5)
#define I2SCON_RECEIVE_INT       (1 << 4)
#define I2SCON_TRANSMIT_PAUSEEN (1 << 3)
#define I2SCON_RECEIVE_PAUSEEN  (1 << 2)
#define I2SCON_MCLK_EN          (1 << 1)
#define I2SCON_I2S_START        (1 << 0)

#define I2SMOD_BS32EN           (1 << 31)
#define I2SMOD_BS16EN           (1 << 30)
#define I2SMOD_LEFTLOW          (1 << 15)
#define I2SMOD_FIFO_SHARE       (1 << 14)
#define I2SMOD_TX_VOLUME_EN     (1 << 13)
#define I2SMOD_RX_VOLUME_EN     (1 << 12)
#define I2SMOD_PCMUS8_STEREO    (0 << 8)
#define I2SMOD_PCMUS8_MONO      (1 << 8)
#define I2SMOD_PCMS8_STEREO     (2 << 8)
#define I2SMOD_PCMS8_MONO       (3 << 8)
#define I2SMOD_PCMUS16_STEREO   (4 << 8)
#define I2SMOD_PCMUS16_MONO     (5 << 8)
#define I2SMOD_PCMS16_STEREO    (6 << 8)
#define I2SMOD_PCMS16_MONO      (7 << 8)
#define I2SMOD_ADPCM_STEREO     (8 << 8)
#define I2SMOD_ADPCM_MONO       (9 << 8)
#define I2SMOD_MODE_NO_TRANS    (0 << 6)
#define I2SMOD_MODE_RECEIVE     (1 << 6)
#define I2SMOD_MODE_TRANSMIT    (2 << 6)
#define I2SMOD_MODE_BOTH        (3 << 6)
#define I2SMOD_SLAVE            (1 << 5)
#define I2SMOD_MSB_FMT          (1 << 4)
#define I2SMOD_MCLK_256         (0 << 2)
#define I2SMOD_MCLK_384         (1 << 2)
#define I2SMOD_MCLK_512         (2 << 2)
#define I2SMOD_MCLK_NO          (3 << 2)
#define I2SMOD_SLK_16           (0)
#define I2SMOD_SLK_32           (1)
#define I2SMOD_SLK_48           (2)
#define I2SMOD_SLK_64           (3)

#define I2SPS_MSTR_EXTCLK         (1 << 4)

#define I2SSTAT_INDEX_LEFT      (1 << 20)
#define I2SSTAT_TRANSMIT_HALF   (1 << 19)
#define I2SSTAT_RECEIVE_HALF    (1 << 18)
#define I2SSTAT_TRANSMIT_READY  (1 << 17)
#define I2SSTAT_RECEIVE_READY   (1 << 16)
#define I2S_FIFO_MAX	128//if not shared mode 128bye
/*
typedef enum{
	PCM_8US=0,//8bit unsigned stereo
	PCM_8UM,
	PCM_8SS, // signed
	PCM_8SM,
	PCM_16US,
	PCM_16UM,
	PCM_16SS,
	PCM_16SM,
	ADPCM_4S,
	ADPCM_4M,
}PCM_TYPE;
*/
typedef enum{
	FREQ_11025=1,
	FREQ_22050,
	FREQ_44100,	
}I2SFreq;

typedef enum 
{
	I2S_ERR_NO = 0,
	I2S_ERR_BUF_FULL,
	I2S_ERR_DATE_LENGTH_MISALIGN,
}I2S_ERROR;

BOOL I2S_TxInit(U32 mode, I2SFreq freq);
I2S_ERROR I2S_TxData(U32* buf, U32 len); //len is 4byte count
void I2S_Stop();
void I2S_Pause(BOOL b);
BOOL I2S_Running();
void I2S_Set_TxISRHook(int (*readfunc)(U32*,int readbytesize)); //only for GDMA Transfer mode
void I2S_Set_TxEndHook(int (*func)());//only for GDMA Transfer mode, func called when tx data is not available anymore
