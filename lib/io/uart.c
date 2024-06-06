#include "uart.h"
#include <stm8s_uart1.h>
#include <stm8s_uart3.h>
#include <stdint.h>
#include <stdlib.h>

void uart_init(uint8_t uart_idx, uint32_t baud_rate)
{
    if (uart_idx == 3)
    {
        UART3_Init(baud_rate, UART3_WORDLENGTH_8D, UART3_STOPBITS_1, UART3_PARITY_NO, UART3_MODE_TXRX_ENABLE);
        UART3_ITConfig(UART3_IT_RXNE_OR, ENABLE);
        UART3_Cmd(ENABLE);
    }

    else if (uart_idx == 1)
    {
        UART1_Init(baud_rate, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);
        UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
        UART1_Cmd(ENABLE);
    }
}

void uart_send_byte(uint8_t uart_idx, uint8_t data)
{
    while (!uart_send_ready(uart_idx))
        ;

    if (uart_idx == 3)
        UART3_SendData8(data);

    else if (uart_idx == 1)
        UART1_SendData8(data);
}

void uart_send(uint8_t uart_idx, uint8_t *data)
{
    for (int i = 0; data[i]; i++)
        uart_send_byte(uart_idx, data[i]);
}

uint8_t uart_read_byte(uint8_t uart_idx)
{
    if (uart_idx == 3)
        return UART3_ReceiveData8();
    else if (uart_idx == 1)
        return UART1_ReceiveData8();
    return 0;
}

int uart_send_ready(uint8_t uart_idx)
{
    if (uart_idx == 3)
        return UART3_GetFlagStatus(UART3_FLAG_TXE);
    else if (uart_idx == 1)
        return UART1_GetFlagStatus(UART1_FLAG_TXE);
    return 1;
}

int uart_read_ready(uint8_t uart_idx)
{
    if (uart_idx == 3)
        return UART3_GetFlagStatus(UART3_FLAG_RXNE);
    else if (uart_idx == 1)
        return UART1_GetFlagStatus(UART1_FLAG_RXNE);
    return 0;
}

inline void uart_read(uint8_t uard_idx, volatile Buffer *data_buffer)
{
    if (data_buffer == NULL)
        return;

    volatile uint8_t *data = data_buffer->data;
    volatile int *data_end_idx = data_buffer->data_end_idx;

    if (data == NULL || data_end_idx == NULL)
        return;

    while (uart_read_ready(uard_idx))
    {
        data[(*data_end_idx)++] = uart_read_byte(uard_idx);
        data[*data_end_idx] = 0;
    }
}