#include <rtthread.h>
#include <rtdevice.h>
#include <drv_spi.h>
#include <drv_gpio.h>
#include "drv_w25q128.h"

#ifdef BSP_USING_SPI_FLASH

#define DRV_DEBUG
#define LOG_TAG                         "drv.w25q128"
#include <drv_log.h>

#ifdef DBG_TAG
#undef DBG_TAG
#endif
#ifdef DBG_LVL
#undef DBG_LVL
#endif

#include "dev_spi_flash.h"
#include "dev_spi_flash_sfud.h"

static int rt_hw_w25q128_init(void)
{
    rt_hw_spi_device_attach(W25Q128_SPI_BUS_NAME, W25Q128_SPI_DEVICE_NAME, W25Q128_CS_PIN);

    if (RT_NULL == rt_sfud_flash_probe(FAL_USING_NOR_FLASH_DEV_NAME, W25Q128_SPI_DEVICE_NAME))
    {
        LOG_E("Failed to probe W25Q128 flash device!");
        return -RT_ERROR;
    }

    return RT_EOK;
}
INIT_COMPONENT_EXPORT(rt_hw_w25q128_init);

#ifdef BSP_SPI_FLASH_USING_FAL
#include <fal.h>

#ifdef BSP_SPI_FLASH_USING_LITTLEFS
#include <dfs_fs.h>

#define FS_PARTITION_NAME       BSP_SPI_FLASH_PARTITION_NAME
#define FS_MOUNT_POINT          BSP_SPI_FLASH_MOUNT_POINT

static int w25q128_mount(void)
{
    struct rt_device *mtd_dev = RT_NULL;

    fal_init();

    mtd_dev = fal_mtd_nor_device_create(FS_PARTITION_NAME);
    if (!mtd_dev)
    {
        LOG_E("Can't create a mtd device on '%s' partition.", FS_PARTITION_NAME);
        return -RT_ERROR;
    }

    if (dfs_mount(FS_PARTITION_NAME, FS_MOUNT_POINT, "lfs", 0, 0) == 0)
    {
        LOG_I("W25Q128 LittleFS filesystem initialized!");
    }
    else
    {
        dfs_mkfs("lfs", FS_PARTITION_NAME);
        if (dfs_mount(FS_PARTITION_NAME, FS_MOUNT_POINT, "lfs", 0, 0) == 0)
        {
            LOG_I("W25Q128 LittleFS filesystem initialized after mkfs!");
        }
        else
        {
            LOG_E("Failed to initialize W25Q128 filesystem!");
            return -RT_ERROR;
        }
    }

    return RT_EOK;
}
INIT_ENV_EXPORT(w25q128_mount);

#else

static int w25q128_fal_init(void)
{
    fal_init();
    return RT_EOK;
}
INIT_ENV_EXPORT(w25q128_fal_init);

#endif
#endif

#endif
