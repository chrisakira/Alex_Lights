/**
 * @file DataTable.c
 * @brief Source Code of Data Table
 */

#include <string.h>
#include "DataTableConfig.h"

/**
 * @var data_pack_st datatable_matrix_buffer[TAG_MAX]
 * @ingroup DataTableVariables
 * @brief Data structure that stores all the datatable_matrix_buffer data. Each element is called "data object",
 * containing the data raw value, tag identifier, data type and data timestamp (last time updated).
 */
static data_object_st datatable_matrix_buffer[TAG_MAX] = {0};

/**
 * @var uint8_t current_device_id
 * @ingroup DataTableVariables
 * @brief Current Device ID value.
 */
static uint8_t current_device_id = (uint8_t)NOK;

/**
 * @brief Gets the number of bytes of the passes data type.
 *
 * @param data_type Data type to check the size
 * @return uint8_t Size of the passed data type in number of bytes
 */
static uint8_t get_size(const types_et data_type)
{
    uint8_t result = (uint8_t)NOK;
    switch (data_type)
    {
        case BOOL :
            result = (uint8_t)sizeof(bool_t);
            break;

        case UINT8 :
            result = (uint8_t)sizeof(uint8_t);
            break;

        case INT8 :
            result = (uint8_t)sizeof(int8_t);
            break;

        case UINT16 :
            result = (uint8_t)sizeof(uint16_t);
            break;

        case INT16 :
            result = (uint8_t)sizeof(int16_t);
            break;

        case UINT32 :
            result = (uint8_t)sizeof(uint32_t);
            break;

        case INT32 :
            result = (uint8_t)sizeof(int32_t);
            break;

        case FLOAT :
            result = (uint8_t)sizeof(float32_t);
            break;

        default :
            result = (uint8_t)NOK;
            break;
    }
    return result;
}

uint8_t init_datatable(uint8_t device_id)
{
    current_device_id = device_id;
    return config_datatable(datatable_matrix_buffer);
}

uint8_t get_current_device_id(void) { return current_device_id; }

void set_current_device_id(uint8_t device_id)
{
    current_device_id = device_id;
    return;
}

uint8_t get_tag_size(const uint32_t data_tag)
{
    uint8_t result = (uint8_t)NOK;
    if ((uint32_t)TAG_MAX > data_tag)
    {
        // Access the tag entry and compute the size based on the stored type.
        result = get_size(datatable_matrix_buffer[data_tag].type);
    }
    return result;
}

uint8_t get_device_id_from_tag(const uint32_t data_tag)
{
    uint8_t result = (uint8_t)NOK;
    if ((uint32_t)TAG_MAX > data_tag)
    {
        if ((uint8_t)ANY_DEVICE >= datatable_matrix_buffer[data_tag].device_id)
        {
            result = datatable_matrix_buffer[data_tag].device_id;
        }
    }
    return result;
}

uint8_t register_debug_var(const uint32_t data_tag, void* data, types_et data_type)
{
    uint8_t result = (uint8_t)NOK;
    if (((uint32_t)TAG_DEBUG_1_DEVICE_1 <= data_tag) && ((uint32_t)TAG_MAX > data_tag) && (NULL != data) &&
        (NULL == datatable_matrix_buffer[data_tag].data_raw_value) && ((types_et)FLOAT >= data_type))
    {
        // Stores the address of the var to be debug for future reads.
        datatable_matrix_buffer[data_tag].data_raw_value = data;

        // Record the declared type so size checks can be enforced.
        datatable_matrix_buffer[data_tag].type = data_type;
        result = (uint8_t)OK;
    }
    return result;
}

uint8_t read_from_datatable(const uint32_t data_tag, void* data, size_t size)
{
    uint8_t result = (uint8_t)NOK;
    if ((NULL != data) && ((uint32_t)TAG_MAX > data_tag) &&
        (NULL != datatable_matrix_buffer[data_tag].data_raw_value) &&
        (size == get_size(datatable_matrix_buffer[data_tag].type)))
    {
        // Reads 'size' bytes from the 'datatable_matrix_buffer[data_tag].data_raw_value' buffer into 'data'
        (void)memcpy(data, datatable_matrix_buffer[data_tag].data_raw_value, size);
        result = (uint8_t)OK;
    }
    return result;
}

uint8_t write_to_datatable(const uint32_t data_tag, const void* data, size_t size)
{
    uint8_t result = (uint8_t)NOK;
    if (((uint32_t)TAG_DEBUG_1_DEVICE_1 > data_tag) && (NULL != data) &&
        (NULL != datatable_matrix_buffer[data_tag].data_raw_value) &&
        (size == get_size(datatable_matrix_buffer[data_tag].type)))
    {
        // Writes 'size' bytes from the 'data' buffer into 'datatable_matrix_buffer[data_tag].data_raw_value'
        (void)memcpy(datatable_matrix_buffer[data_tag].data_raw_value, data, size);
        result = (uint8_t)OK;
    }
    return result;
}
