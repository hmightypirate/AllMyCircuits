#include "notes.h"

uint32_t beep_remaining = 0;

// Enable buzzer in pin B4
void setup_buzzer(void) {
    rcc_periph_clock_enable(RCC_AFIO);
    /* Enable the alternate GPIO for output*/
    gpio_primary_remap(AFIO_MAPR_SWJ_CFG_FULL_SWJ_NO_JNTRST
            , AFIO_MAPR_TIM3_REMAP_PARTIAL_REMAP);
    /* Enable the GPIO */
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
            GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO4);
    /* Enable timer 3 */
    rcc_periph_clock_enable(RCC_TIM3);
    /* Set timer 3 mode to no divisor (72MHz), Edge-aligned, up-counting */
    timer_set_mode(TIM3, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    /* Set divider to 3 */
    timer_set_prescaler(TIM3, 3);
    /* A timer update event is generated only after the specified number of
     * repeat count cycles have been completed. */
    timer_set_repetition_counter(TIM3, 0);
    /* Enable Auto-Reload Buffering. */
    timer_enable_preload(TIM3);
    /* Enable the Timer to Run Continuously. */
    timer_continuous_mode(TIM3);
    /* Specify the timer period in the auto-reload register. */
    timer_set_period(TIM3, 0);

    /* The freq is 72 MHz / ((1+3)*(1+0)*(1+1024)) = 17560,975609756 Hz ->
     * period of 56.9 uS*/

    /* Timer Set Output Compare Mode.

     Specifies how the comparator output will respond to a compare match. The
     mode can be:

     Frozen - the output does not respond to a match.
     Active - the output assumes the active state on the first match.
     Inactive - the output assumes the inactive state on the first match.
     Toggle - The output switches between active and inactive states on each
     match.
     Force inactive. The output is forced low regardless of the compare
     state.
     Force active. The output is forced high regardless of the compare
     state.
     PWM1 - The output is active when the counter is less than the compare
     register contents and inactive otherwise.
     PWM2 - The output is inactive when the counter is less than the compare
     register contents and active otherwise. */
    timer_set_oc_mode(TIM3, TIM_OC1, TIM_OCM_PWM1);

    timer_set_oc_value(TIM3, TIM_OC1, 0);

    timer_enable_oc_output(TIM3, TIM_OC1);

    timer_enable_counter(TIM3);
}

// Continuous beep with tone and duration parameters
// tone - period of the note from notes.h
// duration - duration of the beep in ms
int beep(int tone, int duration) {

    beep_remaining = duration;

    timer_set_prescaler(TIM3, PRESCALER_VALUE);
    timer_set_period(TIM3, tone);
    timer_set_oc_value(TIM3, TIM_OC1, tone/2);

    return 0;
}

int silence() {
    timer_set_oc_value(TIM3, TIM_OC1, 0);
    return 0;
}

uint32_t buzzer_systick() {
    if(beep_remaining > 0) {
      beep_remaining --;
    } else {
      silence();
    }
    return beep_remaining;
}

uint32_t get_beep_remaining(){
    return beep_remaining;
}

void play_melody() {
  int melody[] = {
      NOTE_E7, NOTE_E7, NOTE_PAUSE, NOTE_E7,
      NOTE_PAUSE, NOTE_C7, NOTE_E7, NOTE_PAUSE,
      NOTE_G7, NOTE_PAUSE, NOTE_PAUSE,  NOTE_PAUSE,
      NOTE_G6, NOTE_PAUSE, NOTE_PAUSE, NOTE_PAUSE,

      NOTE_C7, NOTE_PAUSE, NOTE_PAUSE, NOTE_G6,
      NOTE_PAUSE, NOTE_PAUSE, NOTE_E6, NOTE_PAUSE,
      NOTE_PAUSE, NOTE_A6, NOTE_PAUSE, NOTE_B6,
      NOTE_PAUSE, NOTE_AS6, NOTE_A6, NOTE_PAUSE,

      NOTE_G6, NOTE_E7, NOTE_G7,
      NOTE_A7, NOTE_PAUSE, NOTE_F7, NOTE_G7,
      NOTE_PAUSE, NOTE_E7, NOTE_PAUSE, NOTE_C7,
      NOTE_D7, NOTE_B6, NOTE_PAUSE, NOTE_PAUSE,

      NOTE_C7, NOTE_PAUSE, NOTE_PAUSE, NOTE_G6,
      NOTE_PAUSE, NOTE_PAUSE, NOTE_E6, NOTE_PAUSE,
      NOTE_PAUSE, NOTE_A6, NOTE_PAUSE, NOTE_B6,
      NOTE_PAUSE, NOTE_AS6, NOTE_A6, NOTE_PAUSE,

      NOTE_G6, NOTE_E7, NOTE_G7,
      NOTE_A7, NOTE_PAUSE, NOTE_F7, NOTE_G7,
      NOTE_PAUSE, NOTE_E7, NOTE_PAUSE, NOTE_C7,
      NOTE_D7, NOTE_B6, NOTE_PAUSE, NOTE_PAUSE
    };

    int tempo[] = {
        12, 12, 12, 12,
        12, 12, 12, 12,
        12, 12, 12, 12,
        12, 12, 12, 12,

        12, 12, 12, 12,
        12, 12, 12, 12,
        12, 12, 12, 12,
        12, 12, 12, 12,

        9, 9, 9,
        12, 12, 12, 12,
        12, 12, 12, 12,
        12, 12, 12, 12,

        12, 12, 12, 12,
        12, 12, 12, 12,
        12, 12, 12, 12,
        12, 12, 12, 12,

        9, 9, 9,
        12, 12, 12, 12,
        12, 12, 12, 12,
        12, 12, 12, 12,
      };

      int size = sizeof(melody) / sizeof(int);
      for (int thisNote = 0; thisNote < size; thisNote++) {
          int noteDuration = 1000 / tempo[thisNote];

          beep(melody[thisNote],noteDuration);
//          while(get_beep_remaining() > 0){
//              __asm__("nop");
//          }
//
//          beep(NOTE_PAUSE,noteDuration/10);
//          while(get_beep_remaining() > 0){
//              __asm__("nop");
//          }
      }

}
