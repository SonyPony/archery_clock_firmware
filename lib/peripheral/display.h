#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <stdint.h>
#include <shift_register.h>

#define LEFT_DISPLAY_SIZE 3   // 3 symbols
#define RIGHT_DISPLAY_SIZE 3  // 3 symbols
#define MIDDLE_DISPLAY_SIZE 2 // 2 symbols

typedef enum
{
    SemaphorDisplayRed,
    SemaphorDisplayOrange,
    SemaphorDisplayGreen,
    SemaphorDisplayEmpty
} SemaphorDisplayState;

typedef struct
{
    unsigned char left_display[LEFT_DISPLAY_SIZE + 1]; // add 1 for null symbol
    unsigned char right_display[RIGHT_DISPLAY_SIZE + 1];
    unsigned char middle_display[MIDDLE_DISPLAY_SIZE + 1];
    SemaphorDisplayState semaphor_display;
} DisplayState;

typedef struct
{
    ShiftRegister *shift_register;
} DisplayController;

void display_controller_init(DisplayController *display, ShiftRegister *sr);
void display_controller_display(DisplayController *display, DisplayState *state);

/**
 * @brief display_state_clear Set the display state, so it would not display anything.
 * @param display_state Target display state
 */
void display_state_clear(DisplayState *display_state);

/**
 * @brief symbol_to_display_segments Converts an ASCII symbol to a representation for a
 * 7-segment display.
 * @param symbol ASCII symbol. Supported symbols are {0-9, A-D, P}.
 * @return Returns a 7-segment display representation of the symbol.
 */
uint8_t symbol_to_display_segments(unsigned char symbol);

// TODO doc
uint8_t semaphor_to_display_data(SemaphorDisplayState semaphor_state);

void display_state_print(DisplayState *display_state);

#endif