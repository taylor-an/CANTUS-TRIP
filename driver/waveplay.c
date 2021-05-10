#include "cantus.h"

//#define OUTPUT_POLL_MODE

/*******************************************/
/* Define values for Microsoft WAVE format */
/*******************************************/
#define RIFF		0x46464952		/* "RIFF" */
#define WAVE		0x45564157		/* "WAVE" */
#define FACT		0x74636166		/* "fact" */
#define LIST		0x5453494c		/* "LIST" */
#define FMT		0x20746D66		/* "fmt " */
#define DATA		0x61746164		/* "data" */
#define PCM_CODE	0x0001
#define IMA_ADPCM_CODE	0x0011
#define MP3_CODE	0x0055
#define WAVE_MONO	1
#define WAVE_STEREO	2

typedef struct {
	U32 freq;  ///< Audio frequency in samples per second
	U8* samples; ///< 
	U32 size;///< Audio buffer size in samples, should be 4*8byte align.
	S8 bits; ///< ADPCM,PCM8,PCM16 
	S8  channels;///< Number of channels: 1 mono, 2 stereo
	U32  datasize;// full data size
}WAVEFORMT;

struct wavhdr_t {
	//RIFF header
	U32 ID; // 0x46464952 "RIFF"
	U32 chunksize;
	U32 format;//0x45564157 "WAVE"

	U32 fmtID; // 0x20746d66 "fmt "
	U32 subchunk1size; 
	
	U16 wFormatTag; // PCM = 1 (i.e. Linear quantization)	Values other than 1 indicate some form of compression.
	U16 nChannels; // mono = 1, stereo = 2
	U32 nSamplesPerSec; // frequency
	U32 nAvgBytesPerSec;
	U16 nBlockAlign;
	U16 wBitsPerSample;
};
	
static FIL wavfsrc;
static WAVEFORMT wv;
#ifndef OUTPUT_POLL_MODE
static int totalreaddata=0;
static int i2s_readcallback(U32* buf, int readbytesize)
{
	int br;
	if(wv.datasize - totalreaddata < readbytesize)
	{
		readbytesize =wv.datasize - totalreaddata;
	}
	f_read(&wavfsrc,buf,readbytesize,&br);
	if((br != 0) && (br < readbytesize))
	{
		debugstring("End of File\r\n");
		f_close(&wavfsrc);
	}
	totalreaddata += br;
	return br;
}
static int i2s_endcallback(U32* buf, int readbytesize)
{
	debugstring("i2s_endcallback\r\n");
	return 0;
}
#endif
		
BOOL wavefile_play(char* fname)	
{

	struct wavhdr_t wavhdr;	
	FRESULT res;

	//data chunk
	U32 dataID; // 0x61746164
	U32 datasize;
	U32 br;

	res = f_open(&wavfsrc,fname,FA_OPEN_EXISTING|FA_READ);
	if(res != FR_OK)
	{
		debugstring("file not found");
		return FALSE;
	}
	debugstring(fname);
	debugstring(" playing...\r\n");
	f_read(&wavfsrc,&wavhdr,sizeof(struct wavhdr_t),&br);
	if(wavhdr.ID != RIFF
		|| wavhdr.format != WAVE
		|| wavhdr.fmtID != FMT
		|| (wavhdr.wFormatTag != PCM_CODE && wavhdr.wFormatTag !=IMA_ADPCM_CODE) //IMA
		|| (wavhdr.nChannels!=1 && wavhdr.nChannels!=2) )
	{
		debugstring("Unsupport Wave Format\r\n");
		debugprintf("wavhdr.format(0x%x),wavhdr.fmtID(0x%x),wavhdr.subchunk1size(0x%x),wavhdr.wFormatTag(0x%x),wavhdr.nChannels(0x%x) \r\n",
			wavhdr.format,wavhdr.fmtID,wavhdr.subchunk1size,wavhdr.wFormatTag,wavhdr.nChannels);
		return FALSE;
	}
	f_lseek(&wavfsrc,sizeof(struct wavhdr_t) + (wavhdr.subchunk1size - 0x10));
	int i;
	//search data chunk
	for(i=0;i<wavfsrc.fsize- sizeof(struct wavhdr_t);)
	{
		f_read(&wavfsrc,(void*)&dataID,4,&br);
		if(dataID == DATA)
		{
			break;
		}
		i+=4;
	}
	if(i>=wavfsrc.fsize- sizeof(struct wavhdr_t))
	{
		debugstring("Unsupport Wave Format(can not find Data Header) \r\n");
		return FALSE;
	}
	f_read(&wavfsrc,&datasize,4,&br);	
	wv.datasize = datasize;
	U32 mode;

	if(wavhdr.wBitsPerSample==4)
	{
		mode = (wavhdr.nBlockAlign <<16)|(1<<11);
		
	}
	else if(wavhdr.wBitsPerSample==8)
	{
		mode = 0;
	}
	else if(wavhdr.wBitsPerSample==16)
	{
		mode = (3<<9);
	}
	else
	{
		debugprintf("not supported %dbps\r\n",wavhdr.wBitsPerSample);
		return FALSE;
	}

	if(wavhdr.nChannels==1)
	{
		mode |=(1<<8);//mono
	}
	I2SFreq freq;
	if(wavhdr.nSamplesPerSec == 44100)
		freq = FREQ_44100;
	else if(wavhdr.nSamplesPerSec == 22050)
		freq = FREQ_22050;
	else if(wavhdr.nSamplesPerSec == 11025)
		freq = FREQ_11025;
	else
	{
		debugprintf("not supported %d frequency",wavhdr.nSamplesPerSec);
		return FALSE;
	}
	
	I2S_TxInit(mode,freq);

	U32 buf[1152/4];
#ifdef OUTPUT_POLL_MODE
	int readlen;
	for(i=0;i<datasize;)
	{
		readlen=datasize-i;
		if(readlen>1152)
		{
			readlen=1152;
		}
		f_read(&wavfsrc,buf,readlen,&br);
		i+=br;
		while(I2S_TxData(buf,br/4)!=I2S_ERR_NO);
	}
#else	// callback mode: at least, fill two buffer first
	totalreaddata=0;
	I2S_Set_TxISRHook(i2s_readcallback);
	I2S_Set_TxEndHook(i2s_endcallback);
	f_read(&wavfsrc,buf,1152,&br);
	totalreaddata +=br;
	while(I2S_TxData(buf,br/4)!=I2S_ERR_NO);
	f_read(&wavfsrc,buf,1152,&br);
	if(br>0)
	{
		totalreaddata +=br;
		while(I2S_TxData(buf,br/4)!=I2S_ERR_NO);
	}
#endif
	return TRUE;
}

