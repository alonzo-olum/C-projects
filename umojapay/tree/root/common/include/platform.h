/* ---------------------------------------------------------------------------------------------------------------------------------------------
 * platform.h
 * platform abstraction header for GL14 && eZeal systems
 *===========================================================================================================================================*/
 #ifndef __PLATFORM_HEADER_INCLUDED
 #define __PLATFORM_HEADER_INCLUDED
 
 #ifndef max
 #define max(a, b) (a > b)? a: b
 #define min(a, b) (a < b)? a: b
 #endif
 
 #define stricmp strcasecmp
 #define strnicmp strncasecmp
 
 #ifdef GL14VERSION
 
 //typedef int BOOL;
 
 #define ARIEL16_18 		0
 #define ARIEL6_9 			0
 #define ARIAL21_24			0
 #define MONOTIMES8_13 		0
 #define MONOARIEL16_18 	0
 
 #define FONTSMALLNORMAL	1
 #define FONTLARGEBOLD		2
 
 #define BLACK 0
 #define RED 1
 #define GREEN 2
 #define YELLOW 3
 #define BLUE 4
 #define WHITE 7
 
 #define OK 0
 #define OK_CANCEL 1
 #define YES_NO 2
 #define RETRY_CANCEL 3
 
 #define WARNING 16
 #define ICONFAILURE 17
 #define INFORMATION 18
 #define CHOICE 19
 
 #ifndef TRUE
 #define TRUE 1
 #define FALSE 0
 #endif
 
 #define SUCCESS 0
 #define FAILURE 1
 
 #define OUT
 #define IN
 
 #define ENTRYFUNC_NAME main
 
 typedef unsigned char UCHAR;
 typedef unsigned int UINT;
 typedef UCHAR BOOL;
 
 /* DISPLAY */
 #define disp_init								gl14_display_init
 #define disp_close								gl14_display_shut
 #define disp_print_center(Text, Font) 			gl14_print_center(Text)
 #define disp_print_row							gl14_print_centered_row
 #define disp_printf							gl14_printf
 #define disp_set_cursor						gl14_set_cursor
 #define disp_print(Text, Font)					gl14_print(Text)
 #define disp_set_bg							/* Set background color */
 #define disp_set_fg							/* Set foreground color */
 #define disp_select_viewport					/* Select viewport */
 #define disp_clear_viewport					lk_dispclr
 #define disp_change_mode						/* Change display mode */
 
 /* KEYPAD */
 #define kpd_init()										SUCCESS		/* Initialize keypad */
 #define kpd_get_key									lk_getkey
 #define kpd_getkey										kpd_get_key
 #define kpd_flush()									TRUE		/* Flush keypad */
 #define kpd_check_key_press							lk_getkey_wait
 #define kpd_get_data(Buffer, Size, Limit, Caption)							gl14_query_string(Caption, Buffer, Limit, 0)
 #define kpd_get_passwd(Buffer, Size, X1, Y1, X2, Limit)					gl14_query_string(0, Buffer, Limit, 1);
 #define kpd_get_numeric(Buffer, Size, X1, Y1, X2, Limit, Passwd, Font)		gl14_query_integer(0, Buffer, Limit, Passwd)
 #define kpd_close()									SUCCESS		/* Kill the keypad */
 
 /* MAGCARD */
 #define magcard_open														gl14_magcard_open
 #define magcard_close														gl14_magcard_close
 #define magcard_flush														gl14_magcard_flush
 #define magcard_read														gl14_magcard_read
 #define magcard_read_track1(Track1Data,  pTrack1Length)					gl14_magcard_read(Track1Data, pTrack1Length, 0, 0, 0, 0)
 #define magcard_read_track2(Track2Data,  pTrack2Length)					gl14_magcard_read(0, 0, Track2Data, pTrack2Length, 0, 0)
 
 /* Printer */
 #define printer_open									gl14_printer_open
 #define printer_close									gl14_printer_close
 #define printer_flush									gl14_printer_flush
 #define printer_feed									gl14_printer_feed
 #define printer_paper_status							prn_paperstatus
 #define printer_print_bitmap							gl14_printer_print_bitmap
 #define printer_print_text								gl14_printer_print_text
 #define printer_print_line								gl14_printer_line
 #define printer_begin_print()							SUCCESS
 
 
 /* General */
 #define show_menu(Title, Theme, EntryList, ItemCount, TimeOut, ModeSettings)	gl14_do_menu(Title, EntryList, ItemCount)
 #define do_question			gl14_do_question
 #define get_string				gl14_query_string
 #define get_integer			gl14_query_integer
 #define do_curl_init			gl14_curl_init
 #define do_curl_shut			gl14_curl_shut
 #define do_curl_post			gl14_do_curl_post
 #define show_message			gl14_show_message
 #define do_ftp_download		gl14_ftp_download
 
 #define read_serial_no(Buffer, Size)		lk_getpinpadid(Buffer)
 
 #define net_enable							gl14_net_enable
 #define net_disable						gl14_net_disable
 #define net_calibrate						gl14_calibrate_network
 #define net_choose							gl14_choose_network
 
 #define set_alarm							gl14_set_alarm
 
 #elif defined XGDVERSION
 
 //typedef int BOOL;
 
 #define ARIEL16_18 		0
 #define ARIEL6_9 			0
 #define ARIAL21_24			0
 #define MONOTIMES8_13 		0
 #define MONOARIEL16_18 	0
 
 #define FONTSMALLNORMAL	1
 #define FONTLARGEBOLD		2
 
 #define BLACK 0
 #define RED 1
 #define GREEN 2
 #define YELLOW 3
 #define BLUE 4
 #define WHITE 7
 
 #define OK 0
 #define OK_CANCEL 1
 #define YES_NO 2
 #define RETRY_CANCEL 3
 
 #define WARNING 16
 #define ICONFAILURE 17
 #define INFORMATION 18
 #define CHOICE 19
 
 #ifndef TRUE
 #define TRUE 1
 #define FALSE 0
 #endif
 
 #define SUCCESS 0
 #define FAILURE 1
 
 #define OUT
 #define IN
 
 #define ENTRYFUNC_NAME main
 
 typedef unsigned char UCHAR;
 typedef unsigned int UINT;
 
 /* DISPLAY */
 #define disp_init								xgd_display_init
 #define disp_close								xgd_display_shut
 #define disp_print_center(Text, Font) 			xgd_print_center(Text)
 #define disp_print_row							xgd_print_centered_row
 #define disp_printf							xgd_printf
 #define disp_set_cursor						xgd_set_cursor
 #define disp_print(Text, Font)					xgd_print(Text)
 #define disp_set_bg							/* Set background color */
 #define disp_set_fg							/* Set foreground color */
 #define disp_select_viewport					/* Select viewport */
 #define disp_clear_viewport					xgd_disp_clear
 #define disp_change_mode						/* Change display mode */
 
 /* KEYPAD */
 #define kpd_init()										SUCCESS		/* Initialize keypad */
 #define kpd_get_key									xgd_getkey
 #define kpd_getkey										kpd_get_key
 #define kpd_flush()									TRUE		/* Flush keypad */
