 #include "global.h"
 #include "commonfuncs.h"
 #include <stdio.h>
 #include <stdlib.h>
 #include <time.h>
 
 #include "platform.h"
 #include "common.h"
 
 static char serial_no[32];
 static char string_buf[128];
 static char *print_header = "";
 static unsigned char *print_bitmap = NULL;
 static unsigned long logo_size = 0;

uint32_t generate_transaction_id()
{
	uint32_t clk = (uint32_t)time(NULL);
	uint32_t x = clk >> 13;
	x ^= clk;
	
	return x;
}

int verify_version(int major, int minor, int build)
{
	//Return SUCCESS if the queried version is less than or equal to this.
	
	if (major > VERSION_MAJOR) return FAILURE;
	if (minor > VERSION_MINOR) return FAILURE;
	if (build > BUILD_NUMBER) return FAILURE;
	
	return SUCCESS;
}
 
void set_print_header(const char *str)
{
	print_header = (char *)str;
}
 
void set_print_logo(const void *data, unsigned long length)
{
	print_bitmap = (char *)data;
	logo_size = length;
}
  
int get_serial()
{

	memset(serial_no, 0, 32);		
	int res = read_serial_no(&serial_no[0], 20);
	
	int len = strlen(serial_no);
	if (len < 1) return res;
	
	int to_trim = 0;
	
	for (int i = 0; i < len; ++i)
	{
		if ((serial_no[i] == 32 ||
			 serial_no[i] == 9 ||
			 serial_no[i] == 10)
			 
			 && !to_trim) continue;
		
		if (!to_trim) to_trim = (i+1);
		
		if (serial_no[i] < '0') serial_no[i] = 0;	//Less than 0
		if (serial_no[i] > '9')						//Not numerical
		{
			if (serial_no[i] < 'A' || serial_no[i] > 'z')
				serial_no[i] = 0;					//Not alphabetical
		}
	}
	
	if (to_trim > 1)
	{
		char tmps[32];
		strcpy(tmps, serial_no);
		char *trimmed = _trim_trim(tmps);
		
		strcpy(serial_no, trimmed);
	}
			
	return res;
}

int get_amount(bill_t *bill)
{
	int ret = get_integer("Enter Amount", string_buf, 9, 0, FALSE);
	if(ret) bill->amt = atoi(string_buf);
	return ret;
}

int get_phone_no(bill_t *bill)
{
	int ret = get_integer("Enter Phone No.", bill->phone_no, 12, 0, FALSE);
	return ret;
}

int get_pin(bill_t *bill)
{
	int ret = get_integer("Enter PIN", bill->pin, 4, 1, FALSE);
	return ret;
}

int get_acc_no(bill_t *bill)
{
	int ret = get_integer("Enter Account No.", bill->account_no, 12, 0, FALSE);
	return ret;
}

int get_business_no(bill_t *bill)
{
	int ret = get_integer("Enter Customer No.", bill->business_no, 12, 0, FALSE);
	return ret;	
}

char *_trim(char *string)
{
	char *s = string;
	while(*s)
	{
		char c = *s;
		switch(c)
		{
			case 32:
			case 9:
			case '\"':
			case '\n':
				s++;
				continue;
		}
		
		break;
	}
	
	return s;
}

char *_trim_trim(char *string)
{
	char *s = string;
	while(*s)
	{
		char c = *s;
		switch(c)
		{
			case 32:
			case 9:
			case '\"':
			case '\n':
				s++;
				continue;
		}
		
		break;
	}
	
	char *out = s;
	//Find end of string.
	while (*s) s++;
	
	s --;		//last valid character
	while (1)
	{
		char c = *s;
		if (c == 32 || c == 9 || c == '\"')
		{
			*s = 0;
			s --;
			continue;
		}
		
		//We found a proper character.
		//Break here.
		break;
	}
	
	return out;
}

char *_ucase(char *str)
{
#define _cvt_upper(cr) (cr >= 'a' && cr <= 'z')? (cr - 'a') + 'A': cr
	char *s = str;
	while(*s)
	{
		char c = *s;
		c = _cvt_upper(c);
		*s++ = c;
	}

	return str;
#undef _cvt_upper
}

char *drop_spaces(const char *string, char *out)
{
	char *s = string;
	char *o = out;
	
	while(*s)
	{
		char c = *s++;
		
		if(c == 32) continue;
		if(c == 9) continue;
		
		*o++ = c;
	}
	
	*o = 0;
	return out;
}

