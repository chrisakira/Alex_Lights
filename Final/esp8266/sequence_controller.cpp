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
#define DEBOUNCE_TIMER 200


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
};


enum sequence_trigger_s{
    NOT_TRIGGERED = 0,
    SINGLE,
    DOUBLE,
    TRIPLE,
    FOUR,
};


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


typedef struct {
    unsigned long start_time;
    bool condition_met;
    const unsigned long debounce_time;
} debounce_timer_st;


/************** SEQUENCE 1 **************/
sequence_commands_st sequence_1_A[] = {
    {TEXT_1_A1, 0,    3000, 40, 230},
    {TEXT_1_A2, 2000, 4000, 40, 230},
    {TEXT_1_A3, 3000, 6000, 40, 230},
    {TEXT_1_A4, 5000, 7000, 40, 230},
    {TEXT_1_A5, 6000, 9000, 40, 230},
};
uint16_t sequence_1_A_size = sizeof(sequence_1_A) / sizeof(sequence_1_A[0]);

sequence_commands_st sequence_1_B[] = {
    {TEXT_1_B1, 0,    500,  40, 230},
    {TEXT_1_B1, 1000, 1500, 40, 230},
    {TEXT_1_B1, 2000, 3000, 40, 230},
    {TEXT_1_B2, 1500, 2000, 40, 230},
    {TEXT_1_B2, 2500, 3500, 40, 230},
    {TEXT_1_B3, 3000, 3500, 40, 230},
    {TEXT_1_B3, 4000, 4500, 40, 230},
    {TEXT_1_B3, 5000, 6000, 40, 230},
    {TEXT_1_B4, 4500, 5000, 40, 230},
    {TEXT_1_B4, 5500, 6500, 40, 230},
    {TEXT_1_B5, 6000, 6500, 40, 230},
    {TEXT_1_B5, 7000, 7500, 40, 230},
    {TEXT_1_B6, 6500, 7000, 40, 230},
    {TEXT_1_B6, 7500, 8500, 40, 230},
    {TEXT_1_B5, 8000, 9000, 40, 230},
};
uint16_t sequence_1_B_size = sizeof(sequence_1_B) / sizeof(sequence_1_B[0]);

sequence_commands_st sequence_1_C[] = {
    {TEXT_1_C1, 0,    2000, 40, 230},
    {TEXT_1_C2, 1000, 3000, 40, 230},
    {TEXT_1_C3, 2000, 4000, 40, 230},
    {TEXT_1_C4, 3000, 5000, 40, 230},
    {TEXT_1_C5, 4000, 6000, 40, 230},
    {TEXT_1_C1, 7000, 9000, 40, 230},
    {TEXT_1_C2, 8000, 10000, 40, 230},
    {TEXT_1_C3, 9000, 11000, 40, 230},
    {TEXT_1_C4, 10000, 12000, 40, 230},
    {TEXT_1_C5, 11000, 13000, 40, 230},
};
uint16_t sequence_1_C_size = sizeof(sequence_1_C) / sizeof(sequence_1_C[0]);

sequence_commands_st sequence_1_D[] = {
    {TEXT_1_D1, 0,    2000, 40, 230},
    {TEXT_1_D2, 1000, 4000, 40, 230},
    {TEXT_1_D3, 3000, 5000, 40, 230},
    {TEXT_1_D4, 4000, 6000, 40, 230},
    {TEXT_1_D5, 5000, 8000, 40, 230},
    {TEXT_1_D6, 7000, 9000, 40, 230},
    {TEXT_1_D7, 8000, 10000, 40, 230},
    {TEXT_1_D8, 9000, 11000, 40, 230},
};
uint16_t sequence_1_D_size = sizeof(sequence_1_D) / sizeof(sequence_1_D[0]);

sequence_commands_st sequence_1_P[] = {
    {TEXT_1_P1, 0,    250,  40, 230},
    {TEXT_1_P2, 250,  500,  40, 230},
    {TEXT_1_P1, 500,  750,  40, 230},
    {TEXT_1_P2, 750,  1000, 40, 230},

    {TEXT_1_P1, 1000, 1250, 40, 230},
    {TEXT_1_P2, 1250, 1500, 40, 230},
    {TEXT_1_P1, 1500, 1750, 40, 230},
    {TEXT_1_P2, 1750, 2000, 40, 230},

    {TEXT_1_P1, 2000, 2250, 40, 230},
    {TEXT_1_P2, 2250, 2500, 40, 230},
    {TEXT_1_P1, 2500, 2750, 40, 230},
    {TEXT_1_P2, 2750, 3000, 40, 230},

    {TEXT_1_P1, 3000, 3250, 40, 230},
    {TEXT_1_P2, 3250, 3500, 40, 230},
    {TEXT_1_P1, 3500, 3750, 40, 230},
    {TEXT_1_P2, 3750, 4000, 40, 230},
};
uint16_t sequence_1_P_size = sizeof(sequence_1_P) / sizeof(sequence_1_P[0]);


/************** SEQUENCE 2 **************/
sequence_commands_st sequence_2_A[] = {
    {TEXT_2_A1, 0,    2000,  40, 230},
    {TEXT_2_A2, 1000, 3000,  40, 230},
    {TEXT_2_A3, 2000, 4000,  40, 230},
    {TEXT_2_A4, 3000, 5000,  40, 230},
    {TEXT_2_A5, 4000, 6000,  40, 230},
    {TEXT_2_A6, 6000, 8000,  40, 230},
    {TEXT_2_A7, 7000, 9000,  40, 230},
    {TEXT_2_A8, 8000, 10000, 40, 230},
    {TEXT_2_A9, 9000, 11000, 40, 230},
};
uint16_t sequence_2_A_size = sizeof(sequence_2_A) / sizeof(sequence_2_A[0]);

sequence_commands_st sequence_2_B[] = {
    {TEXT_2_B1, 0,    2000,  40, 230},
    {TEXT_2_B2, 1000, 3000,  40, 230},
    {TEXT_2_B3, 2000, 4000,  40, 230},
    {TEXT_2_B4, 3000, 5000,  40, 230},
    {TEXT_2_B5, 4000, 6000,  40, 230},
    {TEXT_2_B6, 6000, 8000,  40, 230},
    {TEXT_2_B7, 7000, 9000,  40, 230},
    {TEXT_2_B8, 8000, 10000, 40, 230},
    {TEXT_2_B9, 9000, 11000, 40, 230},
};
uint16_t sequence_2_B_size = sizeof(sequence_2_B) / sizeof(sequence_2_B[0]);

sequence_commands_st sequence_2_C[] = {
    {TEXT_2_C1, 0,     500, 40, 230},
    {TEXT_2_C2,  500, 1000, 40, 230},
    {TEXT_2_C3, 1000, 1500, 40, 230},
    {TEXT_2_C4, 2000, 2500, 40, 230},

    {TEXT_2_C1, 3000, 3500, 40, 230},
    {TEXT_2_C2, 3500, 4000, 40, 230},
    {TEXT_2_C3, 4000, 4500, 40, 230},
    {TEXT_2_C4, 4500, 5000, 40, 230},
};
uint16_t sequence_2_C_size = sizeof(sequence_2_C) / sizeof(sequence_2_C[0]);

sequence_commands_st sequence_2_D[] = {
    {TEXT_2_D1, 0,     500, 40, 230},
    {TEXT_2_D2, 1000, 1500, 40, 230},
    {TEXT_2_D3, 1500, 2000, 40, 230},
    {TEXT_2_D4, 2500, 3000, 40, 230},

    {TEXT_2_D1, 3000, 3500, 40, 230},
    {TEXT_2_D2, 4000, 4500, 40, 230},
    {TEXT_2_D3, 4500, 5000, 40, 230},
    {TEXT_2_D4, 5500, 6000, 40, 230},
};
uint16_t sequence_2_D_size = sizeof(sequence_2_D) / sizeof(sequence_2_D[0]);

sequence_commands_st sequence_2_E[] = {
    {TEXT_2_E1,     0,  2000, 40, 230},
    {TEXT_2_E2,  1000,  3000, 40, 230},
    {TEXT_2_E3,  2000,  4000, 40, 230},
    {TEXT_2_E4,  3000,  5000, 40, 230},
    {TEXT_2_E5,  4000,  6000, 40, 230},
    {TEXT_2_E1,  6000,  7000, 40, 230},
    {TEXT_2_E2,  6500,  7500, 40, 230},
    {TEXT_2_E3,  7000,  8000, 40, 230},
    {TEXT_2_E4,  8000,  9000, 40, 230},
    {TEXT_2_E5,  9000, 10000, 40, 230},
    {TEXT_2_E1, 10000, 10500, 40, 230},
    {TEXT_2_E2, 10500, 11000, 40, 230},
    {TEXT_2_E3, 10000, 11500, 40, 230},
    {TEXT_2_E4, 11500, 12000, 40, 230}, // Yeah this is correct
    {TEXT_2_E5, 12000, 12500, 40, 230}, // Yeah this is correct

};
uint16_t sequence_2_E_size = sizeof(sequence_2_E) / sizeof(sequence_2_E[0]);

sequence_commands_st sequence_2_P[] = {
    {TEXT_2_P1, 0,     250, 40, 230},
    {TEXT_2_P1, 500,   750, 40, 230},
    {TEXT_2_P2, 1000, 1250, 40, 230},
    {TEXT_2_P2, 1500, 1750, 40, 230},
    {TEXT_2_P3, 2000, 2250, 40, 230},
    {TEXT_2_P3, 2500, 2750, 40, 230},
    {TEXT_2_P4, 3000, 3250, 40, 230},
    {TEXT_2_P4, 3500, 3750, 40, 230},

    {TEXT_2_P1, 4000, 4250, 40, 230},
    {TEXT_2_P1, 4500, 4750, 40, 230},
    {TEXT_2_P2, 5000, 5250, 40, 230},
    {TEXT_2_P2, 5500, 5750, 40, 230},
    {TEXT_2_P3, 6000, 6250, 40, 230},
    {TEXT_2_P3, 6500, 6750, 40, 230},
    {TEXT_2_P4, 7000, 7250, 40, 230},
    {TEXT_2_P4, 7500, 7750, 40, 230},
};
uint16_t sequence_2_P_size = sizeof(sequence_2_P) / sizeof(sequence_2_P[0]);


