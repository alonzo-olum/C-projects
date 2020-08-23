#if !defined __COMMON_HEADER_INCLUDED
#define __COMMON_HEADER_INCLUDED

 #include "global.h"
 #include "commonfuncs.h"
 #include <stdint.h>
 #include <stdio.h>
 #include <stdlib.h> 
 #include "platform.h"
 #include "modules.h"
 
 //#define IS_DEBUG_VERSION 1
 
 #ifndef IS_DEBUG_VERSION
 
   #define VERSION_MAJOR	0
   #define VERSION_MINOR	1
   #define BUILD_NUMBER		1028
 
 #else

   #define VERSION_MAJOR	0
   #define VERSION_MINOR	0
   #define BUILD_NUMBER		0
 
 #endif
 
 #define SAFE_DELETE(p)\
 	if (p) free(p);\
 	p = NULL;
 
 typedef struct
 {
 	char sentinel;		 	//Usually %
 	char FormatCode;	 	//Use only B
 	char PAN_STRING[20]; 	//Upto 19 characters (PAN), look for SectionMarker '^' to determine end.
 	char country_code[4];	//MasterCard Only: PAN STARTS WITH (59)
 	char Name[27];			//Surname/Other Names: Find FS marker '^' for end.
 	char exp_date[5];		//YYMM: or '^' if missing.
 	char svc_code[4];		//Or '^' if missing.
 	//Optional
 	char pvv[6];			//Offset to PVV
 	char dd[16];			//All remaining characters upto end sentinel
 	char sentinel2;			//Usually ?	
 	char LCRC;				//CRC
 }
 MTRACK1_HEADER, MAGTRACK1_HEADER;
 
 typedef struct
 {
 	char *key;
 	char *value;
 }
 epair_t;
 
 typedef struct
 {
 	int epair_count;
 	epair_t *pairs;
 }
 EPAIR_DATA;
 
 #define PRINTFLAG_INCLUDE_TAIL				0x1
 #define PRINTFLAG_MARK_DUPLICATE			0x2
 #define PRINTFLAG_INCLUDE_SIGN				0x4
 #define PRINTFLAG_DEFAULT					(PRINTFLAG_INCLUDE_TAIL)
 
void LoadMagStripeData1(const char *data, MAGTRACK1_HEADER *mtrack);

EPAIR_DATA convert_from_json(const char *json_string);
EPAIR_DATA convert_from_delimited(const char *delimited_string, const char *delimiters);
void destroy_epairs(EPAIR_DATA *epairs);
const char *fetch_key_value(EPAIR_DATA *epairs, const char *keyname);

unsigned char *prep_bitmap(unsigned char *src, int h, int dilation);
int verify_version(int major, int minor, int build); 
void set_print_header(const char *str);
void set_print_logo(const void *data, unsigned long length);
uint32_t generate_transaction_id(); 
int get_serial();
int get_amount(bill_t *bill);
int get_phone_no(bill_t *bill);
int get_pin(bill_t *bill);
int get_acc_no(bill_t *bill);
int copy_acc_no(bill_t *bill);
int get_business_no(bill_t *bill);
char *_ucase(char *str);
char *drop_spaces(const char *string, char *out);
char *copy_string_until(char *src, char *dst, char lim);
void get_next_line(char *str, char *out);
char *_trim(char *string);
char *_trim_trim(char *string);
char ** str_tokenize(char *stres, int *_ct, const char *delimiter);
char ** tokenize(char *stres, int *_ct);
char *align_printer_string(char *string);
const char *fetch_serial_no();
void prepare_printer_string(char *str);
int print_receipt(const char *body, const char *message, const char *tail, int flags);
int print_result(url_data_t *result, char *header_message, int double_copy, int flags);
int enable_connection();

int do_transaction(const char *param_string, const char *url_path, const char *success_message, int print_agent_copy, int flags);
int do_paperless_transaction(const char *param_string, const char *url_path, url_data_t *out);
int do_filtered_transaction(const char *param_string, const char *url_path, const char *success_message, int print_agent_copy, const char **filters, int flags);

int do_formatted_rct_transaction(const char *param_string, const char *url_path, const char *success_message,
								 const char **leader_fields, const char **info_fields, const char **id_field, int print_agent_copy);

int authenticate_user();
void change_user_pin();

#endif