const char *fetch_serial_no()
{
	return (const char *)((void *)&serial_no[0]);
}

void get_next_line(char *str, char *out)
{
	char *s = str;
	char *o = out;
	while(*s)
	{
		*o++ = *s++;
		if(*s == '\n')
		{
			*o = 0;
			return;
		}
	}
	
	*o = 0;
}

char *get_next_line_ex(char *str, char *out)
{
	char *s = str;
	char *o = out;
	while(*s)
	{
		*o++ = *s++;
		if(*s == '\n')
		{
			*o = 0;
			return ++s;
		}
	}
	
	*o = 0;
	return NULL;
}

char ** str_tokenize(char *stres, int *_ct, const char *delimiter)
{
	char *tmp, *bak;

	bak = strdup(stres);
	tmp = strtok(stres, delimiter);
	int count = 0;
	*_ct = 0;

	while(tmp)
	{
		++count;
		tmp = strtok(NULL, delimiter);
	}

	if(!count) return NULL;

	int i=0;
	char **list = (char **)malloc(count * sizeof(char *));
	tmp = strtok(bak, delimiter);

	while(tmp)
	{
		list[i] = (char *)malloc(24);
		strcpy(list[i++], tmp);
		tmp = strtok(NULL, delimiter);
	}
	
	free(bak);
	*_ct = count;
	return list;
}

char *copy_string_until(char *src, char *dst, char lim)
{
	int i;
	int ilen = strlen(src);
	
	for (i = 0; i < ilen; ++i)
	{
		if (src[i] == lim || src[i] == 0) break;
		dst[i] = src[i];
	}
	
	dst[i] = 0;
	
	if (src[i])
		return &src[i+1];
	else
		return NULL;
}

char ** tokenize(char *stres, int *_ct)
{
	return str_tokenize(stres, _ct, ";");
}

void prepare_printer_string(char *str)
{	
#define _cvt_upper(cr) (cr >= 'a' && cr <= 'z')? (cr - 'a') + 'A': cr
	
	char *s = str;
	int preserve = FALSE;		//Preserve text within ""
	
	while(*s)
	{
		char c = *s;
		switch(c)
		{
			case ',':
			case '{':
			{
				if (!preserve)
					*s = '\n';
				break;
			}
			case '\"':
			{
				preserve ^= 1;
				*s = 32;
				break;
			}
			case '}':
			case '>':
			case '[':
			case ']':
				*s = 32;
				break;
			case '=':
				*s = ':';
				break;
			default:
				*s = _cvt_upper(c);
				break;
				/*Etc */
		}
		
		s++;		//Next character
	}
#undef _cvt_upper
}

int print_receipt(const char *body, const char *message, const char *tail, int flags)
{
	printer_flush();
	int status = printer_feed(1);
	
	if (status != SUCCESS)
	{
#ifndef IS_DEBUG_VERSION
	 	return FAILURE;
#else
		return SUCCESS;
#endif
	}
	
	prepare_printer_string((char *)body);
	char *formatted_result = align_printer_string((char *)body);
	
	if(print_bitmap && logo_size)
	{
		printer_print_bitmap(print_bitmap, logo_size);
		printer_print_text("\n", 1, FONTSMALLNORMAL);
	}
	
	printer_print_text(print_header, strlen(print_header), FONTLARGEBOLD);
	printer_print_line('_', FONTLARGEBOLD);
	
	char sbuf[64];
	sprintf(sbuf, "        TERMINAL ID: %s\n\n", serial_no);
	printer_print_text(sbuf, strlen(sbuf), FONTSMALLNORMAL);
	
	if (flags & PRINTFLAG_MARK_DUPLICATE)
	{
		const char *dupmsg = "      ** DUPLICATE RECEIPT **\n\n";
		printer_print_text(dupmsg, strlen(dupmsg), FONTSMALLNORMAL);
	}
	//printer_print_line('_', FONTSMALLNORMAL);
	
	printer_print_text(message, strlen(message), FONTSMALLNORMAL);
	
	printer_print_text(formatted_result, strlen(formatted_result), FONTSMALLNORMAL);
	
	printer_print_line('_', FONTLARGEBOLD);
	
	char *footer = "\nSign: ..............................\nThank You. Keep Receipt.\n";
	printer_print_text(footer, strlen(footer), FONTSMALLNORMAL);
	
	printer_print_line('=', FONTSMALLNORMAL);
	int last_print_status = printer_print_text(tail, strlen(tail), FONTSMALLNORMAL);
	
	status = printer_begin_print();
	printer_flush();
	
	free(formatted_result);

#ifndef IS_DEBUG_VERSION
	if (status != SUCCESS || last_print_status != SUCCESS) return FAILURE;
#endif

	return SUCCESS;
}

