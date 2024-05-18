#include "abc.h"
#include "ab.h"
#include <stdlib.h>
#include <stdio.h> // TODO delete
#include <logging.h>

AbcModeData *abc_mode_init(InitializationData *init_data)
{
    AbcModeData *mode_data = (AbcModeData *)malloc(sizeof(AbcModeData));
    if (mode_data == NULL)
        return NULL;

    mode_data->base = base_mode_init(
        init_data,
        (NextStepCallback_t *)&ab_mode_next_step,
        (HandleSecTickCallback_t *)&ab_mode_handle_sec_tick,
        (PrintCallback_t *)&abc_mode_print,
        (ResetStateCallback_t *)&ab_mode_reset_state,
        (DisplayCallback_t *)&abc_mode_display,
        (FreeCallback_t *)&ab_mode_free);

    return mode_data;
}

void abc_mode_display(DisplayState *display, AbcModeData *mode_data)
{
    if (display == NULL || mode_data == NULL)
        return;

    ab_mode_display(display, mode_data);

    // set turn (overwrite)
    sprintf(display->right_display, "%s", "ABC");
}

void abc_mode_print(AbcModeData *mode_data)
{
    log(
        "ABC(time: %d s, running: %s, prep: %s, training: %s, round: %d)\n\r",
        base_mode_current_time(mode_data->base),
        (base_mode_running(mode_data->base)) ? "true" : "false",
        (base_mode_prep_time(mode_data->base)) ? "true" : "false",
        (mode_data->base->round_info->training) ? "true" : "false",
        mode_data->base->round_info->current_round);
}
