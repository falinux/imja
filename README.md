**[Imja솔루션]**
------------------------------------------------------------------------------------------------------------------------
소스 다운로드

    $ git clone http://192.168.10.12/erunsee/1601_0033_imja_sol.git

or

    $ git clone git@192.168.10.12:erunsee/1601_0033_imja_sol.git

위의 경우에는 gitlab에 ssh키가 등록 되어 있어야 합니다.


   

git디렉토리의 구조는 아래와 같습니다.


    bootloader           <---- 부트로더
    driver               <---- 드라이버(WiFi)
    filesystem           <---- 파일 시스템
    kernel               <---- 커널소스
    toolchain            <---- 툴체인
    images               <---- 최신 이미지가 들어 있음



**[toolchain]**
------------------------------------------------------------------------------------------------------------------------
이 디렉토리는 툴체인이 들어 있습니다. 이 툴체인을 받아 /opt디렉토리에서 압축을 풀어 놓습니다.

        $ sudo cp ~/다운로드폴더/ gcc-linaro-arm-linux-gnueabihf-4.9-2014.09_linux.tgz
        $ cd /opt
        $ sudo tar xvf  gcc-linaro-arm-linux-gnueabihf-4.9-2014.09_linux.tgz
        $ ls
        gcc-linaro-arm-linux-gnueabihf-4.9-2014.09_linux




**[bootloader]**
------------------------------------------------------------------------------------------------------------------------
bootloader에는 u-boot디레토리가 있습니다. 툴체인이 설치되었다는 전체 아래 아래와 같이 빌드 합니다.

      $ cd bootloder/u-boot
      $ sudo ./make-imx6.sh


컴파일이 끝나면 u-boot이미지는 /tftpboot/u-boot.imx6.imja로 복사가 된다.




**[kernel]**
------------------------------------------------------------------------------------------------------------------------

커널 빌드

    $ cd kernel/linux-imx6_3.10.53
    $ ./make-imx6.sh

컴파일 된 이미지는 /tftpboot/uImage.imx6.imja 로 복사가 된다.


모듈 빌드

    $ cd kernel/linux-imx6_3.10.53
    $ ./make-imx6.sh modules
     ...
    $ cd ../out


out디렉토리 안에 빌드된 모듈과 펌웨어가 들어 있다 모듈만 필요 할 경우 펌웨어 디렉토리는 삭제 해 둔다.

**※ out디렉토리는 램디스크 만들때 out디렉토리 안에 있는 내용을 모두 복사하여 램디스크를 만드므로 주의 해야 한다.**



**[driver]**
------------------------------------------------------------------------------------------------------------------------
드라이버 디렉토리에는 WiFi용 툴및 드라이버가 있습니다. 드라이버는 커널에 모듈로 포함 시켜놓은 상태여서 툴만 컴파일 하면되고 그 외의 WiFi에서 필요한 정보를 참고 하시면 됩니다.


WiFi관련 문서

     $ cd driver/RTL8188EUS_linux_v4.3.0.8_13968.20150417
     $ cd document
     $ ls -l
     -rw-r--r-- 1 falbb falbb  42974  2월 21  2014 HowTo_enable_driver_to_support_80211d.pdf
     -rw-r--r-- 1 falbb falbb  16929  2월 21  2014 HowTo_enable_driver_to_support_WIFI_certification_test.pdf
     -rw-r--r-- 1 falbb falbb 154971  2월 21  2014 HowTo_enable_the_power_saving_functionality.pdf
     -rw-r--r-- 1 falbb falbb  15821  2월 21  2014 HowTo_support_more_VidPids.pdf
     -rw-r--r-- 1 falbb falbb 128554  2월 21  2014 Miracast_for_Realtek_WiFi.pdf
     -rw-r--r-- 1 falbb falbb  16779  2월 21  2014 Quick_Start_Guide_for_Bridge.pdf
     -rw-r--r-- 1 falbb falbb  68773  2월 21  2014 Quick_Start_Guide_for_Driver_Compilation_and_Installation.pdf
     -rw-r--r-- 1 falbb falbb  39476  2월 21  2014 Quick_Start_Guide_for_SoftAP.pdf
     -rw-r--r-- 1 falbb falbb  36192  2월 21  2014 Quick_Start_Guide_for_Station_Mode.pdf
     -rw-r--r-- 1 falbb falbb  61385  2월 21  2014 Quick_Start_Guide_for_WOW.pdf
     -rw-r--r-- 1 falbb falbb 128897  2월 21  2014 RTK_P2P_WFD_Programming_guide.pdf
     -rw-r--r-- 1 falbb falbb 174071  2월 21  2014 Realtek_WiFi_concurrent_mode_Introduction.pdf
     -rw-r--r-- 1 falbb falbb  29988  2월 21  2014 SoftAP_Mode_features.pdf
     -rw-r--r-- 1 falbb falbb  13447  2월 21  2014 Wireless_tools_porting_guide.pdf
     -rw-r--r-- 1 falbb falbb   4190  2월 21  2014 linux_dhcp_server_notes.txt
     -rw-r--r-- 1 falbb falbb  41564  2월 21  2014 wpa_cli_with_wpa_supplicant.pdf