int print_receipt_ex(const char *leader, const char *message, const char *info_block, const char *identifier_block,
					 const char *other_data, const char *tail, uint32_t flags)
{
	printer_flush();
	int status = printer_feed(1);
	
	if (status != SUCCESS)
	{
#ifndef IS_DEBUG_VERSION
	 	return FAILURE;
#else
		return SUCCESS;
#endif
	}
		
	if(print_bitmap && logo_size)
	{
		printer_print_bitmap(print_bitmap, logo_size);
		printer_print_text("\n", 1, FONTSMALLNORMAL);
	}
	
	//Print the receipt header ...
	
	printer_print_text(print_header, strlen(print_header), FONTLARGEBOLD);
	printer_print_line('_', FONTLARGEBOLD);
	
	//Print the leader section ...
	
	char sbuf[64];
	char stmp[64];
	
	sprintf(sbuf, "TERMINAL ID     : %s\n", serial_no);
	printer_print_text(sbuf, strlen(sbuf), FONTSMALLNORMAL);
		
	if (leader)
		printer_print_text(leader, strlen(leader), FONTSMALLNORMAL);
	
	printer_print_text(message, strlen(message), FONTSMALLNORMAL);
	
	if (flags & PRINTFLAG_MARK_DUPLICATE)
	{
		const char *dupmsg = "** DUPLICATE RECEIPT [ASK FOR ORIGINAL] **\n";
		printer_print_text(dupmsg, strlen(dupmsg), FONTSMALLNORMAL);
	}
	
	printer_print_line('_', FONTSMALLNORMAL);
	
	//Print the information block
	if (info_block)
		printer_print_text(info_block, strlen(info_block), FONTSMALLNORMAL);
	
	if (identifier_block)
	{
		char *fsection = copy_string_until(identifier_block, stmp, ':');
		char *idstr = _trim_trim(stmp);
		sprintf(sbuf, "%s\n", idstr);
		
		printer_print_text(sbuf, strlen(sbuf), FONTSMALLNORMAL);
		
		char *ident = _trim(fsection);
		
		int ilen = strlen(ident);
		ilen = (18 - ilen);
		if (ilen < 0) ilen = 0;
		sbuf[0] = 0;
		
		for (int loop = 0; loop < ilen; ++loop)
			strcat(sbuf, " ");
			
		strcat(sbuf, ident);
		//strcat(sbuf, "\n");
		
		printer_print_text(sbuf, strlen(sbuf), FONTLARGEBOLD);
	}
	
	if (other_data)
		printer_print_text(other_data, strlen(other_data), FONTSMALLNORMAL);
	
	printer_print_line('=', FONTLARGEBOLD);
	
	//char *svc_string = "You were served by:        \n\n";
	//printer_print_text(svc_string, strlen(svc_string), FONTSMALLNORMAL);
	
#if 0	
	char *footer = "\nSign: ..............................\nThank You. Keep Receipt.\n";
	printer_print_text(footer, strlen(footer), FONTSMALLNORMAL);
	
	printer_print_line('=', FONTSMALLNORMAL);
	int last_print_status = printer_print_text(tail, strlen(tail), FONTSMALLNORMAL);
#endif
	
	if (flags & PRINTFLAG_INCLUDE_SIGN)
	{
		char *sign_note = "\nSign: ..............................";
		printer_print_text(sign_note, strlen(sign_note), FONTSMALLNORMAL);
	}
	
	char *thank_note = "\nThank You. Keep Receipt.\n";
	printer_print_text(thank_note, strlen(thank_note), FONTSMALLNORMAL);
	
	if (flags & PRINTFLAG_INCLUDE_SIGN) printer_print_line('=', FONTSMALLNORMAL);
	
	int last_print_status = printer_print_text(tail, strlen(tail), FONTSMALLNORMAL);
	
	status = printer_begin_print();
	printer_flush();
	
#ifndef IS_DEBUG_VERSION
	if (status != SUCCESS || last_print_status != SUCCESS) return FAILURE;
#endif

	return SUCCESS;
}

