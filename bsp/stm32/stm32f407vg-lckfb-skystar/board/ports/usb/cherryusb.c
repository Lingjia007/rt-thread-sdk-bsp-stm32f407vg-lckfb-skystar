#include "board.h"
#include "rtthread.h"
#include <stdbool.h>

#ifdef BSP_USING_USB_MSC

#define DBG_TAG "app.usb"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

#ifndef BSP_USB_MSC_BLOCK_DEV_NAME
#define BSP_USB_MSC_BLOCK_DEV_NAME "sd0"
#endif

#ifndef CONFIG_USBDEV_MSC_BLOCK_DEV_NAME
#define CONFIG_USBDEV_MSC_BLOCK_DEV_NAME BSP_USB_MSC_BLOCK_DEV_NAME
#endif

#ifndef BSP_USB_MSC_INIT_DELAY_MS
#define BSP_USB_MSC_INIT_DELAY_MS 1000
#endif

static void usb_msc_init_thread(void *parameter)
{
    extern void msc_blkdev_init(uint8_t busid, uintptr_t reg_base);
    rt_device_t blk_dev;
    int retry = 0;
    int max_retry = 20;

    rt_thread_mdelay(BSP_USB_MSC_INIT_DELAY_MS);

    while (retry < max_retry)
    {
        blk_dev = rt_device_find(BSP_USB_MSC_BLOCK_DEV_NAME);
        if (blk_dev != RT_NULL)
        {
            break;
        }
        LOG_D("Waiting for block device '%s'... (%d/%d)", 
              BSP_USB_MSC_BLOCK_DEV_NAME, retry + 1, max_retry);
        rt_thread_mdelay(500);
        retry++;
    }

    if (blk_dev == RT_NULL)
    {
        LOG_E("Block device '%s' not found after %d retries!", 
              BSP_USB_MSC_BLOCK_DEV_NAME, max_retry);
        return;
    }

    msc_blkdev_init(0, USB_OTG_FS_PERIPH_BASE);
    LOG_I("CherryUSB MSC device initialized (block dev: %s)", BSP_USB_MSC_BLOCK_DEV_NAME);
}

static int rt_hw_stm32_cherryusb_msc_init(void)
{
    rt_thread_t tid;

    tid = rt_thread_create("usb_msc", usb_msc_init_thread, RT_NULL,
                           1024, 15, 20);
    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
    }
    else
    {
        LOG_E("Failed to create USB MSC init thread");
        return -RT_ERROR;
    }
    return RT_EOK;
}
INIT_APP_EXPORT(rt_hw_stm32_cherryusb_msc_init);

#ifdef BSP_USB_MSC_READ_ONLY
#include "usbd_msc.h"
static int rt_hw_usb_msc_set_readonly(void)
{
    usbd_msc_set_readonly(0, true);
    LOG_I("USB MSC set to read-only mode");
    return 0;
}
INIT_APP_EXPORT(rt_hw_usb_msc_set_readonly);
#endif

#endif
