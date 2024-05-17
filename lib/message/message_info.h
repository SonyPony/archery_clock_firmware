#ifndef MESSAGE_INFO_H
#define MESSAGE_INFO_H

typedef enum
{
    InitializationMessageType = 0,
    StartMessageType = 1,
    StopMessageType = 2,
    PreviousRoundMessageType = 3,
    NextRoundMessageType = 4,
    PauseMessageType = 5,
    NextStepMessageType = 6,
    BreakMessageType = 7,
    StateMessageType = 'a',
    RoundChangeMessageType = 'b',
    InvalidMessageType = 255,
} MessageType;

typedef struct
{
    int startIdx; // points to the start index in message buffer
    int endIdx;   // points to the end index in message buffer
} MessageInfo;

#endif // MESSAGE_INFO_H
