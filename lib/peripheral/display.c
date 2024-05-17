#include "display.h"
#include <stdlib.h>
#include <stdio.h>
#include <logging.h>

uint8_t symbol_to_display_segments(unsigned char symbol)
{
    /**
     *      0
     *   |----|
     * 5 |  6 | 1
     *   |----|
     * 4 |    | 2
     *   |----|
     *      3
     */

    const uint8_t digits_codes[] = {
        0b00111111, // 0
        0b00000110, // 1
        0b01011011, // 2
        0b01001111, // 3
        0b01100110, // 4
        0b01101101, // 5
        0b01111101, // 6
        0b00000111, // 7
        0b01111111, // 8
        0b01101111  // 9
    };

    if (symbol >= '0' && symbol <= '9') // handle digits
        return digits_codes[symbol - '0'];

    switch (symbol)
    { // handle letters
    case 'A':
        return 0b01110111;
    case 'B':
        return 0b01111100;
    case 'C':
        return 0b00111001;
    case 'D':
        return 0b01011110;
    case 'P':
        return 0b01110011;
    case '<':
        return 0b01000110;
    case '>':
        return 0b01110000;
    default:
        return 0; // display off
    }
}

uint8_t semaphor_to_display_data(SemaphorDisplayState semaphor_state)
{
    if (semaphor_state == SemaphorDisplayOrange)
        return 0b00000010;
    else if (semaphor_state == SemaphorDisplayRed)
        return 0b00000100;
    else if (semaphor_state == SemaphorDisplayGreen)
        return 0b00000001;
    else // empty/clear
        return 0b00000000;
}

void display_state_clear(DisplayState *display_state)
{
    if (display_state == NULL)
        return;

    sprintf(display_state->left_display, "%s", "   ");
    sprintf(display_state->right_display, "%s", "   ");
    sprintf(display_state->middle_display, "%s", "  ");
    display_state->semaphor_display = SemaphorDisplayEmpty;
}

void display_controller_init(DisplayController *display, ShiftRegister *sr)
{
    if (display == NULL)
        return;

    display->shift_register = sr;
}

void display_controller_display(DisplayController *display, DisplayState *state)
{
    for (int i = 0; i < LEFT_DISPLAY_SIZE; i++)
        sr_set_data(display->shift_register, symbol_to_display_segments(state->left_display[i]));
    // TODO semaphor
    sr_set_data(display->shift_register, semaphor_to_display_data(state->semaphor_display));

    for (int i = 0; i < MIDDLE_DISPLAY_SIZE; i++)
        sr_set_data(display->shift_register, symbol_to_display_segments(state->middle_display[i]));

    for (int i = 0; i < RIGHT_DISPLAY_SIZE; i++)
        sr_set_data(display->shift_register, symbol_to_display_segments(state->right_display[i]));

    sr_confirm(display->shift_register);
}

void display_state_print(DisplayState *display_state)
{
    if (display_state == NULL)
        return;

    char semaphor = 'G';
    if (display_state->semaphor_display == SemaphorDisplayOrange)
        semaphor = 'O';
    else if (display_state->semaphor_display == SemaphorDisplayRed)
        semaphor = 'R';

    uart_printf(
        "Display(left: \"%s\", middle: \"%s\", right: \"%s\", semaphor: \"%c\"",
        display_state->left_display,
        display_state->middle_display,
        display_state->right_display,
        semaphor);
}