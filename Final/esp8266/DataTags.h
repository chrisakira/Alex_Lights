/**
 * @file    DataTags.h
 * @brief   Common data tags
 *
 * 
 */

#ifndef __DATA_TAGS_H__
#define __DATA_TAGS_H__

#include "ComponentCommonDefinitions.h"
/**
 * @defgroup DataTagsStructures 2.2.5 List of Data Tags Structures
 */

/**
 * @enum data_tag_et
 * @ingroup DataTagsStructures
 * @brief Data tags enumeration used to access data from an external data source.
 *
 */
typedef enum
{
    TAG_NONE = 0,

    /**********************************/
    /*********** TEXTILE 1 TAGS ***********/
    /**********************************/
    TAG_TEXTILE_1_PREVIOUS_SEQUENCE_STATE,
    TAG_TEXTILE_1_ACTUAL_SEQUENCE_STATE,
    TAG_TEXTILE_1_PRESENCE_DETECTED,

    /**********************************/
    /*********** TEXTILE 2 TAGS ***********/
    /**********************************/
    TAG_TEXTILE_2_PREVIOUS_SEQUENCE_STATE,
    TAG_TEXTILE_2_ACTUAL_SEQUENCE_STATE,
    TAG_TEXTILE_2_PRESENCE_DETECTED,
    
    /**********************************/
    /*********** TEXTILE 3 TAGS ***********/
    /**********************************/
    TAG_TEXTILE_3_PREVIOUS_SEQUENCE_STATE,
    TAG_TEXTILE_3_ACTUAL_SEQUENCE_STATE,
    TAG_TEXTILE_3_PRESENCE_DETECTED,
    
    /**********************************/
    /*********** TEXTILE 4 TAGS ***********/
    /**********************************/
    TAG_TEXTILE_4_PREVIOUS_SEQUENCE_STATE,
    TAG_TEXTILE_4_ACTUAL_SEQUENCE_STATE,
    TAG_TEXTILE_4_PRESENCE_DETECTED,
    
    /**********************************/
    /*********** TEXTILE 5 TAGS ***********/
    /**********************************/
    TAG_TEXTILE_5_PREVIOUS_SEQUENCE_STATE,
    TAG_TEXTILE_5_ACTUAL_SEQUENCE_STATE,
    TAG_TEXTILE_5_PRESENCE_DETECTED,

    /**********************************/
    /*********** TEXTILE 6 TAGS ***********/
    /**********************************/
    TAG_TEXTILE_6_PREVIOUS_SEQUENCE_STATE,
    TAG_TEXTILE_6_ACTUAL_SEQUENCE_STATE,
    TAG_TEXTILE_6_PRESENCE_DETECTED,
    

    /**********************************/
    /*********** DEBUG TAGS ***********/
    /* @warning All application TAGS  */
    /** must be above the debug TAGS **/
    /**********************************/

    TAG_DEBUG_1_DEVICE_1,
    TAG_DEBUG_2_DEVICE_1,
    TAG_DEBUG_3_DEVICE_1,
    TAG_DEBUG_4_DEVICE_1,
    TAG_DEBUG_5_DEVICE_1,
    TAG_DEBUG_6_DEVICE_1,
    TAG_DEBUG_7_DEVICE_1,
    TAG_DEBUG_8_DEVICE_1,
    TAG_DEBUG_9_DEVICE_1,
    TAG_DEBUG_10_DEVICE_1,
    TAG_DEBUG_1_DEVICE_2,
    TAG_DEBUG_2_DEVICE_2,
    TAG_DEBUG_3_DEVICE_2,
    TAG_DEBUG_4_DEVICE_2,
    TAG_DEBUG_5_DEVICE_2,
    TAG_DEBUG_6_DEVICE_2,
    TAG_DEBUG_7_DEVICE_2,
    TAG_DEBUG_8_DEVICE_2,
    TAG_DEBUG_9_DEVICE_2,
    TAG_DEBUG_10_DEVICE_2,
    TAG_DEBUG_1_DEVICE_3,
    TAG_DEBUG_2_DEVICE_3,
    TAG_DEBUG_3_DEVICE_3,
    TAG_DEBUG_4_DEVICE_3,
    TAG_DEBUG_5_DEVICE_3,
    TAG_DEBUG_6_DEVICE_3,
    TAG_DEBUG_7_DEVICE_3,
    TAG_DEBUG_8_DEVICE_3,
    TAG_DEBUG_9_DEVICE_3,
    TAG_DEBUG_10_DEVICE_3,
    TAG_DEBUG_1_DEVICE_4,
    TAG_DEBUG_2_DEVICE_4,
    TAG_DEBUG_3_DEVICE_4,
    TAG_DEBUG_4_DEVICE_4,
    TAG_DEBUG_5_DEVICE_4,
    TAG_DEBUG_6_DEVICE_4,
    TAG_DEBUG_7_DEVICE_4,
    TAG_DEBUG_8_DEVICE_4,
    TAG_DEBUG_9_DEVICE_4,
    TAG_DEBUG_10_DEVICE_4,
    TAG_MAX,
} data_tag_et;

#endif  // __DATA_TAGS_H__
