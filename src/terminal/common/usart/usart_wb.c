/*
 * Author: Savpek Electronics
 * Name:   USART with fifo buffer.
 * Explained:
 *	This USART lib adds XON/XOFF functionality over standard
 *	implementations. This is made by interrupt routine which
 *	is happens every time when character is received. Character
 *	is saved to buffer for later reading. If buffer is going full
 *	device sends XOFF and saves rest incoming characters to buffer.
 *	After buffer empties terminal send XON command again and start
 *	receiving normally.
 *
 * Usage:
 *  Init with command: usart_init_rs232_with_rxdry_irq ( ...
 *		Get more information about parameters from usart_wb.h header
 *		file. Clocks must be correctly initialized before this, and 
 *		also you must init INTC before. 
 *
 *	Settings: 
 *		All configurations can be found from config/usart_settings.h */

/* ASF */
#include "compiler.h"
#include "intc.h"

/* Internal */
#include "./terminal/common/usart/inc/usart_wb.h"
#include "./terminal/common/usart/config/usart_settings.h"

/* TLCR libs */
#include "./common/fifo/inc/fifo.h"
#include "./error_handler/inc/errorh.h"

/* This buffer contains incoming data. */
static fifo_uint8_t usart_fifo;

/* This resets status of transmitter. */
static void usart_reset_status(volatile avr32_usart_t *usart)
{
  usart->cr = AVR32_USART_CR_RSTSTA_MASK;
}

/* Brief Tests if the USART contains a received character. */
static errorc_t usart_test_hit(volatile avr32_usart_t *usart)
{
  return (usart->csr & AVR32_USART_CSR_RXRDY_MASK) != 0;
}

/* Tests if the USART is ready to transmit a character.
 * Returns 1 if the USART Transmit Holding Register is free, otherwise \c 0.*/
static errorc_t usart_tx_ready(volatile avr32_usart_t *usart)
{
  return (usart->csr & AVR32_USART_CSR_TXRDY_MASK) != 0;
}

static errorc_t usart_write_char(uint8_t c)
	{
	avr32_usart_t *usart;
	usart = USART_POINTER;
	
	if (usart_tx_ready(usart))
		{
		usart->thr = (c << AVR32_USART_THR_TXCHR_OFFSET) & AVR32_USART_THR_TXCHR_MASK;
		return EC_SUCCESS;
		}
	else
		return EC_USART_TX_BUSY;
	}

/* This reads character from MCU internal rx buffer. This is used to save received
 * characters to fifo buffer. */
static errorc_t usart_get_character_from_rx_register(volatile avr32_usart_t *usart, int *c)
{
  // Check for errors: frame, parity and overrun. In RS485 mode, a parity error
  // would mean that an address char has been received.
  if (usart->csr & (AVR32_USART_CSR_OVRE_MASK |
                    AVR32_USART_CSR_FRAME_MASK |
                    AVR32_USART_CSR_PARE_MASK))
    return EC_USART_RX_ERROR;

  // No error; if we really did receive a char, read it and return SUCCESS.
  if (usart_test_hit(usart))
  {
    *c = (usart->rhr & AVR32_USART_RHR_RXCHR_MASK) >> AVR32_USART_RHR_RXCHR_OFFSET;
    return EC_SUCCESS;
  }
  else
    return EC_USART_RX_EMPTY;
}

/* RXRDY IRQ routine, this is runned when
 * character is received. When received character is saved to
 * fifo buffer, if there are more than 5 characters in buffer
 * IRQ sends XOFF command to terminal. This because when
 * terminal transfers large datablock, simply routines got
 * overrunned when software have to wait for some reason, for
 * example when writing data to flash memory. */
__attribute__((__interrupt__))
static void usart_rxrdy_irq( void )
	{
	int char_temp = 0;
	
	if( usart_get_character_from_rx_register(USART_POINTER, &char_temp) == EC_USART_RX_ERROR )
		{
		errorh_new_error(EC_USART_RX_ERROR, ERROR_WARNING);
		usart_reset_status(USART_POINTER);
		}
	
	/* Send error message to log if fifo is full, this shouldn't
	 * happen however. This could happen when terminal doesn't have
	 * SW flow control enabled (XON/XOFF). */
	if(!fifo_put_uint8(&usart_fifo, char_temp))
		{
		errorh_new_error(EC_FIFO_FULL, ERROR_WARNING);
		}

	/* Here we send XOFF character if buffer have enough data.
	* After XOFF character we may receive some more characters
	* before terminal receives XOFF, for this reason buffer exist. */ 
	if(USART_XOFF_DELAY == fifo_count_data_uint8(&usart_fifo))
		{
		usart_putchar(USART_ASCII_XOFF);
		}
	}

