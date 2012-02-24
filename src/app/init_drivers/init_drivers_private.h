/*
 * init_drivers_private.h
 *
 * Created: 24.2.2012 0:37:50
 *  Author: savpek
 */


#ifndef INIT_DRIVERS_PRIVATE_H_
#define INIT_DRIVERS_PRIVATE_H_

/*! @brief inits MCU clocks, see clocks_config.h from ../config/. */
extern errorc_t init_driver_clocks( void);

/*! @brief inits usart, see usart_config.h from ../config/. */
extern errorc_t init_driver_usart( void);

/*! @brief inits gpio (all ports from MCU), see gpio_config.h from ../config/. */
extern errorc_t init_driver_gpio( void);

/*! @brief inits pwma, see pwma_config.h from ../config/. */
extern errorc_t init_driver_pwma ( void);

#endif /* INIT_DRIVERS_PRIVATE_H_ */