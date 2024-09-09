#include "my_library.h"

void debug_printf_definition(char *string) { Serial.print(string); }

/* 
 * input -300mV - 300mV 
 * output 0V - 3.3V (0 - 1023)
 */

/* RL is common electrode (yellow) */
#define EMG_PIN_INPUT (14)
#define EMG_PIN_LEAD_OFF_CATHODE (11) /* high when RA (red) electrode is disconnected */
#define EMG_PIN_LEAD_OFF_ANODE (12) /* high when LA (green) electrode is disconnected */
#define EMG_TIME_INTERVAL_PRINT (50LU) /* ms */

#define OUTPUT_MAX (1008)
#define OUTPUT_MIN (0)
#define MAPPED_MAX (300)
#define MAPPED_MIN (-300)

static uint32_t emg_input_value_sum;
static uint32_t emg_input_value_counter;
static uint32_t emg_time_interval_print_last;
static int32_t emg_output_value;

void setup(void)
{
    Serial.begin(9600);

    pinMode(INPUT, EMG_PIN_INPUT);
    pinMode(INPUT, EMG_PIN_LEAD_OFF_CATHODE);
    pinMode(INPUT, EMG_PIN_LEAD_OFF_ANODE);

    analogReference(EXTERNAL);

    emg_time_interval_print_last = millis();
}

void loop(void)
{
    /* leads disconnected */
    if(digitalRead(EMG_PIN_LEAD_OFF_CATHODE) || digitalRead(EMG_PIN_LEAD_OFF_ANODE))
        return;

    emg_input_value_sum += analogRead(EMG_PIN_INPUT);
    emg_input_value_counter++;

    if(millis() - emg_time_interval_print_last >= EMG_TIME_INTERVAL_PRINT)
    {
        Serial.print(MAPPED_MAX);
        Serial.print(",");
        Serial.print(MAPPED_MIN);
        Serial.print(",");
        
        emg_output_value = constrain(emg_input_value_sum / emg_input_value_counter, OUTPUT_MIN, OUTPUT_MAX);
        emg_output_value = map(emg_output_value, OUTPUT_MIN, OUTPUT_MAX, MAPPED_MIN, MAPPED_MAX);
        Serial.println(emg_output_value);

        emg_input_value_sum = emg_input_value_counter = 0;
        emg_time_interval_print_last = millis();
    }
}
