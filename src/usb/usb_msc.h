#ifndef USB_MSC_H
#define USB_MSC_H

#include <stdbool.h>
#include <stdint.h>
#include "tusb.h"

// MSC Configuration Constants
#define MSC_LUN_COUNT 1 // Number of logical units supported
#define SCSI_SENSE_NOT_READY 0x02
#define SCSI_ASC_MEDIUM_NOT_PRESENT 0x3A
#define SCSI_ASCQ_CAUSE_NOT_REPORTABLE 0x00


// Function Prototypes

/**
 * @brief Initializes the USB MSC functionality.
 */
void usb_init(void);

/**
 * @brief Main USB task for handling MSC operations.
 */
//void usb_task(void);

/**
 * @brief Callback for handling MSC SCSI inquiry command.
 * @param lun Logical unit number.
 * @param vendor_id Vendor ID string (8 bytes).
 * @param product_id Product ID string (16 bytes).
 * @param product_rev Product revision string (4 bytes).
 */
void tud_msc_inquiry_cb(uint8_t lun, uint8_t vendor_id[8], uint8_t product_id[16], uint8_t product_rev[4]);

/**
 * @brief Callback to test if the unit is ready.
 * @param lun Logical unit number.
 * @return True if the unit is ready, false otherwise.
 */
bool tud_msc_test_unit_ready_cb(uint8_t lun);

/**
 * @brief Callback to get capacity of the storage.
 * @param lun Logical unit number.
 * @param block_count Number of blocks.
 * @param block_size Size of each block in bytes.
 */
void tud_msc_capacity_cb(uint8_t lun, uint32_t *block_count, uint16_t *block_size);

/**
 * @brief Callback to handle SCSI read operations.
 * @param lun Logical unit number.
 * @param lba Logical block address.
 * @param buffer Buffer to store the data.
 * @param bufsize Buffer size in bytes.
 * @return Number of bytes read.
 */
int32_t tud_msc_read_cb(uint8_t lun, uint32_t lba, void *buffer, uint32_t bufsize);

/**
 * @brief Callback to handle SCSI write operations.
 * @param lun Logical unit number.
 * @param lba Logical block address.
 * @param buffer Buffer containing the data to write.
 * @param bufsize Buffer size in bytes.
 * @return Number of bytes written.
 */
int32_t tud_msc_write_cb(uint8_t lun, uint32_t lba, const void *buffer, uint32_t bufsize);

/**
 * @brief Callback to handle start/stop unit command.
 * @param lun Logical unit number.
 * @param power_condition Power condition.
 * @param start Whether to start (true) or stop (false) the unit.
 * @param load_eject Whether to load (true) or eject (false) the media.
 * @return True if the command is supported, false otherwise.
 */
bool tud_msc_start_stop_cb(uint8_t lun, uint8_t power_condition, bool start, bool load_eject);

#endif // USB_MSC_H
