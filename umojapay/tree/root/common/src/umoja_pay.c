 #include "commonfuncs.h"
 #include <stdio.h>
 #include <stdlib.h>

static void do_withdrawal()
{
}

static void do_deposit()
{
}

static void do_payments()
{
}

static void do_requests()
{
}

static void do_signup()
{
}

void umoja_pay_entry()
{
 	menu_entry_t menulist[] = {	{"CASH WITHDRAWAL", do_withdrawal, NULL},
 								{"DEPOSIT", do_deposit, NULL},
 								{"PAYMENTS", do_payments, NULL},
 								{"REQUESTS", do_requests, NULL},
 								{"SIGN UP", do_signup, NULL}
 							};
 	
  	gl14_do_menu("UmojaPay Menu", menulist, 5);
}
