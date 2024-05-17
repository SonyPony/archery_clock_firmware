#include "ab.h"
#include <stdio.h> // TODO delete
#include <stdlib.h>

AbModeData *ab_mode_init(InitializationData init_data)
{
    AbModeData *mode_data = (AbModeData *)malloc(sizeof(AbModeData));
    if (mode_data == NULL)
        return NULL;

    base_mode_init(
        &mode_data->base,
        init_data,
        (void (*)(void *)) & ab_mode_next_step,
        (void (*)(void *)) & ab_mode_handle_sec_tick,
        (void (*)(void *)) & ab_mode_print,
        (void (*)(void *, InitializationData)) & ab_mode_reset_state,
        (void (*)(DisplayState *, void *)) & ab_mode_display);

    return mode_data;
}

void ab_mode_next_step(AbModeData *mode_data)
{
    if (mode_data == NULL)
        return;

    // prep time can't be skipped
    if (base_mode_running(mode_data->base) && base_mode_prep_time(mode_data->base))
        return;

    if (!base_mode_running(mode_data->base))
    { // not running -> run;
        base_mode_set_running(&mode_data->base, true);
        mode_data->current_time = mode_data->base.init_data.time_per_round;
        return;
    }

    // if it's running do following
    base_mode_set_running(&mode_data->base, false);
    mode_data->current_time = 0;
}

void ab_mode_handle_sec_tick(AbModeData *mode_data)
{
    if (mode_data == NULL)
        return;

    if (!base_mode_running(mode_data->base)) // if it's not running, do nothing
        return;

    base_mode_decrement_current_time(&mode_data->base);

    const bool prep_time_running = base_mode_prep_time(mode_data->base);
    if (prep_time_running)
        return;

    mode_data->base.current_timer = &mode_data->current_time;

    if (base_mode_current_time(mode_data->base) == 0)
    {
        base_mode_set_running(&mode_data->base, false);
        return;
    }
}

void ab_mode_print(AbModeData *mode_data)
{
    printf("AB(time: %d s, running: %s, prep: %s, training: %s, round: %d)\n",
           base_mode_current_time(mode_data->base),
           (base_mode_running(mode_data->base)) ? "true" : "false",
           (base_mode_prep_time(mode_data->base)) ? "true" : "false",
           (mode_data->base.round_info.training) ? "true" : "false",
           mode_data->base.round_info.current_round);
}

void ab_mode_display(DisplayState *display, AbModeData *mode_data)
{
    if (display == NULL || mode_data == NULL)
        return;

    // set time
    sprintf(display->left_display, "%3d", base_mode_current_time(mode_data->base));
    // set turn
    sprintf(display->right_display, "%s", "AB ");

    base_mode_display_semaphor(display, mode_data->base);
    base_mode_round_display(&mode_data->base, display->middle_display);
}

void ab_mode_reset_state(AbModeData *mode_data, InitializationData init_data)
{
    base_mode_reset_state(&mode_data->base, init_data);
}
