/**
 * @file DatabaseConfig.c
 * @brief Source Code of the Database Config
 */

/* This needs to be included first to make sure the SafeRTOS headers are included in the correct order */
#include "DataTableConfig.h"

#include <string.h>

#include "DataTable.h"

/**
 * @var uint32_data_buffer[TAG_DEBUG_1_DEVICE_1]
 * @ingroup DatabaseConfigVariables
 * @brief Static buffer used to store pointers (addresses) to data elements
 * associated with the data table structure.
 *
 * It provides a storage area for references to the data table entries, enabling
 * access to data during debugging or processing operations.
 *
 * @ingroup DataTableConfigVariables
 */
static uint32_t uint32_data_buffer[TAG_DEBUG_1_DEVICE_1] = {0};

/**
 * @var datatable_default_config[]
 * @ingroup DatabaseConfigVariables
 * @brief Default configuration table for the data objects used in the system.
 *
 * This static array defines the initial configuration for each data tag in the database.
 * Each entry specifies:
 *  - device_id: The DEVICE (Process Input/Output) identifier associated with the tag.
 *  - type: The data type (e.g., UINT8, UINT16, UINT32, FLOAT) for the corresponding tag.
 *
 * The table is indexed by tag identifiers, and provides the default mapping between tags,
 * their Device IDs, and their data types.
 *
 * This configuration is used during system initialization to set up the data table
 * with proper type definitions and initial values.
 *
 * @note
 * The size of this array is determined by the number of defined tags in the system.
 */
