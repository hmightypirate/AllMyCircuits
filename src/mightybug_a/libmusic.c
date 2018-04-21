#include "libmusic.h"

static int song_id = 0;
static int system_ticks_counter = 0;
static int notes_index = 0;
static uint8_t enable_music = 0;
static int system_ticks_per_beat_32ave = 0;
static uint8_t * current_notes_pitch = NULL;
static uint8_t * current_notes_duration = NULL;
static uint16_t current_total_notes = 0;
/* wait ticks stores the number of ticks till the next note should be played */
static int wait_ticks = 0;


void stop_note(){
    timer_set_oc_value(TIM3, TIM_OC1, 0);
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

    timer_set_oc_value(TIM3, TIM_OC1, register_values[note_number]/2);

    return 1;
}


void play_music(int mysong_id, int beats_per_minute, uint16_t notes_number, uint8_t * notes_pitch
        , uint8_t * notes_duration){

  song_id = mysong_id;
  play_note(A4);
    notes_index = 0;
    system_ticks_per_beat_32ave = (LIBMUSIC_TICKS_PER_SECOND * 60 / beats_per_minute)/32;  //FIXME: some explanation is needed here
    current_notes_pitch = notes_pitch;
    current_notes_duration = notes_duration;
    current_total_notes = notes_number;
    enable_music = 1;
    wait_ticks = 0;
    stop_note();
}

/* 
 * @brief plays next note if the number of ticks exceed the number of waiting ticks
 */
void play_music_loop(){

    /* returns if music is not enable */
    if (enable_music == 0) return;
    system_ticks_counter++;

    if (system_ticks_counter >= wait_ticks){
      /* should play next note */
        system_ticks_counter = 0;
        play_note(current_notes_pitch[notes_index]);
        wait_ticks = system_ticks_per_beat_32ave * current_notes_duration[notes_index];
        notes_index++;
        /* stop the music if we have reached the maximum number of notes */
        if (notes_index > current_total_notes){
            notes_index = 0;
            enable_music = 0;
            stop_note();
        }
    }
}

/*
 * @brief returns True if there is a song playing
 */ 
uint8_t is_music_playing(){
  return enable_music;
}

/* 
 * @brief get the number of notes of the song being played
 */ 
uint16_t get_current_total_notes(void)
{
   return current_total_notes;
}

/*
 * @brief return the id of the song being played
 */
int get_song_id(void)
{
  return song_id;  
}

/*
 * @brief stops the music
 */
void stop_music_play()
{
  enable_music = 0;
}
