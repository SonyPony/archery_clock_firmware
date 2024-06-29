#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"
#include <lib/message/parser.h>
#include <lib/core/buffer.h>
#include <string.h>
#include <lib/data/data_model.h>

#define MESSAGE_BUFFER_SIZE 512
volatile char message_buffer_data[MESSAGE_BUFFER_SIZE];
volatile uint32_t message_buffer_data_end_idx = 0;
Buffer message_buffer(message_buffer_data, &message_buffer_data_end_idx, MESSAGE_BUFFER_SIZE);

InitializationCommand initCommand;
BreakCommand breakCommand;
BaseCommand baseCommand;

int64_t alarm_callback(alarm_id_t id, void *user_data) {
    // Put your timeout handler code in here
    printf("timer!\n");
    return 0;
} 


int main()
{
    stdio_init_all();

    // add message
    const char* msg = "<012003003ABD><6>";
    void* result = nullptr;
    MessageType msgType;

    for(int i = 0; i < strlen(msg); i++)
        message_buffer.addByte(msg[i]);
    printf("Foo %s\n", message_buffer.data);

    // Timer example code - This example fires off the callback after 2000ms
    add_alarm_in_ms(2000, alarm_callback, nullptr, false);

    while (true) {
        result = parse_message(&message_buffer, &breakCommand, &initCommand, &baseCommand);
        msgType = ((BaseCommand*)result)->type;
        printf("Hello, world!\n %d", (char)(msgType));
        sleep_ms(1000);
    }
}
