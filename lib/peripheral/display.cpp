#include "display.h"
#include <stdlib.h>
#include <stdio.h>
#include <lib/logging/logging.h>


void DisplayState::clear()
{
    sprintf(this->leftDisplay, "%s", "   ");
    sprintf(this->rightDisplay, "%s", "   ");
    sprintf(this->middleDisplay, "%s", "  ");
    this->semaphorDisplay = SemaphorDisplayEmpty;
}

void DisplayState::log() const
{
    char semaphor = '/';
    if (this->semaphorDisplay == SemaphorDisplayOrange)
        semaphor = 'O';
    else if (this->semaphorDisplay == SemaphorDisplayRed)
        semaphor = 'R';
    else if (this->semaphorDisplay == SemaphorDisplayGreen)
        semaphor = 'G';

    Logging::log(
        "Display(left: \"%s\", middle: \"%s\", right: \"%s\", semaphor: \"%c\")",
        this->leftDisplay,
        this->middleDisplay,
        this->rightDisplay,
        semaphor);
}

DisplayController::DisplayController(ShiftRegister *shiftRegister)
{
    this->m_shift_register = shiftRegister;
    this->m_displayState.clear();
}

DisplayState *DisplayController::displayState()
{
    return &this->m_displayState;
}

void DisplayController::display()
{
    for (int i = 0; i < LEFT_DISPLAY_SIZE; i++)
        this->m_shift_register->setData(DisplayController::symbolToDisplayData(m_displayState.leftDisplay[i]));

    this->m_shift_register->setData(DisplayController::semaphorToDisplayData(m_displayState.semaphorDisplay));

    for (int i = 0; i < MIDDLE_DISPLAY_SIZE; i++)
        this->m_shift_register->setData(DisplayController::symbolToDisplayData(m_displayState.middleDisplay[i]));

    for (int i = 0; i < RIGHT_DISPLAY_SIZE; i++)
        this->m_shift_register->setData(DisplayController::symbolToDisplayData(m_displayState.rightDisplay[i]));

    this->m_shift_register->confirm();
}

uint8_t DisplayController::symbolToDisplayData(uint8_t symbol)
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
    case '-':
        return 0b01000000;
    default:
        return 0; // display off
    }
}

uint8_t DisplayController::semaphorToDisplayData(SemaphorDisplayState semaphorState)
{
    if (semaphorState == SemaphorDisplayOrange)
        return 0b00000010;
    else if (semaphorState == SemaphorDisplayRed)
        return 0b00000100;
    else if (semaphorState == SemaphorDisplayGreen)
        return 0b00000001;
    else // empty/clear
        return 0b00000000;
}