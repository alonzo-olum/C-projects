#!/bin/sh

PATH=$PATH:/bin:/sbin:/usr/sbin

make -f makefile-ezeal clean
make -f makefile-ezeal
cd bin/

eZpm umojapay_ezeal
#cp umojapay_ezeal.eZpm /media/sf_Shared/dev/
