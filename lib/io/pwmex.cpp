#include "pwmex.h"
#include <hardware/clocks.h>
#include <hardware/pwm.h>
#include <hardware/timer.h>
#include <pico/stdlib.h>
#include <lib/logging/logging.h>

PWMPin::PWMPin(uint32_t pin) {
    this->m_pin = pin;
    this->m_clockFrequency = clock_get_hz(clk_sys); // in MHz
    this->m_running = false;

    // setup PWM
    this->m_slice = pwm_gpio_to_slice_num(this->m_pin);
    this->m_channel = pwm_gpio_to_channel(this->m_pin);

    this->m_inited = false;
}

PWMPin::PWMPin(uint32_t pin, uint32_t frequency, uint8_t DCL): PWMPin(pin) {
    this->setParameters(frequency, DCL);
}

bool PWMPin::start() {
    if(!this->m_inited) {
        Logging::log(LoggingLevel::Warning, "Can't start, PWM pin %d not setup.\n", this->m_pin);
        return false;
    }

    pwm_set_enabled(this->m_slice, true);
    this->m_running = true;

    return true;
}

bool PWMPin::stop() {
    if(!this->m_inited) {
        Logging::log(LoggingLevel::Warning, "Can't stop, PWM pin %d not setup.\n", this->m_pin);
        return false;
    }

    pwm_set_enabled(this->m_slice, false);
    gpio_put(this->m_pin, false);
    this->m_running = false;

    return true;
}

void PWMPin::setParameters(uint32_t frequency, uint8_t DCL) {
    // how many cycles for one period
    const uint32_t wrap = this->m_clockFrequency / frequency; 

    // how many cycles is it 1?
    const uint32_t level = (wrap * DCL) / 100;

    pwm_set_wrap(this->m_slice, wrap);
    pwm_set_chan_level(this->m_slice, this->m_channel, level);
    this->m_inited = true;
}

bool PWMPin::running() const {
    return this->m_running;
}