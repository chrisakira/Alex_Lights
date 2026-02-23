/**
 * @file    DataTable.h
 * @brief   Data Table header file
 *
 * The Data Table is responsible to offer the functions to access the tag table, including public method to initialize
 * it and to write/read data.
 *
 * 
 */

#ifndef __DATA_TABLE_H__
#define __DATA_TABLE_H__

#include <stddef.h>
#include <stdlib.h>
#include "ComponentCommonDefinitions.h"
#include "DataTags.h"

/**
 * @defgroup DataTableVariables 2.2.1 List of Data Table Variables
 * @defgroup DataTableFunctions 2.2.2 List of Data Table Functions
 */

/**
 * @brief Initializes the data table.
 * @ingroup DataTableFunctions
 *
 * Initializes the internal tag table with the configured Aplication data information.
 * Call this once before any debug registration or read/write operations.
 *
 * @return void
 */
uint8_t init_datatable(uint8_t device_id);

/**
 * @brief Registers a debug variable for a given data tag.
 * @ingroup DataTableFunctions
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
uint8_t register_debug_var(const uint32_t data_tag, void* data, types_et data_type);

/**
 * @brief Retrieves the Device ID associated with a given data tag.
 * @ingroup DataTableFunctions
 *
 * Looks up the tag in the table and, if the entry is valid and consistent,
 * returns the owning/associated Device IDentifier. If the tag is out of range
 * or inconsistent with the table entry, returns NOK.
 *
 * @param[in] data_tag Tag identifier. Must be less than TAGS_MAX.
 * @return Device ID associated with the data_tag if valid; NOK if invalid.
 */
uint8_t get_device_id_from_tag(const uint32_t data_tag);

/**
 * @brief Gets the payload size (in bytes) for a given data tag.
 * @ingroup DataTableFunctions
 *
 * Returns the size associated with the tag's declared type by consulting
 * the tag table and calling get_size(type). If the tag index is invalid,
 * the function returns NOK to indicate an unknown/invalid size.
 *
 * @param[in] data_tag Tag identifier. Must be less than TAGS_MAX.
 * @return Size in bytes if the tag is valid; NOK if invalid.
 */
uint8_t get_tag_size(const uint32_t data_tag);

/**
 * @brief Reads the value of a given data tag into a provided buffer.
 * @ingroup DataTableFunctions
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
uint8_t read_from_datatable(const uint32_t data_tag, void* data, size_t size);

/**
 * @brief Writes a value to a given data tag if DEVICE access is allowed.
 * @ingroup DataTableFunctions
 *
 * Copies the provided buffer into the tag’s registered storage when:
 *  the tag is valid and registered, the source pointer is non-NULL,
 *  the given size matches the tag’s declared size.
 *
 * @param[in] data_tag Tag identifier. Must be less than TAGS_MAX.
 * @param[in] data Pointer to the source buffer. Must not be NULL.
 * @param[in] size Size of the source buffer in bytes. Must match the tag size.
 * @return OK if write is successful; NOK otherwise.
 */
uint8_t write_to_datatable(const uint32_t data_tag, const void* data, size_t size);

/**
 * @brief Returns the current registered Device ID
 * @ingroup DataTableFunctions
 *
 * Returns the global variable that stores the currently active Device ID.
 *
 * @return current_device_id. NOK if no DEVICE was registered.
 */
uint8_t get_current_device_id(void);

/**
 * @brief Sets the current Device ID.
 * @ingroup DataTableFunctions
 *
 * Updates the global variable that stores the currently active Device ID.
 *
 * @param device_id The Device ID to set as the current active identifier.
 *
 * @return void
 */
void set_current_device_id(uint8_t device_id);

#endif  // __DATA_TABLE_H__
