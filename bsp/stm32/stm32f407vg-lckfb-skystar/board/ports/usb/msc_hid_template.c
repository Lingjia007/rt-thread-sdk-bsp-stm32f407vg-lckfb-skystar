#include <rtconfig.h>
#ifdef BSP_USING_USB_MSC_HID
#include "usbd_core.h"
#include "usbd_msc.h"
#include "usbd_hid.h"
#include "usb_config.h"

#define MSC_OUT_EP 0x01
#define MSC_IN_EP  0x81
#define HID_INT_EP 0x82

#ifndef USBD_VID
#define USBD_VID           0x0483
#endif

#ifndef USBD_PID
#define USBD_PID           0x5770
#endif

#define USBD_MAX_POWER     100
#define USBD_LANGID_STRING 1033

#define HID_CUSTOM_REPORT_DESC_SIZE 38

#ifdef CONFIG_USB_HS
#define MSC_MAX_MPS     512
#define HID_MAX_MPS     1024
#define HID_IN_INTERVAL 1
#else
#define MSC_MAX_MPS     64
#define HID_MAX_MPS     64
#define HID_IN_INTERVAL 1
#endif

#define USB_CONFIG_SIZE (9 + MSC_DESCRIPTOR_LEN + HID_MOUSE_DESCRIPTOR_LEN)

#ifndef CONFIG_USBDEV_MSC_MANUFACTURER_STRING
#define CONFIG_USBDEV_MSC_MANUFACTURER_STRING "LCKFB"
#endif

#ifndef CONFIG_USBDEV_MSC_PRODUCT_STRING
#define CONFIG_USBDEV_MSC_PRODUCT_STRING "SKYSTAR MSC+HID"
#endif

static const uint8_t device_descriptor[] = {
    USB_DEVICE_DESCRIPTOR_INIT(USB_2_0, 0x00, 0x00, 0x00, USBD_VID, USBD_PID, 0x0200, 0x01)
};

static const uint8_t config_descriptor[] = {
    USB_CONFIG_DESCRIPTOR_INIT(USB_CONFIG_SIZE, 0x02, 0x01, USB_CONFIG_BUS_POWERED, USBD_MAX_POWER),
    MSC_DESCRIPTOR_INIT(0x00, MSC_OUT_EP, MSC_IN_EP, MSC_MAX_MPS, 0x02),
    HID_MOUSE_DESCRIPTOR_INIT(0x01, 0x00, HID_CUSTOM_REPORT_DESC_SIZE, HID_INT_EP, HID_MAX_MPS, HID_IN_INTERVAL),
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
    'M', 'S', 'C', 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static struct usb_msosv1_descriptor msosv1_desc = {
    .string = msosv1_string,
    .vendor_code = 0x01,
    .compat_id = msosv1_compat_id,
    .comp_id_property = NULL,
};

const struct usb_descriptor msc_hid_descriptor = {
    .device_descriptor_callback = device_descriptor_callback,
    .config_descriptor_callback = config_descriptor_callback,
    .string_descriptor_callback = string_descriptor_callback,
    .msosv1_descriptor = &msosv1_desc
};

static const uint8_t hid_custom_report_desc[HID_CUSTOM_REPORT_DESC_SIZE] = {
    0x06, 0x00, 0xff,
    0x09, 0x01,
    0xa1, 0x01,
    0x85, 0x02,
    0x09, 0x01,
    0x15, 0x00,
    0x26, 0xff, 0x00,
    0x95, 0x40 - 1,
    0x75, 0x08,
    0x81, 0x02,
    0x85, 0x01,
    0x09, 0x01,
    0x15, 0x00,
    0x26, 0xff, 0x00,
    0x95, 0x40 - 1,
    0x75, 0x08,
    0x91, 0x02,
    0xC0
};

#define HID_STATE_IDLE 0
#define HID_STATE_BUSY 1

static volatile uint8_t hid_state = HID_STATE_IDLE;

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
            hid_state = HID_STATE_IDLE;
            rt_kprintf("[USB] Configured (MSC+HID)\n");
            break;
        case USBD_EVENT_SET_REMOTE_WAKEUP:
            break;
        case USBD_EVENT_CLR_REMOTE_WAKEUP:
            break;
        default:
            break;
    }
}

static void usbd_hid_int_callback(uint8_t busid, uint8_t ep, uint32_t nbytes)
{
    hid_state = HID_STATE_IDLE;
}

static struct usbd_endpoint hid_in_ep = {
    .ep_cb = usbd_hid_int_callback,
    .ep_addr = HID_INT_EP
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

void msc_hid_init(uint8_t busid, uintptr_t reg_base)
{
    rt_kprintf("[USB] Initializing MSC+HID composite device...\n");

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

    usbd_desc_register(busid, &msc_hid_descriptor);
    usbd_add_interface(busid, usbd_msc_init_intf(busid, &intf0, MSC_OUT_EP, MSC_IN_EP));
    usbd_add_interface(busid, usbd_hid_init_intf(busid, &intf1, hid_custom_report_desc, HID_CUSTOM_REPORT_DESC_SIZE));
    usbd_add_endpoint(busid, &hid_in_ep);
    usbd_initialize(busid, reg_base, usbd_event_handler);

    rt_kprintf("[USB] MSC+HID composite device initialized, VID=0x%04X, PID=0x%04X\n", USBD_VID, USBD_PID);
}
#endif
