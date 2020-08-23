 #include "common.h"

static const int lst[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
static const char *names[] = {"ZUKU", "KPLC", "STAR TIMES", "GO TV", "WATER"};
static const char *payment_types[] = {"Post-Paid", "Pre-Paid"};

static char string_buf[128];		//General purpose string buffer.
static bill_t* __bill = NULL;
static int selected_service = -1;

//====================================================================================================================================================
static void finalize_transaction(const char **arg_ptr)
{
	const char *svc_name = arg_ptr[0];
	//const char *item_name = arg_ptr[1];
	
	char sbuf[32];
	
	int success = FALSE;
	int retry = FALSE;
	
	bill_t *my_bill = __bill;
	if(!__bill) printf("ERROR: __bill was not initialized!!\n");
	
	while(1)
	{
		if(!get_business_no(my_bill)) goto confirm_retry;
		if(!get_amount(my_bill)) goto confirm_retry;
		
		//All went OK. No cancelations. Print summary and ask for confirmation.
		disp_clear_viewport();
		int line = 1;
		disp_print_row(line++, "Transaction Details:");
		
		sprintf(sbuf, "Service: %s", svc_name);
		disp_print_row(line++, sbuf);
				
		sprintf(sbuf, "Acc No: %s", my_bill->business_no);
		disp_print_row(line++, sbuf);
		
		sprintf(sbuf, "Amount: %d", my_bill->amt);
		disp_print_row(line++, sbuf);
		
		kpd_getkey();
		
		int proceed = show_message("Confirm Transaction?", CHOICE, "Proceed?", OK_CANCEL);
		if(proceed != SUCCESS) goto confirm_retry;
		
		break;
			
confirm_retry:;
		retry = show_message("User canceled or input error. Retry?", CHOICE, "RETRY?", OK_CANCEL);
		if(retry == FAILURE)
		{
			disp_clear_viewport();
		 	goto exeunt;		//Ignore entire transaction
		}
	}
	
//transact:
	disp_clear_viewport();
	disp_print_center("Performing transaction ...", ARIEL16_18);
		
	char tr_buf[48];
	sprintf(tr_buf, /*"service=%s&*/"acc_no=%s&amt=%d&check=%d", /*svc_name,*/ my_bill->business_no, my_bill->amt, *(int *)((void *)arg_ptr[1]));
	
	printf("POST:\n%s", tr_buf);
	printf("\n");
	
	success = do_transaction(tr_buf, "https://www.umojapay.com:2443/pay/pos.php", "         PAYMENT SUCCESSFUL\n", TRUE, PRINTFLAG_DEFAULT);
	
exeunt:;
	//return success;
}

static void select_method(void *p)
{
	int sl = *(int *)p;
	
	const char *svc_name = names[selected_service];
	//const char *pay_type = payment_types[sl];
	
	const char *args[] = {svc_name, p};
	
	__bill = (bill_t *)malloc(sizeof(bill_t));
	finalize_transaction(args);
	
	free(__bill);
	__bill = NULL;
}

static void select_option(void *p)
{
	int sel = *(int *)p;
	selected_service = sel;
	
	const char *svc_name = names[sel];
	const char *args[] = {svc_name, NULL};
	
	if (!strcmp(svc_name, "KPLC"))
	{
		menu_entry_t menulist[] = { {"PRE-PAID", 1, 1, select_method, &lst[1]},
									{"POST-PAID", 1, 2, select_method, &lst[2]}
								  };
								  
 		select_settings_t SelectSettings;	
		SelectSettings.bSelectOption=1;
		SelectSettings.bModeSelect=0;
  		show_menu("Select Type", 0, (void *)menulist, 2, 0, (void *)&SelectSettings); 	
		
		return;
	}
	
	__bill = (bill_t *)malloc(sizeof(bill_t));
	finalize_transaction(args);
	
	free(__bill);
	__bill = NULL;
}

void pay_bill_entry()
{	
	char *header = "  UMOJAPAY SERVICES  \n    CASH SERVICES   \n";
	set_print_header(header);
	
	selected_service = -1;	
		
 	menu_entry_t menulist[] = {/*	{"ZUKU", 1, 1, select_option, lst}, */
 								{"KPLC", 1, 1, select_option, &lst[1]}
 								/*{"STAR TIMES", 1, 3, select_option, &lst[2]},
 								{"GO TV", 1, 4, select_option, &lst[3]},
 								{"WATER", 1, 5, select_option, &lst[4]} */
  							};
 	
 	select_settings_t SelectSettings;	
	SelectSettings.bSelectOption=1;
	SelectSettings.bModeSelect=0;
  	show_menu("Pay Bill Menu", 0, (void *)menulist, 1, 0, (void *)&SelectSettings);
}
