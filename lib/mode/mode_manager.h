#ifndef _MODE_MANAGER_H_
#define _MODE_MANAGER_H_

#include <lib/core/buffer.h>
#include <lib/data/data_model.h>
#include <lib/mode/finals.h>
#include <lib/mode/ab.h>
#include <lib/mode/abc.h>
#include <lib/mode/abcd.h>

typedef struct
{
    void *mode_data;
    AbModeData *_ab_mode_data;
    AbcModeData *_abc_mode_data;
    AbcdModeData *_abcd_mode_data;
    FinalsModeData *_finals_mode_data;
    BaseModeData *base_mode_data;
    InitializationCommand *init_data;
    RoundInfo *round_info;
} ModeManager;

void mode_manager_init(
    ModeManager *mode_manager,
    BaseModeData *base_mode_data,
    AbModeData *ab_mode_data,
    AbcModeData *abc_mode_data,
    AbcdModeData *abcd_mode_data,
    FinalsModeData *finals_mode_data,
    InitializationCommand *init_data,
    RoundInfo *round_info);

/**
 * @brief mode_manager_mode_init Initialized given mode based on the intialization data.
 * @param mode_data Destination of the mode_data.
 * @param base_mode_data Destination where the base mode data will be stored.
 */
void mode_manager_mode_init(ModeManager *mode_manager, InitializationCommand *init_data);

/**
 * @brief mode_manager_mode_data_valid Checks if mode_data and base_mode_data are not null.
 * @return Returns true if both arguments are not null. Otherwise it returns false.
 */
bool mode_manager_mode_data_valid(ModeManager *mode_manager);

/**
 * @brief mode_manager_process_commands Based on the command it modifies the state of the running mode.
 * @param mode_manager Target mode manager.
 * @param command Pointer to either Command, BreakCommand or InitializationCommand. Does not free the space
 * allocated for command.
 */
void mode_manager_process_commands(ModeManager *mode_manager, void *command);

#endif