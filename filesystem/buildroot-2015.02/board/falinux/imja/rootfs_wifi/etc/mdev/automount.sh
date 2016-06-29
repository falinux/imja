#!/bin/sh

MNTPATH=$1
LOGPATH=/dev/console

if [ "$1" == "" ]; then
	echo "automount.sh parameter is none" > $LOGPATH
	exit 1
fi

mounted=`mount | grep $MNTPATH`
MNTPATH=$2

if [ "$mounted" = "" ]; then

	echo "Mount : $1 -> $MNTPATH" > $LOGPATH
	mkdir -p $MNTPATH
	if [ $1 == "mmcblk3p1" ]; then
		mount -v -t ext4 -o data=journal /dev/$1 $MNTPATH
	else	
		mount /dev/$1 $MNTPATH
	fi	
	wait
	echo "Done  : mount $MNTPATH" > $LOGPATH
else

	echo "Umount: $MNTPATH" > $LOGPATH
	umount $MNTPATH
	wait
	rmdir  $MNTPATH
	echo "Done  : umount $MNTPATH" > $LOGPATH
fi

exit 0