int print_result(url_data_t *result, char *header_message, int print_double, int flags)
{
	if(!result->size) return 0;
	
	printer_open();
	
	disp_clear_viewport();
	disp_print_center("Printing ...", ARIEL16_18);
	
	char *ack_customer_tail = "visit www.umojapay.com\n\nCustomer's Copy\n";
	char *ack_agent_tail = "visit www.umojapay.com\n\nAgent's Copy\n";
	
	char *customer_tail = "visit www.umojapay.com\n";
	char *agent_tail = customer_tail;
	
	if (flags & PRINTFLAG_INCLUDE_TAIL)
	{
		customer_tail = ack_customer_tail;
		agent_tail = ack_agent_tail;
	}
	
	int status;
	unsigned char key;
	
reprint:;
			
	if(!print_double) goto print_customer_copy;
	
	status = print_receipt((const char *)result->data, (const char *)header_message, agent_tail, flags);
	if(status != SUCCESS)
	{
		show_message("Out Of Paper!", WARNING, "ERROR!", OK);
		goto printer_error;
	}
	else
		printer_feed(4);
		
	disp_clear_viewport();
	disp_print_center("Press Key To Continue", ARIEL16_18);
	key = kpd_getkey();
	
	if(key == KEY_CANCEL)
	{
		int cc = show_message("Cancel Print-out?", CHOICE, "Really cancel?", YES_NO);
		if(cc == SUCCESS)
		{
			disp_clear_viewport();
			disp_print_center("Print canceled!!", ARIEL16_18);
			key = kpd_getkey();
			
			return 0;
		}
	}
	
print_customer_copy:;

	disp_clear_viewport();
	disp_print_center("Printing ...", ARIEL16_18);
	
	status = print_receipt((const char *)result->data, (const char *)header_message, customer_tail, flags);
	if(status != SUCCESS)
	{
		show_message("Out Of Paper!", WARNING, "ERROR!", OK);
		goto printer_error;
	}
	else
	{
		printer_feed(4);
		//show_message("Receipt printed.", INFORMATION, "UMOJAPAY", OK);
		
		disp_clear_viewport();
		disp_print_row(2, "Receipt Printed");
		
		disp_print_row(4, "Enter to Continue");
		disp_print_row(5, "F1 to print again");
		
		while(1)
		{
			key = kpd_getkey();
			
			if (key == KEY_CANCEL || key == KEY_ENTER) break;
			if (key == KEY_F1) goto reprint;
		}
	}
		
	printer_close();
	disp_clear_viewport();
	
	return SUCCESS;
	
printer_error:;
	
	disp_clear_viewport();
	disp_print_center("Set paper and press a Key.", ARIEL16_18);
	kpd_getkey();
	goto reprint;
}

int print_result_ex(char *header_message, const char *header_fields, const char *info_fields, const char *id_field,
				 	const char *other_fields, int print_double)
{					 
	printer_open();
	
	disp_clear_viewport();
	disp_print_center("Printing ...", ARIEL16_18);

	char *customer_tail = "visit www.umojapay.com\n";//\nCustomer's Copy";;
	char *agent_tail = "visit www.umojapay.com\n";//\nAgent's Copy";
	
	int status;
	int flags = PRINTFLAG_DEFAULT;
	unsigned char key;
	
reprint:;
		
	if(!print_double) goto print_customer_copy;
	
	status = print_receipt_ex(header_fields, header_message, info_fields, id_field, other_fields, agent_tail, flags);
	if(status != SUCCESS)
	{
		show_message("Out Of Paper!", WARNING, "ERROR!", OK);
		goto printer_error;
	}
	else
		printer_feed(3);
		
	disp_clear_viewport();
	disp_print_center("Press Key To Continue", ARIEL16_18);
	key = kpd_getkey();
	
	if(key == KEY_CANCEL)
	{
		status = show_message("Cancel Print-out?", CHOICE, "Really cancel?", YES_NO);
		if(status == SUCCESS)
		{
			disp_clear_viewport();
			disp_print_center("Print canceled!!", ARIEL16_18);
			key = kpd_getkey();
			
			return 0;
		}
	}
	
print_customer_copy:;

	disp_clear_viewport();
	disp_print_center("Printing ...", ARIEL16_18);
	
	status = print_receipt_ex(header_fields, header_message, info_fields, id_field, other_fields, customer_tail, flags);
	if(status != SUCCESS)
	{
		show_message("Out Of Paper!", WARNING, "ERROR!", OK);
		goto printer_error;
	}
	else
	{
		printer_feed(3);
		//show_message("Receipt printed.", INFORMATION, "UMOJAPAY", OK);
		
		disp_clear_viewport();
		disp_print_row(2, "Receipt Printed");
		
		disp_print_row(4, "Enter to Continue");
		disp_print_row(5, "F1 to print again");
		
		while(1)
		{
			key = kpd_getkey();
			
			if (key == KEY_CANCEL || key == KEY_ENTER) break;
			if (key == KEY_F1)
			{
				flags |= PRINTFLAG_MARK_DUPLICATE;
			 	goto reprint;
			}
		}
	}
	
	printer_close();
	disp_clear_viewport();
	
	return SUCCESS;
	
printer_error:;
	disp_clear_viewport();
	disp_print_center("Set paper and press a Key.", ARIEL16_18);
	kpd_getkey();
	goto reprint;
}

