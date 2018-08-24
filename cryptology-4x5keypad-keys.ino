#include <Keyboard.h>
#include <AStar32U4.h>
#include <TaskAction.h>
#include <Keypad.h>

#include <EEPROMex.h>

#include "game_keypad.h"

static byte s_new_key = false;

static const uint8_t SERIAL_BUFFER_LENGTH = 32;
static char s_serial_buffer[SERIAL_BUFFER_LENGTH];
static uint8_t s_serial_idx = 0;

static const uint8_t LED_PIN = 13;
static const uint8_t LAYOUT_SELECT_PIN = 9;
static LAYOUT s_layout;

static int s_debounce_time_address;
static uint16_t s_debounce_time;

static void process_debounce_time_setting(char const * const cmd)
{
    char * end;
    long debounce_time = strtol(cmd, &end, 10);
    if (end > cmd)
    {
        if ((debounce_time > 10) && (debounce_time < 500))
        {
            s_debounce_time = debounce_time;
            EEPROM.writeInt(s_debounce_time_address, s_debounce_time);
            keypad_set_debounce_time(debounce_time);
            Serial.print("Set debounce time to ");
            Serial.println(s_debounce_time);
        }
    }
}

static void reset_buffer()
{
    s_serial_buffer[0] = '\0';
    s_serial_idx = 0;
}

static void process_command(char const * const cmd)
{
    if ((cmd[0] == 'S') && (cmd[1] == '0'))
    {
        process_debounce_time_setting(&cmd[2]);
    }    
}

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
    Serial.print("Debounce time: ");
    Serial.print(s_debounce_time);
    Serial.print("ms");
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

    s_debounce_time_address = EEPROM.getAddress(sizeof(uint16_t));
    s_debounce_time = EEPROM.readInt(s_debounce_time_address);

    keypad_setup(s_new_key, s_layout);
    keypad_set_debounce_time(s_debounce_time);

    reset_buffer();
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

void serialEvent()
{
    while(Serial.available())
    {
        char c = Serial.read();
        if (c != '\n')
        {
            if (s_serial_idx < SERIAL_BUFFER_LENGTH)
            {
                s_serial_buffer[s_serial_idx++] = c;
                s_serial_buffer[s_serial_idx] = '\0';
            }
        }
        else
        {
            Serial.print("Got command ");
            Serial.println(s_serial_buffer);
            process_command(s_serial_buffer);
            reset_buffer();
        }
    }
}

void serialEventRun(void) {
    if (Serial.available()) serialEvent();
}
