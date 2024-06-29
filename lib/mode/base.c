#include "base.h"
#include <stdio.h>
#include <stdlib.h>
#include <display.h>
#include <data_model.h>
#include <logging.h>

void base_mode_init(
    BaseModeData *mode_data,
    RoundInfo* round_info,
    InitializationCommand *init_data,
    NextStepCallback_t *next_step_func,
    HandleSecTickCallback_t *handle_sec_tic_func,
    PrintCallback_t *print_func,
    ResetStateCallback_t *reset_state_func,
    DisplayCallback_t *display_func,
    FreeCallback_t *free_func)
{
    if (mode_data == NULL)
        return;

    mode_data->round_info = round_info;
    mode_data->next_step = next_step_func;
    mode_data->handle_sec_tic = handle_sec_tic_func;
    mode_data->print = print_func;
    mode_data->reset_state = reset_state_func;
    mode_data->free = free_func;
    mode_data->display = display_func;

    base_mode_reset_state(mode_data, init_data);
}

void base_mode_reset_state(BaseModeData *mode_data, InitializationCommand *init_data)
{
    if (mode_data == NULL || init_data == NULL)
        return;

    mode_data->running = false;
    mode_data->paused = false;
    mode_data->init_data = init_data;
    mode_data->is_break = false;
    mode_data->_break_timer = 0;

    base_mode_restore_prep_time(mode_data);

    round_info_init(mode_data->round_info, init_data->training_rounds_count);
}

void base_mode_restore_prep_time(BaseModeData *mode_data)
{
    mode_data->_prep_timer = mode_data->init_data->prep_time;
    mode_data->current_timer = &mode_data->_prep_timer;
}

void base_mode_set_current_time(BaseModeData *mode_data, int value)
{
    if (mode_data == NULL)
        return;

    *(mode_data->current_timer) = value;
}

void base_mode_decrement_current_time(BaseModeData *mode_data)
{
    if (mode_data == NULL)
        return;

    *(mode_data->current_timer) -= 1;
}

int base_mode_current_time(BaseModeData *mode_data)
{
    return *(mode_data->current_timer);
}

void base_mode_set_running(BaseModeData *mode_data, bool running)
{
    mode_data->running = running;
    if (mode_data->running)
        base_mode_restore_prep_time(mode_data);

    else
        base_mode_next_round(mode_data);
}

bool base_mode_running(BaseModeData *mode_data)
{
    if (mode_data == NULL)
        return false;
    return mode_data->running;
}

bool base_mode_prep_time(BaseModeData *mode_data)
{
    if (mode_data == NULL)
        return false;
    return mode_data->_prep_timer > 0;
}

bool base_mode_in_warning_time(BaseModeData *mode_data)
{
    if (mode_data == NULL)
        return false;

    if (!base_mode_prep_time(mode_data))
        return base_mode_current_time(mode_data) <= mode_data->init_data->warning_time;
    return false;
}

void round_info_prev_round(RoundInfo *round_info)
{
    if (round_info == NULL)
        return;

    round_info->current_round--; // decrese the current round

    // switch back to training rounds
    if (!round_info->training && round_info->current_round <= 0)
    {
        round_info->current_round = round_info->_training_rounds_count;
        round_info->training = true;
    }

    // handle underflow of training rounds
    if (round_info->training && round_info->current_round <= 0)
        round_info->current_round = 1;
}

void round_info_next_round(RoundInfo *round_info)
{
    if (round_info == NULL)
        return;

    round_info->current_round++; // increase current round

    // end of training rounds
    if (round_info->training && round_info->current_round > round_info->_training_rounds_count)
    {
        round_info->current_round = 1;
        round_info->training = false;
    }

    // handle maximum number of rounds (99)
    if (round_info->current_round > 99)
        round_info->current_round = 99;
}

void base_mode_next_round(BaseModeData *mode_data)
{
    if (mode_data == NULL)
        return;

    round_info_next_round(mode_data->round_info);
}

void base_mode_prev_round(BaseModeData *mode_data)
{
    if (mode_data == NULL)
        return;

    round_info_prev_round(mode_data->round_info);
}

void round_info_init(RoundInfo *round_info, int training_rounds_count)
{
    if (round_info == NULL)
        return;

    round_info->_training_rounds_count = training_rounds_count;
    round_info->current_round = 1;
    round_info->training = (training_rounds_count > 0);
}

void base_mode_round_display(BaseModeData *mode_data, unsigned char *target)
{
    if (mode_data == NULL || target == NULL)
        return;

    // there are two digits for round
    if (mode_data->round_info->training)
    {
        target[0] = 'P';
        target[1] = mode_data->round_info->current_round % 10 + '0';
        return;
    }

    // competition round
    sprintf(target, "%2d", mode_data->round_info->current_round % 100);
}

void base_mode_free(void **mode_data)
{
    if (*mode_data == NULL)
        return;
    free(*mode_data);
    *mode_data = NULL;
}

bool round_info_first_competition_round(RoundInfo *round_info)
{
    if (round_info == NULL)
        return false;
    return round_info->current_round == 1 && !round_info->training;
}

bool base_mode_current_timer_is_prep(BaseModeData *mode_data)
{
    if (mode_data == NULL)
        return false;
    return mode_data->current_timer == &mode_data->_prep_timer;
}

void base_mode_display_semaphor(DisplayState *display, BaseModeData *mode_data)
{
    if (display == NULL || mode_data == NULL)
        return;

    // set sempahor color
    if (base_mode_prep_time(mode_data))
        display->semaphor_display = SemaphorDisplayRed;
    else if (base_mode_in_warning_time(mode_data))
        display->semaphor_display = SemaphorDisplayOrange;
    else
        display->semaphor_display = SemaphorDisplayGreen;
}

void base_mode_free_internal(BaseModeData **mode_data)
{
    free((*mode_data)->round_info);
    (*mode_data)->round_info = NULL;
    free(*mode_data);
    *mode_data = NULL;
}

void base_mode_pause(BaseModeData *mode_data)
{
    if (mode_data == NULL)
        return;

    mode_data->paused = true;
    mode_data->running = false;
}

void base_mode_resume(BaseModeData *mode_data)
{
    if (mode_data == NULL)
        return;

    mode_data->paused = false;
    mode_data->running = true;
}

void base_mode_break(BaseModeData *mode_data, int break_time)
{
    if (mode_data == NULL)
        return;

    if (base_mode_running(mode_data)) // do nothing if the mode is running (can't insert a break)
        return;

    mode_data->is_break = true;
    mode_data->_break_timer = break_time;
}

void base_mode_display_break(DisplayState *display, BaseModeData *mode_data)
{
    if (display == NULL || mode_data == NULL)
        return;

    const int current_time = mode_data->_break_timer;
    const int minutes = current_time / 60;
    const int seconds = current_time % 60;
    display->semaphor_display = (current_time % 2) ? SemaphorDisplayOrange : SemaphorDisplayEmpty;
    
    sprintf(display->middle_display, "%s", "--");
    sprintf(display->left_display, "%3d", minutes);
    sprintf(display->right_display, "%3d", seconds);

}