int enable_connection()
{
	disp_clear_viewport();
	disp_print_center("Connecting ...", ARIEL16_18);
	
	int connect = net_enable();		//Should we really enable this here????
	disp_clear_viewport();
	
	if(connect == FAILURE) goto connection_error;
	return SUCCESS;
	
connection_error:
	show_message("Connection Failed!!", FAILURE, "ERROR", OK);
	return FAILURE;
}

static void expand(char *s)
{
	s = _trim(s);
	int len = strlen(s);
	
//	printf("STR = \"%s\"\n", s);	
	if (len > 35) return;
	
	char *lastp = s;
	int nb_first = 0;
	
	for (int i = 0; i < len; ++i)
	{
		if (s[i] == ':')
		{
			lastp += i;
			nb_first = i;
			break;
		}
	}
	
	if (nb_first > 16 || nb_first < 1) return;
	
	char tmp[64];
	tmp[0] = 0;
	
	strncpy(tmp, s, nb_first);
	tmp[nb_first] = 0;
	
	for (int i = 0; i < (16-nb_first); ++i)
		strcat(tmp, " ");
		
	strcat(tmp, lastp);
	
//	printf("STR2= \"%s\"\n", tmp);	
	strcpy(s, tmp);
}

char *align_printer_string(char *string)
{
	int lines_count = 0;
	int line_len = 0;
	
	char *s = string;
	
	while (*s)
	{
		char c = *s++;
		if (c == '\n' || c >= 48)
		{
			lines_count ++;
			line_len = 0;
		}
		else line_len ++;
	};
	
	lines_count ++;
	
	int max_sz = (48 * lines_count);
	int use_sz = strlen(string) * 2;
	if (use_sz < max_sz) use_sz = max_sz;
	
	char *result = (char *)malloc(use_sz);
	result[0] = 0;
	
	char *out = (char *)malloc(1024);
	char *buff = string;
	
	while(buff)
	{
		buff = get_next_line_ex(buff, out);
		expand(out);
		strcat(result, out);
		strcat(result, "\n");
	}
	
	free(out);
	return result;
}

