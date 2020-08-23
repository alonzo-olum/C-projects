#!/bin/sh

PATH=$PATH:/bin:/sbin:/usr/sbin

make -f makefile-gl14 clean
make -f makefile-gl14
make -f makefile-gl14 image

#cp bin/umojapay_gl14.img /media/sf_Shared/dev/
