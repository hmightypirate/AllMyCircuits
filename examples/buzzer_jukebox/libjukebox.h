#ifndef __LIBJUKEBOX_H
#define __LIBJUKEBOX_H

#include "jukebox_songs.h"

int current_song; // The order in the jukebox array of the song currently loaded.

void jukebox_setup(void);
void jukebox_setcurrent_song(int order);
void jukebox_play_current(void);
void jukebox_play_next(void);
int is_jukebox_playing(void);



#endif /* __LIBJUKEBOX_H */
