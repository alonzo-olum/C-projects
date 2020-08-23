 #include "common.h"

static char string_buf[128];

static void do_withdrawal()
{
	bill_t my_bill;
	
get_inputs:;

	if(!get_acc_no(&my_bill)) goto confirm_retry;
	if(!get_amount(&my_bill)) goto confirm_retry;
	
	sprintf(string_buf, "amount=%d&collector_code=M001&auth=[username=checkout, password=um0j1p@ych3ck-ut]&account_no=%s&till_code=TL001&f=posWithdrawal&transaction_id=787838", my_bill.amt, my_bill.account_no);

	char *fields[] = {"ACCOUNT_NO", "AMOUNT", "DATE", "TRANSACTION_ID", "COLLECTOR_CODE", "STATUS", NULL};
	do_filtered_transaction(string_buf, "https://www.umojapay.com/service/api/pos", "         WITHDARAWAL SUCCESSFUL\n\n", TRUE, fields, PRINTFLAG_DEFAULT);

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

static void do_deposit()
{
	bill_t my_bill;
	
get_inputs:;

	if(!get_acc_no(&my_bill)) goto confirm_retry;
	if(!get_amount(&my_bill)) goto confirm_retry;
	
	sprintf(string_buf, "amount=%d&collector_code=M001&auth=[username=checkout, password=um0j1p@ych3ck-ut]&account_no=%s&till_code=TL001&f=topupUserAccount&transaction_id=787838", my_bill.amt, my_bill.account_no);
	
	char *fields[] = {"ACCOUNT_NO", "AMOUNT", "DATE", "TRANSACTION_ID", "COLLECTOR_CODE", "STATUS", NULL};
	do_filtered_transaction(string_buf, "https://www.umojapay.com/service/api/pos", "         DEPOSIT SUCCESSFUL\n\n", TRUE, fields, PRINTFLAG_DEFAULT);						
	
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

static void do_payments()
{
}

static void do_ministatement()
{
	bill_t my_bill;
	
get_inputs:;

	if(!get_acc_no(&my_bill)) goto confirm_retry;
	
	sprintf(string_buf, "auth=[username=checkout,password=um0j1p@ych3ck-ut]&account_no=%s&f=getUserAccountMiniStatement", my_bill.account_no);
							
	int opt = show_message("Would you like a print-out?", CHOICE, "GET PRINTOUT?", YES_NO);
	
	if(opt == SUCCESS)
		do_transaction(string_buf, "https://www.umojapay.com/service/api/pos", "         MINISTATEMENT REPORT\n", FALSE, PRINTFLAG_DEFAULT);
	else
	{
		//Do Paperless Transaction.
		//Scan for Last Transaction.
		//Display it.
		
		url_data_t result;
		int success = do_paperless_transaction(string_buf, "https://www.umojapay.com/service/api/pos", &result);
		
		if(success)
		{
			//Find String (AMOUNT).
			//STOP AT STRING (AMOUNT).
			char *org_string = (char *)result.data;
			prepare_printer_string(org_string);
			char *msg = strstr(org_string, "AMOUNT");
			
			if(!msg)
			{
				disp_clear_viewport();
				disp_print_row(2, "NO PREVIOUS TRANSACTIONS");
			
				kpd_getkey();
				return;
			}
			
			char *cnt = msg+4;
			char *end = strstr(cnt, "AMOUNT");
			if(end) *end = 0;
			
			printf(msg);
			
			int line_count;
			char **lines = str_tokenize(msg, &line_count, "\n");
			int line = 1;
			
			disp_clear_viewport();
			
			for(int i = 0; i < line_count; ++i)
			{
				disp_set_cursor(0, line);
				drop_spaces(lines[i], string_buf);
				disp_print(string_buf, ARIEL16_18);
				
				free(lines[i]);
				lines[i] = NULL;
				
				++line;
				
				if(line > 5)
				{
					for(int j = line-1; j < line_count; ++j)
						if(lines[j]) free(lines[j]);
						
				 	break;
				}
			}
			
			free(result.data);
			free(lines);		
			kpd_getkey();
		}
	}
	
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

static void do_balance()
{
	bill_t my_bill;
	
get_inputs:;

	if(!get_acc_no(&my_bill)) goto confirm_retry;
	
	sprintf(string_buf, "auth=[username=checkout,password=um0j1p@ych3ck-ut]&account_no=%s&f=getUserAccountBalance", my_bill.account_no);
							
		//Do Paperless Transaction.
		//Scan for Last Transaction.
		//Display it.
		
	url_data_t result;
	int success = do_paperless_transaction(string_buf, "https://www.umojapay.com/service/api/pos", &result);
		
	if(success)
	{
		//Find String (ACCOUNT_NO).
		char *msg = (char *)result.data;
		char sbuf[48];
		prepare_printer_string(msg);
			
		printf(msg);
			
		char *acc_start = strstr(msg, "ACCOUNT_NO");
		if(!acc_start) goto format_error;
		
		get_next_line(acc_start, string_buf);
		drop_spaces(string_buf, sbuf);
		
		printf("DropSpaces(%s) returned \"%s\"\n", string_buf, sbuf);
		
		disp_set_cursor(0, 1);
		disp_print(sbuf, ARIEL16_18);
		
		acc_start = strstr(msg, "AMOUNT");
		if(!acc_start) goto format_error;
		
		get_next_line(acc_start, string_buf);
		drop_spaces(string_buf, sbuf);
		
		printf("DropSpaces(%s) returned \"%s\"\n", string_buf, sbuf);
		
		disp_set_cursor(0, 3);
		disp_print(sbuf, ARIEL16_18);
		
		free(result.data);					
		kpd_getkey();
	}
	
	return;
	
confirm_retry:;
	int retry = show_message("User canceled or input error. Retry?", CHOICE, "RETRY?", OK_CANCEL);
	if(retry == FAILURE)
	{
		disp_clear_viewport();
	 	return;		//Ignore entire transaction
	}
	
	goto get_inputs;
	
format_error:;
	disp_clear_viewport();
	disp_print_center("Unknown response format.", ARIEL16_18);
	disp_print_row(4, "Press a Key");
	kpd_getkey();
}

static void do_signup()
{
}

void umojapay_entry()
{
	char *header = "  UMOJAPAY SERVICES  \n    CASH SERVICES   \n";
	set_print_header(header);
	
 	menu_entry_t menulist[] = {	{"CASH WITHDRAWAL", 1, 1, do_withdrawal, NULL},
 								{"DEPOSIT", 1, 2, do_deposit, NULL},
 								//{"PAYMENTS", 1, 3, do_payments, NULL},
 								{"MINISTATEMENT", 1, 3, do_ministatement, NULL},
 								{"CHECK BALANCE", 1, 4, do_balance, NULL}
 							  };
 	
 	select_settings_t SelectSettings;	
	SelectSettings.bSelectOption=1;
	SelectSettings.bModeSelect=0;
  	show_menu("Umojapay Menu", 0, (void *)menulist, 4, 0, (void *)&SelectSettings);
}
