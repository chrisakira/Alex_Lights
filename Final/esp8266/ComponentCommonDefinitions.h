/**
 * @file ComponentCommonDefinitions.h
 * @brief Enumerates the shared structures for all software components
 * @version 0.1
 * @date 2025-08-29
 */

#ifndef __COMPONENT_COMMON_DEFINITIONS_H__
#define __COMPONENT_COMMON_DEFINITIONS_H__

#include <stdint.h>
#include <stdbool.h>


/// Unsigned integer equivalent to boolean false
#define FALSE 0U
/// Unsigned integer equivalent to boolean true
#define TRUE 1U

/// Invalid value for uint8_t (0xFF)
#define INVALID_UINT8 0xFFU
/// Invalid value for uint16_t (0xFFFF)
#define INVALID_UINT16 0xFFFFU
/// Invalid value for uint32_t (0xFFFFFFFF)
#define INVALID_UINT32 0xFFFFFFFFU

/// Debounce time of 10ms counted as iterations of a 10ms cycle. 
#define DEBOUNCE_10MS_IN_10MS_CYCLE 1U

/// Debounce time of 50ms counted as iterations of a 10ms cycle. 
#define DEBOUNCE_50MS_IN_10MS_CYCLE 5U

/// Debounce time of 100ms counted as iterations of a 10ms cycle. 
#define DEBOUNCE_100MS_IN_10MS_CYCLE 10U

/// Debounce time of 200ms counted as iterations of a 10ms cycle. 
#define DEBOUNCE_200MS_IN_10MS_CYCLE 20U

/// Debounce time of 300ms counted as iterations of a 10ms cycle. 
#define DEBOUNCE_300MS_IN_10MS_CYCLE 30U

/// Debounce time of 400ms counted as iterations of a 10ms cycle. 
#define DEBOUNCE_400MS_IN_10MS_CYCLE 40U

/// Debounce time of 500ms counted as iterations of a 10ms cycle. 
#define DEBOUNCE_500MS_IN_10MS_CYCLE 50U

/// Debounce time of 1s counted as iterations of a 10ms cycle. 
#define DEBOUNCE_1S_IN_10MS_CYCLE 100U

/// Debounce time of 2s counted as iterations of a 10ms cycle. 
#define DEBOUNCE_2S_IN_10MS_CYCLE 200U

/// Debounce time of 3s counted as iterations of a 10ms cycle. 
#define DEBOUNCE_3S_IN_10MS_CYCLE 300U

/// Debounce time of 5s counted as iterations of a 10ms cycle. 
#define DEBOUNCE_5S_IN_10MS_CYCLE 500U

/// Debounce time of 90s counted as iterations of a 10ms cycle. 
#define DEBOUNCE_90S_IN_10MS_CYCLE 9000U

/// Debounce time of 120s counted as iterations of a 10ms cycle. 
#define DEBOUNCE_120S_IN_10MS_CYCLE 12000U

/**
 * @enum component_version_et 
 * @brief Enumeration of all currently used version numbers. Versions are defined through an 16-bit unsigned integer 
 * where the MSB represents the major version and the LSB the minor version. This enumeration serves only to make code 
 * easier to read, since comparisons can be performed with the defined values.
 */
typedef enum
{
    
    /// VERSION 0.0 - Disables the component
    VERSION_0_0 = 0x0000, 
    
    /// VERSION 0.1
    VERSION_0_1 = 0x0001,
    
    /// VERSION 1.0
    VERSION_1_0 = 0x0100,
    
    /// VERSION 1.1
    VERSION_1_1 = 0x0101,
    
    /// VERSION 1.2
    VERSION_1_2 = 0x0102,
    
    /// VERSION 1.3
    VERSION_1_3 = 0x0103,
    
    /// VERSION 1.4
    VERSION_1_4 = 0x0104,
    
    /// VERSION 1.5
    VERSION_1_5 = 0x0105,
} component_version_et;

/**
 * @enum device_loops_et 
 * @brief Enumeration of all available DEVICE loops. This enum is used during initialization to set the main application
 * time loops correctly.
 */
typedef enum
{
    LOOP_1MS = 0,
    LOOP_10MS,
    LOOP_25MS,
    LOOP_50MS,
    LOOP_100MS,
    LOOP_250MS,
    LOOP_500MS,
    LOOP_1000MS,
} device_loops_et;


/**
 * @enum types_et
 * @ingroup DataTypesStructures
 * @brief Data types to be sent to or received from an external data source
 *
 */
typedef enum
{
    BOOL = 0,  ///< Boolean data type
    UINT8,     ///< Unsigned 8 bits integer data type
    INT8,      ///< Signed 8 bits integer data type
    UINT16,    ///< Unsigned 16 bits integer data type
    INT16,     ///< Signed 16 bits integer data type
    UINT32,    ///< Unsigned 32 bits integer data type
    INT32,     ///< Signed 32 bits integer data type
    FLOAT,     ///< Floating point data type
} types_et;

/**
 * @struct data_object_st
 * @ingroup DataTypesStructures
 * @brief Data Object used to store info into the DataTable.
 *
 */
typedef struct
{ 
    uint8_t device_id;           ///< Device ID that identifies who can write into de database
    void* data_raw_value;       ///< Pointer to the Raw Data Value
    types_et type;              ///< Data type
} data_object_st;

/**
 * @enum FdeviceID_et
 * @ingroup DataTypesStructures
 * @brief Unique identifiers for the DEVICEs
 *
 */
typedef enum 
{
    TEXTILE_0 = 0,
    TEXTILE_1,
    TEXTILE_2,
    TEXTILE_3,
    TEXTILE_4,
    TEXTILE_5,
    TEXTILE_6, 
    PROTECTED,
    ANY_DEVICE, 
} device_id_et;
 

/**
 * @enum ResponseCode_et
 * @ingroup DataTypesStructures
 * @brief Response Codes used to identify function results
 *
 */
typedef enum
{
    RESPONSE_OK = 1,
    RESPONSE_BUFFER_OVERFLOW,
    RESPONSE_INCORRECT_ARGS,
    RESPONSE_NOK = 0xFF,
} response_code_et;

// Backward compatibility - define OK/NOK as macros for library code
// These will be used by the datatable library C files
#define OK RESPONSE_OK
#define NOK RESPONSE_NOK


/**
 *  @bried  Floats type
 *  @note   Don't use native C types
 */
typedef float                   float32_t;   

/**
 *  @bried  Boolean type
 */
typedef bool                    bool_t;

#ifndef K_FALSE
# define K_FALSE                (false)
#endif

#ifndef K_TRUE
# define K_TRUE                 (true)
#endif

#ifndef FALSE
# define FALSE                  (K_FALSE)
#endif

#ifndef TRUE
# define TRUE                   (K_TRUE)
#endif

#endif
