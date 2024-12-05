#ifndef _PICO_STDIO_USB_TUSB_CONFIG_H
#define _PICO_STDIO_USB_TUSB_CONFIG_H

#define CFG_TUSB_RHPORT0_MODE   (OPT_MODE_DEVICE)

#define CFG_TUD_CDC                 1
#define CFG_TUD_MSC                 1
#define CFG_TUD_CDC_RX_BUFSIZE      64
#define CFG_TUD_CDC_TX_BUFSIZE      64
#define CFG_TUD_MSC_BUFSIZE         512

#endif