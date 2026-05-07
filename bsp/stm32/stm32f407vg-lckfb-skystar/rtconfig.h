#ifndef RT_CONFIG_H__
#define RT_CONFIG_H__

#define SOC_STM32F407VG
#define BOARD_STM32F407VG_LCKFB

/* RT-Thread Kernel */

/* klibc options */

/* rt_vsnprintf options */

/* end of rt_vsnprintf options */

/* rt_vsscanf options */

/* end of rt_vsscanf options */

/* rt_memset options */

/* end of rt_memset options */

/* rt_memcpy options */

/* end of rt_memcpy options */

/* rt_memmove options */

/* end of rt_memmove options */

/* rt_memcmp options */

/* end of rt_memcmp options */

/* rt_strstr options */

/* end of rt_strstr options */

/* rt_strcasecmp options */

/* end of rt_strcasecmp options */

/* rt_strncpy options */

/* end of rt_strncpy options */

/* rt_strcpy options */

/* end of rt_strcpy options */

/* rt_strncmp options */

/* end of rt_strncmp options */

/* rt_strcmp options */

/* end of rt_strcmp options */

/* rt_strlen options */

/* end of rt_strlen options */

/* rt_strnlen options */

/* end of rt_strnlen options */
/* end of klibc options */
#define RT_NAME_MAX 12
#define RT_CPUS_NR 1
#define RT_ALIGN_SIZE 8
#define RT_THREAD_PRIORITY_32
#define RT_THREAD_PRIORITY_MAX 32
#define RT_TICK_PER_SECOND 1000
#define RT_USING_OVERFLOW_CHECK
#define RT_USING_HOOK
#define RT_HOOK_USING_FUNC_PTR
#define RT_USING_IDLE_HOOK
#define RT_IDLE_HOOK_LIST_SIZE 4
#define IDLE_THREAD_STACK_SIZE 1024

/* kservice options */

/* end of kservice options */
#define RT_USING_DEBUG
#define RT_DEBUGING_ASSERT
#define RT_DEBUGING_COLOR
#define RT_DEBUGING_CONTEXT

/* Inter-Thread communication */

#define RT_USING_SEMAPHORE
#define RT_USING_MUTEX
#define RT_USING_EVENT
#define RT_USING_MAILBOX
#define RT_USING_MESSAGEQUEUE
/* end of Inter-Thread communication */

/* Memory Management */

#define RT_USING_MEMPOOL
#define RT_USING_SMALL_MEM
#define RT_USING_SMALL_MEM_AS_HEAP
#define RT_USING_HEAP
/* end of Memory Management */
#define RT_USING_DEVICE
#define RT_USING_DEVICE_OPS
#define RT_USING_CONSOLE
#define RT_CONSOLEBUF_SIZE 512
#define RT_CONSOLE_DEVICE_NAME "uart4"
#define RT_USING_CONSOLE_OUTPUT_CTL
#define RT_VER_NUM 0x50300
#define RT_BACKTRACE_LEVEL_MAX_NR 32
/* end of RT-Thread Kernel */
#define RT_USING_HW_ATOMIC
#define ARCH_USING_HW_ATOMIC_8
#define ARCH_USING_HW_ATOMIC_16
#define RT_USING_CPU_FFS
#define ARCH_ARM
#define ARCH_ARM_CORTEX_M
#define ARCH_ARM_CORTEX_M4

/* RT-Thread Components */

#define RT_USING_COMPONENTS_INIT
#define RT_USING_USER_MAIN
#define RT_MAIN_THREAD_STACK_SIZE 2048
#define RT_MAIN_THREAD_PRIORITY 10
#define RT_USING_MSH
#define RT_USING_FINSH
#define FINSH_USING_MSH
#define FINSH_THREAD_NAME "tshell"
#define FINSH_THREAD_PRIORITY 20
#define FINSH_THREAD_STACK_SIZE 4096
#define FINSH_USING_HISTORY
#define FINSH_HISTORY_LINES 5
#define FINSH_USING_SYMTAB
#define FINSH_CMD_SIZE 512
#define MSH_USING_BUILT_IN_COMMANDS
#define FINSH_USING_DESCRIPTION
#define FINSH_ARG_MAX 10
#define FINSH_USING_OPTION_COMPLETION

/* DFS: device virtual file system */

