#ifndef AB_H
#define AB_H

#include <lib/mode/base.h>
#include <lib/data/data_model.h>

class AbModeData: public BaseModeData
{
    private:
        int m_currentTime;

    public:
        AbModeData();

        bool nextStep() override;
        bool handleSecTick() override;
        void log() const override;
        bool display(DisplayState* displayState) const override;
        void resetState(InitializationCommand initData) override;
};

void ab_mode_next_step(AbModeData *mode_data);
void ab_mode_handle_sec_tick(AbModeData *mode_data);
void ab_mode_print(AbModeData *mode_data);
void ab_mode_reset_state(AbModeData *mode_data, InitializationCommand *init_data);
void ab_mode_display(DisplayState *display, AbModeData *mode_data);

#endif // AB_H
