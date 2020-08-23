#if !defined ___PPP_API_INCLUDED
#define ___PPP_API_INCLUDED

#include <unistd.h>
#include<signal.h>
#include <sys/signal.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include <sys/ioctl.h>
#include<string.h>
#include<fcntl.h>
#include<termios.h>
#include <errno.h>
#include <V91sdlc.h>
#include <sys/types.h>
#include<sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<sys/time.h>
#include<time.h>
#include <net/if.h>
#include <dirent.h>
#include <sys/select.h>
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "curl.h"
#include "easy.h"
#include "type.h"
#include "V91ppp.h"
#include "scr_errors.h"
#include "soundapp.h"
#include "usb_api.h"
#include<../include/curlver.h>
//#include"fcntl.h"
#include<sys/types.h>
#include"V91modem.h"
/*Specific Include Files */

//#include"fcntl.h"
#include<sys/types.h>
#define SUCCESS 0
#define ERROR -

//for cdma&gprs
int  pppdial_cdma(char *dialno);
int  pppdial_gprs(char *dialno);
int  pppondial_pstn(char *dialno);
int  ppp_options(char *userid,char *password);
int  ppp_dial_update(char *dialno,char *gprsdomain);
int  pppdial_gprs(char *dialno);
int  ppp_on_dial(char *dialno);
int  ppp_options_ruim(char *userid,char *password);
int  ppp_pstn(char *dialno,char *userid,char *password,char *localip,
		char *remoteip,char *netmask);
void ppp_close();
int  ppp_open();

//for Gsm
int modem_open(void) ;
int modem_close (void) ;
int modem_get_handle();
char * modem_config (char *) ;
void modem_setspeed(int baudrate);
int  modem_dial (const char *) ;
int modem_tx_data (unsigned char * , int) ;
int modem_rx_data(unsigned char *,int) ;
int modem_disconnect (void) ;
void alarm_handler (int signum);

#define  NOTCONNECTED           -1
#define  CONNECTED               0

#endif
