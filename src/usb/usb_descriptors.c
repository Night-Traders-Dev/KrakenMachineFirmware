#include "tusb.h"

#define EPNUM_MSC_OUT 0x01
#define EPNUM_MSC_IN  0x81

// Device descriptor
tusb_desc_device_t const desc_device = {
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    .bcdUSB             = 0x0200,
    .bDeviceClass       = TUSB_CLASS_MISC,
    .bDeviceSubClass    = MISC_SUBCLASS_COMMON,
    .bDeviceProtocol    = MISC_PROTOCOL_IAD,
    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,

    .idVendor           = 0xCAFE,
    .idProduct          = 0x4000,
    .bcdDevice          = 0x0100,

    .iManufacturer      = 0x01,
    .iProduct           = 0x02,
    .iSerialNumber      = 0x03,

    .bNumConfigurations = 0x01
};

// Configuration descriptor
uint8_t const desc_configuration[] = {
    TUD_CONFIG_DESCRIPTOR(1, 2, 0, TUD_CONFIG_DESC_LEN + TUD_MSC_DESC_LEN, 0x80, 100),
    TUD_MSC_DESCRIPTOR(0, 4, EPNUM_MSC_OUT, EPNUM_MSC_IN, 64)
};

uint8_t const *tud_descriptor_device_cb(void) {
    return (uint8_t const *)&desc_device;
}

uint8_t const *tud_descriptor_configuration_cb(uint8_t index) {
    (void)index;
    return desc_configuration;
}

char const *string_desc[] = {
    (const char[]){0x09, 0x04}, // Supported language is English (0x0409)
    "SLAPCHOP",                // Manufacturer
    "Kraken Device",           // Product
    "123456789ABCDEF"          // Serial number
};

uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
    (void)langid;
    if (index == 0) return (uint16_t const *)string_desc[0];

    if (!(index < sizeof(string_desc) / sizeof(string_desc[0]))) return NULL;

    static uint16_t desc[32];
    char const *str = string_desc[index];

    uint8_t len = strlen(str);
    if (len > 31) len = 31;

    desc[0] = (TUSB_DESC_STRING << 8) | (2 * len + 2);
    for (uint8_t i = 0; i < len; i++) desc[1 + i] = str[i];
    return desc;
}
