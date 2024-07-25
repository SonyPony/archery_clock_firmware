#ifndef DATA_MODEL_H
#define DATA_MODEL_H

#include <lib/message/message_info.h>
#include <lib/mode/round_info.h>
#include <cstdint>

#define PREP_TIME 10    // in seconds

enum TurnType
{
    AB_TurnType,
    ABC_TurnType,
    ABCD_TurnType,
    FinalsTeams_TurnType,
    FinalsIndividual_TurnType,
    None_TurnType,
};

struct BaseCommand
{
    MessageType type;

    virtual ~BaseCommand() = default;
    virtual const char* toBytes() { return nullptr; }
    virtual uint32_t bytesCount() const { return 0; }
};

struct InitializationCommand: public BaseCommand
{
    int time_per_round;
    int warning_time;
    int training_rounds_count;
    TurnType turn_type;
    int prep_time;
    int turns_per_round; // A -> B -> A -> B -> ROUND 2 -> 2 turns per round;
};

struct BreakCommand: public BaseCommand
{
    int break_time;
};

struct RoundChangeCommandInfo: public BaseCommand
{
    // <b{1:isTraining}{2:round}> -> 6 len
    private:
        static constexpr uint32_t m_messageLen = 6;
        char m_buffer[m_messageLen + 1];
        RoundInfo m_roundInfo;

    public:
        RoundChangeCommandInfo(RoundInfo roundInfo);

        const char* toBytes() override;
        uint32_t bytesCount() const override;
};

struct ModeChangeCommandInfo: public BaseCommand
{
    // <d<AB- | ABC | ABD | F-I | F-T | ---> 6 -> len
    private:
        static constexpr uint32_t m_messageLen = 6;
        char m_buffer[m_messageLen + 1];
        TurnType m_modeType;

    public:
        ModeChangeCommandInfo(TurnType modeType);

        const char* toBytes() override;
        uint32_t bytesCount() const override;
};

#endif // RUN_DATA_H