static data_object_st datatable_default_config[] = {
    [TAG_NONE]                                              = {.device_id = (uint8_t)PROTECTED, .type = UINT32},

    /**********************************/
    /*********** TEXTILE 1 TAGS ***********/
    /**********************************/
    [TAG_TEXTILE_1_PREVIOUS_SEQUENCE_STATE]                 = {.device_id = (uint8_t)TEXTILE_1,    .type = UINT32},
    [TAG_TEXTILE_1_ACTUAL_SEQUENCE_STATE]                   = {.device_id = (uint8_t)TEXTILE_1,    .type = UINT32},
    [TAG_TEXTILE_1_PRESENCE_DETECTED]                       = {.device_id = (uint8_t)TEXTILE_1,    .type = UINT32},
    
    /**********************************/
    /*********** TEXTILE 2 TAGS ***********/
    /**********************************/
    [TAG_TEXTILE_2_PREVIOUS_SEQUENCE_STATE]                 = {.device_id = (uint8_t)TEXTILE_2,    .type = UINT32},
    [TAG_TEXTILE_2_ACTUAL_SEQUENCE_STATE]                   = {.device_id = (uint8_t)TEXTILE_2,    .type = UINT32},
    [TAG_TEXTILE_2_PRESENCE_DETECTED]                       = {.device_id = (uint8_t)TEXTILE_2,    .type = UINT32},
    
    /**********************************/
    /*********** TEXTILE 3 TAGS ***********/
    /**********************************/
    [TAG_TEXTILE_3_PREVIOUS_SEQUENCE_STATE]                 = {.device_id = (uint8_t)TEXTILE_3,    .type = UINT32},
    [TAG_TEXTILE_3_ACTUAL_SEQUENCE_STATE]                   = {.device_id = (uint8_t)TEXTILE_3,    .type = UINT32},
    [TAG_TEXTILE_3_PRESENCE_DETECTED]                       = {.device_id = (uint8_t)TEXTILE_3,    .type = UINT32},
    
    /**********************************/
    /*********** TEXTILE 4 TAGS ***********/
    /**********************************/
    [TAG_TEXTILE_4_PREVIOUS_SEQUENCE_STATE]                 = {.device_id = (uint8_t)TEXTILE_4,    .type = UINT32},
    [TAG_TEXTILE_4_ACTUAL_SEQUENCE_STATE]                   = {.device_id = (uint8_t)TEXTILE_4,    .type = UINT32},
    [TAG_TEXTILE_4_PRESENCE_DETECTED]                       = {.device_id = (uint8_t)TEXTILE_4,    .type = UINT32},
    
    /**********************************/
    /*********** TEXTILE 5 TAGS ***********/
    /**********************************/
    [TAG_TEXTILE_5_PREVIOUS_SEQUENCE_STATE]                 = {.device_id = (uint8_t)TEXTILE_5,    .type = UINT32},
    [TAG_TEXTILE_5_ACTUAL_SEQUENCE_STATE]                   = {.device_id = (uint8_t)TEXTILE_5,    .type = UINT32},
    [TAG_TEXTILE_5_PRESENCE_DETECTED]                       = {.device_id = (uint8_t)TEXTILE_5,    .type = UINT32},
    
    /**********************************/
    /*********** TEXTILE 6 TAGS ***********/
    /**********************************/
    [TAG_TEXTILE_6_PREVIOUS_SEQUENCE_STATE]                 = {.device_id = (uint8_t)TEXTILE_6,    .type = UINT32},
    [TAG_TEXTILE_6_ACTUAL_SEQUENCE_STATE]                   = {.device_id = (uint8_t)TEXTILE_6,    .type = UINT32},
    [TAG_TEXTILE_6_PRESENCE_DETECTED]                       = {.device_id = (uint8_t)TEXTILE_6,    .type = UINT32},
    
    /**********************************/
    /*********** DEBUG TAGS ***********/
    /**********************************/
    [TAG_DEBUG_1_DEVICE_1]   = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_DEBUG_2_DEVICE_1]   = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_DEBUG_3_DEVICE_1]   = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_DEBUG_4_DEVICE_1]   = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_DEBUG_5_DEVICE_1]   = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_DEBUG_6_DEVICE_1]   = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_DEBUG_7_DEVICE_1]   = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_DEBUG_8_DEVICE_1]   = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_DEBUG_9_DEVICE_1]   = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_DEBUG_10_DEVICE_1]  = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_DEBUG_1_DEVICE_2]   = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_DEBUG_2_DEVICE_2]   = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_DEBUG_3_DEVICE_2]   = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_DEBUG_4_DEVICE_2]   = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_DEBUG_5_DEVICE_2]   = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_DEBUG_6_DEVICE_2]   = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_DEBUG_7_DEVICE_2]   = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_DEBUG_8_DEVICE_2]   = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_DEBUG_9_DEVICE_2]   = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_DEBUG_10_DEVICE_2]  = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_DEBUG_1_DEVICE_3]   = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_DEBUG_2_DEVICE_3]   = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_DEBUG_3_DEVICE_3]   = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_DEBUG_4_DEVICE_3]   = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_DEBUG_5_DEVICE_3]   = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_DEBUG_6_DEVICE_3]   = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_DEBUG_7_DEVICE_3]   = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_DEBUG_8_DEVICE_3]   = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_DEBUG_9_DEVICE_3]   = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_DEBUG_10_DEVICE_3]  = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_DEBUG_1_DEVICE_4]   = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_DEBUG_2_DEVICE_4]   = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_DEBUG_3_DEVICE_4]   = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_DEBUG_4_DEVICE_4]   = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_DEBUG_5_DEVICE_4]   = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_DEBUG_6_DEVICE_4]   = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_DEBUG_7_DEVICE_4]   = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_DEBUG_8_DEVICE_4]   = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_DEBUG_9_DEVICE_4]   = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_DEBUG_10_DEVICE_4]  = {.device_id = (uint8_t)PROTECTED,    .type = BOOL},
    [TAG_MAX]                = {.device_id = (uint8_t)PROTECTED, .type = BOOL},

};

uint8_t config_datatable(data_object_st *datatable)
{
    uint8_t result = (uint8_t)NOK;
    if ((datatable != NULL) &&
        ((uint32_t)(sizeof(uint32_data_buffer) / sizeof(uint32_t)) <= (uint32_t)TAG_DEBUG_1_DEVICE_1))
    {
        for (uint32_t i = 0; i < (uint32_t)sizeof(datatable_default_config) / sizeof(datatable_default_config[0]); i++)
        {
            datatable[i].type = datatable_default_config[i].type;
            datatable[i].device_id = datatable_default_config[i].device_id;
            if ((uint32_t)TAG_DEBUG_1_DEVICE_1 > i)
            {
                datatable[i].data_raw_value = &uint32_data_buffer[i];
            }
            else
            {
                datatable[i].data_raw_value = NULL;  // Debug data addresses will be assigned by the components
            }
        }
        result = (uint8_t)OK;
    }
    return result;
}
