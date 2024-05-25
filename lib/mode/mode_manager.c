#include <mode_manager.h>
#include <finals.h>
#include <ab.h>
#include <abc.h>
#include <abcd.h>
#include <stdlib.h>
#include <parser.h>

void mode_manager_init(ModeManager *mode_manager)
{
    if (mode_manager == NULL)
        return;

    mode_manager->base_mode_data = NULL;
    mode_manager->mode_data = NULL;
}

void mode_manager_mode_init(ModeManager *mode_manager, InitializationCommand *init_data)
{
    if (init_data == NULL || mode_manager == NULL)
        return;

    switch (init_data->turn_type)
    {
    case AB_TurnType:
        mode_manager->mode_data = ab_mode_init(init_data);
        mode_manager->base_mode_data = ((AbModeData *)(mode_manager->mode_data))->base;
        break;

    case ABC_TurnType:
        mode_manager->mode_data = abc_mode_init(init_data);
        mode_manager->base_mode_data = ((AbcModeData *)(mode_manager->mode_data))->base;
        break;

    case ABCD_TurnType:
        mode_manager->mode_data = abcd_mode_init(init_data);
        mode_manager->base_mode_data = ((AbcdModeData *)(mode_manager->mode_data))->base;
        break;

    case FinalsIndividual_TurnType:
    case FinalsTeams_TurnType:
        mode_manager->mode_data = finals_mode_init(init_data);
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
        // remove old mode data
        if (mode_manager_mode_data_valid(mode_manager))
        {
            mode_manager->base_mode_data->free(&mode_manager->mode_data);
            mode_manager->base_mode_data = NULL;
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
            base_mode_break(base_mode_data, ((BreakCommand*)command)->break_time);
            break;
        }
    }
}