//====================================================================================================================================================
int do_paperless_transaction(const char *param_string, const char *url_path, url_data_t *out)
{
	if(enable_connection() == FAILURE) return 0;
	
	char buf[128];
	
repeat_post:;

	disp_clear_viewport();
	disp_print_center("Processing ...", ARIEL16_18);

	int err = do_curl_post(url_path, param_string, out);
	
	if(err == FAILURE) goto transaction_error;
	if(out->size == 0) goto transaction_error;
	//TODO: Confirm success by checking values:
	
	char *response = (char *)out->data;
	
	printf("SERVER_RESPONSE:\n%s", response);
	printf("\n");
	if(response[0] == '<') goto server_error;
	
	char *statuscode = response;
	while(1)
	{
		statuscode = strstr(statuscode, "status");
		if(statuscode)
		{
			char err_no = statuscode[9];		//skip =>
			if(err_no == '-' || err_no > '1') goto bad_status;
			
			statuscode += 4;	//Skip ahead to prevent hanging.		
		}
		else break;
	}
	
	char *fault_str = strcasestr(response, "faultString");
	if(fault_str) goto faultstr_error;
	
	fault_str = strcasestr(response, "error");	
	if (fault_str) goto bad_payload;
	
	//net_disable();
	return 1;
	
transaction_error:;
	SAFE_DELETE(out->data);
	err = show_message("Connection Error. Retry?", CHOICE, "RETRY?", YES_NO);
	if(err == FAILURE) return 0;
	goto repeat_post;
	
server_error:;
	SAFE_DELETE(out->data);
	err = show_message("Server returned error. Retry?", CHOICE, "RETRY?", YES_NO);
	if(err == FAILURE) return 0;
	goto repeat_post;
	
faultstr_error:;
	char *invalid_number = strcasestr(response, "exceptions.ValueError");
	if (invalid_number == NULL) goto server_error;
	
	disp_clear_viewport();
	disp_print_row(1, "An Error Occurred.");
	disp_print_row(4, "Press A Key.");
	
	kpd_getkey();

	disp_clear_viewport();
	disp_print_row(1, "INVALID NUMBER DETECTED!");
	disp_print_row(5, "Press A Key");
		
	kpd_getkey();
	
	SAFE_DELETE(out->data);
	err = show_message("Do you want to retry?", CHOICE, "RETRY?", YES_NO);
	if(err == FAILURE) return 0;
	
	goto repeat_post;

bad_payload:;
	disp_clear_viewport();
	disp_print_row(1, "An Error Occurred.");
	disp_print_row(4, "Press A Key.");
	
	kpd_getkey();
	
	prepare_printer_string(response);	
	char *edesc = strstr(response, "ERROR");
	if(edesc)
	{
		edesc += 8;							//Skip all the junk and get to the message.
		get_next_line(edesc, buf);
			
		disp_clear_viewport();
		disp_print_row(0, "ERROR DESCRIPTION:");
		//disp_set_cursor(0, 2);
		char *emsg = _trim(buf);
		if (strlen(emsg) > 84) emsg[84] = 0;
		
		disp_print_row(1, emsg);
		disp_print_row(5, "Press A Key");
		
		kpd_getkey();
	}
	
	SAFE_DELETE(out->data);	
	err = show_message("Do you want to retry?", CHOICE, "RETRY?", YES_NO);
	if(err == FAILURE) return 0;
	
	goto repeat_post;
		
bad_status:;
	/*	If we get here, the server responded but with an error message instead of success.
		We display the error message and then ask the user if they still want to try the transaction
		again. If a description was not given, it would be wiser to just cancel, but an option to
		re-send the info incase of server hang-downs is also given.
	*/
	
	disp_clear_viewport();
	disp_print_row(1, "Transaction Failed.");
	disp_print_row(4, "Press A Key.");
	
	kpd_getkey();
	
	prepare_printer_string(response);
	
	//Find DESCRIPTION FIELD...
	//@NOTE: Order is important here!	
	char *desc = strstr(response, "DESCRIPTION :");
	if (!desc) desc = strstr(response, "RESP :");
	if (!desc) desc = strstr(response, "DESCRIPTION: ");
	if (!desc) desc = strstr(response, "RESP:");
	if (!desc) desc = strstr(response, "DESCRIPTION");
	
	if(desc)
	{
		//Find field delimiter and skip leading spaces...
		while (*desc && *desc != ':' && *desc != '\n') desc ++;
		while (*desc == 32 || *desc == ':') desc ++;
		
		get_next_line(desc, buf);
			
		disp_clear_viewport();
		disp_print_row(0, "ERROR DESCRIPTION:");
		//disp_set_cursor(0, 2);
		char *emsg = _trim(buf);
		if (strlen(emsg) > 84) emsg[84] = 0;
		
		disp_print_row(1, emsg);
		disp_print_row(5, "Press A Key");
		
		kpd_getkey();
	}
	else
	{
		SAFE_DELETE(out->data);
		err = show_message("No description was received. Retry?", CHOICE, "RETRY SENDING?", YES_NO);
		if(err == FAILURE) return 0;
		goto repeat_post;
	}
	
	SAFE_DELETE(out->data);
	err = show_message("Do you want to retry?", CHOICE, "RETRY?", YES_NO);
	if(err == FAILURE) return 0;
	
	goto repeat_post;
}

int do_transaction(const char *param_string, const char *url_path, const char *success_message, int print_agent_copy, int flags)
{
	url_data_t ret;
	int success = do_paperless_transaction(param_string, url_path, &ret);
	
	if (success)
	{
		print_result(&ret, success_message, print_agent_copy, flags);
		free(ret.data);	
	}
	
	return success;
}

