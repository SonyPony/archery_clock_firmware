#ifndef AB_H
#define AB_H

#include "base.h"
#include <data_model.h>

typedef struct
{
    int current_time;
    BaseModeData base;
} AbModeData;

AbModeData *ab_mode_init(InitializationData init_data);
void ab_mode_next_step(AbModeData *mode_data);
void ab_mode_handle_sec_tick(AbModeData *mode_data);
void ab_mode_print(AbModeData *mode_data);
void ab_mode_reset_state(AbModeData *mode_data, InitializationData init_data);
void ab_mode_display(DisplayState *display, AbModeData *mode_data);

#endif // AB_H
