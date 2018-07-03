#include "libjukebox.h"

static int current_song = 0; // The order in the jukebox array of the song currently loaded.
static uint32_t last_millisec = 0;
static uint8_t enable_jukebox_signal = ENABLE_JUKEBOX_AT_START;
static uint8_t stacked_jukebox_signal = ENABLE_JUKEBOX_AT_START;

/*
 * @brief jukebox setup
 *
 * @note execute before anything else
 */
void jukebox_setup(){
    init_jukebox_songs();
    current_song = 0;
}

void jukebox_setcurrent_song(int order){
	current_song = order;
}

void jukebox_play_current(){
  
  play_music(current_song,
             jukebox_song_list[current_song].bpm,
             jukebox_song_list[current_song].length,
             jukebox_song_list[current_song].notes_pitch,
             jukebox_song_list[current_song].notes_duration);
}

void jukebox_play_next(){
    current_song = current_song + 1;
    if (current_song >= get_total_songs_number()){
      current_song = 0;
    }
    jukebox_play_current();
}

int is_jukebox_playing(){
    return is_music_playing();
}

/*
 * @brief play current song in loop
 */ 
void jukebox_play_in_loop(uint32_t current_millisecs)
{

  if (!enable_jukebox_signal)
    return;
    
  /* we are not playing */
  if (!is_jukebox_playing()) {
      jukebox_play_current();
    }
  /* we are playing another song */
  else if (get_song_id() != current_song)
    {
      jukebox_play_current();
    }

  if (last_millisec < current_millisecs)
    {
      play_music_loop();
      last_millisec = current_millisecs;
    }
}

/* 
 * @brief enable jukebox
 *
 */
void enable_jukebox()
{
  enable_jukebox_signal = 1;
}

/*
 * @brief disable jukebox
 */
void disable_jukebox()
{
  // stopping the current note first
  stop_music_play();
  
 enable_jukebox_signal = 0;
}

/*
 * @brief check if the jukebox is currently enabled
 */
uint8_t is_jukebox_enabled()
{
  return enable_jukebox_signal;
}

/*
 * @brief stack the enable jukebox signal
 */
void push_enable_jukebox()
{
  stacked_jukebox_signal = enable_jukebox_signal;
}

/*
 * @brief return the enable jukebox signal from stack
 */
void pull_enable_jukebox()
{
  enable_jukebox_signal = stacked_jukebox_signal;
}
