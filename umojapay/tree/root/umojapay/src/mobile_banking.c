 #include "common.h"
 #include <time.h>

char *mobile_services[] = {"MPESA", "YUCASH", "AIRTELMONEY", "ORANGEMONEY", "TANGAZA"};
static int selected_service = -1;
static char string_buf[128];

static void do_airtel_withdrawal()
{
	//@TODO:
	//Get Amount
	//Get Pin
	//Do Transaction
	
	bill_t my_bill;
	char buf[32];
	int ret;
	
get_inputs:;
	
	if(!get_phone_no(&my_bill)) goto confirm_retry;
	if(!get_amount(&my_bill)) goto confirm_retry;
	
	if(strncmp(my_bill.phone_no, "254", 3))
	{
		if(my_bill.phone_no[0] != '0') goto confirm_retry;
		
		char *strp = (char *)&my_bill.phone_no[1];

		strcpy(buf, "254");
		strcat(buf, strp);
		strcpy(my_bill.phone_no, buf);	
	}
	
get_authcode:;
	
	if(!(ret = get_string("Enter Auth Code", my_bill.account_no, 6, FALSE, FALSE))) goto confirm_retry;
	if (ret != 6)
	{
		ret = show_message("Invalid Auth Code Length! Retry?", CHOICE, "RETRY?", YES_NO);
		if (ret == SUCCESS) goto get_authcode;
		return;
	}

get_pincode:;
	if(!(ret = get_pin(&my_bill))) goto confirm_retry;
	if (ret != 4)
	{
		ret = show_message("PIN is of invalid length! Retry?", CHOICE, "RETRY?", YES_NO);
		if (ret == SUCCESS) goto get_pincode;
		return;
	}
		
	disp_clear_viewport();
	disp_print_row(1, "AIRTEL MONEY WITHDRAW");
	
	sprintf(buf, "NO: %s", my_bill.phone_no);
	disp_print_row(2, buf);
	
	sprintf(buf, "CODE: %s", my_bill.account_no);
	disp_print_row(3, buf);
	
	disp_print_row(4, "Press Enter to Send");
	disp_print_row(5, "Press Cancel to Exit");
	
	while(1)
	{
		unsigned char key = kpd_getkey();
		if (key == KEY_ENTER) break;
		if (key == KEY_CANCEL) return;
	}
	
	char tr_id[16];
	sprintf(tr_id, "%d", generate_transaction_id());
	
	sprintf(string_buf, "auth=[username=checkout,password=um0j1p@ych3ck-ut]&account_no=%s&transaction_id=%s&amount=%d&reference=%s&pin=%s&f=airtelCashIn",
			 my_bill.phone_no, tr_id, my_bill.amt, my_bill.account_no, my_bill.pin);
			 
	printf("\nAirtelWithdrawals: %s", string_buf);
	
	char *fields[] = {"AMOUNT", "ACCOUNT_NO", "DATE", "VENDOR", "VENDER ", "REFERENCE_NO", "STATUS", "RESP :", "DESCRIPTION :", NULL};
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

static void do_airtel_deposit()
{
	//@TODO:
	//Get Amount
	//Get Pin
	//Do Transaction
	
	//char acc_string[32];
	bill_t my_bill;
	char buf[32];
	
get_inputs:;

	if(!get_phone_no(&my_bill)) goto confirm_retry;
	
	if(strncmp(my_bill.phone_no, "254", 3))
	{
		if(my_bill.phone_no[0] != '0') goto confirm_retry;
		
		char *strp = (char *)&my_bill.phone_no[1];

		strcpy(buf, "254");
		strcat(buf, strp);
		strcpy(my_bill.phone_no, buf);	
	}
	
	if(!get_amount(&my_bill)) goto confirm_retry;

	char tr_id[16];
	sprintf(tr_id, "%d", generate_transaction_id());
	
	disp_clear_viewport();
	disp_print_row(1, "AIRTEL MONEY DEPOSIT");
	
	sprintf(buf, "NO: %s", my_bill.phone_no);
	disp_print_row(2, buf);
	
	sprintf(buf, "AMT: %d", my_bill.amt);
	disp_print_row(3, buf);
	
	disp_print_row(4, "Press Enter to Send");
	disp_print_row(5, "Press Cancel to Exit");
	
	while(1)
	{
		unsigned char key = kpd_getkey();
		if (key == KEY_ENTER) break;
		if (key == KEY_CANCEL) return;
	}
	
//	if(!get_pin(&my_bill)) goto confirm_retry;
	sprintf(string_buf, "auth=[username=checkout,password=um0j1p@ych3ck-ut]&transaction_id=%s&account_no=%s&amount=%d&f=airtelCashOut", tr_id, my_bill.phone_no, my_bill.amt);					
	
	char *fields[] = {"AMOUNT", "ACCOUNT_NO", "DATE", "VENDOR", "VENDER ", "REFERENCE_NO", "STATUS", "RESP :", "DESCRIPTION :", NULL};
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

static int cvt_date_to_iso(char *date)
{
	//Input DD.MM.YYYY
	//Output YYYYMMDD
	char year[8];
	char day[4];
	char month[4];
	
	if (date[2] != '.' || date[5] != '.')
		return -1;
	
	day[0] = date[0];
	day[1] = date[1];
	day[2] = 0;
	
	month[0] = date[3];
	month[1] = date[4];
	month[2] = 0;
	
	year[0] = date[6];
	year[1] = date[7];
	year[2] = date[8];
	year[3] = date[9];
	year[4] = 0;
	
	strcpy(date, year);
	strcat(date, "-");
	strcat(date, month);
	strcat(date, "-");
	strcat(date, day);
	
	return 0;
}

static void do_airtel_registration()
{
	char firstname[24];
	char lastname[24];
	char nationality[24];
	char place_of_birth[24];
	char phone_no[16];
	char streetaddress[24];
	char town[24];
	char id_no[16];
	char dob[16];
	
	char buf[32];
	int stat = 0;
	
get_inputs:;
	
	stat = get_integer("Enter Phone No.", phone_no, 12, FALSE, FALSE);
	if(!stat) goto input_error;
	
	if(strncmp(phone_no, "254", 3))
	{
		if(phone_no[0] != '0' && phone_no[0] != '7') goto input_error;
		
		char *strp = (char *)&phone_no[1];
		if (phone_no[0] == '7') strp = &phone_no[0];

		strcpy(buf, "254");
		strcat(buf, strp);
		strcpy(phone_no, buf);
	}
	
	stat = get_string("Enter First Name", firstname, 15, FALSE, FALSE);
	if (!stat) goto input_error;
	
	stat = get_string("Enter Last Name", lastname, 15, FALSE, FALSE);
	if (!stat) goto input_error;
	
	stat = get_integer("Enter ID No.", id_no, 10, FALSE, FALSE);
	if (!stat) goto input_error;
	
get_dob:;

	stat = get_string("Date Of Birth [D.M.Y]", dob, 12, FALSE, FALSE);
	if (!stat) goto input_error;
	
	if (stat != 10 || cvt_date_to_iso(dob))
	{
		show_message("Invalid date format!", ICONFAILURE, "Input Error", OK);
		goto get_dob;
	}
		
	stat = get_string("Enter Town", town, 18, FALSE, FALSE);
	if (!stat) goto input_error;
	
	strcpy(streetaddress, town);
	strcpy(place_of_birth, town);
	
	stat = get_string("Enter Address", streetaddress, 18, FALSE, TRUE);
	if (!stat) goto input_error;
		
	stat = get_string("Enter Place of Birth", place_of_birth, 18, FALSE, TRUE);
	if (!stat) goto input_error;
	
	strcpy(nationality, "KENYAN");
	stat = get_string("Enter Nationality", nationality, 18, FALSE, TRUE);
	if (!stat) goto input_error;
	
	_ucase(firstname);
	_ucase(lastname);
	_ucase(streetaddress);
	_ucase(town);
	_ucase(place_of_birth);
	_ucase(nationality);
	
	disp_clear_viewport();
	disp_print_row(1, "NEW AIRTEL-M USER");
	
	sprintf(buf, "NO: %s", phone_no);
	disp_print_row(2, buf);
	
	sprintf(buf, "NAME: %s %s", firstname, lastname);
	disp_print_row(3, buf);
	
	disp_print_row(4, "Press Enter to Send");
	disp_print_row(5, "Press Cancel to Exit");
	
	while(1)
	{
		unsigned char key = kpd_getkey();
		if (key == KEY_ENTER) break;
		if (key == KEY_CANCEL) return;
	}
	
	string_buf[0] = 0;
	char *msg_buf = (char *)malloc(1024);
	if (!msg_buf)
	{
		printf("MALLOC(1024) returned 0x00000000!\nCheck for leakage!");
		
		show_message("Out of Memory. Please Restart!", ICONFAILURE, "FATAL ERROR", OK);
		return;
	}
	
	sprintf(msg_buf, "auth=[username=checkout,password=um0j1p@ych3ck-ut]&f=registerAirtel&serial=%s&msisdn=%s&account_id=6053930&first_name=%s&last_name=%s&street_address=%s&town=%s&document_type=ID&id_number=%s&place_of_birth=%s&dob=%s&nationality=%s&reference=%d",
				"7672t72t72", phone_no, firstname, lastname, streetaddress, town, id_no, place_of_birth, dob, nationality, generate_transaction_id());
	
	printf("MSG_BUF := %s\nMSG_LEN := %d\n", msg_buf, strlen(msg_buf));
#if 0
	url_data_t res;
	stat = do_paperless_transaction(msg_buf, "https://www.umojapay.com/service/api/pos", &res);
		
	if (stat) SAFE_DELETE(res.data);
	SAFE_DELETE(msg_buf);
	
	msg_buf = NULL;
	res.data = NULL;
#endif
	
	stat = do_transaction(msg_buf, "https://www.umojapay.com/service/api/pos", "AIRTEL MONEY REGISTRATION SUCCESSFUL", FALSE, PRINTFLAG_DEFAULT);		
	if (stat)
		show_message("New User Registered!", INFORMATION, "UMOJAPAY", OK);
	
	return;
	
input_error:;
	
	stat = show_message("Input Error Or User Cancelled! Retry?", CHOICE, "RETRY?", OK_CANCEL);
	if (stat != SUCCESS)
		return;
		
	goto get_inputs;
}

static void do_reversal()
{
}

static void select_option(void *p)
{
	int selection = *(int *)p;
	selected_service = selection;
	//Select this bank from array and call transaction function.
	
	menu_entry_t menulist[] = { {"Withdraw", 1, 1, do_airtel_withdrawal, NULL},
								{"Deposit", 1, 2, do_airtel_deposit, NULL},
								{"Registration", 1, 3, do_airtel_registration, NULL}
							  };
							  
  	select_settings_t SelectSettings;
	SelectSettings.bSelectOption=1;
	SelectSettings.bModeSelect=0;
  	show_menu("Select an option", 0, (void *)menulist, 3, 0, (void *)&SelectSettings);
}

void mobile_banking_entry()
{
	char *header = "  UMOJAPAY SERVICES  \n    CASH SERVICES   \n";
	set_print_header(header);
	
	selected_service = -1;
	
	int test_array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
 	menu_entry_t menulist[] = {	/*{"MPESA", 1, 1, select_option, test_array},
 								{"YUCASH", 1, 2, select_option, &test_array[1]}, */
 								{"AIRTEL MONEY", 1, 1, select_option, &test_array[2]}
 								//{"ORANGE MONEY", 1, 4, select_option, &test_array[3]},
 								//{"TANGAZA", 1, 5, select_option, &test_array[4]}
 							};
 	
  	select_settings_t SelectSettings;	
	SelectSettings.bSelectOption=1;
	SelectSettings.bModeSelect=0;
  	show_menu("Mobile Banking Menu", 0, (void *)menulist, 1, 0, (void *)&SelectSettings);
}
