 #include "common.h"
 
void do_cardless_transaction()
{
	//@TODO:
	//Get Account No.
	//Get PIN
	//Get Amount
	//Do Transaction
}

void do_card_transaction()
{
	//@TODO:
	//Swipe Card
	//Get Pin
	//Get Amount
	//Do Transaction
	

	//Else
	//Generate transaction string and POST it.
}

void bank_select_option(void *p)
{
	int selection = *(int *)p;
	//Select this bank from array and call transaction function.
	
	menu_entry_t menulist[] = { {"With Card", 1, 1, do_card_transaction, NULL},
								{"Cardless", 1, 2, do_cardless_transaction, NULL}
							  };
							  
   	select_settings_t SelectSettings;	
	SelectSettings.bSelectOption=1;
	SelectSettings.bModeSelect=0;
  	show_menu("Select an option", 0, (void *)menulist, 2, 0, (void *)&SelectSettings);
}

void agency_banking_entry()
{

	int test_array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
 	menu_entry_t menulist[] = {	{"EQUITY", 1, 1, bank_select_option, test_array},
 								{"DTB", 1, 2, bank_select_option, &test_array[1]},
 								{"KCB", 1, 3, bank_select_option, &test_array[2]},
 								{"COOP", 1, 4, bank_select_option, &test_array[3]},
 								{"FAMILY", 1, 5, bank_select_option, &test_array[4]},
 								{"STANCHART", 1, 6, bank_select_option, &test_array[5]},
 								{"BARCLAYS", 1, 7, bank_select_option, &test_array[6]},
 								{"TRANSNATIONAL", 1, 8, bank_select_option, &test_array[7]},
 								{"I&M", 1, 9, bank_select_option, &test_array[8]},
 								{"CFC STANBIK", 1, 10, bank_select_option, &test_array[9]},
 								{"GIRO BANK", 1, 11, bank_select_option, &test_array[10]},
 								{"ABC", 1, 12, bank_select_option, &test_array[11]},
 								{"BANK OF AFRICA", 1, 13, bank_select_option, &test_array[12]},
 								{"IMPERIAL BANK", 1, 14, bank_select_option, &test_array[13]},
 								{"BIASHARA BANK", 1, 15, bank_select_option, &test_array[14]},
 								{"CREDIT BANK", 1, 16, bank_select_option, &test_array[15]},
 								{"DUBAI BANK", 1, 17, bank_select_option, &test_array[16]},
 								{"PRIME BANK", 1, 18, bank_select_option, &test_array[17]}
 							};
 	
   	select_settings_t SelectSettings;	
	SelectSettings.bSelectOption=1;
	SelectSettings.bModeSelect=0;
  	show_menu("Agency Banking Menu", 0, (void *)menulist, 18, 0, (void *)&SelectSettings);

}
