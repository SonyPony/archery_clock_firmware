#ifndef ABCD_H
#define ABCD_H

#include <lib/mode/base.h>

enum AbcdTurn
{
    AbTurn,
    CdTurn
};

class AbcdModeData: public BaseModeData {
    private:
        int m_currentTime;
        AbcdTurn m_currentTurn;
        int m_currentTurnThisRound;

    public:
        AbcdModeData();

        bool nextStep() override;
        bool handleSecTick() override;
        void log() const override;
        bool display(DisplayState* displayState) const override;
        void resetState(InitializationCommand initData) override;

        void nextTurn();
};

#endif // ABCD_H
