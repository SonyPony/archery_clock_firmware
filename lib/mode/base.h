#ifndef BASE_H
#define BASE_H

#include <stdbool.h>
#include <data_model.h>
#include <display.h>

#define BASE_MODE_HANDLE_NEXT_STEP() { \
    /* prep time can't be skipped */ \
    if(base_mode_running(mode_data->base) && base_mode_prep_time(mode_data->base)) \
        return; \
    if(mode_data->base->paused) { \
        base_mode_resume(mode_data->base); \
        return; \
    } \
    if(mode_data->base->is_break) { \
        mode_data->base->is_break = false; \
        mode_data->base->_break_timer = 0; \
    } \
}

#define BASE_MODE_HANDLE_SEC_TICK() { \
    if(mode_data->base->is_break) { /* handle break */ \
        mode_data->base->_break_timer--; \
        if(mode_data->base->_break_timer <= 0) { \
            mode_data->base->is_break = false; \
            mode_data->base->next_step(mode_data); \
        } \
        return; \
    } \
\
    if(!base_mode_running(mode_data->base)) \
        return; \
    /* handle prep time */ \
    base_mode_decrement_current_time(mode_data->base); \
\
    const bool prep_time_running =  base_mode_prep_time(mode_data->base); \
    if(prep_time_running) \
        return; \
}

#define BASE_MODE_DISPLAY() { \
    if(mode_data->base->is_break) { \
        base_mode_display_break(display, mode_data->base); \
        return; \
    } \
}

typedef struct
{
    int current_round;
    bool training;
    int _training_rounds_count;
} RoundInfo;

void round_info_init(RoundInfo *round_info, int training_rounds_count);
void round_info_prev_round(RoundInfo *round_info);
void round_info_next_round(RoundInfo *round_info);
bool round_info_first_competition_round(RoundInfo *round_info);

typedef void(NextStepCallback_t)(void *);
typedef void(HandleSecTickCallback_t)(void *);
typedef void(PrintCallback_t)(void *);
typedef void(FreeCallback_t)(void **);
typedef void(ResetStateCallback_t)(void *, InitializationData *);
typedef void(DisplayCallback_t)(DisplayState *, void *);

typedef struct
{
    InitializationData *init_data; // in seconds
    int *current_timer;
    bool running;
    bool paused;
    bool is_break;

    RoundInfo *round_info;

    // timers
    int _break_timer;
    int _prep_timer;

    // methods
    NextStepCallback_t *next_step;
    HandleSecTickCallback_t *handle_sec_tic;
    PrintCallback_t *print;
    FreeCallback_t *free;
    ResetStateCallback_t *reset_state;
    DisplayCallback_t *display;
} BaseModeData;

BaseModeData *base_mode_init(
    InitializationData *init_data,
    NextStepCallback_t *next_step_func,
    HandleSecTickCallback_t *handle_sec_tic_func,
    PrintCallback_t *print_func,
    ResetStateCallback_t *reset_state_func,
    DisplayCallback_t *display_func,
    FreeCallback_t *free_func);

/**
 * @brief base_mode_break Starts a break time. Can only be started if the mode is
 * not running. If it's running, it will do nothing.
 * @param break_time Break time in seconds.
*/
void base_mode_break(BaseModeData *mode_data, int break_time);

void base_mode_display_break(DisplayState *display, BaseModeData *mode_data);
void base_mode_pause(BaseModeData *mode_data);
void base_mode_resume(BaseModeData *mode_data);
void base_mode_reset_state(BaseModeData *mode_data, InitializationData *init_data);
void base_mode_restore_prep_time(BaseModeData *mode_data);
void base_mode_set_current_time(BaseModeData *mode_data, int value);
void base_mode_decrement_current_time(BaseModeData *mode_data);
int base_mode_current_time(BaseModeData *mode_data);
void base_mode_set_running(BaseModeData *mode_data, bool running);
bool base_mode_running(BaseModeData *mode_data);
bool base_mode_prep_time(BaseModeData *mode_data);
bool base_mode_in_warning_time(BaseModeData *mode_data);
void base_mode_prev_round(BaseModeData *mode_data);
void base_mode_next_round(BaseModeData *mode_data);
void base_mode_round_display(BaseModeData *mode_data, unsigned char *target);
void base_mode_display_semaphor(DisplayState *display, BaseModeData *mode_data);
void base_mode_free(void **mode_data);
void base_mode_free_internal(BaseModeData **mode_data);
bool base_mode_current_timer_is_prep(BaseModeData *mode_data);

#endif // BASE_H
