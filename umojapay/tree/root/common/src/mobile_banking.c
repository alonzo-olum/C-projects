 #include "commonfuncs.h"
 #include <stdio.h>
 #include <stdlib.h>

char *mobile_services[] = {"MPESA", "YUCASH", "AIRTELMONEY", "ORANGEMONEY", "TANGAZA"};
static int selected_service = -1;

static void do_transaction(char *params)
{
	//gl14_do_curl_post();
}

static void do_withdrawal()
{
	//@TODO:
	//Get Amount
	//Get Pin
	//Do Transaction
	
	//char acc_string[32];
	char str[32];
	char acc_string[16];
	char pin_str[8];
	char amt_str[12];
	
	gl14_query_integer("Enter Account Number", acc_string, 14, 0);	
	gl14_query_integer("Enter PIN", pin_str, 8, 1);
	gl14_query_integer("Enter Amount", amt_str, 12, 0);
	
	sprintf(str, "Amount: Kshs. %s", amt_str);
	int res = gl14_do_question("Proceed ?", str);
	
	if(!res) return;
	
	//Else
	//Generate transaction string and POST it.
	//@PLACEHOLDER
	char *accstring = (char *)malloc(128);
	sprintf(accstring, "mob=%s&service=withdraw&accno=%s&pin=%s&amt=%s", mobile_services[selected_service], acc_string, pin_str, amt_str);
	
	do_transaction(accstring);
	free(accstring);
}

static void do_deposit()
{
	//@TODO:
	//Get Amount
	//Get Pin
	//Do Transaction
	
	//char acc_string[32];
	char str[32];
	char acc_string[16];
	char pin_str[8];
	char amt_str[12];
	
	gl14_query_integer("Enter Account Number", acc_string, 14, 0);	
	gl14_query_integer("Enter PIN", pin_str, 8, 1);
	gl14_query_integer("Enter Amount", amt_str, 12, 0);
	
	sprintf(str, "Amount: Kshs. %s", amt_str);
	int res = gl14_do_question("Proceed ?", str);
	
	if(!res) return;
	
	//Else
	//Generate transaction string and POST it.
	
	//@PLACEHOLDER
	char *accstring = (char *)malloc(128);
	sprintf(accstring, "mob=%s&service=deposit&accno=%s&pin=%s&amt=%s", mobile_services[selected_service], acc_string, pin_str, amt_str);
	
	do_transaction(accstring);
	free(accstring);
}

static void select_option(void *p)
{
	int selection = *(int *)p;
	//Select this bank from array and call transaction function.
	
	menu_entry_t menulist[] = { {"Withdraw", do_withdraw, NULL},
								{"Deposit", do_deposit, NULL}
							  };
							  
	gl14_do_menu("Select Option", menulist, 2);
}

void mobile_banking_entry()
{

	int test_array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
 	menu_entry_t menulist[] = {	{"MPESA", select_option, test_array},
 								{"YUCASH", select_option, &test_array[1]},
 								{"AIRTEL MONEY", select_option, &test_array[2]},
 								{"ORANGE MONEY", select_option, &test_array[3]},
 								{"TANGAZA", select_option, &test_array[4]}
 							};
 	
  	gl14_do_menu("Mobile Banking Menu", menulist, 5);

}
