/* =============================================================================================================================================
 * MainMenu.c - GL14 Display Testing App.
 * @karokidii
 *============================================================================================================================================*/
 
 #include "commonfuncs.h"
 #include <stdio.h>
 #include <stdlib.h>
 
 #include "modules.h"
 #include "platform.h"
 
 void print_option(void *p)
 {
 	int *id = (int *)p;
 	char str[32];
 	
 	sprintf(str, "Menu option %d was selected", id[0]);
 	disp_clear_viewport();

 	disp_print_center(str, 0);
 }
  
 void do_exit(void *p)
 {
 	disp_close();
 	exit(0);
 }
 
 int main()
 {
 	disp_init();
 	
 	disp_print_center("WELCOME TO UMOJA PAY", 0);
 	uint8_t lkey = kpd_get_key();
 	
 	int test_array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
 	
 	menu_entry_t menulist[] = {	{"UMOJA PAY", 1, 1, print_option, test_array},
 								{"MOBILE BANKING", 1, 2, print_option, &test_array[1]},
 								{"AGENCY BANKING", 1, 3, print_option, &test_array[2]},
 								{"PAY BILL", 1, 4, pay_bill_entry, NULL},					//Pay Bill Enable
 								{"EMV", 1, 5, print_option, &test_array[4]},
 								{"AGENT SETTINGS", 1, 6, print_option, &test_array[5]},
 								{"DEVICE SETTINGS", 1, 7, print_option, &test_array[6]},
 								{"EXIT", 1, 8, do_exit, NULL}
 							};
 	
 	while(1)						
 		gl14_do_menu("MENU TITLE", menulist, 8);
 	
// 	lk_dispfill();				//Check if we reached here.
 	lkey = kpd_get_key();
 	
 	disp_close();
 }
