/**
 * @file DataTableSyncHandler.c
 * @brief Source Code of Data Table Handler
 */

#include "DataTableSyncHandler.h"
#include "DataTable.h"

#include <string.h>


/**
 * @def DATA_TABLE_MAX_NUMBER_OF_DEVICES
 * @ingroup DataTableSyncHandlerDefines
 * @brief Maximum number of DEVICEs to configure the Data Table tag table.
 */
#define DATA_TABLE_MAX_NUMBER_OF_DEVICES 34

/**
 * @struct tag_item_st
 * @ingroup DataTableSyncHandlerStructures
 * @brief Represents an entry in the sorted tag list.
 *
 * This structure associates a Data Table tag with its corresponding Device ID.
 * It is primarily used for organizing and synchronizing tags within the Data Table
 * during operations such as lookup, sorting, or synchronization.
 */
typedef struct
{
    /// Data Table tag identifier.
    uint32_t tag;
    /// Device ID associated with the tag.
    uint8_t device_id;
} tag_item_st;

/**
 * @struct tag_item_st
 * @ingroup DataTableSyncHandlerStructures
 * @brief Represents a block of items associated with a specific Device ID.
 *
 * This structure defines the starting index and size of a contiguous block
 * of items that belong to the same Device ID within the sorted list.
 * It is useful for efficient grouping and iteration over items related to a single DEVICE
 */
typedef struct
{
    /// Starting index of the Device ID block.
    uint32_t index;
    /// Size (number of items) in the Device ID block.
    uint32_t size;
} device_id_item_st;

/**
 * @struct sync_handler_comm_st
 * @ingroup DataTableSyncHandlerStructures
 * @brief Holds the communication function pointers and state for the sync handler.
 *
 * This structure stores the registered send/receive function pointers and
 * configuration for the broadcast communication layer.
 */
typedef struct
{
    /// Function pointer for sending broadcast data.
    send_broadcast_fn send_fn;
    /// Function pointer for receiving broadcast data.
    receive_broadcast_fn receive_fn;
    /// Maximum buffer size for broadcast operations.
    size_t max_buffer_size;
    /// Current position ID for chunked sending (per device).
    uint32_t send_position_id;
    /// Flag indicating if the comm layer has been initialized.
    uint8_t initialized;
} sync_handler_comm_st;

/**
 * @var device_id_list[DATA_TABLE_MAX_NUMBER_OF_DEVICES]
 * @ingroup DataTableSyncHandlerVariables
 * @brief Index/size map for each Device ID within the sorted tag list.
 *
 * This array stores, for each Device ID, the start index and the block size
 * in the @ref sorted_tag_list where tags associated with that Device ID are grouped.
 * It enables fast lookup and iteration over contiguous tag ranges per DEVICE,
 * facilitating synchronization and data table operations.
 *
 */
static device_id_item_st device_id_list[DATA_TABLE_MAX_NUMBER_OF_DEVICES] = {0U};

/**
 * @var sorted_tag_list[TAG_DEBUG_1_DEVICE_1]
 * @ingroup DataTableSyncHandlerVariables
 * @brief Primary sorted list of Data Table tags and their associated Device IDs.
 *
 * This array contains the sorted mapping of Data Table tags to
 * their corresponding Device IDs used during normal (non-debug) operation.
 * The list is organized to group entries by Device ID so that the ranges
 * can be indexed via @ref device_id_list.
 */
static tag_item_st sorted_tag_list[TAG_DEBUG_1_DEVICE_1] = {0};

/**
 * @var debug_device_id_list[DATA_TABLE_MAX_NUMBER_OF_DEVICES]
 * @ingroup DataTableSyncHandlerVariables
 * @brief Debug-only index/size map for Device ID blocks in the debug tag list.
 *
 * Mirrors @ref device_id_list but references ranges within
 * @ref debug_sorted_tag_list. This structure is used for diagnostics,
 * validation, and tracing scenarios where a separate debug tag set is
 * required without affecting the primary runtime structures.
 */
static device_id_item_st debug_device_id_list[DATA_TABLE_MAX_NUMBER_OF_DEVICES] = {0U};

/**
 * @var debug_sorted_tag_list[TAG_MAX - TAG_DEBUG_1_DEVICE_1]
 * @ingroup DataTableSyncHandlerVariables
 * @brief Secondary (debug) sorted list of Data Table tags not covered by the primary list.
 *
 * Holds the remaining portion of the tag space used for debugging,
 * testing, or instrumentation that is separate from @ref sorted_tag_list.
 * Entries are similarly organized by Device ID so that contiguous ranges
 * can be indexed via @ref debug_device_id_list.
 */
