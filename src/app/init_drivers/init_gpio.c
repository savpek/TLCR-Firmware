#include "compiler.h"
#include "./init_drivers/private/init_drivers_private.h"
#include "./init_drivers/config/gpio_config.h"
#include "./gpio/public/gpio.h"

#define CONFIG_PIN(pin_name) \
	gpio_configure_pin(	AVR32_PIN_##pin_name, INITD_GPIO_##pin_name##_CONFIG )

#define FUNCTION_PIN(pin_name) \
	gpio_enable_module_pin(	AVR32_PIN_##pin_name,	INITD_GPIO_##pin_name##_FUNCTION)

/*! @brief inits gpio (all ports from MCU), see gpio_config.h from ../config/. */
errorc_t init_driver_gpio( void) {
	/* For some reason, you should not configure certain pins after they have set function.
	 * For example, USART TX stop working if you do so... */
	#ifdef INITD_GPIO_PA00_FUNCTION
		FUNCTION_PIN(PA00);
		#ifdef INITD_GPIO_PA00_CONFIG
			CONFIG_PIN(PA00);
		#endif
	#endif

	#ifdef INITD_GPIO_PA01_FUNCTION
		FUNCTION_PIN(PA01);
		#ifdef INITD_GPIO_PA01_CONFIG
			CONFIG_PIN(PA01);
		#endif
	#endif

	#ifdef INITD_GPIO_PA02_FUNCTION
		FUNCTION_PIN(PA02);
		#ifdef INITD_GPIO_PA02_CONFIG
			CONFIG_PIN(PA02);
		#endif
	#endif

	#ifdef INITD_GPIO_PA03_FUNCTION
		FUNCTION_PIN(PA03);
		#ifdef INITD_GPIO_PA03_CONFIG
			CONFIG_PIN(PA03);
		#endif
	#endif

	#ifdef INITD_GPIO_PA04_FUNCTION
		FUNCTION_PIN(PA04);
		#ifdef INITD_GPIO_PA04_CONFIG
			CONFIG_PIN(PA04);
		#endif
	#endif

	#ifdef INITD_GPIO_PA05_FUNCTION
		FUNCTION_PIN(PA05);
		#ifdef INITD_GPIO_PA05_CONFIG
			CONFIG_PIN(PA05);
		#endif
	#endif

	#ifdef INITD_GPIO_PA06_FUNCTION
		FUNCTION_PIN(PA06);
		#ifdef INITD_GPIO_PA06_CONFIG
			CONFIG_PIN(PA06);
		#endif
	#endif

	#ifdef INITD_GPIO_PA07_FUNCTION
		FUNCTION_PIN(PA07);
		#ifdef INITD_GPIO_PA07_CONFIG
			CONFIG_PIN(PA07);
		#endif
	#endif

	#ifdef INITD_GPIO_PA08_FUNCTION
		FUNCTION_PIN(PA08);
		#ifdef INITD_GPIO_PA08_CONFIG
			CONFIG_PIN(PA08);
		#endif
	#endif

	#ifdef INITD_GPIO_PA09_FUNCTION
		FUNCTION_PIN(PA09);
		#ifdef INITD_GPIO_PA09_CONFIG
			CONFIG_PIN(PA09);
		#endif
	#endif

	#ifdef INITD_GPIO_PA10_FUNCTION
		FUNCTION_PIN(PA10);
		#ifdef INITD_GPIO_PA10_CONFIG
			CONFIG_PIN(PA10);
		#endif
	#endif

	#ifdef INITD_GPIO_PA11_FUNCTION
		FUNCTION_PIN(PA11);
		#ifdef INITD_GPIO_PA11_CONFIG
			CONFIG_PIN(PA11);
		#endif
	#endif

	#ifdef INITD_GPIO_PA12_FUNCTION
		FUNCTION_PIN(PA12);
		#ifdef INITD_GPIO_PA12_CONFIG
			CONFIG_PIN(PA12);
		#endif
	#endif

	#ifdef INITD_GPIO_PA13_FUNCTION
		FUNCTION_PIN(PA13);
		#ifdef INITD_GPIO_PA13_CONFIG
			CONFIG_PIN(PA13);
		#endif
	#endif

	#ifdef INITD_GPIO_PA14_FUNCTION
		FUNCTION_PIN(PA14);
		#ifdef INITD_GPIO_PA14_CONFIG
			CONFIG_PIN(PA14);
		#endif
	#endif

	#ifdef INITD_GPIO_PA15_FUNCTION
		FUNCTION_PIN(PA15);
		#ifdef INITD_GPIO_PA15_CONFIG
			CONFIG_PIN(PA15);
		#endif
	#endif

	#ifdef INITD_GPIO_PA16_FUNCTION
		FUNCTION_PIN(PA16);
		#ifdef INITD_GPIO_PA16_CONFIG
			CONFIG_PIN(PA16);
		#endif
	#endif

	#ifdef INITD_GPIO_PA17_FUNCTION
		FUNCTION_PIN(PA17);
		#ifdef INITD_GPIO_PA17_CONFIG
			CONFIG_PIN(PA17);
		#endif
	#endif

	#ifdef INITD_GPIO_PA18_FUNCTION
		FUNCTION_PIN(PA18);
		#ifdef INITD_GPIO_PA18_CONFIG
			CONFIG_PIN(PA18);
		#endif
	#endif

	#ifdef INITD_GPIO_PA19_FUNCTION
		FUNCTION_PIN(PA19);
		#ifdef INITD_GPIO_PA19_CONFIG
			CONFIG_PIN(PA19);
		#endif
	#endif

	#ifdef INITD_GPIO_PA20_FUNCTION
		FUNCTION_PIN(PA20);
		#ifdef INITD_GPIO_PA20_CONFIG
			CONFIG_PIN(PA20);
		#endif
	#endif

	#ifdef INITD_GPIO_PA21_FUNCTION
		FUNCTION_PIN(PA21);
		#ifdef INITD_GPIO_PA21_CONFIG
			CONFIG_PIN(PA21);
		#endif
	#endif

	#ifdef INITD_GPIO_PA22_FUNCTION
		FUNCTION_PIN(PA22);
		#ifdef INITD_GPIO_PA22_CONFIG
			CONFIG_PIN(PA22);
		#endif
	#endif

	#ifdef INITD_GPIO_PB00_FUNCTION
		FUNCTION_PIN(PB00);
		#ifdef INITD_GPIO_PB00_CONFIG
			CONFIG_PIN(PB00);
		#endif
	#endif

	#ifdef INITD_GPIO_PB01_FUNCTION
		FUNCTION_PIN(PB01);
		#ifdef INITD_GPIO_PB01_CONFIG
			CONFIG_PIN(PB01);
		#endif
	#endif

	#ifdef INITD_GPIO_PB02_FUNCTION
		FUNCTION_PIN(PB02);
		#ifdef INITD_GPIO_PB02_CONFIG
			CONFIG_PIN(PB02);
		#endif
	#endif

	#ifdef INITD_GPIO_PB03_FUNCTION
		FUNCTION_PIN(PB03);
		#ifdef INITD_GPIO_PB03_CONFIG
			CONFIG_PIN(PB03);
		#endif
	#endif

	#ifdef INITD_GPIO_PB04_FUNCTION
		FUNCTION_PIN(PB04);
		#ifdef INITD_GPIO_PB04_CONFIG
			CONFIG_PIN(PB04);
		#endif
	#endif

	#ifdef INITD_GPIO_PB05_FUNCTION
		FUNCTION_PIN(PB05);
		#ifdef INITD_GPIO_PB05_CONFIG
			CONFIG_PIN(PB05);
		#endif
	#endif

	#ifdef INITD_GPIO_PB06_FUNCTION
		FUNCTION_PIN(PB06);
		#ifdef INITD_GPIO_PB06_CONFIG
			CONFIG_PIN(PB06);
		#endif
	#endif

	#ifdef INITD_GPIO_PB07_FUNCTION
		FUNCTION_PIN(PB07);
		#ifdef INITD_GPIO_PB07_CONFIG
			CONFIG_PIN(PB07);
		#endif
	#endif

	#ifdef INITD_GPIO_PB08_FUNCTION
		FUNCTION_PIN(PB08);
		#ifdef INITD_GPIO_PB08_CONFIG
			CONFIG_PIN(PB08);
		#endif
	#endif

	#ifdef INITD_GPIO_PB09_FUNCTION
		FUNCTION_PIN(PB09);
		#ifdef INITD_GPIO_PB09_CONFIG
			CONFIG_PIN(PB09);
		#endif
	#endif

	#ifdef INITD_GPIO_PB10_FUNCTION
		FUNCTION_PIN(PB10);
		#ifdef INITD_GPIO_PB10_CONFIG
			CONFIG_PIN(PB10);
		#endif
	#endif

	#ifdef INITD_GPIO_PB11_FUNCTION
		FUNCTION_PIN(PB11);
		#ifdef INITD_GPIO_PB11_CONFIG
			CONFIG_PIN(PB11);
		#endif
	#endif

	#ifdef INITD_GPIO_PB12_FUNCTION
		FUNCTION_PIN(PB12);
		#ifdef INITD_GPIO_PB12_CONFIG
			CONFIG_PIN(PB12);
		#endif
	#endif
}
