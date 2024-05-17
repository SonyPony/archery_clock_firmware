#ifndef FINALS_H
#define FINALS_H

#include <stdbool.h>
#include "../data/run_data.h"
#include "base.h"

typedef enum
{
    FinalsLeftTurn,
    FinalsRightTurn,
} FinalsTurns;

typedef struct
{
    FinalsTurns current_turn;
    int left_current_time;        // in seconds
    int right_current_time;       // in seconds
    int _current_turn_this_round; // number of turns that have were this round
    bool _reset_time_after_turn;
    BaseModeData base;
} FinalsModeData;

FinalsModeData *finals_mode_init(InitializationData init_data);
void finals_mode_next_step(FinalsModeData *mode_data);
void finals_mode_handle_sec_tick(FinalsModeData *mode_data);
void finals_mode_print(FinalsModeData *mode_data);
void finals_mode_reset_state(FinalsModeData *mode_data, InitializationData init_data);
void finals_mode_display(DisplayState *display, FinalsModeData *mode_data);
void finals_mode_next_turn(FinalsModeData *mode_data);
void finals_mode_end_round(FinalsModeData *mode_data);

#endif // FINALS_H
