
#include "compiler.h"
#include "usart.h"
#include "ascii_chars.h"
#include "storage.h"

//------------------------------------------------------------------------------
/*! \name Private Functions
 */
//! @{


/*! \brief Checks if the USART is in multidrop mode.
 *
 * \param usart Base address of the USART instance.
 *
 * \return \c 1 if the USART is in multidrop mode, otherwise \c 0.
 */
#if (defined __GNUC__)
__attribute__((__always_inline__))
#endif
static __inline__ int usart_mode_is_multidrop(volatile avr32_usart_t *usart)
{
  return ((usart->mr >> AVR32_USART_MR_PAR_OFFSET) & AVR32_USART_MR_PAR_MULTI) == AVR32_USART_MR_PAR_MULTI;
}


/*! \brief Calculates a clock divider (\e CD) and a fractional part (\e FP) for
 *         the USART asynchronous modes to generate a baud rate as close as
 *         possible to the baud rate set point.
 *
 * Baud rate calculation:
 * \f$ Baudrate = \frac{SelectedClock}{Over \times (CD + \frac{FP}{8})} \f$, \e Over being 16 or 8.
 * The maximal oversampling is selected if it allows to generate a baud rate close to the set point.
 *
 * \param usart     Base address of the USART instance.
 * \param baudrate  Baud rate set point.
 * \param pba_hz    USART module input clock frequency (PBA clock, Hz).
 *
 * \retval USART_SUCCESS        Baud rate successfully initialized.
 * \retval USART_INVALID_INPUT  Baud rate set point is out of range for the given input clock frequency.
 */
static int usart_set_async_baudrate(volatile avr32_usart_t *usart, unsigned int baudrate, unsigned long pba_hz)
{
  unsigned int over = (pba_hz >= 16 * baudrate) ? 16 : 8;
  unsigned int cd_fp = ((1 << AVR32_USART_BRGR_FP_SIZE) * pba_hz + (over * baudrate) / 2) / (over * baudrate);
  unsigned int cd = cd_fp >> AVR32_USART_BRGR_FP_SIZE;
  unsigned int fp = cd_fp & ((1 << AVR32_USART_BRGR_FP_SIZE) - 1);

  if (cd < 1 || cd > (1 << AVR32_USART_BRGR_CD_SIZE) - 1)
    return USART_INVALID_INPUT;

  usart->mr = (usart->mr & ~(AVR32_USART_MR_USCLKS_MASK |
                             AVR32_USART_MR_SYNC_MASK |
                             AVR32_USART_MR_OVER_MASK)) |
              AVR32_USART_MR_USCLKS_MCK << AVR32_USART_MR_USCLKS_OFFSET |
              ((over == 16) ? AVR32_USART_MR_OVER_X16 : AVR32_USART_MR_OVER_X8) << AVR32_USART_MR_OVER_OFFSET;

  usart->brgr = cd << AVR32_USART_BRGR_CD_OFFSET |
                fp << AVR32_USART_BRGR_FP_OFFSET;

  return USART_SUCCESS;
}


/*! \brief Calculates a clock divider (\e CD) for the USART synchronous master
 *         modes to generate a baud rate as close as possible to the baud rate
 *         set point.
 *
 * Baud rate calculation:
 * \f$ Baudrate = \frac{SelectedClock}{CD} \f$.
 *
 * \param usart     Base address of the USART instance.
 * \param baudrate  Baud rate set point.
 * \param pba_hz    USART module input clock frequency (PBA clock, Hz).
 *
 * \retval USART_SUCCESS        Baud rate successfully initialized.
 * \retval USART_INVALID_INPUT  Baud rate set point is out of range for the given input clock frequency.
 */
static int usart_set_sync_master_baudrate(volatile avr32_usart_t *usart, unsigned int baudrate, unsigned long pba_hz)
{
  unsigned int cd = (pba_hz + baudrate / 2) / baudrate;

  if (cd < 1 || cd > (1 << AVR32_USART_BRGR_CD_SIZE) - 1)
    return USART_INVALID_INPUT;

  usart->mr = (usart->mr & ~AVR32_USART_MR_USCLKS_MASK) |
              AVR32_USART_MR_USCLKS_MCK << AVR32_USART_MR_USCLKS_OFFSET |
              AVR32_USART_MR_SYNC_MASK;

  usart->brgr = cd << AVR32_USART_BRGR_CD_OFFSET;

  return USART_SUCCESS;
}

