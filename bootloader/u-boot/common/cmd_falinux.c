/*

	커널이미지는 uImage 로처리하고
	ramdisk 는 uImage 가 아니기 때문에 직접 해당하는 주소에 넣는다.


	todo
		env 위치수정
		bootargs_ezb 생성
		bootcmd_ezb 생성 
		load default 구현
		tfk, tfr, tfb, tfd, rst 명령어 생성

*/


#include <common.h>
#include <command.h>
#include <exports.h>

DECLARE_GLOBAL_DATA_PTR;

enum {
	MMC_SECTOR_BOOT    = 0x2,
	MMC_SECTOR_ENV     = 0x900,
	MMC_SECTOR_DT      = 0xA00,
	MMC_SECTOR_KERNEL  = 0x1000,
	MMC_SECTOR_RAMDISK = 0x7000,
	MMC_SECTOR_LOGO    = 0xF000
};

enum {
	MMC_SSIZE_BOOT    = 0x800,	// max 0x8FE
	MMC_SSIZE_ENV     = 0x20,	// max 0x100
	MMC_SSIZE_DT      = 0x100,	// max 0x600
	MMC_SSIZE_KERNEL  = 0x4000, // max 0x5000
	MMC_SSIZE_RAMDISK = 0x4000, // max 0x8000 
	MMC_SSIZE_LOGO    = 0x1000  // max 0x8000  
};


#define	DRAM_DOWNLOAD_BASE	0x10000000	// 
#define	DRAM_DTREE_BOOTM	0x11F00000	
#define	DRAM_KERNEL_BOOTM	0x12000000	
#define	DRAM_RAMDISK_ADDR	0x1E000000		// cmdline 과도 연결된다.

//static char ezb_boot_args[256] =                                
//	" console=ttymxc0,115200"                    				
// 	" root=/dev/ram0 rw --no-log initrd=0x1e000000,16M ramdisk=32768"     
// 	" video=mxcfb0:dev=hdmi,1920x1080@60,if=RGB24 vmalloc=192M"  
// 	;

#define KEY_LOAD_K		"ezb_load_k"
#define KEY_LOAD_R		"ezb_load_r"
#define KEY_LOAD_DT		"ezb_load_dt"
#define KEY_BOOTM		"ezb_bootm"
#define KEY_BOOTARGS	"ezb_bootargs"

enum {
	F_IDX_ETHADDR   = 0,
	F_IDX_IPADDR     ,
	F_IDX_NETMASK    ,
	F_IDX_GATEWAYIP  ,
	F_IDX_SERVERIP   ,
	F_IDX_KERNEL     ,
	F_IDX_RAMDISK    ,
	F_IDX_UBOOT      ,
	F_IDX_DT         ,
	F_IDX_AUTOEXEC   ,
	F_IDX_BOOTDELAY  ,
	F_IDX_KCMD1      ,
	F_IDX_KCMD2      ,
	F_IDX_KCMD3      ,
	F_IDX_KCMD4      ,
	F_IDX_KCMD5      ,
	F_IDX_COUNT
};

//	F_IDX_MMC_SECTOR ,
//	F_IDX_MMC_SSIZE  ,
//	F_IDX_BOOTARGS   ,

static char ezb_key[F_IDX_COUNT][32] = 
{
	"ethaddr"       ,
	"ipaddr"        ,
	"netmask"       ,
	"gatewayip"     ,
	"serverip"      ,
	"ezb_uImage"    ,
	"ezb_ramdisk"   ,
	"ezb_uboot"     ,
	"ezb_dt"        ,
	"ezb_autoexec"  ,
	"bootdelay"     ,
	"ezb_KCMD1"     ,
	"ezb_KCMD2"     ,
	"ezb_KCMD3"     ,
	"ezb_KCMD4"     ,
	"ezb_KCMD5"     ,
};


static char ezb_def_str[F_IDX_COUNT][80] = 
{
	"00:fa:20:15:A5:00"       ,
	"192.168.10.235"          ,
	"255.255.255.0"           ,
	"192.168.10.1"            ,
	"192.168.10.30"           ,
	"uImage.imx6"             ,
	"ramdisk.imx6-1.0-32M.gz" ,
	"uboot.imx6dq"            ,
	"dt.imx6dq"               ,
	"/app/run.sh"             ,
	"2",
	"console=ttymxc0,115200 --no-log"              				,
 	"root=/dev/ram0 rw initrd=0x1e000000,16M ramdisk=32768"     ,
 	"video=mxcfb0:dev=hdmi,1920x1080@60,if=RGB24 vmalloc=192M"  ,
 	" "                                                  ,
 	" "                                                         ,
};

