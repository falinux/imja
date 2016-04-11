#!/bin/sh

#--------------------------------
# configs/mx6qterra_defconfig
#
# DDR Clock 400MHz
# 	IMX_CONFIG=board/falinux/mx6qterra/mx6qterra.cfg,MX6Q,DDR_400MHZ,FALINUX,TERRA
#
# DDR Clock 528MHz
# 	IMX_CONFIG=board/falinux/mx6qterra/mx6qterra.cfg,MX6Q,FALINUX,TERRA
#--------------------------------

defconfig="mx6qterra_defconfig"
image_filename="u-boot.imx"

if [ ! -f .config ]; then
    ARCH=arm CROSS_COMPILE=/opt/gcc-linaro-arm-linux-gnueabihf-4.8-2014.04_linux/bin/arm-linux-gnueabihf- make ${defconfig}
fi

if [ "$1" = "" ]; then
    ARCH=arm CROSS_COMPILE=/opt/gcc-linaro-arm-linux-gnueabihf-4.8-2014.04_linux/bin/arm-linux-gnueabihf- make
else
    ARCH=arm CROSS_COMPILE=/opt/gcc-linaro-arm-linux-gnueabihf-4.8-2014.04_linux/bin/arm-linux-gnueabihf- make $1 $2 $3
fi

if [ -f $image_filename ]; then
    echo "copy from $image_filename to /tftpboot/$target_filename"
    sudo cp  $image_filename /tftpboot/$target_filename
    sudo chmod 777 /tftpboot/$target_filename
fi

