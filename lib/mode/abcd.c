#include "abcd.h"
#include <stdlib.h>
#include <stdio.h> // TODO delete
#include "lib/mode/base.h"

AbcdModeData *abcd_mode_init(InitializationData init_data)
{
    AbcdModeData *mode_data = (AbcdModeData *)malloc(sizeof(AbcdModeData));
    if (mode_data == NULL)
        return NULL;

    base_mode_init(
        &mode_data->base,
        init_data,
        (void (*)(void *)) & abcd_mode_next_step,
        (void (*)(void *)) & abcd_mode_handle_sec_tick,
        (void (*)(void *)) & abcd_mode_print,
        (void (*)(void *, InitializationData)) & abcd_mode_reset_state,
        (void (*)(DisplayState *, void *)) & abcd_mode_display);
    abcd_mode_reset_state(mode_data, init_data);

    return mode_data;
}

void abcd_mode_next_step(AbcdModeData *mode_data)
{
    if (mode_data == NULL)
        return;

    // prep time can't be skipped
    if (base_mode_running(mode_data->base) && base_mode_prep_time(mode_data->base))
        return;

    if (!base_mode_running(mode_data->base))
    { // not running -> run; take turn from previous round
        base_mode_set_running(&mode_data->base, true);
        mode_data->current_time = mode_data->base.init_data.time_per_round;

        // handle that there is even number of training rounds -> start the competition always with Ab round
        if (round_info_first_competition_round(mode_data->base.round_info))
        {
            mode_data->current_turn = AbTurn;
        }
    }

    else
    { // if it's running, set next turn (turn overflow is handled in that method)
        abcd_mode_next_turn(mode_data);
    }
}

void abcd_mode_handle_sec_tick(AbcdModeData *mode_data)
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
        abcd_mode_next_turn(mode_data);
    }
}

void abcd_mode_next_turn(AbcdModeData *mode_data)
{
    if (mode_data == NULL)
        return;

    if (mode_data->_current_turn_this_round >= mode_data->base.init_data.turns_per_round)
    {
        mode_data->_current_turn_this_round = 1;
        // turn is not set, because the last one is starting in the next round
        base_mode_set_running(&mode_data->base, false);
    }

    else
    {
        mode_data->_current_turn_this_round++;
        mode_data->current_turn = (mode_data->current_turn == AbTurn) ? CdTurn : AbTurn;
        mode_data->current_time = mode_data->base.init_data.time_per_round;
    }
}

void abcd_mode_print(AbcdModeData *mode_data)
{
    printf("ABCD(time: %d s, turn: %s, running: %s, prep: %s, training: %s, round: %d)\n",
           base_mode_current_time(mode_data->base),
           (mode_data->current_turn == AbTurn) ? "AB" : "CD",
           (base_mode_running(mode_data->base)) ? "true" : "false",
           (base_mode_prep_time(mode_data->base)) ? "true" : "false",
           (mode_data->base.round_info.training) ? "true" : "false",
           mode_data->base.round_info.current_round);
}

void abcd_mode_display(DisplayState *display, AbcdModeData *mode_data)
{
    if (display == NULL || mode_data == NULL)
        return;

    // set time
    sprintf(display->left_display, "%3d", base_mode_current_time(mode_data->base));
    // set turn
    sprintf(display->right_display, "%s",
             (mode_data->current_turn == AbTurn) ? "AB " : "CD ");

    base_mode_display_semaphor(display, mode_data->base);
    base_mode_round_display(&mode_data->base, display->middle_display);
}

void abcd_mode_reset_state(AbcdModeData *mode_data, InitializationData init_data)
{
    base_mode_reset_state(&mode_data->base, init_data);
    mode_data->current_turn = AbTurn;
    mode_data->_current_turn_this_round = 1;
}