static tag_item_st debug_sorted_tag_list[TAG_MAX - TAG_DEBUG_1_DEVICE_1] = {0};

/**
 * @var sync_handler_comm
 * @ingroup DataTableSyncHandlerVariables
 * @brief Static structure holding the communication configuration for the sync handler.
 *
 * Stores the registered send/receive function pointers and state for
 * managing broadcast communication.
 */
static sync_handler_comm_st sync_handler_comm = {
    .send_fn = NULL,
    .receive_fn = NULL,
    .max_buffer_size = BROADCAST_MSG_MAX_PAYLOAD_SIZE,
    .send_position_id = 0U,
    .initialized = (uint8_t)FALSE
};

/**
 * @var broadcast_msg
 * @ingroup DataTableSyncHandlerVariables
 * @brief Static broadcast message structure used for send/receive operations.
 *
 * This structure is used by the sync handler loop to build and receive
 * broadcast messages.
 */
static broadcast_msg_st broadcast_msg = {0};

/**
 * @brief Stable merge of two adjacent runs [left, mid) and [mid, right) by device_id (ascending).
 * @ingroup DataTableSyncHandlerFunctions
 *
 * Merges two already-sorted runs from @p src into @p dst preserving stability
 * (on equality, the element from the left run is taken first).
 *
 * @param[in]  src   Source array containing the two runs to merge.
 * @param[out] dst   Destination array where the merged range [left, right) is written.
 * @param[in]  left  Start index of the left run (inclusive).
/**
 * @brief Simple bubble sort of tag_item_st by device_id (ascending).
 * @ingroup DataTableSyncHandlerFunctions
 *
 * In-place bubble sort. Simple and reliable for small arrays.
 *
 * @param[in,out] array Pointer to the array to sort (in-place result).
 * @param[in] array_size Number of elements in @p array.
 * @return void
 */
static void sort_tag_list_array(tag_item_st *array, size_t array_size)
{
    if ((NULL != array) && (array_size > 1U))
    {
        for (size_t i = 0U; i < array_size - 1U; i++)
        {
            for (size_t j = 0U; j < array_size - 1U - i; j++)
            {
                if (array[j].device_id > array[j + 1U].device_id)
                {
                    // Swap elements
                    tag_item_st temp = array[j];
                    array[j] = array[j + 1U];
                    array[j + 1U] = temp;
                }
            }
        }
    }
    return;
}

void init_data_table_sync_handler(void)
{
    uint32_t tag_list_index = 0U;

    (void)memset(sorted_tag_list, 0x00, sizeof(sorted_tag_list));
    (void)memset(device_id_list, INVALID_UINT8, sizeof(device_id_list));

    // Walk through all tag IDs and place them in the row of their corresponding DEVICE.
    // Starts at 1, tag 0 is reserved.
    for (uint32_t i = 1; i < (uint32_t)TAG_DEBUG_1_DEVICE_1; i++)
    {
        // Query which DEVICE owns this tag.
        sorted_tag_list[tag_list_index] = (tag_item_st){.tag = i, .device_id = get_device_id_from_tag(i)};
        tag_list_index++;  // Advance the per-DEVICE index.
    }

    sort_tag_list_array(sorted_tag_list, (size_t)tag_list_index);
    uint8_t current_id = sorted_tag_list[0].device_id;
    device_id_list[current_id].index = 0;
    device_id_list[current_id].size = 0U;

    for (uint32_t i = 0; i < tag_list_index; i++)
    {
        if (current_id != sorted_tag_list[i].device_id)
        {
            current_id = sorted_tag_list[i].device_id;
            device_id_list[current_id].index = i;
            device_id_list[current_id].size = 0U;
        }
        if (current_id == sorted_tag_list[i].device_id)
        {
            device_id_list[current_id].size++;
        }
    }

    (void)memset(debug_sorted_tag_list, 0x00, sizeof(debug_sorted_tag_list));
    (void)memset(debug_device_id_list, INVALID_UINT8, sizeof(debug_device_id_list));

    tag_list_index = 0U;
    for (uint32_t i = (uint32_t)TAG_DEBUG_1_DEVICE_1; i < (uint32_t)TAG_MAX; i++)
    {
        // Query which DEVICE owns this tag.
        debug_sorted_tag_list[tag_list_index] = (tag_item_st){.tag = i, .device_id = get_device_id_from_tag(i)};
        tag_list_index++;  // Advance the per-DEVICE index.
    }
    sort_tag_list_array(debug_sorted_tag_list, (size_t)tag_list_index);

    current_id = debug_sorted_tag_list[0].device_id;
    debug_device_id_list[current_id].index = 0;
    debug_device_id_list[current_id].size = 0U;

    for (uint32_t i = 0; i < tag_list_index; i++)
    {
        if (current_id != debug_sorted_tag_list[i].device_id)
        {
            current_id = debug_sorted_tag_list[i].device_id;
            debug_device_id_list[current_id].index = i;
            debug_device_id_list[current_id].size = 0U;
        }
        if (current_id == debug_sorted_tag_list[i].device_id)
        {
            debug_device_id_list[current_id].size++;
        }
    }

    return;
}

