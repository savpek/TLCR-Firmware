/*
 * terminal_program_motor.c
 *
 * Created: 12.12.2011 16:41:37
 *  Author: savpek
 */ 

#include "compiler.h"

/* Terminal internal */
#include "./app/terminal/config/terminal_program_list.h"
#include "./api/terminalapi/inc/terminalapi.h"

#include "./driver/motor/inc/motor.h"

volatile motor_t terminal_test_motor =
	{
	/* Define physical pins for PWMA */
	.a_pos_pwma_pin = AVR32_PWMA_28_PIN,	/* Often RED wire*/
	.a_neg_pwma_pin = AVR32_PWMA_29_PIN,	/* Often BLUE wire*/
	.b_pos_pwma_pin = AVR32_PWMA_30_PIN,	/* Often YELLOW wire*/
	.b_neg_pwma_pin = AVR32_PWMA_31_PIN,	/* Often WHITE wire*/

		/* Define PWMA channel for each coil pin */
	.a_pos_pwma_chan = 28,
	.a_neg_pwma_chan = 29,
	.b_pos_pwma_chan = 30,
	.b_neg_pwma_chan = 31,
	.pwma_function = AVR32_PWMA_15_FUNCTION,	
	};

void terminal_program_motor_init(terminalapi_cmd_t *cmd_struct)
	{
	motor_init(&terminal_test_motor);
	}
	
void terminal_program_motor_use(terminalapi_cmd_t *cmd_struct)
	{
	int32_t delay_value_ms = 0;
	if(!terminalapi_try_get_int32(cmd_struct, &delay_value_ms)) return;

	if(delay_value_ms > 0)
		{
		motor_step_delay(&terminal_test_motor, (uint32_t)delay_value_ms, MOTOR_CW);
		}
	else if (delay_value_ms < 0)
		{
		motor_step_delay(&terminal_test_motor, (uint32_t)(delay_value_ms*-1), MOTOR_COUNTERCW);
		}
	else
		{
		motor_step_delay(&terminal_test_motor, (uint32_t)(delay_value_ms), MOTOR_STOP);
		}
	}