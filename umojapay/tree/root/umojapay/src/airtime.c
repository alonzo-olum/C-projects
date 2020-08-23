 #include "common.h"

static char string_buf[128];

static int confirm_trx(char *acc_no, int amt)
{
	disp_clear_viewport();
	
	sprintf(string_buf, "NO: %s", acc_no);
	disp_print_row(1, string_buf);
	
	sprintf(string_buf, "AMOUNT: %d", amt);
	disp_print_row(2, string_buf);
	
	disp_print_row(4, "Press Enter to Commit");
	disp_print_row(5, "Press Cancel to Exit");
	
	while(1)
	{
		int key = kpd_getkey();
		if (key == KEY_ENTER) return SUCCESS;
		if (key == KEY_CANCEL) return FAILURE;
	}
	
	return FAILURE;
}

static void do_airtime(char *operator)
{
	bill_t my_bill;
	
get_inputs:;

//	if(!get_acc_no(&my_bill)) goto confirm_retry;
	if(!get_amount(&my_bill)) goto confirm_retry;
		
	sprintf(string_buf, "amt=%d&operator=%s", my_bill.amt, operator);
	char response_header[128];
	
	sprintf(response_header, "            AIRTIME TOPUP\n\nOperator: %s:\nToken number is given below:", operator);					
	do_transaction(string_buf, "http://www.technilink.co.ke/umoja_pos/server_new.php", response_header, FALSE, PRINTFLAG_DEFAULT);
	return;
	
confirm_retry:;
	int retry = show_message("User canceled or input error. Retry?", CHOICE, "RETRY?", OK_CANCEL);
	if(retry == FAILURE)
	{
		disp_clear_viewport();
	 	return;		//Ignore entire transaction
	}
	
	goto get_inputs;
}

static void do_airtel_airtime()
{
	bill_t my_bill;
	char svc_buf[32];
	
get_inputs:;
	char __phone_no[16];
	int err = get_integer("Enter Phone No.", __phone_no, 12, FALSE, FALSE);
	if(!err) goto confirm_retry;
	
	if(strncmp(__phone_no, "254", 3))
	{
		if(__phone_no[0] != '0') goto confirm_retry;		
		char *strp = (char *)&__phone_no[1];

		strcpy(svc_buf, "254");
		strcat(svc_buf, strp);
		strcpy(__phone_no, svc_buf);	
	}

	if(!get_amount(&my_bill)) goto confirm_retry;	
	if (confirm_trx(__phone_no, my_bill.amt) == FAILURE) return;

	uint32_t trid = generate_transaction_id();	
	sprintf(string_buf, "auth=[username=checkout,password=um0j1p@ych3ck-ut]&account_no=%s&transaction_id=%d&amount=%d&f=airtelElectronicAirtime", __phone_no, trid, my_bill.amt);

	char *fields[] = {"TXNSTATUS", "TXNID", "DATE", "EXTREFNUM", NULL};
	do_filtered_transaction(string_buf, "https://www.umojapay.com/service/api/pos", "     AIRTEL AIRTIME TOP-UP\n\n", TRUE, fields, PRINTFLAG_DEFAULT);

	return;
	
confirm_retry:;
	int retry = show_message("User canceled or input error. Retry?", CHOICE, "RETRY?", OK_CANCEL);
	if(retry == FAILURE)
	{
		disp_clear_viewport();
	 	return;		//Ignore entire transaction
	}
	
	goto get_inputs;
}

void airtime_entry()
{
	char *header = "  UMOJAPAY SERVICES \n    CASH SERVICES   \n";
	set_print_header(header);
	
	char *list[] = {"Airtel", "Safaricom", "Orange", "YU"};
 	menu_entry_t menulist[] = {	{"Airtel", 1, 1, do_airtel_airtime, list[0]},
 								{"Safaricom", 1, 2, do_airtime, list[1]},
 								{"Orange", 1, 3, do_airtime, list[2]},
 								{"YU", 1, 4, do_airtime, list[3]}
  							  };
 	
 	select_settings_t SelectSettings;	
	SelectSettings.bSelectOption=1;
	SelectSettings.bModeSelect=0;
  	show_menu("Select Operator", 0, (void *)menulist, 4, 0, (void *)&SelectSettings);
}
