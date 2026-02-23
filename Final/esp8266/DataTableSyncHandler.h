/**
 * @file    DataTableSyncHandler.h
 * @brief   Data Table Sync Handler header file
 *
 * The DataTable Handler serves as a middleware layer between the DataTable and other software components.
 * It abstracts direct access to the DataTable and enforces access control policies to ensure secure interactions.
 *
 * 
 */

#ifndef __DATA_TABLE_SYNC_HANDLER_H__
#define __DATA_TABLE_SYNC_HANDLER_H__

#include <stddef.h>
#include <stdlib.h>
#include "ComponentCommonDefinitions.h"
#include "DataTable.h"
#include "DataTags.h"

/**
 * @defgroup DataTableSyncHandlerVariables 2.2.7 List of Data Table Functions
 * @defgroup DataTableSyncHandlerStructures 2.2.8 List of Data Table Functions
 * @defgroup DataTableSyncHandlerDefines 2.2.9 List of Data Table Functions
 * @defgroup DataTableSyncHandlerFunctions 2.2.10 List of Data Table Functions
 * @defgroup DataTableSyncHandlerTypedefs 2.2.11 List of Data Table Typedefs
 */

/**
 * @def BROADCAST_MSG_MAX_PAYLOAD_SIZE
 * @ingroup DataTableSyncHandlerDefines
 * @brief Maximum payload size for broadcast messages in bytes.
 */
#define BROADCAST_MSG_MAX_PAYLOAD_SIZE 200U

/**
 * @struct broadcast_msg_st
 * @ingroup DataTableSyncHandlerStructures
 * @brief Structure that encapsulates a broadcast message with payload and metadata.
 *
 * This structure is used to transmit and receive broadcast data between devices.
 * It contains the payload data, the chunk index for chunked transmission,
 * the source device ID, and the actual payload size.
 */
typedef struct
{
    /// Payload data buffer with maximum size of BROADCAST_MSG_MAX_PAYLOAD_SIZE bytes.
    uint8_t payload[BROADCAST_MSG_MAX_PAYLOAD_SIZE];
    /// Chunk index for chunked transmission (0 = first chunk, 1 = second, etc.).
    /// Set to INVALID_UINT32 when this is the last/only chunk.
    uint32_t chunk_index;
    /// Source device ID that originated this broadcast message.
    uint8_t source_device_id;
    /// Actual size of valid data in the payload buffer.
    uint16_t payload_size;
} broadcast_msg_st;

/**
 * @typedef send_broadcast_fn
 * @ingroup DataTableSyncHandlerTypedefs
 * @brief Function pointer type for sending broadcast messages.
 *
 * This function pointer type defines the signature for a send function
 * that transmits a broadcast message structure to other devices.
 *
 * @param[in] msg Pointer to the broadcast message structure to send.
 * @return OK if the message was sent successfully, NOK otherwise.
 */
typedef uint8_t (*send_broadcast_fn)(const broadcast_msg_st* msg);

/**
 * @typedef receive_broadcast_fn
 * @ingroup DataTableSyncHandlerTypedefs
 * @brief Function pointer type for receiving broadcast messages.
 *
 * This function pointer type defines the signature for a receive function
 * that receives a broadcast message structure from other devices.
 *
 * @param[out] msg Pointer to the broadcast message structure where received data will be stored.
 * @return OK if a message was received successfully, NOK if no data is available or error occurred.
 */
typedef uint8_t (*receive_broadcast_fn)(broadcast_msg_st* msg);

/**
 * @brief Initializes the data table sync handler with communication function pointers.
 * @ingroup DataTableSyncHandlerFunctions
 *
 * This function initializes the sync handler by registering the send and receive
 * function pointers that will be used for broadcast communication. It also calls
 * init_data_table_sync_handler() internally to set up the tag sorting structures.
 *
 * @param[in] send_fn Function pointer for sending broadcast data. Can be NULL if only receiving.
 * @param[in] receive_fn Function pointer for receiving broadcast data. Can be NULL if only sending.
 * @param[in] max_buffer_size Maximum buffer size to use for broadcast operations.
 * @return OK if initialization is successful, NOK otherwise.
 */
uint8_t init_data_table_sync_handler_comm(send_broadcast_fn send_fn, receive_broadcast_fn receive_fn, size_t max_buffer_size);

/**
 * @brief Main loop function to process broadcast send and receive operations.
 * @ingroup DataTableSyncHandlerFunctions
 *
 * This function should be called periodically to handle broadcast synchronization.
 * It will:
 * 1. Build and send the broadcast buffer for the current device using the registered send function.
 * 2. Receive broadcast data from other devices using the registered receive function.
 * 3. Write received data to the data table.
 *
 * The function handles chunked transmission for large data sets automatically.
 *
 * @return OK if the loop executed successfully, NOK if an error occurred.
 */
uint8_t data_table_sync_handler_loop(void);