#define RT_USING_DFS
#define DFS_USING_POSIX
#define DFS_USING_WORKDIR
#define DFS_FD_MAX 16
#define RT_USING_DFS_V1
#define DFS_FILESYSTEMS_MAX 4
#define DFS_FILESYSTEM_TYPES_MAX 4
#define RT_USING_DFS_DEVFS
/* end of DFS: device virtual file system */
#define RT_USING_FAL
#define FAL_USING_DEBUG
#define FAL_PART_HAS_TABLE_CFG
#define FAL_USING_SFUD_PORT
#define FAL_USING_NOR_FLASH_DEV_NAME "w25q128"
#define FAL_DEV_NAME_MAX 24
#define FAL_DEV_BLK_MAX 6

/* Device Drivers */

#define RT_USING_DEVICE_IPC
#define RT_UNAMED_PIPE_NUMBER 64
#define RT_USING_SYSTEM_WORKQUEUE
#define RT_SYSTEM_WORKQUEUE_STACKSIZE 2048
#define RT_SYSTEM_WORKQUEUE_PRIORITY 23
#define RT_USING_SERIAL
#define RT_USING_SERIAL_V1
#define RT_SERIAL_USING_DMA
#define RT_SERIAL_RB_BUFSZ 1024
#define RT_USING_MTD_NOR
#define RT_USING_SPI
#define RT_USING_SPI_ISR
#define RT_USING_SFUD
#define RT_SFUD_USING_SFDP
#define RT_SFUD_USING_FLASH_INFO_TABLE
#define RT_SFUD_SPI_MAX_HZ 50000000
#define RT_USING_PIN
/* end of Device Drivers */

/* C/C++ and POSIX layer */

/* ISO-ANSI C layer */

/* Timezone and Daylight Saving Time */

#define RT_LIBC_USING_LIGHT_TZ_DST
#define RT_LIBC_TZ_DEFAULT_HOUR 8
#define RT_LIBC_TZ_DEFAULT_MIN 0
#define RT_LIBC_TZ_DEFAULT_SEC 0
/* end of Timezone and Daylight Saving Time */
/* end of ISO-ANSI C layer */

/* POSIX (Portable Operating System Interface) layer */

#define RT_USING_POSIX_FS
#define RT_USING_POSIX_DEVIO
#define RT_USING_POSIX_POLL
#define RT_USING_POSIX_SELECT
#define RT_USING_POSIX_SOCKET

/* Interprocess Communication (IPC) */

#define RT_USING_POSIX_PIPE
#define RT_USING_POSIX_PIPE_SIZE 512

/* Socket is in the 'Network' category */

/* end of Interprocess Communication (IPC) */
/* end of POSIX (Portable Operating System Interface) layer */
/* end of C/C++ and POSIX layer */

/* Network */

#define RT_USING_SAL
#define SAL_INTERNET_CHECK
#define SOCKET_TABLE_STEP_LEN 4

/* Docking with protocol stacks */

#define SAL_USING_AT
/* end of Docking with protocol stacks */
#define SAL_USING_POSIX
#define RT_USING_NETDEV
#define NETDEV_USING_IFCONFIG
#define NETDEV_USING_PING
#define NETDEV_USING_NETSTAT
#define NETDEV_USING_AUTO_DEFAULT
#define NETDEV_IPV4 1
#define NETDEV_IPV6 0
#define RT_USING_AT
#define AT_USING_CLIENT
#define AT_CLIENT_NUM_MAX 1
#define AT_USING_SOCKET
#define AT_USING_CLI
#define AT_SW_VERSION_NUM 0x10301
/* end of Network */

/* Memory protection */

/* end of Memory protection */

/* Utilities */

#define RT_USING_RESOURCE_ID
/* end of Utilities */

/* Using USB legacy version */

/* end of Using USB legacy version */
/* end of RT-Thread Components */

/* RT-Thread Utestcases */

/* end of RT-Thread Utestcases */

/* RT-Thread online packages */

/* IoT - internet of things */

#define PKG_USING_PAHOMQTT
#define PAHOMQTT_PIPE_MODE
#define RT_PKG_MQTT_THREAD_STACK_SIZE 4096
#define PKG_PAHOMQTT_SUBSCRIBE_HANDLERS 8
#define MQTT_DEBUG
#define PKG_USING_PAHOMQTT_LATEST

/* Wi-Fi */

/* Marvell WiFi */

/* end of Marvell WiFi */

/* Wiced WiFi */

/* end of Wiced WiFi */

/* CYW43012 WiFi */

