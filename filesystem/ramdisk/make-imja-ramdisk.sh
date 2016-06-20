#!/bin/sh

NOW=$(date +"%Y%m%d")
FILE="rootfs_$NOW.tar.gz"
BUILD_ROOT_PATH="buildroot-2015.02"
RAMDISK_FILE=ramdisk.imx6-imja-2.0-32M

echo ""
echo "============================================================================="
echo "rootfile-system start...."
echo ""

sudo rm -rf ./.rootfs_tmp
sudo rm -rf ./view_rootfs

if [ -d ./rootfs ]; then
	sudo cp -a ./rootfs ./.rootfs_tmp
else
	if [ ! -f ./rootfs.tar.gz ]; then
		echo " > rootfs.tar.gz file find error...."
		echo " >   ls ../output/images/rootfs.tar.gz"
		echo " >   cp ../output/images/rootfs.tar.gz ./"
                echo " >   cp ../${BUILD_ROOT_PATH}/output/images/rootfs.tar.gz ./"
		echo "============================================================================="
                cp ../${BUILD_ROOT_PATH}/output/images/rootfs.tar.gz ./
	fi

	mkdir ./.rootfs_tmp
	chmod 777 ./.rootfs_tmp
	sudo tar -zxf rootfs.tar.gz -C ./.rootfs_tmp/
fi

sudo rm -rf ./add_file/os-release

if [ -d ../../kernel/out/lib ]; then
	sudo cp -a ../../kernel/out/lib/* ./.rootfs_tmp/lib/
else
	echo ""
	echo "-------------------------------------"
	echo " kernel/out/lib not found!!!"
	echo "-------------------------------------"
	echo ""
fi

sudo cp -a ./add_file/* ./.rootfs_tmp/

if [ -d ./.rootfs_tmp/mnt/nfs ]; then
	sudo chmod 777 ./.rootfs_tmp/mnt/nfs
else	
	sudo mkdir ./.rootfs_tmp/mnt/nfs
	sudo chmod 777 ./.rootfs_tmp/mnt/nfs
fi

if [ -f ./.rootfs_tmp/etc/os-release ]; then
	sudo rm ./.rootfs_tmp/etc/os-release
fi

#----------------------------
#----------------------------
sleep 1

#---------------------------
# view rootfs 
cp -a ./.rootfs_tmp ./view_rootfs

sudo chown root ./.rootfs_tmp -R
sudo chgrp root ./.rootfs_tmp -R

echo ""
echo "rootfile-system up data to [ rootfs_$NOW.tar.gz ] file, please wait..."
OWD=`pwd`
cd ./.rootfs_tmp
sudo tar -zcf ../${FILE} *
cd ${OWD}

#---------------------------
# delete 
if [ -d ./rootfs ]; then
	sudo rm -rf ./rootfs
fi

./mk_ramfs.sh -r ./view_rootfs -n ${RAMDISK_FILE} -c /tftpboot/

echo ""
echo "rootfile-system end...."
echo "============================================================================="
echo ""