uint32_t build_broadcast_buffer(uint8_t *broadcast_buffer, size_t buffer_size, uint8_t device_id, size_t max_output_size, uint32_t *position_id)
{
    uint32_t result = (uint8_t)OK;
    uint32_t DEVICE_range_buffer_index = 0;
    uint32_t tag_list_index = device_id_list[device_id].index;
    uint32_t start_tag_index = 0U;
    uint32_t current_position = 0U;
    size_t effective_max_size = (0U == max_output_size) ? buffer_size : max_output_size;

    if ((NULL != broadcast_buffer) && (device_id_list[device_id].size <= buffer_size) &&
        (INVALID_UINT32 != device_id_list[device_id].index) && (INVALID_UINT32 != device_id_list[device_id].size))
    {
        // Calculate the starting tag index based on position_id
        if (NULL != position_id)
        {
            current_position = *position_id;
            uint32_t accumulated_size = 0U;
            uint32_t chunk_count = 0U;

            // Find the starting tag index for the requested chunk
            for (uint32_t i = 0; i < device_id_list[device_id].size; i++)
            {
                uint8_t tag_size = get_tag_size(sorted_tag_list[tag_list_index + i].tag);

                if ((accumulated_size + tag_size) > effective_max_size)
                {
                    chunk_count++;
                    if (chunk_count == current_position)
                    {
                        start_tag_index = i;
                        break;
                    }
                    accumulated_size = 0U;
                }
                accumulated_size += tag_size;

                // If this is the first chunk (position 0), start from beginning
                if ((0U == current_position) && (0U == i))
                {
                    start_tag_index = 0U;
                }
            }

            // If requested position exceeds available chunks, return NOK
            if ((current_position > 0U) && (start_tag_index == 0U) && (chunk_count < current_position))
            {
                *position_id = INVALID_UINT32;
                return (uint8_t)NOK;
            }
        }

        tag_list_index += start_tag_index;

        for (uint32_t i = start_tag_index; i < device_id_list[device_id].size; i++)
        {
            uint32_t tmp_data = 0U;
            uint8_t tmp_array[4];
            uint8_t tmp_tag_size = get_tag_size(sorted_tag_list[tag_list_index].tag);

            // Check if adding this tag would exceed the max output size
            if ((DEVICE_range_buffer_index + tmp_tag_size) > effective_max_size)
            {
                // Update position_id to indicate more chunks are available
                if (NULL != position_id)
                {
                    *position_id = current_position + 1U;
                }
                break;
            }

            if (1U == read_from_datatable(sorted_tag_list[tag_list_index].tag, &tmp_data, (size_t)tmp_tag_size))
            {
                (void)memcpy((void *)tmp_array, (const void *)(const uint8_t *)&tmp_data, sizeof(tmp_data));
                // Store the value in the broadcast buffer for this DEVICE and slot.
                for (uint8_t j = 0U; j < tmp_tag_size; j++)
                {
                    if ((DEVICE_range_buffer_index < effective_max_size) && (4U > j))
                    {
                        broadcast_buffer[DEVICE_range_buffer_index] = tmp_array[j];
                        DEVICE_range_buffer_index++;
                    }
                }
            }
            else
            {
                DEVICE_range_buffer_index += tmp_tag_size;  // Adding here to keep the order of the message intact in case
                                                          // of error reading from the DataTable
            }
            tag_list_index++;

            // If we've processed all tags, mark as complete
            if ((i == (device_id_list[device_id].size - 1U)) && (NULL != position_id))
            {
                *position_id = 0;  // Indicate no more chunks
            }
        }
        result = DEVICE_range_buffer_index;
    }
    else
    {
        result = (uint8_t)NOK;
    }
    return result;
}

