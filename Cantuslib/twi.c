#include "cantus.h"

int twi_set_freq(U32 freq)
{
    unsigned long long tmp;
    U32 twibr0_cnt, twibr1_cnt;

    tmp = (unsigned long long)GetAPBclock()*700/1000000000;    
    twibr0_cnt = tmp + 3;
    twibr1_cnt = (GetAPBclock()/(2*freq)) - ((twibr0_cnt+7) >> 1);
	
	*R_TWIBR0 = twibr0_cnt;
	*R_TWIBR1 = twibr1_cnt;
	
    return 0;
}

void twi_trans_complete(void)
{
    volatile U16 status;
	
    do
	{
		status = *R_TWISTR;
    }while((status & 0x80) != 0x80);
}

void twi_wait_busy(void)    /* TWI Status is Busy */
{
	volatile U16 status;
	
    do
	{
		status = *R_TWISTR;
    }while(status & TWISTR_TWIBUSY);
}

/*
 * devaddr : Base of DEVICE ADDRESS
 * waddr : Buffer of Word Address
 * len : length of 'waddr', 1 or 2
 * flags : TWI_TRANSMIT, TWI_RECEIVE
 */
void twi_start (U8 devaddr, U8 *waddr, int len, int flags)
{
    int i;
    U8 con = 0;

    con = TWICONR_TWIEN | TWICONR_MASTER | TWICONR_TWITX;

	*R_TWIDR = devaddr;
	*R_TWICONR = con;
    twi_trans_complete();

    for(i = 0; i < len; i++)
	{
		*R_TWIDR = waddr[i];
        twi_trans_complete(); 
    }

    if(flags == TWI_RECEIVE)
	{
        con = TWICONR_TWIEN | TWICONR_MASTER | TWICONR_REPST;
		*R_TWICONR = con;
		*R_TWIDR = devaddr+1;
        twi_trans_complete();
    }
}

/*
 * devaddr : Base of DEVICE ADDRESS
 * waddr : Buffer of Word Address
 * len : length of 'waddr', 1 or 2
 * buffer : Writing buffer
 * length : Size of Writing
 */
int twi_write(U8 devaddr, U8 *waddr, int len, U8 *buffer, int length)
{
    int write_num, i;

    if(length <= 0)
        return -1;

    write_num = 0;

    twi_start(devaddr, waddr, len, TWI_TRANSMIT);

    for(i = 0; i < length; i++)
	{
		*R_TWIDR = buffer[i];	// When word is 8bit, use this
        twi_trans_complete();
        write_num++;
    }
	
	*R_TWICONR = TWICONR_TWIEN;
    twi_wait_busy();
	
    return write_num;
}

/*
* devaddr : Base of DEVICE ADDRESS
* waddr : Byte Address
* buffer : Writing buffer
* length : Size of Writing
*/
int twi_write2(U8 devaddr, U8 waddr, U8 *buffer, int length)
{
	int write_num, i;

	if(length <= 0)
		return -1;

	write_num = 0;

	twi_start(devaddr, &waddr, 1, TWI_TRANSMIT);

	for(i = 0; i < length; i++)
	{
		*R_TWIDR = buffer[i];
		twi_trans_complete();
		write_num++;
	}

	*R_TWICONR = TWICONR_TWIEN;
	twi_wait_busy();

	return write_num;
}

/*
 * devaddr : Base of DEVICE ADDRESS
 * addr : Word Address
 * len : length of Word Address (1 or 2)
 * buffer : read buffer
 * length : length of buffer
 */
int twi_read(U8 devaddr, U8 *waddr, int len, U8 *buffer, int length)
{
    U32 i;
	U8	tmp;
    int read_cnt = 0;
    U16 status;

    twi_start(devaddr, waddr, len, TWI_RECEIVE);

	status = *R_TWIDR;     // Read for SCL Generation
	twi_trans_complete();
	
    for(i = 0; i < length-1; i++)
	{
		buffer[i] = *R_TWIDR;
        twi_trans_complete();
        read_cnt++;
    }

	*R_TWICONR = TWICONR_TWIEN | TWICONR_MASTER | TWICONR_NOACK;
    buffer[i] = *R_TWIDR;
    twi_trans_complete();
    read_cnt++;
	
    tmp = *R_TWIDR;    // dummy read

	*R_TWICONR = TWICONR_TWIEN;
    twi_wait_busy();
    
	return read_cnt;
}

/*
 * devaddr : Base of DEVICE ADDRESS
 * addr : Word Address
 * len : length of Word Address (1 or 2)
 * buffer : read buffer
 * length : length of buffer
 */
int twi_read2(U8 devaddr, U8 *waddr, int len, U8 *buffer, int length)
{
    U32 i;
    int read_cnt = 0;
    U16 status;

    twi_start(devaddr, waddr, len, TWI_RECEIVE);

    if(length == 1)
	{
		*R_TWICONR |= TWICONR_NOACK;
		status  = *R_TWIDR;     // Dummy Read for SCL Generation
		twi_trans_complete();
		
		*R_TWICONR = TWICONR_TWIEN;
		twi_wait_busy();
		
		*R_TWICONR = 0;
		
		buffer[0] = *R_TWIDR;
		read_cnt++;
	}
	else
	{
		status  = *R_TWIDR;     // Dummy Read for SCL Generation
		twi_trans_complete();	
		
		for(i=0; i<length-2; i++)
		{
			buffer[i] = *R_TWIDR;
			twi_trans_complete();
			read_cnt++;
		}
		
		*R_TWICONR |= TWICONR_NOACK;
		buffer[i++] = *R_TWIDR;
		read_cnt++;
		twi_trans_complete();
		
		*R_TWICONR = TWICONR_TWIEN;
		twi_wait_busy();
		
		*R_TWICONR = 0;
		buffer[i++] = *R_TWIDR;
		read_cnt++;
	}
    
	return read_cnt;
}
