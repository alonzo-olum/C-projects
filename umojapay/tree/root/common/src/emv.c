 #include "commonfuncs.h"
 #include <stdio.h>
 #include <stdlib.h>

static void do_selection(void *p)
{
	int selected = *(int *)p;
	
	lk_dispclr();
	gl14_print_center("Swipe Your Card");
	
	//Wait for card swipe.
	gl14_wait_for_key();	//@PLACEHOLDER
}

void umoja_pay_entry()
{
	int lst[] = {1, 2, 3, 0};
 	menu_entry_t menulist[] = {	{"EURO PAY", do_selection, lst},
 								{"MASTERCARD", do_selection, &lst[1]},
 								{"VISA", do_selection, &lst[2]}
 							};
 	
  	gl14_do_menu("UmojaPay Menu", menulist, 5);
}
