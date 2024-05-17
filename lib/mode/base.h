#ifndef BASE_H
#define BASE_H

#include <stdbool.h>
#include <data_model.h>
#include <display.h>

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

    RoundInfo *round_info;

    // prep timer
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