//------------------------------------------------------------------------------
/*! \name Initialization Functions
 */
//! @{


void usart_reset(volatile avr32_usart_t *usart)
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
#ifndef AVR32_USART_440_H_INCLUDED
// Note: Modem Signal Management DTR-DSR-DCD-RI are not included in USART rev.440.
              AVR32_USART_CR_DTRDIS_MASK  |
#endif
              AVR32_USART_CR_RTSDIS_MASK;
}


int usart_init_rs232(volatile avr32_usart_t *usart, const usart_options_t *opt, long pba_hz)
{
  // Reset the USART and shutdown TX and RX.
  usart_reset(usart);

  // Check input values.
  if (!opt || // Null pointer.
      opt->charlength < 5 || opt->charlength > 9 ||
      opt->paritytype > 7 ||
      opt->stopbits > 2 + 255 ||
      opt->channelmode > 3 ||
      usart_set_async_baudrate(usart, opt->baudrate, pba_hz) == USART_INVALID_INPUT)
    return USART_INVALID_INPUT;

  if (opt->charlength == 9)
  {
    // Character length set to 9 bits. MODE9 dominates CHRL.
    usart->mr |= AVR32_USART_MR_MODE9_MASK;
  }
  else
  {
    // CHRL gives the character length (- 5) when MODE9 = 0.
    usart->mr |= (opt->charlength - 5) << AVR32_USART_MR_CHRL_OFFSET;
  }

  usart->mr |= opt->paritytype << AVR32_USART_MR_PAR_OFFSET |
               opt->channelmode << AVR32_USART_MR_CHMODE_OFFSET;

  if (opt->stopbits > USART_2_STOPBITS)
  {
    // Set two stop bits
    usart->mr |= AVR32_USART_MR_NBSTOP_2 << AVR32_USART_MR_NBSTOP_OFFSET;
    // and a timeguard period gives the rest.
    usart->ttgr = opt->stopbits - USART_2_STOPBITS;
  }
  else
    // Insert 1, 1.5 or 2 stop bits.
    usart->mr |= opt->stopbits << AVR32_USART_MR_NBSTOP_OFFSET;

  // Set normal mode.
  usart->mr = (usart->mr & ~AVR32_USART_MR_MODE_MASK) |
              AVR32_USART_MR_MODE_NORMAL << AVR32_USART_MR_MODE_OFFSET;

  // Setup complete; enable communication.
  // Enable input and output.
  usart->cr = AVR32_USART_CR_RXEN_MASK |
              AVR32_USART_CR_TXEN_MASK;

  return USART_SUCCESS;
}


int usart_init_hw_handshaking(volatile avr32_usart_t *usart, const usart_options_t *opt, long pba_hz)
{
  // First: Setup standard RS232.
  if (usart_init_rs232(usart, opt, pba_hz) == USART_INVALID_INPUT)
    return USART_INVALID_INPUT;

  // Set hardware handshaking mode.
  usart->mr = (usart->mr & ~AVR32_USART_MR_MODE_MASK) |
              AVR32_USART_MR_MODE_HARDWARE << AVR32_USART_MR_MODE_OFFSET;

  return USART_SUCCESS;
}

