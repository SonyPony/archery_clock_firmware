#ifndef ROUND_INFO_H
#define ROUND_INFO_H

class RoundInfo
{
    private:
        int m_trainingRoundsCount;
        int m_currentRound;
        bool m_isTraining;

    public:
        RoundInfo(int trainingRoundsCount = 0);

        bool isTraining() const;
        int currentRound() const;
        bool isFirstCompetitionRound() const;

        void setPreviousRound();
        void setNextRound();

        bool operator==(const RoundInfo& other) const;
};

#endif
