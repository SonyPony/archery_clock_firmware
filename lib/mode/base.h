#ifndef BASE_H
#define BASE_H

#include <stdbool.h>
#include <lib/data/run_data.h>
#include <lib/display/utils.h>

typedef struct
{
    int current_round;
    bool training;
    int _training_rounds_count;
} RoundInfo;

void round_info_init(RoundInfo *round_info, int training_rounds_count);
void round_info_prev_round(RoundInfo *round_info);
void round_info_next_round(RoundInfo *round_info);
bool round_info_first_competition_round(RoundInfo round_info);

typedef struct
{
    InitializationData init_data; // in seconds
    int *current_timer;
    bool running;

    RoundInfo round_info;

    // prep timer
    int _prep_timer;

    // methods
    void (*next_step)(void *);
    void (*handle_sec_tic)(void *);
    void (*print)(void *);
    void (*free)(void **);
    void (*reset_state)(void *, InitializationData);
    void (*display)(DisplayState *, void *);
} BaseModeData;

void base_mode_init(
    BaseModeData *mode_data,
    InitializationData init_data,
    void (*next_step_func)(void *),
    void (*handle_sec_tic_func)(void *),
    void (*print_func)(void *),
    void (*reset_state_func)(void *, InitializationData),
    void (*display)(DisplayState *, void *));
void base_mode_reset_state(BaseModeData *mode_data, InitializationData init_data);

void base_mode_restore_prep_time(BaseModeData *mode_data);
void base_mode_set_current_time(BaseModeData *mode_data, int value);
void base_mode_decrement_current_time(BaseModeData *mode_data);
int base_mode_current_time(BaseModeData mode_data);
void base_mode_set_running(BaseModeData *mode_data, bool running);
bool base_mode_running(BaseModeData mode_data);
bool base_mode_prep_time(BaseModeData mode_data);
bool base_mode_in_warning_time(BaseModeData mode_data);
void base_mode_prev_round(BaseModeData *mode_data);
void base_mode_next_round(BaseModeData *mode_data);
void base_mode_round_display(BaseModeData *mode_data, unsigned char *target);
void base_mode_display_semaphor(DisplayState *display, BaseModeData mode_data);
void base_mode_free(void **mode_data);
bool base_mode_current_timer_is_prep(BaseModeData *mode_data);

#endif // BASE_H
