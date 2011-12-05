/*
 * mcu.c
 *
 *  Created on: 27.7.2011
 *      Author: savpek
 */

/* ASF */
#include "compiler.h"
#include "scif_uc3l.h"
#include "power_clocks_lib.h"
#include "gpio.h"

/* Lib internal headers */
#include "./api/mcu/inc/mcu.h"
#include "./api/mcu/config/mcu_config.h"

extern void mcu_init_clocks()
	{
	/* Set reguired changes for flash wait state and readmode */
	flashcdw_set_flash_waitstate_and_readmode(MCU_CPU_F);
		
	/* We will use Atmels power_clocks_lib to configure clocks in MCU. This library supports
	 * high level of abstraction over settings. */
	static scif_gclk_opt_t gc_dfllif_ref_opt = { SCIF_GCCTRL_SLOWCLOCK, 0, false };
	static pcl_freq_param_t pcl_dfll_freq_param =
		{
		.main_clk_src = PCL_MC_DFLL0,
		.cpu_f        = MCU_CPU_F,
		.pba_f        = MCU_PBA_F,
		.pbb_f        = MCU_PBB_F,
		.dfll_f       = MCU_DFLL_F,
		.pextra_params = &gc_dfllif_ref_opt
		};

	/* Configure core clocks (MCU, PBA, PBB, DFFL */
	pcl_configure_clocks(&pcl_dfll_freq_param);
	}
