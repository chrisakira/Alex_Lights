#ifndef __SEQUENCE_CONTROLLER_H__
#define __SEQUENCE_CONTROLLER_H__

#include <stddef.h>
#include <stdlib.h>

#include "extras.h"

extern uint8_t active_sequence;


void loop_sequence(uint8_t debug_info);
void textile_1_loop(void);
void textile_2_loop(void);
void textile_3_loop(void); 
void textile_4_loop(void);




#endif // __SEQUENCE_CONTROLLER_H__