static char fmt_menu[] = 
	"\nver 0.1.0\n\n"
	"  1) mac address              : %s\n"
	"  2) local ip                 : %s\n"
	"  3) local netmask            : %s\n"
	"  4) local gateway            : %s\n"
	"  5) server ip                : %s\n"
	"  6) uImage      file name    : %s\n"
	"  7) uRamdisk    file name    : %s\n"
	"  8) uBoot       file name    : %s\n"
	"  9) DeviceTree  file name    : %s\n"
	" 10) auto execute full name   : %s\n"
	" 11) autoboot wait time       : %s\n"
	" 12) KCMD1 : %s\n"
	" 13) KCMD2 : %s\n"
	" 14) KCMD3 : %s\n"
	" 15) KCMD4 : %s\n"
	" 16) KCMD5 : %s\n"
	"\n"
	"  L)  Load default               \n"
	"      LF) load cmdline root=flash  \n"
	"      LR) load cmdline root=ramdisk\n"
	"  M)  generlate Mac address      \n"
	"  S)  Save to flash              \n"
	"  Q)  Quit with apply            \n"
	"\n"
	;

//------------------------------------------------------------------------------
/** @brief
 *///---------------------------------------------------------------------------
static char *get_ezb_strv( int f_idx )
{
	char *pf;
	
	pf = getenv( ezb_key[f_idx] );	
	if ( NULL == pf ) 
	{
		setenv( ezb_key[f_idx], ezb_def_str[f_idx] );
		pf = getenv( ezb_key[f_idx] );	
	}

	return pf;
} 

//------------------------------------------------------------------------------
/** @brief
 *///---------------------------------------------------------------------------
static void setenv_common( void )
{
	char  str_env[CONFIG_SYS_CBSIZE];
	
	sprintf( str_env, "setenv bootargs ${%s} ${%s} ${%s} ${%s} ${%s}", 
		ezb_key[F_IDX_KCMD1], ezb_key[F_IDX_KCMD2], ezb_key[F_IDX_KCMD3], ezb_key[F_IDX_KCMD4], ezb_key[F_IDX_KCMD5] ); 
	setenv( KEY_BOOTARGS, str_env );
	
	setenv( "bootcmd", "run "KEY_BOOTARGS" "KEY_BOOTM );
}

//------------------------------------------------------------------------------
/** @brief
 *///---------------------------------------------------------------------------
static int do_set(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int   nbytes, menu;
	
	setenv_common();
	
	while( 1 )
	{
		printf( fmt_menu, 
			get_ezb_strv( F_IDX_ETHADDR  ),
			get_ezb_strv( F_IDX_IPADDR   ),
			get_ezb_strv( F_IDX_NETMASK  ),
			get_ezb_strv( F_IDX_GATEWAYIP),
			get_ezb_strv( F_IDX_SERVERIP ),
			get_ezb_strv( F_IDX_KERNEL   ),
			get_ezb_strv( F_IDX_RAMDISK  ),
			get_ezb_strv( F_IDX_UBOOT    ),
			get_ezb_strv( F_IDX_DT       ),
			get_ezb_strv( F_IDX_AUTOEXEC ),
			get_ezb_strv( F_IDX_BOOTDELAY),
			get_ezb_strv( F_IDX_KCMD1    ),
			get_ezb_strv( F_IDX_KCMD2    ),
			get_ezb_strv( F_IDX_KCMD3    ),
			get_ezb_strv( F_IDX_KCMD4    ),
			get_ezb_strv( F_IDX_KCMD5    )
		);

		// 입력 대기
		nbytes =cli_readline ("  select> ");
		if ( 0 < nbytes ) 
		{
			char *ptr = console_buffer;
				 
			switch ( *ptr )
			{
			case 's' : case 'S' : saveenv(); break;
			case 'q' : case 'Q' : return 0;
			}

			// 숫자 메뉴 확인
			menu = simple_strtoul( ptr, NULL, 0 );
			menu -= 1; 
			
			if ( ( 0 <= menu ) && ( menu < F_IDX_COUNT ) )
			{
     			char buffer[CONFIG_SYS_CBSIZE];
     			char *init_val;
				
				init_val = getenv( ezb_key[menu] );
     			if (init_val)
     			    sprintf(buffer, "%s", init_val );
     			else
     			    buffer[0] = '\0';
 
     			if ( 0 <= cli_readline_into_buffer( "  edit> ", buffer, 0 ) )
     			{
     				setenv( ezb_key[menu], buffer );
     			}
			}	
		}
	}	
	
	return 0;
}

U_BOOT_CMD(
	set,	CONFIG_SYS_MAXARGS,	1,	do_set,
	"set",
	"legacy ezboot command [set]"
);


//------------------------------------------------------------------------------
/** @brief
*///---------------------------------------------------------------------------
#define MMC_ERASE_SIZE		(4096)
#define CALC_MMC_SECTOR(b)	((b+MMC_ERASE_SIZE-1)>>9)	// div 512

static int download_by_tftp( char *fname )
{
	char str_cmd[CONFIG_SYS_CBSIZE];

	sprintf( str_cmd, "tftpboot 0x%x %s",
		DRAM_DOWNLOAD_BASE,
		fname
	);
	run_command( str_cmd, 0 );
	
	return (int)NetBootFileXferSize;
}
//------------------------------------------------------------------------------
/** @brief
*///---------------------------------------------------------------------------
static void write_to_mmc( int sector, int ssize )
{
	char str_cmd[CONFIG_SYS_CBSIZE];

	sprintf( str_cmd, "mmc write 0x%x 0x%x 0x%x",
		DRAM_DOWNLOAD_BASE,
		sector,
		ssize
	);
	run_command( str_cmd, 0 );
}



