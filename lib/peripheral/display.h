#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <stdint.h>
#include <lib/peripheral/shift_register.h>


enum SemaphorDisplayState
{
    SemaphorDisplayRed,
    SemaphorDisplayOrange,
    SemaphorDisplayGreen,
    SemaphorDisplayEmpty
};

struct DisplayState
{
    static constexpr uint32_t leftDisplaySize = 3;      // 3 symbols
    static constexpr uint32_t rightDisplaySize = 3;     // 3 symbols
    static constexpr uint32_t middleDisplaySize = 2;    // 2 symbols

    char leftDisplay[DisplayState::leftDisplaySize + 1]; // add 1 for null symbol
    char rightDisplay[DisplayState::rightDisplaySize + 1];
    char middleDisplay[DisplayState::middleDisplaySize + 1];
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