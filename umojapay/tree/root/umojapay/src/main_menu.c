/* =============================================================================================================================================
 * MainMenu.c - MainEntry.
 * @karokidii
 *============================================================================================================================================*/
 
 #include "common.h"
 #include "wallet.h"		//Wallet Logo (GIMP)
  
 static void do_exit(void *p)
 {
 	disp_clear_viewport();
 	disp_print_center("Shutting down...", ARIEL16_18);
 	
 	disp_close();
 	system("poweroff");
 	
 	sleep(3);
 	printf("Failed to poweroff!!");
 	exit(0);
 }
 
 void do_bash()
 {
 	net_disable();
 	
 	disp_close();
 	exit(0);
 }
  
 void auto_update()
 {
 #if !defined EZEALVERSION
 
 	if (enable_connection() != SUCCESS) return;
 
 	disp_clear_viewport();
 	disp_print_center("Checking ...", ARIEL16_18);
 	
 	url_data_t version_info;
 	int status = do_curl_post("http://www.technilink.co.ke/umoja_pos/check_version.php", "", &version_info);
 	
 	if (status == FAILURE)
 	{
 		disp_clear_viewport();
 		disp_print_center("Verification Failed!", ARIEL16_18);
 		disp_print_row(5, "Press a Key");
 		
 		kpd_getkey();
 		return;
	}
	
	int version_major, version_minor, build_no;
	char *vstr = (char *)version_info.data;
	
	printf("VSTR: %s", vstr);
	printf("\n");
	
	sscanf(vstr, "%d.%d.%d", &version_major, &version_minor, &build_no);
	
	status = verify_version(version_major, version_minor, build_no);
	free(version_info.data);
	
	if (status == SUCCESS)
	{
 		disp_clear_viewport();
 		disp_print_center("No Update Required", ARIEL16_18);
 		disp_print_row(5, "Press a Key");
 		
 		kpd_getkey();
 		return;
	}
	
 	disp_clear_viewport();
 	disp_print_center("Downloading ...", ARIEL16_18);
 	
 	status = do_ftp_download("http://www.technilink.co.ke/umoja_pos/image_dl/umojapay_gl14.img", "/mnt/tmp/dl_img.img");
 	if (status == SUCCESS)
 	{
 		disp_clear_viewport();
 		disp_print_center("Installing ...", ARIEL16_18);
 	
 		system("cp /mnt/tmp/dl_img.img /mnt/tmp/app.img");
 		system("/home/./downloadapplication");
 		
 		disp_print_center("Image Installed.", ARIEL16_18);
 		sleep(2);
 		
 		disp_clear_viewport();
		disp_print_center("Rebooting ...", ARIEL16_18);
		
		system("reboot");
		while(1) sleep(2);		//Wait for reboot		
	}
	else
	{
 		disp_print_center("Download Failed!", ARIEL16_18);
 		disp_print_row(5, "Press a Key");
 		kpd_getkey();
	}
 
 #endif
 }
 
 void device_options()
 {
 #if !defined EZEALVERSION
   	
   	menu_entry_t menulist[] = {	{"CHOOSE NETWORK", 1, 1, net_choose, NULL},
 								{"CONFIGURE NETWORK", 1, 2, net_calibrate, NULL},
 								{"UPGRADE SOFTWARE", 1, 3, auto_update, NULL}
 							  };
 							
 	show_menu("DEVICE SETTINGS", 0, (void *)menulist, 3, 0, NULL);
 
 #endif
 }
 
 void usb_install()
 {
 	disp_clear_viewport();
 	int ret = system("/home/./install-gl14");
 	
 	if(ret)
 	{
 		disp_print_center("Error Installing Image!", ARIEL16_18);
 		disp_print_row(4, "Press a Key");
 		kpd_getkey();
 	}
 	else
 		disp_print_center("Image Installed.", ARIEL16_18);
	
	if(!ret) 
	{
		sleep(1);
		disp_clear_viewport();
		disp_print_center("Rebooting ...", ARIEL16_18);
		
		system("reboot");
		while(1) sleep(2);		//Wait for reboot
	}
 }
 
 void admin_options()
 {
 #ifndef EZEALVERSION
 
 	char npin[8];
 	if(!get_integer("Enter Admin PIN", npin, 4, 1, FALSE)) return;
 	
 	if(strcmp(npin, "4636"))
 	{
 		disp_clear_viewport();
 		disp_print_center("Invalid PIN", ARIEL16_18);
 		sleep(1);
 	 	return;
 	}
 
 	menu_entry_t menulist[] = {{"Change PIN", 1, 1, change_user_pin, NULL},
 							   {"USB Installation", 1, 2, usb_install, NULL},
 							   {"Download Image", 1, 3, auto_update, NULL},
 							   {"BASH Prompt", 1, 4, do_bash, 0}
 							  };
 							  
 	show_menu("SYSTEM ADMIN", 0, (void *)menulist, 4, 0, NULL);
 
 #endif
 }
 
 static void do_system_info()
 {
 	char system_version[16];
 	
 	sprintf(system_version, "%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, BUILD_NUMBER);
 	
 	disp_clear_viewport();
 	disp_print_row(1, "My Serial Number:");
 	disp_print_row(2, fetch_serial_no());
 	disp_print_row(3, "Software Verison:");
 	disp_print_row(4, system_version);
 	
 	kpd_getkey();
 }

 static void my_device_menu()
 {
 	menu_entry_t menulist[] = {{"System Information", 1, 1, do_system_info, NULL}};
 	show_menu("MY DEVICE", 0, (void *)menulist, 1, 0, NULL);
 }

 void print_option(void *p)
 {
 	int *id = (int *)p;
 	//char str[32];
 	
 	//sprintf(str, "This function is not implemented.", id[0]);
 	disp_clear_viewport();

 	disp_print_center("The selected module is not installed", ARIEL16_18);
 	unsigned char lkey = kpd_getkey();
 }
 
 void system_menu(int access)
 {
  	int test_array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
 	menu_entry_t menulist[] = {/*{"UMOJA PAY", 1, 1, umojapay_entry, test_array}, */
 								{"COUNTY SERVICES", 1, 1, county_entry, NULL},
 								{"MOBILE BANKING", 1, 2, mobile_banking_entry, NULL},
 								/*//{"AGENCY BANKING", 1, 3, agency_banking_entry, NULL}, */
 								//{"AIRTIME", 1, 2, airtime_entry, NULL},
 								//{"PAY BILL", 1, 3, pay_bill_entry, NULL}, /**/ //Pay Bill Enable
 								//{"EMV", 1, 6, emv_entry, NULL},
 								//{"AGENT SETTINGS", 1, 7, print_option, &test_array[6]},
 								{"DEVICE SETTINGS", 1, 3, device_options, NULL},
 								{"ADMINISTRATOR*", 1, 4, admin_options, NULL},
 								{"MY DEVICE", 1, 5, my_device_menu, NULL},
 								{"EXIT", 1, 6, do_exit, NULL}
 							};
 	
 	select_settings_t mode_settings;
 	mode_settings.bSelectOption=0;
 	mode_settings.bModeSelect=0;
 	
 	while(1)						
 		show_menu("MAIN MENU", 0, (void *)menulist, 6, 0, (void *)&mode_settings);
 }
 
 int authenticate_owner()
 {
 	return authenticate_user();
/* 	int err = magcard_open();
	if(err != SUCCESS) goto open_error;
	
	magcard_flush();
	
	disp_clear_viewport();
	disp_print_center("Swipe your card ...", ARIEL16_18);
	
	uint8_t read_length;
	char buf[256];
	
	err = magcard_read_track1(buf, &read_length);
	if(err == FAILURE) goto read_error;

		
	MTRACK1_HEADER mtrack;
	LoadMagStripeData1(buf, &mtrack);
	
//	disp_clear_viewport();
//	disp_print_row(2, mtrack.Name);
//	disp_print_row(4, mtrack.PAN_STRING);
	
//	sleep(1);
	
	magcard_close();
	return;
		
open_error:;
	show_message("Card Reader Malfunction!!", INFORMATION, "ERROR!", OK);
	return;
	
read_error:;
	show_message("Error reading from card!!", INFORMATION, "ERROR!", OK); */
 }
  
 void login_screen()
 {
 	int result;
 	while(1)
 	{
 		disp_clear_viewport();
 		disp_print_row(2, "SYSTEM LOGGED OUT");
 		
 		disp_print_row(4, "Press Enter To Log In");
 		disp_print_row(5, "Press Cancel To Exit");
 		
 		unsigned char key = kpd_getkey();
 		switch(key)
 		{
 			case KEY_ENTER:
 			{
 				if (authenticate_owner() != SUCCESS) continue;
 				return;
 			}
 			case KEY_CANCEL:
 			{
 				result = show_message("Really Exit?", CHOICE, "QUIT?", YES_NO);
 				if(result == SUCCESS)
 					do_exit(0);
 				else
 					continue;
			}
		}
 	}
 }
 
 void system_entry()
 {
 	if(authenticate_owner() != SUCCESS) return;
 	
 	set_alarm(login_screen, 3600);
 	system_menu(0);
 }
  
 int ENTRYFUNC_NAME()
 {
 	disp_init();
 	disp_clear_viewport();
 	
 	disp_print_center("WELCOME TO UMOJA PAY", ARIEL16_18);
 	disp_print_row(5, "Press A Key");
 	
 	uint8_t lkey = kpd_get_key();
 	
 	get_serial();
 	
 	menu_entry_t menulist[] = {	{"LOG IN", 1, 1, system_entry, NULL},
 								{"DEVICE SETTINGS", 1, 2, device_options, NULL},
 								{"EXIT", 1, 3, do_exit, NULL}
 				   			  };
 	
 	select_settings_t mode_settings;
 	mode_settings.bSelectOption=0;
 	mode_settings.bModeSelect=0;
 	
 	while(1)						
 		show_menu("LOGIN SCREEN", 0, (void *)menulist, 3, 0, (void *)&mode_settings);
 	
 	//Only an illegal jump can get us here. 	
 	disp_close();
 	return 0;		//Shut the compiler up.
 }
