#include "cantus.h"
#include "tripinit.h"
#include "trip.h"
#include "timer_tick.h"
#include "wizchip_conf.h"
#include "dhcp.h"
#include "loopback.h"

#define SOCK_TCPS       0
#define SOCK_UDPS       1
#define SOCK_DHCP       6
#define PORT_TCPS		5000
#define PORT_UDPS       3000

#define MY_MAX_DHCP_RETRY			2
uint8_t my_dhcp_retry = 0;

wiz_NetInfo gWIZNETINFO =
{
	.mac = {0x00, 0x08, 0xdc, 0x12, 0x34, 0x56},
	.ip = {192, 168, 0, 1},
	.sn = {255, 255, 255, 0},
	.gw = {192, 168, 0, 1},
	.dns = {168, 126, 63, 1},
	.dhcp = NETINFO_DHCP
};

uint8_t gDATABUF[DATA_BUF_SIZE];

static void  wizchip_select(void)
{
	SPI_CS_LOW();
}

static void  wizchip_deselect(void)
{
	SPI_CS_HIGH();
}

static U8 wizchip_read()
{
	U8 rb;
	spi_master_xfer(NULL, 0, &rb, 1, 0);
	return rb;
}

static void  wizchip_write(U8 wb)
{
	spi_master_xfer(&wb, 1, NULL, 0, 0);
}

static void wizchip_readburst(U8* pBuf, U16 len)
{
	spi_master_xfer(NULL, 0, pBuf, len, 0);
}

static void  wizchip_writeburst(U8* pBuf, U16 len)
{
	spi_master_xfer(pBuf, len, NULL, 0, 0);
}

static void Net_Conf()
{
	/* wizchip netconf */
	ctlnetwork(CN_SET_NETINFO, (void*) &gWIZNETINFO);
}

void Display_Net_Conf()
{
	uint8_t tmpstr[6] = {0,};
	wiz_NetInfo gWIZNETINFO;

	ctlnetwork(CN_GET_NETINFO, (void*) &gWIZNETINFO);
	ctlwizchip(CW_GET_ID,(void*)tmpstr);

	// Display Network Information
	if(gWIZNETINFO.dhcp == NETINFO_DHCP) printf("\r\n===== %s NET CONF : DHCP =====\r\n",(char*)tmpstr);
		else debugprintf("\r\n===== %s NET CONF : Static =====\r\n",(char*)tmpstr);

	debugprintf("\r\nMAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n", gWIZNETINFO.mac[0], gWIZNETINFO.mac[1], gWIZNETINFO.mac[2], gWIZNETINFO.mac[3], gWIZNETINFO.mac[4], gWIZNETINFO.mac[5]);
	debugprintf("IP: %d.%d.%d.%d\r\n", gWIZNETINFO.ip[0], gWIZNETINFO.ip[1], gWIZNETINFO.ip[2], gWIZNETINFO.ip[3]);
	debugprintf("GW: %d.%d.%d.%d\r\n", gWIZNETINFO.gw[0], gWIZNETINFO.gw[1], gWIZNETINFO.gw[2], gWIZNETINFO.gw[3]);
	debugprintf("SN: %d.%d.%d.%d\r\n", gWIZNETINFO.sn[0], gWIZNETINFO.sn[1], gWIZNETINFO.sn[2], gWIZNETINFO.sn[3]);
	debugprintf("DNS: %d.%d.%d.%d\r\n", gWIZNETINFO.dns[0], gWIZNETINFO.dns[1], gWIZNETINFO.dns[2], gWIZNETINFO.dns[3]);
}

void my_ip_assign(void)
{
   getIPfromDHCP(gWIZNETINFO.ip);
   getGWfromDHCP(gWIZNETINFO.gw);
   getSNfromDHCP(gWIZNETINFO.sn);
   getDNSfromDHCP(gWIZNETINFO.dns);
   gWIZNETINFO.dhcp = NETINFO_DHCP;
   /* Network initialization */
   Net_Conf();      // apply from DHCP
   
   Display_Net_Conf();
   debugprintf("DHCP LEASED TIME : %ld Sec.\r\n", getDHCPLeasetime());
   debugprintf("\r\n");
}

