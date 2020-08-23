 #include "common.h"

static void do_selection(void *p)
{
	int selected = *(int *)p;
	
	disp_clear_viewport();
	disp_print_center("Swipe Your Card", ARIEL16_18);
	
	//Wait for card swipe.
	kpd_getkey();	//@PLACEHOLDER
}

void emv_entry()
{
	int lst[] = {1, 2, 3, 0};
 	menu_entry_t menulist[] = {	{"EURO PAY", 1, 1, do_selection, lst},
 								{"MASTERCARD", 1, 2, do_selection, &lst[1]},
 								{"VISA", 1, 3, do_selection, &lst[2]}
 							};
 	
   	select_settings_t SelectSettings;	
	SelectSettings.bSelectOption=1;
	SelectSettings.bModeSelect=0;
  	show_menu("EMV Menu", 0, (void *)menulist, 3, 0, (void *)&SelectSettings);
}