int do_filtered_transaction(const char *param_string, const char *url_path, const char *success_message, int print_agent_copy, const char **filters, int flags)
{
	url_data_t ret;
	int success = do_paperless_transaction(param_string, url_path, &ret);
	
	if (success)
	{
		char *response = (char *)ret.data;
		
		prepare_printer_string(response);
		int olen = strlen(response);
		
		char *output = (char *)malloc(olen+4);
		output[0] = 0;
		
		char buf[64];
		
		char **flist = filters;
		while(flist[0])
		{
			char *this_filter = flist[0];
			flist ++;
			
			char *string_begin = strstr(response, this_filter);
			if (!string_begin) continue;
			
			get_next_line(string_begin, buf);
			strcat(output, buf);
			strcat(output, "\n");
		}
		
		url_data_t fake;
		fake.data = output;
		fake.size = strlen(output);
		
		print_result(&fake, success_message, print_agent_copy, flags);
		
		free(output);
		free(ret.data);	
	}

	return success;
}

int field_exists(char *line, const char **filters)
{
	if (!filters) return FALSE;
	
	char **filter = filters;
	while(filter[0])
	{
		char *f = filter[0];
		if (strstr(line, f)) return TRUE;
		
		filter++;
	}
	
	return FALSE;
}

int do_formatted_rct_transaction(const char *param_string, const char *url_path, const char *success_message,
								 const char **leader_fields, const char **info_fields, const char **id_field, int print_agent_copy)
{
	url_data_t ret;
	int success = do_paperless_transaction(param_string, url_path, &ret);
#if 0	
	char *stuff = "date => 2014-05-10, receipt no => 12345678, car number => KAA123X, phone no => 254712474074, amount => 100, service => parking fees/lorry";
	ret.data = (char *)malloc(256);
	strcpy((char *)ret.data, stuff);
#endif
	if (success)
	{
		char *response = (char *)ret.data;
		
		prepare_printer_string(response);
		int olen = strlen(response);
		
/**/	
		char *leader = (char *)malloc(olen+4);
		char *info = (char *)malloc(olen+4);
		char *idfield = (char *)malloc(64);
		char *otherdata = (char *)malloc(olen+4);
		
		leader[0] = 0;
		info[0] = 0;
		idfield[0] = 0;
		otherdata[0] = 0;
		
		char buf[128];
		
		printf("generating leaders...\n");
				
		char **flist = leader_fields;
		while(flist[0])
		{
			char *this_filter = flist[0];
			flist ++;
			
			char *string_begin = strstr(response, this_filter);
			if (!string_begin) continue;
			
			get_next_line(string_begin, buf);
			strcat(leader, buf);
			strcat(leader, "\n");
			
			printf("adding %s\n", buf);
		}
		
		printf("generating info block...\n");
		
		flist = info_fields;
		while(flist[0])
		{
			char *this_filter = flist[0];
			flist ++;
			
			char *string_begin = strstr(response, this_filter);
			if (!string_begin) continue;
			
			get_next_line(string_begin, buf);
			strcat(info, buf);
			//strcat(info, "\n");
			
			printf("adding %s\n", buf);
		}
		
		printf("Extracting ID field...\n");
		
		char *this_filter = id_field[0];
		printf ("Value of id_field[0] = 0x%X\n", id_field[0]);
		printf ("Id Field is: %s\n", this_filter);
			
		char *string_begin = strstr(response, this_filter);
		
		if (string_begin)
		{
			get_next_line(string_begin, buf);
			strcat(idfield, buf);
			//strcat(idfield, "\n");
			
			printf("adding %s\n", buf);
		}
		
		printf("Compiling other data...\n");
		char *lastline = response;
		
		while (lastline)
		{
			lastline = copy_string_until(lastline, buf, '\n');
			if (field_exists(buf, leader_fields)) continue;
			if (field_exists(buf, info_fields)) continue;
			if (field_exists(buf, id_field)) continue;
			
			strcat(otherdata, buf);
			strcat(otherdata, "\n");
			
			printf("adding %s\n", buf);
		}
		
		printf("Finished!!\n");
		printf("\n");

//int print_result_ex(char *header_message, const char *header_fields, const char *info_fields, const char *id_field,
//				 	const char *other_fields, int print_double)
		
		char *header_block = align_printer_string(leader);
		char *info_block = align_printer_string(info);
		char *id_block = align_printer_string(idfield);
		char *other_block = align_printer_string(otherdata);
		
		free(leader);
		free(info);
		free(idfield);
		free(otherdata);
			
		print_result_ex(success_message, header_block, info_block, id_block, other_block, print_agent_copy);
		
		free(header_block);
		free(info_block);
		free(id_block);
		free(other_block);
		
		free(ret.data);	
	}

	return success;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------
//	MAGSTRIPE_READER
//-------------------------------------------------------------------------------------------------------------------------------------------------------

char *GetNextField(const char *in, char *out)
{
	if(!in) return NULL;
	//Replace characters until either EndOfString or '^' is reached.
	char *s = in;
	char *o = out;
	
	while(*s && *s != '^')
	{
		char str[2] = {*s, 0};
		*o++ = *s++;
	}
		
	*o = 0;
	
	if(*s) s++;
	return s;
}

void LoadMagStripeData1(const char *data, MAGTRACK1_HEADER *mtrack)
{
	char *dt = (char *)data;
	char buffer[32];
	int is_mastercard = 0;
	
	memset(mtrack, 0, sizeof(MAGTRACK1_HEADER));
	
	mtrack->sentinel = data[0];
	mtrack->FormatCode = data[1];
	
	if(mtrack->sentinel != '%' && mtrack->FormatCode != 'B')
	{
		printf("Unknown magcard format!!!\n");
		return;
	}
	
	dt = GetNextField(dt+2, mtrack->PAN_STRING);
	if(mtrack->PAN_STRING[0] == '5' && mtrack->PAN_STRING[1] == '9') is_mastercard = 1;
	
	printf("IsMASTERCARD(%d)\n", is_mastercard);
	//======================================================================================
	
	if(is_mastercard) dt = GetNextField(dt, mtrack->country_code);
	dt = GetNextField(dt, mtrack->Name);
	
	memcpy(mtrack->exp_date, dt, 4);
	//dt += 4;
	//memcpy(mtrack->svc_code, dt, 3);
	
	//Rest of the fields are useless.
}

//=======================================================================================================================================================
//=============================PREPARE HEX BITMAP FOR PRINT/DISPLAY======================================================================================

 unsigned char *prep_bitmap(unsigned char *src, int h, int dilation)
 {
 	//Dilation factor scales the image vertically since pixels are rectangular on print head.
 	//KISII LOGO
 	
 	if (dilation < 1) dilation = 1;
 	if (dilation > 4) dilation = 4;
 	
 	unsigned char *outdata = (unsigned char *)malloc(48*h*dilation);
 	if (dilation == 1)
 	{
 		memcpy(outdata, src, 48*h);
 		return outdata;
 	}
 	
 	unsigned char *src_ptr = src;
 	unsigned char *dst_ptr = outdata;
 	
 	for (int row = 0; row < h; ++row)
 	{
 		//printf ("Block copy, 0x%X ==> 0x%X\n", src_ptr, dst_ptr);
 		memcpy(dst_ptr, src_ptr, 48);
 		
  		int rem = dilation;
 		while(rem-- > 1)
 		{
 			dst_ptr += 48;
 			
 			//printf ("Dilation, 0x%X ==> 0x%X\n", src_ptr, dst_ptr);			
 			memcpy(dst_ptr, src_ptr, 48);
 		}
 		
 		src_ptr += 48;
 		dst_ptr += 48;
 	}
 	
 	return outdata;
 
 #if 0
 	//First we decode the image...
 	unsigned char *buffer = src;
 	
 	//Then pack the decoded bmp into bits. Printer understands 1-bit images only.
 	//Dilation factor scales the image vertically since pixels are rectangular on print head.
 	
 	unsigned char *bitmapdata = (unsigned char *)malloc(48 * h * dilation);
 	unsigned char *currentline = bitmapdata;
	int linelength = (48*dilation);
 	
 	for (int row = 0; row < h; ++row)
 	{
 		memset(currentline, 0, linelength);
 		unsigned char *current_px = &buffer[(row * bpp * gimp_image.width)];
 		
 		for (int col = 0; col < 384; ++col)
 		{
 			if (col == gimp_image.width) break;
 			
 			if (*current_px < 128)
 			{
 				int current_byte = (col >> 3);		//Divide by 8
 				int current_bit =  7-(col & 7);		//Get modulus col % 8
 				
 				uint8_t this_mask = (1 << current_bit);
 				currentline[current_byte] |= this_mask;
 			}
 			
 			current_px += bpp;
 		}
 		
 		if (dilation > 1)
 		{
 			int rem = dilation;
 			int lastoffset = 48;
 			
 			while(rem-- > 1)
 			{
 				memcpy(currentline+lastoffset, currentline, 48);
 				lastoffset += 48;
 			}
 		}
 				
 		currentline += linelength;
 	}
 	
 	free(buffer);
 	return bitmapdata;
 #endif
 }
