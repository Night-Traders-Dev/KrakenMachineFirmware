#include "tusb.h"
#include "usb_msc.h"
#include <string.h>

static bool msc_ready = false;
static uint32_t block_count = 128;
static uint16_t block_size = 512;
static uint8_t storage[128 * 512];

void tud_msc_inquiry_cb(uint8_t lun, uint8_t vendor_id[8], uint8_t product_id[16], uint8_t product_rev[4]) {
    memcpy(vendor_id, "KRKN_DEV", 8);
    memcpy(product_id, "MSC_DEV", 16);
    memcpy(product_rev, "1.0", 4);
}

bool tud_msc_test_unit_ready_cb(uint8_t lun) {
    if (lun != 0) {
        tud_msc_set_sense(lun, SCSI_SENSE_ILLEGAL_REQUEST, 0x25, 0x00); // Invalid LUN
        return false;
    }

    return msc_ready; // Return the state of the media
}

void tud_msc_capacity_cb(uint8_t lun, uint32_t *out_block_count, uint16_t *out_block_size) {
    *out_block_count = block_count;
    *out_block_size = block_size;
}

int32_t tud_msc_read10_cb(uint8_t lun, uint32_t lba, uint32_t offset, void *buffer, uint32_t bufsize) {
    if ((lba * block_size + offset + bufsize) > sizeof(storage)) {
        return -1; // Out of bounds
    }

    memcpy(buffer, &storage[lba * block_size + offset], bufsize);
    return bufsize;
}

int32_t tud_msc_write10_cb(uint8_t lun, uint32_t lba, uint32_t offset, uint8_t *buffer, uint32_t bufsize) {
    if ((lba * block_size + offset + bufsize) > sizeof(storage)) {
        return -1; // Out of bounds
    }

    memcpy(&storage[lba * block_size + offset], buffer, bufsize);
    return bufsize;
}

bool tud_msc_start_stop_cb(uint8_t lun, uint8_t power_condition, bool start, bool load_eject) {
    (void)power_condition; // Not used
    if (load_eject) {
        msc_ready = start; // Load or eject media
    }
    return true;
}

int32_t tud_msc_scsi_cb(uint8_t lun, const uint8_t scsi_cmd[16], void *buffer, uint16_t bufsize) {
    tud_msc_set_sense(lun, SCSI_SENSE_ILLEGAL_REQUEST, 0x20, 0x00); // Command not supported
    return -1;
}

/*void usb_init() {
    tusb_init();
}*/
