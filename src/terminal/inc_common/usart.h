
#ifndef _USART_H_
#define _USART_H_

#include <avr32/io.h>
#include "compiler.h"


/*! \name Return Values
 */
//! @{
#define USART_SUCCESS                 0 //!< Successful completion.
#define USART_FAILURE                -1 //!< Failure because of some unspecified reason.
#define USART_INVALID_INPUT           1 //!< Input value out of range.
#define USART_INVALID_ARGUMENT       -1 //!< Argument value out of range.
#define USART_TX_BUSY                 2 //!< Transmitter was busy.
#define USART_RX_EMPTY                3 //!< Nothing was received.
#define USART_RX_ERROR                4 //!< Transmission error occurred.
#define USART_MODE_FAULT              5 //!< USART not in the appropriate mode.
//! @}

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

/*! \name Stop Bits Settings
 */
//! @{
#define USART_1_STOPBIT               AVR32_USART_MR_NBSTOP_1   //!< Use 1 stop bit.
#define USART_1_5_STOPBITS            AVR32_USART_MR_NBSTOP_1_5 //!< Use 1.5 stop bits.
#define USART_2_STOPBITS              AVR32_USART_MR_NBSTOP_2   //!< Use 2 stop bits (for more, just give the number of bits).
//! @}

/*! \name Channel Modes
 */
//! @{
#define USART_NORMAL_CHMODE           AVR32_USART_MR_CHMODE_NORMAL      //!< Normal communication.
#define USART_AUTO_ECHO               AVR32_USART_MR_CHMODE_ECHO        //!< Echo data.
#define USART_LOCAL_LOOPBACK          AVR32_USART_MR_CHMODE_LOCAL_LOOP  //!< Local loopback.
#define USART_REMOTE_LOOPBACK         AVR32_USART_MR_CHMODE_REMOTE_LOOP //!< Remote loopback.
//! @}

#if defined(AVR32_USART_400_H_INCLUDED) || \
    defined(AVR32_USART_410_H_INCLUDED) || \
    defined(AVR32_USART_420_H_INCLUDED) || \
    defined(AVR32_USART_440_H_INCLUDED) || \
    defined(AVR32_USART_602_H_INCLUDED)

/*! \name LIN Node Actions
 */
//! @{
#define USART_LIN_PUBLISH_ACTION      AVR32_USART_LINMR_NACT_PUBLISH    //!< The USART transmits the response.
#define USART_LIN_SUBSCRIBE_ACTION    AVR32_USART_LINMR_NACT_SUBSCRIBE  //!< The USART receives the response.
#define USART_LIN_IGNORE_ACTION       AVR32_USART_LINMR_NACT_IGNORE     //!< The USART does not transmit and does not receive the reponse.
//! @}

/*! \name LIN Checksum Types
 */
//! @{
#define USART_LIN_ENHANCED_CHECKSUM   0 //!< LIN 2.0 "enhanced" checksum.
#define USART_LIN_CLASSIC_CHECKSUM    1 //!< LIN 1.3 "classic" checksum.
//! @}

#endif  // USART rev. >= 4.0.0


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

//! Input parameters when initializing ISO7816 mode.
typedef struct
{
  //! Set the frequency of the ISO7816 clock.
  unsigned long iso7816_hz;

  //! The number of ISO7816 clock ticks in every bit period (1 to 2047, 0 = disable clock).
  //! Bit rate = \ref iso7816_hz / \ref fidi_ratio.
  unsigned short fidi_ratio;

  //! How to calculate the parity bit: \ref USART_EVEN_PARITY for normal mode or
  //! \ref USART_ODD_PARITY for inverse mode.
  unsigned char paritytype;

  //! Inhibit Non Acknowledge:\n
  //!   - 0: the NACK is generated;\n
  //!   - 1: the NACK is not generated.
  //!
  //! \note This bit will be used only in ISO7816 mode, protocol T = 0 receiver.
  int inhibit_nack;

  //! Disable successive NACKs.
  //! Successive parity errors are counted up to the value in the \ref max_iterations field.
  //! These parity errors generate a NACK on the ISO line. As soon as this value is reached,
  //! no addititional NACK is sent on the ISO line. The ITERATION flag is asserted.
  int dis_suc_nack;

  //! Max number of repetitions (0 to 7).
  unsigned char max_iterations;

  //! Bit order in transmitted characters:\n
  //!   - 0: LSB first;\n
  //!   - 1: MSB first.
  int bit_order;
} usart_iso7816_options_t;

//------------------------------------------------------------------------------
/*! \name Initialization Functions
 */
//! @{

/*! \brief Resets the USART and disables TX and RX.
 *
 * \param usart   Base address of the USART instance.
 */
extern void usart_reset(volatile avr32_usart_t *usart);

