#ifndef _MODE_MANAGER_H_
#define _MODE_MANAGER_H_

#include <lib/core/buffer.h>
#include <lib/data/data_model.h>
#include <lib/mode/finals.h>
#include <lib/mode/ab.h>
#include <lib/mode/abc.h>
#include <lib/mode/abcd.h>

class ModeManager
{
    private:
        BaseModeData* m_currentMode;
        AbModeData m_abModeData;
        AbcModeData m_abcModeData;
        AbcdModeData m_abcdModeData;
        FinalsModeData m_finalsModeData;

    public:
        ModeManager();

        /**
         * @brief initMode Initialized given mode based on the intialization data.
         */
        void initMode(InitializationCommand initData);

        /**
         * @brief modeDataValid Checks if mode_data and base_mode_data are not null.
         * @return Returns true if both arguments are not null. Otherwise it returns false.
         */
        bool modeDataValid() const;

        /**
         * @brief processCommand Based on the command it modifies the state of the running mode.
         * @param command Pointer to either Command, BreakCommand or InitializationCommand. Does not free the space
         * allocated for command.
         */
        void processCommand(const BaseCommand* command); 

        BaseModeData* currentMode() const;
};

#endif