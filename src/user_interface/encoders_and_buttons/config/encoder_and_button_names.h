/*
 * encoder_and_button_names.h
 *
 * Created: 2.10.2011 14:09:08
 *  Author: savpek
 */ 


#ifndef ENCODER_AND_BUTTON_NAMES_H_
#define ENCODER_AND_BUTTON_NAMES_H_

/* Here are names for encoders and buttons,
 * Name means defination that points handler of
 * button or encoder, for example:
 * encoder_handle[ENCODER_NAME_X_SPEED] or
 * button_handle[BUTTON_NAME_CHANGE_MODE] */
#define ENCODER_NAME_X_SPEED 0
#define ENCODER_NAME_ROT_SPEED 1
#define ENCODER_NAME_MODE 2

#define BUTTON_NAME_ENC_1 0
#define BUTTON_NAME_LIMIT_SW_LEFT 3
#define BUTTON_NAME_LIMIT_SW_RIGHT 4

#endif /* ENCODER_AND_BUTTON_NAMES_H_ */