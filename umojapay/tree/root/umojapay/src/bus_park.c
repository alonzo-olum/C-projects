 #include "common.h"

static const int lst[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

static char string_buf[128];		//General purpose string buffer.
static bill_t* __bill = NULL;

//====================================================================================================================================================

static char url_buf[128];
static char *url_root = NULL;

static const char *url_roots[] = {
							"https://www.umojapay.com:3443/rcs/main",	//NAKURU
							"https://www.umojapay.com:3443/krcs/main"	//KISII
						   };

//====================================================================================================================================================
static char *gen_url(const char *function)
{
	strcpy(url_buf, url_root);
	strcat(url_buf, "/");
	strcat(url_buf, function);
	
	return url_buf;
}

void get_entrance_receipt(char *vehicle_class)
{
	char __car_no[10];
	char __phone_no[16];
	char buf[16];
	
	int dlg;	//Declare this here; I'm not sure how GCC will unwind the stack from the double-jump, so ...
	
repeat_inputs:;

	int stat = get_string("Enter Plate No", buf, 8, FALSE, FALSE);
	
	if (!stat) goto input_error;
	if (strlen(buf) < 5) goto input_error;	
	
	_ucase(buf);
	drop_spaces(buf, __car_no);
	
	stat = get_string("Enter Phone No", __phone_no, 14, FALSE, FALSE);
	if (!stat) goto input_error;
	
	if(strncmp(__phone_no, "254", 3))
	{
		if(__phone_no[0] != '0') goto input_error;
		
		char *strp = (char *)&__phone_no[1];
		char ph_tmp[16];

		strcpy(ph_tmp, "254");
		strcat(ph_tmp, strp);
		strcpy(__phone_no, ph_tmp);	
	}
	
	sprintf(string_buf, "phone_no=%s&car_no=%s&park=BusPark1&sn_no=%s&car_type=%s", __phone_no, __car_no, fetch_serial_no(), vehicle_class);
	do_transaction(string_buf, gen_url("bus_park_entry"), "       SUCCESSFULLY REGISTERED\n", FALSE, PRINTFLAG_DEFAULT);

	return;

input_error:;
	dlg = show_message("Invalid Plate No. Retry?", CHOICE, "RETRY?", YES_NO);
	if (dlg == SUCCESS) goto repeat_inputs;
}

void bill_entrance()
{
	char *vlist[] = {"Matatu", "Bus"};
	
	menu_entry_t menulist[] = {{"MATATU", 1, 1, get_entrance_receipt, vlist[0]},
								{"BUS", 1, 2, get_entrance_receipt, vlist[1]}
							  };

	select_settings_t SelectSettings;
	SelectSettings.bSelectOption=1;
	SelectSettings.bModeSelect=0;
  	show_menu("Select Vehicle", 0, (void *)menulist, 2, 0, (void *)&SelectSettings);
}

int bill_out_phase_one(char *rct_no)
{
	url_data_t result;
	sprintf(string_buf, "pay_no=%s&sn_no=%s", rct_no, fetch_serial_no());
	int success = do_paperless_transaction(string_buf, gen_url("bus_park_exit"), &result);
	
	if(!success) return 0;
	
	disp_clear_viewport();
	
	prepare_printer_string(result.data);
	char *CostStr = strstr(result.data, "COST");
	char *VehStr = strstr(result.data, "CAR NO");
	
	if(!CostStr || !VehStr)
	{
		free(result.data);
		return 0;
	}
	
	char buf[16];
	get_next_line(VehStr, buf);
	
	disp_print_row(1, buf);
	disp_print_row(2, CostStr);
	disp_print_row(4, "Press Enter To Pay");
 	disp_print_row(5, "Press Cancel To Exit");
	printf(result.data);
	
	free(result.data);
	
	while(1)
	{
		unsigned char key = kpd_getkey();
		
		if(key == KEY_CANCEL) return -1;
		if(key == KEY_ENTER) break;
	}
	
	return 1;
}

void bill_out_phase_two(char *rct_no)
{
	sprintf(string_buf, "pay_no=%s&sn_no=%s", rct_no, fetch_serial_no());
	do_transaction(string_buf, gen_url("update_bus_park_exit"), "      TRANSACTION SUCCESSFUL\n", TRUE, PRINTFLAG_DEFAULT);
}

void bill_exit()
{
	char rct_no[16];
	int dlg;

repeat_inputs:;

	int stat = get_integer("Enter Ticket Number", rct_no, 10, FALSE, FALSE);
	if(!stat) goto input_error;
	if(strlen(rct_no) < 8) goto input_error;
	
	stat = bill_out_phase_one(rct_no);
	if(!stat) goto input_error;
	
	if(stat == 1)
		bill_out_phase_two(rct_no);
	
	return;
	
input_error:;
	dlg = show_message("Invalid Receipt No. Retry?", CHOICE, "RETRY", YES_NO);
	if(dlg == SUCCESS) goto repeat_inputs;
}

void buspark_entry(int *county)
{	
	/* @NOTE: This section should actually start with a selection of the park the attendant is assigned.
		Functionality currently disabled. */
	
	url_root = url_roots[*county];
	menu_entry_t menulist[] = {	{"GENERATE TICKET", 1, 1, bill_entrance, NULL},
								{"GENERATE RECEIPT", 1, 2, bill_exit, NULL}
							  };
 	
 	select_settings_t SelectSettings;	
	SelectSettings.bSelectOption=1;
	SelectSettings.bModeSelect=0;
  	show_menu("Select Position", 0, (void *)menulist, 2, 0, (void *)&SelectSettings);
}
