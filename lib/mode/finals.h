#ifndef FINALS_H
#define FINALS_H

#include <lib/data/data_model.h>
#include <lib/mode/base.h>

enum FinalsTurns
{
    FinalsLeftTurn,
    FinalsRightTurn,
};

class FinalsModeData: public BaseModeData
{
    private:
        FinalsTurns m_currentTurn;
        int m_leftCurrentTime;        // in seconds
        int m_rightCurrentTime;       // in seconds
        int m_currentTurnThisRound; // number of turns that have were this round
        bool m_resetTimeAfterTurn;

    public:
        FinalsModeData();

        bool nextStep() override;
        bool handleSecTick() override;
        void log() const override;
        bool display(DisplayState* displayState) const override;
        void resetState(InitializationCommand initData) override;

        void nextTurn();
        void endRound();
};

#endif // FINALS_H
