#include <ppp_api.h>


//#define TEST 	1
//Gsm modem variables
sig_atomic_t timeoutflag=0;

typedef struct
{ int handle; } MODEMPORT;

static MODEMPORT modemport;                                                                                      
static struct termios modem_termios;

static char config_buf[255]= "" ;
static char dial_buf[255] = "" ;
static char wbuf[255]= "" ;
static int result;
static int status = NOTCONNECTED ;

//----------------------------------------------------------------------------------------------
int copy_file (char *dest, char *src)
{
	unsigned char c[512];
	int in, out,n;
 
	in = open(src , O_RDONLY);
	out = open(dest, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);

	if (in == -1 || out == -1)
	{
 		fprintf(stderr,"unable to open file %d %d\n", in, out);
 		return -1;
	}
		

	while(1)
 	{
		n=read(in, c, 512);
 		if (n == 0)
  			break;
  			
 		write(out, c, n);
 	}

	close(in);
	close(out);
}

/*********************************************/
/* Script used to connect ppp */

int  pppdial_cdma(char *dialno)
{
	char str[512];
	FILE *rfp;
	
	//puts("In pppdial_cdma\n");
	sprintf(str, "#!/bin/sh\nTELEPHONE=\"%s\" \nexport TELEPHONE \nrm -f /var/run/ppp.link \nexec /usr/sbin/pppd    \\\n        debug /dev/ttymxc0  \\\n        115200 0.0.0.0:0.0.0.0  \\\n        connect /etc/ppp/ppp-on-dialer", dialno);                                                             
	//fprintf(stderr, "before fopen\n");
	rfp = fopen("/etc/ppp/dialout", "w");
	//fprintf(stderr,"after fopen\n");
	if(rfp<0) return -1;
	
	fprintf(rfp,"%s",str);
	fclose(rfp);

//#define TEST
#ifdef TEST
	printf("%s", str);
	printf("\n");
#endif
	//puts("Out of pppdial_cdma\n");
	return 0;
}

int pppdial_gprs(char *dialno)  
{
	char str[512];
	FILE *rfp;

	sprintf(str, "#!/bin/sh\nTELEPHONE=\"%s\" \nexport TELEPHONE\nrm -f /var/run/ppp.link \nexec /usr/sbin/pppd    \\\n        debug /dev/ttymxc0  \\\n        9600 0.0.0.0:0.0.0.0  \\\n        connect /etc/ppp/ppp-on-dialer",dialno);
	
	rfp=fopen("/etc/ppp/dialout","w");
	if(rfp<0)return -1;
	fprintf(rfp,"%s",str);
	fclose(rfp);

#ifdef TEST
	printf("%s",str);
	printf("\n");
#endif

	return 0;
}

/* Script used to set the PPP options */

int  ppp_options(char *userid, char *password)
{
	char str[512];
	char sf2[32];
	
	FILE *rfp;
	strcpy(str, "debug\n-detach\nlock\nasyncmap 0\ncrtscts\ndefaultroute\nusepeerdns\nmodem\nmru 1064\nmtu 1064");                                                                                  

	if(userid && strlen(userid))
	{
		sprintf(sf2, "\nname %s", userid);
		strcat(str, sf2);
    }
    
	if(password && strlen(password))
	{
		sprintf(sf2, "\npassword %s", password);
		strcat(str, sf2);
    }

	rfp=fopen("/etc/ppp/options", "w");
	if(rfp<0) return -1;
	fprintf(rfp,"%s",str);
	fclose(rfp);
#ifdef TEST
	printf("%s",str);
	printf("\n");
#endif

	return 0;
}                                                                                                

//for gprs ppp-on-dialer

int ppp_dial_update(char *dialno, char *gprsdomain)
{
	char str[1024];
	FILE *rfp;
	sprintf(str, "#!/bin/sh\n/usr/sbin/chat -v      \\\n        TIMEOUT         3    \\\n        ABORT           \'\\nBUSY\\r\'      \\\n        ABORT           \'\\nNO ANSWER\\r\' \\\n        ABORT          \'\\nRINGING\\r\\n\\r\\nRINGING\\r\'    \\\n        ECHO            ON      \\\n        \'\'               AT     \\\n        \'OK-+++\\C-OK'   ATH0    \\\n        \'\'              AT+CSQ      \\\n        OK              \'AT+CGATT=1\' \\\n        OK              \'AT+CGDCONT=1,\"IP\",\"%s\"\' \\\n        TIMEOUT         45      \\\n        OK              ATDT\'%s\'  \\\n        CONNECT         \'\'", gprsdomain,dialno);

	rfp=fopen("/etc/ppp/ppp-on-dialer", "w");
	if(rfp < 0) return -1;
	fprintf(rfp, "%s", str);
	fclose(rfp);
#ifdef TEST
	printf("%s",str);
	printf("\n");
#endif
	return 0;
}

