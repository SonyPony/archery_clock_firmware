#include "finals.h"
#include <stdlib.h>
#include <stdio.h> // TODO delete
#include <lib/data/data_model.h>
#include <lib/logging/logging.h>

void finals_mode_init(FinalsModeData *mode_data, BaseModeData *base_mode_data, RoundInfo *round_info, InitializationCommand *init_data)
{
    if (mode_data == NULL)
        return;

    mode_data->base = base_mode_data;
    base_mode_init(
        mode_data->base,
        round_info,
        init_data,
        (NextStepCallback_t *)&finals_mode_next_step,
        (HandleSecTickCallback_t *)&finals_mode_handle_sec_tick,
        (PrintCallback_t *)&finals_mode_print,
        (ResetStateCallback_t *)&finals_mode_reset_state,
        (DisplayCallback_t *)&finals_mode_display,
        (FreeCallback_t *)&finals_mode_free);
    finals_mode_reset_state(mode_data, init_data);
}

void finals_mode_next_turn(FinalsModeData *mode_data)
{
    if (mode_data == NULL)
        return;

    if (mode_data->_current_turn_this_round >= mode_data->base->init_data->turns_per_round)
    {
        finals_mode_end_round(mode_data);
    }

    else
    {
        mode_data->_current_turn_this_round++;
        if (mode_data->current_turn == FinalsLeftTurn)
        {
            mode_data->current_turn = FinalsRightTurn;
            mode_data->base->current_timer = &mode_data->right_current_time;
            if (mode_data->_reset_time_after_turn)
            {
                mode_data->left_current_time = mode_data->base->init_data->time_per_round;
            }
        }
        else
        {
            mode_data->current_turn = FinalsLeftTurn;
            mode_data->base->current_timer = &mode_data->left_current_time;
            if (mode_data->_reset_time_after_turn)
            {
                mode_data->right_current_time = mode_data->base->init_data->time_per_round;
            }
        }
    }
}

void finals_mode_next_step(FinalsModeData *mode_data)
{
    if (mode_data == NULL)
        return;

    BASE_MODE_HANDLE_NEXT_STEP();

    if (!base_mode_running(mode_data->base))
    {                                                 // not running -> run;
        base_mode_set_running(mode_data->base, true); // start of another round (turn is already prepared from the last call)
        mode_data->left_current_time = mode_data->base->init_data->time_per_round;
        mode_data->right_current_time = mode_data->base->init_data->time_per_round;

        // handle that there is even number of training rounds -> start the competition always with Ab round
        if (round_info_first_competition_round(mode_data->base->round_info))
        {
            mode_data->current_turn = FinalsLeftTurn;
        }
    }

    else
    { // if it's running, set next turn (turn overflow is handled in that method)
        finals_mode_next_turn(mode_data);
    }
}

void finals_mode_handle_sec_tick(FinalsModeData *mode_data)
{
    if (mode_data == NULL)
        return;

    BASE_MODE_HANDLE_SEC_TICK();

    if (base_mode_current_timer_is_prep(mode_data->base))
    {
        mode_data->base->current_timer = &mode_data->left_current_time;
    }

    if (base_mode_current_time(mode_data->base) == 0)
    {
        if (mode_data->current_turn == FinalsLeftTurn)
        {
            finals_mode_next_step(mode_data);
        }

        else
        {
            finals_mode_end_round(mode_data);
        }
    }
}

void finals_mode_print(FinalsModeData *mode_data)
{
    Logging::log(
        "Finals(time: %d s, turn: %s, running: %s, prep: %s, training: %s, round: %d)\n\r",
        base_mode_current_time(mode_data->base),
        (mode_data->current_turn == FinalsLeftTurn) ? "L" : "R",
        (base_mode_running(mode_data->base)) ? "true" : "false",
        (base_mode_prep_time(mode_data->base)) ? "true" : "false",
        (mode_data->base->round_info->training) ? "true" : "false",
        mode_data->base->round_info->current_round);
}

void finals_mode_reset_state(FinalsModeData *mode_data, InitializationCommand *init_data)
{
    base_mode_reset_state(mode_data->base, init_data);
    finals_mode_end_round(mode_data);
    mode_data->_reset_time_after_turn = init_data->turn_type == FinalsIndividual_TurnType;
    mode_data->left_current_time = mode_data->base->init_data->time_per_round;
        mode_data->right_current_time = mode_data->base->init_data->time_per_round;
}

void finals_mode_display(DisplayState *display, FinalsModeData *mode_data)
{
    if (display == NULL || mode_data == NULL)
        return;

    BASE_MODE_DISPLAY();

    base_mode_display_semaphor(display, mode_data->base);
    // set time
    sprintf(display->left_display, "%3d", mode_data->left_current_time);
    sprintf(display->right_display, "%3d", mode_data->right_current_time);

    // display turn
    sprintf(display->middle_display, "%s",
            (mode_data->current_turn == FinalsLeftTurn) ? "< " : " >");
}

void finals_mode_end_round(FinalsModeData *mode_data)
{
    mode_data->_current_turn_this_round = 1;
    mode_data->current_turn = FinalsLeftTurn;
    base_mode_set_running(mode_data->base, false);
}

void finals_mode_free(void **mode_data)
{
    FinalsModeData **casted_mode_data = (FinalsModeData **)(mode_data);
    base_mode_free_internal(&(*casted_mode_data)->base);
    base_mode_free(mode_data);
}
