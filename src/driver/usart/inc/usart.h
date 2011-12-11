
#ifndef _USART_H_
#define _USART_H_

/* ASF */
#include <avr32/io.h>
#include "compiler.h"

/* TLCR lib */
#include "./api/errorh/inc/errorh.h"

/* USART ascii marks */
#define USART_ASCII_DEL 0x7F
#define USART_ASCII_CR 0x0D
#define USART_ASCII_ESC 0x1B
#define USART_ASCII_SPACE 0x20
#define USART_ASCII_NL 0x0A

#define USART_ASCII_XON 0x11
#define USART_ASCII_XOFF 0x13

#define USART_ASCII_NUMBER_LOW_LIMIT 48
#define USART_ASCII_NUMBER_HIGH_LIMIT 57

#define USART_ASCII_VISIBLEC_LOW_LIMIT 32
#define USART_ASCII_VISIBLEC_HIGH_LIMIT 126

//! Default time-out value (number of attempts).
#define USART_DEFAULT_TIMEOUT         10000

/*! \name Parity Settings
 */
//! @{
#define USART_EVEN_PARITY             AVR32_USART_MR_PAR_EVEN   //!< Use even parity on character transmission.
#define USART_ODD_PARITY              AVR32_USART_MR_PAR_ODD    //!< Use odd parity on character transmission.
#define USART_SPACE_PARITY            AVR32_USART_MR_PAR_SPACE  //!< Use a space as parity bit.
#define USART_MARK_PARITY             AVR32_USART_MR_PAR_MARK   //!< Use a mark as parity bit.
#define USART_NO_PARITY               AVR32_USART_MR_PAR_NONE   //!< Don't use a parity bit.
#define USART_MULTIDROP_PARITY        AVR32_USART_MR_PAR_MULTI  //!< Parity bit is used to flag address characters.
//! @}

/* Stop Bits Settings */
#define USART_1_STOPBIT               AVR32_USART_MR_NBSTOP_1   //!< Use 1 stop bit.
#define USART_1_5_STOPBITS            AVR32_USART_MR_NBSTOP_1_5 //!< Use 1.5 stop bits.
#define USART_2_STOPBITS              AVR32_USART_MR_NBSTOP_2   //!< Use 2 stop bits (for more, just give the number of bits).

/* Channel Mode */
#define USART_NORMAL_CHMODE           AVR32_USART_MR_CHMODE_NORMAL      //!< Normal communication.
#define USART_AUTO_ECHO               AVR32_USART_MR_CHMODE_ECHO        //!< Echo data.
#define USART_LOCAL_LOOPBACK          AVR32_USART_MR_CHMODE_LOCAL_LOOP  //!< Local loopback.
#define USART_REMOTE_LOOPBACK         AVR32_USART_MR_CHMODE_REMOTE_LOOP //!< Remote loopback.

/* This setups USART to use common RS232 protocol. This inits usart use fifo buffer to avoid
 * possible overruns during large data block transfers. As parameter, you must define PORTA 
 * frequency, since USART module speed is based on it. Define actual usart settings in 
 * config/usart_settings.h */
extern errorc_t usart_init_rs232_with_rxdry_irq(unsigned long pba_hz);

/* Sends a character with the USART.
 * Waits until character transfer is complete or timeout is expired.
 * EC_SUCCESS  The character was written.
 * EC_FAILURE  The function timed out before the USART transmitter became ready to send. */
extern void usart_putchar(uint8_t c);


/* This checks is there received characters in fifo, if there is return one
 * character from buffer. Otherwise send XON mark to terminal and return error code.
 * EC_SUCCESS  The character was read successfully.
 * EC_USART_RX_EMPTY The RX buffer was empty.
 * EC_USART_RX_ERROR An error was deteceted. */
extern errorc_t usart_read_char(uint8_t *c);

/* Same as usart_read_char, for that exeption that this waits until
 * character is received. Returns character.*/
extern uint8_t usart_getchar( void );

/* Writes character string to the USART (constant). */
extern void usart_write_line(uint8_t *string);

/* Writes non-constant string to usart. */
extern void usart_write_string(uint8_t *string);

#endif //#ifndef _USART_H_