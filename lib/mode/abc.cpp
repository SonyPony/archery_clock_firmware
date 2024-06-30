#include "abc.h"
#include <lib/mode/ab.h>
#include <stdlib.h>
#include <stdio.h> // TODO delete
#include <lib/logging/logging.h>

void abc_mode_init(AbcModeData *mode_data, BaseModeData *base_mode_data, RoundInfo *round_info, InitializationCommand *init_data)
{
    if (mode_data == NULL)
        return;

    mode_data->base = base_mode_data;
    base_mode_init(
        mode_data->base,
        round_info,
        init_data,
        (NextStepCallback_t *)&ab_mode_next_step,
        (HandleSecTickCallback_t *)&ab_mode_handle_sec_tick,
        (PrintCallback_t *)&abc_mode_print,
        (ResetStateCallback_t *)&ab_mode_reset_state,
        (DisplayCallback_t *)&abc_mode_display,
        (FreeCallback_t *)&ab_mode_free);
}

void abc_mode_display(DisplayState *display, AbcModeData *mode_data)
{
    if (display == NULL || mode_data == NULL)
        return;

    BASE_MODE_DISPLAY();

    ab_mode_display(display, mode_data);

    // set turn (overwrite)
    sprintf(display->rightDisplay, "%s", "ABC");
}

void abc_mode_print(AbcModeData *mode_data)
{
    Logging::log(
        "ABC(time: %d s, running: %s, prep: %s, training: %s, round: %d)\n\r",
        base_mode_current_time(mode_data->base),
        (base_mode_running(mode_data->base)) ? "true" : "false",
        (base_mode_prep_time(mode_data->base)) ? "true" : "false",
        (mode_data->base->round_info->training) ? "true" : "false",
        mode_data->base->round_info->current_round);
}
