#include "beeper.h"
#include "pico/stdlib.h"

Beeper::Beeper(uint32_t beeperPin): m_beeperPin(beeperPin, GPIOMode::GPIOOut, false) 
{}

void Beeper::beep(uint8_t beepCount) {
    for(uint8_t i = 0; i < beepCount; i++) {
        for(int j = 0; j < 250; j++) {  // 250 PWM pulses, TODO parametrize to duration
            this->m_beeperPin.writeHigh();
            sleep_us(63);
            this->m_beeperPin.writeLow();
            sleep_us(63);
        }
        sleep_ms(100);
    }
}