#include "cantus.h"
#include "tripinit.h"

#define ANGLE_RESOLUTION	180	// 180���� 2���� ����.
#define MAX_AMPLITUDE	19	// ���� ���� 20���� DAC�� VPP�� 2V�̹Ƿ� 0.1V	������ ��.
#define MAX_FREQUENCY	900000	// 5KHz=900KHz/180
#define MIN_FREQUENCY	180000	// 1KHZ=180KHz/180

U16 wave_tablea[ANGLE_RESOLUTION]={
	0,   15,  30,  45,  60,  75,  90,  104, 119, 133,147, 162, 175, 189, 202, 216, 228, 241, 253, 265, 
	277, 288, 299, 310, 320, 330, 340, 349, 357, 366, 373, 381, 387, 394, 400, 405, 410, 414, 418, 422, 
	425, 427, 429, 430, 431, 431, 431, 430, 429, 427, 425, 422, 418, 414, 410, 405, 400, 394, 387, 381, 
	373, 366, 357, 349, 340, 330, 320, 310, 299, 288, 277, 265, 253, 241, 228, 216, 202, 189, 175, 162, 
	147, 133, 119, 104, 90,  75,  60,  45,  30,  15, 0,   -15, -30, -45, -60, -75, -90, -104,-119, -133, 
	-147, -162, -175, -189, -202, -216, -228, -241, -253, -265, -277, -288, -299, -310, -320, -330, -340, -349, -357, -366, 
	-373, -381, -388, -394, -400, -405, -410, -414, -418, -422, -425, -427, -429, -430, -431, -431, -431, -430, -429, -427, 
	-425, -422, -418, -414, -410, -405, -400, -394, -387, -381, -373, -366, -357, -349, -340, -330, -320, -310, -299, -288, 
	-277, -265, -253, -241, -228, -216, -202, -189, -175, -161, -147, -133, -119, -104, -90, -75, -60, -45, -30, -15 
};

unsigned short amp_resul=0;
U8 ch=0;
U16 amp_value=MAX_AMPLITUDE;// �ִ� �������� ���
U32 freq_value=MAX_FREQUENCY/2;// �ִ� ���ļ��� �������� ���
void SetFreq(U32 freq);
void TIMER0_ISR(void){  
	// if(*R_P5oLEV &(1<<7))	*R_P5oLOW = (1<<7);//PIO5.7 low , led on
	// else	*R_P5oHIGH = (1<<7);//PIO5.7 low , led on
	*R_VOICEDAC=((wave_tablea[amp_resul]*amp_value)&0x3fff);
	amp_resul++;
	if(amp_resul==ANGLE_RESOLUTION){//360���� �Ǿ����� ���� �� ���ļ� ���� �� ������.
		amp_resul=0;
		if(ch=='+'){// ���� ����
			if(amp_value<MAX_AMPLITUDE)
				amp_value++;
			ch=0;
		}
		else{
			if(ch=='-'){// ���� ����
				if(amp_value)
					amp_value--;
				ch=0;
			}else{// ���ļ� ����
				if(ch=='p'){
					if(freq_value<MAX_FREQUENCY)// 5KHz���� ������ ���ļ� ����
						freq_value+=1800;// ������ ����ڰ� �°� ����(10Hz )
					SetFreq(freq_value);
					ch=0;
					// if(*R_P5oLEV &(1<<7))	*R_P5oLOW = (1<<7);//PIO5.7 low , led on
					// else	*R_P5oHIGH = (1<<7);//PIO5.7 low , led on
				}
				else{// ���ļ� ����
					if(ch=='m'){
						if(freq_value>MIN_FREQUENCY)// 1KHz���� ũ�� ���ļ� ����
							freq_value-=1800;// ������ ����ڰ� �°� ����(10Hz)
						SetFreq(freq_value);
						ch=0;
						// if(*R_P5oLEV &(1<<7))	*R_P5oLOW = (1<<7);//PIO5.7 low , led on
						// else	*R_P5oHIGH = (1<<7);//PIO5.7 low , led on
					}
				}
			}
		}
	}
} 

void SetFreq(U32 freq){
	U32 aa;

	*R_TP0CON=2;// system clock=96MHz
	*R_TP0CON=0;// system clock=96MHz
	*R_TM0CON=0;// pre-scale=APB Clock/2=24MHz
	*R_TM0CNT=aa=GetAPBclock()/2/freq+1;
	*R_TM0CON=1;// Timer0 run
}

void SetDAC(void){

	*((volatile U32*)0x80020424)|=(1<<6);
	*((volatile U32*)0x80020418)|=(1<<2);
	*((volatile U32*)0x80020424)&=~(1<<6);
	*R_VOICEPWD= VOICEPWD_REF_ON ;
	delayms(100);
	*R_VOICEPWD |= VOICEPWD_DAC_ON;
	delayms(10);
	*R_VOICEPWD |= VOICEPWD_RESET|VOICEPWD_DAC_MUTE_DIS;
	*R_VOICECON = VOICECON_EN;
	delayms(100);
	*R_VOICEDAC=0;
}

int main(void){
	TRIPinit();
	InitInterrupt();
	UartConfig(7,115200,DATABITS_8,STOPBITS_1,UART_PARNONE);
	debugstring("================================================\r\n");
	debugprintf(" DAC Sine Wave Output: 4~5KHz, resolution: 180 System Clock(%dMhz)\r\n",GetAHBclock()/1000000);
	debugstring("================================================\r\n");
	SetDAC();
	setinterrupt(INTNUM_TIMER0,TIMER0_ISR);
	SetFreq(MAX_FREQUENCY/2);// �⺻ ������ �ִ� ���ļ��� �������� ���
	EnableInterrupt(INTNUM_TIMER0,TRUE);
//PRINTLINE;
	while(1){
		if(UartGetCh(7,&ch))
			UartPutCh(7,ch);
	}
	return 0;
}
