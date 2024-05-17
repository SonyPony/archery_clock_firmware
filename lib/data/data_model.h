#ifndef DATA_MODEL_H
#define DATA_MODEL_H

typedef enum
{
    AB_TurnType,
    ABC_TurnType,
    ABCD_TurnType,
    FinalsTeams_TurnType,
    FinalsIndividual_TurnType,
} TurnType;

typedef struct
{
    int time_per_round;
    int warning_time;
    int training_rounds_count;
    TurnType turn_type;
    int prep_time;
    int turns_per_round; // A -> B -> A -> B -> ROUND 2 -> 2 turns per round;
} InitializationData;

typedef struct
{
    int break_time;
} BreakData;

#endif // RUN_DATA_H