int usart_init_sync_master(volatile avr32_usart_t *usart, const usart_options_t *opt, long pba_hz)
{
  // Reset the USART and shutdown TX and RX.
  usart_reset(usart);

  // Check input values.
  if (!opt || // Null pointer.
      opt->charlength < 5 || opt->charlength > 9 ||
      opt->paritytype > 7 ||
      opt->stopbits == 1 || opt->stopbits > 2 + 255 ||
      opt->channelmode > 3 ||
      usart_set_sync_master_baudrate(usart, opt->baudrate, pba_hz) == USART_INVALID_INPUT)
    return USART_INVALID_INPUT;

  if (opt->charlength == 9)
  {
    // Character length set to 9 bits. MODE9 dominates CHRL.
    usart->mr |= AVR32_USART_MR_MODE9_MASK;
  }
  else
  {
    // CHRL gives the character length (- 5) when MODE9 = 0.
    usart->mr |= (opt->charlength - 5) << AVR32_USART_MR_CHRL_OFFSET;
  }

  usart->mr |= opt->paritytype << AVR32_USART_MR_PAR_OFFSET |
               opt->channelmode << AVR32_USART_MR_CHMODE_OFFSET;

  if (opt->stopbits > USART_2_STOPBITS)
  {
    // Set two stop bits
    usart->mr |= AVR32_USART_MR_NBSTOP_2 << AVR32_USART_MR_NBSTOP_OFFSET;
    // and a timeguard period gives the rest.
    usart->ttgr = opt->stopbits - USART_2_STOPBITS;
  }
  else
    // Insert 1 or 2 stop bits.
    usart->mr |= opt->stopbits << AVR32_USART_MR_NBSTOP_OFFSET;

  // Set normal mode.
  usart->mr = (usart->mr & ~AVR32_USART_MR_MODE_MASK) |
              AVR32_USART_MR_MODE_NORMAL << AVR32_USART_MR_MODE_OFFSET |
              AVR32_USART_MR_CLKO_MASK;

  // Setup complete; enable communication.
  // Enable input and output.
  usart->cr = AVR32_USART_CR_RXEN_MASK |
              AVR32_USART_CR_TXEN_MASK;

  return USART_SUCCESS;
}

//------------------------------------------------------------------------------
/*! \name Transmit/Receive Functions
 */
//! @{


int usart_send_address(volatile avr32_usart_t *usart, int address)
{
  // Check if USART is in multidrop / RS485 mode.
  if (!usart_mode_is_multidrop(usart)) return USART_MODE_FAULT;

  // Prepare to send an address.
  usart->cr = AVR32_USART_CR_SENDA_MASK;

  // Write the address to TX.
  usart_bw_write_char(usart, address);

  return USART_SUCCESS;
}


int usart_write_char(volatile avr32_usart_t *usart, int c)
{
  if (usart_tx_ready(usart))
  {
    usart->thr = (c << AVR32_USART_THR_TXCHR_OFFSET) & AVR32_USART_THR_TXCHR_MASK;
    return USART_SUCCESS;
  }
  else
    return USART_TX_BUSY;
}


int usart_putchar(volatile avr32_usart_t *usart, int c)
{
  int timeout = USART_DEFAULT_TIMEOUT;

  do
  {
    if (!timeout--) return USART_FAILURE;
  } while (usart_write_char(usart, c) != USART_SUCCESS);

  return USART_SUCCESS;
}


int usart_read_char(volatile avr32_usart_t *usart, int *c)
{
  // Check for errors: frame, parity and overrun. In RS485 mode, a parity error
  // would mean that an address char has been received.
  if (usart->csr & (AVR32_USART_CSR_OVRE_MASK |
                    AVR32_USART_CSR_FRAME_MASK |
                    AVR32_USART_CSR_PARE_MASK))
    return USART_RX_ERROR;

  // No error; if we really did receive a char, read it and return SUCCESS.
  if (usart_test_hit(usart))
  {
    *c = (usart->rhr & AVR32_USART_RHR_RXCHR_MASK) >> AVR32_USART_RHR_RXCHR_OFFSET;
    return USART_SUCCESS;
  }
  else
    return USART_RX_EMPTY;
}


int usart_getchar(volatile avr32_usart_t *usart)
{
  int c, ret;

  while ((ret = usart_read_char(usart, &c)) == USART_RX_EMPTY);

  if (ret == USART_RX_ERROR)
    return USART_FAILURE;

  return c;
}


void usart_write_line(volatile avr32_usart_t *usart, const char *string)
{
  while (*string != '\0')
    usart_putchar(usart, *string++);
}


