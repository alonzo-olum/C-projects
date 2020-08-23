/*----------------------------------------------------------------------------------------------------------------------------------
 * CommonFuncs.h
 * Common Function Abstraction Module
 * @karokidii
 *==================================================================================================================================*/
 
 #ifndef __GL14_COMMON_FUNCS_H_INCLUDED
 #define __GL14_COMMON_FUNCS_H_INCLUDED
 
 #include "commonstructs.h"
 #include <curl.h>
 #include <malloc.h>
 #include <stdlib.h>
 #include <0202lcd.h>
 #include <V91printer.h>
 #include <V91magswipe.h>
 
 #define min(a, b) (a < b)? a: b
 #define max(a, b) (a > b)? a: b
 
 #define SAFE_DELETE(p) if(p) free(p)
  
 #define KEY_ASTERICK   10
 #define KEY_HASH		11
 #define KEY_CANCEL		12
 #define KEY_CLEAR		13
 #define KEY_ENTER		15
 #define KEY_F1			17
 #define KEY_F2			18
 #define KEY_F3			19
 #define KEY_F4			20
 #define KEY_F5			21
 #define KEY_F6			22
 
 #define gl14_wait_for_key lk_getkey
 
 typedef struct
 {
 	char phone_no[16];
 	char apn_name[16];
 	char username[10];
 	char password[10];
 }
 gprs_settings_t;
 
 int gl14_display_init();
 int gl14_display_shut();
 int gl14_set_cursor(int x, int y);
 int gl14_show_message(const char *prompt, unsigned int style, const char *title, unsigned int buttons);
 int gl14_query_integer(char *caption, char *out, int max_chars, int password, int use_hint);
 int gl14_query_string(char *caption, char *out, int max_chars, int password, int use_hint);
 int gl14_do_question(const char *title, const char *prompt);
 int gl14_print_center(const char *text);
 int gl14_print_centered_row(unsigned char row, const char *text);
 int gl14_do_curl_init();
 int gl14_do_curl_shut();
 int gl14_do_menu(const char *title, menu_entry_t *menu_entries, int entry_count);
 int gl14_do_curl_post(const char *url, const char *params, url_data_t *out);
 int gl14_ftp_download(const char *url, const char *filename);
 int gl14_magcard_open();
 int gl14_magcard_close();
 int gl14_magcard_flush();
 int gl14_magcard_read(uint8_t *Track1Data, uint8_t *Track1Length, uint8_t *Track2Data, uint8_t *Track2Length, uint8_t *Track3Data, uint8_t *Track3Length);
 int gl14_printer_open();
 int gl14_printer_close();
 int gl14_printer_line(unsigned char symbol, int Font);
 int gl14_printer_flush();
 int gl14_printer_feed(int Lines);
 int gl14_printer_print_text(char *text, int length, int font);
 int gl14_printer_print_bitmap(void *bitmap, unsigned int length);
 int gl14_net_enable();
 int gl14_net_disable();
 void gl14_calibrate_network();
 void gl14_choose_network();
 int gl14_get_gsm_location(void *);
 int gl14_set_alarm(alarm_handler_t func, int seconds);
 
 #endif
