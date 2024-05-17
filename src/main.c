#include "stm8s.h"
#include "stm8s_it.h"
#include <stm8s_gpio.h>
#include <stm8s_tim4.h>
#include <stm8s_itc.h>
#include <stm8s_clk.h>
#include <milis.h>
#include <port_linked_pin.h>
#include <gpioex.h>
#include <buffer.h>
#include <shift_register.h>
#include <display.h>
#include <message_info.h>
#include <stdlib.h>
#include <parser.h>
#include <string.h> // TODO delete?
#include <uart.h>
#include <stdarg.h> // TODO delete?
#include <stdio.h>  // TODO delete?

// message buffer
#define MESSAGE_BUFFER_SIZE 512
volatile uint8_t message_buffer_data[MESSAGE_BUFFER_SIZE];
volatile int message_buffer_data_end_idx = 0;
Buffer message_buffer = {.data = message_buffer_data, .size = MESSAGE_BUFFER_SIZE, .data_end_idx = &message_buffer_data_end_idx};

// UART channels
#define PC_UART_CHANNEL 1
#define WIFI_UART_CHANNEL 3

// ports definition
PortLinkedPin led_pin = {.port = GPIOC, .pin = GPIO_PIN_5};
const PortLinkedPin sr_clk_pin = {.port = GPIOD, .pin = GPIO_PIN_0};
const PortLinkedPin sr_data_pin = {.port = GPIOD, .pin = GPIO_PIN_3};
const PortLinkedPin sr_cs_pin = {.port = GPIOD, .pin = GPIO_PIN_2};
const PortLinkedPin beeper_pin = {.port = GPIOB, .pin = GPIO_PIN_5};

// TODO move to display
// display definitions
DisplayState display_state;
ShiftRegister shift_register;
DisplayController display_controller;

uint8_t buffer[MESSAGE_BUFFER_SIZE];
void uart_printf(const char* format, ...) {
  memset(buffer, 0, MESSAGE_BUFFER_SIZE);

  va_list args;
  va_start(args, format);
  vsprintf(buffer, format, args);
  va_end(args);

  uart_send(PC_UART_CHANNEL, buffer);
}

void initialized_mcu()
{
  GPIOex_Init(&led_pin, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIOex_Init(&beeper_pin, GPIO_MODE_OUT_PP_LOW_SLOW);

  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // 16MHz from internal RC oscillator
  init_milis();

  // communication channels init
  uart_init(PC_UART_CHANNEL, 115200);
  uart_init(WIFI_UART_CHANNEL, 115200);

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

  // TODO delete dummy
  const uint8_t message_stream[] = "asd<a<1>fg<001000302F-I> <700300>s";
  strncpy(message_buffer.data, message_stream, strlen(message_stream));
  message_buffer_data_end_idx = strlen(message_stream);

  MessageInfo msg_info;

  while (1)
  {
    parse_message_info(&message_buffer, &msg_info);
    if (!message_info_valid(&msg_info))
      continue;

    i_var++;
    if (milis() - last_time > 1000)
    {

      last_time = milis();
      GPIOex_WriteReverse(&led_pin);
    }
  }

  return 0;
}

INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
{
  uart_read(PC_UART_CHANNEL, &message_buffer);
}

INTERRUPT_HANDLER(UART3_RX_IRQHandler, 21)
{
  // TODO receive data
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