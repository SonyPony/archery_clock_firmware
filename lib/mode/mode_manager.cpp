#include "mode_manager.h"
#include <stdlib.h>
#include <lib/message/parser.h>
#include <string.h>

void mode_manager_init(
    ModeManager *mode_manager,
    BaseModeData *base_mode_data,
    AbModeData *ab_mode_data,
    AbcModeData *abc_mode_data,
    AbcdModeData *abcd_mode_data,
    FinalsModeData *finals_mode_data,
    InitializationCommand *init_data,
    RoundInfo *round_info)
{
    if (mode_manager == NULL || base_mode_data == NULL || ab_mode_data == NULL || abc_mode_data == NULL || abcd_mode_data == NULL || finals_mode_data == NULL || init_data == NULL || round_info == NULL)
        return;

    mode_manager->_ab_mode_data = ab_mode_data;
    mode_manager->_abc_mode_data = abc_mode_data;
    mode_manager->_abcd_mode_data = abcd_mode_data;
    mode_manager->_finals_mode_data = finals_mode_data;
    mode_manager->base_mode_data = base_mode_data;
    mode_manager->mode_data = NULL;
    mode_manager->init_data = init_data;
    mode_manager->round_info = round_info;
}

void mode_manager_mode_init(ModeManager *mode_manager, InitializationCommand *init_data)
{
    if (init_data == NULL || mode_manager == NULL)
        return;

    mode_manager->init_data = init_data;    // TODO check
    //if (mode_manager->init_data == NULL)
    //    return;

    // copy init data TODO delete
    // memcpy(mode_manager->init_data, init_data, sizeof(InitializationCommand));

    switch (init_data->turn_type)
    {
    case AB_TurnType:
        ab_mode_init(mode_manager->_ab_mode_data, mode_manager->base_mode_data, mode_manager->round_info, mode_manager->init_data);
        mode_manager->mode_data = mode_manager->_ab_mode_data;
        mode_manager->base_mode_data = ((AbModeData *)(mode_manager->mode_data))->base;
        break;

    case ABC_TurnType:
        abc_mode_init(mode_manager->_abc_mode_data, mode_manager->base_mode_data, mode_manager->round_info, mode_manager->init_data);
        mode_manager->mode_data = mode_manager->_abc_mode_data;
        mode_manager->base_mode_data = ((AbcModeData *)(mode_manager->mode_data))->base;
        break;

    case ABCD_TurnType:
        abcd_mode_init(mode_manager->_abcd_mode_data, mode_manager->base_mode_data, mode_manager->round_info, mode_manager->init_data);
        mode_manager->mode_data = mode_manager->_abcd_mode_data;
        mode_manager->base_mode_data = ((AbcdModeData *)(mode_manager->mode_data))->base;
        break;

    case FinalsIndividual_TurnType:
    case FinalsTeams_TurnType:
        finals_mode_init(mode_manager->_finals_mode_data, mode_manager->base_mode_data, mode_manager->round_info, mode_manager->init_data);
        mode_manager->mode_data = mode_manager->_finals_mode_data;
        mode_manager->base_mode_data = ((FinalsModeData *)(mode_manager->mode_data))->base;
        break;

    default:
        break;
    }
}

bool mode_manager_mode_data_valid(ModeManager *mode_manager)
{
    if (mode_manager == NULL)
        return false;
    return mode_manager->mode_data != NULL && mode_manager->base_mode_data != NULL;
}

void mode_manager_process_commands(ModeManager *mode_manager, void *command)
{
    if (mode_manager == NULL || command == NULL)
        return;

    MessageType command_type = ((BaseCommand *)command)->type;

    if (command_type == InitializationMessageType)
    {
        // remove old mode data TODO check
        if (mode_manager_mode_data_valid(mode_manager))
        {
            //mode_manager->base_mode_data->free(&mode_manager->mode_data);
            //mode_manager->base_mode_data = NULL;
            //mode_manager->mode_data = NULL;
        }
        mode_manager_mode_init(mode_manager, (InitializationCommand *)command);
    }

    else
    {
        if (!mode_manager_mode_data_valid(mode_manager)) // check valid mode structures
            return;

        void *mode_data = mode_manager->mode_data;
        BaseModeData *base_mode_data = mode_manager->base_mode_data;

        switch (command_type) // handle commands
        {
        case NextStepMessageType:
        case StartMessageType:
            base_mode_data->next_step(mode_data);
            break;

        case StopMessageType:
            base_mode_data->running = false;
            break;

        case PreviousRoundMessageType:
            base_mode_prev_round(base_mode_data);
            break;

        case NextRoundMessageType:
            base_mode_next_round(base_mode_data);
            break;

        case PauseMessageType:
            base_mode_pause(base_mode_data);
            break;

        case BreakMessageType:
            base_mode_break(base_mode_data, ((BreakCommand *)command)->break_time);
            break;
        }
    }
}