// #define kpd_check_key_press							lk_getkey_wait
 #define kpd_get_data(Buffer, Size, Limit, Caption)							xgd_query_string(Caption, Buffer, Limit, 0)
 #define kpd_get_passwd(Buffer, Size, X1, Y1, X2, Limit)					xgd_query_string(0, Buffer, Limit, 1);
 #define kpd_get_numeric(Buffer, Size, X1, Y1, X2, Limit, Passwd, Font)		xgd_query_integer(0, Buffer, Limit, Passwd)
 #define kpd_close()									SUCCESS		/* Kill the keypad */
 
 /* MAGCARD */
 #define magcard_open														xgd_magcard_open
 #define magcard_close														xgd_magcard_close
 #define magcard_flush														xgd_magcard_flush
 #define magcard_read														xgd_magcard_read
 #define magcard_read_track1(Track1Data,  pTrack1Length)					xgd_magcard_read(Track1Data, pTrack1Length, 0, 0, 0, 0)
 #define magcard_read_track2(Track2Data,  pTrack2Length)					xgd_magcard_read(0, 0, Track2Data, pTrack2Length, 0, 0)
 
 /* Printer */
 #define printer_open									xgd_printer_open
 #define printer_close									xgd_printer_close
 #define printer_flush									xgd_printer_flush
 #define printer_feed									xgd_printer_feed
 #define printer_paper_status							prn_paperstatus
 #define printer_print_bitmap							xgd_printer_print_bitmap
 #define printer_print_text								xgd_printer_print_text
 #define printer_print_line								xgd_printer_line
 #define printer_begin_print()							SUCCESS
 
 
 /* General */
 #define show_menu(Title, Theme, EntryList, ItemCount, TimeOut, ModeSettings)	xgd_do_menu(Title, EntryList, ItemCount)
 #define do_question			xgd_do_question
 #define get_string				xgd_query_string
 #define get_integer			xgd_query_integer
 #define do_curl_post			xgd_do_curl_post
 #define show_message			xgd_show_message
 #define do_ftp_download		xgd_ftp_download
 
 #define read_serial_no(Buffer, Size)		sdkReadPosSn(Buffer)
 
 #define net_enable							xgd_net_enable
 #define net_disable						xgd_net_disable
 #define net_calibrate						xgd_calibrate_network
 #define net_choose							xgd_choose_network
 
 #define set_alarm							xgd_set_alarm			
 
 #elif defined V9294VERSION		//--------------------------------------------------------------------------------------------------------------
 
 //typedef int BOOL;
 
 #define ARIEL16_18 		0
 #define ARIEL6_9 			0
 #define ARIAL21_24			0
 #define MONOTIMES8_13 		0
 #define MONOARIEL16_18 	0
 
 #define FONTSMALLNORMAL	1
 #define FONTLARGEBOLD		2
 
 #define BLACK 0
 #define RED 1
 #define GREEN 2
 #define YELLOW 3
 #define BLUE 4
 #define WHITE 7
 
 #define OK 0
 #define OK_CANCEL 1
 #define YES_NO 2
 #define RETRY_CANCEL 3
 
 #define WARNING 16
 #define ICONFAILURE 17
 #define INFORMATION 18
 #define CHOICE 19
 
 #ifndef TRUE
 #define TRUE 1
 #define FALSE 0
 #endif
 
 #define SUCCESS 0
 #define FAILURE 1
 
 #define OUT
 #define IN
 
 #define ENTRYFUNC_NAME main
 
 typedef unsigned char UCHAR;
 typedef unsigned int UINT;
 typedef UCHAR BOOL;
 
 /* DISPLAY */
 #define disp_init								V9294_display_init
 #define disp_close								V9294_display_shut
 #define disp_print_center(Text, Font) 			V9294_print_center(Text)
 #define disp_print_row							V9294_print_centered_row
 #define disp_printf							V9294_printf
 #define disp_set_cursor						V9294_set_cursor
 #define disp_print(Text, Font)					V9294_print(Text)
 #define disp_set_bg							/* Set background color */
 #define disp_set_fg							/* Set foreground color */
 #define disp_select_viewport					/* Select viewport */
 #define disp_clear_viewport					lk_dispclr
 #define disp_change_mode						/* Change display mode */
 
 /* KEYPAD */
 #define kpd_init()										SUCCESS		/* Initialize keypad */
 #define kpd_get_key									lk_getkey
 #define kpd_getkey										kpd_get_key
 #define kpd_flush()									TRUE		/* Flush keypad */
 #define kpd_check_key_press							lk_getkey_wait
 #define kpd_get_data(Buffer, Size, Limit, Caption)							V9294_query_string(Caption, Buffer, Limit, 0)
 #define kpd_get_passwd(Buffer, Size, X1, Y1, X2, Limit)					V9294_query_string(0, Buffer, Limit, 1);
 #define kpd_get_numeric(Buffer, Size, X1, Y1, X2, Limit, Passwd, Font)		V9294_query_integer(0, Buffer, Limit, Passwd)
 #define kpd_close()									SUCCESS		/* Kill the keypad */
 
 /* MAGCARD */
 #define magcard_open														V9294_magcard_open
 #define magcard_close														V9294_magcard_close
 #define magcard_flush														V9294_magcard_flush
 #define magcard_read														V9294_magcard_read
 #define magcard_read_track1(Track1Data,  pTrack1Length)					V9294_magcard_read(Track1Data, pTrack1Length, 0, 0, 0, 0)
 #define magcard_read_track2(Track2Data,  pTrack2Length)					V9294_magcard_read(0, 0, Track2Data, pTrack2Length, 0, 0)
 
 /* Printer */
 #define printer_open									V9294_printer_open
 #define printer_close									V9294_printer_close
 #define printer_flush									V9294_printer_flush
 #define printer_feed									V9294_printer_feed
 #define printer_paper_status							prn_paperstatus
 #define printer_print_bitmap							V9294_printer_print_bitmap
 #define printer_print_text								V9294_printer_print_text
 #define printer_print_line								V9294_printer_line
 #define printer_begin_print()							SUCCESS
 
 
 /* General */
 #define show_menu(Title, Theme, EntryList, ItemCount, TimeOut, ModeSettings)	V9294_do_menu(Title, EntryList, ItemCount)
 #define do_question			V9294_do_question
 #define get_string				V9294_query_string
 #define get_integer			V9294_query_integer
 #define do_curl_post			V9294_do_curl_post
 #define show_message			V9294_show_message
 #define do_ftp_download		V9294_ftp_download
 
 #define read_serial_no(Buffer, Size)		lk_gethwid(Buffer)
 
 #define net_enable							V9294_net_enable
 #define net_disable						V9294_net_disable
 #define net_calibrate						V9294_calibrate_network
 #define net_choose							V9294_choose_network
 
 #define set_alarm							V9294_set_alarm
 
 #elif defined EZEALVERSION		//--------------------------------------------------------------------------------------------------------------
 
 #define ENTRYFUNC_NAME eZealMain
 
  /* DISPLAY */
 #define disp_init								bLCDInitialize
 #define disp_close								bLCDClose
 #define disp_print_center			 			bLCDDispCenter
 #define disp_print_row							ezl_print_centered_row
 #define disp_printf							bLCDPrintf
 #define disp_set_cursor(x, y)					vSetCursorPos(x, y, 1)
 #define disp_print(Text, Font)					bLCDDispData
 #define disp_set_bg							bSetColorBackground	/* Set background color */
 #define disp_set_fg							bSetColorForeground /* Set foreground color */
 #define disp_select_viewport					ucSelViewPort		/* Select viewport */
 #define disp_clear_viewport					vLCDClrViewPort
 #define disp_change_mode						bLCDChDispMode/* Change display mode */
 
 /* KEYPAD */
 #define kpd_init										bKeypadInitialize	/* Initialize keypad */
 #define kpd_get_key									ucGetKey
 #define kpd_getkey										kpd_get_key
 #define kpd_flush										bKeypadFlushBuf		/* Flush keypad */
 #define kpd_check_key_press							ucCheckKeyPress
 #define kpd_get_data									iKeypadGetData
 #define kpd_get_passwd									iGetPassword
 #define kpd_get_numeric								iGetNumericKey
 #define kpd_close										/* Kill the keypad */
 
 /* MAGCARD */
 #define magcard_open														bMagcardInitialize
 #define magcard_close														bMagcardClose
 #define magcard_flush														bFlushMagcardData 
 #define magcard_read(Track1,  Length1, Track2,\
 					  Length2, Track3,  Length3)						bMagcardReadData(Track1, Length1, Track2, Length2, 0)
 
 #define magcard_read_track1(Track1Data,  pTrack1Length)					bMagcardReadTrack1Data(Track1Data, Track1Length, 0)
 #define magcard_read_track2(Track2Data,  pTrack2Length)					bMagcardReadTrack1Data(Track2Data, Track2Length, 0)
 
 #define printer_open									bPrinterInitialize
 #define printer_close									bPrinterClose
 #define printer_flush									vPrinterFlushBuf
 #define printer_feed(Lines)							ezl_printer_feed(Lines)
 #define printer_paper_status							SUCCESS
 #define printer_print_bitmap()							SUCCESS
 #define printer_print_text(Data, Length, Font)			bPrinterAddData(Font, Data, Length)
 #define printer_print_line								bPrinterAddLine
 #define printer_begin_print							ucStartPrinting
 
 /* GENERAL */
 #define show_menu								ucMenuNavigation
 #define do_question							ezl_do_question
 #define get_string								ezl_query_string
 #define get_integer							ezl_query_integer
 #define do_curl_post							ezl_do_curl_post
 #define show_message							uiMsgBox
 
 #define read_serial_no							iReadSerialNumber
 
 #define net_enable						iDataNetworkEnable
 #define net_disable					iDataNetworkDisable
 #define net_calibrate()
 #define net_choose()
 
 #define set_alarm								ezl_set_alarm

 #endif		//ifdef GL14VERSION elif EZEALVERSION
 
 #endif		//PLATFORM_HEADER_INCLUDED