//for cdma
int ppp_on_dial(char *dialno)
{
	char str[1000];
	FILE *rfp;
	//printf("in ppp_on_dial");
	sprintf(str,"#!/bin/sh\n/usr/sbin/chat -v      \\\n        TIMEOUT         3    \\\n        ABORT           \'\\nBUSY\\r\'      \\\n        ABORT           \'\\nNO ANSWER\\r\' \\\n        ABORT         \'\\nRINGING\\r\\n\\r\\nRINGING\\r\'    \\\n        \'\'               AT     \\\n        \'OK-+++\\C-OK'    AT      \\\n        ECHO             ON    \\\n        TIMEOUT         45      \\\n        OK             ATDT\'%s\'  \\\n        CONNECT         \'\'", dialno);

	rfp=fopen("/etc/ppp/ppp-on-dialer", "w");
	if(rfp<0) return -1;
	fprintf(rfp,"%s",str);
	fclose(rfp);
	//#ifdef TEST
	//printf("%s",str);
	//printf("\n");
	//#endif
	return 0;
}


int  ppp_options_ruim(char *userid, char *password)
{
	char str[512];
	char sf2[32];
	
	FILE *rfp;                                                                                       

	strcpy(str, "-detach\nlock\nasyncmap 0\ncrtscts\ndefaultroute\nusepeerdns\nlogin\nrefuse-chap\nrefuse-eap\nrefuse-mschap\nrefuse-mschap-v2\nmodem\nmru 552\nmtu 552");                                                                                             
	
	if(userid && strlen(userid))
	{
		sprintf(sf2, "\nname %s", userid);
		strcat(str, sf2);
    }
    
	if(password && strlen(password))
	{
		sprintf(sf2, "\npassword %s", password);
		strcat(str, sf2);
    }
	
	rfp=fopen("/etc/ppp/options","w");
	if(rfp < 0) return -1;
	
	fprintf(rfp,"%s",str);
	fclose(rfp);
	                                                                                     
#if 0
	printf("%s",str);
	printf("\n");
#endif

	return 0;
}
//to terminate the ppp connection
void ppp_close()
{
/*	char p_id[8];
	char str[50];
	int fd9;
	int ret;

	fd9 = open("/var/run/ppp0.pid",O_RDONLY);
	if (fd9)
	{
		ret=read(fd9, p_id,6);
		p_id[ret] = '\0';
		sprintf(str, "kill -1 %s", p_id);
		system(str);
		close(fd9);
	} */
	
	system("killall -HUP pppd");
	
#if 0
//	system("insmod /home/imx_dtrrts.ko > /dev/null 2>&1 ");
//	sleep(2);
//	system("rmmod imx_dtrrts.ko > /dev/null 2>&1");
#endif	
}

int ppp_open()
{
	int fd9, i;
	printf("PPP starting\n");
	system("rm -f /etc/ppp/resolv.conf");
	system("/etc/ppp/dialout&");
	
	sleep(6);
	i = 0;

	while (i++ < 15)
	{
		fd9 = open("/etc/ppp/resolv.conf", O_RDONLY);
		printf("%d\n", fd9);
		if (fd9 > 0)
		{
			close(fd9);
			break;
		}
		sleep(1);
	}
	
	if(i >= 15)
	{
		ppp_close();
		return -1;
	}
	
	printf("PPP Connected \n");
	copy_file ("/etc/resolv.conf", "/etc/ppp/resolv.conf");
	sleep(2);
	return 0;
}

int ppp_info_open()
{
	int fd9,i;
	FILE *fp;
	char str[80] = "";

	printf("PPP starting\n");
	system("rm -f /var/run/ppp.link");
	system("/etc/ppp/dialout > ppp_info &");

	sleep(6);
	i=0;

	while (i < 35)
	{
		fd9 = open("/var/run/ppp.link", O_RDONLY);
		printf("%d\n", fd9);
		if (fd9 > 0)
		{
			close(fd9);
			break;
		}
		sleep(1);
		i++;
	}
	if(i>=35)
	{
		fp=fopen("ppp_info", "r");   //checking  init scripts for ramdisk configuration file
		if(fp==NULL)
		{
			printf("\nfile open error\n");
			return -1;
		}


		else
			while((fgets(str,80,fp))!=NULL)
			{
				if((strstr(str,"Modem hangup")) != NULL)
				{
					printf("sarath\n");
					fclose(fp);
					ppp_close();
					return -2;
				}
			}

			fclose(fp);
			ppp_close();
			return -1;
	}

	printf("PPP Connected \n");
	copy_file ("/etc/resolv.conf","/etc/ppp/resolv.conf");
	sleep(2);
	return 0;
}