/************************************
 * @ brief Call back for ip Conflict
 ************************************/
void my_ip_conflict(void)
{
	debugprintf("CONFLICT IP from DHCP\r\n");
   //halt or reset or any...
   while(1); // this example is halt.
}

int main()
{
	dcache_invalidate_way();
	TRIPinit();
	InitInterrupt();
	
	UartConfig(TRIP_UART_DEBUG, 115200, DATABITS_8, STOPBITS_1, UART_PARNONE);
	debugstring("\r\n================================================\r\n");
	debugprintf(" W5500 TEST. System Clock(%dMhz)\r\n", GetAHBclock()/1000000);
	debugstring("================================================\r\n");
	
	timer_init(1);
	
	*R_P2oLOW = TRIP_F_W5500_RST;
	delayms(500);
	*R_P2oHIGH = TRIP_F_W5500_RST;
	delayms(500);
	
	spi_master_init();
    int baud = spi_set_freq(SPI_MASTER, SPI_MASTER_MAXFREQ);
	debugprintf("SPI SCK : %d KHz\n\r", SPI_SCK(baud)/1000 );
	
	reg_wizchip_cs_cbfunc(wizchip_select, wizchip_deselect);
	reg_wizchip_spi_cbfunc(wizchip_read, wizchip_write);
	reg_wizchip_spiburst_cbfunc(wizchip_readburst, wizchip_writeburst);

	U8 memsize[2][8] = { { 2, 2, 2, 2, 2, 2, 2, 2 }, { 2, 2, 2, 2, 2, 2, 2, 2 } };
	
	/* W5500 Chip Initialization */
	if (ctlwizchip(CW_INIT_WIZCHIP, (void*) memsize) == -1) {
		debugprintf("WIZCHIP Initialized fail.\r\n");
		while (1);
	}
	
	Net_Conf();
	
	uint8_t tmpstr[6] = {0,};
	ctlwizchip(CW_GET_ID,(void*)tmpstr);
	debugprintf("Device = %s\r\n", tmpstr);
	
	bool run_user_applications = false;
	
	if(gWIZNETINFO.dhcp == NETINFO_DHCP)
	{
		setinterrupt(INTNUM_TIMER1, DHCP_time_handler);
		settimer(1, 1000);
		
		DHCP_init(SOCK_DHCP, gDATABUF);
		reg_dhcp_cbfunc(my_ip_assign, my_ip_assign, my_ip_conflict);
		
		run_user_applications = false; 	// flag for running user's code
	}
	else
	{
		// Static
		Display_Net_Conf();
		run_user_applications = true; 	// flag for running user's code
	}
	
	while(1)
	{
		if(gWIZNETINFO.dhcp == NETINFO_DHCP)
    	{
			switch(DHCP_run())
			{
				case DHCP_IP_ASSIGN:
				case DHCP_IP_CHANGED:
					/* If this block empty, act with default_ip_assign & default_ip_update */
					//
					// This example calls my_ip_assign in the two case.
					//
					// Add to ...
					//
					break;
				case DHCP_IP_LEASED:
					//
					// TODO: insert user's code here
					run_user_applications = true;
					//
					break;
				case DHCP_FAILED:
					/* ===== Example pseudo code =====  */
					// The below code can be replaced your code or omitted.
					// if omitted, retry to process DHCP
					my_dhcp_retry++;
					if(my_dhcp_retry > MY_MAX_DHCP_RETRY)
					{
						gWIZNETINFO.dhcp = NETINFO_STATIC;
						DHCP_stop();      // if restart, recall DHCP_init()
							
						printf(">> DHCP %d Failed\r\n", my_dhcp_retry);
						Net_Conf();
						Display_Net_Conf();   // print out static netinfo to serial
						my_dhcp_retry = 0;
					}
					break;
				default:
					break;
			}
    	}
		
		if(run_user_applications)
		{
			loopback_tcps(SOCK_TCPS, gDATABUF, PORT_TCPS);
		}
		
		standby(2);
	}
	
	return 0;
}


