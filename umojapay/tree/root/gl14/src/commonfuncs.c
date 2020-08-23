/*----------------------------------------------------------------------------------------------------------------------------------
 * CommonFuncs.h
 * Common Function Abstraction Module
 * @karokidii
 *==================================================================================================================================*/
 
 
 #include "commonfuncs.h"
 #include <V91magswipe.h>
 
 #undef _VARGS_DEFINED
 #undef _vargs_defined
 #include <stdarg.h>
 #include <stdio.h>
 #include <X6x8.h>
 #include <signal.h>
 
 #include "ppp_api.h"
 #define GL14VERSION
 #include "platform.h"
 /*
 #ifndef _vargs_defined
 #define _vargs_defined

 typedef char * va_list;
 #define _ADDRESSOF(v) (unsigned int)((char *)(&v))
 #define _INTSIZEOF(n)   ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )
 #define va_start(ap,v)  ( ap = (va_list)_ADDRESSOF(v) + _INTSIZEOF(v) )
 #define va_arg(ap,t)    ( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) )
 #define va_end(ap)      ( ap = (va_list)0 )
 
 #define _G_va_list va_list

#endif */

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#define SUCCESS 0
#define FAILURE 1

#define GSM 0
#define ETHERNET 1
 
 static int cur_x = 0, cur_y = 0;
 static int sim_present = 0;
 static int ppp_active = 0;
 static int eth0_active = 0;
 static int network_type = GSM;
 static int net_swap = 0;
 static int alarm_set = 0;
 static alarm_handler_t alarm_func = NULL;
 static int alarm_time = 60;
 
 static CURL *__gl14_curl_handle = NULL;

 int gl14_display_init()
 {
 	int ret = lk_open();
 	if(!ret) ret = lk_lcdintensity(24);
 	
 	lk_dispfont((void *)X6x8_bits, 6);
 	return (ret == 0);
 }
 
 int gl14_display_shut()
 {
 	lk_close();
 }
 
 int gl14_print_centered_row(unsigned char row, const char *text)
 {
 	int slen = strlen(text);
 	int halfl = slen >> 1;		//divide slen by 2
 	const int hscrl = 10;		//no. of screen characters per line divided by 2. Font 0 = 21 characters per line.
 	
 	int xof = hscrl - halfl;
 	xof = max(xof, 0);
 	
 	row = min(row, 5);
 	int p = lk_disptext(row, xof, (unsigned char *)text, 0);
 	return (p > 0);
 }
 
 int gl14_print_center(const char *text)
 {
 	int slen = strlen(text);
 	int halfl = slen >> 1;		//divide slen by 2
 	const int hscrl = 10;		//no. of screen characters per line divided by 2. Font 0 = 21 characters per line.
 	
 	int xof = hscrl - halfl;
 	xof = max(xof, 0);
 	
 	int p = lk_disptext(3, xof, (unsigned char *)text, 0);
 	return (p > 0);
 }
 
 int gl14_clear_row(int row)
 { 	
 	char *blankstr = "                     ";
 	row = min(row, 5);
 	int p = lk_disptext(row, 0, blankstr, 0);
 	return (p > 0);
 }
 
 int gl14_print(const char *text)
 {
 	return (lk_disptext(cur_y, cur_x, (unsigned char *)text, 0) > 0);
 }
 
 int gl14_set_cursor(int x, int y)
 {
 	y %= 6;
 	x %= 21;
 	
 	cur_x = x;
 	cur_y = y;
 	
 	return 1;
 }

 int gl14_printf(int mode, int x, int y, int font, const char *text, ...)
 {
 	char sbuf[32];
 	int ret = 0;
 	
 	va_list ap;
 	va_start(ap, text); 	
 	vsprintf(sbuf, text, ap);
 	va_end(ap);
 	
 	if(mode) gl14_print_center(sbuf);
 	else
 	{
 		gl14_set_cursor(x, y);
 		ret = gl14_print(sbuf);
 	}
 	
 	return ret;
 }
	
 int gl14_query_integer(char *caption, char *out, int max_chars, int password, int use_hint)
 {
	//Clear screen
	//Print Caption
	//Draw Box
	//Get Numeric Value and return
	lk_dispclr();
	gl14_print_centered_row(2, caption);
	lk_dispbox(4, 0);
	
	//Get integer from keypad and display on 4
	
	char buf[32];		//String buffer.
	int ret;
	
	char *dst = buf;
	
	if(!password)
	{
		int ilen = 0;
		
		if (use_hint)
		{
			use_hint = FALSE;
			ilen = strlen(out);
			
			if (ilen > 21) ilen = 0;
			else
			{
				dst = out;
				use_hint = TRUE;
			}
		}
		
		ret = lk_getnumeric(4, 2, dst, max_chars, ilen);
		if(ret < 1) return 0;
	}
	else
	{
		use_hint = FALSE;
		ret = lk_getpassword(buf, 4, max_chars);
		if(ret < 1) return 0;
	}
	
	dst[ret] = 0;
	if(out && !use_hint){ memcpy(out, buf, ret); out[ret] = 0; }
	
	return ret;
 }

 int gl14_query_string(char *caption, char *out, int max_chars, int password, int use_hint)
 {
	//Clear Screen
	//Print Caption
	//Draw Box
	//Get String and Store in [out]

	lk_dispclr();
	gl14_print_centered_row(2, caption);
	lk_dispbox(4, 0);
	
	//Get string from keypad and display on 4
	
	char buf[32];		//String buffer.
	int ret;
	char *dst = buf;
	
	if(!password)
	{
		int ilen = 0;
		
		if (use_hint)
		{
			use_hint = FALSE;
			ilen = strlen(out);
			
			if (ilen > 21) ilen = 0;
			else
			{
				dst = out;
				use_hint = TRUE;
			}
			
			//printf("USE HINT FLAG IS SET!\n");
			//printf("STR = %s, LEN = %d\n", dst, ilen); 
		}
		
		ret = lk_getalpha(4, 2, dst, max_chars, ilen, 0);
		if(ret < 1) return 0;
	}
	else
	{
		use_hint = FALSE;
		ret = lk_getpassword(buf, 4, max_chars);
		if(ret < 1) return 0;
	}
	
	dst[ret] = 0;
	if(out && !use_hint){ memcpy(out, buf, ret); out[ret] = 0; }
	
	return ret;
 }

 
 static void __gl14_menu_draw(menu_entry_t *entries, int first, int count, int selected)
 {
 	//Clear Screen
 	lk_dispclr();
 		
 	//int FONT = 0;
 	int col = 0;
 	int row = 0;
 	
 	char str[16];
 	for(int i = 0; i < count; ++i)
 	{
 		int idx = i+first;
 		//Get Display String
 		sprintf(str, "%d %s", (idx+1), entries[idx].display_name);
 		//Print this.
 		lk_disptext(row++, col, str, 0);
	}
	
 	if(selected >= first)
	{
		uint8_t sel = (uint8_t)(selected-first);
		if(sel < 6) lk_disphlight(sel);
	}
 }

 int gl14_do_menu(const char *title, menu_entry_t *menu_entries, int entry_count)
 {
 	//Maximum no. of lines displayed = 5
 	//Support scrolling.
 	
 	const int max_entries = 6;		//Max entries that can fit on screen
 	int first = 0;
 	int count = min(entry_count, max_entries);
 	int tmp;
 	int dirty = 1;					//Dirty flag. Set to redraw menu when changes occur.
 	int select = 0;
 	
 	lk_dispclr();
 	gl14_print_center(title);		//Flash the menu title briefly
 	sleep(1);
 	
 	while(1)
 	{
 		if(alarm_set)
 		{
 			if(alarm_func) alarm_func();
 			alarm_set = 0;
 			dirty = 1;
 			alarm(alarm_time);
 		}
 		
 		if(dirty)
 			__gl14_menu_draw(menu_entries, first, count, select);
 			 		
 		dirty = 0;
 		uint8_t lkey = lk_getkey();
 		
 		if((lkey <= entry_count) && (lkey <= 9) && (lkey > 0))
 		{
 			if((lkey > first) && (lkey <= (first+max_entries)))
 			{
 				select = (lkey-1);
 				dirty = 1;
 			}
 			
 			continue;
		}
		
		
		{//Empty Context
			switch(lkey)
			{
				case KEY_F3:
				{
					//Scroll down
					select ++;
					
					if(select >= entry_count)
						select = (entry_count-1);
					else
					{
						tmp = (first+max_entries-1);		//Page limit
						
						if(select > tmp)
						{
							first += max_entries;
							count = entry_count-first;
							count = min(count, max_entries);
							dirty = 1;
						}
					}
					
					dirty = 1;				
					continue;
				}
				case KEY_F2:
				{
					//Scroll up
					select --;
					
					if(select < 0) select = 0;
					else
					{
						if(select < first)				//Check if we are on previous page.
						{
							first -= max_entries;
							first = max(first, 0);
							count = entry_count-first;
							count = min(count, max_entries);
							dirty = 1;
						}
					}
					
					dirty = 1;
					continue;
				}
				case KEY_ENTER:
				{
					//Select tmp
 					menu_entry_t *entry = &menu_entries[select];
 					if(entry->callback) entry->callback(entry->arg_ptr);
 					else
 					{
 						lk_dispclr();
 						gl14_print_center("No User Function!");
 						sleep(2);
 					}
 					//return 1;
 					
 					dirty = 1;
 					break;
				}
				case KEY_CANCEL:
				{
					//Cancel this menu.
					goto finish;
				}//CASE
			}//Switch
		}//Empty Context
	}//While
	
 finish:
 	//Clear screen
 	lk_dispclr();
 	return SUCCESS;
 
 }
 
 size_t __gl14_curl_write_callback(void *data, size_t nsize, size_t count, void *stream)
 {
     size_t sz = nsize * count;
  	 url_data_t *mem = (url_data_t *)stream;
 	 mem->data = realloc(mem->data, mem->size + sz + 1);
 	
  	 if(mem->data == NULL)
  	 {
    	/* out of memory! */ 
    	 //printf("not enough memory (realloc returned NULL)\n");
    	 return 0;
     }
	
	 memcpy(&mem->data[mem->size], data, sz);
	 mem->size += sz;
	 mem->data[mem->size] = 0;
 
	 return sz;
 }
 
 int gl14_show_message(const char *message, unsigned int style, const char *title, unsigned int button_style)
 {
 	lk_dispclr();
 	lk_underline(1, 1, 19, 6);
 	gl14_print_centered_row(1, title);
 	gl14_print_centered_row(2, message);
 	
 	gl14_print_centered_row(5, "ENTER(OK)  or  CANCEL");
 	
 	while(1)
	{
		unsigned char c = lk_getkey();
		if(c == KEY_ENTER) return SUCCESS;
		if(c == KEY_CANCEL) return FAILURE;
	}
 }
 
 int gl14_do_question(const char *title, const char *prompt)
 {
 	return gl14_show_message(prompt, CHOICE, title, OK_CANCEL);
 }
 
 int gl14_do_curl_init()
 {
 	if(__gl14_curl_handle)
 	{
 	 	curl_easy_cleanup(__gl14_curl_handle);
		curl_global_cleanup();
 	}
 	
 	curl_global_init(CURL_GLOBAL_ALL);
 	__gl14_curl_handle = curl_easy_init();
 	
 	curl_easy_setopt(__gl14_curl_handle, CURLOPT_DNS_CACHE_TIMEOUT, -1);
 	curl_easy_setopt(__gl14_curl_handle, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(__gl14_curl_handle, CURLOPT_SSL_VERIFYHOST, 0);
	curl_easy_setopt(__gl14_curl_handle, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(__gl14_curl_handle, CURLOPT_CONNECTTIMEOUT, 60L);
	curl_easy_setopt(__gl14_curl_handle, CURLOPT_TIMEOUT, 90L);
	curl_easy_setopt(__gl14_curl_handle, CURLOPT_VERBOSE, 0L);
 }
 
 int gl14_do_curl_shut()
 {
 	if(!__gl14_curl_handle) return;
 	 
 	curl_easy_cleanup(__gl14_curl_handle);
	curl_global_cleanup();
	
	__gl14_curl_handle = NULL;
 }
 
 int gl14_do_curl_post(const char *url, const char *params, url_data_t *out)
 {
  	out->size = 0;
 		
 	CURLcode res;
	url_data_t mem;
	
	if(!url || !params) return FAILURE;

	CURL *curl_handle = __gl14_curl_handle;
	if (!curl_handle)
	{
		gl14_do_curl_init();
		curl_handle = __gl14_curl_handle;
	}
		
	if(!curl_handle) return FAILURE;
	
	mem.data=malloc(1);
	mem.data[0] = 0;
	mem.size=0;
	
	curl_easy_setopt(curl_handle, CURLOPT_URL, url);
	curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, params);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, __gl14_curl_write_callback);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&mem);
	res = curl_easy_perform(curl_handle);
		