/************** SEQUENCE 3 **************/
sequence_commands_st sequence_3_A[] = {
    {TEXT_3_A1,  0,    1000, 40, 230},
    {TEXT_3_A1,  1500, 2000, 40, 230},
    {TEXT_3_A2,  2000, 3000, 40, 230},
    {TEXT_3_A2,  3500, 4000, 40, 230},
    {TEXT_3_A3,  4000, 5000, 40, 230},
    {TEXT_3_A3,  5500, 6000, 40, 230},
    {TEXT_3_A4,  6000, 7000, 40, 230},
    {TEXT_3_A4,  7500, 8000, 40, 230},
    {TEXT_3_A5,  8000, 9000, 40, 230},
    {TEXT_3_A5,  9500, 10000, 40, 230},
    {TEXT_3_A6,  10000, 11000, 40, 230},
    {TEXT_3_A6,  11500, 12000, 40, 230},
    {TEXT_3_A7,  12000, 13000, 40, 230},
    {TEXT_3_A7,  13500, 14000, 40, 230},
    {TEXT_3_A8,  14000, 15000, 40, 230},
    {TEXT_3_A8,  15500, 16000, 40, 230},
    {TEXT_3_A9,  16000, 17000, 40, 230},
    {TEXT_3_A9,  17500, 18000, 40, 230},
    {TEXT_3_A10, 18000, 19000, 40, 230},
    {TEXT_3_A10, 19500, 20000, 40, 230},
    {TEXT_3_A11, 20000, 21000, 40, 230},
    {TEXT_3_A11, 21500, 22000, 40, 230},
    {TEXT_3_A12, 22000, 23000, 40, 230},
    {TEXT_3_A12, 23500, 24000, 40, 230},
};
uint16_t sequence_3_A_size = sizeof(sequence_3_A) / sizeof(sequence_3_A[0]);

sequence_commands_st sequence_3_B[] = {
    {TEXT_3_B1, 0,     2000,  40, 230},
    {TEXT_3_B1, 2500,  3000,  40, 230},
    {TEXT_3_B1, 3500,  4000,  40, 230},
    {TEXT_3_B1, 4000,  5000,  40, 230},
    {TEXT_3_B2, 6000,  7000,  40, 230},
    {TEXT_3_B2, 7500,  8000,  40, 230},
    {TEXT_3_B2, 8500,  9000,  40, 230},
    {TEXT_3_B2, 9500,  10000, 40, 230},
    {TEXT_3_B3, 10000, 12000, 40, 230},
    {TEXT_3_B3, 12500, 13000, 40, 230},
    {TEXT_3_B3, 13500, 14000, 40, 230},
    {TEXT_3_B3, 14500, 15000, 40, 230},
};
uint16_t sequence_3_B_size = sizeof(sequence_3_B) / sizeof(sequence_3_B[0]);

sequence_commands_st sequence_3_C[] = {
    {TEXT_3_C1,     0,  500, 40, 230},
    {TEXT_3_C2,   500, 1000, 40, 230},
    {TEXT_3_C3,  1000, 1500, 40, 230},
    {TEXT_3_C4,  1500, 2000, 40, 230},
    {TEXT_3_C5,  2000, 2500, 40, 230},
    {TEXT_3_C6,  2500, 3000, 40, 230},
    {TEXT_3_C7,  3000, 3500, 40, 230},
    {TEXT_3_C8,  3500, 4000, 40, 230},
    {TEXT_3_C9,  4000, 4500, 40, 230},
    {TEXT_3_C10, 4500, 5000, 40, 230},
    {TEXT_3_C11, 5000, 5500, 40, 230},
    {TEXT_3_C12, 5500, 6000, 40, 230},
};
uint16_t sequence_3_C_size = sizeof(sequence_3_C) / sizeof(sequence_3_C[0]);

sequence_commands_st sequence_3_D[] = {
    {TEXT_3_D1, 0,    1500, 40, 230},
    {TEXT_3_D2, 1500, 3000, 40, 230},
    {TEXT_3_D3, 3000, 4500, 40, 230},

    {TEXT_3_D1, 5000, 5500, 40, 230},
    {TEXT_3_D2, 5500, 6000, 40, 230},
    {TEXT_3_D3, 6000, 6500, 40, 230},

    {TEXT_3_D1, 7000, 7500, 40, 230},
    {TEXT_3_D2, 7500, 8000, 40, 230},
    {TEXT_3_D3, 8000, 8500, 40, 230},

    {TEXT_3_D1, 9500,  10000, 40, 230},
    {TEXT_3_D1, 10500, 11000, 40, 230},
    {TEXT_3_D1, 11500, 12000, 40, 230},
    {TEXT_3_D2, 11000, 11500, 40, 230},
    {TEXT_3_D2, 12000, 12500, 40, 230},
    {TEXT_3_D2, 13000, 13500, 40, 230},
    {TEXT_3_D3, 12500, 13000, 40, 230},
    {TEXT_3_D3, 13500, 14000, 40, 230},
    {TEXT_3_D3, 14500, 15000, 40, 230},
};
uint16_t sequence_3_D_size = sizeof(sequence_3_D) / sizeof(sequence_3_D[0]);

sequence_commands_st sequence_3_P[] = {
    {TEXT_3_P1,    0,  250, 40, 230},
    {TEXT_3_P1,  500,  750, 40, 230},
    {TEXT_3_P2, 1000, 1250, 40, 230},
    {TEXT_3_P2, 1500, 1750, 40, 230},
    {TEXT_3_P3, 2000, 2250, 40, 230},
    {TEXT_3_P3, 2500, 2750, 40, 230},
    {TEXT_3_P4, 3000, 3250, 40, 230},
    {TEXT_3_P4, 3500, 3750, 40, 230},
    {TEXT_3_P5, 4000, 4250, 40, 230},
    {TEXT_3_P5, 4500, 4750, 40, 230},
    {TEXT_3_P6, 5000, 5250, 40, 230},
    {TEXT_3_P6, 5500, 5750, 40, 230},
};
uint16_t sequence_3_P_size = sizeof(sequence_3_P) / sizeof(sequence_3_P[0]);


/************** SEQUENCE 4 **************/
sequence_commands_st sequence_4_A[] = {
    {TEXT_4_A1,    0, 1000, 40, 230},
    {TEXT_4_A2,  500, 1500, 40, 230},
    {TEXT_4_A3, 1000, 2000, 40, 230},
    {TEXT_4_A4, 1500, 2500, 40, 230},
    {TEXT_4_A5, 2000, 3000, 40, 230},
    {TEXT_4_A6, 2500, 3500, 40, 230},
    {TEXT_4_A7, 3000, 4000, 40, 230},

    {TEXT_4_A1, 3500, 4500, 40, 230},
    {TEXT_4_A2, 4000, 5000, 40, 230},
    {TEXT_4_A3, 4500, 5500, 40, 230},
    {TEXT_4_A4, 5000, 6000, 40, 230},
    {TEXT_4_A5, 5500, 6500, 40, 230},
    {TEXT_4_A6, 6000, 7000, 40, 230},
    {TEXT_4_A7, 6500, 7500, 40, 230},
};
uint16_t sequence_4_A_size = sizeof(sequence_4_A) / sizeof(sequence_4_A[0]);

sequence_commands_st sequence_4_B[] = {
    {TEXT_4_B1,    0,   500,  40, 230},
    {TEXT_4_B2, 1000,  1500,  40, 230},
    {TEXT_4_B3, 2000,  2500,  40, 230},
    {TEXT_4_B4, 3000,  3500,  40, 230},

    {TEXT_4_B1, 4000,  4500,  40, 230},
    {TEXT_4_B2, 5000,  5500,  40, 230},
    {TEXT_4_B3, 6000,  6500,  40, 230},
    {TEXT_4_B4, 7000,  7500,  40, 230},

    {TEXT_4_B1, 8000,  8500,  40, 230},
    {TEXT_4_B2, 9000,  9500,  40, 230},
    {TEXT_4_B3, 10000, 10500, 40, 230},
    {TEXT_4_B4, 11000, 11500, 40, 230},
};
uint16_t sequence_4_B_size = sizeof(sequence_4_B) / sizeof(sequence_4_B[0]);

sequence_commands_st sequence_4_C[] = {
    {TEXT_4_C1,     0,  1000, 40, 230},
    {TEXT_4_C1,  1500,  3000, 40, 230},
    {TEXT_4_C2,  2000,  3000, 40, 230},
    {TEXT_4_C2,  3500,  5000, 40, 230},
    {TEXT_4_C3,  4000,  5000, 40, 230},
    {TEXT_4_C3,  5500,  7000, 40, 230},
    {TEXT_4_C4,  6000,  7000, 40, 230},
    {TEXT_4_C4,  7500,  9000, 40, 230},
    {TEXT_4_C5,  8000,  9000, 40, 230},
    {TEXT_4_C5,  9500, 11000, 40, 230},
    {TEXT_4_C6, 10000, 11000, 40, 230},
    {TEXT_4_C6, 11500, 13000, 40, 230},
    {TEXT_4_C7, 12000, 13000, 40, 230},
    {TEXT_4_C7, 13500, 15000, 40, 230},
};
uint16_t sequence_4_C_size = sizeof(sequence_4_C) / sizeof(sequence_4_C[0]);