int usart_get_echo_line(volatile avr32_usart_t *usart)
{
  int rx_char;
  int retval = USART_SUCCESS;

  while (1)
  {
    rx_char = usart_getchar(usart);
    if (rx_char == USART_FAILURE)
    {
      usart_write_line(usart, "Error!!!\r\n");
      retval = USART_FAILURE;
      break;
    }
    if (rx_char == '\x03')
    {
      retval = USART_FAILURE;
      break;
    }
    usart_putchar(usart, rx_char);
    if (rx_char == '\r')
    { // Add a LF and consider this as the end of the line.
      usart_putchar(usart, '\n');
      break;
    }
  }

  return retval;
}

/*@ This function is for printing non constant strings. */
extern void usart_write_string(volatile avr32_usart_t *usart, char *string)
	{
	uint16_t char_index = 0;
	while(string[char_index] != 0) usart_putchar(usart, string[char_index]);
	}

/*@ Prints selected segment as text from storage to terminal. */
extern void usart_write_from_storage(volatile avr32_usart_t *usart, uint8_t segment_id)
	{
	uint8_t char_temp = 0;
	uint16_t char_index = 0;
	
	do 
		{
		storage_read_segment(segment_id, char_index, &char_temp);
		if(char_temp != 0xFF) usart_putchar(usart, char_temp);
		char_index++;
		} while (char_temp != 0xFF && char_temp != 0x00);
	}

/*@Degug message function, prints variable as bit value to terminal */
extern void usart_write_bits (volatile avr32_usart_t *usart, int32_t bit_value, uint8_t print_length)
	{
	int8_t bit_shift = print_length;
	
	while (bit_shift >= 0)
		{
		/* (bit_value>>bit_shift)&0x0001 gives 0 or 1 as result debending shifted
		 * bit. By adding 48 we move it to number area in ASCII table. */
		 usart_putchar(usart, (((bit_value>>bit_shift)&0x0001)+48));	
		
		/* Print space by every 4 bits, makes reading much easier. */
		if (bit_shift%4 == 0)
			{
			 usart_putchar(usart, ASCII_SPACE);
			}
		
		bit_shift--;
		}		
	}

/*@Degug message function, prints variable as hex value to terminal */
extern void usart_write_hex (volatile avr32_usart_t *usart, int32_t hex_value, uint8_t print_length)
	{
	int32_t hex_value_temp = 0;
	int8_t bit_shift = print_length-4;
	
	usart_write_line(usart, "0x");
	
	while(bit_shift >= 0)
		{
		/* First shift bits (from 12->0, 4 bit steps). In this order because 
		 * most markable bits must be printed first to terminal. */
		hex_value_temp = hex_value>>bit_shift;
		
		/* After shift, 4 last binary numbers are one character,
		 * which should get printed. So we mask rest than last 4 markable
		 * bits out. */
		hex_value_temp = hex_value_temp&0x000F;
		
		/* If character is bigger than 9 (A-F letter) add number 65
		 * to move it correct point in ASCII table. (point where A letter is). */
		if(hex_value_temp > 9)
			{
			hex_value_temp += 65-10;	
			}
		/* If less than 9, add number 48 to print it as number from ASCII table */
		else
			{
			hex_value_temp += 48;	
			}
		
		usart_putchar(usart, hex_value_temp);
		
		bit_shift -= 4;
		}
	}
	
void usart_write_decimal(volatile avr32_usart_t *usart, uint32_t int_value)
	{
	/* Because maximum value of uint32 is 4294967296, it means we need 10 char
	 * string at most. */
	char string_temp[10] = {0};
	uint8_t string_index = 0;
	
	/* Now we make reversed string version of
	 * number value. */
	do 
		{
		string_temp[string_index] = int_value%10;
		string_index++;
		
		/* Divide by ten, as long as there are number left */
		int_value = int_value/10;
		
		} while (int_value > 0);
	
	/* String is in reversed order now, lets print it. */
	do 
		{
		string_index--;
		usart_putchar(usart, string_temp[string_index]+48);
		} while (string_index != 0);
	}