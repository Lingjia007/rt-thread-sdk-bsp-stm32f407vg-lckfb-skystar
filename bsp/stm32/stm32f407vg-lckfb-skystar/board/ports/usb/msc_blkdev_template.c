#include "usbd_core.h"
#include "usbd_msc.h"
#include "usb_config.h"

#define MSC_IN_EP  0x81
#define MSC_OUT_EP 0x02

#ifndef USBD_VID
#define USBD_VID           0x0483
#endif

#ifndef USBD_PID
#define USBD_PID           0x5720
#endif

#define USBD_MAX_POWER     100
#define USBD_LANGID_STRING 1033

#define USB_CONFIG_SIZE (9 + MSC_DESCRIPTOR_LEN)

#ifdef CONFIG_USB_HS
#define MSC_MAX_MPS 512
#else
#define MSC_MAX_MPS 64
#endif

#ifndef CONFIG_USBDEV_MSC_MANUFACTURER_STRING
#define CONFIG_USBDEV_MSC_MANUFACTURER_STRING "LCKFB"
#endif

#ifndef CONFIG_USBDEV_MSC_PRODUCT_STRING
#define CONFIG_USBDEV_MSC_PRODUCT_STRING "SKYSTAR SD Card"
#endif

static const uint8_t device_descriptor[] = {
    USB_DEVICE_DESCRIPTOR_INIT(USB_2_0, 0x00, 0x00, 0x00, USBD_VID, USBD_PID, 0x0200, 0x01)
};

static const uint8_t config_descriptor[] = {
    USB_CONFIG_DESCRIPTOR_INIT(USB_CONFIG_SIZE, 0x01, 0x01, USB_CONFIG_BUS_POWERED, USBD_MAX_POWER),
    MSC_DESCRIPTOR_INIT(0x00, MSC_OUT_EP, MSC_IN_EP, MSC_MAX_MPS, 0x02)
};

static const char *string_descriptors[] = {
    (const char[]){ 0x09, 0x04 },
    CONFIG_USBDEV_MSC_MANUFACTURER_STRING,
    CONFIG_USBDEV_MSC_PRODUCT_STRING,
    "2024010101",
};

static const uint8_t *device_descriptor_callback(uint8_t speed)
{
    return device_descriptor;
}

static const uint8_t *config_descriptor_callback(uint8_t speed)
{
    return config_descriptor;
}

static const char *string_descriptor_callback(uint8_t speed, uint8_t index)
{
    if (index > 3) {
        return NULL;
    }
    return string_descriptors[index];
}

const struct usb_descriptor msc_blkdev_descriptor = {
    .device_descriptor_callback = device_descriptor_callback,
    .config_descriptor_callback = config_descriptor_callback,
    .string_descriptor_callback = string_descriptor_callback
};

static void usbd_event_handler(uint8_t busid, uint8_t event)
{
    switch (event) {
        case USBD_EVENT_RESET:
            rt_kprintf("[USB] Reset\n");
            break;
        case USBD_EVENT_CONNECTED:
            rt_kprintf("[USB] Connected\n");
            break;
        case USBD_EVENT_DISCONNECTED:
            rt_kprintf("[USB] Disconnected\n");
            break;
        case USBD_EVENT_RESUME:
            break;
        case USBD_EVENT_SUSPEND:
            break;
        case USBD_EVENT_CONFIGURED:
            rt_kprintf("[USB] Configured\n");
            break;
        case USBD_EVENT_SET_REMOTE_WAKEUP:
            break;
        case USBD_EVENT_CLR_REMOTE_WAKEUP:
            break;
        default:
            break;
    }
}

#include <rtdevice.h>

#ifndef CONFIG_USBDEV_MSC_BLOCK_DEV_NAME
#define CONFIG_USBDEV_MSC_BLOCK_DEV_NAME "sd0"
#endif

static rt_device_t blk_dev = RT_NULL;
static struct rt_device_blk_geometry geometry = { 0 };

void usbd_msc_get_cap(uint8_t busid, uint8_t lun, uint32_t *block_num, uint32_t *block_size)
{
    if (blk_dev) {
        rt_device_control(blk_dev, RT_DEVICE_CTRL_BLK_GETGEOME, &geometry);
        *block_num = geometry.sector_count;
        *block_size = geometry.bytes_per_sector;
    } else {
        *block_num = 0;
        *block_size = 512;
    }
}

int usbd_msc_sector_read(uint8_t busid, uint8_t lun, uint32_t sector, uint8_t *buffer, uint32_t length)
{
    if (blk_dev && geometry.bytes_per_sector > 0) {
        rt_device_read(blk_dev, sector, buffer, length / geometry.bytes_per_sector);
    }
    return 0;
}

int usbd_msc_sector_write(uint8_t busid, uint8_t lun, uint32_t sector, uint8_t *buffer, uint32_t length)
{
    if (blk_dev && geometry.bytes_per_sector > 0) {
        rt_device_write(blk_dev, sector, buffer, length / geometry.bytes_per_sector);
    }
    return 0;
}

static struct usbd_interface intf0;

void msc_blkdev_init(uint8_t busid, uintptr_t reg_base)
{
    rt_kprintf("[USB] Initializing MSC device...\n");
    
    blk_dev = rt_device_find(CONFIG_USBDEV_MSC_BLOCK_DEV_NAME);
    if (blk_dev == RT_NULL) {
        rt_kprintf("[USB] Block device '%s' not found!\n", CONFIG_USBDEV_MSC_BLOCK_DEV_NAME);
        return;
    }

    if (rt_device_open(blk_dev, RT_DEVICE_OFLAG_RDWR) != RT_EOK) {
        rt_kprintf("[USB] Failed to open block device '%s'\n", CONFIG_USBDEV_MSC_BLOCK_DEV_NAME);
        return;
    }

    rt_kprintf("[USB] Block device '%s' opened successfully\n", CONFIG_USBDEV_MSC_BLOCK_DEV_NAME);

    usbd_desc_register(busid, &msc_blkdev_descriptor);
    usbd_add_interface(busid, usbd_msc_init_intf(busid, &intf0, MSC_OUT_EP, MSC_IN_EP));
    usbd_initialize(busid, reg_base, usbd_event_handler);
    
    rt_kprintf("[USB] MSC device initialized, VID=0x%04X, PID=0x%04X\n", USBD_VID, USBD_PID);
}
