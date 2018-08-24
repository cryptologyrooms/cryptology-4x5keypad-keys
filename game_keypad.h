#ifndef _GAME_KEYPAD_H_
#define _GAME_KEYPAD_H_

typedef enum layout
{
	LAYOUT_1,
	LAYOUT_2
} LAYOUT;

void keypad_setup(byte& key, LAYOUT layout);
void keypad_tick();

byte keypad_get();

#endif
