#ifndef _UART_H_
#define _UART_H_

#include <stdint.h>
#include <lib/core/buffer.h>

/**
 * @brief uart_init The function initializes TX and RX for given UART channel with
 * 1 stopbit and 8 data bits.
 * @param uart_idx UART Channel in uint8_t type. UART1 corresponds to channel 1 and UART3 to channel 3.
 * @param baud_rate Baud rate of the uart channel.
 */
void uart_init(uint8_t uart_idx, uint32_t baud_rate);

/**
 * @brief uart_send_byte Sends a byte via given UART channel. The method already contains
 * waiting for TX to be ready.
 * @param uart_idx Target UART channel.
 * @param data Byte data to be sent.
 */
void uart_send_byte(uint8_t uart_idx, uint8_t data);

/**
 * @brief uart_send Sends a string via uart channel. The string has to end with null character.
 * @param uart_idx Target UART channel.
 * @param data String that will be sent. The string will has to end with null character.
 */
void uart_send(uint8_t uart_idx, char *data);

/**
 * @brief uart_read_byte Reads byte from read buffer. It doesn't contain waiting for read ready flag.
 * @param uart_idx Target UART channel.
 */
uint8_t uart_read_byte(uint8_t uart_idx);

/**
 * @brief uart_read_ready Returns true if there is data to be read in the read data.
 * @return Returns true if there is data to be read in the read buffer, otherwise it returns false.
 */
int uart_read_ready(uint8_t uart_idx);

/**
 * @brief uart_send_read Return true if the UART channel is read to send data, otherwise it returns false.
 * @return Returns true, if data are ready to be send, otherwise it returns false.
 */
int uart_send_ready(uint8_t uart_idx);

/**
 * @brief uart_read Reads the whole receive/read buffer. The read data are stored in given data buffer.
 * The function already contains modification of data buffer and waiting for ready read flag.
 * @param uart_idx Target UART channel.
 * @param data_buffer Target data buffer where the read data will be stored.
 */
inline void uart_read(uint8_t uard_idx, volatile Buffer *data_buffer);

#endif