/*


int ppp_pstn(char *dialno,char *userid,char *password,char *localip,char *remoteip,char *netmask)
{
	char str[1000];
	FILE *rfp;
	sprintf(str,"#!/bin/sh\nTELEPHONE=%s \nACCOUNT=%s \nPASSWORD=%s \nLOCAL_IP=%s \nREMOTE_IP=%s \nNETMASK=%s \nexport TELEPHONE ACCOUNT PASSWORD \nDIALER_SCRIPT=/etc/ppp/ppp-on-dialer \nexec /usr/sbin/pppd  debug /dev/ttymxc0 4800  \\\n        $LOCAL_IP:$REMOTE_IP \\\n        connect $DIALER_SCRIPT ",dialno,userid,password,localip,remoteip,netmask);

	rfp=fopen("/etc/ppp/dialout","w");
	if(rfp<0)return -1;
	fprintf(rfp,"%s",str);
	fclose(rfp);
#ifdef TEST
	printf("%s",str);
	printf("\n");
#endif

	return 0;
}

int pppondial_pstn(char *dialno)
{
	char str[1000];
	FILE *rfp;
	sprintf(str,"#!/bin/sh\n/usr/sbin/chat -v      \\\n        TIMEOUT         3    \\\n        ABORT           \'\\nBUSY\\r\'      \\\n        ABORT           \'\\nNO ANSWER\\r\' \\\n        ABORT          \'\\nRINGING\\r\\n\\r\\nRINGING\\r\'    \\\n        \'\'              AT       \\\n        ECHO            ON     \\\n        TIMEOUT         45      \\\n        OK             ATDT\'%s\'  \\\n        TIMEOUT         75     \\\n        CONNECT         \'\'",dialno);

	rfp=fopen("/etc/ppp/ppp-on-dialer","w");
	if(rfp<0) return -1;
	fprintf(rfp,"%s",str);
	fclose(rfp);
#ifdef TEST
	printf("%s",str);
	printf("\n");
#endif
	return 0;
}
*/

/*******************Gsm modem functions ***************************/

int modem_open()
{
	int modem_handle,flag =0,i;
	char buff[255] = "" ;

	modem_handle=open("/dev/ttymxc0",O_RDWR|O_NOCTTY|O_NONBLOCK);
	if(modem_handle<0)
		return -1;

	memset( &modem_termios, 0, sizeof( modem_termios ) );

#ifdef __DIALUP__                                                                                                 
	cfsetispeed( &modem_termios, B2400 );
	cfsetospeed( &modem_termios, B2400 );
#else                                                                                            
	cfsetispeed( &modem_termios, B9600 );//for GSM
	cfsetospeed( &modem_termios, B9600 );
#endif                                                                                                 
	modem_termios.c_cflag |= CS8;
	modem_termios.c_cflag |= CLOCAL;
	modem_termios.c_cflag |= CREAD;
	if ( tcsetattr(modem_handle, TCSANOW, &modem_termios ) < 0 )
	{
		close(modem_handle );
		return -1;
	}
	if ( tcflush( modem_handle, TCIOFLUSH ) < 0 )
	{
		close( modem_handle);
		return -1;
	}                                                                                                                                                                                           
	modemport.handle = modem_handle;

	printf ("Modemhandler = %d\n", modem_handle) ;
	write (modem_handle,"ATH0\r\n",6) ;
	sleep(1) ;
	read (modem_handle,buff,10) ;
	for (i=0;i<10;i++)
	{
		if (buff[i] == 'K' )
			return 0 ;
	}
	return -1 ;
}//modem open

int modem_close()
{
	int  fd;
	fd = modemport.handle;
	if (!fd)
	{
		close(fd);
		return 0 ;
	}
	else
		return -1 ;
}//modem close

void modem_setspeed(int baudrate) /* setup serial port */
{
  	struct termios newtio;
  	bzero(&newtio, sizeof(newtio));
  	newtio.c_cflag = CS8 | CREAD;
  
  	newtio.c_iflag = IGNPAR;
  	newtio.c_oflag = 0;
  	newtio.c_lflag = 0;
  	newtio.c_cc[VTIME]    = 0;
  	newtio.c_cc[VMIN]     = 0;

  	switch (baudrate)
  	{
   		 case 300:    baudrate = B300; break;
   		 case 1200:   baudrate = B1200; break;
   		 case 2400:   baudrate = B2400; break;
   		 case 9600:   baudrate = B9600; break;
   		 case 19200:  baudrate = B19200; break;
   		 case 38400:  baudrate = B38400; break;
#ifdef B57600
   		 case 57600:  baudrate = B57600; break;
#endif
#ifdef B115200
   		 case 115200: baudrate = B115200; break;
#endif
#ifdef B230400
   		 case 230400: baudrate = B230400; break;
#endif
   		 default: baudrate = B9600;
  	}

 	cfsetispeed(&newtio, baudrate);
  	cfsetospeed(&newtio, baudrate);
  	tcsetattr(modemport.handle, TCSANOW, &newtio);
}

