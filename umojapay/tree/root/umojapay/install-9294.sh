#!/bin/sh

PATH=$PATH:/bin:/sbin:/usr/sbin

make -f makefile-9294 clean
make -f makefile-9294
make -f makefile-9294 image

#cp bin/umojapay_9294.img /media/sf_Shared/dev/
