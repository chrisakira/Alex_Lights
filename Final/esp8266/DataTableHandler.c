/**
 * @file DataTableHandler.c
 * @brief Source Code of Data Table Handler
 */
#include "DataTableHandler.h"

#include "DataTable.h"

uint8_t init_data_table_handler(uint8_t device_id) { return init_datatable(device_id); }

uint8_t register_debug_var_handler(const uint32_t data_tag, void* data, types_et data_type)
{
    uint8_t result = (uint8_t)NOK;
    if (NULL != data)
    {
        result = register_debug_var(data_tag, data, data_type);
    }
    return result;
}

uint8_t get_data_table(const uint32_t data_tag, void* data, size_t size)
{
    uint8_t result = (uint8_t)NOK;
    if (NULL != data)
    {
        result = read_from_datatable(data_tag, data, size);
    }
    return result;
}

uint8_t write_data_table(const uint32_t data_tag, void* data, size_t size)
{
    uint8_t result = (uint8_t)NOK;

    // Enforce access controll.
    if (((get_device_id_from_tag(data_tag) == get_current_device_id()) ||
         (get_device_id_from_tag(data_tag) == (uint8_t)ANY_DEVICE)) &&
        ((uint8_t)NOK != get_device_id_from_tag(data_tag)) && ((uint8_t)NOK != get_current_device_id()))
    {
        result = write_to_datatable(data_tag, data, size);
    }
    return result;
}
