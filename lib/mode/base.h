#ifndef BASE_H
#define BASE_H

#include <lib/data/data_model.h>
#include <lib/peripheral/display.h>

#define BASE_MODE_HANDLE_NEXT_STEP() { \
    /* prep time can't be skipped */ \
    if(base_mode_running(mode_data->base) && base_mode_prep_time(mode_data->base)) \
        return; \
    if(mode_data->base->paused) { \
        base_mode_resume(mode_data->base); \
        return; \
    } \
    if(mode_data->base->is_break) { \
        mode_data->base->is_break = false; \
        mode_data->base->_break_timer = 0; \
    } \
}

#define BASE_MODE_HANDLE_SEC_TICK() { \
    if(mode_data->base->is_break) { /* handle break */ \
        mode_data->base->_break_timer--; \
        if(mode_data->base->_break_timer <= 0) { \
            mode_data->base->is_break = false; \
            mode_data->base->next_step(mode_data); \
        } \
        return; \
    } \
\
    if(!base_mode_running(mode_data->base)) \
        return; \
    /* handle prep time */ \
    base_mode_decrement_current_time(mode_data->base); \
\
    const bool prep_time_running =  base_mode_prep_time(mode_data->base); \
    if(prep_time_running) \
        return; \
}

#define BASE_MODE_DISPLAY() { \
    if(mode_data->base->is_break) { \
        base_mode_display_break(display, mode_data->base); \
        return; \
    } \
}

class RoundInfo
{
    private:
        int m_trainingRoundsCount;
        int m_currentRound;
        bool m_isTraining;

    public:
        RoundInfo(int trainingRoundsCount);

        bool isTraining() const;
        int currentRound() const;
        bool isFirstCompetitionRound() const;

        void setPreviousRound();
        void setNextRound();
};

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
        BaseModeData(InitializationCommand initData);

        void resetState(InitializationCommand initData);
        void restorePrepTimer();

        virtual void nextStep();
        virtual void handleSecTick();
        virtual void log();
        virtual void resetState();
        virtual void display(DisplayState* displayState);

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

        bool running() const;
        int currentTime() const;
        bool isPrepTime() const;
        bool isWarningTime() const;
        bool currentTimerIsPrepTimer() const;
} ;

#endif // BASE_H