/* end of CYW43012 WiFi */

/* BL808 WiFi */

/* end of BL808 WiFi */

/* CYW43439 WiFi */

/* end of CYW43439 WiFi */
/* end of Wi-Fi */
#define PKG_USING_AT_DEVICE
#define AT_DEVICE_USING_ESP8266
#define AT_DEVICE_ESP8266_INIT_ASYN
#define AT_DEVICE_ESP8266_SOCKET
#define AT_DEVICE_ESP8266_SAMPLE
#define AT_DEVICE_ESP8266_SAMPLE_BSP_TAKEOVER
#define ESP8266_SAMPLE_WIFI_SSID "iQOO Neo8 Pro"
#define ESP8266_SAMPLE_WIFI_PASSWORD "12345678"
#define ESP8266_SAMPLE_CLIENT_NAME "uart1"
#define ESP8266_SAMPLE_RECV_BUFF_LEN 4096
#define PKG_USING_AT_DEVICE_LATEST_VERSION
#define PKG_AT_DEVICE_VER_NUM 0x99999

/* IoT Cloud */

/* end of IoT Cloud */
/* end of IoT - internet of things */

/* security packages */

/* end of security packages */

/* language packages */

/* JSON: JavaScript Object Notation, a lightweight data-interchange format */

#define PKG_USING_CJSON
#define PKG_USING_CJSON_V1717
/* end of JSON: JavaScript Object Notation, a lightweight data-interchange format */

/* XML: Extensible Markup Language */

/* end of XML: Extensible Markup Language */
/* end of language packages */

/* multimedia packages */

/* LVGL: powerful and easy-to-use embedded GUI library */

/* end of LVGL: powerful and easy-to-use embedded GUI library */

/* u8g2: a monochrome graphic library */

/* end of u8g2: a monochrome graphic library */
/* end of multimedia packages */

/* tools packages */

/* end of tools packages */

/* system packages */

/* enhanced kernel services */

/* end of enhanced kernel services */

/* acceleration: Assembly language or algorithmic acceleration packages */

/* end of acceleration: Assembly language or algorithmic acceleration packages */

/* CMSIS: ARM Cortex-M Microcontroller Software Interface Standard */

#define PKG_USING_CMSIS_CORE
#define PKG_USING_CMSIS_CORE_LATEST_VERSION
/* end of CMSIS: ARM Cortex-M Microcontroller Software Interface Standard */

/* Micrium: Micrium software products porting for RT-Thread */

/* end of Micrium: Micrium software products porting for RT-Thread */
#define PKG_USING_LITTLEFS
#define PKG_USING_LITTLEFS_LATEST_VERSION
#define LFS_READ_SIZE 256
#define LFS_PROG_SIZE 256
#define LFS_BLOCK_SIZE 4096
#define LFS_NO_DEBUG
#define LFS_CACHE_SIZE 256
#define LFS_BLOCK_CYCLES 500
#define LFS_THREADSAFE
#define LFS_LOOKAHEAD_MAX 128
#define RT_DEF_LFS_DRIVERS 1
/* end of system packages */

/* peripheral libraries and drivers */

/* HAL & SDK Drivers */

/* STM32 HAL & SDK Drivers */

#define PKG_USING_STM32F4_HAL_DRIVER
#define PKG_USING_STM32F4_HAL_DRIVER_LATEST_VERSION
#define PKG_USING_STM32F4_CMSIS_DRIVER
#define PKG_USING_STM32F4_CMSIS_DRIVER_LATEST_VERSION
/* end of STM32 HAL & SDK Drivers */

/* Infineon HAL Packages */

/* end of Infineon HAL Packages */

/* Kendryte SDK */

/* end of Kendryte SDK */

/* WCH HAL & SDK Drivers */

/* end of WCH HAL & SDK Drivers */

/* AT32 HAL & SDK Drivers */

/* end of AT32 HAL & SDK Drivers */

/* HC32 DDL Drivers */

/* end of HC32 DDL Drivers */

/* NXP HAL & SDK Drivers */

/* end of NXP HAL & SDK Drivers */

/* NUVOTON Drivers */

/* end of NUVOTON Drivers */

/* GD32 Drivers */

/* end of GD32 Drivers */

/* HPMicro SDK */

/* end of HPMicro SDK */

/* FT32 HAL & SDK Drivers */

/* end of FT32 HAL & SDK Drivers */
/* end of HAL & SDK Drivers */

/* sensors drivers */

