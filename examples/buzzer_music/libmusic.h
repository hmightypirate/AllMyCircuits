
#define C0 0
#define CX0 1
#define D0 2
#define DX0 3
#define E0 4
#define F0 5
#define FX0 6
#define G0 7
#define GX0 8
#define A0 9
#define AX0 10
#define B0 11
#define C1 12
#define CX1 13
#define D1 14
#define DX1 15
#define E1 16
#define F1 17
#define FX1 18
#define G1 19
#define GX1 20
#define A1 21
#define AX1 22
#define B1 23
#define C2 24
#define CX2 25
#define D2 26
#define DX2 27
#define E2 28
#define F2 29
#define FX2 30
#define G2 31
#define GX2 32
#define A2 33
#define AX2 34
#define B2 35
#define C3 36
#define CX3 37
#define D3 38
#define DX3 39
#define E3 40
#define F3 41
#define FX3 42
#define G3 43
#define GX3 44
#define A3 45
#define AX3 46
#define B3 47
#define C4 48
#define CX4 49
#define D4 50
#define DX4 51
#define E4 52
#define F4 53
#define FX4 54
#define G4 55
#define GX4 56
#define A4 57
#define AX4 58
#define B4 59
#define C5 60
#define CX5 61
#define D5 62
#define DX5 63
#define E5 64
#define F5 65
#define FX5 66
#define G5 67
#define GX5 68
#define A5 69
#define AX5 70
#define B5 71
#define C6 72
#define CX6 73
#define D6 74
#define DX6 75
#define E6 76
#define F6 77
#define FX6 78
#define G6 79
#define GX6 80
#define A6 81
#define AX6 82
#define B6 83
#define C7 84
#define CX7 85
#define D7 86
#define DX7 87
#define E7 88
#define F7 89
#define FX7 90
#define G7 91
#define GX7 92
#define A7 93
#define AX7 94
#define B7 95
#define C8 96
#define CX8 97
#define D8 98
#define DX8 99
#define E8 100
#define F8 101
#define FX8 102
#define G8 103
#define GX8 104
#define A8 105
#define AX8 106
#define B8 107
#define R 108 // no note, or rest


void pwm_setup() {

    int use_alternate_gpio = 1;
    if (use_alternate_gpio == 1){
        rcc_periph_clock_enable(RCC_AFIO);
        /* Enable the alternate GPIO for output*/
        gpio_primary_remap(AFIO_MAPR_SWJ_CFG_FULL_SWJ_NO_JNTRST
                , AFIO_MAPR_TIM3_REMAP_PARTIAL_REMAP);
        /* Enable the GPIO */
        gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
                GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO4);
    } else {
        /* Enable output GPIOs */
        rcc_periph_clock_enable(RCC_GPIOA);
        gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
                GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO6);
    }

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

void music_setup(){
    pwm_setup();
}

int stop_note(){
    timer_set_oc_value(TIM3, TIM_OC1, 0);
    return 1;
}


int play_note(int note_number){

    if (note_number >= 108){
        stop_note();
        return -1;
    }
    if (note_number < 0){
        stop_note();
        return -1;
    }

    uint32_t prescaler_values[108] = {70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70};
    //uint32_t repetition_values[108] = {16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16};
    uint32_t register_values[108] = {62023, 58549, 55262, 52137, 49226, 46453, 43861, 41390, 39062, 36875, 34799, 32849, 31011, 29266, 27623, 26075, 24613, 23231, 21925, 20695, 19534, 18437, 17402, 16424, 15503, 14632, 13811, 13037, 12304, 11614, 10962, 10347, 9766, 9218, 8701, 8212, 7751, 7316, 6906, 6518, 6152, 5807, 5481, 5173, 4898, 4608, 4350, 4106, 3875, 3658, 3452, 3258, 3075, 2903, 2740, 2586, 2441, 2304, 2174, 2052, 1937, 1828, 1726, 1629, 1537, 1451, 1369, 1292, 1220, 1151, 1087, 1026, 968, 914, 862, 814, 768, 725, 684, 646, 609, 575, 543, 512, 484, 456, 431, 406, 384, 362, 342, 322, 304, 287, 271, 256, 241, 228, 215, 203, 191, 180, 170, 161, 152, 143, 135, 127};

    timer_set_prescaler(TIM3, prescaler_values[note_number]);
    //timer_set_repetition_counter(TIM3, repetition_values[note_number]);
    timer_set_period(TIM3, register_values[note_number]);

    printf(" play_note(%d): prescaler %d, period %d\n"
            , note_number
            , (int) prescaler_values[note_number]
            , (int) register_values[note_number]);

    timer_set_oc_value(TIM3, TIM_OC1, register_values[note_number]/2);

    return 1;
}

int notes_index = 0;
int enable_music = 0;
int system_ticks_per_beat_32ave = 0;
int * current_notes_pitch = NULL;
int * current_notes_duration = NULL;
int current_total_notes = 0;
int wait_ticks = 0;

void play_music(int beats_per_minute, int notes_number, int * notes_pitch
        , int * notes_duration){

    play_note(A4);
    notes_index = 0;
    system_ticks_per_beat_32ave = (10000 * 60 / beats_per_minute) / 32;
    current_notes_pitch = notes_pitch;
    current_notes_duration = notes_duration;
    current_total_notes = notes_number;
    enable_music = 1;
    wait_ticks = 0;
    stop_note();
}

int tics = 0;
int system_ticks_counter = 0;
void play_music_loop(){

    if (enable_music == 0) return;
    system_ticks_counter++;

    if (system_ticks_counter >= wait_ticks){
        system_ticks_counter = 0;
        play_note(current_notes_pitch[notes_index]);
        wait_ticks = system_ticks_per_beat_32ave * current_notes_duration[notes_index];
        notes_index++;
        if (notes_index >= current_total_notes){
            notes_index = 0;
            enable_music = 0;
        }
    }
}

int is_music_playing(){
    return enable_music;
}
