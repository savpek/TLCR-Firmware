/*
 * utest_config.h
 *
 * Created: 18.3.2012 23:11:39
 *  Author: savpek
 */


#ifndef UTEST_CONFIG_H_
#define UTEST_CONFIG_H_

/* Define output function which framework should use! */
#define UTEST_OUTPUT_CHAR(a) usart_putchar(a)

/* Undefine if you don't want test case names printed while running. */
#define UTEST_VERBOSE

#endif /* UTEST_CONFIG_H_ */