uint32_t build_debug_broadcast_buffer(uint8_t *broadcast_buffer, size_t buffer_size, uint8_t device_id)
{
    uint32_t result = (uint8_t)OK;
    uint32_t DEVICE_range_buffer_index = 0;
    uint32_t tag_list_index = debug_device_id_list[device_id].index;
    if ((NULL != broadcast_buffer) && (debug_device_id_list[device_id].size <= buffer_size) &&
        (INVALID_UINT32 != debug_device_id_list[device_id].index) && (INVALID_UINT32 != debug_device_id_list[device_id].size))
    {
        for (uint32_t i = 0; i < debug_device_id_list[device_id].size; i++)
        {
            uint32_t tmp_data = 0U;
            uint8_t tmp_array[4];
            uint8_t tmp_tag_size = get_tag_size(debug_sorted_tag_list[tag_list_index].tag);

            if (1U == read_from_datatable(debug_sorted_tag_list[tag_list_index].tag, &tmp_data, (size_t)tmp_tag_size))
            {
                (void)memcpy((void *)tmp_array, (const void *)(const uint8_t *)&tmp_data, sizeof(tmp_data));
                // Store the value in the broadcast buffer for this DEVICE and slot.
                for (uint8_t j = 0U; j < tmp_tag_size; j++)
                {
                    if ((DEVICE_range_buffer_index < buffer_size) && (4U > j))
                    {
                        broadcast_buffer[DEVICE_range_buffer_index] = tmp_array[j];
                        DEVICE_range_buffer_index++;
                    }
                }
            }
            else
            {
                // Adding here to keep the order of the message intact in case of error reading from the DataTable
                DEVICE_range_buffer_index += tmp_tag_size;
            }
            tag_list_index++;
        }
        result = DEVICE_range_buffer_index;
    }
    else
    {
        result = (uint8_t)NOK;
    }
    return result;
}

uint8_t write_broadcast_buffer(uint8_t *broadcast_buffer, size_t buffer_size, uint8_t incoming_device_id, size_t max_input_size, uint32_t *position_id)
{
    uint8_t result = (uint8_t)OK;
    uint32_t DEVICE_range_buffer_index = 0;
    uint32_t tag_list_index = device_id_list[incoming_device_id].index;
    uint32_t start_tag_index = 0U;
    uint32_t current_position = 0U;
    size_t effective_max_size = (0U == max_input_size) ? buffer_size : max_input_size;

    if ((NULL != broadcast_buffer) && (device_id_list[incoming_device_id].size <= buffer_size) &&
        (INVALID_UINT32 != device_id_list[incoming_device_id].index) && (INVALID_UINT32 != device_id_list[incoming_device_id].size))
    {
        // Calculate the starting tag index based on position_id
        if (NULL != position_id)
        {
            current_position = *position_id;
            uint32_t accumulated_size = 0U;
            uint32_t chunk_count = 0U;

            // Find the starting tag index for the requested chunk
            for (uint32_t i = 0; i < device_id_list[incoming_device_id].size; i++)
            {
                uint8_t tag_size = get_tag_size(sorted_tag_list[tag_list_index + i].tag);

                if ((accumulated_size + tag_size) > effective_max_size)
                {
                    chunk_count++;
                    if (chunk_count == current_position)
                    {
                        start_tag_index = i;
                        break;
                    }
                    accumulated_size = 0U;
                }
                accumulated_size += tag_size;

                // If this is the first chunk (position 0), start from beginning
                if ((0U == current_position) && (0U == i))
                {
                    start_tag_index = 0U;
                }
            }

            // If requested position exceeds available chunks, return NOK
            if ((current_position > 0U) && (start_tag_index == 0U) && (chunk_count < current_position))
            {
                *position_id = INVALID_UINT32;
                return (uint8_t)NOK;
            }
        }

        tag_list_index += start_tag_index;

        for (uint32_t i = start_tag_index; i < device_id_list[incoming_device_id].size; i++)
        {
            uint32_t tmp_data = 0U;
            uint8_t tmp_array[4];
            uint8_t tmp_tag_size = get_tag_size(sorted_tag_list[tag_list_index].tag);

            // Check if reading this tag would exceed the max input size
            if ((DEVICE_range_buffer_index + tmp_tag_size) > effective_max_size)
            {
                // Update position_id to indicate more chunks are available
                if (NULL != position_id)
                {
                    *position_id = current_position + 1U;
                }
                break;
            }

            for (uint8_t j = 0U; j < tmp_tag_size; j++)
            {
                if ((DEVICE_range_buffer_index < effective_max_size) && (4U > j))
                {
                    tmp_array[j] = broadcast_buffer[DEVICE_range_buffer_index];
                    DEVICE_range_buffer_index++;
                }
                else
                {
                    result = (uint8_t)NOK;
                }
            }
            (void)memcpy((void *)(uint8_t *)&tmp_data, (const void *)tmp_array, tmp_tag_size);

            if (1U != write_to_datatable(sorted_tag_list[tag_list_index].tag, &tmp_data, tmp_tag_size))
            {
                result = (uint8_t)NOK;
                break;
            }
            tag_list_index++;

            // If we've processed all tags, mark as complete
            if ((i == (device_id_list[incoming_device_id].size - 1U)) && (NULL != position_id))
            {
                *position_id = INVALID_UINT32;  // Indicate no more chunks
            }
        }
    }
    else
    {
        result = (uint8_t)NOK;
    }
    return result;
}

