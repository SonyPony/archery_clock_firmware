#ifndef DATA_MODEL_H
#define DATA_MODEL_H

#define PREP_TIME 10    // in seconds

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
} InitializationCommand;

typedef struct
{
    int break_time;
} BreakCommand;

#endif // RUN_DATA_H
