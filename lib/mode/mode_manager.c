#include "mode_manager.h"
#include <finals.h>
#include <ab.h>
#include <abc.h>
#include <abcd.h>
#include <stdlib.h>

void mode_manager_mode_init(InitializationData *init_data, void **mode_data, BaseModeData **base_mode_data)
{
    if (init_data == NULL)
        return;

    switch (init_data->turn_type)
    {
    case AB_TurnType:
        *mode_data = ab_mode_init(init_data);
        *base_mode_data = ((AbModeData *)(*mode_data))->base;
        break;

    case ABC_TurnType:
        *mode_data = abc_mode_init(init_data);
        *base_mode_data = ((AbcModeData *)(*mode_data))->base;
        break;

    case ABCD_TurnType:
        *mode_data = abcd_mode_init(init_data);
        *base_mode_data = ((AbcdModeData *)(*mode_data))->base;
        break;

    case FinalsIndividual_TurnType:
    case FinalsTeams_TurnType:
        *mode_data = finals_mode_init(init_data);
        *base_mode_data = ((FinalsModeData *)(*mode_data))->base;
        break;

    default:
        break;
    }
}

bool mode_manager_mode_data_valid(void* mode_data, BaseModeData* base_mode_data)
{
  return mode_data != NULL && base_mode_data != NULL;
}