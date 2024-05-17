#ifndef _MODE_MANAGER_H_
#define _MODE_MANAGER_H_

#include <base.h>
#include <data_model.h>

/**
 * @brief mode_manager_mode_init Initialized given mode based on the intialization data.
 * @param mode_data Destination of the mode_data.
 * @param base_mode_data Destination where the base mode data will be stored.
*/
void mode_manager_mode_init(InitializationData *init_data, void** mode_data, BaseModeData** base_mode_data);

/**
 * @brief mode_manager_mode_data_valid Checks if mode_data and base_mode_data are not null.
 * @return Returns true if both arguments are not null. Otherwise it returns false.
*/
bool mode_manager_mode_data_valid(void* mode_data, BaseModeData* base_mode_data);

#endif