uint8_t init_data_table_sync_handler_comm(send_broadcast_fn send_fn, receive_broadcast_fn receive_fn, size_t max_buffer_size)
{
    uint8_t result = (uint8_t)OK;

    // At least one function pointer must be provided
    if ((NULL == send_fn) && (NULL == receive_fn))
    {
        result = (uint8_t)NOK;
    }
    else
    {
        // Initialize the base sync handler
        init_data_table_sync_handler();

        // Store the function pointers
        sync_handler_comm.send_fn = send_fn;
        sync_handler_comm.receive_fn = receive_fn;

        // Set the max buffer size (use default if 0 is provided)
        // Note: max_buffer_size is clamped to BROADCAST_MSG_MAX_PAYLOAD_SIZE
        if (0U != max_buffer_size)
        {
            // Clamp to the max payload size
            if (max_buffer_size > BROADCAST_MSG_MAX_PAYLOAD_SIZE)
            {
                sync_handler_comm.max_buffer_size = BROADCAST_MSG_MAX_PAYLOAD_SIZE;
            }
            else
            {
                sync_handler_comm.max_buffer_size = max_buffer_size;
            }
        }
        else
        {
            sync_handler_comm.max_buffer_size = BROADCAST_MSG_MAX_PAYLOAD_SIZE;
        }

        // Reset the position ID for chunked sending
        sync_handler_comm.send_position_id = 0U;

        // Mark as initialized
        sync_handler_comm.initialized = (uint8_t)TRUE;
    }

    return result;
}

