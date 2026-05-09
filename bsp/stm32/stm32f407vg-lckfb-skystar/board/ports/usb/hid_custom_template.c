#include <rtconfig.h>
#ifdef BSP_USING_USB_HID
#include "usbd_core.h"
#include "usbd_hid.h"
#include "usb_config.h"

#define HID_IN_EP  0x81
#define HID_OUT_EP 0x02

#ifndef USBD_VID
#define USBD_VID           0x0483
#endif

#ifndef USBD_PID
#define USBD_PID           0x5750
#endif

#define USBD_MAX_POWER     100
#define USBD_LANGID_STRING 1033

#define HID_CUSTOM_REPORT_DESC_SIZE 38

#ifdef CONFIG_USB_HS
#define HID_MAX_MPS        1024
#define HID_IN_INTERVAL    1
#else
#define HID_MAX_MPS        64
#define HID_IN_INTERVAL    1
#endif

#define USB_CONFIG_SIZE (9 + HID_CUSTOM_INOUT_DESCRIPTOR_LEN)

static const uint8_t device_descriptor[] = {
    USB_DEVICE_DESCRIPTOR_INIT(USB_2_0, 0x00, 0x00, 0x00, USBD_VID, USBD_PID, 0x0200, 0x01)
};

static const uint8_t config_descriptor[] = {
    USB_CONFIG_DESCRIPTOR_INIT(USB_CONFIG_SIZE, 0x01, 0x01, USB_CONFIG_BUS_POWERED, USBD_MAX_POWER),
    HID_CUSTOM_INOUT_DESCRIPTOR_INIT(0x00, 0x00, HID_CUSTOM_REPORT_DESC_SIZE, HID_OUT_EP, HID_IN_EP, HID_MAX_MPS, HID_IN_INTERVAL),
};

static const char *string_descriptors[] = {
    (const char[]){ 0x09, 0x04 },
    "LCKFB",
    "SKYSTAR Custom HID",
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
    'H', 'I', 'D', 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static struct usb_msosv1_descriptor msosv1_desc = {
    .string = msosv1_string,
    .vendor_code = 0x01,
    .compat_id = msosv1_compat_id,
    .comp_id_property = NULL,
};

const struct usb_descriptor hid_descriptor = {
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

USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t hid_read_buffer[HID_MAX_MPS];
USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t hid_write_buffer[HID_MAX_MPS];

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
            usbd_ep_start_read(busid, HID_OUT_EP, hid_read_buffer, HID_MAX_MPS);
            rt_kprintf("[USB] Configured (HID)\n");
            break;
        case USBD_EVENT_SET_REMOTE_WAKEUP:
            break;
        case USBD_EVENT_CLR_REMOTE_WAKEUP:
            break;
        default:
            break;
    }
}

static void usbd_hid_custom_in_callback(uint8_t busid, uint8_t ep, uint32_t nbytes)
{
    hid_state = HID_STATE_IDLE;
}

static void usbd_hid_custom_out_callback(uint8_t busid, uint8_t ep, uint32_t nbytes)
{
    usbd_ep_start_read(busid, ep, hid_read_buffer, HID_MAX_MPS);
    hid_read_buffer[0] = 0x02;
    usbd_ep_start_write(busid, HID_IN_EP, hid_read_buffer, nbytes);
}

static struct usbd_endpoint hid_in_ep = {
    .ep_cb = usbd_hid_custom_in_callback,
    .ep_addr = HID_IN_EP
};

static struct usbd_endpoint hid_out_ep = {
    .ep_cb = usbd_hid_custom_out_callback,
    .ep_addr = HID_OUT_EP
};

static struct usbd_interface intf0;

void hid_custom_init(uint8_t busid, uintptr_t reg_base)
{
    rt_kprintf("[USB] Initializing HID device...\n");

    usbd_desc_register(busid, &hid_descriptor);
    usbd_add_interface(busid, usbd_hid_init_intf(busid, &intf0, hid_custom_report_desc, HID_CUSTOM_REPORT_DESC_SIZE));
    usbd_add_endpoint(busid, &hid_in_ep);
    usbd_add_endpoint(busid, &hid_out_ep);
    usbd_initialize(busid, reg_base, usbd_event_handler);

    rt_kprintf("[USB] HID device initialized, VID=0x%04X, PID=0x%04X\n", USBD_VID, USBD_PID);
}
#endif