sequence_commands_st sequence_4_D[] = {
    {TEXT_4_D1,     0,   500, 40, 230},
    {TEXT_4_D1,  1000,  1500, 40, 230},
    {TEXT_4_D1,  2000,  2500, 40, 230},
    {TEXT_4_D2,  2500,  3000, 40, 230},
    {TEXT_4_D2,  3500,  4000, 40, 230},
    {TEXT_4_D2,  4500,  5000, 40, 230},
    {TEXT_4_D4,  3000,  3500, 40, 230},
    {TEXT_4_D4,  4000,  4500, 40, 230},
    {TEXT_4_D4,  5000,  5500, 40, 230},
    {TEXT_4_D3,  5500,  6000, 40, 230},
    {TEXT_4_D3,  6500,  7000, 40, 230},
    {TEXT_4_D3,  7500,  8000, 40, 230},
    {TEXT_4_D2,  8000,  8500, 40, 230},
    {TEXT_4_D2,  9500, 10000, 40, 230},
    {TEXT_4_D2, 11000, 11500, 40, 230},
    {TEXT_4_D4,  8500,  9000, 40, 230},
    {TEXT_4_D4, 10000, 10500, 40, 230},
    {TEXT_4_D4, 11500, 12000, 40, 230},
    {TEXT_4_D6,  9000,  9500, 40, 230},
    {TEXT_4_D6, 10500, 11000, 40, 230},
    {TEXT_4_D6, 12000, 12500, 40, 230},
    {TEXT_4_D5, 12500, 13000, 40, 230},
    {TEXT_4_D5, 13500, 14000, 40, 230},
    {TEXT_4_D5, 14500, 15000, 40, 230},
    {TEXT_4_D2, 15000, 15500, 40, 230},
    {TEXT_4_D2, 16500, 17000, 40, 230},
    {TEXT_4_D2, 18000, 18500, 40, 230},
    {TEXT_4_D4, 15500, 16000, 40, 230},
    {TEXT_4_D4, 17000, 17500, 40, 230},
    {TEXT_4_D4, 18500, 19000, 40, 230},
    {TEXT_4_D6, 16000, 16500, 40, 230},
    {TEXT_4_D6, 17500, 18000, 40, 230},
    {TEXT_4_D6, 19000, 19500, 40, 230},
    {TEXT_4_D7, 19500, 20000, 40, 230},
    {TEXT_4_D7, 20500, 21000, 40, 230},
    {TEXT_4_D7, 21500, 22000, 40, 230},
};
uint16_t sequence_4_D_size = sizeof(sequence_4_D) / sizeof(sequence_4_D[0]);

sequence_commands_st sequence_4_E[] = {
    {TEXT_4_E1,  0,     1000, 40, 230},
    {TEXT_4_E2,  1000,  2000, 40, 230},
    {TEXT_4_E3,  2000,  2500, 40, 230},
    {TEXT_4_E3,  3000,  3500, 40, 230},
    {TEXT_4_E4,  3500,  4500, 40, 230},
    {TEXT_4_E5,  4500,  5500, 40, 230},
    {TEXT_4_E6,  5500,  6000, 40, 230},
    {TEXT_4_E6,  6500,  7000, 40, 230},
    {TEXT_4_E7,  7000,  8000, 40, 230},
    {TEXT_4_E8,  8000,  9000, 40, 230},
    {TEXT_4_E9,  9000,  9500, 40, 230},
    {TEXT_4_E9, 10000, 10500, 40, 230},
};
uint16_t sequence_4_E_size = sizeof(sequence_4_E) / sizeof(sequence_4_E[0]);

