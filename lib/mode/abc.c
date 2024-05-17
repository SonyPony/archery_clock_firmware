#include "abc.h"
#include "ab.h"
#include <stdlib.h>
#include <stdio.h> // TODO delete

AbcModeData *abc_mode_init(InitializationData init_data)
{
    AbcModeData *mode_data = (AbcModeData *)malloc(sizeof(AbcModeData));
    if (mode_data == NULL)
        return NULL;

    base_mode_init(
        &mode_data->base,
        init_data,
        (void (*)(void *)) & ab_mode_next_step,
        (void (*)(void *)) & ab_mode_handle_sec_tick,
        (void (*)(void *)) & ab_mode_print,
        (void (*)(void *, InitializationData)) & ab_mode_reset_state,
        (void (*)(DisplayState *, void *)) & abc_mode_display);

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

void abc_model_print(AbcModeData *mode_data)
{
    printf("ABC(time: %d s, running: %s, prep: %s, training: %s, round: %d)\n",
           base_mode_current_time(mode_data->base),
           (base_mode_running(mode_data->base)) ? "true" : "false",
           (base_mode_prep_time(mode_data->base)) ? "true" : "false",
           (mode_data->base.round_info.training) ? "true" : "false",
           mode_data->base.round_info.current_round);
}
