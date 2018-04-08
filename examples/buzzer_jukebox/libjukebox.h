#include "jukebox_songs.h"

int current_song; // The order in the jukebox array of the song currently loaded.

int jukebox_setup(){
    music_setup();
    init_jukebox_songs();
    current_song = 0;
    return 1;
}

void jukebox_setcurrent_song(int order){
	current_song = order;
}

int jukebox_play_current(){
    play_music(jukebox_song_list[current_song].bpm
        , jukebox_song_list[current_song].length
        , jukebox_song_list[current_song].notes_pitch
        , jukebox_song_list[current_song].notes_duration);
    return 1;
}

int jukebox_play_next(){
    current_song = current_song + 1;
    if (current_song >= get_total_songs_number()){ current_song = 0;}
    return jukebox_play_current();
}

int is_jukebox_playing(){
    return is_music_playing();
}
