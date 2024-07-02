#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"
#include "hardware/uart.h"
#include <lib/message/parser.h>
#include <lib/core/buffer.h>
#include <string.h>
#include <lib/data/data_model.h>
#include <lib/logging/logging.h>
#include <lib/message/message_info.h>

#include <lib/mode/mode_manager.h>
#include <lib/mode/finals.h>
#include <lib/mode/ab.h>
#include <lib/mode/abc.h>
#include <lib/mode/abcd.h>
#include <lib/peripheral/shift_register.h>

#define MESSAGE_BUFFER_SIZE 128
volatile char message_buffer_data[MESSAGE_BUFFER_SIZE];
volatile uint32_t message_buffer_data_end_idx = 0;
Buffer message_buffer(message_buffer_data, &message_buffer_data_end_idx, MESSAGE_BUFFER_SIZE);

MessageParser msgParser(&message_buffer);

// TODO refactor
ModeManager mode_manager;
AbModeData ab_mode_data;
AbcModeData abc_mode_data;
AbcdModeData abcd_mode_data;
FinalsModeData finals_mode_data;
RoundInfo round_info;

const uint32_t sr_clk_pin = 28;
const uint32_t sr_data_pin = 27;
const uint32_t sr_cs_pin = 26;
ShiftRegister shift_register(sr_clk_pin, sr_data_pin, sr_cs_pin);
DisplayController display_controller(&shift_register);


void on_uart_rx() {
    while (uart_is_readable(uart0)) {
        const uint8_t data = uart_getc(uart0);
        message_buffer.addByte(data);
    }
}

int main()
{
    stdio_init_all();

    // init uart
    irq_set_exclusive_handler(UART0_IRQ , on_uart_rx);
    irq_set_enabled(UART0_IRQ , true);
    uart_set_irq_enables(uart0, true, false);

    // add message
    const char* msg = "<012003003ABD><700120>";
    MessageType msgType;
    
    for(int i = 0; i < strlen(msg); i++)
        message_buffer.addByte(msg[i]);

        // init display
    display_controller.displayState()->clear();
    display_controller.display();

    // modes handling

    int j = 0;
    while (true) {
        const BaseCommand* command = msgParser.parseMessage();
        // TODO nullptr check
        if(command != nullptr) {
            msgType = ((BaseCommand*)command)->type;
            Logging::log("Message: %s\n", messageTypeToString(msgType));
            mode_manager.processCommand(command);
        }
        sleep_ms(1000);
        //Logging::log("health\r\n");

        if (mode_manager.modeDataValid())
        {
            BaseModeData* modeData = mode_manager.currentMode();
            modeData->display(display_controller.displayState()); // set the display state
            // TODO display on display

            //- LOGGING -----------------------------------------

            Logging::log("%6d: ", j);
            display_controller.displayState()->log();
            Logging::log(",  ");
            modeData->log();
            //---------------------------------------------------

            modeData->handleSecTick();
            //mode_manager.base_mode_data->handle_sec_tic(mode_manager.mode_data);
        }
        
        j++;
    }
}
