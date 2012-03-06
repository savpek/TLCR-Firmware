/*
 * init_drivers_private.c
 *
 * Created: 24.2.2012 11:15:57
 *  Author: savpek
 */

#include "./init_drivers/private/init_drivers_private.h"
#include "./init_drivers/config/usart_config.h"
#include "./init_drivers/config/clocks_config.h"
#include "./init_drivers/config/gpio_config.h"
#include "./gpio/public/gpio.h"

#include "scif_uc3l.h"
#include "pm_uc3l.h"

#include "./driver/usart/public/usart.h"
#include "power_clocks_lib.h"

/*! @brief inits MCU clocks, see clocks_config.h from ../config/. */
errorc_t init_driver_clocks( void) {

	/* Set reguired changes for flash wait state and readmode */
	flashcdw_set_flash_waitstate_and_readmode(INITD_CPU_F);

	/* We will use Atmels power_clocks_lib to configure clocks in MCU. This library supports
	 * high level of abstraction over settings. */
	static scif_gclk_opt_t gc_dfllif_ref_opt = { SCIF_GCCTRL_SLOWCLOCK, 0, false };
	static pcl_freq_param_t pcl_dfll_freq_param =
		{
		.main_clk_src = PCL_MC_DFLL0,
		.cpu_f        = INITD_CPU_F,
		.pba_f        = INITD_PBA_F,
		.pbb_f        = INITD_PBB_F,
		.dfll_f       = INITD_DFLL_F,
		.pextra_params = &gc_dfllif_ref_opt
		};

	/* Configure core clocks (MCU, PBA, PBB, DFFL */
	pcl_configure_clocks(&pcl_dfll_freq_param);
}

/*! @brief inits usart, see usart_config.h from ../config/. */
errorc_t init_driver_usart( void) {
	const usart_options_t usart_settings = {
		.baudrate = INITD_USART_BAUDRATE,
		.charlength = INITD_USART_CHARLENGTH,
		.paritytype = INITD_USART_PARITYPE,
		.stopbits = INITD_USART_STOPBITS,
		.channelmode = INITD_USART_CHANNELMODE,
	};

	usart_init_rs232(INITD_USART_POINTER, &usart_settings, INITD_PBA_F);
}

/*! @brief inits pwma, see pwma_config.h from ../config/. */
errorc_t init_driver_pwma ( void);