
#ifndef _USART_H_
#define _USART_H_

/* ASF */
#include <avr32/io.h>
#include "compiler.h"

/* TLCR lib */
#include "./errorh/inc/errorh.h"


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

//! Input parameters when initializing RS232 and similar modes.
typedef struct
{
  //! Set baud rate of the USART (unused in slave modes).
  unsigned long baudrate;

  //! Number of bits to transmit as a character (5 to 9).
  unsigned char charlength;

  //! How to calculate the parity bit: \ref USART_EVEN_PARITY, \ref USART_ODD_PARITY,
  //! \ref USART_SPACE_PARITY, \ref USART_MARK_PARITY, \ref USART_NO_PARITY or
  //! \ref USART_MULTIDROP_PARITY.
  unsigned char paritytype;

  //! Number of stop bits between two characters: \ref USART_1_STOPBIT,
  //! \ref USART_1_5_STOPBITS, \ref USART_2_STOPBITS or any number from 3 to 257
  //! which will result in a time guard period of that length between characters.
  //! \note \ref USART_1_5_STOPBITS is supported in asynchronous modes only.
  unsigned short stopbits;

  //! Run the channel in testmode: \ref USART_NORMAL_CHMODE, \ref USART_AUTO_ECHO,
  //! \ref USART_LOCAL_LOOPBACK or \ref USART_REMOTE_LOOPBACK.
  unsigned char channelmode;
} usart_options_t;

/* This setups USART to use common RS232 protocol. */
errorc_t usart_init_rs232(volatile avr32_usart_t *usart, const usart_options_t *opt, long pba_hz);

/* Sends a character with the USART.
 * Waits until character transfer is complete or timeout is expired.
 * EC_SUCCESS  The character was written.
 * EC_FAILURE  The function timed out before the USART transmitter became ready to send. */
extern void (*usart_putchar)(char c);

/* This checks is there received characters in fifo, if there is return one
 * character from buffer. Otherwise send XON mark to terminal and return error code.
 * EC_SUCCESS  The character was read successfully.
 * EC_USART_RX_EMPTY The RX buffer was empty.
 * EC_USART_RX_ERROR An error was deteceted. */
extern errorc_t usart_read_char(char *c);

/* Same as usart_read_char, for that exeption that this waits until
 * character is received. Returns character.*/
extern char usart_getchar( void );

/* Writes character string to the USART (constant). */
extern void usart_write_line(char *string);

/* Writes non-constant string to usart. */
extern void usart_write_string(char *string);

#endif //#ifndef _USART_H_