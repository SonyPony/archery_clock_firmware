#include <stm8s.h>
#include <stm8s_it.h>
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
#include <logging.h>
#include <mode_manager.h>
#include <delay.h>
#include <finals.h>
#include <ab.h>
#include <abc.h>
#include <abcd.h>

// message buffer
#define MESSAGE_BUFFER_SIZE 512
/*volatile uint8_t message_buffer_data[MESSAGE_BUFFER_SIZE];
volatile int message_buffer_data_end_idx = 0;
volatile Buffer message_buffer = {.data = message_buffer_data, .size = MESSAGE_BUFFER_SIZE, .data_end_idx = &message_buffer_data_end_idx};*/

/*BreakCommand break_command; 
InitializationCommand init_command; 
BaseCommand base_command;

// UART channels
#define WIFI_UART_CHANNEL 3

// ports definition
PortLinkedPin led_pin = {.port = GPIOC, .pin = GPIO_PIN_5};
const PortLinkedPin sr_clk_pin = {.port = GPIOD, .pin = GPIO_PIN_0};
const PortLinkedPin sr_data_pin = {.port = GPIOD, .pin = GPIO_PIN_3};
const PortLinkedPin sr_cs_pin = {.port = GPIOD, .pin = GPIO_PIN_2};
const PortLinkedPin beeper_pin = {.port = GPIOB, .pin = GPIO_PIN_5};*/

// display definitions
/*DisplayState display_state;
ShiftRegister shift_register;
DisplayController display_controller;

// mode handling
ModeManager mode_manager;
AbModeData ab_mode_data;
AbcModeData abc_mode_data;
AbcdModeData abcd_mode_data;
FinalsModeData finals_mode_data;
BaseModeData base_mode_data;
RoundInfo round_info;*/

void initialized_mcu()
{
  //GPIOex_Init(&led_pin, GPIO_MODE_OUT_PP_LOW_SLOW);
  //GPIOex_Init(&beeper_pin, GPIO_MODE_OUT_PP_LOW_SLOW);

  //CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // 16MHz from internal RC oscillator
  //init_milis();

  // clear message buffer
  //buffer_clear(&message_buffer);

  // communication channels init
  uart_init(PC_UART_CHANNEL, 115200);
  //uart_init(WIFI_UART_CHANNEL, 115200);

  //enableInterrupts();

  // init shift register controller
  //sr_init(&shift_register, &sr_clk_pin, &sr_data_pin, &sr_cs_pin);

  // init display
  //display_controller_init(&display_controller, &shift_register);
  //display_state_clear(&display_state);
  //display_controller_display(&display_controller, &display_state);

  // modes handling
  //mode_manager_init(&mode_manager, &base_mode_data, &ab_mode_data, &abc_mode_data, &abcd_mode_data, &finals_mode_data, &init_command, &round_info);
}

// main function with loop
int main(void)
{
  //initialized_mcu();

  /*int i = 0; // TODO delete
  uint16_t last_time = 0;
  void *command = NULL;
  int j = 0;*/
  int i = 0;
  int j = 0;
  uart_init(PC_UART_CHANNEL, 115200);
  enableInterrupts();

  GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_SLOW);

  // TODO delete dummy
  //const uint8_t message_stream[] = "<012003003ABD><6>"; //"asd<a<1>fg<001000302AB-> <700300>s";
  //strncpy(message_buffer.data, message_stream, strlen(message_stream));
  //message_buffer_data_end_idx = strlen(message_stream);
  // test2();
  

  while (1)
  {
    // clock functionality
    for(j = 0; j < 1000; j++)
      _delay_us(1000);
    //if (milis() - last_time > 1000) // ticking second
    {
      // parse input messages
      //disableInterrupts();
      //command = parse_message(&message_buffer, &break_command, &init_command, &base_command);
      //enableInterrupts();

      // log("%06d: Buffer: %s\n\r", i, message_buffer.data);
      //last_time = milis();
      //GPIOex_WriteReverse(&led_pin);
      GPIO_WriteReverse(GPIOC, GPIO_PIN_5);
      //disableInterrupts();
      //uart_send(PC_UART_CHANNEL, "ahoj\r\n");
      {
      log("%06d: Buffer: \n\r", i);
      }
      //enableInterrupts();
      i++;

      // clock ticking
      /*if (mode_manager_mode_data_valid(&mode_manager))
      {
        mode_manager.base_mode_data->display(&display_state, mode_manager.mode_data); // set the display state
        // TODO display on display

        //- LOGGING -----------------------------------------

        log("%6d: ", i);
        display_state_print(&display_state);
        log(",  ");
        mode_manager.base_mode_data->print(mode_manager.mode_data);
        //---------------------------------------------------

        mode_manager.base_mode_data->handle_sec_tic(mode_manager.mode_data);
      }

      // handle commands
      if (command != NULL)
      {
        log("dooo\n\r");
        mode_manager_process_commands(&mode_manager, command);

        // free command memory, cleanups
        //free(command);
        command = NULL;
      }*/
    }
  }

  return 0;
}

INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
{
  /*disableInterrupts();
  uart_read(PC_UART_CHANNEL, &message_buffer);
  enableInterrupts();*/
  // log("%06d: Buffer: %s\n\r", 0, message_buffer.data);
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