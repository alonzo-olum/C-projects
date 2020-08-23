 #include "commonfuncs.h"
 #include <stdio.h>
 #include <stdlib.h>

void do_cardless_transaction()
{
	//@TODO:
	//Get Account No.
	//Get PIN
	//Get Amount
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
}

void do_card_transaction()
{
	//@TODO:
	//Swipe Card
	//Get Pin
	//Get Amount
	//Do Transaction
	
	char str[32];
	char acc_string[16];
	char pin_str[8];
	char amt_str[12];
	
	lk_dispclr();
	gl14_print_center("SWIPE CARD.");
	
	gl14_wait_for_key();
	
	//TODO: Get Magcard info and display it with key wait.
	
	gl14_query_integer("Enter PIN", pin_str, 8, 1);
	gl14_query_integer("Enter Amount", amt_str, 12, 0);
	
	sprintf(str, "Amount: Kshs. %s", amt_str);
	int res = gl14_do_question("Proceed ?", str);
	
	if(!res) return;
	
	//Else
	//Generate transaction string and POST it.
}

void bank_select_option(void *p)
{
	int selection = *(int *)p;
	//Select this bank from array and call transaction function.
	
	menu_entry_t menulist[] = { {"With Card", do_card_transaction, NULL},
								{"Cardless", do_cardless_transaction, NULL}
							  };
							  
	gl14_do_menu("Select Option", menulist, 2);
}

void agency_banking_entry()
{

	int test_array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
 	menu_entry_t menulist[] = {	{"EQUITY", bank_select_option, test_array},
 								{"DTB", bank_select_option, &test_array[1]},
 								{"KCB", bank_select_option, &test_array[2]},
 								{"COOP", bank_select_option, &test_array[3]},
 								{"FAMILY", bank_select_option, &test_array[4]},
 								{"STANCHART", bank_select_option, &test_array[5]},
 								{"BARCLAYS", bank_select_option, &test_array[6]},
 								{"TRANSNATIONAL", bank_select_option, &test_array[7]},
 								{"I&M", bank_select_option, &test_array[8]},
 								{"CFC STANBIK", bank_select_option, &test_array[9]},
 								{"GIRO BANK", bank_select_option, &test_array[10]},
 								{"ABC", bank_select_option, &test_array[11]},
 								{"BANK OF AFRICA", bank_select_option, &test_array[12]},
 								{"IMPERIAL BANK", bank_select_option, &test_array[13]},
 								{"BIASHARA BANK", bank_select_option, &test_array[14]},
 								{"CREDIT BANK", bank_select_option, &test_array[15]},
 								{"DUBAI BANK", bank_select_option, &test_array[16]},
 								{"PRIME BANK", bank_select_option, &test_array[17]}
 							};
 	
  	gl14_do_menu("Agency Banking Menu", menulist, 18);

}
