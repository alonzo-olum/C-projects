 #include "global.h"
 #include "commonfuncs.h"
 #include <stdio.h>
 #include <stdlib.h>
 
 #include "platform.h"
 #include "common.h"
 
 static char string_buf[32];

 void download_entry()
 {
 	//TODO:
 	//1. Post Serial Number and Software Version to see if it is up to date.
 	//2. Process server response and determine whether we are to initiate a download
 	//3. Download the file through a curl POST or GET.
 	//4. If successful run downloadapplication.
 	
 	//<> do_ftp_download(url, filename) is already up.
 	//<> todo: FTP site for downloading.
 }
