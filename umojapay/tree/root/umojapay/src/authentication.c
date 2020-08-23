 #include "global.h"
 #include "commonfuncs.h"
 #include <stdio.h>
 #include <stdlib.h>
 
 #include "platform.h"
 #include "common.h"
 
 static char string_buf[32];
 
 typedef struct __attribute__((packed)) __password_info_t
 {
 	int pin_code[4];
 	int failed_attempts;
 }
 password_info_t;

 int authenticate_user()
 {
 	char passd[8];
 	memset(passd, 0, 8);
 	
	int ret = get_integer("Enter PIN", passd, 4, 1, FALSE);
	if (!ret) return FAILURE;
	
	if (!strcmp(passd, "4636"))
		return SUCCESS;

 	FILE *fp = fopen("/etc/passd.vk", "r+b");
 	if (!fp)
 	{
 		disp_clear_viewport();
 		disp_print_center("PIN BLOCKED", ARIEL16_18);
 		disp_print_row(4, "Contact Admin");
 		sleep(1);
 	 	return FAILURE;
	}
	
	password_info_t passd_info;
	fread(&passd_info, sizeof(passd_info), 1, fp);
	
	if (passd_info.failed_attempts >= 3)
	{
 		disp_clear_viewport();
 		disp_print_center("PIN BLOCKED", ARIEL16_18);
 		disp_print_row(4, "Contact Admin");
 		sleep(1);
 		fclose(fp);
 	 	
 	 	return FAILURE;	
	}
	
	char dummy[5];
	dummy[4] = 0;
	dummy[3] = (char) passd_info.pin_code[3];
	dummy[2] = (char) passd_info.pin_code[2];
	dummy[1] = (char) passd_info.pin_code[1];
	dummy[0] = (char) passd_info.pin_code[0];
	
	if (strcmp(dummy, passd))
	{
		char tmp[32];
		passd_info.failed_attempts ++;
		
		sprintf(tmp, "%d attempts left", (3 - passd_info.failed_attempts));
		disp_clear_viewport();
		disp_print_row(1, "Invalid PIN");
		disp_print_row(3, tmp);
		disp_print_row(5, "Press A Key");
		kpd_getkey();
		
		rewind(fp);
		fwrite(&passd_info, sizeof(passd_info), 1, fp);
		fclose(fp);
		
		return FAILURE;
	}
	
	if (passd_info.failed_attempts > 0)
	{
		passd_info.failed_attempts = 0;
		rewind(fp);
		fwrite(&passd_info, sizeof(passd_info), 1, fp);
	}
	
	fclose(fp);
	return SUCCESS;
 }
 
 void change_user_pin()
 {
 	char passd[8];
 	memset(passd, 0, 8);
 	
	int ret = get_integer("Enter Admin PIN", passd, 4, 1, FALSE);
	if (!ret) return;
	
	if (strcmp(passd, "4636"))
	{
 		disp_clear_viewport();
 		disp_print_center("Invalid PIN", ARIEL16_18);
 		sleep(1);
 	 	return;
	}
	
	ret = get_integer("Enter New PIN", passd, 4, 1, FALSE);
	if (!ret) return;
 	
 	FILE *fp = fopen("/etc/passd.vk", "w+b");
 	if (!fp)
 	{
 		disp_clear_viewport();
 		disp_print_center("Error Writing Record", ARIEL16_18);
 		sleep(1);
 	 	return;
	}
	
	password_info_t passd_info;
	passd_info.pin_code[0] = (int)passd[0];
	passd_info.pin_code[1] = (int)passd[1];
	passd_info.pin_code[2] = (int)passd[2];
	passd_info.pin_code[3] = (int)passd[3];
	passd_info.failed_attempts = 0;
	
	fwrite(&passd_info, sizeof(passd_info), 1, fp);
	fclose(fp);
	
 	disp_clear_viewport();
 	disp_print_center("PIN changed", ARIEL16_18);
 	sleep(1);
 }
