#include "stm8s.h"
#include "stm8s_it.h"
#include <stm8s_gpio.h>
#include <stm8s_tim4.h>
#include <stm8s_itc.h>
#include <stm8s_clk.h>
#include <stm8s_uart3.h>
#include <milis.h>
#include <port_linked_pin.h>
#include <gpioex.h>
#include <buffer.h>
#include <shift_register.h>
#include <display.h>

// message buffer
#define MESSAGE_BUFFER_SIZE 128
volatile uint8_t message_buffer_data[128];
Buffer message_buffer = {.data = message_buffer_data, .size = MESSAGE_BUFFER_SIZE};

// ports definition
const PortLinkedPin led_pin = {.port = GPIOC, .pin = GPIO_PIN_5};
const PortLinkedPin sr_clk_pin = {.port = GPIOD, .pin = GPIO_PIN_0};
const PortLinkedPin sr_data_pin = {.port = GPIOD, .pin = GPIO_PIN_3};
const PortLinkedPin sr_cs_pin = {.port = GPIOD, .pin = GPIO_PIN_2};
const PortLinkedPin beeper_pin = {.port = GPIOB, .pin = GPIO_PIN_5};

// TODO move to display
// display definitions
DisplayState display_state;
ShiftRegister shift_register;
DisplayController display_controller;

void initialized_mcu()
{
  GPIOex_Init(&led_pin, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIOex_Init(&beeper_pin, GPIO_MODE_OUT_PP_LOW_SLOW);

  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // 16MHz from internal RC oscillator
  init_milis();

  UART3_Init(115200, UART3_WORDLENGTH_8D, UART3_STOPBITS_1, UART3_PARITY_NO, UART3_MODE_TXRX_ENABLE);
  UART3_ITConfig(UART3_IT_RXNE_OR, ENABLE);
  UART3_Cmd(ENABLE);

  enableInterrupts();

  // init shift register controller
  sr_init(&shift_register, &sr_clk_pin, &sr_data_pin, &sr_cs_pin);

  // init display
  display_controller_init(&display_controller, &shift_register);
  display_state_clear(&display_state);
  display_controller_display(&display_controller, &display_state);

  // clear message buffer
  buffer_clear(&message_buffer);
}


// main function with loop
int main(void)
{
  initialized_mcu();

  uint8_t i_var = 1;
  uint16_t last_time = 0;
  uint8_t io = 255;
  io++;

  while (1)
  {

    i_var++;
    if (milis() - last_time > 1000)
    {
      last_time = milis();
      GPIOex_WriteReverse(&led_pin);
    }
  }

  return 0;
}

#ifdef USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *   where the assert_param error has occurred.
 * @param file: pointer to the source file name
 * @param line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif