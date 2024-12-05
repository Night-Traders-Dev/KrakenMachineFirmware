#ifndef USB_MSC_H
#define USB_MSC_H

#include <stdint.h>
#include <stdbool.h>
#include "tusb.h"

// Define disk properties
#define DISK_BLOCK_SIZE  512
#define DISK_BLOCK_NUM   128  // Total size = 64 KB

// Function declarations for USB MSC callbacks
int32_t tud_msc_read10_cb(uint8_t lun, uint32_t lba, uint32_t offset, void *buffer, uint32_t bufsize);
int32_t tud_msc_write10_cb(uint8_t lun, uint32_t lba, uint32_t offset, const void *buffer, uint32_t bufsize);
void tud_msc_capacity_cb(uint8_t lun, uint32_t *block_count, uint16_t *block_size);
bool tud_msc_is_writable_cb(uint8_t lun);
int32_t tud_msc_scsi_cb(uint8_t lun, const uint8_t scsi_cmd[16], void *buffer, uint16_t bufsize);

#endif // USB_MSC_H
