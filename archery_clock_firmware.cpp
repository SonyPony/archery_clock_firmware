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

#define MESSAGE_BUFFER_SIZE 8
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
BaseModeData base_mode_data;
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
    //const char* msg = "<3><4>";//"<012003003ABD><6>";
    const BaseCommand* result = nullptr;
    MessageType msgType;

        // init display
    display_controller.display();

    // modes handling
    InitializationCommand init_command;
    mode_manager_init(&mode_manager, &base_mode_data, &ab_mode_data, &abc_mode_data, &abcd_mode_data, &finals_mode_data, &init_command, &round_info);

    /*for(int i = 0; i < strlen(msg); i++)
        message_buffer.addByte(msg[i]);*/

    int j = 0;
    while (true) {
        result = msgParser.parseMessage();
        // TODO nullptr check
        if(result != nullptr) {
            msgType = ((BaseCommand*)result)->type;
            Logging::log("Message: %s\n", messageTypeToString(msgType));
        }
        sleep_ms(1000);

        if (mode_manager_mode_data_valid(&mode_manager))
        {
            mode_manager.base_mode_data->display(display_controller.displayState(), mode_manager.mode_data); // set the display state
            // TODO display on display

            //- LOGGING -----------------------------------------

            Logging::log("%6d: ", j);
            display_controller.displayState()->log();
            Logging::log(",  ");
            mode_manager.base_mode_data->print(mode_manager.mode_data);
            //---------------------------------------------------

            mode_manager.base_mode_data->handle_sec_tic(mode_manager.mode_data);
        }
        
        j++;
    }
}
