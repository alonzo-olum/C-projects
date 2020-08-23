#ifndef __EZEAL_COMMONFUNCS_INC
#define __EZEAL_COMMONFUNCS_INC

#include <stdio.h>
#include <string.h>
#include <memory.h>
#include "commonstructs.h"
#include "LCD.h"
#include "Utility.h"

 int ezl_query_integer(char *caption, char *out, int max_chars, int password);
 int ezl_query_string(char *caption, char *out, int max_chars, int password);
 int ezl_do_question(const char *title, const char *prompt);
 int ezl_print_center(const char *text);
 int ezl_print_centered_row(unsigned char row, const char *text);
 int ezl_do_curl_post(const char *url, const char *params, url_data_t *out);

#endif
