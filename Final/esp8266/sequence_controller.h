#ifndef __SEQUENCE_CONTROLLER_H__
#define __SEQUENCE_CONTROLLER_H__

#include <stddef.h>
#include <stdlib.h>

#include "extras.h"

enum sequences_s{
    IDLE = 0,
    SEQUENCE_A,
    SEQUENCE_B,
    SEQUENCE_C,
    SEQUENCE_D,
    SEQUENCE_P,
    FINISHED,
}sequences_st;

enum sequence_trigger_s{
    NOT_TRIGGERED = 0,
    SINGLE,
    DOUBLE,
    TRIPLE,
    FOUR,
}sequence_trigger_st;

typedef struct led_command_e{
    uint8_t positive_pin;
    uint8_t negative_pin;
} led_command_st;


typedef struct sequence_commands_e{
    led_command_st led_index;
    
    uint32_t time_starts_at;
    uint32_t time_ends_at;

    uint8_t value_starts_at;
    uint8_t value_ends_at;
} sequence_commands_st;

uint8_t active_sequence = IDLE;







#endif // __SEQUENCE_CONTROLLER_H__