/*! \brief Sets up the USART to use the standard RS232 protocol.
 *
 * \param usart   Base address of the USART instance.
 * \param opt     Options needed to set up RS232 communication (see \ref usart_options_t).
 * \param pba_hz  USART module input clock frequency (PBA clock, Hz).
 *
 * \retval USART_SUCCESS        Mode successfully initialized.
 * \retval USART_INVALID_INPUT  One or more of the arguments is out of valid range.
 */
extern int usart_init_rs232(volatile avr32_usart_t *usart, const usart_options_t *opt, long pba_hz);

/*! \brief Sets up the USART to use hardware handshaking.
 *
 * \param usart   Base address of the USART instance.
 * \param opt     Options needed to set up RS232 communication (see \ref usart_options_t).
 * \param pba_hz  USART module input clock frequency (PBA clock, Hz).
 *
 * \retval USART_SUCCESS        Mode successfully initialized.
 * \retval USART_INVALID_INPUT  One or more of the arguments is out of valid range.
 *
 * \note \ref usart_init_rs232 does not need to be invoked before this function.
 */
extern int usart_init_hw_handshaking(volatile avr32_usart_t *usart, const usart_options_t *opt, long pba_hz);

/*! \brief Sets up the USART to use a synchronous RS232-like protocol in master mode.
 *
 * \param usart   Base address of the USART instance.
 * \param opt     Options needed to set up RS232 communication (see \ref usart_options_t).
 * \param pba_hz  USART module input clock frequency (PBA clock, Hz).
 *
 * \retval USART_SUCCESS        Mode successfully initialized.
 * \retval USART_INVALID_INPUT  One or more of the arguments is out of valid range.
 */
extern int usart_init_sync_master(volatile avr32_usart_t *usart, const usart_options_t *opt, long pba_hz);

/*! \brief Sets up the USART to use a synchronous RS232-like protocol in slave mode.
 *
 * \param usart   Base address of the USART instance.
 * \param opt     Options needed to set up RS232 communication (see \ref usart_options_t).
 * \param pba_hz  USART module input clock frequency (PBA clock, Hz).
 *
 * \retval USART_SUCCESS        Mode successfully initialized.
 * \retval USART_INVALID_INPUT  One or more of the arguments is out of valid range.
 */
extern int usart_init_sync_slave(volatile avr32_usart_t *usart, const usart_options_t *opt, long pba_hz);

//------------------------------------------------------------------------------
/*! \name Read and Reset Error Status Bits
 */
//! @{

/*! \brief Resets the error status.
 *
 * This function resets the status bits indicating that a parity error,
 * framing error or overrun has occurred. The RXBRK bit, indicating
 * a start/end of break condition on the RX line, is also reset.
 *
 * \param usart   Base address of the USART instance.
 */
#if (defined __GNUC__)
__attribute__((__always_inline__))
#endif
static inline void usart_reset_status(volatile avr32_usart_t *usart)
{
  usart->cr = AVR32_USART_CR_RSTSTA_MASK;
}

/*! \brief Checks if a parity error has occurred since last status reset.
 *
 * \param usart   Base address of the USART instance.
 *
 * \return \c 1 if a parity error has been detected, otherwise \c 0.
 */
#if (defined __GNUC__)
__attribute__((__always_inline__))
#endif
static inline int usart_parity_error(volatile avr32_usart_t *usart)
{
  return (usart->csr & AVR32_USART_CSR_PARE_MASK) != 0;
}

/*! \brief Checks if a framing error has occurred since last status reset.
 *
 * \param usart   Base address of the USART instance.
 *
 * \return \c 1 if a framing error has been detected, otherwise \c 0.
 */
#if (defined __GNUC__)
__attribute__((__always_inline__))
#endif
static inline int usart_framing_error(volatile avr32_usart_t *usart)
{
  return (usart->csr & AVR32_USART_CSR_FRAME_MASK) != 0;
}

/*! \brief Checks if an overrun error has occurred since last status reset.
 *
 * \param usart   Base address of the USART instance.
 *
 * \return \c 1 if a overrun error has been detected, otherwise \c 0.
 */
#if (defined __GNUC__)
__attribute__((__always_inline__))
#endif
static inline int usart_overrun_error(volatile avr32_usart_t *usart)
{
  return (usart->csr & AVR32_USART_CSR_OVRE_MASK) != 0;
}

//! @}

//------------------------------------------------------------------------------
/*! \name Transmit/Receive Functions
 */
//! @{

/*! \brief Addresses a receiver.
 *
 * While in RS485 mode, receivers only accept data addressed to them.
 * A packet/char with the address tag set has to precede any data.
 * This function is used to address a receiver. This receiver should read
 * all the following data, until an address packet addresses another receiver.
 *
 * \param usart     Base address of the USART instance.
 * \param address   Address of the target device.
 *
 * \retval USART_SUCCESS    Address successfully sent (if current mode is RS485).
 * \retval USART_MODE_FAULT Wrong operating mode.
 */
