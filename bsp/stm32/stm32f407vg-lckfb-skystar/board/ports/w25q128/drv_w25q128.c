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

#ifdef BSP_SPI_FLASH_AUTO_MOUNT

#include <dfs_fs.h>
#include <dirent.h>

#define FS_PARTITION_NAME       BSP_SPI_FLASH_PARTITION_NAME
#define FS_MOUNT_POINT          BSP_SPI_FLASH_MOUNT_POINT

#ifdef BSP_SPI_FLASH_USING_LITTLEFS
#define FS_TYPE                 "lfs"
#elif defined(BSP_SPI_FLASH_USING_FATFS)
#define FS_TYPE                 "elm"
#endif

static void w25q128_mount_entry(void *parameter)
{
    struct rt_device *mtd_dev = RT_NULL;
    DIR *dir = RT_NULL;
    int ret;
    int retry = 0;
    int max_retry = BSP_SPI_FLASH_MOUNT_RETRY;

    fal_init();

    mtd_dev = fal_mtd_nor_device_create(FS_PARTITION_NAME);
    if (!mtd_dev)
    {
        LOG_E("Can't create a mtd device on '%s' partition.", FS_PARTITION_NAME);
        return;
    }

    while (retry < max_retry)
    {
        rt_thread_mdelay(500);

        dir = opendir(FS_MOUNT_POINT);
        if (dir == RT_NULL)
        {
            ret = mkdir(FS_MOUNT_POINT, 0);
            if (ret != 0)
            {
                LOG_W("mkdir '%s' failed, ret=%d, errno=%d", FS_MOUNT_POINT, ret, rt_get_errno());
                retry++;
                continue;
            }
        }
        else
        {
            closedir(dir);
        }

        ret = dfs_mount(FS_PARTITION_NAME, FS_MOUNT_POINT, FS_TYPE, 0, 0);
        if (ret == 0)
        {
#ifdef BSP_SPI_FLASH_USING_LITTLEFS
            LOG_I("W25Q128 LittleFS filesystem initialized!");
#elif defined(BSP_SPI_FLASH_USING_FATFS)
            LOG_I("W25Q128 FATFS filesystem initialized!");
#endif
            return;
        }

        LOG_W("mount failed, ret=%d, errno=%d", ret, rt_get_errno());

#ifdef BSP_SPI_FLASH_FORMAT_IF_FAIL
        LOG_I("Trying to format filesystem...");

        ret = dfs_mkfs(FS_TYPE, FS_PARTITION_NAME);
        if (ret != 0)
        {
            LOG_E("mkfs failed, ret=%d", ret);
            retry++;
            continue;
        }

        ret = dfs_mount(FS_PARTITION_NAME, FS_MOUNT_POINT, FS_TYPE, 0, 0);
        if (ret == 0)
        {
#ifdef BSP_SPI_FLASH_USING_LITTLEFS
            LOG_I("W25Q128 LittleFS filesystem initialized after mkfs!");
#elif defined(BSP_SPI_FLASH_USING_FATFS)
            LOG_I("W25Q128 FATFS filesystem initialized after mkfs!");
#endif
            return;
        }

        LOG_E("Failed to mount after mkfs, ret=%d, errno=%d", ret, rt_get_errno());
#else
        LOG_E("Mount failed. Please format the filesystem manually.");
#endif

        retry++;
    }

    LOG_E("Failed to initialize W25Q128 filesystem after %d retries!", retry);
}

static int w25q128_mount(void)
{
    rt_thread_t tid;

    tid = rt_thread_create("flash_mount", w25q128_mount_entry, RT_NULL,
                           BSP_SPI_FLASH_MOUNT_THREAD_STACK, 
                           BSP_SPI_FLASH_MOUNT_THREAD_PRIORITY, 
                           10);
    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
    }
    else
    {
        LOG_E("create flash_mount thread err!");
    }

    return RT_EOK;
}
INIT_APP_EXPORT(w25q128_mount);

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
