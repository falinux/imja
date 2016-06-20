#!/bin/sh

image_filename="uboot"
target_filename="uboot.imx.imja"
CROSS_PATH="/opt/gcc-linaro-arm-linux-gnueabihf-4.9-2014.09_linux"
CROSS_PREFIX="arm-linux-gnueabihf-"


if [ ! -f .config ]; then
	echo ".....imx6 imja defconfig"	
	ARCH=arm make arm=ARM CROSS_COMPILE=${CROSS_PATH}/bin/${CROSS_PREFIX} distclean 
	ARCH=arm make arm=ARM CROSS_COMPILE=${CROSS_PATH}/bin/${CROSS_PREFIX} falinux_imx6q_imja_defconfig
fi


if [ "$1" = "" ]; then
	ARCH=arm make arm=ARM CROSS_COMPILE=${CROSS_PATH}/bin/${CROSS_PREFIX} 
fi

if [ -f $image_filename ]; then
   echo "copy from image $image_filename to /tftpboot/$target_filename"
   sudo cp  $image_filename /tftpboot/$target_filename
   echo "copy from dtb $dtb_path/$dtb_filename to /tftpboot/$dtb_filename"
   sudo cp  $dtb_path/$dtb_filename /tftpboot/$dtb_filename
   sudo chmod 777 /tftpboot/$target_filename
fi

