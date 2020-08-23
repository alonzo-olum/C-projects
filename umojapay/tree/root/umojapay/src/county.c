 #include "common.h"
 #include "kisii_logo.h"

static const int lst[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

static char string_buf[256];		//General purpose string buffer.
static char url_buf[128];
static bill_t* __bill = NULL;
static char *url_root = NULL;

static int curr_county = 0;

static char **menu_properties = NULL;

static char *print_headers[] = {
									"    NAKURU COUNTY   \n   PAYMENT SERVICES  \n", 
									"     KISII COUNTY   \n   PAYMENT SERVICES  \n"
							   };
							   
static const char *url_roots[] = {
									"https://www.umojapay.com:2443/rcs/main",	//NAKURU
									"https://www.umojapay.com:2443/krcs/main"	//KISII
						   		 };

//====================================================================================================================================================
static char *gen_url(const char *function)
{
	strcpy(url_buf, url_root);
	strcat(url_buf, "/");
	strcat(url_buf, function);
	
	return url_buf;
}

int confirm_fines(char *reg_no)
{
	disp_clear_viewport();
	disp_print_center("Checking ...", ARIEL16_18);
	
	char tmp[32];
	url_data_t result;
	sprintf(tmp, "car_no=%s&sn_no=%s", reg_no, fetch_serial_no());
	int success = do_paperless_transaction(tmp, gen_url("check_car_fines"), &result);
	
	disp_clear_viewport();
	if(!success) return FAILURE;
	
	prepare_printer_string(result.data);
	char *VehStr = strstr(result.data, "CAR NO");
	char *CostStr = strstr(result.data, "FINE");
	
	if(!CostStr || !VehStr)
	{
		free(result.data);
		return FAILURE;
	}
	
	char buf[16];
	get_next_line(CostStr, buf);
	
	disp_print_row(1, _trim(VehStr));
	disp_print_row(2, _trim(buf));
	disp_print_row(4, "Press Enter To Pay");
 	disp_print_row(5, "Press Cancel To Exit");
	
	printf(result.data);
	free(result.data);
	
	while(1)
	{
		unsigned char key = kpd_getkey();
		
		if(key == KEY_CANCEL) return FAILURE;
		if(key == KEY_ENTER) break;
	}
	
	return SUCCESS;
}

int confirm_payments(const char *svc_name, const char *identifier)
{
	disp_clear_viewport();
		
	disp_print_row(1, svc_name);
	disp_print_row(2, identifier);
	disp_print_row(4, "Press Enter To Pay");
 	disp_print_row(5, "Press Cancel To Exit");
		
	while(1)
	{
		unsigned char key = kpd_getkey();
		
		if(key == KEY_CANCEL) return FAILURE;
		if(key == KEY_ENTER) break;
	}
	
	return SUCCESS;
}

void instant_fines(int *p)
{
	int svc = *p;
	
	char __phone_no[16];		//Max 12 Characters.
	char __id_no[10];
	//char __amt[10];
	char __reg_no[10];			//Max 8 characters
	
	//string_buf;
	char svc_buf[32];
	string_buf[0] = 0;
	
	char tmps[32];
	int err = 0;
	
	svc <<= 1;
	char *svcname = menu_properties[svc];
	char *identifier = menu_properties[svc+1];
	
begin_inputs:;

	if(enable_connection() == FAILURE) return;
		
	err = get_integer("Enter Phone No.", __phone_no, 12, FALSE, FALSE);
	if(!err) goto input_error;
	
	if(strncmp(__phone_no, "254", 3))
	{
		if(__phone_no[0] != '0' && __phone_no[0] != '7') goto input_error;
		
		char *strp = (char *)&__phone_no[1];
		if (__phone_no[0] == '7') strp = &__phone_no[0];

		strcpy(svc_buf, "254");
		strcat(svc_buf, strp);
		strcpy(__phone_no, svc_buf);
	}
	
	if (strlen(__phone_no) != 12)
	{
		show_message("Invalid Phone No!", WARNING, "Invalid No.", OK);
		goto begin_inputs;
	}
	
	sprintf(svc_buf, "phone_no=%s&", __phone_no);
	strcat(string_buf, svc_buf);

	strcpy(tmps, svcname);
	sprintf(svc_buf, "service_header=%s", _trim_trim(tmps));
	strcat(string_buf, svc_buf);
	
	sprintf(tmps, "Enter %s", identifier);
	err = get_string(tmps, svc_buf, 16, FALSE, FALSE);
	
	if(!err) goto input_error;
	if(strlen(svc_buf) < 5) goto input_error;
		
	_ucase(svc_buf);
	drop_spaces(svc_buf, __reg_no);
	
	sprintf(svc_buf, "&car_no=%s", __reg_no);
	strcat(string_buf, svc_buf);
	
	if (confirm_fines(__reg_no) == FAILURE)
		return;
	
	strcat(string_buf, "&sn_no=");
	strcat(string_buf, fetch_serial_no());
	
repeat_post:;
	
	disp_clear_viewport();
	disp_print_center("Processing ...", ARIEL16_18);
	
	printf("POST: String buf:\n%s\n", string_buf);
	printf("\n");
	
	do_transaction(string_buf, gen_url("gen_service"), "      PAYMENT WAS SUCCESSFUL", FALSE, PRINTFLAG_DEFAULT);	
	return;
	
input_error:;
	err = show_message("Input Error. Retry?", CHOICE, "RETRY?", YES_NO);
	if(err == FAILURE) return;
	goto begin_inputs;
}

void instant_payments(int *p)
{	
	int svc = *p;
	
	char __phone_no[16];		//Max 12 Characters.
	char __id_no[10];
	char __amt[10];
	char __reg_no[10];			//Max 8 characters
	
	//string_buf;
	char svc_buf[32];
	string_buf[0] = 0;
	char tmps[32];
	int err = 0;
	
	svc <<= 1;
	char *svcname = menu_properties[svc];
	char *identifier = menu_properties[svc+1];
	
	if(strcasestr(svcname, "parking fines"))
	{
		instant_fines(p);
		return;
	}
	
begin_inputs:;

	if(enable_connection() == FAILURE) return;
	
	err = get_integer("Enter Phone No.", __phone_no, 12, FALSE, FALSE);
	if(!err)
	{
		if (show_message("Use the default number?", CHOICE, "Use Default?", YES_NO) == FAILURE) goto input_error;
		strcpy(__phone_no, "254788119813");
	}
	else if(strncmp(__phone_no, "254", 3))
	{
		if(__phone_no[0] != '0' && __phone_no[0] != '7') goto input_error;
		
		char *strp = (char *)&__phone_no[1];
		if (__phone_no[0] == '7') strp = &__phone_no[0];

		strcpy(svc_buf, "254");
		strcat(svc_buf, strp);
		strcpy(__phone_no, svc_buf);
	}
	
	if (strlen(__phone_no) != 12)
	{
		show_message("Invalid Phone No!", WARNING, "Invalid No.", OK);
		goto begin_inputs;
	}
	
	sprintf(svc_buf, "phone_no=%s&", __phone_no);
	strcat(string_buf, svc_buf);
	
	strcpy(tmps, svcname);
	sprintf(svc_buf, "service_header=%s", _trim_trim(tmps));
	strcat(string_buf, svc_buf);
	
	sprintf(tmps, "Enter %s", identifier);
	err = get_string(tmps, svc_buf, 16, FALSE, FALSE);
	
	if(!err) goto input_error;
	if(strlen(svc_buf) < 5) goto input_error;
		
	_ucase(svc_buf);
	drop_spaces(svc_buf, __reg_no);
	
	if (confirm_payments(svcname, __reg_no) == FAILURE) return;
	
	sprintf(svc_buf, "&car_no=%s", __reg_no);
	strcat(string_buf, svc_buf);
	
	int ilen = strlen(string_buf);
	char *foo = &string_buf[ilen];
	
	strcat(string_buf, "&sn_no=");
	strcat(string_buf, fetch_serial_no());

#if 0	
	disp_clear_viewport();
	disp_print_row(1, "MY SERIAL NO:");
	
	const char *foov = fetch_serial_no();
	sprintf(svc_buf, "SN: %s", foov);
	disp_print_row(2, svc_buf);
	
	ilen = strlen(svc_buf);
	int last_char = (int)svc_buf[ilen-1];
	sprintf(svc_buf, "LAST CHAR: %d, LEN: %d", last_char, ilen);
	disp_print_row(4, svc_buf);

	kpd_getkey();
		
	disp_clear_viewport();
	disp_print_row(0, foo);
	kpd_getkey();
#endif

repeat_post:;
	
	disp_clear_viewport();
	disp_print_center("Processing ...", ARIEL16_18);
	
	printf("POST: String buf:\n%s\n", string_buf);
	printf("\n");
	
	strcpy(svc_buf, identifier);
	_ucase(svc_buf);
	
	char *leaders[] = {"RECEIPT NO", "RECIEPT NO", "DATE", NULL};
	char *info[] = {"SERVICE", NULL};
	char *ident[] = {svc_buf, NULL};
	
	//do_transaction(string_buf, gen_url("gen_service"), "      PAYMENT WAS SUCCESSFUL", TRUE, PRINTFLAG_DEFAULT);	
	do_formatted_rct_transaction(string_buf, gen_url("gen_service"), "PAYMENT WAS SUCCESSFUL", leaders, info, ident, FALSE);
	return;
	
input_error:;
	err = show_message("Input Error. Retry?", CHOICE, "RETRY?", YES_NO);
	if(err == FAILURE) return;
	goto begin_inputs;
}

char **fetch_county_services(int *count, char *type)
{
	*count = 0;
	if(enable_connection() == FAILURE)
		return NULL;
	
get_info:;

	url_data_t url_data;
	
//	disp_clear_viewport();
//	disp_print_center("Fetching Items ...", ARIEL16_18);
	
	sprintf(string_buf, "type=%s", type);
	int ret = do_paperless_transaction(string_buf, gen_url("dynamic_menu"), &url_data);
	
	if(ret == FALSE)
		return NULL;
		
	char *resp = (char *)url_data.data;
	int len = strlen(resp);	
	
	for (int i = len; i > 0; --i)
	{
		if (resp[i] == 0) continue;
		if (resp[i] >= 'A' && resp[i] <= 'z') break;
		
		if (resp[i] == ';')
		{
			resp[i] = 32;
			break;
		}
	}
	
	char **names = tokenize((char *)url_data.data, count);
	free(url_data.data);
	return names;
}

void do_auto_menu()
{
	int entry_count = 0;
	char **entrynames= fetch_county_services(&entry_count, "");
	
	if (!entrynames) return;
	if (entry_count & 1)
	{
		disp_clear_viewport();
		disp_print_center("Broken List Detected!", ARIEL16_18);
		disp_print_row(4, "Contact Admin");
		
		kpd_getkey();
		return;
	}
	
	int menu_count = (entry_count >> 1);
	menu_entry_t *menulist = (menu_entry_t *)malloc(sizeof(menu_entry_t) * menu_count);
	
	int ct = 0;
	char tmps[32];
	
	for(int i = 0; i < entry_count; i+=2)
	{
		menu_entry_t *entry = &menulist[ct++];
		
		char *title = _trim(entrynames[i]);
		int ln = strlen(title);
		if (ln < 16)
		{
			strcpy(tmps, title);
			_ucase(tmps);
		}
		else
		{
			strncpy(tmps, title, 16);
			tmps[17] = 0;
			_ucase(tmps);
		}
				
		strcpy(entry->display_name, tmps);
		entry->level = 1;
		entry->elem = ct;
		entry->callback = instant_payments;
		entry->arg_ptr = (void *)&lst[i>>1];
	}
	 		
	select_settings_t select_settings;
	select_settings.bSelectOption = 1;
	select_settings.bModeSelect = 0;
	
	menu_properties = entrynames;		
	show_menu("County Services", 0, (void *)menulist, menu_count, 0, (void *)&select_settings);
			
	for(int i = 0; i < entry_count; ++i)
		free(entrynames[i]);
	
	free(entrynames);		
	free(menulist);
	
	menu_properties = NULL;
}

void do_xreport()
{
	//if(authenticate_owner() != SUCCESS) return;
	//@NOTE: Authorization changed to allow Supervisors Only!!
	
	char npin[8];
 	if(!get_integer("Enter Supervisor PIN", npin, 4, 1, FALSE)) return;
 	
 	if(strcmp(npin, "4224"))
 	{
 		disp_clear_viewport();
 		disp_print_center("Invalid PIN", ARIEL16_18);
 		sleep(1);
 	 	return;
 	}
	
	disp_clear_viewport();
	disp_print_center("Processing ...", ARIEL16_18);
	
	sprintf(string_buf, "sn_no=%s", fetch_serial_no());
//	printf("POST: String buf:\n%s\n", string_buf);
//	printf("\n");
	
	do_transaction(string_buf, gen_url("x_report"), "       SALES SUMMARY REPORT", FALSE, PRINTFLAG_DEFAULT|PRINTFLAG_INCLUDE_SIGN);	
	return;
}

void county_services_entry()
{
	menu_entry_t menulist[] = {	{"Services Menu", 1, 1, do_auto_menu, NULL},
								{"Get Sales Report", 1, 2, do_xreport, NULL}
							  };
 	
 	select_settings_t SelectSettings;	
	SelectSettings.bSelectOption=1;
	SelectSettings.bModeSelect=0;
  	show_menu("Select Option", 0, (void *)menulist, 2, 0, (void *)&SelectSettings); /**/
}

void kisii_county_entry(int *ptr)
{
//	char *header_str = print_headers[idx];
//	set_print_header(header_str);
	
	url_root = url_roots[1];
	curr_county = 1;
	
 	unsigned char *print_logo = NULL;
 	print_logo = prep_bitmap(kisii_logo, kisii_image_height, 2);

	set_print_logo(print_logo, 48*kisii_image_height*2);
	set_print_header("");
	
#if 0	
	//Select to go for subscribed or general services.
	menu_entry_t menulist[] = {	{"SUBSCRIBED SERVICES", 1, 1, county_menu_entry, NULL},
								{"GENERAL SERVICES", 1, 2, county_services_entry, NULL}
							  };
 	
 	select_settings_t SelectSettings;	
	SelectSettings.bSelectOption=1;
	SelectSettings.bModeSelect=0;
  	show_menu("County Menu", 0, (void *)menulist, 2, 0, (void *)&SelectSettings);
#else
	county_services_entry();
	
	set_print_logo(NULL, 0);
	free(print_logo);
#endif
}
#if 0
void do_dummy()
{
	char *leaders[] = {"RECEIPT NO", "DATE", NULL};
	char *info[] = {"SERVICE", NULL};
	char *ident[] = {"CAR NUMBER", NULL};
	
	char *header_str = print_headers[1];
	set_print_header(header_str);
	
	do_formatted_rct_transaction("", "", "SUCCESSFUL PAYMENT",
								 leaders, info, ident, 
								 FALSE);
}
#endif
void county_entry()
{	
	int list[] = {0, 1, 2, 3};	
	menu_entry_t menulist[] = {/*	{"NAKURU COUNTY", 1, 1, select_paybill_type, list}, */
								{"KISII COUNTY", 1, 1, kisii_county_entry, &list[1]}
								//{"DUMMY TRANSACTION", 1, 1, do_dummy, NULL}
							  };
 	
 	select_settings_t SelectSettings;
	SelectSettings.bSelectOption=1;
	SelectSettings.bModeSelect=0;
  	show_menu("Select County", 0, (void *)menulist, 1, 0, (void *)&SelectSettings);
}
