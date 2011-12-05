/*
 * subprog_testf.c
 *
 * Created: 4.12.2011 0:03:37
 *  Author: savpek
 */ 
#include "compiler.h"

#include "./app/terminal/common/common_functions/inc/terminal_common_functions.h"
#include "./app/terminal/subprograms/config/subprogram_list.h"
#include "./app/terminal/inc/terminal.h"

void subprog_test_get_int(char* arg_str)
	{
	int32_t test_int = 0;
	terminal_try_get_int_value (arg_str, &test_int);

	asm("nop");
	}
	