//Get Modem Handle
int modem_get_handle()
{
	return modemport.handle;
}

char *  modem_config (char *cmd )
{
	int  fd ,val,i,j=0,bytes;
	fd = modemport.handle ;
	//char wbuf[255]= "" ;

	val = strlen(cmd) ;
	if (cmd[val-1] == '\n' && cmd[val-2] == '\r')
	{
		write (fd, cmd,val) ;
		sleep(2) ;
	}
	else
	{
		for (i=0 ; cmd[i]!= '\0' ; i++)
			wbuf[j++] = cmd[i] ;
		wbuf[val++] = '\r' ;
		wbuf[val++] = '\n' ;
		wbuf[val++] = '\0' ;
		write (fd,wbuf,strlen(wbuf)) ;
		sleep(2) ;
	}
	
	sleep(1) ;
	ioctl(fd,FIONREAD,&bytes) ;
	read (fd,config_buf,20) ;
	return config_buf;
}//modem config

int modem_dial (const char *phno)
{
	int fd, i=0, j=0, flag=0, bytes=0, len, totlen =0 ;
	char buff[35] = "ATDT" ;

	fd = modemport.handle  ;

	if (phno[0] =='A' || phno[0] == 'a')
	{
		write (fd , phno,strlen(phno)) ;
		printf ("dialing.....\n") ;
	}
	else
	{
		buff[j++] = 'A' ;
		buff[j++] = 'T' ;
		buff[j++] = 'D' ;
		buff[j++] = 'T' ;
		
		for (i=0;phno[i]!='\0';i++)
		{
			buff[j++] =phno[i] ;
		}
		
		buff[j++] = '\r' ;
		buff[j++] = '\n' ;
		buff[j++] = '\0' ;
		
		if(write (fd,buff,strlen(buff))<strlen(buff))
			fprintf(stderr,"write Failed \n");
		
		printf ("dialing.....\n") ;
	}
	
	sleep(20) ;
	
	do
	{
		int i=0,cnt=0;
		ioctl(fd,FIONREAD,&bytes);

		len = read(fd,(dial_buf+totlen),255) ;
		dial_buf[len]= '\0';
		printf("DIAL_BUF = %s\n",dial_buf);
		usleep(10000);
		
		if(len>0)
		{
			totlen+=len;
			for(i=0;(i<totlen&&totlen>=2);i++)
			{
				if(dial_buf[i-1]=='T' && dial_buf[i-3] =='E'&&dial_buf[i-7]=='C')
					flag=1;
			}
		}
		
		cnt++;
		if(cnt == 50000)
			flag=2;
	} 
	while(len <=0 && errno==-EAGAIN && flag==0 && bytes>=0);

	/*   timeoutflag=0;
	signal(SIGALRM,alarm_handler);
	alarm(60);
	do{
	len = read(fd,(dial_buf+totlen),255) ;
	if(len>0){
	totlen+=len;
	dial_buf[totlen]='\0';
	}
	puts(dial_buf);
	if(strstr(dial_buf,"CONNECT")!=NULL)flag=1;
	else if(strstr(dial_buf,"NO")!=NULL)flag=2;
	}while(timeoutflag==0&&flag==0);
	alarm(0);*/

	printf("FLAG = %d\n",flag);
	if (timeoutflag)
	{
		puts ("TIMEOUT\n") ;
		return 1;
	}
	if (flag ==1)
	{
		status = CONNECTED;
		puts ("CONNECTED\n") ;
		return 0 ;
	}
	else
	{
		puts ("NOT CONNECTED\n") ;
		return -1 ;
	}
}

int modem_tx_data (unsigned char *txbuf , int tx_buf_len)
{
	int fd, wlen ;
	fd = modemport.handle ;

	wlen = write (fd, txbuf, tx_buf_len) ;
	if (wlen >0)
		return 0 ;
	else
		return -1 ;

	return -1 ;
}

int modem_rx_data (unsigned char *rxbuf, int rx_buf_len)
{
	int fd, rlen ;
	fd = modemport.handle ;
	sleep(1) ;

	if (status == CONNECTED)
	{
		rlen = read (fd, rxbuf, rx_buf_len) ;
		if (rlen >0)
			return 0 ;
		else
			return -1 ;
	}
	
	return -1 ;
}

int  modem_disconnect (void)
{
	int fd ;
	fd =modemport.handle ;                                                                                       
	ioctl(fd, TIOCMGET, &result);
	result &= ~TIOCM_DTR;
	sleep(5) ;
	ioctl(fd,TIOCMSET,&result);
	result |=TIOCM_DTR ;
	return 0 ;
}//modem disconnect

void alarm_handler (int signum)
{
	timeoutflag=1;
}
