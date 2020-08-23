/*----------------------------------------------------------------------------------------------------------------------------------
 * CommonFuncs.h
 * Common Function Abstraction Module
 * @karokidii
 *==================================================================================================================================*/
 
 #ifndef __COMMON_STRUCTS_H_INCLUDED
 #define __COMMON_STRUCTS_H_INCLUDED
 
 #include <stdint.h>
 #include <unistd.h>
 #include <string.h>
 #include <memory.h>
 
 typedef struct /* __attribute((packed))*/
 {
 	char display_name[18];		/* Name to display */
 	unsigned int level;			/* Level */
 	unsigned int elem;			/* Element no. */
 	void (*callback)(void *);	/* Callback function */
 	void *arg_ptr;				/* Argument pointer */
 }
 menu_entry_t;
 
 typedef struct __attribute__((packed))
 {
 	size_t size;
 	char *data;
 }
 url_data_t;
 
 typedef struct
 {
	unsigned char bModeSelect;
	unsigned char bSelectOption;
	unsigned char ucSelectedItem;
 }
 select_settings_t;
 
typedef struct
{
	char account_no[16];
	char business_no[16];
	char phone_no[16];
	unsigned int amt;
	char pin[8];
	//char svc[64];
	//char *billType;
}
bill_t;

typedef void (*alarm_handler_t)(void);		//Define alarm_handler_t prototype.
 
 #endif
 
