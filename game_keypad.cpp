/* Arduino Library Includes */

#include <Keyboard.h>
#include <Keypad.h>
#include "TaskAction.h"

/* Application Includes */

#include "game_keypad.h"

/* Defines, typedefs, constants */

/* Private Variables */
static char *s_last_key = 0;

static const byte LAYOUT_1_ROWS = 5;
static const byte LAYOUT_1_COLS = 4;

static char s_layout_1_keys[LAYOUT_1_ROWS][LAYOUT_1_COLS] = {
    {'T', 'G', 'B', 'L'},
    {'R', 'F', 'V', 'K'},
    {'E', 'D', 'C', 'P'},
    {'W', 'S', 'X', 'O'},
    {'Q', 'A', 'Z', 'I'},
};
static byte LAYOUT_1_ROW_PINS[LAYOUT_1_ROWS] = {2,3,4,5,6};
static byte LAYOUT_1_COL_PINS[LAYOUT_1_COLS] = {10,16,14,15};

static const byte LAYOUT_2_ROWS = 4;
static const byte LAYOUT_2_COLS = 5;

static char s_layout_2_keys[LAYOUT_2_ROWS][LAYOUT_2_COLS] = {
    {'Q', 'W', 'E', 'R', 'T'},
    {'A', 'S', 'D', 'F', 'G'},
    {'Z', 'X', 'C', 'V', 'B'},
    {'I', 'O', 'P', 'K', 'L'}
};

static byte LAYOUT_2_ROW_PINS[LAYOUT_2_ROWS] = {2,3,4,5};
static byte LAYOUT_2_COL_PINS[LAYOUT_2_COLS] = {10,16,14,15,A0};

static Keypad * sp_keypad;

/* Private Functions */

/* Public Functions */

void keypad_setup(byte& key, LAYOUT layout)
{
    s_last_key = &key;

    if (layout == LAYOUT_1)
    {
        sp_keypad = new Keypad(
            makeKeymap(s_layout_1_keys), LAYOUT_1_ROW_PINS, LAYOUT_1_COL_PINS, LAYOUT_1_ROWS, LAYOUT_1_COLS
        );
    }
    else if (layout == LAYOUT_2)
    {
        sp_keypad = new Keypad(
            makeKeymap(s_layout_2_keys), LAYOUT_2_ROW_PINS, LAYOUT_2_COL_PINS, LAYOUT_2_ROWS, LAYOUT_2_COLS
        );
    }
}

void keypad_tick()
{
    if (sp_keypad)
    {
        byte key = sp_keypad->getKey();

        if (key)
        {
            Serial.println(key);
            *s_last_key = key;
        }
    }
}

byte keypad_get()
{
    return *s_last_key;
}
