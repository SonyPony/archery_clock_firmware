#include "ab.h"
#include <stdio.h> // TODO delete
#include <stdlib.h>
#include <lib/logging/logging.h>

void ab_mode_init(AbModeData *mode_data, BaseModeData *base_mode_data, RoundInfo *round_info, InitializationCommand *init_data)
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
        (PrintCallback_t *)&ab_mode_print,
        (ResetStateCallback_t *)&ab_mode_reset_state,
        (DisplayCallback_t *)&ab_mode_display,
        (FreeCallback_t *)&ab_mode_free);
}

void ab_mode_next_step(AbModeData *mode_data)
{
    if (mode_data == NULL)
        return;

    BASE_MODE_HANDLE_NEXT_STEP();

    if (!base_mode_running(mode_data->base))
    { // not running -> run;
        base_mode_set_running(mode_data->base, true);
        mode_data->current_time = mode_data->base->init_data->time_per_round;
        return;
    }

    // if it's running do following
    base_mode_set_running(mode_data->base, false);
    mode_data->current_time = 0;
}

void ab_mode_handle_sec_tick(AbModeData *mode_data)
{
    if (mode_data == NULL)
        return;

    BASE_MODE_HANDLE_SEC_TICK();

    mode_data->base->current_timer = &mode_data->current_time;

    if (base_mode_current_time(mode_data->base) == 0)
    {
        base_mode_set_running(mode_data->base, false);
        return;
    }
}

void ab_mode_print(AbModeData *mode_data)
{
    log(
        "AB(time: %d s, running: %s, prep: %s, training: %s, round: %d)\n\r",
        base_mode_current_time(mode_data->base),
        (base_mode_running(mode_data->base)) ? "true" : "false",
        (base_mode_prep_time(mode_data->base)) ? "true" : "false",
        (mode_data->base->round_info->training) ? "true" : "false",
        mode_data->base->round_info->current_round);
}

void ab_mode_display(DisplayState *display, AbModeData *mode_data)
{
    if (display == NULL || mode_data == NULL)
        return;

    BASE_MODE_DISPLAY();

    // set time
    sprintf(display->left_display, "%3d", base_mode_current_time(mode_data->base));
    // set turn
    sprintf(display->right_display, "%s", "AB ");

    base_mode_display_semaphor(display, mode_data->base);
    base_mode_round_display(mode_data->base, display->middle_display);
}

void ab_mode_reset_state(AbModeData *mode_data, InitializationCommand *init_data)
{
    base_mode_reset_state(mode_data->base, init_data);
    mode_data->current_time = mode_data->base->init_data->time_per_round;
}

void ab_mode_free(void **mode_data)
{
    AbModeData **casted_mode_data = (AbModeData **)(mode_data);
    base_mode_free_internal(&(*casted_mode_data)->base);
    base_mode_free(mode_data);
}
