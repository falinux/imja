#!/bin/sh

build_path="../build_linux_3.10.53"
module_install="../out"
firmware_install="../out"
image_filename="$build_path/arch/arm/boot/uImage"
target_filename="uImage.imx6_imja"
dtb_path="$build_path/arch/arm/boot/dts"
dtb_filename="imx6q-terra-imja.dtb"
#dtb_filename="imx6q-nitrogen6x.dtb"
CROSS_PATH="/opt/gcc-linaro-arm-linux-gnueabihf-4.9-2014.09_linux"
CROSS_PREFIX="arm-linux-gnueabihf-"

if [ -f .config ]; then
	echo ".....mrproper"
	make mrproper
fi

if [ ! -d $build_path ]; then
	mkdir $build_path
	chmod 777 $build_path
fi


#if [ ${PATH%%:*} != "/usr/gcc-linaro-arm-linux-gnueabihf-4.8-2014.03_linux/bin" ]; then
#    echo "$PATH"
#    echo ">> source env.sh <<"
#fi

if [ ! -f $build_path/.config ]; then
	echo ".....imx6 SIB-IMJA defconfig"	
	ARCH=arm make arm=ARM CROSS_COMPILE=${CROSS_PATH}/bin/${CROSS_PREFIX} O=$build_path distclean 
#	ARCH=arm make arm=ARM CROSS_COMPILE=${CROSS_PATH}/bin/${CROSS_PREFIX} O=$build_path imx_v7_terra_defconfig
	ARCH=arm make arm=ARM CROSS_COMPILE=${CROSS_PATH}/bin/${CROSS_PREFIX} O=$build_path imx_v7_imja_defconfig
#	ARCH=arm make arm=ARM CROSS_COMPILE=${CROSS_PATH}/bin/${CROSS_PREFIX} O=$build_path nitrogen6x_defconfig
fi


if [ "$1" = "" ]; then
	ARCH=arm make arm=ARM CROSS_COMPILE=${CROSS_PATH}/bin/${CROSS_PREFIX} O=$build_path -j4 uImage LOADADDR=0x10008000
else
    ARCH=arm make arm=ARM CROSS_COMPILE=${CROSS_PATH}/bin/${CROSS_PREFIX} O=$build_path $1 $2 $3 LOADADDR=0x10008000
fi

if [ "$1" = "dtb" ]; then
	ARCH=arm make arm=ARM CROSS_COMPILE=${CROSS_PATH}/bin/${CROSS_PREFIX} O=$build_path dtbs
fi


# build kernel modules
if [ "$1" = "modules" ] ; then
	ARCH=arm INSTALL_MOD_PATH=$module_install make arm=ARM CROSS_COMPILE=${CROSS_PATH}/bin/${CROSS_PREFIX} O=$build_path modules
	ARCH=arm INSTALL_MOD_PATH=$module_install make arm=ARM CROSS_COMPILE=${CROSS_PATH}/bin/${CROSS_PREFIX} O=$build_path modules_install

	ARCH=arm INSTALL_MOD_PATH=$firmware_install make arm=ARM CROSS_COMPILE=${CROSS_PATH}/bin/${CROSS_PREFIX} O=$build_path firmware_install
fi

if [ -f $image_filename ]; then
   echo "copy from image $image_filename to /tftpboot/$target_filename"
   sudo cp  $image_filename /tftpboot/$target_filename
   echo "copy from dtb $dtb_path/$dtb_filename to /tftpboot/$dtb_filename"
   sudo cp  $dtb_path/$dtb_filename /tftpboot/$dtb_filename
   sudo chmod 777 /tftpboot/$target_filename
fi

