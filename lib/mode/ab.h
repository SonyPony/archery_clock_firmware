#ifndef AB_H
#define AB_H

#include "base.h"
#include <lib/data/data_model.h>

struct AbModeData
{
    int current_time;
    BaseModeData *base;
};

void ab_mode_init(AbModeData *mode_data, BaseModeData *base_mode_data, RoundInfo *round_info, InitializationCommand *init_data);
void ab_mode_next_step(AbModeData *mode_data);
void ab_mode_handle_sec_tick(AbModeData *mode_data);
void ab_mode_print(AbModeData *mode_data);
void ab_mode_reset_state(AbModeData *mode_data, InitializationCommand *init_data);
void ab_mode_display(DisplayState *display, AbModeData *mode_data);
void ab_mode_free(void **mode_data);

#endif // AB_H
