#!/bin/sh

#------------------------------------------
# 램디스크를 만든다. 
#------------------------------------------
RAMDISK=ramdisk.imx6-2.0-32M.gz

/sbin/losetup -d /dev/loop1
rm -rf $RAMDISK
dd if=/dev/zero of=t_ramdisk bs=1k count=32768
/sbin/losetup /dev/loop1 t_ramdisk
/sbin/mke2fs /dev/loop1 
/sbin/losetup -d /dev/loop1
mount -t ext2 -o loop t_ramdisk dir_ramdisk
#ldconfig -r target_ramdiskroot
cp -a target_ramdiskroot/* dir_ramdisk
umount dir_ramdisk
gzip t_ramdisk
mv t_ramdisk.gz $RAMDISK 
chmod 777 $RAMDISK

/sbin/losetup -d /dev/loop1

cp -a $RAMDISK ramdisk.imx6.gz
cp -a $RAMDISK /tftpboot/
