/****************************************************************
	Readme file:
		Timelapse device.
	Company:
		Savpek Electronics
	Author:
		Pekka Savolainen

	Shortly:
		Firmware for Timelapse camera move and capture device.
		Includes possibility to move and rotate camera with 
		high accuaracy and long time periods.

*****************************************************************/




Source files shortly:
	error_codes.h, includes error codes and typedef for errors.
	terminal.h, includes terminal, which can be used through serial interface.
	mcu.h, init processor.
	storage.h, service to save data with ease to flash chip.

Function naming:
	Static private functions begin with "_", _function for example.
	Global extern functions which meant to be used outside of library are named normally,
	with exeption that they have library name in their name (like terminal_init()).

Build notes:
	Project is made in AVR studio 5 environment. Compiler is AVR32-GNU/GCC compiler.
	MCU used is AT32UC3L32. 