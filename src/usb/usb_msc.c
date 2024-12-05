#include "usb_msc.h"
#include "tusb.h"
#include <string.h>

// Example RAM disk: 64 KB
#define DISK_BLOCK_SIZE  512
#define DISK_BLOCK_NUM   128  // Total size = 64 KB
static uint8_t ram_disk[DISK_BLOCK_SIZE * DISK_BLOCK_NUM];

// Callback: Read sectors from storage
int32_t tud_msc_read10_cb(uint8_t lun, uint32_t lba, uint32_t offset, void *buffer, uint32_t bufsize) {
    memcpy(buffer, &ram_disk[lba * DISK_BLOCK_SIZE + offset], bufsize);
    return bufsize;
}

// Callback: Write sectors to storage
int32_t tud_msc_write10_cb(uint8_t lun, uint32_t lba, uint32_t offset, const void *buffer, uint32_t bufsize) {
    memcpy(&ram_disk[lba * DISK_BLOCK_SIZE + offset], buffer, bufsize);
    return bufsize;
}

// Callback: Get disk capacity
void tud_msc_capacity_cb(uint8_t lun, uint32_t *block_count, uint16_t *block_size) {
    *block_count = DISK_BLOCK_NUM;
    *block_size = DISK_BLOCK_SIZE;
}

// Callback: Check if write-protected
bool tud_msc_is_writable_cb(uint8_t lun) {
    return true;
}

// Callback: Handle SCSI commands
int32_t tud_msc_scsi_cb(uint8_t lun, const uint8_t scsi_cmd[16], void *buffer, uint16_t bufsize) {
    return -1;  // SCSI commands not supported
}
