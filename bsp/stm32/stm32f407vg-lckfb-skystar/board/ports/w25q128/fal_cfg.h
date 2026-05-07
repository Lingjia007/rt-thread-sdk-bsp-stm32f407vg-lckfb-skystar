#ifndef _FAL_CFG_H_
#define _FAL_CFG_H_

#include <rtthread.h>
#include <board.h>

#ifdef BSP_USING_ON_CHIP_FLASH
#define FLASH_SIZE_GRANULARITY_16K   (4 * 16 * 1024)
#define FLASH_SIZE_GRANULARITY_64K   (64 * 1024)
#define FLASH_SIZE_GRANULARITY_128K  (7 * 128 * 1024)

#define STM32_FLASH_START_ADRESS_16K  STM32_FLASH_START_ADRESS
#define STM32_FLASH_START_ADRESS_64K  (STM32_FLASH_START_ADRESS_16K + FLASH_SIZE_GRANULARITY_16K)
#define STM32_FLASH_START_ADRESS_128K (STM32_FLASH_START_ADRESS_64K + FLASH_SIZE_GRANULARITY_64K)

extern const struct fal_flash_dev stm32_onchip_flash_16k;
extern const struct fal_flash_dev stm32_onchip_flash_64k;
extern const struct fal_flash_dev stm32_onchip_flash_128k;
#endif

extern struct fal_flash_dev nor_flash0;

#ifdef BSP_USING_ON_CHIP_FLASH
#define FAL_FLASH_DEV_TABLE                                          \
{                                                                    \
    &stm32_onchip_flash_16k,                                         \
    &stm32_onchip_flash_64k,                                         \
    &stm32_onchip_flash_128k,                                        \
    &nor_flash0,                                                     \
}
#else
#define FAL_FLASH_DEV_TABLE          \
{                                    \
    &nor_flash0,                     \
}
#endif

#ifdef FAL_PART_HAS_TABLE_CFG
#ifndef BSP_SPI_FLASH_PARTITION_NAME
#define BSP_SPI_FLASH_PARTITION_NAME  "filesystem"
#endif

#ifdef BSP_USING_ON_CHIP_FLASH
#define FAL_PART_TABLE                                                                                              \
{                                                                                                                   \
    {FAL_PART_MAGIC_WORD,        "app", "onchip_flash_128k",                            0,       512 * 1024, 0}, \
    {FAL_PART_MAGIC_WORD,      "param", "onchip_flash_128k",                   512 * 1024,       384 * 1024, 0}, \
    {FAL_PART_MAGIC_WORD, BSP_SPI_FLASH_PARTITION_NAME, FAL_USING_NOR_FLASH_DEV_NAME,  0, 16 * 1024 * 1024, 0}, \
}
#else
#define FAL_PART_TABLE                                                                                      \
{                                                                                                           \
    {FAL_PART_MAGIC_WORD, BSP_SPI_FLASH_PARTITION_NAME, FAL_USING_NOR_FLASH_DEV_NAME, 0, 16 * 1024 * 1024, 0}, \
}
#endif
#endif

#endif