/* end of sensors drivers */

/* touch drivers */

/* end of touch drivers */
/* end of peripheral libraries and drivers */

/* AI packages */

/* end of AI packages */

/* Signal Processing and Control Algorithm Packages */

/* end of Signal Processing and Control Algorithm Packages */

/* miscellaneous packages */

/* project laboratory */

/* end of project laboratory */

/* samples: kernel and components samples */

/* end of samples: kernel and components samples */

/* entertainment: terminal games and other interesting software packages */

/* end of entertainment: terminal games and other interesting software packages */
/* end of miscellaneous packages */

/* Arduino libraries */


/* Projects and Demos */

/* end of Projects and Demos */

/* Sensors */

/* end of Sensors */

/* Display */

/* end of Display */

/* Timing */

/* end of Timing */

/* Data Processing */

/* end of Data Processing */

/* Data Storage */

/* Communication */

/* end of Communication */

/* Device Control */

/* end of Device Control */

/* Other */

/* end of Other */

/* Signal IO */

/* end of Signal IO */

/* Uncategorized */

/* end of Arduino libraries */
/* end of RT-Thread online packages */
#define SOC_FAMILY_STM32
#define SOC_SERIES_STM32F4

/* Hardware Drivers Config */

/* On-chip Peripheral Drivers */

#define BSP_USING_GPIO
#define BSP_USING_UART
#define BSP_STM32_UART_V1_TX_TIMEOUT 2000
#define BSP_USING_UART1
#define BSP_USING_UART4
#define BSP_UART4_RX_BUFSIZE 256
#define BSP_UART4_TX_BUFSIZE 0
#define BSP_USING_ON_CHIP_FLASH
#define BSP_USING_SPI
#define BSP_USING_SPI1
/* end of On-chip Peripheral Drivers */

/* Onboard Peripheral Drivers */

/* LED Driver */

#define BSP_USING_LED
#define BSP_LED_BOOT_MODE_PWM
#define BSP_LED_BOOT_BREATHE
#define BSP_LED_BLINK_INTERVAL 500
#define BSP_LED_BREATHE_PERIOD 3600
#define BSP_LED_SW_PWM_PERIOD 20
/* end of LED Driver */

/* ESP8266 Driver */

#define BSP_USING_ESP8266

/* May adjust RT_SERIAL_RB_BUFSZ up to 512 if using the Serial V1 device driver */

/* end of ESP8266 Driver */

/* W25Q128 SPI Flash Driver */

#define BSP_USING_SPI_FLASH
#define BSP_SPI_FLASH_CS_PORT_A
#define BSP_SPI_FLASH_CS_PIN_NUM 4
#define BSP_SPI_FLASH_USING_FAL
#define BSP_SPI_FLASH_PARTITION_NAME "spi-flash"
#define BSP_SPI_FLASH_USING_LITTLEFS
#define BSP_SPI_FLASH_MOUNT_POINT "/"
/* end of W25Q128 SPI Flash Driver */

/* Board Support Packages */

#define BSP_USING_ONENET
#define BSP_ONENET_USING_SAMPLE
#define BSP_ONENET_USING_MQTT
#define BSP_ONENET_AUTO_INIT
#define BSP_ONENET_NETDEV_NAME "esp0"
#define BSP_ONENET_AUTO_RECONNECT
#define BSP_ONENET_DEVICE_DIRECT

/* MQTT Topics Configuration */

#define BSP_ONENET_USING_PROPERTY_POST
#define BSP_ONENET_USING_PROPERTY_SET
#define BSP_ONENET_USING_PROPERTY_GET
#define BSP_ONENET_USING_PROPERTY_DESIRED_GET
#define BSP_ONENET_USING_PROPERTY_DESIRED_DELETE
#define BSP_ONENET_USING_OTA
/* end of MQTT Topics Configuration */

/* Auto Reply Configuration */

#define BSP_ONENET_AUTO_REPLY_SET
#define BSP_ONENET_AUTO_REPLY_GET
#define BSP_ONENET_AUTO_REPLY_OTA
/* end of Auto Reply Configuration */
#define BSP_ONENET_INFO_DEVID "STM32F407VGT6_SKYSTAR"
#define BSP_ONENET_INFO_PROID "OmZ40ZapAD"
/* end of Board Support Packages */
/* end of Onboard Peripheral Drivers */

/* Board extended module Drivers */

/* end of Hardware Drivers Config */

#endif