uint8_t data_table_sync_handler_loop(void)
{
    uint8_t result = (uint8_t)OK;
    uint8_t current_device = get_current_device_id();

    // Check if the comm layer is initialized
    if ((uint8_t)FALSE == sync_handler_comm.initialized)
    {
        result = (uint8_t)NOK;
    }
    else
    {
        // ============================================================
        // SEND: Build and send the broadcast message for current device
        // ============================================================
        if (NULL != sync_handler_comm.send_fn)
        {
            uint32_t position_id = sync_handler_comm.send_position_id;

            // Clear the message structure before building
            (void)memset(&broadcast_msg, 0x00, sizeof(broadcast_msg));

            // Build the broadcast buffer for the current device
            uint32_t build_result = build_broadcast_buffer(
                broadcast_msg.payload,
                BROADCAST_MSG_MAX_PAYLOAD_SIZE,
                current_device,
                BROADCAST_MSG_MAX_PAYLOAD_SIZE,
                &position_id
            );

            if (((uint8_t)NOK != build_result) && (0U < build_result))
            {
                // Fill in the message metadata
                broadcast_msg.payload_size = (uint16_t)build_result;
                broadcast_msg.source_device_id = current_device;
                broadcast_msg.chunk_index = sync_handler_comm.send_position_id;

                // Send the broadcast message
                uint8_t send_result = sync_handler_comm.send_fn(&broadcast_msg);
                
                if ((uint8_t)OK == send_result)
                {
                    // Update position ID for next chunk or reset if complete
                    if (INVALID_UINT32 == position_id)
                    {
                        // All data sent, reset to start
                        sync_handler_comm.send_position_id = 0U;
                    }
                    else
                    {
                        // More chunks to send
                        sync_handler_comm.send_position_id = position_id;
                    }
                }
                else
                {
                    result = (uint8_t)NOK;
                }
            }
            else if ((uint8_t)NOK == build_result)
            {
                result = (uint8_t)NOK;
            }
            // If build_result is 0, there's no data to send (which is OK)
        }

        // ============================================================
        // RECEIVE: Receive and write broadcast data from other devices
        // ============================================================
        if (NULL != sync_handler_comm.receive_fn)
        {
            // Clear the message structure before receiving
            (void)memset(&broadcast_msg, 0x00, sizeof(broadcast_msg));

            // Try to receive a message
            uint8_t receive_result = sync_handler_comm.receive_fn(&broadcast_msg);

            // Process received message if successful
            if ((uint8_t)OK == receive_result)
            {
                // Validate the received message
                if ((0U < broadcast_msg.payload_size) && 
                    (BROADCAST_MSG_MAX_PAYLOAD_SIZE >= broadcast_msg.payload_size) &&
                    ((uint8_t)INVALID_UINT8 != broadcast_msg.source_device_id))
                {
                    // Don't write data from our own device
                    if (broadcast_msg.source_device_id != current_device)
                    {
                        uint32_t write_position_id = broadcast_msg.chunk_index;

                        // Write the received broadcast buffer to the data table
                        uint8_t write_result = write_broadcast_buffer(
                            broadcast_msg.payload,
                            BROADCAST_MSG_MAX_PAYLOAD_SIZE,
                            broadcast_msg.source_device_id,
                            (size_t)broadcast_msg.payload_size,
                            &write_position_id
                        );

                        if ((uint8_t)OK != write_result)
                        {
                            result = (uint8_t)NOK;
                        }
                    }
                }
            }
            // If receive returns NOK, no data available (which is OK)
        }
    }

    return result;
}
uint8_t write_debug_broadcast_buffer(uint8_t *broadcast_buffer, size_t buffer_size, uint8_t incoming_device_id)
{
    uint8_t result = (uint8_t)OK;
    uint32_t DEVICE_range_buffer_index = 0;
    uint32_t tag_list_index = debug_device_id_list[incoming_device_id].index;

    if ((NULL != broadcast_buffer) && (debug_device_id_list[incoming_device_id].size <= buffer_size) &&
        (INVALID_UINT32 != debug_device_id_list[incoming_device_id].index) &&
        (INVALID_UINT32 != debug_device_id_list[incoming_device_id].size) && (get_current_device_id() != incoming_device_id))
    {
        for (uint32_t i = 0; i < debug_device_id_list[incoming_device_id].size; i++)
        {
            uint32_t tmp_data = 0U;
            uint8_t tmp_array[4];
            uint8_t tmp_tag_size = get_tag_size(debug_sorted_tag_list[tag_list_index].tag);

            for (uint8_t j = 0U; j < tmp_tag_size; j++)
            {
                if ((DEVICE_range_buffer_index < buffer_size) && (4U > j))
                {
                    tmp_array[j] = broadcast_buffer[DEVICE_range_buffer_index];
                    DEVICE_range_buffer_index++;
                }
                else
                {
                    result = (uint8_t)NOK;
                }
            }
            (void)memcpy((void *)(uint8_t *)&tmp_data, (const void *)tmp_array, tmp_tag_size);

            if (1U != write_to_datatable(debug_sorted_tag_list[tag_list_index].tag, &tmp_data, tmp_tag_size))
            {
                result = (uint8_t)NOK;
                break;
            }
            tag_list_index++;
        }
    }
    else
    {
        result = (uint8_t)NOK;
    }
    return result;
}

void debug_get_device_id_list_info(uint8_t device_id, uint32_t* index, uint32_t* size)
{
    if ((NULL != index) && (NULL != size) && (device_id < DATA_TABLE_MAX_NUMBER_OF_DEVICES))
    {
        *index = device_id_list[device_id].index;
        *size = device_id_list[device_id].size;
    }
}

void debug_get_sorted_tag_entry(uint32_t list_index, uint32_t* tag, uint8_t* device_id)
{
    if ((NULL != tag) && (NULL != device_id) && (list_index < (uint32_t)TAG_DEBUG_1_DEVICE_1))
    {
        *tag = sorted_tag_list[list_index].tag;
        *device_id = sorted_tag_list[list_index].device_id;
    }
}