**[filesystem]**
------------------------------------------------------------------------------------------------------------------------
파일 시스템은 buildroot를 이용하여 루트 파일을 생성후, 램디스크를 만드는 디렉토리 입니다.

램디스크에 추가 할 파일 이 있을 경우에는 다음의 디렉토리에 파일을 배치 하도록 한다.

    $ cd filesystem/buildroot-2015.02/board/falinux/imja/rootfs_overlay

현재 파일 구조는 다음과 같습니다.

        ├── bin
        │   └── cmd_parsing
        ├── etc
        │   ├── init.d
        │   │   ├── S10mdev
        │   │   ├── S80wireless
        │   │   └── S90falinux
        │   ├── mdev
        │   │   └── automount.sh
        │   ├── mdev.conf
        │   ├── passwd
        │   ├── securetty
        │   ├── shadow
        │   └── wpa_supplicant.conf
        ├── home
        │   └── falinux
        ├── lib
        ├── mnt
        │   └── mmcblk3p1
        └── root
            ├── .mmc_patiton.sh
            ├── .mmc_patiton_fusing.sh
            ├── .temp.sh
            ├── mmc_patiton.sh
            ├── nfsmnt
            └── temp.sh



루트 파일시스템 빌드

     $ cd filesystem/buildroot-2015.02
     $ ./make-imja.sh


램디스크 만들기


루트파일 시스템을 만든후 실행 해야 함.

    $ cd filesystem/ramdisk
    $ sudo ./make-imja-ramdisk.sh
    $ ls -l
     drwxr-xr-x  4 falbb falbb     4096  6월 22 16:21 ./
     drwxr-xr-x  4 falbb falbb     4096  6월 22 13:50 ../
     drwxr-xr-x 19 root  root      4096  6월 22 15:53 .rootfs_tmp/
     -rwxrwxrwx  1 falbb falbb     2224  6월 22 14:09 make-imja-ramdisk.sh*
     -rwxrwxrwx  1 falbb falbb     6268  6월 22 13:26 mk_ramfs.sh*
     -rw-rw-rw-  1 root  root  11982113  6월 22 15:54 ramdisk.imx6-imja-2.0-32M.gz
     -rw-r--r--  1 root  root  11174952  6월 22 15:54 rootfs.tar.gz
     -rw-r--r--  1 root  root  11657681  6월 22 15:54 rootfs_20160622.tar.gz

 

실행이 끝나면 /tftpboot/ramdisk.imx6-imja-2.0-32M.gz로 복사가 된다.


테스트용 으로 파일을 추가 할 경우에는 add_file디렉토리를 만들어 추가 한다. add_file디렉토리가 루트이므로 그에 맞게 파일을 배치 한 다음 make-imja-ramdisk.sh를 실행 한다.




**[images]**
------------------------------------------------------------------------------------------------------------------------
최신 커널, u-boot, dtb, 램디스크 이미지가 들어 있는 디렉토리 입니다.