sequence_commands_st sequence_4_P[] = {
    {TEXT_4_P1,    0,  250, 40, 230},
    {TEXT_4_P1,  500,  750, 40, 230},
    {TEXT_4_P2, 1000, 1250, 40, 230},
    {TEXT_4_P2, 1500, 1750, 40, 230},
    {TEXT_4_P3, 2000, 2250, 40, 230},
    {TEXT_4_P3, 2500, 2750, 40, 230},

    {TEXT_4_P1, 3000, 3250, 40, 230},
    {TEXT_4_P1, 3500, 3750, 40, 230},
    {TEXT_4_P2, 4000, 4250, 40, 230},
    {TEXT_4_P2, 4500, 4750, 40, 230},
    {TEXT_4_P3, 5000, 5250, 40, 230},
    {TEXT_4_P3, 5500, 5750, 40, 230},

    {TEXT_4_P1, 6000, 6250, 40, 230},
    {TEXT_4_P1, 6500, 6750, 40, 230},
    {TEXT_4_P2, 7000, 7250, 40, 230},
    {TEXT_4_P2, 7500, 7750, 40, 230},
    {TEXT_4_P3, 8000, 8250, 40, 230},
    {TEXT_4_P3, 8500, 8750, 40, 230},
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

void update_sequence(uint32_t current_sequence);

static bool inline debounce(debounce_timer_st* timer, bool condition) {
    uint32_t millis_timer = millis();
    if (condition) {
        if (!timer->condition_met) {
            timer->start_time = millis_timer;
            timer->condition_met = true;
        }
        
        if (millis_timer - timer->start_time >= timer->debounce_time) {
            return true;
        }
    } else {
        timer->condition_met = false;
        timer->start_time = 0;
    }
    
    return false;
}

void clear_debounce(debounce_timer_st* timer) {
    timer->start_time = 0;
    timer->condition_met = false;
}

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

void sequence_init(){
    update_sequence(IDLE);
}

void print_debug_datatable_data(void){
    // printf("Textile Presence.       1: %d, 2: %d, 3: %d, 4: %d\n", textile_1_presence,           textile_2_presence,          textile_3_presence,          textile_4_presence);
    printf("State:  1: %d, 2: %d, 3: %d, 4: %d\n", textile_1_current_sequence,   textile_2_current_sequence,  textile_3_current_sequence,  textile_4_current_sequence);
    // printf("Textile previous state. 1: %d, 2: %d, 3: %d, 4: %d\n", textile_1_previous_sequence,  textile_2_previous_sequence, textile_3_previous_sequence, textile_4_previous_sequence);
    // printf("Textile Trigger Type.   1: %d, 2: %d, 3: %d, 4: %d\n", textile_1_trigger_type,       textile_2_trigger_type,      textile_3_trigger_type,      textile_4_trigger_type);
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
    write_data_table(current_sequence_tag, &tmp_var, sizeof(tmp_var));
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

void loop_sequence(uint8_t debug_info){

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
    const unsigned long TIMEOUT_VALUE  = 12000UL; // 90 sec
 
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

    // Initialize debounce timers
    static debounce_timer_st textile_1_single_debounce      = {0, false, DEBOUNCE_TIMER};
    static debounce_timer_st textile_1_double_debounce      = {0, false, DEBOUNCE_TIMER};
    static debounce_timer_st textile_1_triple_debounce      = {0, false, DEBOUNCE_TIMER};
    static debounce_timer_st textile_1_four_debounce        = {0, false, DEBOUNCE_TIMER};
    static debounce_timer_st textile_1_from2_debounce       = {0, false, DEBOUNCE_TIMER};
    static debounce_timer_st textile_1_from3_debounce       = {0, false, DEBOUNCE_TIMER};
    static debounce_timer_st textile_1_from4_debounce       = {0, false, DEBOUNCE_TIMER};
    static debounce_timer_st textile_1_from_triple_debounce = {0, false, DEBOUNCE_TIMER};
    static debounce_timer_st textile_1_finished_debounce    = {0, false, DEBOUNCE_TIMER};

    bool all_idle = (textile_1_current_sequence == IDLE &&
                     textile_2_current_sequence == IDLE &&
                     textile_3_current_sequence == IDLE &&
                     textile_4_current_sequence == IDLE);

    uint8_t active_count = (textile_1_presence != 0) + (textile_2_presence != 0) +
                           (textile_3_presence != 0) + (textile_4_presence != 0);

    // Single: only device 1 present → 1D
    bool sequence_1_D_single = (textile_1_presence != 0);
    if (debounce(&textile_1_single_debounce, sequence_1_D_single)) {
        printf("Sequence 1: Single detected\n");
        update_trigger(SINGLE);
        update_sequence(SEQUENCE_D);
        clear_debounce(&textile_1_single_debounce);
    }

    // Double: device 1 + one other → 1D (device 1 always primary)
    bool sequence_1_double_cond = (all_idle && textile_1_presence != 0 && active_count == 2);
    if (debounce(&textile_1_double_debounce, sequence_1_double_cond)) {
        update_trigger(DOUBLE);
        update_sequence(SEQUENCE_D);
        clear_debounce(&textile_1_double_debounce);
    }

    // Triple: device 1 + two others → 1P immediately
    bool sequence_1_triple_cond = (all_idle && textile_1_presence != 0 && active_count == 3);
    if (debounce(&textile_1_triple_debounce, sequence_1_triple_cond)) {
        update_trigger(TRIPLE);
        update_sequence(SEQUENCE_P);
        clear_debounce(&textile_1_triple_debounce);
    }

    // Four: all devices → 1P immediately
    bool sequence_1_four_cond = (all_idle && textile_1_presence != 0 && active_count == 4);
    if (debounce(&textile_1_four_debounce, sequence_1_four_cond)) {
        update_trigger(FOUR);
        update_sequence(SEQUENCE_P);
        clear_debounce(&textile_1_four_debounce);
    }

    // From textile 2 chain: 2B → 3C → 4D → 1C
    bool sequence_1_from2_cond = (textile_1_current_sequence == IDLE &&
                                  textile_2_current_sequence == FINISHED &&
                                  textile_3_current_sequence == FINISHED &&
                                  textile_4_current_sequence == FINISHED &&
                                  (textile_2_trigger_type == SINGLE ||
                                   (textile_2_trigger_type == DOUBLE && textile_1_trigger_type != DOUBLE)));
    if (debounce(&textile_1_from2_debounce, sequence_1_from2_cond)) {
        update_sequence(SEQUENCE_C);
        clear_debounce(&textile_1_from2_debounce);
    }

    // From textile 3 chain: 3A → 1A
    bool sequence_1_from3_cond = (textile_1_current_sequence == IDLE &&
                                  textile_2_current_sequence == IDLE &&
                                  textile_3_current_sequence == FINISHED &&
                                  textile_4_current_sequence == IDLE &&
                                  (textile_3_trigger_type == SINGLE ||
                                   (textile_3_trigger_type == DOUBLE && textile_1_trigger_type != DOUBLE && textile_2_trigger_type != DOUBLE)));
    if (debounce(&textile_1_from3_debounce, sequence_1_from3_cond)) {
        update_sequence(SEQUENCE_A);
        clear_debounce(&textile_1_from3_debounce);
    }

    // From textile 4 chain: 4C → 3D → 2D → 1B
    bool sequence_1_from4_cond = (textile_1_current_sequence == IDLE &&
                                  textile_2_current_sequence == FINISHED &&
                                  textile_3_current_sequence == FINISHED &&
                                  textile_4_current_sequence == FINISHED &&
                                  (textile_4_trigger_type == SINGLE ||
                                   (textile_4_trigger_type == DOUBLE && textile_1_trigger_type != DOUBLE &&
                                    textile_2_trigger_type != DOUBLE && textile_3_trigger_type != DOUBLE)));
    if (debounce(&textile_1_from4_debounce, sequence_1_from4_cond)) {
        update_sequence(SEQUENCE_B);
        clear_debounce(&textile_1_from4_debounce);
    }

    // From triple (2+3+4): device 1 is the 4th, plays P after others finish
    bool sequence_1_from_triple_cond = (textile_1_current_sequence == IDLE &&
                                        textile_2_current_sequence == FINISHED &&
                                        textile_3_current_sequence == FINISHED &&
                                        textile_4_current_sequence == FINISHED &&
                                        textile_2_trigger_type == TRIPLE &&
                                        textile_3_trigger_type == TRIPLE &&
                                        textile_4_trigger_type == TRIPLE);
    if (debounce(&textile_1_from_triple_debounce, sequence_1_from_triple_cond)) {
        update_sequence(SEQUENCE_P);
        clear_debounce(&textile_1_from_triple_debounce);
    }

    // All finished → reset (device 1 is never DOUBLE secondary)
    bool sequence_1_all_finished = (textile_1_current_sequence == FINISHED &&
                                    textile_2_current_sequence == FINISHED &&
                                    textile_3_current_sequence == FINISHED &&
                                    textile_4_current_sequence == FINISHED);
    if (debounce(&textile_1_finished_debounce, sequence_1_all_finished)) {
        update_trigger(NOT_TRIGGERED);
        update_sequence(IDLE);
        clear_debounce(&textile_1_finished_debounce);
    }

    if( textile_1_current_sequence == SEQUENCE_A ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        uint8_t is_44_on = false;
        uint8_t is_45_on = false;
        for (uint16_t i = 0 ; i < sequence_1_A_size ; i++)
        {
            if((current_timer > sequence_1_A[i].time_starts_at) &&
               (current_timer < sequence_1_A[i].time_ends_at))
            {
                if (sequence_1_A[i].led_index.positive_pin == IDX_D44)
                    is_44_on = true;
                if (sequence_1_A[i].led_index.positive_pin == IDX_D45)
                    is_45_on = true;
            }
        }
        if(!is_44_on)
            setPin(IDX_D44, 0);
        else
            setPin(IDX_D44, 255);

        if(!is_45_on)
            setPin(IDX_D45, 0);
        else
            setPin(IDX_D45, 255);
        for (uint16_t i = 0 ; i < sequence_1_A_size ; i++)
        {
            if((current_timer > sequence_1_A[i].time_starts_at) &&
               (current_timer < sequence_1_A[i].time_ends_at))
            {
                uint32_t duration = (sequence_1_A[i].time_ends_at - sequence_1_A[i].time_starts_at);
                uint32_t middle_point = (sequence_1_A[i].time_starts_at + (duration/2));
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_1_A[i].value_starts_at, sequence_1_A[i].value_ends_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_1_A[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_1_A[i].value_ends_at, sequence_1_A[i].value_starts_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_1_A[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_1_A[sequence_1_A_size - 1].time_ends_at)
        {
            clearPins();
            update_sequence(FINISHED);
        }
    }

    if( textile_1_current_sequence == SEQUENCE_B ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        uint8_t is_44_on = false;
        uint8_t is_45_on = false;
        for (uint16_t i = 0 ; i < sequence_1_B_size ; i++)
        {
            if((current_timer > sequence_1_B[i].time_starts_at) &&
               (current_timer < sequence_1_B[i].time_ends_at))
            {
                if (sequence_1_B[i].led_index.positive_pin == IDX_D44)
                    is_44_on = true;
                if (sequence_1_B[i].led_index.positive_pin == IDX_D45)
                    is_45_on = true;
            }
        }
        if(!is_44_on)
            setPin(IDX_D44, 0);
        else
            setPin(IDX_D44, 255);

        if(!is_45_on)
            setPin(IDX_D45, 0);
        else
            setPin(IDX_D45, 255);
        for (uint16_t i = 0 ; i < sequence_1_B_size ; i++)
        {
            if((current_timer > sequence_1_B[i].time_starts_at) &&
               (current_timer < sequence_1_B[i].time_ends_at))
            {
                uint32_t duration = (sequence_1_B[i].time_ends_at - sequence_1_B[i].time_starts_at);
                uint32_t middle_point = (sequence_1_B[i].time_starts_at + (duration/2));
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_1_B[i].value_starts_at, sequence_1_B[i].value_ends_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_1_B[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point; 
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_1_B[i].value_ends_at, sequence_1_B[i].value_starts_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_1_B[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_1_B[sequence_1_B_size - 1].time_ends_at)
        {
            clearPins();
            update_sequence(FINISHED);
        }
    }

    if( textile_1_current_sequence == SEQUENCE_C ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        uint8_t is_44_on = false;
        uint8_t is_45_on = false;
        for (uint16_t i = 0 ; i < sequence_1_C_size ; i++)
        {
            if((current_timer > sequence_1_C[i].time_starts_at) &&
               (current_timer < sequence_1_C[i].time_ends_at))
            {
                if (sequence_1_C[i].led_index.positive_pin == IDX_D44)
                    is_44_on = true;
                if (sequence_1_C[i].led_index.positive_pin == IDX_D45)
                    is_45_on = true;
            }
        }
        if(!is_44_on)
            setPin(IDX_D44, 0);
        else
            setPin(IDX_D44, 255);

        if(!is_45_on)
            setPin(IDX_D45, 0);
        else
            setPin(IDX_D45, 255);
        for (uint16_t i = 0 ; i < sequence_1_C_size ; i++)
        {
            if((current_timer > sequence_1_C[i].time_starts_at) &&
               (current_timer < sequence_1_C[i].time_ends_at))
            {
                uint32_t duration = (sequence_1_C[i].time_ends_at - sequence_1_C[i].time_starts_at);
                uint32_t middle_point = (sequence_1_C[i].time_starts_at + (duration/2));
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer; 
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_1_C[i].value_starts_at, sequence_1_C[i].value_ends_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_1_C[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_1_C[i].value_ends_at, sequence_1_C[i].value_starts_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_1_C[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_1_C[sequence_1_C_size-1].time_ends_at)
        {
            clearPins();
            update_sequence(FINISHED);
        }
    }

    if( textile_1_current_sequence == SEQUENCE_D ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        uint8_t is_44_on = false;
        uint8_t is_45_on = false;
        for (uint16_t i = 0 ; i < sequence_1_D_size ; i++)
        {
            if((current_timer > sequence_1_D[i].time_starts_at) &&
               (current_timer < sequence_1_D[i].time_ends_at))
            {
                if (sequence_1_D[i].led_index.positive_pin == IDX_D44)
                    is_44_on = true;
                if (sequence_1_D[i].led_index.positive_pin == IDX_D45)
                    is_45_on = true;
            }
        }
        if(!is_44_on)
            setPin(IDX_D44, 0);
        else
            setPin(IDX_D44, 255);

        if(!is_45_on)
            setPin(IDX_D45, 0);
        else
            setPin(IDX_D45, 255);
        for (uint16_t i = 0 ; i < sequence_1_D_size ; i++)
        {
            if((current_timer > sequence_1_D[i].time_starts_at) &&
               (current_timer < sequence_1_D[i].time_ends_at))
            {
                uint32_t duration = (sequence_1_D[i].time_ends_at - sequence_1_D[i].time_starts_at);
                uint32_t middle_point = (sequence_1_D[i].time_starts_at + (duration/2));
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_1_D[i].value_starts_at, sequence_1_D[i].value_ends_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_1_D[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_1_D[i].value_ends_at, sequence_1_D[i].value_starts_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_1_D[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_1_D[sequence_1_D_size - 1].time_ends_at)
        {
            clearPins();
            update_sequence(FINISHED);
        }
    }

    if( textile_1_current_sequence == SEQUENCE_P ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        for (uint16_t i = 0 ; i < sequence_1_P_size ; i++)
        {
            if((current_timer > sequence_1_P[i].time_starts_at) &&
               (current_timer < sequence_1_P[i].time_ends_at))
            {
                uint32_t duration = (sequence_1_P[i].time_ends_at - sequence_1_P[i].time_starts_at);
                uint32_t middle_point = (sequence_1_P[i].time_starts_at + (duration/2));
                if (sequence_1_P[i].led_index.positive_pin == IDX_D44)
                    setPin(IDX_D45, 0);
                if (sequence_1_P[i].led_index.positive_pin == IDX_D45)
                    setPin(IDX_D44, 0);
                setPin(sequence_1_P[i].led_index.positive_pin, 255);
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_1_P[i].value_starts_at, sequence_1_P[i].value_ends_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_1_P[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_1_P[i].value_ends_at, sequence_1_P[i].value_starts_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_1_P[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_1_P[sequence_1_P_size - 1].time_ends_at)
        {
            clearPins();
            if (textile_1_trigger_type == DOUBLE) {
                update_trigger(NOT_TRIGGERED);
                update_sequence(IDLE);
            } else {
                update_sequence(FINISHED);
            }
        }
    }

}


void textile_2_loop(void){
    // Initialize debounce timers
    static debounce_timer_st textile_2_single_debounce      = {0, false, DEBOUNCE_TIMER};
    static debounce_timer_st textile_2_double_pri_debounce   = {0, false, DEBOUNCE_TIMER};
    static debounce_timer_st textile_2_double_sec_debounce   = {0, false, DEBOUNCE_TIMER};
    static debounce_timer_st textile_2_triple_debounce      = {0, false, DEBOUNCE_TIMER};
    static debounce_timer_st textile_2_four_debounce        = {0, false, DEBOUNCE_TIMER};
    static debounce_timer_st textile_2_from1_debounce       = {0, false, DEBOUNCE_TIMER};
    static debounce_timer_st textile_2_from3_debounce       = {0, false, DEBOUNCE_TIMER};
    static debounce_timer_st textile_2_from4_debounce       = {0, false, DEBOUNCE_TIMER};
    static debounce_timer_st textile_2_from_triple_debounce = {0, false, DEBOUNCE_TIMER};
    static debounce_timer_st textile_2_finished_debounce    = {0, false, DEBOUNCE_TIMER};

    bool all_idle = (textile_1_current_sequence == IDLE &&
                     textile_2_current_sequence == IDLE &&
                     textile_3_current_sequence == IDLE &&
                     textile_4_current_sequence == IDLE);

    uint8_t active_count = (textile_1_presence != 0) + (textile_2_presence != 0) +
                           (textile_3_presence != 0) + (textile_4_presence != 0);

    // Single: only device 2 present → 2B
    bool sequence_2_single_cond = (all_idle && textile_2_presence != 0 && active_count == 1);
    if (debounce(&textile_2_single_debounce, sequence_2_single_cond)) {
        update_trigger(SINGLE);
        update_sequence(SEQUENCE_B);
        clear_debounce(&textile_2_single_debounce);
    }

    // Double primary: device 2 + higher device only (no device 1) → 2B
    bool sequence_2_double_pri = (all_idle && textile_2_presence != 0 &&
                                  textile_1_presence == 0 && active_count == 2);
    if (debounce(&textile_2_double_pri_debounce, sequence_2_double_pri)) {
        update_trigger(DOUBLE);
        update_sequence(SEQUENCE_B);
        clear_debounce(&textile_2_double_pri_debounce);
    }

    // Double secondary: device 2 + device 1 (lower present) → just set trigger, wait
    bool sequence_2_double_sec = (all_idle && textile_2_presence != 0 &&
                                  textile_1_presence != 0 && active_count == 2);
    if (debounce(&textile_2_double_sec_debounce, sequence_2_double_sec)) {
        update_trigger(DOUBLE);
        clear_debounce(&textile_2_double_sec_debounce);
    }

    // Triple: device 2 + two others → 2P immediately
    bool sequence_2_triple_cond = (all_idle && textile_2_presence != 0 && active_count == 3);
    if (debounce(&textile_2_triple_debounce, sequence_2_triple_cond)) {
        update_trigger(TRIPLE);
        update_sequence(SEQUENCE_P);
        clear_debounce(&textile_2_triple_debounce);
    }

    // Four: all devices → 2P immediately
    bool sequence_2_four_cond = (all_idle && textile_2_presence != 0 && active_count == 4);
    if (debounce(&textile_2_four_debounce, sequence_2_four_cond)) {
        update_trigger(FOUR);
        update_sequence(SEQUENCE_P);
        clear_debounce(&textile_2_four_debounce);
    }

    // From textile 1 chain: 1D → 2A
    bool sequence_2_from1_cond = (textile_1_current_sequence == FINISHED &&
                                  textile_2_current_sequence == IDLE &&
                                  textile_3_current_sequence == IDLE &&
                                  textile_4_current_sequence == IDLE &&
                                  (textile_1_trigger_type == SINGLE || textile_1_trigger_type == DOUBLE));
    if (debounce(&textile_2_from1_debounce, sequence_2_from1_cond)) {
        update_sequence(SEQUENCE_A);
        clear_debounce(&textile_2_from1_debounce);
    }

    // From textile 3 chain: 3A → 1A → 4E → 2E
    bool sequence_2_from3_cond = (textile_1_current_sequence == FINISHED &&
                                  textile_2_current_sequence == IDLE &&
                                  textile_3_current_sequence == FINISHED &&
                                  textile_4_current_sequence == FINISHED &&
                                  (textile_3_trigger_type == SINGLE ||
                                   (textile_3_trigger_type == DOUBLE && textile_1_trigger_type != DOUBLE &&
                                    textile_2_trigger_type != DOUBLE)));
    if (debounce(&textile_2_from3_debounce, sequence_2_from3_cond)) {
        update_sequence(SEQUENCE_E);
        clear_debounce(&textile_2_from3_debounce);
    }

    // From textile 4 chain: 4C → 3D → 2D
    bool sequence_2_from4_cond = (textile_1_current_sequence == IDLE &&
                                  textile_2_current_sequence == IDLE &&
                                  textile_3_current_sequence == FINISHED &&
                                  textile_4_current_sequence == FINISHED &&
                                  (textile_4_trigger_type == SINGLE ||
                                   (textile_4_trigger_type == DOUBLE && textile_1_trigger_type != DOUBLE &&
                                    textile_2_trigger_type != DOUBLE && textile_3_trigger_type != DOUBLE)));
    if (debounce(&textile_2_from4_debounce, sequence_2_from4_cond)) {
        update_sequence(SEQUENCE_D);
        clear_debounce(&textile_2_from4_debounce);
    }

    // From triple (1+3+4): device 2 is the 4th, plays P after others finish
    bool sequence_2_from_triple_cond = (textile_2_current_sequence == IDLE &&
                                        textile_1_current_sequence == FINISHED &&
                                        textile_3_current_sequence == FINISHED &&
                                        textile_4_current_sequence == FINISHED &&
                                        textile_1_trigger_type == TRIPLE &&
                                        textile_3_trigger_type == TRIPLE &&
                                        textile_4_trigger_type == TRIPLE);
    if (debounce(&textile_2_from_triple_debounce, sequence_2_from_triple_cond)) {
        update_sequence(SEQUENCE_P);
        clear_debounce(&textile_2_from_triple_debounce);
    }

    // All finished
    bool sequence_2_all_finished = (textile_1_current_sequence == FINISHED &&
                                    textile_2_current_sequence == FINISHED &&
                                    textile_3_current_sequence == FINISHED &&
                                    textile_4_current_sequence == FINISHED);
    if (debounce(&textile_2_finished_debounce, sequence_2_all_finished)) {
        // Check if device 2 is DOUBLE secondary (paired with device 1)
        if (textile_2_trigger_type == DOUBLE && textile_1_trigger_type == DOUBLE) {
            update_sequence(SEQUENCE_P);
        } else {
            update_trigger(NOT_TRIGGERED);
            update_sequence(IDLE);
        }
        clear_debounce(&textile_2_finished_debounce);
    }

    if( textile_2_current_sequence == SEQUENCE_A ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        for (uint16_t i = 0 ; i < sequence_2_A_size ; i++)
        {
            if((current_timer > sequence_2_A[i].time_starts_at) &&
               (current_timer < sequence_2_A[i].time_ends_at))
            {
                uint32_t duration = (sequence_2_A[i].time_ends_at - sequence_2_A[i].time_starts_at);
                uint32_t middle_point = (sequence_2_A[i].time_starts_at + (duration/2));
                if (sequence_2_A[i].led_index.positive_pin == IDX_D44)
                    setPin(IDX_D45, 0);
                if (sequence_2_A[i].led_index.positive_pin == IDX_D45)
                    setPin(IDX_D44, 0);
                setPin(sequence_2_A[i].led_index.positive_pin, 255);
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_2_A[i].value_starts_at, sequence_2_A[i].value_ends_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_2_A[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_2_A[i].value_ends_at, sequence_2_A[i].value_starts_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_2_A[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_2_A[sequence_2_A_size - 1].time_ends_at)
        {
            clearPins();
            update_sequence(FINISHED);
        }
    }

    if( textile_2_current_sequence == SEQUENCE_B ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        for (uint16_t i = 0 ; i < sequence_2_B_size ; i++)
        {
            if((current_timer > sequence_2_B[i].time_starts_at) &&
               (current_timer < sequence_2_B[i].time_ends_at))
            {
                uint32_t duration = (sequence_2_B[i].time_ends_at - sequence_2_B[i].time_starts_at);
                uint32_t middle_point = (sequence_2_B[i].time_starts_at + (duration/2));
                if (sequence_2_B[i].led_index.positive_pin == IDX_D44)
                    setPin(IDX_D45, 0);
                if (sequence_2_B[i].led_index.positive_pin == IDX_D45)
                    setPin(IDX_D44, 0);
                setPin(sequence_2_B[i].led_index.positive_pin, 255);
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_2_B[i].value_starts_at, sequence_2_B[i].value_ends_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_2_B[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_2_B[i].value_ends_at, sequence_2_B[i].value_starts_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_2_B[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_2_B[sequence_2_B_size - 1].time_ends_at)
        {
            clearPins();
            update_sequence(FINISHED);
        }
    }

    if( textile_2_current_sequence == SEQUENCE_C ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        for (uint16_t i = 0 ; i < sequence_2_C_size ; i++)
        {
            if((current_timer > sequence_2_C[i].time_starts_at) &&
               (current_timer < sequence_2_C[i].time_ends_at))
            {
                uint32_t duration = (sequence_2_C[i].time_ends_at - sequence_2_C[i].time_starts_at);
                uint32_t middle_point = (sequence_2_C[i].time_starts_at + (duration/2));
                if (sequence_2_C[i].led_index.positive_pin == IDX_D44)
                    setPin(IDX_D45, 0);
                if (sequence_2_C[i].led_index.positive_pin == IDX_D45)
                    setPin(IDX_D44, 0);
                setPin(sequence_2_C[i].led_index.positive_pin, 255);
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_2_C[i].value_starts_at, sequence_2_C[i].value_ends_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_2_C[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_2_C[i].value_ends_at, sequence_2_C[i].value_starts_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_2_C[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_2_C[sequence_2_C_size - 1].time_ends_at)
        {
            clearPins();
            update_sequence(FINISHED);
        }
    }

    if( textile_2_current_sequence == SEQUENCE_D ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        for (uint16_t i = 0 ; i < sequence_2_D_size ; i++)
        {
            if((current_timer > sequence_2_D[i].time_starts_at) &&
               (current_timer < sequence_2_D[i].time_ends_at))
            {
                uint32_t duration = (sequence_2_D[i].time_ends_at - sequence_2_D[i].time_starts_at);
                uint32_t middle_point = (sequence_2_D[i].time_starts_at + (duration/2));
                if (sequence_2_D[i].led_index.positive_pin == IDX_D44)
                    setPin(IDX_D45, 0);
                if (sequence_2_D[i].led_index.positive_pin == IDX_D45)
                    setPin(IDX_D44, 0);
                setPin(sequence_2_D[i].led_index.positive_pin, 255);
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_2_D[i].value_starts_at, sequence_2_D[i].value_ends_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_2_D[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_2_D[i].value_ends_at, sequence_2_D[i].value_starts_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_2_D[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_2_D[sequence_2_D_size - 1].time_ends_at)
        {
            clearPins();
            update_sequence(FINISHED);
        }
    }

    if( textile_2_current_sequence == SEQUENCE_E ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        for (uint16_t i = 0 ; i < sequence_2_E_size ; i++)
        {
            if((current_timer > sequence_2_E[i].time_starts_at) &&
               (current_timer < sequence_2_E[i].time_ends_at))
            {
                uint32_t duration = (sequence_2_E[i].time_ends_at - sequence_2_E[i].time_starts_at);
                uint32_t middle_point = (sequence_2_E[i].time_starts_at + (duration/2));
                if (sequence_2_E[i].led_index.positive_pin == IDX_D44)
                    setPin(IDX_D45, 0);
                if (sequence_2_E[i].led_index.positive_pin == IDX_D45)
                    setPin(IDX_D44, 0);
                setPin(sequence_2_E[i].led_index.positive_pin, 255);
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_2_E[i].value_starts_at, sequence_2_E[i].value_ends_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_2_E[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_2_E[i].value_ends_at, sequence_2_E[i].value_starts_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_2_E[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_2_E[sequence_2_E_size - 1].time_ends_at)
        {
            clearPins();
            update_sequence(FINISHED);
        }
    }

    if( textile_2_current_sequence == SEQUENCE_P ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        for (uint16_t i = 0 ; i < sequence_2_P_size ; i++)
        {
            if((current_timer > sequence_2_P[i].time_starts_at) &&
               (current_timer < sequence_2_P[i].time_ends_at))
            {
                uint32_t duration = (sequence_2_P[i].time_ends_at - sequence_2_P[i].time_starts_at);
                uint32_t middle_point = (sequence_2_P[i].time_starts_at + (duration/2));
                if (sequence_2_P[i].led_index.positive_pin == IDX_D44)
                    setPin(IDX_D45, 0);
                if (sequence_2_P[i].led_index.positive_pin == IDX_D45)
                    setPin(IDX_D44, 0);
                setPin(sequence_2_P[i].led_index.positive_pin, 255);
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_2_P[i].value_starts_at, sequence_2_P[i].value_ends_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_2_P[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_2_P[i].value_ends_at, sequence_2_P[i].value_starts_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_2_P[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_2_P[sequence_2_P_size - 1].time_ends_at)
        {
            clearPins();
            if (textile_2_trigger_type == DOUBLE) {
                update_trigger(NOT_TRIGGERED);
                update_sequence(IDLE);
            } else {
                update_sequence(FINISHED);
            }
        }
    }

}


void textile_3_loop(void){
    // Initialize debounce timers
    static debounce_timer_st textile_3_single_debounce      = {0, false, DEBOUNCE_TIMER};
    static debounce_timer_st textile_3_double_pri_debounce   = {0, false, DEBOUNCE_TIMER};
    static debounce_timer_st textile_3_double_sec_debounce   = {0, false, DEBOUNCE_TIMER};
    static debounce_timer_st textile_3_triple_debounce      = {0, false, DEBOUNCE_TIMER};
    static debounce_timer_st textile_3_four_debounce        = {0, false, DEBOUNCE_TIMER};
    static debounce_timer_st textile_3_from1_debounce       = {0, false, DEBOUNCE_TIMER};
    static debounce_timer_st textile_3_from2_debounce       = {0, false, DEBOUNCE_TIMER};
    static debounce_timer_st textile_3_from4_debounce       = {0, false, DEBOUNCE_TIMER};
    static debounce_timer_st textile_3_from_triple_debounce = {0, false, DEBOUNCE_TIMER};
    static debounce_timer_st textile_3_finished_debounce    = {0, false, DEBOUNCE_TIMER};

    bool all_idle = (textile_1_current_sequence == IDLE &&
                     textile_2_current_sequence == IDLE &&
                     textile_3_current_sequence == IDLE &&
                     textile_4_current_sequence == IDLE);

    uint8_t active_count = (textile_1_presence != 0) + (textile_2_presence != 0) +
                           (textile_3_presence != 0) + (textile_4_presence != 0);

    // Single: only device 3 present → 3A
    bool sequence_3_single_cond = (all_idle && textile_3_presence != 0 && active_count == 1);
    if (debounce(&textile_3_single_debounce, sequence_3_single_cond)) {
        update_trigger(SINGLE);
        update_sequence(SEQUENCE_A);
        clear_debounce(&textile_3_single_debounce);
    }

    // Double primary: device 3 + device 4 only (no device 1 or 2) → 3A
    bool sequence_3_double_pri = (all_idle && textile_3_presence != 0 &&
                                  textile_1_presence == 0 && textile_2_presence == 0 && active_count == 2);
    if (debounce(&textile_3_double_pri_debounce, sequence_3_double_pri)) {
        update_trigger(DOUBLE);
        update_sequence(SEQUENCE_A);
        clear_debounce(&textile_3_double_pri_debounce);
    }

    // Double secondary: device 3 + lower device (1 or 2) → just set trigger, wait
    bool sequence_3_double_sec = (all_idle && textile_3_presence != 0 &&
                                  (textile_1_presence != 0 || textile_2_presence != 0) && active_count == 2);
    if (debounce(&textile_3_double_sec_debounce, sequence_3_double_sec)) {
        update_trigger(DOUBLE);
        clear_debounce(&textile_3_double_sec_debounce);
    }

    // Triple: device 3 + two others → 3P immediately
    bool sequence_3_triple_cond = (all_idle && textile_3_presence != 0 && active_count == 3);
    if (debounce(&textile_3_triple_debounce, sequence_3_triple_cond)) {
        update_trigger(TRIPLE);
        update_sequence(SEQUENCE_P);
        clear_debounce(&textile_3_triple_debounce);
    }

    // Four: all devices → 3P immediately
    bool sequence_3_four_cond = (all_idle && textile_3_presence != 0 && active_count == 4);
    if (debounce(&textile_3_four_debounce, sequence_3_four_cond)) {
        update_trigger(FOUR);
        update_sequence(SEQUENCE_P);
        clear_debounce(&textile_3_four_debounce);
    }

    // From textile 1 chain: 1D → 2A → 3B
    bool sequence_3_from1_cond = (textile_1_current_sequence == FINISHED &&
                                  textile_2_current_sequence == FINISHED &&
                                  textile_3_current_sequence == IDLE &&
                                  textile_4_current_sequence == IDLE &&
                                  (textile_1_trigger_type == SINGLE || textile_1_trigger_type == DOUBLE));
    if (debounce(&textile_3_from1_debounce, sequence_3_from1_cond)) {
        update_sequence(SEQUENCE_B);
        clear_debounce(&textile_3_from1_debounce);
    }

    // From textile 2 chain: 2B → 3C
    bool sequence_3_from2_cond = (textile_1_current_sequence == IDLE &&
                                  textile_2_current_sequence == FINISHED &&
                                  textile_3_current_sequence == IDLE &&
                                  textile_4_current_sequence == IDLE &&
                                  (textile_2_trigger_type == SINGLE ||
                                   (textile_2_trigger_type == DOUBLE && textile_1_trigger_type != DOUBLE)));
    if (debounce(&textile_3_from2_debounce, sequence_3_from2_cond)) {
        update_sequence(SEQUENCE_C);
        clear_debounce(&textile_3_from2_debounce);
    }

    // From textile 4 chain: 4C → 3D
    bool sequence_3_from4_cond = (textile_1_current_sequence == IDLE &&
                                  textile_2_current_sequence == IDLE &&
                                  textile_3_current_sequence == IDLE &&
                                  textile_4_current_sequence == FINISHED &&
                                  (textile_4_trigger_type == SINGLE ||
                                   (textile_4_trigger_type == DOUBLE && textile_1_trigger_type != DOUBLE &&
                                    textile_2_trigger_type != DOUBLE && textile_3_trigger_type != DOUBLE)));
    if (debounce(&textile_3_from4_debounce, sequence_3_from4_cond)) {
        update_sequence(SEQUENCE_D);
        clear_debounce(&textile_3_from4_debounce);
    }

    // From triple (1+2+4): device 3 is the 4th, plays P after others finish
    bool sequence_3_from_triple_cond = (textile_3_current_sequence == IDLE &&
                                        textile_1_current_sequence == FINISHED &&
                                        textile_2_current_sequence == FINISHED &&
                                        textile_4_current_sequence == FINISHED &&
                                        textile_1_trigger_type == TRIPLE &&
                                        textile_2_trigger_type == TRIPLE &&
                                        textile_4_trigger_type == TRIPLE);
    if (debounce(&textile_3_from_triple_debounce, sequence_3_from_triple_cond)) {
        update_sequence(SEQUENCE_P);
        clear_debounce(&textile_3_from_triple_debounce);
    }

    // All finished
    bool sequence_3_all_finished = (textile_1_current_sequence == FINISHED &&
                                    textile_2_current_sequence == FINISHED &&
                                    textile_3_current_sequence == FINISHED &&
                                    textile_4_current_sequence == FINISHED);
    if (debounce(&textile_3_finished_debounce, sequence_3_all_finished)) {
        // Check if device 3 is DOUBLE secondary (paired with device 1 or 2)
        if (textile_3_trigger_type == DOUBLE &&
            (textile_1_trigger_type == DOUBLE || textile_2_trigger_type == DOUBLE)) {
            update_sequence(SEQUENCE_P);
        } else {
            update_trigger(NOT_TRIGGERED);
            update_sequence(IDLE);
        }
        clear_debounce(&textile_3_finished_debounce);
    }

    if( textile_3_current_sequence == SEQUENCE_A ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        for (uint16_t i = 0 ; i < sequence_3_A_size ; i++)
        {
            if((current_timer > sequence_3_A[i].time_starts_at) &&
               (current_timer < sequence_3_A[i].time_ends_at))
            {
                uint32_t duration = (sequence_3_A[i].time_ends_at - sequence_3_A[i].time_starts_at);
                uint32_t middle_point = (sequence_3_A[i].time_starts_at + (duration/2));
                if (sequence_3_A[i].led_index.positive_pin == IDX_D44)
                    setPin(IDX_D45, 0);
                if (sequence_3_A[i].led_index.positive_pin == IDX_D45)
                    setPin(IDX_D44, 0);
                setPin(sequence_3_A[i].led_index.positive_pin, 255);
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_3_A[i].value_starts_at, sequence_3_A[i].value_ends_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_3_A[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_3_A[i].value_ends_at, sequence_3_A[i].value_starts_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_3_A[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_3_A[sequence_3_A_size - 1].time_ends_at)
        {
            clearPins();
            update_sequence(FINISHED);
        }
    }

    if( textile_3_current_sequence == SEQUENCE_B ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        for (uint16_t i = 0 ; i < sequence_3_B_size ; i++)
        {
            if((current_timer > sequence_3_B[i].time_starts_at) &&
               (current_timer < sequence_3_B[i].time_ends_at))
            {
                uint32_t duration = (sequence_3_B[i].time_ends_at - sequence_3_B[i].time_starts_at);
                uint32_t middle_point = (sequence_3_B[i].time_starts_at + (duration/2));
                if (sequence_3_B[i].led_index.positive_pin == IDX_D44)
                    setPin(IDX_D45, 0);
                if (sequence_3_B[i].led_index.positive_pin == IDX_D45)
                    setPin(IDX_D44, 0);
                setPin(sequence_3_B[i].led_index.positive_pin, 255);
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_3_B[i].value_starts_at, sequence_3_B[i].value_ends_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_3_B[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_3_B[i].value_ends_at, sequence_3_B[i].value_starts_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_3_B[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_3_B[sequence_3_B_size - 1].time_ends_at)
        {
            clearPins();
            update_sequence(FINISHED);
        }
    }

    if( textile_3_current_sequence == SEQUENCE_C ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        for (uint16_t i = 0 ; i < sequence_3_C_size ; i++)
        {
            if((current_timer > sequence_3_C[i].time_starts_at) &&
               (current_timer < sequence_3_C[i].time_ends_at))
            {
                uint32_t duration = (sequence_3_C[i].time_ends_at - sequence_3_C[i].time_starts_at);
                uint32_t middle_point = (sequence_3_C[i].time_starts_at + (duration/2));
                if (sequence_3_C[i].led_index.positive_pin == IDX_D44)
                    setPin(IDX_D45, 0);
                if (sequence_3_C[i].led_index.positive_pin == IDX_D45)
                    setPin(IDX_D44, 0);
                setPin(sequence_3_C[i].led_index.positive_pin, 255);
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_3_C[i].value_starts_at, sequence_3_C[i].value_ends_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_3_C[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_3_C[i].value_ends_at, sequence_3_C[i].value_starts_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_3_C[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_3_C[sequence_3_C_size - 1].time_ends_at)
        {
            clearPins();
            update_sequence(FINISHED);
        }
    }

    if( textile_3_current_sequence == SEQUENCE_D ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        for (uint16_t i = 0 ; i < sequence_3_D_size ; i++)
        {
            if((current_timer > sequence_3_D[i].time_starts_at) &&
               (current_timer < sequence_3_D[i].time_ends_at))
            {
                uint32_t duration = (sequence_3_D[i].time_ends_at - sequence_3_D[i].time_starts_at);
                uint32_t middle_point = (sequence_3_D[i].time_starts_at + (duration/2));
                if (sequence_3_D[i].led_index.positive_pin == IDX_D44)
                    setPin(IDX_D45, 0);
                if (sequence_3_D[i].led_index.positive_pin == IDX_D45)
                    setPin(IDX_D44, 0);
                setPin(sequence_3_D[i].led_index.positive_pin, 255);
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_3_D[i].value_starts_at, sequence_3_D[i].value_ends_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_3_D[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_3_D[i].value_ends_at, sequence_3_D[i].value_starts_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_3_D[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_3_D[sequence_3_D_size - 1].time_ends_at)
        {
            clearPins();
            update_sequence(FINISHED);
        }
    }

    if( textile_3_current_sequence == SEQUENCE_P ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        for (uint16_t i = 0 ; i < sequence_3_P_size ; i++)
        {
            if((current_timer > sequence_3_P[i].time_starts_at) &&
               (current_timer < sequence_3_P[i].time_ends_at))
            {
                uint32_t duration = (sequence_3_P[i].time_ends_at - sequence_3_P[i].time_starts_at);
                uint32_t middle_point = (sequence_3_P[i].time_starts_at + (duration/2));
                if (sequence_3_P[i].led_index.positive_pin == IDX_D44)
                    setPin(IDX_D45, 0);
                if (sequence_3_P[i].led_index.positive_pin == IDX_D45)
                    setPin(IDX_D44, 0);
                setPin(sequence_3_P[i].led_index.positive_pin, 255);
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_3_P[i].value_starts_at, sequence_3_P[i].value_ends_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_3_P[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_3_P[i].value_ends_at, sequence_3_P[i].value_starts_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_3_P[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_3_P[sequence_3_P_size - 1].time_ends_at)
        {
            clearPins();
            if (textile_3_trigger_type == DOUBLE) {
                update_trigger(NOT_TRIGGERED);
                update_sequence(IDLE);
            } else {
                update_sequence(FINISHED);
            }
        }
    }

}


void textile_4_loop(void){
    // Initialize debounce timers
    static debounce_timer_st textile_4_single_debounce      = {0, false, DEBOUNCE_TIMER};
    static debounce_timer_st textile_4_double_sec_debounce   = {0, false, DEBOUNCE_TIMER};
    static debounce_timer_st textile_4_triple_debounce      = {0, false, DEBOUNCE_TIMER};
    static debounce_timer_st textile_4_four_debounce        = {0, false, DEBOUNCE_TIMER};
    static debounce_timer_st textile_4_from1_debounce       = {0, false, DEBOUNCE_TIMER};
    static debounce_timer_st textile_4_from2_debounce       = {0, false, DEBOUNCE_TIMER};
    static debounce_timer_st textile_4_from3_debounce       = {0, false, DEBOUNCE_TIMER};
    static debounce_timer_st textile_4_from_triple_debounce = {0, false, DEBOUNCE_TIMER};
    static debounce_timer_st textile_4_finished_debounce    = {0, false, DEBOUNCE_TIMER};

    bool all_idle = (textile_1_current_sequence == IDLE &&
                     textile_2_current_sequence == IDLE &&
                     textile_3_current_sequence == IDLE &&
                     textile_4_current_sequence == IDLE);

    uint8_t active_count = (textile_1_presence != 0) + (textile_2_presence != 0) +
                           (textile_3_presence != 0) + (textile_4_presence != 0);

    // Single: only device 4 present → 4C
    bool sequence_4_single_cond = (all_idle && textile_4_presence != 0 && active_count == 1);
    if (debounce(&textile_4_single_debounce, sequence_4_single_cond)) {
        update_trigger(SINGLE);
        update_sequence(SEQUENCE_C);
        clear_debounce(&textile_4_single_debounce);
    }

    // Double secondary: device 4 is always secondary (highest number) → just set trigger, wait
    bool sequence_4_double_sec = (all_idle && textile_4_presence != 0 && active_count == 2);
    if (debounce(&textile_4_double_sec_debounce, sequence_4_double_sec)) {
        update_trigger(DOUBLE);
        clear_debounce(&textile_4_double_sec_debounce);
    }

    // Triple: device 4 + two others → 4P immediately
    bool sequence_4_triple_cond = (all_idle && textile_4_presence != 0 && active_count == 3);
    if (debounce(&textile_4_triple_debounce, sequence_4_triple_cond)) {
        update_trigger(TRIPLE);
        update_sequence(SEQUENCE_P);
        clear_debounce(&textile_4_triple_debounce);
    }

    // Four: all devices → 4P immediately
    bool sequence_4_four_cond = (all_idle && textile_4_presence != 0 && active_count == 4);
    if (debounce(&textile_4_four_debounce, sequence_4_four_cond)) {
        update_trigger(FOUR);
        update_sequence(SEQUENCE_P);
        clear_debounce(&textile_4_four_debounce);
    }

    // From textile 1 chain: 1D → 2A → 3B → 4B
    bool sequence_4_from1_cond = (textile_1_current_sequence == FINISHED &&
                                  textile_2_current_sequence == FINISHED &&
                                  textile_3_current_sequence == FINISHED &&
                                  textile_4_current_sequence == IDLE &&
                                  (textile_1_trigger_type == SINGLE || textile_1_trigger_type == DOUBLE));
    if (debounce(&textile_4_from1_debounce, sequence_4_from1_cond)) {
        update_sequence(SEQUENCE_B);
        clear_debounce(&textile_4_from1_debounce);
    }

    // From textile 2 chain: 2B → 3C → 4D
    bool sequence_4_from2_cond = (textile_1_current_sequence == IDLE &&
                                  textile_2_current_sequence == FINISHED &&
                                  textile_3_current_sequence == FINISHED &&
                                  textile_4_current_sequence == IDLE &&
                                  (textile_2_trigger_type == SINGLE ||
                                   (textile_2_trigger_type == DOUBLE && textile_1_trigger_type != DOUBLE)));
    if (debounce(&textile_4_from2_debounce, sequence_4_from2_cond)) {
        update_sequence(SEQUENCE_D);
        clear_debounce(&textile_4_from2_debounce);
    }

    // From textile 3 chain: 3A → 1A → 4E
    bool sequence_4_from3_cond = (textile_1_current_sequence == FINISHED &&
                                  textile_2_current_sequence == IDLE &&
                                  textile_3_current_sequence == FINISHED &&
                                  textile_4_current_sequence == IDLE &&
                                  (textile_3_trigger_type == SINGLE ||
                                   (textile_3_trigger_type == DOUBLE && textile_1_trigger_type != DOUBLE &&
                                    textile_2_trigger_type != DOUBLE)));
    if (debounce(&textile_4_from3_debounce, sequence_4_from3_cond)) {
        update_sequence(SEQUENCE_E);
        clear_debounce(&textile_4_from3_debounce);
    }

    // From triple (1+2+3): device 4 is the 4th, plays P after others finish
    bool sequence_4_from_triple_cond = (textile_4_current_sequence == IDLE &&
                                        textile_1_current_sequence == FINISHED &&
                                        textile_2_current_sequence == FINISHED &&
                                        textile_3_current_sequence == FINISHED &&
                                        textile_1_trigger_type == TRIPLE &&
                                        textile_2_trigger_type == TRIPLE &&
                                        textile_3_trigger_type == TRIPLE);
    if (debounce(&textile_4_from_triple_debounce, sequence_4_from_triple_cond)) {
        update_sequence(SEQUENCE_P);
        clear_debounce(&textile_4_from_triple_debounce);
    }

    // All finished
    bool sequence_4_all_finished = (textile_1_current_sequence == FINISHED &&
                                    textile_2_current_sequence == FINISHED &&
                                    textile_3_current_sequence == FINISHED &&
                                    textile_4_current_sequence == FINISHED);
    if (debounce(&textile_4_finished_debounce, sequence_4_all_finished)) {
        // Check if device 4 is DOUBLE secondary (paired with any lower device)
        if (textile_4_trigger_type == DOUBLE &&
            (textile_1_trigger_type == DOUBLE || textile_2_trigger_type == DOUBLE ||
             textile_3_trigger_type == DOUBLE)) {
            update_sequence(SEQUENCE_P);
        } else {
            update_trigger(NOT_TRIGGERED);
            update_sequence(IDLE);
        }
        clear_debounce(&textile_4_finished_debounce);
    }

    if( textile_4_current_sequence == SEQUENCE_A ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        for (uint16_t i = 0 ; i < sequence_4_A_size ; i++)
        {
            if((current_timer > sequence_4_A[i].time_starts_at) &&
               (current_timer < sequence_4_A[i].time_ends_at))
            {
                uint32_t duration = (sequence_4_A[i].time_ends_at - sequence_4_A[i].time_starts_at);
                uint32_t middle_point = (sequence_4_A[i].time_starts_at + (duration/2));
                if (sequence_4_A[i].led_index.positive_pin == IDX_D44)
                    setPin(IDX_D45, 0);
                if (sequence_4_A[i].led_index.positive_pin == IDX_D45)
                    setPin(IDX_D44, 0);
                setPin(sequence_4_A[i].led_index.positive_pin, 255);
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_4_A[i].value_starts_at, sequence_4_A[i].value_ends_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_4_A[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_4_A[i].value_ends_at, sequence_4_A[i].value_starts_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_4_A[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_4_A[sequence_4_A_size - 1].time_ends_at)
        {
            clearPins();
            update_sequence(FINISHED);
        }
    }

    if( textile_4_current_sequence == SEQUENCE_B ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        for (uint16_t i = 0 ; i < sequence_4_B_size ; i++)
        {
            if((current_timer > sequence_4_B[i].time_starts_at) &&
               (current_timer < sequence_4_B[i].time_ends_at))
            {
                uint32_t duration = (sequence_4_B[i].time_ends_at - sequence_4_B[i].time_starts_at);
                uint32_t middle_point = (sequence_4_B[i].time_starts_at + (duration/2));
                if (sequence_4_B[i].led_index.positive_pin == IDX_D44)
                    setPin(IDX_D45, 0);
                if (sequence_4_B[i].led_index.positive_pin == IDX_D45)
                    setPin(IDX_D44, 0);
                setPin(sequence_4_B[i].led_index.positive_pin, 255);
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_4_B[i].value_starts_at, sequence_4_B[i].value_ends_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_4_B[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_4_B[i].value_ends_at, sequence_4_B[i].value_starts_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_4_B[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_4_B[sequence_4_B_size - 1].time_ends_at)
        {
            clearPins();
            update_sequence(FINISHED);
        }
    }

    if( textile_4_current_sequence == SEQUENCE_C ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        for (uint16_t i = 0 ; i < sequence_4_C_size ; i++)
        {
            if((current_timer > sequence_4_C[i].time_starts_at) &&
               (current_timer < sequence_4_C[i].time_ends_at))
            {
                uint32_t duration = (sequence_4_C[i].time_ends_at - sequence_4_C[i].time_starts_at);
                uint32_t middle_point = (sequence_4_C[i].time_starts_at + (duration/2));
                if (sequence_4_C[i].led_index.positive_pin == IDX_D44)
                    setPin(IDX_D45, 0);
                if (sequence_4_C[i].led_index.positive_pin == IDX_D45)
                    setPin(IDX_D44, 0);
                setPin(sequence_4_C[i].led_index.positive_pin, 255);
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_4_C[i].value_starts_at, sequence_4_C[i].value_ends_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_4_C[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_4_C[i].value_ends_at, sequence_4_C[i].value_starts_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_4_C[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_4_C[sequence_4_C_size - 1].time_ends_at)
        {
            clearPins();
            update_sequence(FINISHED);
        }
    }

    if( textile_4_current_sequence == SEQUENCE_D ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        for (uint16_t i = 0 ; i < sequence_4_D_size ; i++)
        {
            if((current_timer > sequence_4_D[i].time_starts_at) &&
               (current_timer < sequence_4_D[i].time_ends_at))
            {
                uint32_t duration = (sequence_4_D[i].time_ends_at - sequence_4_D[i].time_starts_at);
                uint32_t middle_point = (sequence_4_D[i].time_starts_at + (duration/2));
                if (sequence_4_D[i].led_index.positive_pin == IDX_D44)
                    setPin(IDX_D45, 0);
                if (sequence_4_D[i].led_index.positive_pin == IDX_D45)
                    setPin(IDX_D44, 0);
                setPin(sequence_4_D[i].led_index.positive_pin, 255);
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_4_D[i].value_starts_at, sequence_4_D[i].value_ends_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_4_D[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_4_D[i].value_ends_at, sequence_4_D[i].value_starts_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_4_D[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_4_D[sequence_4_D_size - 1].time_ends_at)
        {
            clearPins();
            update_sequence(FINISHED);
        }
    }

    if( textile_4_current_sequence == SEQUENCE_E ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        for (uint16_t i = 0 ; i < sequence_4_E_size ; i++)
        {
            if((current_timer > sequence_4_E[i].time_starts_at) &&
               (current_timer < sequence_4_E[i].time_ends_at))
            {
                uint32_t duration = (sequence_4_E[i].time_ends_at - sequence_4_E[i].time_starts_at);
                uint32_t middle_point = (sequence_4_E[i].time_starts_at + (duration/2));
                if (sequence_4_E[i].led_index.positive_pin == IDX_D44)
                    setPin(IDX_D45, 0);
                if (sequence_4_E[i].led_index.positive_pin == IDX_D45)
                    setPin(IDX_D44, 0);
                setPin(sequence_4_E[i].led_index.positive_pin, 255);
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_4_E[i].value_starts_at, sequence_4_E[i].value_ends_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_4_E[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_4_E[i].value_ends_at, sequence_4_E[i].value_starts_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_4_E[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_4_E[sequence_4_E_size - 1].time_ends_at)
        {
            clearPins();
            update_sequence(FINISHED);
        }
    }

    if( textile_4_current_sequence == SEQUENCE_P ){
        uint32_t current_timer = millis() - internal_sequence_timer;
        for (uint16_t i = 0 ; i < sequence_4_P_size ; i++)
        {
            if((current_timer > sequence_4_P[i].time_starts_at) &&
               (current_timer < sequence_4_P[i].time_ends_at))
            {
                uint32_t duration = (sequence_4_P[i].time_ends_at - sequence_4_P[i].time_starts_at);
                uint32_t middle_point = (sequence_4_P[i].time_starts_at + (duration/2));
                if (sequence_4_P[i].led_index.positive_pin == IDX_D44)
                    setPin(IDX_D45, 0);
                if (sequence_4_P[i].led_index.positive_pin == IDX_D45)
                    setPin(IDX_D44, 0);
                setPin(sequence_4_P[i].led_index.positive_pin, 255);
                if(current_timer <= middle_point){
                    uint32_t duration_left = middle_point - current_timer;
                    uint8_t mapped_value = map(duration_left, (duration/2), 0, sequence_4_P[i].value_starts_at, sequence_4_P[i].value_ends_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_4_P[i].led_index.negative_pin, mapped_value);
                }
                if(current_timer > middle_point){
                    uint32_t duration_left = current_timer - middle_point;
                    uint8_t mapped_value = map(duration_left, 0, (duration/2), sequence_4_P[i].value_ends_at, sequence_4_P[i].value_starts_at);
                    if (mapped_value > 220)
                        mapped_value = 245;
                    setPin(sequence_4_P[i].led_index.negative_pin, mapped_value);
                }
            }
        }
        if(current_timer > sequence_4_P[sequence_4_P_size - 1].time_ends_at)
        {
            clearPins();
            if (textile_4_trigger_type == DOUBLE) {
                update_trigger(NOT_TRIGGERED);
                update_sequence(IDLE);
            } else {
                update_sequence(FINISHED);
            }        }
    }

}