//------------------------------------------------------------------------------
/** @brief
 *///---------------------------------------------------------------------------
static void saveenv_after_download( void )
{
	char  str_env[CONFIG_SYS_CBSIZE];

	// env, bootm
	sprintf( str_env, "mmc dev 0; mmcinfo; %s; %s; bootm 0x%x",
		getenv( KEY_LOAD_K ),
		getenv( KEY_LOAD_R ),
		DRAM_KERNEL_BOOTM
	);			
	setenv( KEY_BOOTM, str_env );

	setenv_common();
	
	saveenv();
}

//------------------------------------------------------------------------------
/** @brief
 *///---------------------------------------------------------------------------
static int do_ezb_cmd(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	char  str_env[CONFIG_SYS_CBSIZE];
	int   fsize, ssize;
	
	if ( 0 == strcmp( "rst", argv[0] ) )
	{
		run_command( "reset", 0 );
	}
	if ( 0 == strcmp( "gk", argv[0] ) )
	{
		run_command( "run bootcmd", 0 );
	}
	else if ( 0 == strcmp( "tfk", argv[0] ) )
	{
		// download and write
		fsize = download_by_tftp( get_ezb_strv( F_IDX_KERNEL ) );
		ssize = CALC_MMC_SECTOR(fsize);
		write_to_mmc( MMC_SECTOR_KERNEL, ssize );

		// env, load kernel
		sprintf( str_env, "mmc read 0x%x 0x%x 0x%x", DRAM_KERNEL_BOOTM, MMC_SECTOR_KERNEL, ssize );	
		setenv( KEY_LOAD_K, str_env );

		// save env
		saveenv_after_download();
	}
	else if ( 0 == strcmp( "tfr", argv[0] ) )
	{
		// download and write
		fsize = download_by_tftp( get_ezb_strv( F_IDX_RAMDISK ) );
		ssize = CALC_MMC_SECTOR(fsize);
		write_to_mmc( MMC_SECTOR_RAMDISK, ssize );

		// env, load ramdisk
		sprintf( str_env, "mmc read 0x%x 0x%x 0x%x", DRAM_RAMDISK_ADDR, MMC_SECTOR_RAMDISK, ssize );	
		setenv( KEY_LOAD_R, str_env );
		
		// save env
		saveenv_after_download();
	}
	else if ( 0 == strcmp( "tfd", argv[0] ) )
	{
		// download and write
		fsize = download_by_tftp( get_ezb_strv( F_IDX_DT ) );
		ssize = CALC_MMC_SECTOR(fsize);
		write_to_mmc( MMC_SECTOR_DT, ssize );

		// env, load dt
		sprintf( str_env, "mmc read 0x%x 0x%x 0x%x", DRAM_DTREE_BOOTM, MMC_SECTOR_DT, ssize );	
		setenv( KEY_LOAD_DT, str_env );
		
		// save env
		saveenv_after_download();
	}
	else if ( 0 == strcmp( "tfb", argv[0] ) )
	{
		fsize = download_by_tftp( get_ezb_strv( F_IDX_UBOOT ) );
		ssize = CALC_MMC_SECTOR(fsize);
		
		write_to_mmc( MMC_SECTOR_BOOT, ssize );
	}
	
	return 0;
}

U_BOOT_CMD(
	tfk,	CONFIG_SYS_MAXARGS,	0,	do_ezb_cmd,
	"tfk", 
	"download <uimage> by tftpboot then write on flash or mmc"
);

U_BOOT_CMD(
	tfr,	CONFIG_SYS_MAXARGS,	0,	do_ezb_cmd,
	"tfr", 
	"download <ramdisk> by tftpboot then write on flash or mmc"
);

U_BOOT_CMD(
	tfd,	CONFIG_SYS_MAXARGS,	0,	do_ezb_cmd,
	"tfd", 
	"download <dt> by tftpboot then write on flash or mmc"
);

U_BOOT_CMD(
	tfb,	CONFIG_SYS_MAXARGS,	0,	do_ezb_cmd,
	"tfb for bootloader", 
	"download <uboot> by tftpboot then write on flash or mmc"
);

U_BOOT_CMD(
	rst,	CONFIG_SYS_MAXARGS,	0,	do_ezb_cmd,
	"rst", 
	"reset"
);

U_BOOT_CMD(
	gk,	CONFIG_SYS_MAXARGS,	0,	do_ezb_cmd,
	"gk", 
	"go kernel"
);



/*
U_BOOT_CMD(name,maxargs,repeatable,command,"usage","help")

	- name			is the name of the commad. THIS IS NOT a string.
	- maxargs		the maximumn numbers of arguments this function takes
	- repeatable
	- command		Function pointer (*cmd)(struct cmd_tbl_s *, int, int, char *[]);
	- usage			Short description. This is a string
	- help			long description. This is a string
*/


