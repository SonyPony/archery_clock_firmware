#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <stdint.h>
#include <lib/peripheral/shift_register.h>

#define LEFT_DISPLAY_SIZE 3   // 3 symbols
#define RIGHT_DISPLAY_SIZE 3  // 3 symbols
#define MIDDLE_DISPLAY_SIZE 2 // 2 symbols

enum SemaphorDisplayState
{
    SemaphorDisplayRed,
    SemaphorDisplayOrange,
    SemaphorDisplayGreen,
    SemaphorDisplayEmpty
};

struct DisplayState
{
    char leftDisplay[LEFT_DISPLAY_SIZE + 1]; // add 1 for null symbol
    char rightDisplay[RIGHT_DISPLAY_SIZE + 1];
    char middleDisplay[MIDDLE_DISPLAY_SIZE + 1];
    SemaphorDisplayState semaphorDisplay;

    /**
     * @brief display_state_clear Set the display state, so it would not display anything.
     */
    void clear();

    void log() const;
};

class DisplayController
{
    private:
        ShiftRegister *m_shift_register;
        DisplayState m_displayState;

    protected:
        /**
         * @brief symbol_to_display_segments Converts an ASCII symbol to a representation for a
         * 7-segment display.
         * @param symbol ASCII symbol. Supported symbols are {0-9, A-D, P}.
         * @return Returns a 7-segment display representation of the symbol.
         */
        static uint8_t symbolToDisplayData(uint8_t symbol);

        // TODO doc
        static uint8_t semaphorToDisplayData(SemaphorDisplayState semaphorState);

    public:
        DisplayController(ShiftRegister* shiftRegister);

        DisplayState* displayState();
        void display();
};

#endif