/* Calculates a clock divider (CD) and a fractional part (FP) for
 * the USART asynchronous modes to generate a baud rate as close as
 * possible to the baud rate set point.
 *
 * Baud rate calculation:
 * Baudrate = \frac{SelectedClock}{Over \times (CD + \frac{FP}{8})} \f$, \e Over being 16 or 8.
 * The maximal oversampling is selected if it allows to generate a baud rate close to the set point.
 *
 * EC_USART_SUCCESS        Baud rate successfully initialized.
 * EC_USART_INVALID_INPUT  Baud rate set point is out of range for the given input clock frequency.
 */
static errorc_t usart_set_async_baudrate(volatile avr32_usart_t *usart, unsigned int baudrate, unsigned long pba_hz)
	{
	unsigned int over = (pba_hz >= 16 * baudrate) ? 16 : 8;
	unsigned int cd_fp = ((1 << AVR32_USART_BRGR_FP_SIZE) * pba_hz + (over * baudrate) / 2) / (over * baudrate);
	unsigned int cd = cd_fp >> AVR32_USART_BRGR_FP_SIZE;
	unsigned int fp = cd_fp & ((1 << AVR32_USART_BRGR_FP_SIZE) - 1);

	if (cd < 1 || cd > (1 << AVR32_USART_BRGR_CD_SIZE) - 1)
	return EC_USART_INVALID_INPUT;

	usart->mr = (usart->mr & ~(AVR32_USART_MR_USCLKS_MASK |
								 AVR32_USART_MR_SYNC_MASK |
								 AVR32_USART_MR_OVER_MASK)) |
				  AVR32_USART_MR_USCLKS_MCK << AVR32_USART_MR_USCLKS_OFFSET |
				  ((over == 16) ? AVR32_USART_MR_OVER_X16 : AVR32_USART_MR_OVER_X8) << AVR32_USART_MR_OVER_OFFSET;

	usart->brgr = cd << AVR32_USART_BRGR_CD_OFFSET |
				  fp << AVR32_USART_BRGR_FP_OFFSET;

	return EC_SUCCESS;
	}

static void usart_reset(volatile avr32_usart_t *usart)
	{
	bool global_interrupt_enabled = cpu_irq_is_enabled();

	// Disable all USART interrupts.
	// Interrupts needed should be set explicitly on every reset.
	if (global_interrupt_enabled) cpu_irq_disable();
	usart->idr = 0xFFFFFFFF;
	usart->csr;
	if (global_interrupt_enabled) cpu_irq_enable();

	// Reset mode and other registers that could cause unpredictable behavior after reset.
	usart->mr = 0;
	usart->rtor = 0;
	usart->ttgr = 0;

	// Shutdown TX and RX (will be re-enabled when setup has successfully completed),
	// reset status bits and turn off DTR and RTS.
	usart->cr = AVR32_USART_CR_RSTRX_MASK   |
				AVR32_USART_CR_RSTTX_MASK   |
				AVR32_USART_CR_RSTSTA_MASK  |
				AVR32_USART_CR_RSTIT_MASK   |
				AVR32_USART_CR_RSTNACK_MASK |
				AVR32_USART_CR_RTSDIS_MASK;
	}

