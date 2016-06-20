#!/bin/sh

#------------------------------------------
# 램디스크를 만든다. 
#------------------------------------------
RAMDISK=ramdisk.imx6-2.0-32M.gz
TARGET_ROOT=target_ramdisk
DIR_RAMDISK=dir_ramdisk
ROOT_FILE=../../buildroot-2015.02/output/images/rootfs.tar


if [-f ${ROOT_FILE}]; then
   echo "tar xf ${ROOT_FILE} -C ${TARGET_ROOT}"
   tar xf ${ROOT_FILE} -C ${TARGET_ROOT}
else
  echo "no such file ${ROOT_FILE}"
  exit 1
fi

/sbin/losetup -d /dev/loop1
rm -rf $RAMDISK
dd if=/dev/zero of=t_ramdisk bs=1k count=32768
/sbin/losetup /dev/loop1 t_ramdisk
/sbin/mke2fs /dev/loop1 
/sbin/losetup -d /dev/loop1
mount -t ext2 -o loop t_ramdisk dir_ramdisk


if [-d ${TARGET_ROOT}]; then
    cp -a target_ramdiskroot/* dir_ramdisk
else

    mkdir ${TARGET_ROOT} ${DIR_RAMDISK}
    cp -a ${TARGET_ROOT}/* ${DIR_RAMDISK}
fi

umount dir_ramdisk
gzip t_ramdisk
mv t_ramdisk.gz $RAMDISK 
chmod 777 $RAMDISK

/sbin/losetup -d /dev/loop1

cp -a $RAMDISK ramdisk.imx6.gz
cp -a $RAMDISK /tftpboot/
