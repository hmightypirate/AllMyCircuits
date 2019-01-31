#ifndef __LIBJUKEBOX_H
#define __LIBJUKEBOX_H

#include "jukebox_songs.h"

/* Set to true to enable buzzer at start */
#define ENABLE_JUKEBOX_AT_START 1

void jukebox_setup(void);
void jukebox_setcurrent_song(int order);
void jukebox_play_current(void);
void jukebox_play_next(void);
int is_jukebox_playing(void);
void jukebox_play_in_loop(uint32_t current_millisecs);
void enable_jukebox();
void disable_jukebox();
uint8_t is_jukebox_enabled(void);
void push_enable_jukebox();
void pull_enable_jukebox();
void toggle_sound(void);
  

#endif /* __LIBJUKEBOX_H */