//#endif

//	url_data_t mem;
//	mem.data = (void *)strdup("Butchery Licence;Hawker's Licence");	
	//mem.size = strlen((char *)mem.data);
	
	int sz = strlen(mem.data);
	out->size = sz;
	out->data = mem.data;
	
	//char str[16];

	//gl14_print_center("CURL FINISHED.");
	//sleep(2);
	//if(out->size) gl14_print_center((char *)out->data);
	//sleep(2);

	return SUCCESS;	
 }
 
 int gl14_magcard_open()
 {
 	return (mscr_open() < 0)? FAILURE: SUCCESS;
 }
 
 int gl14_magcard_close()
 {
 	return (mscr_close() < 0)? FAILURE: SUCCESS;
 }
 
 int gl14_magcard_flush()
 {
 	return (mscr_flush() < 0)? FAILURE: SUCCESS;
 }
 
 int gl14_magcard_read(uint8_t *Track1Data, uint8_t *Track1Length, uint8_t *Track2Data, uint8_t *Track2Length, uint8_t *Track3Data, uint8_t *Track3Length)
 {
 	char dummy[256], dummy2[256], dummy3[256];
 	
 	memset(dummy, 0, 256);
 	memset(dummy2, 0, 256);
 	memset(dummy3, 0, 256);
 	
 	char *Track1 = (Track1Data)? Track1Data: dummy;
 	char *Track2 = (Track2Data)? Track2Data: dummy2;
 	char *Track3 = (Track3Data)? Track3Data: dummy3;
 	
 	int Status = 0;
 	
 	while(1)
 	{
 		Status = mscr_getstate();
 		
 		if(Status == 1)
 		{
 			Status = mscr_read(Track1, Track2, Track3);
 			sleep(2);
 			break;
 		}
 		
 		sleep(1);
 	}
 	
 	if(Track1Length) *Track1Length = 0;
 	if(Track2Length) *Track2Length = 0;
 	if(Track3Length) *Track3Length = 0;
 	
 	switch(Status)					/* We have to use a switch insted of bitflags here... */
 	{
 		case -1: return FAILURE;
 		case 1:
 		{	
 			if(Track1Length && Track1Data) *Track1Length = strlen(Track1);
 			break;
 		}
  		case 2:
 		{	
 			if(Track2Length && Track2Data) *Track2Length = strlen(Track2);
 			break;
 		}
  		case 3:
 		{	
 			if(Track3Length && Track3Data) *Track3Length = strlen(Track3);
 			break;
 		}
 		case 4:
 		{	
 			if(Track1Length && Track1Data) *Track1Length = strlen(Track1);
 			if(Track2Length && Track2Data) *Track2Length = strlen(Track2);
 			break;
 		}
 		case 5:
 		{
  			if(Track2Length && Track2Data) *Track2Length = strlen(Track2);
 			if(Track3Length && Track3Data) *Track3Length = strlen(Track3);
 			break;			
 		}
 		case 6:
 		{	
 			if(Track1Length && Track1Data) *Track1Length = strlen(Track1);
 			if(Track3Length && Track3Data) *Track3Length = strlen(Track3);
 			break;
 		}
 		case 7:
 		{
 			if(Track1Length && Track1Data) *Track1Length = strlen(Track1);
 			if(Track2Length && Track2Data) *Track2Length = strlen(Track2);
 			if(Track3Length && Track3Data) *Track3Length = strlen(Track3);
 			break;
 		} 		
	}
	
	return SUCCESS;
 }
 
 int gl14_printer_line(unsigned char symbol, int Font)
 {
 	char *s = (char *)malloc(48);
 	
 	for(int i = 0; i < 42; ++i) s[i] = (char)symbol;

 	int line_size = 42;
 	if(Font == FONTLARGEBOLD) line_size = 21;
 	
 	s[line_size++] = '\n';
 	s[line_size] = 0;
 	
 	int err = prn_write_text(s, line_size, Font);
 	free(s);
 	
 	return err;
 }
 
 int gl14_printer_flush()
 {
 	 //prn_write_text(n_array, 4, 0);
 	 //prn_close();
 	 //prn_open();
 	 
	 return SUCCESS;
 }
 
 int gl14_printer_feed(int Lines)
 {
 	int stat = SUCCESS;
 	for(int i = 0; i < Lines; ++i)
 		stat = prn_write_text("\n", 1, 0);
 		
 	if (stat < 0) return FAILURE;
 	return SUCCESS;
 }
 
 int gl14_printer_open()
 {
 	return (prn_open() < 0)? FAILURE: SUCCESS;
 }
 
 int gl14_printer_close()
 {
 	return (prn_close() < 0)? FAILURE: SUCCESS;
 }
 
 int gl14_printer_print_text(char *text, int length, int font)
 {
 	return (prn_write_text(text, length, font) < 0)? FAILURE: SUCCESS;
 }
 
 int gl14_printer_print_bitmap(void *bitmap, unsigned int length)
 {
 	if(prn_write_bmp((uint8_t *)bitmap, (long)length) < 0) return FAILURE;
 	return SUCCESS;
 }
 
