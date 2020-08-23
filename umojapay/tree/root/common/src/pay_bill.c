 #include "global.h"
 #include "commonfuncs.h"
 #include <stdio.h>
 #include <stdlib.h>
 
 #include "platform.h"

static const char *providers[] = {"ZUKU", "KPLC", "STAR_TIMES", "GOTV", "WATER"};
static const char *mobile_services[] = {"MPESA", "YUCASH", "AIRTELMONEY", "ORANGEMONEY", "TANGAZA"};
static const char *pay_methods[] = {"pre-paid", "post-paid"};
static const char *pay_accounts[] = {"mobile", "umojapay"};

static int selected_provider=-1;
static int selected_service = -1;
static int pay_method = -1;
static int pay_account = -1;

static const char *url_path = "http://technilink.co.ke/umoja_pos/server.php";

static void do_transaction()
{
	if(selected_service < 0 ||
		pay_method < 0 ||
		pay_account < 0 ||
		selected_provider < 0)
	{
		disp_clear_viewport;
		disp_print_center("Configuration Error!", 0);
		return;
	}
	
	char acc_string[16];
	char pin_string[16];
	char amt_string[16];
	
	int success = gl14_query_string((pay_account)? "Enter Account Number": "Enter Phone Number", acc_string, 16, FALSE); if(!success) goto fail;
	success = gl14_query_string("Enter PIN", pin_string, 16, FALSE); if(!success) goto fail;
	success = gl14_query_string("Enter Amount", amt_string, 16, FALSE); if(!success) goto fail;
	
	char sbuf[128];
	sprintf(sbuf, "provider=%s&service=%s&method=%s&acc_type=%s&acc_no=%s&pin=%s&amt=%s",
					providers[selected_provider],
					mobile_services[selected_service],
					pay_methods[pay_method],
					pay_accounts[pay_account],
					acc_string,
					pin_string,
					amt_string);
	
	url_data_t result;				
	gl14_do_curl_post(url_path, sbuf, &result);
	return;
	
fail:
	
	disp_clear_viewport();
	disp_print_center("An Error Occurred!", 0);
}

static void select_payment_method(void *p)
{
	pay_method = *(int *)p;
}

static void select_account(void *p)
{
	pay_account = *(int *)p;
}

static void select_service(void *p)
{
	selected_service = *(int *)p;
}

static void select_option(void *p)
{
	selected_provider = *(int *)p;
	//Select this bank from array and call transaction function.
	
	int lst[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
	
	menu_entry_t svc_menu[] = {				
								{"MPESA", 1, 1, select_service, lst},
 								{"YUCASH", 1, 1, select_service, &lst[1]},
 								{"AIRTEL MONEY", 1, 1, select_service, &lst[2]},
 								{"ORANGE MONEY", 1, 1, select_service, &lst[3]},
 								{"TANGAZA", 1, 1, select_service, &lst[4]}
 							};
 							
 	gl14_do_menu("Select Option", svc_menu, 5);
 							
	menu_entry_t menulist[] = { 				{"Pre-paid", 1, 1, select_payment_method, lst},
								{"Post-paid", 1, 1, select_payment_method, &lst[1]}
							  };
							  
	gl14_do_menu("Select Option", menulist, 2);
	
	menu_entry_t accmenu[] = { {"Phone no.", 1, 1, select_account, lst},
				   {"Account no.", 1, 1, select_account, &lst[1]}
							 };
							  
	gl14_do_menu("Select Option", accmenu, 2);
	do_transaction();
}

void pay_bill_entry()
{
	selected_provider = -1;
	selected_service = -1;
	pay_method = -1;
	pay_account = -1;

	int test_array[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
 	menu_entry_t menulist[] = {	{"ZUKU", 1, 1, select_option, test_array},
 								{"KPLC", 1, 1, select_option, &test_array[1]},
 								{"STAR TIMES", 1, 1, select_option, &test_array[2]},
 								{"GO TV", 1, 1, select_option, &test_array[3]},
 								{"WATER", 1, 1, select_option, &test_array[4]}
 							};
 	
  	gl14_do_menu("Pay Bill Menu", menulist, 5);
}
