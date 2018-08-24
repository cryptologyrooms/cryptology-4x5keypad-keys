#include <Keyboard.h>
#include <AStar32U4.h>
#include <TaskAction.h>
#include <Keypad.h>

#include "game_keypad.h"

static byte s_new_key = false;

static const uint8_t LED_PIN = 13;
static const uint8_t LAYOUT_SELECT_PIN = 9;

static LAYOUT s_layout;

static void debug_task_fn(TaskAction * pThisTask)
{
    (void)pThisTask;
    static bool s_led = false;
    digitalWrite(LED_PIN, s_led = !s_led);
}
static TaskAction s_debug_task(debug_task_fn, 500, INFINITE_TICKS);

static void post_setup_task_fn(TaskAction * pThisTask)
{
    (void)pThisTask;
    Serial.print("Using layout ");
    Serial.println(s_layout+1);
}
static TaskAction s_post_setup_task(post_setup_task_fn, 5000, 1);

static byte check_and_clear(byte& key)
{
    byte value = key;
    cli();
    key = '\0';
    sei();
    return value;
}

void setup()
{
    Serial.begin(115200);

    pinMode(LED_PIN, OUTPUT);
    pinMode(LAYOUT_SELECT_PIN, INPUT_PULLUP);
    
    Keyboard.begin();

    delay(300);
    s_layout = digitalRead(LAYOUT_SELECT_PIN) == HIGH ? LAYOUT_1 : LAYOUT_2;

    keypad_setup(s_new_key, s_layout);
}

void loop()
{
    keypad_tick();
    
    char key;
    if ((key = check_and_clear(s_new_key)))
    {
        Serial.println(key);
        Keyboard.press(key);
        Keyboard.release(key);
    }

    s_debug_task.tick();
    s_post_setup_task.tick();
}
