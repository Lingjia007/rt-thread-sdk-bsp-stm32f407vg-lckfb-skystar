#include <rtconfig.h>
#ifdef BSP_USING_USB_CDC_MSC
#include "usbd_core.h"
#include "usbd_cdc_acm.h"
#include "usbd_msc.h"
#include "usb_config.h"

#define CDC_INT_EP 0x81
#define CDC_OUT_EP 0x02
#define CDC_IN_EP  0x82
#define MSC_OUT_EP 0x03
#define MSC_IN_EP  0x83

#ifndef USBD_VID
#define USBD_VID           0x0483
#endif

#ifndef USBD_PID
#define USBD_PID           0x5741
#endif

#define USBD_MAX_POWER     100
#define USBD_LANGID_STRING 1033

#define USB_CONFIG_SIZE (9 + CDC_ACM_DESCRIPTOR_LEN + MSC_DESCRIPTOR_LEN)

#ifdef CONFIG_USB_HS
#define CDC_MAX_MPS 512
#define MSC_MAX_MPS 512
#else
#define CDC_MAX_MPS 64
#define MSC_MAX_MPS 64
#endif

#ifndef CONFIG_USBDEV_MSC_MANUFACTURER_STRING
#define CONFIG_USBDEV_MSC_MANUFACTURER_STRING "LCKFB"
#endif

#ifndef CONFIG_USBDEV_MSC_PRODUCT_STRING
#define CONFIG_USBDEV_MSC_PRODUCT_STRING "SKYSTAR Composite Device"
#endif

static const uint8_t device_descriptor[] = {
    USB_DEVICE_DESCRIPTOR_INIT(USB_2_0, 0xEF, 0x02, 0x01, USBD_VID, USBD_PID, 0x0200, 0x01)
};

static const uint8_t config_descriptor[] = {
    USB_CONFIG_DESCRIPTOR_INIT(USB_CONFIG_SIZE, 0x03, 0x01, USB_CONFIG_BUS_POWERED, USBD_MAX_POWER),
    CDC_ACM_DESCRIPTOR_INIT(0x00, CDC_INT_EP, CDC_OUT_EP, CDC_IN_EP, CDC_MAX_MPS, 0x02),
    MSC_DESCRIPTOR_INIT(0x02, MSC_OUT_EP, MSC_IN_EP, MSC_MAX_MPS, 0x00)
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

static const uint8_t msosv1_string[] = {
    USB_MSOSV1_STRING_DESCRIPTOR_INIT(0x01)
};

static const uint8_t msosv1_compat_id[] = {
    USB_MSOSV1_COMP_ID_HEADER_DESCRIPTOR_INIT(0x01),
    0x00, 0x01,
    'C', 'D', 'C', 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static struct usb_msosv1_descriptor msosv1_desc = {
    .string = msosv1_string,
    .vendor_code = 0x01,
    .compat_id = msosv1_compat_id,
    .comp_id_property = NULL,
};

const struct usb_descriptor cdc_msc_blkdev_descriptor = {
    .device_descriptor_callback = device_descriptor_callback,
    .config_descriptor_callback = config_descriptor_callback,
    .string_descriptor_callback = string_descriptor_callback,
    .msosv1_descriptor = &msosv1_desc
};

USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t cdc_read_buffer[CDC_MAX_MPS];
USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t cdc_write_buffer[CDC_MAX_MPS];

static volatile bool ep_tx_busy_flag = false;
static volatile bool dtr_enable = false;

static void usbd_event_handler(uint8_t busid, uint8_t event)
{
    switch (event) {
        case USBD_EVENT_RESET:
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
            ep_tx_busy_flag = false;
            usbd_ep_start_read(busid, CDC_OUT_EP, cdc_read_buffer, CDC_MAX_MPS);
            rt_kprintf("[USB] Configured (CDC+MSC)\n");
            break;
        case USBD_EVENT_SET_REMOTE_WAKEUP:
            break;
        case USBD_EVENT_CLR_REMOTE_WAKEUP:
            break;
        default:
            break;
    }
}

void usbd_cdc_acm_bulk_out(uint8_t busid, uint8_t ep, uint32_t nbytes)
{
    usbd_ep_start_write(busid, CDC_IN_EP, cdc_read_buffer, nbytes);
}

void usbd_cdc_acm_bulk_in(uint8_t busid, uint8_t ep, uint32_t nbytes)
{
    if ((nbytes % usbd_get_ep_mps(busid, ep)) == 0 && nbytes) {
        usbd_ep_start_write(busid, CDC_IN_EP, NULL, 0);
    } else {
        ep_tx_busy_flag = false;
        usbd_ep_start_read(busid, CDC_OUT_EP, cdc_read_buffer, CDC_MAX_MPS);
    }
}

void usbd_cdc_acm_set_dtr(uint8_t busid, uint8_t intf, bool dtr)
{
    dtr_enable = dtr;
}

void usbd_cdc_acm_set_rts(uint8_t busid, uint8_t intf, bool rts)
{
}

static struct usbd_endpoint cdc_out_ep = {
    .ep_addr = CDC_OUT_EP,
    .ep_cb = usbd_cdc_acm_bulk_out
};

static struct usbd_endpoint cdc_in_ep = {
    .ep_addr = CDC_IN_EP,
    .ep_cb = usbd_cdc_acm_bulk_in
};

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
static struct usbd_interface intf1;
static struct usbd_interface intf2;

void cdc_msc_blkdev_init(uint8_t busid, uintptr_t reg_base)
{
    rt_kprintf("[USB] Initializing CDC+MSC composite device...\n");

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

    usbd_desc_register(busid, &cdc_msc_blkdev_descriptor);
    usbd_add_interface(busid, usbd_cdc_acm_init_intf(busid, &intf0));
    usbd_add_interface(busid, usbd_cdc_acm_init_intf(busid, &intf1));
    usbd_add_endpoint(busid, &cdc_out_ep);
    usbd_add_endpoint(busid, &cdc_in_ep);
    usbd_add_interface(busid, usbd_msc_init_intf(busid, &intf2, MSC_OUT_EP, MSC_IN_EP));
    usbd_initialize(busid, reg_base, usbd_event_handler);

    rt_kprintf("[USB] CDC+MSC composite device initialized, VID=0x%04X, PID=0x%04X\n", USBD_VID, USBD_PID);
}
#endif
