#include <Arduino.h>

#include "sequence_controller.h"
#include "sequence_defines.h"

extern "C" {
#include "ComponentCommonDefinitions.h"
#include "DataTableHandler.h"
#include "DataTableSyncHandler.h"
#include "DataTags.h"
}
#define MIN_VALUE 0
#define MAX_VALUE 255


enum sequences_s{
    NOT_CONNECTED = 0,
    IDLE,
    SEQUENCE_A,
    SEQUENCE_B,
    SEQUENCE_C,
    SEQUENCE_D,
    SEQUENCE_E,
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

/************** SEQUENCE 1 **************/
sequence_commands_st sequence_1_A[] = {
    {TEXT_1_A1, 0,    3000, 0, 255},
    {TEXT_1_A2, 2000, 4000, 0, 255},
    {TEXT_1_A3, 3000, 6000, 0, 255},
    {TEXT_1_A4, 5000, 7000, 0, 255},
    {TEXT_1_A5, 6000, 9000, 0, 255},
};
uint16_t sequence_1_A_size = sizeof(sequence_1_A) / sizeof(sequence_1_A[0]);

sequence_commands_st sequence_1_B[] = {
    {TEXT_1_B1, 0,    1000, 0, 255},
    {TEXT_1_B2, 1000, 2000, 0, 255},
    {TEXT_1_B3, 2000, 3000, 0, 255},
    {TEXT_1_B4, 3000, 4000, 0, 255},
    {TEXT_1_B5, 4000, 5000, 0, 255},
    {TEXT_1_B6, 4000, 5000, 0, 255},
};
uint16_t sequence_1_B_size = sizeof(sequence_1_B) / sizeof(sequence_1_B[0]);

sequence_commands_st sequence_1_C[] = {
    {TEXT_1_C1, 0,    1000, 0, 255},
    {TEXT_1_C2, 1000, 2000, 0, 255},
    {TEXT_1_C3, 2000, 3000, 0, 255},
    {TEXT_1_C4, 3000, 4000, 0, 255},
    {TEXT_1_C5, 4000, 5000, 0, 255},
};
uint16_t sequence_1_C_size = sizeof(sequence_1_C) / sizeof(sequence_1_C[0]);

sequence_commands_st sequence_1_D[] = {
    {TEXT_1_D1, 0,    1000, 0, 255},
    {TEXT_1_D2, 1000, 2000, 0, 255},
    {TEXT_1_D3, 2000, 3000, 0, 255},
    {TEXT_1_D4, 3000, 4000, 0, 255},
    {TEXT_1_D5, 4000, 5000, 0, 255},
    {TEXT_1_D6, 4000, 5000, 0, 255},
    {TEXT_1_D7, 4000, 5000, 0, 255},
    {TEXT_1_D8, 4000, 5000, 0, 255},
};
uint16_t sequence_1_D_size = sizeof(sequence_1_D) / sizeof(sequence_1_D[0]);

sequence_commands_st sequence_1_P[] = {
    {TEXT_1_P1, 0,    1000, 0, 255},
    {TEXT_1_P2, 1000, 2000, 0, 255},
};
uint16_t sequence_1_P_size = sizeof(sequence_1_P) / sizeof(sequence_1_P[0]);


/************** SEQUENCE 2 **************/
sequence_commands_st sequence_2_A[] = {
    {TEXT_1_A1, 0,    3000, 0, 255},
    {TEXT_1_A2, 2000, 4000, 0, 255},
    {TEXT_1_A3, 3000, 6000, 0, 255},
    {TEXT_1_A4, 5000, 7000, 0, 255},
    {TEXT_1_A5, 6000, 9000, 0, 255},
};
uint16_t sequence_2_A_size = sizeof(sequence_2_A) / sizeof(sequence_2_A[0]);

sequence_commands_st sequence_2_B[] = {
    {TEXT_1_B1, 0,    1000, 0, 255},
    {TEXT_1_B2, 1000, 2000, 0, 255},
    {TEXT_1_B3, 2000, 3000, 0, 255},
    {TEXT_1_B4, 3000, 4000, 0, 255},
    {TEXT_1_B5, 4000, 5000, 0, 255},
    {TEXT_1_B6, 4000, 5000, 0, 255},
};
uint16_t sequence_2_B_size = sizeof(sequence_2_B) / sizeof(sequence_2_B[0]);

sequence_commands_st sequence_2_C[] = {
    {TEXT_1_C1, 0,    1000, 0, 255},
    {TEXT_1_C2, 1000, 2000, 0, 255},
    {TEXT_1_C3, 2000, 3000, 0, 255},
    {TEXT_1_C4, 3000, 4000, 0, 255},
    {TEXT_1_C5, 4000, 5000, 0, 255},
};
uint16_t sequence_2_C_size = sizeof(sequence_2_C) / sizeof(sequence_2_C[0]);

sequence_commands_st sequence_2_D[] = {
    {TEXT_1_D1, 0,    1000, 0, 255},
    {TEXT_1_D2, 1000, 2000, 0, 255},
    {TEXT_1_D3, 2000, 3000, 0, 255},
    {TEXT_1_D4, 3000, 4000, 0, 255},
    {TEXT_1_D5, 4000, 5000, 0, 255},
    {TEXT_1_D6, 4000, 5000, 0, 255},
    {TEXT_1_D7, 4000, 5000, 0, 255},
    {TEXT_1_D8, 4000, 5000, 0, 255},
};
uint16_t sequence_2_D_size = sizeof(sequence_2_D) / sizeof(sequence_2_D[0]);

sequence_commands_st sequence_2_P[] = {
    {TEXT_1_P1, 0,    1000, 0, 255},
    {TEXT_1_P2, 1000, 2000, 0, 255},
};
uint16_t sequence_2_P_size = sizeof(sequence_2_P) / sizeof(sequence_2_P[0]);


/************** SEQUENCE 3 **************/
sequence_commands_st sequence_3_A[] = {
    {TEXT_1_A1, 0,    3000, 0, 255},
    {TEXT_1_A2, 2000, 4000, 0, 255},
    {TEXT_1_A3, 3000, 6000, 0, 255},
    {TEXT_1_A4, 5000, 7000, 0, 255},
    {TEXT_1_A5, 6000, 9000, 0, 255},
};
uint16_t sequence_3_A_size = sizeof(sequence_3_A) / sizeof(sequence_3_A[0]);

sequence_commands_st sequence_3_B[] = {
    {TEXT_1_B1, 0,    1000, 0, 255},
    {TEXT_1_B2, 1000, 2000, 0, 255},
    {TEXT_1_B3, 2000, 3000, 0, 255},
    {TEXT_1_B4, 3000, 4000, 0, 255},
    {TEXT_1_B5, 4000, 5000, 0, 255},
    {TEXT_1_B6, 4000, 5000, 0, 255},
};
uint16_t sequence_3_B_size = sizeof(sequence_3_B) / sizeof(sequence_3_B[0]);

sequence_commands_st sequence_3_C[] = {
    {TEXT_1_C1, 0,    1000, 0, 255},
    {TEXT_1_C2, 1000, 2000, 0, 255},
    {TEXT_1_C3, 2000, 3000, 0, 255},
    {TEXT_1_C4, 3000, 4000, 0, 255},
    {TEXT_1_C5, 4000, 5000, 0, 255},
};
uint16_t sequence_3_C_size = sizeof(sequence_3_C) / sizeof(sequence_3_C[0]);

sequence_commands_st sequence_3_D[] = {
    {TEXT_1_D1, 0,    1000, 0, 255},
    {TEXT_1_D2, 1000, 2000, 0, 255},
    {TEXT_1_D3, 2000, 3000, 0, 255},
    {TEXT_1_D4, 3000, 4000, 0, 255},
    {TEXT_1_D5, 4000, 5000, 0, 255},
    {TEXT_1_D6, 4000, 5000, 0, 255},
    {TEXT_1_D7, 4000, 5000, 0, 255},
    {TEXT_1_D8, 4000, 5000, 0, 255},
};
uint16_t sequence_3_D_size = sizeof(sequence_3_D) / sizeof(sequence_3_D[0]);

sequence_commands_st sequence_3_P[] = {
    {TEXT_1_P1, 0,    1000, 0, 255},
    {TEXT_1_P2, 1000, 2000, 0, 255},
};
uint16_t sequence_3_P_size = sizeof(sequence_3_P) / sizeof(sequence_3_P[0]);

/************** SEQUENCE 4 **************/
sequence_commands_st sequence_4_A[] = {
    {TEXT_1_A1, 0,    3000, 0, 255},
    {TEXT_1_A2, 2000, 4000, 0, 255},
    {TEXT_1_A3, 3000, 6000, 0, 255},
    {TEXT_1_A4, 5000, 7000, 0, 255},
    {TEXT_1_A5, 6000, 9000, 0, 255},
};
uint16_t sequence_4_A_size = sizeof(sequence_4_A) / sizeof(sequence_4_A[0]);

sequence_commands_st sequence_4_B[] = {
    {TEXT_1_B1, 0,    1000, 0, 255},
    {TEXT_1_B2, 1000, 2000, 0, 255},
    {TEXT_1_B3, 2000, 3000, 0, 255},
    {TEXT_1_B4, 3000, 4000, 0, 255},
    {TEXT_1_B5, 4000, 5000, 0, 255},
    {TEXT_1_B6, 4000, 5000, 0, 255},
};
uint16_t sequence_4_B_size = sizeof(sequence_4_B) / sizeof(sequence_4_B[0]);

sequence_commands_st sequence_4_C[] = {
    {TEXT_1_C1, 0,    1000, 0, 255},
    {TEXT_1_C2, 1000, 2000, 0, 255},
    {TEXT_1_C3, 2000, 3000, 0, 255},
    {TEXT_1_C4, 3000, 4000, 0, 255},
    {TEXT_1_C5, 4000, 5000, 0, 255},
};
uint16_t sequence_4_C_size = sizeof(sequence_4_C) / sizeof(sequence_4_C[0]);

sequence_commands_st sequence_4_D[] = {
    {TEXT_1_D1, 0,    1000, 0, 255},
    {TEXT_1_D2, 1000, 2000, 0, 255},
    {TEXT_1_D3, 2000, 3000, 0, 255},
    {TEXT_1_D4, 3000, 4000, 0, 255},
    {TEXT_1_D5, 4000, 5000, 0, 255},
    {TEXT_1_D6, 4000, 5000, 0, 255},
    {TEXT_1_D7, 4000, 5000, 0, 255},
    {TEXT_1_D8, 4000, 5000, 0, 255},
};
uint16_t sequence_4_D_size = sizeof(sequence_4_D) / sizeof(sequence_4_D[0]);

sequence_commands_st sequence_4_E[] = {
    {TEXT_1_D1, 0,    1000, 0, 255},
    {TEXT_1_D2, 1000, 2000, 0, 255},
    {TEXT_1_D3, 2000, 3000, 0, 255},
    {TEXT_1_D4, 3000, 4000, 0, 255},
    {TEXT_1_D5, 4000, 5000, 0, 255},
    {TEXT_1_D6, 4000, 5000, 0, 255},
    {TEXT_1_D7, 4000, 5000, 0, 255},
    {TEXT_1_D8, 4000, 5000, 0, 255},
};
uint16_t sequence_4_E_size = sizeof(sequence_4_E) / sizeof(sequence_4_E[0]);

sequence_commands_st sequence_4_P[] = {
    {TEXT_1_P1, 0,    1000, 0, 255},
    {TEXT_1_P2, 1000, 2000, 0, 255},
};
uint16_t sequence_4_P_size = sizeof(sequence_4_P) / sizeof(sequence_4_P[0]);


static uint32_t internal_sequence_timer = 0U;

static uint32_t textile_1_presence = 0;
static uint32_t textile_2_presence = 0;
static uint32_t textile_3_presence = 0;
static uint32_t textile_4_presence = 0;
    
static uint32_t textile_1_current_sequence = 0;
static uint32_t textile_2_current_sequence = 0;
static uint32_t textile_3_current_sequence = 0;
static uint32_t textile_4_current_sequence = 0;
    
static uint32_t textile_1_previous_sequence = 0;
static uint32_t textile_2_previous_sequence = 0;
static uint32_t textile_3_previous_sequence = 0;
static uint32_t textile_4_previous_sequence = 0;

static uint32_t textile_1_trigger_type = 0;
static uint32_t textile_2_trigger_type = 0;
static uint32_t textile_3_trigger_type = 0;
static uint32_t textile_4_trigger_type = 0;

void update_datatable_data(void){
    get_data_table(TAG_TEXTILE_1_PRESENCE_DETECTED, &textile_1_presence, sizeof(textile_1_presence));
    get_data_table(TAG_TEXTILE_2_PRESENCE_DETECTED, &textile_2_presence, sizeof(textile_2_presence));
    get_data_table(TAG_TEXTILE_3_PRESENCE_DETECTED, &textile_3_presence, sizeof(textile_3_presence));
    get_data_table(TAG_TEXTILE_4_PRESENCE_DETECTED, &textile_4_presence, sizeof(textile_4_presence));

    get_data_table(TAG_TEXTILE_1_CURRENT_SEQUENCE_STATE, &textile_1_current_sequence, sizeof(textile_1_current_sequence));
    get_data_table(TAG_TEXTILE_2_CURRENT_SEQUENCE_STATE, &textile_2_current_sequence, sizeof(textile_2_current_sequence));
    get_data_table(TAG_TEXTILE_3_CURRENT_SEQUENCE_STATE, &textile_3_current_sequence, sizeof(textile_3_current_sequence));
    get_data_table(TAG_TEXTILE_4_CURRENT_SEQUENCE_STATE, &textile_4_current_sequence, sizeof(textile_4_current_sequence));

    get_data_table(TAG_TEXTILE_1_PREVIOUS_SEQUENCE_STATE, &textile_1_previous_sequence, sizeof(textile_1_previous_sequence));
    get_data_table(TAG_TEXTILE_2_PREVIOUS_SEQUENCE_STATE, &textile_2_previous_sequence, sizeof(textile_2_previous_sequence));
    get_data_table(TAG_TEXTILE_3_PREVIOUS_SEQUENCE_STATE, &textile_3_previous_sequence, sizeof(textile_3_previous_sequence));
    get_data_table(TAG_TEXTILE_4_PREVIOUS_SEQUENCE_STATE, &textile_4_previous_sequence, sizeof(textile_4_previous_sequence));

    get_data_table(TAG_TEXTILE_1_TRIGGER_TYPE, &textile_1_trigger_type, sizeof(textile_1_trigger_type));
    get_data_table(TAG_TEXTILE_2_TRIGGER_TYPE, &textile_2_trigger_type, sizeof(textile_2_trigger_type));
    get_data_table(TAG_TEXTILE_3_TRIGGER_TYPE, &textile_3_trigger_type, sizeof(textile_3_trigger_type));
    get_data_table(TAG_TEXTILE_4_TRIGGER_TYPE, &textile_4_trigger_type, sizeof(textile_4_trigger_type));

}

void print_debug_datatable_data(void){
    printf("Textile Presence.       1: %d, 2: %d, 3: %d, 4: %d", textile_1_presence,           textile_2_presence,          textile_3_presence,          textile_4_presence);
    printf("Textile Current state.  1: %d, 2: %d, 3: %d, 4: %d", textile_1_current_sequence,   textile_2_current_sequence,  textile_3_current_sequence,  textile_4_current_sequence);
    printf("Textile previous state. 1: %d, 2: %d, 3: %d, 4: %d", textile_1_previous_sequence,  textile_2_previous_sequence, textile_3_previous_sequence, textile_4_previous_sequence);
    printf("Textile Trigger Type.   1: %d, 2: %d, 3: %d, 4: %d", textile_1_trigger_type,       textile_2_trigger_type,      textile_3_trigger_type,      textile_4_trigger_type);
}


static uint32_t get_current_sequence()
{
    uint32_t current_sequence_tag = 0;
    
    switch (textile_id) {
        case TEXTILE_1:
            current_sequence_tag  = TAG_TEXTILE_1_CURRENT_SEQUENCE_STATE;
            break;
        case TEXTILE_2:
            current_sequence_tag  = TAG_TEXTILE_2_CURRENT_SEQUENCE_STATE;
            break;
        case TEXTILE_3:
            current_sequence_tag  = TAG_TEXTILE_3_CURRENT_SEQUENCE_STATE;
            break;
        case TEXTILE_4:
            current_sequence_tag  = TAG_TEXTILE_4_CURRENT_SEQUENCE_STATE;
            break;
        default:
            current_sequence_tag  = TAG_TEXTILE_1_CURRENT_SEQUENCE_STATE;
    }
    uint32_t tmp_var = 0;
    get_data_table(current_sequence_tag, &tmp_var, sizeof(tmp_var));
    return tmp_var;
}

void update_sequence(uint32_t current_sequence)
{
    uint32_t current_sequence_tag = 0;
    uint32_t previous_sequence_tag = 0;

    switch (textile_id) {
        case TEXTILE_1:
            current_sequence_tag  = TAG_TEXTILE_1_CURRENT_SEQUENCE_STATE;
            previous_sequence_tag = TAG_TEXTILE_1_PREVIOUS_SEQUENCE_STATE;
            break;
        case TEXTILE_2:
            current_sequence_tag  = TAG_TEXTILE_2_CURRENT_SEQUENCE_STATE;
            previous_sequence_tag = TAG_TEXTILE_2_PREVIOUS_SEQUENCE_STATE;
            break;
        case TEXTILE_3:
            current_sequence_tag  = TAG_TEXTILE_3_CURRENT_SEQUENCE_STATE;
            previous_sequence_tag = TAG_TEXTILE_3_PREVIOUS_SEQUENCE_STATE;
            break;
        case TEXTILE_4:
            current_sequence_tag  = TAG_TEXTILE_4_CURRENT_SEQUENCE_STATE;
            previous_sequence_tag = TAG_TEXTILE_4_PREVIOUS_SEQUENCE_STATE;
            break;
        default:
            current_sequence_tag  = TAG_TEXTILE_1_CURRENT_SEQUENCE_STATE;
            previous_sequence_tag = TAG_TEXTILE_1_PREVIOUS_SEQUENCE_STATE;
    }
    uint32_t tmp_var = 0;
    get_data_table(current_sequence_tag, &tmp_var, sizeof(tmp_var));
    write_data_table(previous_sequence_tag, &tmp_var, sizeof(tmp_var));
    
    tmp_var = current_sequence;
    write_data_table(current_sequence_tag, &active_sequence, sizeof(active_sequence));
    internal_sequence_timer = millis();
}

void update_trigger(uint32_t new_trigger)
{
    uint32_t trigger_tag = 0;

    switch (textile_id) {
        case TEXTILE_1:
            trigger_tag  = TAG_TEXTILE_1_TRIGGER_TYPE;
            break;
        case TEXTILE_2:
            trigger_tag  = TAG_TEXTILE_2_TRIGGER_TYPE;
            break;
        case TEXTILE_3:
            trigger_tag  = TAG_TEXTILE_3_TRIGGER_TYPE;
            break;
        case TEXTILE_4:
            trigger_tag  = TAG_TEXTILE_4_TRIGGER_TYPE;
            break;
        default:
            trigger_tag  = TAG_TEXTILE_1_TRIGGER_TYPE;
    }

    uint32_t tmp_var = new_trigger; 
    write_data_table(trigger_tag, &tmp_var, sizeof(tmp_var));
}

void loop_sequence(uint8_t debug_info = 0){

    update_datatable_data();
    if(0 != debug_info)
        print_debug_datatable_data();

    switch (textile_id) {
        case TEXTILE_1:
            textile_1_loop();
            break;
        case TEXTILE_2:
            textile_2_loop();
            break;
        case TEXTILE_3:
            textile_3_loop();
            break;
        case TEXTILE_4:
            textile_4_loop();
            break;
        default:
            textile_1_loop();
            break;
    }

    
    static unsigned long lastTimeoutMs = 0;
    const unsigned long TIMEOUT_VALUE  = 10000UL; // 10 s

    unsigned long now = millis();

    uint32_t current_state_tmp = get_current_sequence();

    if(current_state_tmp == IDLE)
    {
        lastTimeoutMs = now;
    }

    if(current_state_tmp != IDLE)
    {
        if (now - lastTimeoutMs >= TIMEOUT_VALUE) {
            lastTimeoutMs = now;
            update_sequence(IDLE);
            update_trigger(NOT_TRIGGERED);
            clearPins();
        }
    }
}

void textile_1_loop(void){
    if( textile_1_current_sequence == IDLE &&
        textile_2_current_sequence == IDLE &&
        textile_3_current_sequence == IDLE &&
        textile_4_current_sequence == IDLE && 
        textile_1_presence != 0 && 
        textile_2_presence == 0 &&
        textile_3_presence == 0 &&
        textile_4_presence == 0 ) // STARTS SEQUENCE AT TEXT 1 SINGLE
    {
        update_trigger(SINGLE);
        update_sequence(SEQUENCE_D);
    }


    if( textile_1_current_sequence == IDLE &&
        textile_2_current_sequence == FINISHED &&
        textile_3_current_sequence == FINISHED &&
        textile_4_current_sequence == FINISHED &&
        textile_2_trigger_type == SINGLE) // STARTS SEQUENCE AT TEXT 2
    {
        update_sequence(SEQUENCE_C);
    }
    
    if( textile_1_current_sequence == IDLE &&
        textile_2_current_sequence == IDLE &&
        textile_3_current_sequence == FINISHED &&
        textile_4_current_sequence == IDLE &&
        textile_3_trigger_type == SINGLE) // STARTS SEQUENCE AT TEXT 3
    {
        update_sequence(SEQUENCE_A);
    }

    if( textile_1_current_sequence == IDLE &&
        textile_2_current_sequence == FINISHED &&
        textile_3_current_sequence == FINISHED &&
        textile_4_current_sequence == FINISHED &&
        textile_4_trigger_type == SINGLE) // STARTS SEQUENCE AT TEXT 4
    {
        update_sequence(SEQUENCE_B);
    }

    if( textile_1_current_sequence == FINISHED &&
        textile_2_current_sequence == FINISHED &&
        textile_3_current_sequence == FINISHED &&
        textile_4_current_sequence == FINISHED) // EVERY ONE IS FINISHED RETURN TO IDLE
    {
        update_trigger(NOT_TRIGGERED);
        update_sequence(IDLE);
    }

    if( textile_1_current_sequence == SEQUENCE_A ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        for (uint16_t i = 0 ; i < sequence_1_A_size ; i++)
        {
            if((current_timer >= sequence_1_A[i].time_starts_at) &&
               (current_timer <= sequence_1_A[i].time_ends_at))
            {
                uint32_t duration = (sequence_1_A[i].time_ends_at - sequence_1_A[i].time_starts_at);
                uint32_t middle_point = (sequence_1_A[i].time_starts_at + (duration/2));
                setPin(sequence_1_A[i].led_index.positive_pin, 255);
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, 0 , (duration/2), sequence_1_A[i].value_starts_at, sequence_1_A[i].value_ends_at);
                    setPin(sequence_1_A[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_1_A[i].value_starts_at, sequence_1_A[i].value_ends_at);
                    setPin(sequence_1_A[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_1_A[sequence_1_A_size - 1].time_ends_at)
        {
            for (uint16_t i = 0 ; i < sequence_1_A_size ; i++)
            {
                setPin(sequence_1_A[i].led_index.positive_pin, 0);
                setPin(sequence_1_A[i].led_index.negative_pin, 0);
            }
            update_sequence(FINISHED);
        }
    }

    if( textile_1_current_sequence == SEQUENCE_B ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        for (uint16_t i = 0 ; i < sequence_1_B_size ; i++)
        {
            if((current_timer >= sequence_1_B[i].time_starts_at) &&
               (current_timer <= sequence_1_B[i].time_ends_at))
            {
                uint32_t duration = (sequence_1_B[i].time_ends_at - sequence_1_B[i].time_starts_at);
                uint32_t middle_point = (sequence_1_B[i].time_starts_at + (duration/2));
                setPin(sequence_1_B[i].led_index.positive_pin, 255);
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, 0 , (duration/2), sequence_1_B[i].value_starts_at, sequence_1_B[i].value_ends_at);
                    setPin(sequence_1_B[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point; 
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_1_B[i].value_starts_at, sequence_1_B[i].value_ends_at);
                    setPin(sequence_1_B[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_1_B[sequence_1_B_size - 1].time_ends_at)
        {
            for (uint16_t i = 0 ; i < sequence_1_B_size ; i++)
            {
                setPin(sequence_1_B[i].led_index.positive_pin, 0);
                setPin(sequence_1_B[i].led_index.negative_pin, 0);
            }
            update_sequence(FINISHED);
        }
    }

    if( textile_1_current_sequence == SEQUENCE_C ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        for (uint16_t i = 0 ; i < sequence_1_C_size ; i++)
        {
            if((current_timer >= sequence_1_C[i].time_starts_at) &&
               (current_timer <= sequence_1_C[i].time_ends_at))
            {
                uint32_t duration = (sequence_1_C[i].time_ends_at - sequence_1_C[i].time_starts_at);
                uint32_t middle_point = (sequence_1_C[i].time_starts_at + (duration/2));
                setPin(sequence_1_C[i].led_index.positive_pin, 255);
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer; 
                    uint8_t mapped_value = map(duration_left, 0 , (duration/2), sequence_1_C[i].value_starts_at, sequence_1_C[i].value_ends_at);
                    setPin(sequence_1_C[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_1_C[i].value_starts_at, sequence_1_C[i].value_ends_at);
                    setPin(sequence_1_C[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_1_C[sequence_1_C_size-1].time_ends_at)
        {
            for (uint16_t i = 0 ; i < sequence_1_C_size ; i++)
            {
                setPin(sequence_1_C[i].led_index.positive_pin, 0);
                setPin(sequence_1_C[i].led_index.negative_pin, 0);
            }
            update_sequence(FINISHED);
        }
    }

    if( textile_1_current_sequence == SEQUENCE_D ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        for (uint16_t i = 0 ; i < sequence_1_D_size ; i++)
        {
            if((current_timer >= sequence_1_D[i].time_starts_at) &&
               (current_timer <= sequence_1_D[i].time_ends_at))
            {
                uint32_t duration = (sequence_1_D[i].time_ends_at - sequence_1_D[i].time_starts_at);
                uint32_t middle_point = (sequence_1_D[i].time_starts_at + (duration/2));
                setPin(sequence_1_D[i].led_index.positive_pin, 255);
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_1_D[i].value_starts_at, sequence_1_D[i].value_ends_at);
                    setPin(sequence_1_D[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_1_D[i].value_starts_at, sequence_1_D[i].value_ends_at);
                    setPin(sequence_1_D[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_1_D[sequence_1_D_size - 1].time_ends_at)
        {
            for (uint16_t i = 0 ; i < sequence_1_D_size ; i++)
            {
                setPin(sequence_1_D[i].led_index.positive_pin, 0);
                setPin(sequence_1_D[i].led_index.negative_pin, 0);
            }
            update_sequence(FINISHED);
        }
    }

    if( textile_1_current_sequence == SEQUENCE_P ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        for (uint16_t i = 0 ; i < sequence_1_P_size ; i++)
        {
            if((current_timer >= sequence_1_P[i].time_starts_at) &&
               (current_timer <= sequence_1_P[i].time_ends_at))
            {
                uint32_t duration = (sequence_1_P[i].time_ends_at - sequence_1_P[i].time_starts_at);
                uint32_t middle_point = (sequence_1_P[i].time_starts_at + (duration/2));
                setPin(sequence_1_P[i].led_index.positive_pin, 255);
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_1_P[i].value_starts_at, sequence_1_P[i].value_ends_at);
                    setPin(sequence_1_P[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_1_P[i].value_starts_at, sequence_1_P[i].value_ends_at);
                    setPin(sequence_1_P[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_1_P[sequence_1_P_size - 1].time_ends_at)
        {
            for (uint16_t i = 0 ; i < sequence_1_P_size ; i++)
            {
                setPin(sequence_1_P[i].led_index.positive_pin, 0);
                setPin(sequence_1_P[i].led_index.negative_pin, 0);
            }
            update_sequence(FINISHED);
        }
    }

}


void textile_2_loop(void){
    if( textile_1_current_sequence == IDLE &&
        textile_2_current_sequence == IDLE &&
        textile_3_current_sequence == IDLE &&
        textile_4_current_sequence == IDLE && 
        textile_1_presence == 0 && 
        textile_2_presence != 0 &&
        textile_3_presence == 0 &&
        textile_4_presence == 0 ) // STARTS SEQUENCE AT TEXT 2 SINGLE
    {
        uint32_t tmp = SINGLE;
        write_data_table(TAG_TEXTILE_2_TRIGGER_TYPE, &tmp, sizeof(tmp));
        
        update_sequence(SEQUENCE_B);
    }


    if( textile_1_current_sequence == FINISHED &&
        textile_2_current_sequence == IDLE &&
        textile_3_current_sequence == IDLE &&
        textile_4_current_sequence == IDLE &&
        textile_1_trigger_type == SINGLE) // STARTS SEQUENCE AT TEXT 1
    {
        update_sequence(SEQUENCE_A);
    }
    
    if( textile_1_current_sequence == FINISHED &&
        textile_2_current_sequence == FINISHED &&
        textile_3_current_sequence == IDLE &&
        textile_4_current_sequence == FINISHED &&
        textile_3_trigger_type == SINGLE) // STARTS SEQUENCE AT TEXT 3
    {
        update_sequence(SEQUENCE_E);
    }

    if( textile_1_current_sequence == IDLE &&
        textile_2_current_sequence == IDLE &&
        textile_3_current_sequence == FINISHED &&
        textile_4_current_sequence == FINISHED &&
        textile_4_trigger_type == SINGLE) // STARTS SEQUENCE AT TEXT 4
    {
        update_sequence(SEQUENCE_D);
    }

    if( textile_1_current_sequence == FINISHED &&
        textile_2_current_sequence == FINISHED &&
        textile_3_current_sequence == FINISHED &&
        textile_4_current_sequence == FINISHED) // EVERY ONE IS FINISHED RETURN TO IDLE
    {
        uint32_t tmp = NOT_TRIGGERED;
        write_data_table(TAG_TEXTILE_2_TRIGGER_TYPE, &tmp, sizeof(tmp));
        
        update_sequence(IDLE);
    }

    if( textile_2_current_sequence == SEQUENCE_A ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        for (uint16_t i = 0 ; i < sequence_2_A_size ; i++)
        {
            if((current_timer >= sequence_2_A[i].time_starts_at) &&
               (current_timer <= sequence_2_A[i].time_ends_at))
            {
                uint32_t duration = (sequence_2_A[i].time_ends_at - sequence_2_A[i].time_starts_at);
                uint32_t middle_point = (sequence_2_A[i].time_starts_at + (duration/2));
                setPin(sequence_2_A[i].led_index.positive_pin, 255);
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_2_A[i].value_starts_at, sequence_2_A[i].value_ends_at);
                    setPin(sequence_2_A[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_2_A[i].value_starts_at, sequence_2_A[i].value_ends_at);
                    setPin(sequence_2_A[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_2_A[sequence_2_A_size - 1].time_ends_at)
        {
            for (uint16_t i = 0 ; i < sequence_2_A_size ; i++)
            {
                setPin(sequence_2_A[i].led_index.positive_pin, 0);
                setPin(sequence_2_A[i].led_index.negative_pin, 0);
            }
            update_sequence(FINISHED);
        }
    }

    if( textile_2_current_sequence == SEQUENCE_B ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        for (uint16_t i = 0 ; i < sequence_2_B_size ; i++)
        {
            if((current_timer >= sequence_2_B[i].time_starts_at) &&
               (current_timer <= sequence_2_B[i].time_ends_at))
            {
                uint32_t duration = (sequence_2_B[i].time_ends_at - sequence_2_B[i].time_starts_at);
                uint32_t middle_point = (sequence_2_B[i].time_starts_at + (duration/2));
                setPin(sequence_2_B[i].led_index.positive_pin, 255);
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_2_B[i].value_starts_at, sequence_2_B[i].value_ends_at);
                    setPin(sequence_2_B[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_2_B[i].value_starts_at, sequence_2_B[i].value_ends_at);
                    setPin(sequence_2_B[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_2_B[sequence_2_B_size - 1].time_ends_at)
        {
            for (uint16_t i = 0 ; i < sequence_2_B_size ; i++)
            {
                setPin(sequence_2_B[i].led_index.positive_pin, 0);
                setPin(sequence_2_B[i].led_index.negative_pin, 0);
            }
            update_sequence(FINISHED);
        }
    }

    if( textile_2_current_sequence == SEQUENCE_C ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        for (uint16_t i = 0 ; i < sequence_2_C_size ; i++)
        {
            if((current_timer >= sequence_2_C[i].time_starts_at) &&
               (current_timer <= sequence_2_C[i].time_ends_at))
            {
                uint32_t duration = (sequence_2_C[i].time_ends_at - sequence_2_C[i].time_starts_at);
                uint32_t middle_point = (sequence_2_C[i].time_starts_at + (duration/2));
                setPin(sequence_2_C[i].led_index.positive_pin, 255);
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_2_C[i].value_starts_at, sequence_2_C[i].value_ends_at);
                    setPin(sequence_2_C[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_2_C[i].value_starts_at, sequence_2_C[i].value_ends_at);
                    setPin(sequence_2_C[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_2_C[sequence_2_C_size - 1].time_ends_at)
        {
            for (uint16_t i = 0 ; i < sequence_2_C_size ; i++)
            {
                setPin(sequence_2_C[i].led_index.positive_pin, 0);
                setPin(sequence_2_C[i].led_index.negative_pin, 0);
            }
            update_sequence(FINISHED);
        }
    }

    if( textile_2_current_sequence == SEQUENCE_D ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        for (uint16_t i = 0 ; i < sequence_2_D_size ; i++)
        {
            if((current_timer >= sequence_2_D[i].time_starts_at) &&
               (current_timer <= sequence_2_D[i].time_ends_at))
            {
                uint32_t duration = (sequence_2_D[i].time_ends_at - sequence_2_D[i].time_starts_at);
                uint32_t middle_point = (sequence_2_D[i].time_starts_at + (duration/2));
                setPin(sequence_2_D[i].led_index.positive_pin, 255);
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_2_D[i].value_starts_at, sequence_2_D[i].value_ends_at);
                    setPin(sequence_2_D[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_2_D[i].value_starts_at, sequence_2_D[i].value_ends_at);
                    setPin(sequence_2_D[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_2_D[sequence_2_D_size - 1].time_ends_at)
        {
            for (uint16_t i = 0 ; i < sequence_2_D_size ; i++)
            {
                setPin(sequence_2_D[i].led_index.positive_pin, 0);
                setPin(sequence_2_D[i].led_index.negative_pin, 0);
            }
            update_sequence(FINISHED);
        }
    }

    if( textile_2_current_sequence == SEQUENCE_P ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        for (uint16_t i = 0 ; i < sequence_2_P_size ; i++)
        {
            if((current_timer >= sequence_2_P[i].time_starts_at) &&
               (current_timer <= sequence_2_P[i].time_ends_at))
            {
                uint32_t duration = (sequence_2_P[i].time_ends_at - sequence_2_P[i].time_starts_at);
                uint32_t middle_point = (sequence_2_P[i].time_starts_at + (duration/2));
                setPin(sequence_2_P[i].led_index.positive_pin, 255);
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_2_P[i].value_starts_at, sequence_2_P[i].value_ends_at);
                    setPin(sequence_2_P[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_2_P[i].value_starts_at, sequence_2_P[i].value_ends_at);
                    setPin(sequence_2_P[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_2_P[sequence_2_P_size - 1].time_ends_at)
        {
            for (uint16_t i = 0 ; i < sequence_2_P_size ; i++)
            {
                setPin(sequence_2_P[i].led_index.positive_pin, 0);
                setPin(sequence_2_P[i].led_index.negative_pin, 0);
            }
            update_sequence(FINISHED);
        }
    }

}


void textile_3_loop(void){
    if( textile_1_current_sequence == IDLE &&
        textile_2_current_sequence == IDLE &&
        textile_3_current_sequence == IDLE &&
        textile_4_current_sequence == IDLE && 
        textile_1_presence == 0 && 
        textile_2_presence == 0 &&
        textile_3_presence != 0 &&
        textile_4_presence == 0 ) // STARTS SEQUENCE AT TEXT 3 SINGLE
    {
        uint32_t tmp = SINGLE;
        write_data_table(TAG_TEXTILE_3_TRIGGER_TYPE, &tmp, sizeof(tmp));
        
        update_sequence(SEQUENCE_A);
    }


    if( textile_1_current_sequence == FINISHED &&
        textile_2_current_sequence == FINISHED &&
        textile_3_current_sequence == IDLE &&
        textile_4_current_sequence == IDLE &&
        textile_1_trigger_type == SINGLE) // STARTS SEQUENCE AT TEXT 1
    {
        update_sequence(SEQUENCE_B);
    }
    
    if( textile_1_current_sequence == FINISHED &&
        textile_2_current_sequence == IDLE &&
        textile_3_current_sequence == IDLE &&
        textile_4_current_sequence == IDLE &&
        textile_2_trigger_type == SINGLE) // STARTS SEQUENCE AT TEXT 2
    {
        update_sequence(SEQUENCE_C);
    }

    if( textile_1_current_sequence == IDLE &&
        textile_2_current_sequence == IDLE &&
        textile_3_current_sequence == IDLE &&
        textile_4_current_sequence == FINISHED &&
        textile_4_trigger_type == SINGLE) // STARTS SEQUENCE AT TEXT 4
    {
        update_sequence(SEQUENCE_D);
    }

    if( textile_1_current_sequence == FINISHED &&
        textile_2_current_sequence == FINISHED &&
        textile_3_current_sequence == FINISHED &&
        textile_4_current_sequence == FINISHED) // EVERY ONE IS FINISHED RETURN TO IDLE
    {
        uint32_t tmp = NOT_TRIGGERED;
        write_data_table(TAG_TEXTILE_3_TRIGGER_TYPE, &tmp, sizeof(tmp));
        
        update_sequence(IDLE);
    }

    if( textile_3_current_sequence == SEQUENCE_A ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        for (uint16_t i = 0 ; i < sequence_3_A_size ; i++)
        {
            if((current_timer >= sequence_3_A[i].time_starts_at) &&
               (current_timer <= sequence_3_A[i].time_ends_at))
            {
                uint32_t duration = (sequence_3_A[i].time_ends_at - sequence_3_A[i].time_starts_at);
                uint32_t middle_point = (sequence_3_A[i].time_starts_at + (duration/2));
                setPin(sequence_3_A[i].led_index.positive_pin, 255);
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_3_A[i].value_starts_at, sequence_3_A[i].value_ends_at);
                    setPin(sequence_3_A[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_3_A[i].value_starts_at, sequence_3_A[i].value_ends_at);
                    setPin(sequence_3_A[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_3_A[sequence_3_A_size - 1].time_ends_at)
        {
            for (uint16_t i = 0 ; i < sequence_3_A_size ; i++)
            {
                setPin(sequence_3_A[i].led_index.positive_pin, 0);
                setPin(sequence_3_A[i].led_index.negative_pin, 0);
            }
            update_sequence(FINISHED);
        }
    }

    if( textile_3_current_sequence == SEQUENCE_B ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        for (uint16_t i = 0 ; i < sequence_3_B_size ; i++)
        {
            if((current_timer >= sequence_3_B[i].time_starts_at) &&
               (current_timer <= sequence_3_B[i].time_ends_at))
            {
                uint32_t duration = (sequence_3_B[i].time_ends_at - sequence_3_B[i].time_starts_at);
                uint32_t middle_point = (sequence_3_B[i].time_starts_at + (duration/2));
                setPin(sequence_3_B[i].led_index.positive_pin, 255);
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_3_B[i].value_starts_at, sequence_3_B[i].value_ends_at);
                    setPin(sequence_3_B[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_3_B[i].value_starts_at, sequence_3_B[i].value_ends_at);
                    setPin(sequence_3_B[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_3_B[sequence_3_B_size - 1].time_ends_at)
        {
            for (uint16_t i = 0 ; i < sequence_3_B_size ; i++)
            {
                setPin(sequence_3_B[i].led_index.positive_pin, 0);
                setPin(sequence_3_B[i].led_index.negative_pin, 0);
            }
            update_sequence(FINISHED);
        }
    }

    if( textile_3_current_sequence == SEQUENCE_C ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        for (uint16_t i = 0 ; i < sequence_3_C_size ; i++)
        {
            if((current_timer >= sequence_3_C[i].time_starts_at) &&
               (current_timer <= sequence_3_C[i].time_ends_at))
            {
                uint32_t duration = (sequence_3_C[i].time_ends_at - sequence_3_C[i].time_starts_at);
                uint32_t middle_point = (sequence_3_C[i].time_starts_at + (duration/2));
                setPin(sequence_3_C[i].led_index.positive_pin, 255);
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_3_C[i].value_starts_at, sequence_3_C[i].value_ends_at);
                    setPin(sequence_3_C[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_3_C[i].value_starts_at, sequence_3_C[i].value_ends_at);
                    setPin(sequence_3_C[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_3_C[sequence_3_C_size - 1].time_ends_at)
        {
            for (uint16_t i = 0 ; i < sequence_3_C_size ; i++)
            {
                setPin(sequence_3_C[i].led_index.positive_pin, 0);
                setPin(sequence_3_C[i].led_index.negative_pin, 0);
            }
            update_sequence(FINISHED);
        }
    }

    if( textile_3_current_sequence == SEQUENCE_D ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        for (uint16_t i = 0 ; i < sequence_3_D_size ; i++)
        {
            if((current_timer >= sequence_3_D[i].time_starts_at) &&
               (current_timer <= sequence_3_D[i].time_ends_at))
            {
                uint32_t duration = (sequence_3_D[i].time_ends_at - sequence_3_D[i].time_starts_at);
                uint32_t middle_point = (sequence_3_D[i].time_starts_at + (duration/2));
                setPin(sequence_3_D[i].led_index.positive_pin, 255);
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_3_D[i].value_starts_at, sequence_3_D[i].value_ends_at);
                    setPin(sequence_3_D[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_3_D[i].value_starts_at, sequence_3_D[i].value_ends_at);
                    setPin(sequence_3_D[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_3_D[sequence_3_D_size - 1].time_ends_at)
        {
            for (uint16_t i = 0 ; i < sequence_3_D_size ; i++)
            {
                setPin(sequence_3_D[i].led_index.positive_pin, 0);
                setPin(sequence_3_D[i].led_index.negative_pin, 0);
            }
            update_sequence(FINISHED);
        }
    }

    if( textile_3_current_sequence == SEQUENCE_P ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        for (uint16_t i = 0 ; i < sequence_3_P_size ; i++)
        {
            if((current_timer >= sequence_3_P[i].time_starts_at) &&
               (current_timer <= sequence_3_P[i].time_ends_at))
            {
                uint32_t duration = (sequence_3_P[i].time_ends_at - sequence_3_P[i].time_starts_at);
                uint32_t middle_point = (sequence_3_P[i].time_starts_at + (duration/2));
                setPin(sequence_3_P[i].led_index.positive_pin, 255);
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_3_P[i].value_starts_at, sequence_3_P[i].value_ends_at);
                    setPin(sequence_3_P[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_3_P[i].value_starts_at, sequence_3_P[i].value_ends_at);
                    setPin(sequence_3_P[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_3_P[sequence_3_P_size - 1].time_ends_at)
        {
            for (uint16_t i = 0 ; i < sequence_3_P_size ; i++)
            {
                setPin(sequence_3_P[i].led_index.positive_pin, 0);
                setPin(sequence_3_P[i].led_index.negative_pin, 0);
            }
            update_sequence(FINISHED);
        }
    }

}


void textile_4_loop(void){
    if( textile_1_current_sequence == IDLE &&
        textile_2_current_sequence == IDLE &&
        textile_3_current_sequence == IDLE &&
        textile_4_current_sequence == IDLE && 
        textile_1_presence == 0 && 
        textile_2_presence == 0 &&
        textile_3_presence == 0 &&
        textile_4_presence != 0 ) // STARTS SEQUENCE AT TEXT 4 SINGLE
    {
        uint32_t tmp = SINGLE;
        write_data_table(TAG_TEXTILE_4_TRIGGER_TYPE, &tmp, sizeof(tmp));
        
        update_sequence(SEQUENCE_C);
    }


    if( textile_1_current_sequence == FINISHED &&
        textile_2_current_sequence == FINISHED &&
        textile_3_current_sequence == FINISHED &&
        textile_4_current_sequence == IDLE &&
        textile_1_trigger_type == SINGLE) // STARTS SEQUENCE AT TEXT 1
    {
        update_sequence(SEQUENCE_B);
    }
    
    if( textile_1_current_sequence == IDLE &&
        textile_2_current_sequence == FINISHED &&
        textile_3_current_sequence == FINISHED &&
        textile_4_current_sequence == IDLE &&
        textile_2_trigger_type == SINGLE) // STARTS SEQUENCE AT TEXT 2
    {
        update_sequence(SEQUENCE_D);
    }

    if( textile_1_current_sequence == FINISHED &&
        textile_2_current_sequence == IDLE &&
        textile_3_current_sequence == FINISHED &&
        textile_4_current_sequence == IDLE &&
        textile_3_trigger_type == SINGLE) // STARTS SEQUENCE AT TEXT 3
    {
        update_sequence(SEQUENCE_E);
    }

    if( textile_1_current_sequence == FINISHED &&
        textile_2_current_sequence == FINISHED &&
        textile_3_current_sequence == FINISHED &&
        textile_4_current_sequence == FINISHED) // EVERY ONE IS FINISHED RETURN TO IDLE
    {
        uint32_t tmp = NOT_TRIGGERED;
        write_data_table(TAG_TEXTILE_4_TRIGGER_TYPE, &tmp, sizeof(tmp));
        
        update_sequence(IDLE);
    }

    if( textile_4_current_sequence == SEQUENCE_A ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        for (uint16_t i = 0 ; i < sequence_4_A_size ; i++)
        {
            if((current_timer >= sequence_4_A[i].time_starts_at) &&
               (current_timer <= sequence_4_A[i].time_ends_at))
            {
                uint32_t duration = (sequence_4_A[i].time_ends_at - sequence_4_A[i].time_starts_at);
                uint32_t middle_point = (sequence_4_A[i].time_starts_at + (duration/2));
                setPin(sequence_4_A[i].led_index.positive_pin, 255);
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_4_A[i].value_starts_at, sequence_4_A[i].value_ends_at);
                    setPin(sequence_4_A[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_4_A[i].value_starts_at, sequence_4_A[i].value_ends_at);
                    setPin(sequence_4_A[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_4_A[sequence_4_A_size - 1].time_ends_at)
        {
            for (uint16_t i = 0 ; i < sequence_4_A_size ; i++)
            {
                setPin(sequence_4_A[i].led_index.positive_pin, 0);
                setPin(sequence_4_A[i].led_index.negative_pin, 0);
            }
            update_sequence(FINISHED);
        }
    }

    if( textile_4_current_sequence == SEQUENCE_B ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        for (uint16_t i = 0 ; i < sequence_4_B_size ; i++)
        {
            if((current_timer >= sequence_4_B[i].time_starts_at) &&
               (current_timer <= sequence_4_B[i].time_ends_at))
            {
                uint32_t duration = (sequence_4_B[i].time_ends_at - sequence_4_B[i].time_starts_at);
                uint32_t middle_point = (sequence_4_B[i].time_starts_at + (duration/2));
                setPin(sequence_4_B[i].led_index.positive_pin, 255);
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_4_B[i].value_starts_at, sequence_4_B[i].value_ends_at);
                    setPin(sequence_4_B[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_4_B[i].value_starts_at, sequence_4_B[i].value_ends_at);
                    setPin(sequence_4_B[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_4_B[sequence_4_B_size - 1].time_ends_at)
        {
            for (uint16_t i = 0 ; i < sequence_4_B_size ; i++)
            {
                setPin(sequence_4_B[i].led_index.positive_pin, 0);
                setPin(sequence_4_B[i].led_index.negative_pin, 0);
            }
            update_sequence(FINISHED);
        }
    }

    if( textile_4_current_sequence == SEQUENCE_C ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        for (uint16_t i = 0 ; i < sequence_4_C_size ; i++)
        {
            if((current_timer >= sequence_4_C[i].time_starts_at) &&
               (current_timer <= sequence_4_C[i].time_ends_at))
            {
                uint32_t duration = (sequence_4_C[i].time_ends_at - sequence_4_C[i].time_starts_at);
                uint32_t middle_point = (sequence_4_C[i].time_starts_at + (duration/2));
                setPin(sequence_4_C[i].led_index.positive_pin, 255);
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_4_C[i].value_starts_at, sequence_4_C[i].value_ends_at);
                    setPin(sequence_4_C[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_4_C[i].value_starts_at, sequence_4_C[i].value_ends_at);
                    setPin(sequence_4_C[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_4_C[sequence_4_C_size - 1].time_ends_at)
        {
            for (uint16_t i = 0 ; i < sequence_4_C_size ; i++)
            {
                setPin(sequence_4_C[i].led_index.positive_pin, 0);
                setPin(sequence_4_C[i].led_index.negative_pin, 0);
            }
            update_sequence(FINISHED);
        }
    }

    if( textile_4_current_sequence == SEQUENCE_D ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        for (uint16_t i = 0 ; i < sequence_4_D_size ; i++)
        {
            if((current_timer >= sequence_4_D[i].time_starts_at) &&
               (current_timer <= sequence_4_D[i].time_ends_at))
            {
                uint32_t duration = (sequence_4_D[i].time_ends_at - sequence_4_D[i].time_starts_at);
                uint32_t middle_point = (sequence_4_D[i].time_starts_at + (duration/2));
                setPin(sequence_4_D[i].led_index.positive_pin, 255);
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_4_D[i].value_starts_at, sequence_4_D[i].value_ends_at);
                    setPin(sequence_4_D[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_4_D[i].value_starts_at, sequence_4_D[i].value_ends_at);
                    setPin(sequence_4_D[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_4_D[sequence_4_D_size - 1].time_ends_at)
        {
            for (uint16_t i = 0 ; i < sequence_4_D_size ; i++)
            {
                setPin(sequence_4_D[i].led_index.positive_pin, 0);
                setPin(sequence_4_D[i].led_index.negative_pin, 0);
            }
            update_sequence(FINISHED);
        }
    }

    if( textile_4_current_sequence == SEQUENCE_E ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        for (uint16_t i = 0 ; i < sequence_4_E_size ; i++)
        {
            if((current_timer >= sequence_4_E[i].time_starts_at) &&
               (current_timer <= sequence_4_E[i].time_ends_at))
            {
                uint32_t duration = (sequence_4_E[i].time_ends_at - sequence_4_E[i].time_starts_at);
                uint32_t middle_point = (sequence_4_E[i].time_starts_at + (duration/2));
                setPin(sequence_4_E[i].led_index.positive_pin, 255);
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_4_E[i].value_starts_at, sequence_4_E[i].value_ends_at);
                    setPin(sequence_4_E[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_4_E[i].value_starts_at, sequence_4_E[i].value_ends_at);
                    setPin(sequence_4_E[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_4_E[sequence_4_E_size - 1].time_ends_at)
        {
            for (uint16_t i = 0 ; i < sequence_4_E_size ; i++)
            {
                setPin(sequence_4_E[i].led_index.positive_pin, 0);
                setPin(sequence_4_E[i].led_index.negative_pin, 0);
            }
            update_sequence(FINISHED);
        }
    }

    if( textile_4_current_sequence == SEQUENCE_P ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        for (uint16_t i = 0 ; i < sequence_4_P_size ; i++)
        {
            if((current_timer >= sequence_4_P[i].time_starts_at) &&
               (current_timer <= sequence_4_P[i].time_ends_at))
            {
                uint32_t duration = (sequence_4_P[i].time_ends_at - sequence_4_P[i].time_starts_at);
                uint32_t middle_point = (sequence_4_P[i].time_starts_at + (duration/2));
                setPin(sequence_4_P[i].led_index.positive_pin, 255);
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_4_P[i].value_starts_at, sequence_4_P[i].value_ends_at);
                    setPin(sequence_4_P[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_4_P[i].value_starts_at, sequence_4_P[i].value_ends_at);
                    setPin(sequence_4_P[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_4_P[sequence_4_P_size - 1].time_ends_at)
        {
            for (uint16_t i = 0 ; i < sequence_4_P_size ; i++)
            {
                setPin(sequence_4_P[i].led_index.positive_pin, 0);
                setPin(sequence_4_P[i].led_index.negative_pin, 0);
            }
            update_sequence(FINISHED);
        }
    }

}


