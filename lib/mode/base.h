#ifndef BASE_H
#define BASE_H

#include <lib/data/commands.h>
#include <lib/peripheral/display.h>
#include "round_info.h"
#include <functional>


class BaseModeData
{
    private:
        InitializationCommand m_initData;
        bool m_running;
        bool m_paused;
        bool m_isBreak;
        RoundInfo m_roundInfo;

        // timers
        int *m_currentTimer;
        int m_breakTimer;
        int m_prepTimer;

    protected:
        void displayRound(char* targetBuffer) const;
        void displaySemaphor(DisplayState* displayState) const;
        void displayBreak(DisplayState* displayState) const;

    public:
        std::function<void()> prepStartCallback;
        std::function<void()> prepEndCallback;
        std::function<void(bool)> runningChangeCallback;
        std::function<void(RoundInfo)> roundChangeCallback; 
        std::function<void(bool)> pausedChangeCallback; 
        BaseModeData();

        void restorePrepTimer();

        // TODO doc
        virtual bool nextStep();
        virtual bool handleSecTick();
        virtual void log() const = 0;
        virtual void resetState(InitializationCommand initData);
        virtual bool display(DisplayState* displayState) const;

        void pause();
        void resume();

        /**
         * @brief startBreak Starts a break time. Can only be started if the mode is
         * not running. If it's running, it will do nothing.
         * @param break_time Break time in seconds.
        */
        void startBreak(int breakTime);

        void setRunning(bool running);
        void setNextRound();
        void setPreviousRound();
        void decrementCurrentTime();
        void setCurrentTimer(int* newTimer);

        bool paused() const;
        bool running() const;
        int currentTime() const;
        bool isPrepTime() const;
        bool isWarningTime() const;
        bool currentTimerIsPrepTimer() const;
        InitializationCommand initializationData() const;
        RoundInfo roundInfo() const;
        bool isBreak() const;
} ;

#endif // BASE_H
