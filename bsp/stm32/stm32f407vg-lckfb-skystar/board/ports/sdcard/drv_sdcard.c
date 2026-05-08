#include <rtthread.h>

#ifdef BSP_USING_SDCARD

#include <dfs_elm.h>
#include <dfs_fs.h>
#include <dfs_file.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/statfs.h>
#include "drv_gpio.h"
#include "drv_sdio.h"

#define DBG_TAG "app.card"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

#ifdef BSP_SDCARD_DET_PORT_A
#define SD_CARD_DET_PORT    GET_PIN(A, BSP_SDCARD_DET_PIN_NUM)
#elif defined(BSP_SDCARD_DET_PORT_B)
#define SD_CARD_DET_PORT    GET_PIN(B, BSP_SDCARD_DET_PIN_NUM)
#elif defined(BSP_SDCARD_DET_PORT_C)
#define SD_CARD_DET_PORT    GET_PIN(C, BSP_SDCARD_DET_PIN_NUM)
#elif defined(BSP_SDCARD_DET_PORT_D)
#define SD_CARD_DET_PORT    GET_PIN(D, BSP_SDCARD_DET_PIN_NUM)
#elif defined(BSP_SDCARD_DET_PORT_E)
#define SD_CARD_DET_PORT    GET_PIN(E, BSP_SDCARD_DET_PIN_NUM)
#elif defined(BSP_SDCARD_DET_PORT_F)
#define SD_CARD_DET_PORT    GET_PIN(F, BSP_SDCARD_DET_PIN_NUM)
#elif defined(BSP_SDCARD_DET_PORT_G)
#define SD_CARD_DET_PORT    GET_PIN(G, BSP_SDCARD_DET_PIN_NUM)
#elif defined(BSP_SDCARD_DET_PORT_H)
#define SD_CARD_DET_PORT    GET_PIN(H, BSP_SDCARD_DET_PIN_NUM)
#else
#define SD_CARD_DET_PORT    GET_PIN(D, 3)
#endif

#ifdef BSP_SDCARD_DET_ACTIVE_LOW
#define SD_CARD_IS_INSERTED()    (rt_pin_read(SD_CARD_DET_PORT) == PIN_LOW)
#else
#define SD_CARD_IS_INSERTED()    (rt_pin_read(SD_CARD_DET_PORT) == PIN_HIGH)
#endif

#ifndef BSP_SDCARD_MOUNT_POINT
#define BSP_SDCARD_MOUNT_POINT    "/sdcard"
#endif

#ifndef BSP_SDCARD_MOUNT_RETRY
#define BSP_SDCARD_MOUNT_RETRY    3
#endif

static void _sdcard_mount(void)
{
    rt_device_t device;
    int ret;
    int retry = 0;
    int max_retry = BSP_SDCARD_MOUNT_RETRY;

    device = rt_device_find("sd0");
    if (device == RT_NULL)
    {
        mmcsd_wait_cd_changed(0);
        stm32_mmcsd_change();
        mmcsd_wait_cd_changed(RT_WAITING_FOREVER);
        device = rt_device_find("sd0");
    }

    if (device == RT_NULL)
    {
        LOG_E("SD card device not found!");
        return;
    }

    while (retry < max_retry)
    {
        rt_thread_mdelay(200);

        ret = dfs_mount("sd0", BSP_SDCARD_MOUNT_POINT, "elm", 0, 0);
        if (ret == RT_EOK)
        {
            LOG_I("SD card mounted to '%s'", BSP_SDCARD_MOUNT_POINT);
            return;
        }

        LOG_W("SD card mount to '%s' failed, retry %d/%d, ret=%d, errno=%d", 
              BSP_SDCARD_MOUNT_POINT, retry + 1, max_retry, ret, rt_get_errno());

#ifdef BSP_SDCARD_FORMAT_IF_FAIL
        if (retry == max_retry - 1)
        {
            LOG_W("Trying to format SD card...");
            LOG_W("WARNING: This will erase all data on the SD card!");

            ret = dfs_mkfs("elm", "sd0");
            if (ret == RT_EOK)
            {
                LOG_I("SD card formatted successfully");

                ret = dfs_mount("sd0", BSP_SDCARD_MOUNT_POINT, "elm", 0, 0);
                if (ret == RT_EOK)
                {
                    LOG_I("SD card mounted to '%s' after format", BSP_SDCARD_MOUNT_POINT);
                    return;
                }
                else
                {
                    LOG_E("Failed to mount after format, ret=%d", ret);
                }
            }
            else
            {
                LOG_E("Failed to format SD card, ret=%d", ret);
            }
        }
#endif

        retry++;
    }

    LOG_E("Failed to mount SD card after %d retries", max_retry);
}

static void _sdcard_unmount(void)
{
    rt_thread_mdelay(200);
    dfs_unmount(BSP_SDCARD_MOUNT_POINT);
    LOG_I("Unmounted '%s'", BSP_SDCARD_MOUNT_POINT);

    mmcsd_wait_cd_changed(0);
    stm32_mmcsd_change();
    mmcsd_wait_cd_changed(RT_WAITING_FOREVER);
}

#ifdef BSP_SDCARD_HOT_PLUG

static void sd_mount(void *parameter)
{
    rt_uint8_t last_state = 1;

    rt_pin_mode(SD_CARD_DET_PORT, PIN_MODE_INPUT_PULLUP);

    while (1)
    {
        rt_thread_mdelay(200);
        
        if (last_state && SD_CARD_IS_INSERTED())
        {
            _sdcard_mount();
            last_state = 0;
        }

        if (!last_state && !SD_CARD_IS_INSERTED())
        {
            _sdcard_unmount();
            last_state = 1;
        }
    }
}

int stm32_sdcard_mount(void)
{
    rt_thread_t tid;

    tid = rt_thread_create("sd_mount", sd_mount, RT_NULL,
                           BSP_SDCARD_MOUNT_THREAD_STACK, 
                           BSP_SDCARD_MOUNT_THREAD_PRIORITY, 
                           20);
    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
    }
    else
    {
        LOG_E("create sd_mount thread err!");
    }
    return RT_EOK;
}

#else

int stm32_sdcard_mount(void)
{
    rt_pin_mode(SD_CARD_DET_PORT, PIN_MODE_INPUT_PULLUP);
    
    if (SD_CARD_IS_INSERTED())
    {
        _sdcard_mount();
    }
    else
    {
        LOG_W("No SD card detected!");
    }
    return RT_EOK;
}

#endif

#ifdef BSP_SDCARD_AUTO_MOUNT
INIT_APP_EXPORT(stm32_sdcard_mount);
#endif

#endif
