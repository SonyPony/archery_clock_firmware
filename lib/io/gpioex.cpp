#include "gpioex.h"

GPIOPin::GPIOPin(uint32_t pin, enum GPIOMode mode, bool defaultValue)
{
    this->m_pin = pin;
    gpio_init(pin);
    gpio_set_dir(pin, static_cast<decltype(GPIO_OUT)>(mode));

    if (mode == GPIOMode::GPIOOut)
        this->write(defaultValue);
}

void GPIOPin::write(bool value)
{
    gpio_put(m_pin, value);
}

void GPIOPin::writeHigh()
{
    this->write(true);
}

void GPIOPin::writeLow()
{
    this->write(false);
}