int test_modem_response(char *command, char *response, char *expect[], int timeout)
{
	struct timeval tv;
	int result, count, tot_bytes_read=0, bytes_avlbl=0;
  
	int Modemfd = modem_get_handle();
  
	fd_set rfds;

	FD_ZERO(&rfds);
	FD_SET(Modemfd, &rfds);
  
	tv.tv_sec =timeout;
	tv.tv_usec=0;
	tcflush(Modemfd, TCIOFLUSH);
  
	if(modem_tx_data(command, strlen(command))<0)
    {
    	fprintf(stderr,"Writing Command to the Modem Failed\n");
    	return -1;
    }
  
  	do
    {
    	int i=0;
    	result = select(Modemfd+1, &rfds, NULL, NULL, &tv);
      
    	if(result>0)
		{
			ioctl(Modemfd, FIONREAD, &bytes_avlbl);
	  	
	  		if(bytes_avlbl > 0)
	    		count = read(Modemfd, (response+tot_bytes_read), bytes_avlbl);
	  	
	  		if(count>0)
	   		{
	    		tot_bytes_read += count;
	    		response[tot_bytes_read] = '\0';
	    	}
		}
      
   		for(i=0; expect[i] != NULL; i++)
        	if(strstr(response, expect[i]))
	  			return i;
    }
    while(tv.tv_sec > 0);
 
  	return -2;
}

 int sim_status_test (void)
 {
       int res;
       char response[256];
       char *expect[32]={"READY", "ERROR", NULL};

       res = modem_open();
        
       if(res<0)
       {
           printf("Modem Open failed!!");
           sleep(1);
           return -1;                
       }
         
       modem_setspeed(9600);
       memset(response, 0, 256);

       res = test_modem_response("AT+CPIN?\r\n", response, expect, 10);

       fprintf(stderr, "The value of res=%d\n", res);
       sleep(1); 

       printf("Response of GSM Sim = %s\n", response);
       modem_close();

       if (res) return -1; // sim is not there
            
       else return 0;      // sim is present
 }
 
 int gl14_calibrate_gsm_network()
 {
 	int err= 0;
 	
 	 if (!sim_present)
 	 {
 	 	if(sim_status_test() >= 0)
 	 		sim_present = 1;
 	 	else
 	 		err = -1;
 	 }
 	 	 
	 if(err)
 	 {
 		 gl14_print_center("No SIM card detected!!");
 		 sleep(1);
 		 return FAILURE;
	 }
	 else
		 gl14_print_center("SIM detected OK.");
	 
enter_settings:;

	gprs_settings_t gprs_info;
	 
	 gprs_info.apn_name[0] = 0;
	 gprs_info.username[0] = 0;
	 gprs_info.password[0] = 0;
	//strcpy(gprs_info.apn_name, "safaricom");
	//strcpy(gprs_info.username, "saf");
	//strcpy(gprs_info.password, "data");
	 
	err = gl14_query_string("Enter APN", gprs_info.apn_name, 16, FALSE, FALSE);
	if(!err) goto settings_failed;
	 
	//gl14_query_integer("Enter Dial-Out Number", gprs_info.phone_no, 8, FALSE);
	//if(!strlen(gprs_info.phone_no)) goto settings_failed;
	strcpy(gprs_info.phone_no, "*99#");
	 
	 gl14_query_string("Enter Username", gprs_info.username, 10, FALSE, FALSE);
	 gl14_query_string("Enter Password", gprs_info.password, 10, FALSE, FALSE);
	 
	 ppp_options(gprs_info.username, gprs_info.password);
	 
	 err = pppdial_gprs(gprs_info.phone_no);
	 if(err < 0) return FAILURE;
	 
	 ppp_dial_update(gprs_info.phone_no, gprs_info.apn_name);
	 return SUCCESS;
	 
settings_failed:;
	
	int res = gl14_do_question("RETRY?", "User canceled configuration");
	if(res == SUCCESS) goto enter_settings;
	return FAILURE;
 }
 
 int gl14_calibrate_ethernet_network()
 { 		 
enter_settings:;

	char gateway[16];
	char netmask[16];
	char static_ip[16];
	
	strcpy(netmask, "255.255.255.0");
	strcpy(gateway, "192.168.1.1");
	
	int err = gl14_show_message("Use automatic settings?", CHOICE,"Use DHCP?", YES_NO);
	if(err == SUCCESS)
	{
		lk_dispclr();
		gl14_print_center("Please wait ...");
		
		system("ifconfig eth0 up");
		system("udhcpc --interface=eth0");
		system("route add default gw eth0");
		
		lk_dispclr();
		return SUCCESS;
	}
	
	err = gl14_query_string("Enter IP (Static)", static_ip, 15, FALSE, FALSE);
	if(!err) goto settings_failed;

	err = gl14_query_string("Enter Netmask", netmask, 15, FALSE, TRUE);
	if(!err) goto settings_failed;
	
	int fill_gw = FALSE;
	if (strstr(static_ip, "192.168.1")) fill_gw = TRUE;
	
	err = gl14_query_string("Enter Gateway", gateway, 15, FALSE, fill_gw);
	if(!err) goto settings_failed;
	
	char str[64];
	sprintf(str, "ifconfig eth0 %s netmask %s", static_ip, netmask);
	system(str);
	
	sprintf(str, "route add default gw %s", gateway);
	system(str);
	
	FILE *fd = fopen("/etc/resolv.conf", "w+");
	if(fd)
	{
		fprintf(fd, "nameserver %s", gateway);
		fclose(fd);
	}

	 return SUCCESS;
	 
settings_failed:;
	
	int res = gl14_do_question("RETRY?", "User canceled configuration");
	if(res == SUCCESS) goto enter_settings;
	return FAILURE;
 }
 
 void set_network(void *result)
 {
 	int p = *(int *)result;
 	if(!p)
 	{
 		if(network_type == GSM) goto finale;
 		
 		net_swap = 1;
 		gl14_net_disable();
 		network_type = GSM;
 	}
 	else
 	{
 		if(network_type == ETHERNET) goto finale;
 		
 		net_swap = 1;
 		gl14_net_disable();
 		network_type = ETHERNET;
 	}
 	
 finale:;
 
 	lk_dispclr();
 	gl14_print_center("Network Selected.");
 	sleep(1);
 }
 
 void gl14_choose_network()
 {
 	 int list[] = {0, 1, 2, 3};
 	 
 	 menu_entry_t menulist[] = { {"MOBILE NETWORK", 1, 1, set_network, list},
 	 							 {"ETHERNET", 1, 2, set_network, &list[1]}
 	 						   };
 	 
 	 gl14_do_menu("Choose Connection", menulist, 2);
 }
 
 void gl14_calibrate_network()
 {
 	int res = SUCCESS;
 	
 	if(network_type == ETHERNET)
 		res = gl14_calibrate_ethernet_network();
 	else
 		res = gl14_calibrate_gsm_network();
 		
 	if(res != SUCCESS)
 	{
 		lk_dispclr();
 		gl14_print_center("Calibration Error!!");
 		sleep(1);
 	}	
 }
  
 int gl14_net_enable()
 {
 	/* Routine to Enable GSM internet connection.
 		@NOTE: This function uses undocumented APIs from VISIONTEK. */
 	
 	/* Procedure:
 		1. Check For SIM.
 		2. Get APN
 		3. Get UserName
 		4. Get Password
 		5. Attempt Dial-Out to ISP.
 	*/
 	 //system("sim1_enable");
 	 
 	 if((network_type == GSM) && !sim_present)
 	 {
 	 	if(sim_status_test() >= 0) sim_present = 1;
 	 	else
 	 	{
 	 		lk_dispclr();
 	 		gl14_print_center("No SIM card!!");
 	 		return FAILURE;
 	 	}
	 }
	 	   
	 int err;
	 if(network_type == GSM)
	 	err = (ppp_active)? 0: ppp_open();
	 else
	 {
	 	err = 0;
	 	system("ifconfig eth0 up");
	 }
	 
	 if(err < 0) return FAILURE;
	
	 if(network_type == GSM)
	 	ppp_active = 1;
	 else
	 	eth0_active = 1;
	 	
	 return SUCCESS;
 }
 
 int gl14_net_disable()
 {
      gl14_do_curl_shut();
      
 	  if(net_swap)
 	  {
 	  		if(ppp_active && (network_type == GSM))
 	  			ppp_close();
 	  		else if(network_type == ETHERNET)
 	  			system("ifconfig eth0 down");
 	  		
 	  		ppp_active = 0;
 	  		eth0_active = 0;
 	  }
 	  //ppp_close();
 	  sleep(1);
 	  return SUCCESS;
 }
 
 int gl14_get_gsm_location(void *buffer)
 {
 	if(!sim_present)
 	{
 	 	if(sim_status_test() >= 0) sim_present = 1;
 	 	else
 	 	{
 	 		gl14_print_center("No SIM card!!");
 	 		return FAILURE;
 	 	}
 	}
 	
 	//SIM PRESENT
 	//@STEPS:
 	//Set AT+CREG=2
 	//Send AT+CREG?
 	//Get Result.
 	//Restore AT+CREG=1
 	
 	int restore_ppp = 0;
 	if (ppp_active)
 	{
 		ppp_close();
 		ppp_active = 0;
 		restore_ppp = 1;
 	}
 	
 	char response[256];
 	char *expect[32] = {"READY", "OK", "ERROR", NULL};
 	
 	uint32_t MNC, MCC;
 	int res = test_modem_response("AT+COPS=0,0,2\r\n", response, expect, 10);
 	if(res > 1) goto failed;
 	
 	test_modem_response("AT+COPS?\r\n", response, expect, 10);
	
	//Scan Output for MCC& MNC
	printf("Modem Responded: %s\n", response);
	sscanf(response,"+COPS: %d,%d,\"%X %X\"", &MNC, &MCC);
	
	char *out = (char *)buffer;
	char sbuf[64];
	sprintf(sbuf, "mcc=%X&mnc=%X", MNC, MCC);
	strcpy(out, sbuf);
 	 	
 	res = test_modem_response("AT+CREG=2\r\n", response, expect, 10);
 	
 	if(res > 1) goto failed;
 	res = test_modem_response("AT+CREG?\r\n", response, expect, 10);
 	
 	//Scan Output for +CREG: Params.
 	//Scan Response for +CREG:%d,%d, %X, %X
 	uint32_t M0, M1, LAC, CELLID;
 	sscanf(response, "+CREG: %d,%d,\"%X\",\"%X\"", &M0, &M1, &LAC, &CELLID);
 	
 	sprintf(sbuf, "&lac=0x%X&cellid=0x%X", LAC, CELLID);	
 	strcat(out, sbuf);
 	
 	res = test_modem_response("AT+CREG=1\r\n", response, expect, 10);
 	 	
 	//if(res > 1) reset_modem();
 	printf ("\nGsmLocate().Results: %s\n", out);
 	return SUCCESS;
 	
 failed:
 	
 	if (restore_ppp)
 	{
 		ppp_open();
 		ppp_active = 1;
	}
	
	return FAILURE;
 }
 
 //=====================================================================================================================================================
 
 void alarm_catcher(int sig)
 {
 	//catches a SIGALARM event.
 	alarm_set = 1;
 	signal(sig, alarm_catcher);
 }
 
 int gl14_set_alarm(alarm_handler_t func, int seconds)
 {
 	 alarm_set = 0;
 	 alarm_func = func;
 	 alarm_time = seconds;
 	 
 	 signal(SIGALRM, alarm_catcher);
 	 
 	 alarm(seconds);
 	 return SUCCESS;
 }
 