extern int usart_send_address(volatile avr32_usart_t *usart, int address);

/*! \brief Tests if the USART is ready to transmit a character.
 *
 * \param usart   Base address of the USART instance.
 *
 * \return \c 1 if the USART Transmit Holding Register is free, otherwise \c 0.
 */
#if (defined __GNUC__)
__attribute__((__always_inline__))
#endif
static inline int usart_tx_ready(volatile avr32_usart_t *usart)
{
  return (usart->csr & AVR32_USART_CSR_TXRDY_MASK) != 0;
}

/*! \brief Writes the given character to the TX buffer if the transmitter is ready.
 *
 * \param usart   Base address of the USART instance.
 * \param c       The character (up to 9 bits) to transmit.
 *
 * \retval USART_SUCCESS  The transmitter was ready.
 * \retval USART_TX_BUSY  The transmitter was busy.
 */
extern int usart_write_char(volatile avr32_usart_t *usart, int c);

/*! \brief An active wait writing a character to the USART.
 *
 * \param usart   Base address of the USART instance.
 * \param c       The character (up to 9 bits) to transmit.
 */
#if (defined __GNUC__)
__attribute__((__always_inline__))
#endif
static inline void usart_bw_write_char(volatile avr32_usart_t *usart, int c)
{
  while (usart_write_char(usart, c) != USART_SUCCESS);
}

/*! \brief Sends a character with the USART.
 *
 * \param usart   Base address of the USART instance.
 * \param c       Character to write.
 *
 * \retval USART_SUCCESS  The character was written.
 * \retval USART_FAILURE  The function timed out before the USART transmitter became ready to send.
 */
extern int usart_putchar(volatile avr32_usart_t *usart, int c);

/*! \brief Tests if all requested USART transmissions are over.
 *
 * \param usart   Base address of the USART instance.
 *
 * \return \c 1 if the USART Transmit Shift Register and the USART Transmit
 *         Holding Register are free, otherwise \c 0.
 */
#if (defined __GNUC__)
__attribute__((__always_inline__))
#endif
static inline int usart_tx_empty(volatile avr32_usart_t *usart)
{
  return (usart->csr & AVR32_USART_CSR_TXEMPTY_MASK) != 0;
}

/*! \brief Tests if the USART contains a received character.
 *
 * \param usart   Base address of the USART instance.
 *
 * \return \c 1 if the USART Receive Holding Register is full, otherwise \c 0.
 */
#if (defined __GNUC__)
__attribute__((__always_inline__))
#endif
static inline int usart_test_hit(volatile avr32_usart_t *usart)
{
  return (usart->csr & AVR32_USART_CSR_RXRDY_MASK) != 0;
}

/*! \brief Checks the RX buffer for a received character, and stores it at the
 *         given memory location.
 *
 * \param usart   Base address of the USART instance.
 * \param c       Pointer to the where the read character should be stored
 *                (must be at least short in order to accept 9-bit characters).
 *
 * \retval USART_SUCCESS  The character was read successfully.
 * \retval USART_RX_EMPTY The RX buffer was empty.
 * \retval USART_RX_ERROR An error was deteceted.
 */
extern int usart_read_char(volatile avr32_usart_t *usart, int *c);

/*! \brief Waits until a character is received, and returns it.
 *
 * \param usart   Base address of the USART instance.
 *
 * \return The received character, or \ref USART_FAILURE upon error.
 */
extern int usart_getchar(volatile avr32_usart_t *usart);

/*! \brief Writes one character string to the USART.
 *
 * \param usart   Base address of the USART instance.
 * \param string  String to be written.
 */
extern void usart_write_line(volatile avr32_usart_t *usart, const char *string);

/*!!@ Savpek electronics function to output string (array of chars) */
extern void usart_write_string(volatile avr32_usart_t *usart, char *string);

extern void usart_write_from_storage(volatile avr32_usart_t *usart, uint8_t segment_id);

/*! \brief Gets and echoes characters until end of line (detected by a CR character).
 *
 * \param usart   Base address of the USART instance.
 *
 * \retval USART_SUCCESS  Success.
 * \retval USART_FAILURE  Low-level error detected or ETX character received.
 */
extern int usart_get_echo_line(volatile avr32_usart_t *usart);

extern void usart_write_bits (volatile avr32_usart_t *usart, int32_t bit_value, uint8_t print_length);
extern void usart_write_hex (volatile avr32_usart_t *usart, int32_t hex_value, uint8_t print_length);

#endif //#ifndef _USART_H_