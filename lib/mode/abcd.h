#ifndef ABCD_H
#define ABCD_H

#include "base.h"

typedef enum
{
    AbTurn,
    CdTurn
} AbcdTurn;

typedef struct
{
    int current_time;
    AbcdTurn current_turn;
    int _current_turn_this_round; // number of turns that have were this round
    BaseModeData *base;
} AbcdModeData;

void abcd_mode_init(AbcdModeData *mode_data, BaseModeData *base_mode_data, InitializationCommand *init_data);
void abcd_mode_next_step(AbcdModeData *mode_data);
void abcd_mode_handle_sec_tick(AbcdModeData *mode_data);
void abcd_mode_print(AbcdModeData *mode_data);
void abcd_mode_reset_state(AbcdModeData *mode_data, InitializationCommand *init_data);
void abcd_mode_display(DisplayState *display, AbcdModeData *mode_data);
void abcd_mode_next_turn(AbcdModeData *mode_data);
void abcd_mode_free(void **mode_data);

#endif // ABCD_H
