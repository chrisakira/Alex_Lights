/**
 * @file    DataTableHandler.h
 * @brief   Data Table Handler header file
 *
 * The DataTable Handler serves as a middleware layer between the DataTable and other software components.
 * It abstracts direct access to the DataTable and enforces access control policies to ensure secure interactions.
 *
 * 
 */

#ifndef __DATA_TABLE_HANDLER_H__
#define __DATA_TABLE_HANDLER_H__

#include <stddef.h>
#include <stdlib.h>
#include "ComponentCommonDefinitions.h"
#include "DataTags.h"

/**
 * @defgroup DataTableHandlerFunctions 2.2.6 List of Data Table Functions
 */

/**
 * @brief Initializes the data table handler and sets the current Device ID.
 * @ingroup DataTableHandlerFunctions
 *
 * Initializes the internal tag table by requesting/populating default/test
 * entries (via get_datatable_test_data) and sets the module-wide DEVICE
 * identifier. Call this once before any registration or read/write operations.
 *
 * @param[in] device_id Identifier of the current DEVICE.
 * @return void
 */
uint8_t init_data_table_handler(uint8_t device_id);

/**
 * @brief Registers a debug variable for a given data tag.
 * @ingroup DataTableHandlerFunctions
 *
 * Binds a caller-provided storage pointer to the tag entry so future reads
 * and writes operate directly on that memory. Registration succeeds only if
 * the tag is valid, the pointer is non-NULL, the tag was not yet registered,
 * and the tag index matches the table’s tag field. The pointer’s lifetime and
 * size must be managed by the caller.
 *
 * @param[in] data_tag Tag identifier. Must be less than TAGS_MAX.
 * @param[in] data Pointer to the variable to register. Must not be NULL.
 * @param[in] data_type Type of the variable (used for size validation).
 * @return OK if registration is successful; NOK otherwise.
 */
uint8_t register_debug_var_handler(const uint32_t data_tag, void* data, types_et data_type);

/**
 * @brief Reads the value of a given data tag into a provided buffer.
 * @ingroup DataTableHandlerFunctions
 *
 * Copies exactly get_size(tag.type) bytes from the tag’s registered storage
 * into the destination buffer, provided the tag is valid and registered,
 * the buffer is non-NULL, and the size matches the tag’s declared size.
 *
 * @param[in]  data_tag Tag identifier. Must be less than TAGS_MAX.
 * @param[out] data Pointer to the destination buffer. Must not be NULL.
 * @param[in]  size Size of the destination buffer in bytes. Must match the tag size.
 * @return OK if read is successful; NOK otherwise.
 */
uint8_t get_data_table(const uint32_t data_tag, void* data, size_t size);

/**
 * @brief Writes a value to a given data tag if DEVICE access is allowed.
 * @ingroup DataTableHandlerFunctions
 *
 * Copies the provided buffer into the tag’s registered storage when:
 *  the tag is valid and registered, the source pointer is non-NULL,
 *  the given size matches the tag’s declared size, and the write is authorized
 *  (tag’s DEVICE matches the caller’s device_id or is ANY_DEVICE).
 *
 * @param[in] data_tag Tag identifier. Must be less than TAGS_MAX.
 * @param[in] data Pointer to the source buffer. Must not be NULL.
 * @param[in] size Size of the source buffer in bytes. Must match the tag size.
 * @return OK if write is successful; NOK otherwise.
 */
uint8_t write_data_table(const uint32_t data_tag, void* data, size_t size);

#endif  // __DATA_TABLE_HANDLER_H__