/**
 * @brief Initializes the data table handler and sets the current Device ID.
 * @ingroup DataTableSyncHandlerFunctions
 *
 * Creates the DEVICE_range matrix.
 * The DataTable must have been initialized for this function
 * to properly create the DEVICE_range matrix.
 *
 * @return void
 */
void init_data_table_sync_handler(void);

/**
 * @brief Builds the broadcast buffer for a given DEVICE by reading all its tags.
 * @ingroup DatabaseFunctions
 *
 * Iterates over the row of tags in DEVICE_range for @p device_id. For each valid tag ID,
 * reads its current value from the datatable into a temporary 8-bit buffer and then
 * stores that value in broadcast_buffer. Supports breaking the broadcast into smaller
 * chunks using max_output_size and position_id parameters.
 *
 * @param[in] broadcast_buffer Buffer where the broadcast data shall be written into.
 * @param[in] buffer_size Total buffer size available for the device.
 * @param[in] device_id Device IDentification number.
 * @param[in] max_output_size Maximum size of the output buffer chunk. If 0, uses full buffer_size.
 * @param[in,out] position_id Pointer to position identifier indicating which chunk to build.
 *                            On input: the chunk index (0 = first chunk, 1 = second chunk, etc.).
 *                            On output: updated to next chunk index, or set to INVALID_UINT32 if complete.
 * @return the size of the message written into the broadcast_buffer if successful, NOK otherwise
 */
uint32_t build_broadcast_buffer(uint8_t* broadcast_buffer, size_t buffer_size, uint8_t device_id, size_t max_output_size, uint32_t* position_id);

/**
 * @brief Builds the debug broadcast buffer for a given DEVICE by reading all its tags.
 * @ingroup DatabaseFunctions
 *
 * Iterates over the row of tags in DEVICE_range for @p device_id. For each valid tag ID,
 * reads its current value from the datatable into a temporary 8-bit buffer and then
 * stores that value in broadcast_buffer.
 *
 * @param[in] broadcast_buffer Buffer where the broadcast data shall be written into.
 * @param[in] buffer_size Buffer size.
 * @param[in] device_id Device IDentification number.
 * @return the size of the message written into the broadcast_buffer if successful, NOK otherwise
 */
uint32_t build_debug_broadcast_buffer(uint8_t* broadcast_buffer, size_t buffer_size, uint8_t device_id);

/**
 * @brief Writes the broadcast buffer back to the datatable for a given DEVICE.
 * @ingroup DatabaseFunctions
 *
 * Parses the broadcast_buffer, using the DEVICE_range matrix, and writes the parsed data into the DataTable.
 * Supports chunked writing for large datasets using max_input_size and position_id parameters.
 *
 * @param[in] broadcast_buffer Buffer containing all the new Data to be parsed and written into the DataTable.
 * @param[in] buffer_size Total buffer size available for the device.
 * @param[in] incoming_device_id Target Device IDentifier whose broadcast buffer will be written to the datatable.
 * @param[in] max_input_size Maximum size of the input buffer chunk. If 0, uses full buffer_size.
 * @param[in,out] position_id Pointer to position identifier indicating which chunk to write.
 *                            On input: the chunk index (0 = first chunk, 1 = second chunk, etc.).
 *                            On output: updated to next chunk index, or set to INVALID_UINT32 if complete.
 * @return OK if successful, NOK otherwise
 */
uint8_t write_broadcast_buffer(uint8_t* broadcast_buffer, size_t buffer_size, uint8_t incoming_device_id, size_t max_input_size, uint32_t* position_id);

/**
 * @brief Writes the debug broadcast buffer back to the datatable for a given DEVICE.
 * @ingroup DatabaseFunctions
 *
 * Parses the broadcast_buffer, using the DEVICE_range matrix, and writes the parsed data into the DataTable.
 *
 * @param[in] broadcast_buffer Buffer containing all the new Data to be parsed and written into the DataTable.
 * @param[in] buffer_size Buffer size.
 * @param[in] incoming_device_id Target Device IDentifier whose broadcast buffer will be written to the datatable.
 * @return OK if successful, NOK otherwise
 */
uint8_t write_debug_broadcast_buffer(uint8_t* broadcast_buffer, size_t buffer_size, uint8_t incoming_device_id);

/**
 * @brief Debug function to get device ID list info for a specific device.
 * @ingroup DataTableSyncHandlerFunctions
 *
 * @param[in] device_id Device ID to query.
 * @param[out] index Pointer to store the index value.
 * @param[out] size Pointer to store the size value.
 */
void debug_get_device_id_list_info(uint8_t device_id, uint32_t* index, uint32_t* size);

/**
 * @brief Debug function to get sorted tag list entry.
 * @ingroup DataTableSyncHandlerFunctions
 *
 * @param[in] list_index Index in the sorted tag list.
 * @param[out] tag Pointer to store the tag value.
 * @param[out] device_id Pointer to store the device ID.
 */
void debug_get_sorted_tag_entry(uint32_t list_index, uint32_t* tag, uint8_t* device_id);

#endif  // __DATA_TABLE_SYNC_HANDLER_H__