errorc_t usart_init_rs232_with_rxdry_irq(unsigned long pba_hz)
	{
	avr32_usart_t *usart;
	usart = USART_POINTER;
	
	// Reset the USART and shutdown TX and RX.
	usart_reset(usart);
	
	// Resets fifo buffer
	fifo_reset_uint8(&usart_fifo);

	// Check input values.
	if (USART_CHARLENGTH < 5 || USART_CHARLENGTH > 9 ||
		  USART_PARITYPE > 7 ||
		  USART_STOPBITS > 2 + 255 ||
		  USART_CHANNELMODE > 3 ||
		  usart_set_async_baudrate(usart, USART_BAUDRATE, pba_hz) == EC_USART_INVALID_INPUT)
	return EC_USART_INVALID_INPUT;

	if (USART_CHARLENGTH == 9)
		{
		// Character length set to 9 bits. MODE9 dominates CHRL.
		usart->mr |= AVR32_USART_MR_MODE9_MASK;
		}
	else
		{
		// CHRL gives the character length (- 5) when MODE9 = 0.
		usart->mr |= (USART_CHARLENGTH - 5) << AVR32_USART_MR_CHRL_OFFSET;
		}

	usart->mr |= USART_PARITYPE << AVR32_USART_MR_PAR_OFFSET |
				 USART_CHANNELMODE << AVR32_USART_MR_CHMODE_OFFSET;

	if (USART_STOPBITS > USART_2_STOPBITS)
		{
		// Set two stop bits
		usart->mr |= AVR32_USART_MR_NBSTOP_2 << AVR32_USART_MR_NBSTOP_OFFSET;
		// and a timeguard period gives the rest.
		usart->ttgr = USART_STOPBITS - USART_2_STOPBITS;
		}
	else
		{
		// Insert 1, 1.5 or 2 stop bits.
		usart->mr |= USART_STOPBITS << AVR32_USART_MR_NBSTOP_OFFSET;	
		}

	// Set normal mode.
	usart->mr = (usart->mr & ~AVR32_USART_MR_MODE_MASK) |
				AVR32_USART_MR_MODE_NORMAL << AVR32_USART_MR_MODE_OFFSET;

	//Enables interrupt vector from usart.
	usart->ier |= 1<<AVR32_USART_IER_RXRDY_OFFSET;

	/* Register IRQ for RXRDY buffer. */
	INTC_register_interrupt(&usart_rxrdy_irq, AVR32_USART0_IRQ, AVR32_INTC_INT0);

	// Setup complete; enable communication.
	// Enable input and output.
	usart->cr = AVR32_USART_CR_RXEN_MASK |
				AVR32_USART_CR_TXEN_MASK;

	return EC_SUCCESS;
}

errorc_t usart_putchar(uint8_t c)
	{
	int timeout = USART_DEFAULT_TIMEOUT;

	do
		{
		if (!timeout--) return EC_FAILURE;
	} while (usart_write_char(c) != EC_SUCCESS);

	return EC_SUCCESS;
	}

/* Reads single character from buffer. Don't stay here if buffer is empty. 
 * However, every 260 read attempt we send XON character to terminal. This
 * makes sure that terminal never stop responding completly.*/
errorc_t usart_read_char(uint8_t *c)
	{
	avr32_usart_t *usart;
	usart = USART_POINTER;
	static unsigned long int xon_delay_counter = 0;
	
	if(xon_delay_counter > USART_XON_DELAY)
		{
		usart_putchar(USART_ASCII_XON);	
		xon_delay_counter = 0;
		}
	
	/* If fifo buffer isn't empty, return character. */
	if(EC_FIFO_EMPTY != fifo_get_uint8(&usart_fifo, c))
		{
		xon_delay_counter = 0;
		return EC_SUCCESS;
		}
	else
		{
		xon_delay_counter++;
		return EC_USART_RX_EMPTY;
		}
	}

/* Get character (from buffer), stays inside of function
 * as long as character is received. If*/
uint8_t usart_getchar( )
{
	uint8_t temp_char;
	
	/* We stay here as long as needed to receive characters. */
	while(EC_USART_RX_EMPTY == usart_read_char(&temp_char));
	return (int)temp_char;
}

void usart_write_line(const char *string)
	{
	avr32_usart_t *usart;
	usart = USART_POINTER;
	
	while (*string != '\0')
		{
		usart_putchar(*string++);	
		}
	}

/*@ This function is for printing non constant strings. */
extern void usart_write_string(char *string)
	{
	avr32_usart_t *usart;
	usart = USART_POINTER;
	
	uint16_t char_index = 0;
	while(string[char_index] != 0) usart_putchar(string[char_index]);
	}