//=====================================================================================================================================================
// FTP file downloads.
//=====================================================================================================================================================
 
typedef struct _ftp_out_file
{
	const char *file_name;
	FILE *stream;
}
ftp_file_t;
 
size_t gl14_ftp_fwrite(void *buffer, size_t size, size_t count, void *stream)
{
	ftp_file_t *out = (ftp_file_t *)stream;
	
	if(out && !out->stream)
	{
    	/* open file for writing */ 
    	out->stream = fopen(out->file_name, "wb");
    	if(!out->stream)
      		return -1; /* failure, can't open file to write */ 
  	}
  	
  	printf("DATA: %d, %d", size, count);
  	printf("\n");
  	
  	char tstr[8];
  	int kb = (ftell(out->stream) + (size*count)) / 1024;
  	sprintf(tstr, "DL: %d K", kb);
  	
  	gl14_clear_row(4);
  	disp_print_row(4, tstr);
  	
  	return fwrite(buffer, size, count, out->stream);
}

int gl14_ftp_download(const char *url, const char *filename)
{
	CURL *curl;
	CURLcode res;
	ftp_file_t ftp_file;
 
	curl = __gl14_curl_handle;
	if (!curl)
	{
		gl14_do_curl_init();
		curl = __gl14_curl_handle;
	}
			
	if(!curl) return FAILURE;
	
	ftp_file.file_name = filename;
	ftp_file.stream = NULL;
  
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, NULL);
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, gl14_ftp_fwrite);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ftp_file);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 60L);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 600L);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
    
    res = curl_easy_perform(curl);
    
    //Restore CURL handle options to our defaults...
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 60);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 90L);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
  
    if(ftp_file.stream)
    	fclose(ftp_file.stream);
 
    if(res != CURLE_OK)
    {
      	/* we failed */ 
    	fprintf(stderr, "CURL ERROR: %d\n", res);
    	char buf[128];
    	sprintf(buf, "rm -rf %s", filename);
    	system(buf);
    	
    	return FAILURE;
    }
 
  	return SUCCESS;
}
