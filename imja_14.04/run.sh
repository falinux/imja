MOUNT_DIR="/mnt/mmc"
IMAGE_PATH="images"
UBOOT_IMAGE="u-boot.imx6_imja"
KERNEL_IMAGE="uImage.imx6_imja"
RAMDISK_IMAGE="ramdisk.imx6-2.0-32M.gz"

DEV_NAME=$1
FS_NAME=$2

if [ "$1" = "" ]; then
        echo "** mmc fussing "
        echo "Usage : $0 [DEV_NAME]"
        echo "exam  : ./run /dev/sdb/ ImJa_XXXXXXX.tar"
        exit
fi

if [ "$2" = "" ]; then
        echo "** mmc fussing "
        echo "Usage : $0 [DEV_NAME]"
        echo "exam  : ./run /dev/sdb/ ImJa_XXXXXXX.tar"
        exit
fi

str=`udevadm info --query=all -n $1 | grep ID_BUS`
#usb=${str##*ID_BUS=}
usb='usb'
#echo "$str"
#echo "$usb"


if [ "$usb" = "" ]; then
        echo "** $1 Device Node Not Found "
        exit 0
fi

if [ "$usb" = "usb" ]; then
        echo "** $1 is a USB "

        echo umount "$1"1
        umount "$1"1
        sleep 1

        echo "$1 512 byte region zero set"
        dd if=/dev/zero of=${DEV_NAME} bs=512 count=1

       echo "DEL PARTITION"
       (echo p; echo d; echo w) | fdisk ${DEV_NAME}

        echo "FORMAT USB"
        (echo p; echo n; echo p; echo 1; echo 131072; echo ; echo w) | fdisk ${DEV_NAME}

        echo "Format ext4 ${DEV_NAME}1"
        sleep 1
        mkfs.ext4 ${DEV_NAME}1 -L rootfs

        echo "Input U-Boot"
        echo "${IMAGE_PATH}/${UBOOT_IMAGE}"
        sleep 1
		if [ -e ${IMAGE_PATH}/${UBOOT_IMAGE} ]; then
            dd if=/dev/zero of=${DEV_NAME} bs=512 seek=2 count=2046
            dd if=${IMAGE_PATH}/${UBOOT_IMAGE} of=${DEV_NAME} bs=512 seek=2 && sync
		else
		   echo "U-Boot File Not Found!!!"
		   
		fi

        echo "Input Kernel"
        sleep 1
		if [ -e ${IMAGE_PATH}/${KERNEL_IMAGE} ]; then
			dd if=${IMAGE_PATH}/${KERNEL_IMAGE} of=${DEV_NAME} bs=512 seek=4096 && sync
		else
			echo "Kernel File Not Found!!!"
		fi
		
        echo "Input ramdisk"
        sleep 1
		
		if [ -e ${IMAGE_PATH}/${KERNEL_IMAGE} ]; then
			dd if=${IMAGE_PATH}/${RAMDISK_IMAGE} of=${DEV_NAME} bs=512 seek=16384 && sync
		else
			echo "Ramdisk File Not Found!!!"
		fi

        echo "FileSystem unzip........!!!"
        echo -e "mount ${DEV_NAME}1 $MOUNT_DIR"
        mount ${DEV_NAME}1 $MOUNT_DIR
        
        echo -e "tar -pxvf $RFS -C $MOUNT_DIR"
        tar  -pxvf $2 -C $MOUNT_DIR

        ls $MOUNT_DIR

        echo -e " rm $MOUNT_DIR/etc/udev/rules.d/70-persistent-*"
        rm $MOUNT_DIR/etc/udev/rules.d/70-persistent-* 

        echo -e "umount $MOUNT_DIR"
        umount $MOUNT_DIR




        echo "Make MMC Complete!!!"
else
        echo "** $1 is Not a USB "
        exit 0
fi


