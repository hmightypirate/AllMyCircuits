#include "jukebox_songs.h"


// Returns the number of songs in the jukebox
uint8_t init_jukebox_songs() {

    jukebox_song_list[SONG_MACGYVER_ORDER].length = SONG_LEN_MACGYVER;
    jukebox_song_list[SONG_MACGYVER_ORDER].bpm = SONG_VEL_MACGYVER;
    jukebox_song_list[SONG_MACGYVER_ORDER].notes_pitch = notes_pitch_MACGYVER;
    jukebox_song_list[SONG_MACGYVER_ORDER].notes_duration = notes_duration_MACGYVER;

    jukebox_song_list[SONG_UNDER_THE_SEA_ORDER].length = SONG_LEN_UNDER_THE_SEA;
    jukebox_song_list[SONG_UNDER_THE_SEA_ORDER].bpm = SONG_VEL_UNDER_THE_SEA;
    jukebox_song_list[SONG_UNDER_THE_SEA_ORDER].notes_pitch = notes_pitch_UNDER_THE_SEA;
    jukebox_song_list[SONG_UNDER_THE_SEA_ORDER].notes_duration = notes_duration_UNDER_THE_SEA;

    jukebox_song_list[SONG_BATMAN_ORDER].length = SONG_LEN_BATMAN;
    jukebox_song_list[SONG_BATMAN_ORDER].bpm = SONG_VEL_BATMAN;
    jukebox_song_list[SONG_BATMAN_ORDER].notes_pitch = notes_pitch_BATMAN;
    jukebox_song_list[SONG_BATMAN_ORDER].notes_duration = notes_duration_BATMAN;

    jukebox_song_list[SONG_LIGHTMYFIRE_ORDER].length = SONG_LEN_LIGHTMYFIRE;
    jukebox_song_list[SONG_LIGHTMYFIRE_ORDER].bpm = SONG_VEL_LIGHTMYFIRE;
    jukebox_song_list[SONG_LIGHTMYFIRE_ORDER].notes_pitch = notes_pitch_LIGHTMYFIRE;
    jukebox_song_list[SONG_LIGHTMYFIRE_ORDER].notes_duration = notes_duration_LIGHTMYFIRE;

    jukebox_song_list[SONG_TEARS_IN_HEAVEN_ORDER].length = SONG_LEN_TEARS_IN_HEAVEN;
    jukebox_song_list[SONG_TEARS_IN_HEAVEN_ORDER].bpm = SONG_VEL_TEARS_IN_HEAVEN;
    jukebox_song_list[SONG_TEARS_IN_HEAVEN_ORDER].notes_pitch = notes_pitch_TEARS_IN_HEAVEN;
    jukebox_song_list[SONG_TEARS_IN_HEAVEN_ORDER].notes_duration = notes_duration_TEARS_IN_HEAVEN;

    jukebox_song_list[SONG_AADAMS_ORDER].length = SONG_LEN_AADAMS;
    jukebox_song_list[SONG_AADAMS_ORDER].bpm = SONG_VEL_AADAMS;
    jukebox_song_list[SONG_AADAMS_ORDER].notes_pitch = notes_pitch_AADAMS;
    jukebox_song_list[SONG_AADAMS_ORDER].notes_duration = notes_duration_AADAMS;

    jukebox_song_list[SONG_DRNO_ORDER].length = SONG_LEN_DRNO;
    jukebox_song_list[SONG_DRNO_ORDER].bpm = SONG_VEL_DRNO;
    jukebox_song_list[SONG_DRNO_ORDER].notes_pitch = notes_pitch_DRNO;
    jukebox_song_list[SONG_DRNO_ORDER].notes_duration = notes_duration_DRNO;

    jukebox_song_list[SONG_BA_BA_ORDER].length = SONG_LEN_BA_BA;
    jukebox_song_list[SONG_BA_BA_ORDER].bpm = SONG_VEL_BA_BA;
    jukebox_song_list[SONG_BA_BA_ORDER].notes_pitch = notes_pitch_BA_BA;
    jukebox_song_list[SONG_BA_BA_ORDER].notes_duration = notes_duration_BA_BA;

    jukebox_song_list[SONG_GEORGIA_ON_MY_MIND_ORDER].length = SONG_LEN_GEORGIA_ON_MY_MIND;
    jukebox_song_list[SONG_GEORGIA_ON_MY_MIND_ORDER].bpm = SONG_VEL_GEORGIA_ON_MY_MIND;
    jukebox_song_list[SONG_GEORGIA_ON_MY_MIND_ORDER].notes_pitch = notes_pitch_GEORGIA_ON_MY_MIND;
    jukebox_song_list[SONG_GEORGIA_ON_MY_MIND_ORDER].notes_duration = notes_duration_GEORGIA_ON_MY_MIND;

    jukebox_song_list[SONG_MISSIONIMPOSSI_ORDER].length = SONG_LEN_MISSIONIMPOSSI;
    jukebox_song_list[SONG_MISSIONIMPOSSI_ORDER].bpm = SONG_VEL_MISSIONIMPOSSI;
    jukebox_song_list[SONG_MISSIONIMPOSSI_ORDER].notes_pitch = notes_pitch_MISSIONIMPOSSI;
    jukebox_song_list[SONG_MISSIONIMPOSSI_ORDER].notes_duration = notes_duration_MISSIONIMPOSSI;

    jukebox_song_list[SONG_BEETHOVEN_ORDER].length = SONG_LEN_BEETHOVEN;
    jukebox_song_list[SONG_BEETHOVEN_ORDER].bpm = SONG_VEL_BEETHOVEN;
    jukebox_song_list[SONG_BEETHOVEN_ORDER].notes_pitch = notes_pitch_BEETHOVEN;
    jukebox_song_list[SONG_BEETHOVEN_ORDER].notes_duration = notes_duration_BEETHOVEN;

    jukebox_song_list[SONG_FLNTSTN_ORDER].length = SONG_LEN_FLNTSTN;
    jukebox_song_list[SONG_FLNTSTN_ORDER].bpm = SONG_VEL_FLNTSTN;
    jukebox_song_list[SONG_FLNTSTN_ORDER].notes_pitch = notes_pitch_FLNTSTN;
    jukebox_song_list[SONG_FLNTSTN_ORDER].notes_duration = notes_duration_FLNTSTN;

    jukebox_song_list[SONG_LITHIUM_ORDER].length = SONG_LEN_LITHIUM;
    jukebox_song_list[SONG_LITHIUM_ORDER].bpm = SONG_VEL_LITHIUM;
    jukebox_song_list[SONG_LITHIUM_ORDER].notes_pitch = notes_pitch_LITHIUM;
    jukebox_song_list[SONG_LITHIUM_ORDER].notes_duration = notes_duration_LITHIUM;

    jukebox_song_list[SONG_DEATH_MARCH_ORDER].length = SONG_LEN_DEATH_MARCH;
    jukebox_song_list[SONG_DEATH_MARCH_ORDER].bpm = SONG_VEL_DEATH_MARCH;
    jukebox_song_list[SONG_DEATH_MARCH_ORDER].notes_pitch = notes_pitch_DEATH_MARCH;
    jukebox_song_list[SONG_DEATH_MARCH_ORDER].notes_duration = notes_duration_DEATH_MARCH;

    jukebox_song_list[SONG_DIRE_ORDER].length = SONG_LEN_DIRE;
    jukebox_song_list[SONG_DIRE_ORDER].bpm = SONG_VEL_DIRE;
    jukebox_song_list[SONG_DIRE_ORDER].notes_pitch = notes_pitch_DIRE;
    jukebox_song_list[SONG_DIRE_ORDER].notes_duration = notes_duration_DIRE;

    jukebox_song_list[SONG_DASBOOT_ORDER].length = SONG_LEN_DASBOOT;
    jukebox_song_list[SONG_DASBOOT_ORDER].bpm = SONG_VEL_DASBOOT;
    jukebox_song_list[SONG_DASBOOT_ORDER].notes_pitch = notes_pitch_DASBOOT;
    jukebox_song_list[SONG_DASBOOT_ORDER].notes_duration = notes_duration_DASBOOT;

    jukebox_song_list[SONG_SUPERMAN_ORDER].length = SONG_LEN_SUPERMAN;
    jukebox_song_list[SONG_SUPERMAN_ORDER].bpm = SONG_VEL_SUPERMAN;
    jukebox_song_list[SONG_SUPERMAN_ORDER].notes_pitch = notes_pitch_SUPERMAN;
    jukebox_song_list[SONG_SUPERMAN_ORDER].notes_duration = notes_duration_SUPERMAN;

    jukebox_song_list[SONG_GREENDAY_ORDER].length = SONG_LEN_GREENDAY;
    jukebox_song_list[SONG_GREENDAY_ORDER].bpm = SONG_VEL_GREENDAY;
    jukebox_song_list[SONG_GREENDAY_ORDER].notes_pitch = notes_pitch_GREENDAY;
    jukebox_song_list[SONG_GREENDAY_ORDER].notes_duration = notes_duration_GREENDAY;

    jukebox_song_list[SONG_DON_T_WANNA_MISS_A_THING_ORDER].length = SONG_LEN_DON_T_WANNA_MISS_A_THING;
    jukebox_song_list[SONG_DON_T_WANNA_MISS_A_THING_ORDER].bpm = SONG_VEL_DON_T_WANNA_MISS_A_THING;
    jukebox_song_list[SONG_DON_T_WANNA_MISS_A_THING_ORDER].notes_pitch = notes_pitch_DON_T_WANNA_MISS_A_THING;
    jukebox_song_list[SONG_DON_T_WANNA_MISS_A_THING_ORDER].notes_duration = notes_duration_DON_T_WANNA_MISS_A_THING;

    jukebox_song_list[SONG_GIRLFROMIPANE_ORDER].length = SONG_LEN_GIRLFROMIPANE;
    jukebox_song_list[SONG_GIRLFROMIPANE_ORDER].bpm = SONG_VEL_GIRLFROMIPANE;
    jukebox_song_list[SONG_GIRLFROMIPANE_ORDER].notes_pitch = notes_pitch_GIRLFROMIPANE;
    jukebox_song_list[SONG_GIRLFROMIPANE_ORDER].notes_duration = notes_duration_GIRLFROMIPANE;

    jukebox_song_list[SONG_KNIGHTRIDER_ORDER].length = SONG_LEN_KNIGHTRIDER;
    jukebox_song_list[SONG_KNIGHTRIDER_ORDER].bpm = SONG_VEL_KNIGHTRIDER;
    jukebox_song_list[SONG_KNIGHTRIDER_ORDER].notes_pitch = notes_pitch_KNIGHTRIDER;
    jukebox_song_list[SONG_KNIGHTRIDER_ORDER].notes_duration = notes_duration_KNIGHTRIDER;

    jukebox_song_list[SONG_90210_ORDER].length = SONG_LEN_90210;
    jukebox_song_list[SONG_90210_ORDER].bpm = SONG_VEL_90210;
    jukebox_song_list[SONG_90210_ORDER].notes_pitch = notes_pitch_90210;
    jukebox_song_list[SONG_90210_ORDER].notes_duration = notes_duration_90210;

    jukebox_song_list[SONG_HALLOWEEN_ORDER].length = SONG_LEN_HALLOWEEN;
    jukebox_song_list[SONG_HALLOWEEN_ORDER].bpm = SONG_VEL_HALLOWEEN;
    jukebox_song_list[SONG_HALLOWEEN_ORDER].notes_pitch = notes_pitch_HALLOWEEN;
    jukebox_song_list[SONG_HALLOWEEN_ORDER].notes_duration = notes_duration_HALLOWEEN;

    jukebox_song_list[SONG_CASTLE_ON_A_CLOUD_ORDER].length = SONG_LEN_CASTLE_ON_A_CLOUD;
    jukebox_song_list[SONG_CASTLE_ON_A_CLOUD_ORDER].bpm = SONG_VEL_CASTLE_ON_A_CLOUD;
    jukebox_song_list[SONG_CASTLE_ON_A_CLOUD_ORDER].notes_pitch = notes_pitch_CASTLE_ON_A_CLOUD;
    jukebox_song_list[SONG_CASTLE_ON_A_CLOUD_ORDER].notes_duration = notes_duration_CASTLE_ON_A_CLOUD;

    jukebox_song_list[SONG_IMPERIAL_ORDER].length = SONG_LEN_IMPERIAL;
    jukebox_song_list[SONG_IMPERIAL_ORDER].bpm = SONG_VEL_IMPERIAL;
    jukebox_song_list[SONG_IMPERIAL_ORDER].notes_pitch = notes_pitch_IMPERIAL;
    jukebox_song_list[SONG_IMPERIAL_ORDER].notes_duration = notes_duration_IMPERIAL;

    jukebox_song_list[SONG_RICH_ORDER].length = SONG_LEN_RICH;
    jukebox_song_list[SONG_RICH_ORDER].bpm = SONG_VEL_RICH;
    jukebox_song_list[SONG_RICH_ORDER].notes_pitch = notes_pitch_RICH;
    jukebox_song_list[SONG_RICH_ORDER].notes_duration = notes_duration_RICH;

    jukebox_song_list[SONG_AXELF_ORDER].length = SONG_LEN_AXELF;
    jukebox_song_list[SONG_AXELF_ORDER].bpm = SONG_VEL_AXELF;
    jukebox_song_list[SONG_AXELF_ORDER].notes_pitch = notes_pitch_AXELF;
    jukebox_song_list[SONG_AXELF_ORDER].notes_duration = notes_duration_AXELF;

    jukebox_song_list[SONG_THINK_PINK_ORDER].length = SONG_LEN_THINK_PINK;
    jukebox_song_list[SONG_THINK_PINK_ORDER].bpm = SONG_VEL_THINK_PINK;
    jukebox_song_list[SONG_THINK_PINK_ORDER].notes_pitch = notes_pitch_THINK_PINK;
    jukebox_song_list[SONG_THINK_PINK_ORDER].notes_duration = notes_duration_THINK_PINK;

    jukebox_song_list[SONG_FIGARO_ORDER].length = SONG_LEN_FIGARO;
    jukebox_song_list[SONG_FIGARO_ORDER].bpm = SONG_VEL_FIGARO;
    jukebox_song_list[SONG_FIGARO_ORDER].notes_pitch = notes_pitch_FIGARO;
    jukebox_song_list[SONG_FIGARO_ORDER].notes_duration = notes_duration_FIGARO;

    jukebox_song_list[SONG_ARGENTINA_ORDER].length = SONG_LEN_ARGENTINA;
    jukebox_song_list[SONG_ARGENTINA_ORDER].bpm = SONG_VEL_ARGENTINA;
    jukebox_song_list[SONG_ARGENTINA_ORDER].notes_pitch = notes_pitch_ARGENTINA;
    jukebox_song_list[SONG_ARGENTINA_ORDER].notes_duration = notes_duration_ARGENTINA;

    jukebox_song_list[SONG_MORE_THAN_WORDS_ORDER].length = SONG_LEN_MORE_THAN_WORDS;
    jukebox_song_list[SONG_MORE_THAN_WORDS_ORDER].bpm = SONG_VEL_MORE_THAN_WORDS;
    jukebox_song_list[SONG_MORE_THAN_WORDS_ORDER].notes_pitch = notes_pitch_MORE_THAN_WORDS;
    jukebox_song_list[SONG_MORE_THAN_WORDS_ORDER].notes_duration = notes_duration_MORE_THAN_WORDS;

    jukebox_song_list[SONG_MICROMELODY_ORDER].length = SONG_LEN_MICROMELODY;
    jukebox_song_list[SONG_MICROMELODY_ORDER].bpm = SONG_VEL_MICROMELODY;
    jukebox_song_list[SONG_MICROMELODY_ORDER].notes_pitch = notes_pitch_MICROMELODY;
    jukebox_song_list[SONG_MICROMELODY_ORDER].notes_duration = notes_duration_MICROMELODY;

    jukebox_song_list[SONG_SW_END_ORDER].length = SONG_LEN_SW_END;
    jukebox_song_list[SONG_SW_END_ORDER].bpm = SONG_VEL_SW_END;
    jukebox_song_list[SONG_SW_END_ORDER].notes_pitch = notes_pitch_SW_END;
    jukebox_song_list[SONG_SW_END_ORDER].notes_duration = notes_duration_SW_END;

    jukebox_song_list[SONG_STAIRWAY_ORDER].length = SONG_LEN_STAIRWAY;
    jukebox_song_list[SONG_STAIRWAY_ORDER].bpm = SONG_VEL_STAIRWAY;
    jukebox_song_list[SONG_STAIRWAY_ORDER].notes_pitch = notes_pitch_STAIRWAY;
    jukebox_song_list[SONG_STAIRWAY_ORDER].notes_duration = notes_duration_STAIRWAY;

    jukebox_song_list[SONG_BEATLES_LET_IT_BE_ORDER].length = SONG_LEN_BEATLES_LET_IT_BE;
    jukebox_song_list[SONG_BEATLES_LET_IT_BE_ORDER].bpm = SONG_VEL_BEATLES_LET_IT_BE;
    jukebox_song_list[SONG_BEATLES_LET_IT_BE_ORDER].notes_pitch = notes_pitch_BEATLES_LET_IT_BE;
    jukebox_song_list[SONG_BEATLES_LET_IT_BE_ORDER].notes_duration = notes_duration_BEATLES_LET_IT_BE;

    jukebox_song_list[SONG_GODFATHER_ORDER].length = SONG_LEN_GODFATHER;
    jukebox_song_list[SONG_GODFATHER_ORDER].bpm = SONG_VEL_GODFATHER;
    jukebox_song_list[SONG_GODFATHER_ORDER].notes_pitch = notes_pitch_GODFATHER;
    jukebox_song_list[SONG_GODFATHER_ORDER].notes_duration = notes_duration_GODFATHER;

    jukebox_song_list[SONG_CANTINA_ORDER].length = SONG_LEN_CANTINA;
    jukebox_song_list[SONG_CANTINA_ORDER].bpm = SONG_VEL_CANTINA;
    jukebox_song_list[SONG_CANTINA_ORDER].notes_pitch = notes_pitch_CANTINA;
    jukebox_song_list[SONG_CANTINA_ORDER].notes_duration = notes_duration_CANTINA;

    jukebox_song_list[SONG_SWEND_ORDER].length = SONG_LEN_SWEND;
    jukebox_song_list[SONG_SWEND_ORDER].bpm = SONG_VEL_SWEND;
    jukebox_song_list[SONG_SWEND_ORDER].notes_pitch = notes_pitch_SWEND;
    jukebox_song_list[SONG_SWEND_ORDER].notes_duration = notes_duration_SWEND;

    jukebox_song_list[SONG_EUROPE_ORDER].length = SONG_LEN_EUROPE;
    jukebox_song_list[SONG_EUROPE_ORDER].bpm = SONG_VEL_EUROPE;
    jukebox_song_list[SONG_EUROPE_ORDER].notes_pitch = notes_pitch_EUROPE;
    jukebox_song_list[SONG_EUROPE_ORDER].notes_duration = notes_duration_EUROPE;

    jukebox_song_list[SONG_BARBIEGIRL_ORDER].length = SONG_LEN_BARBIEGIRL;
    jukebox_song_list[SONG_BARBIEGIRL_ORDER].bpm = SONG_VEL_BARBIEGIRL;
    jukebox_song_list[SONG_BARBIEGIRL_ORDER].notes_pitch = notes_pitch_BARBIEGIRL;
    jukebox_song_list[SONG_BARBIEGIRL_ORDER].notes_duration = notes_duration_BARBIEGIRL;

    jukebox_song_list[SONG_FUNKYTOWN_ORDER].length = SONG_LEN_FUNKYTOWN;
    jukebox_song_list[SONG_FUNKYTOWN_ORDER].bpm = SONG_VEL_FUNKYTOWN;
    jukebox_song_list[SONG_FUNKYTOWN_ORDER].notes_pitch = notes_pitch_FUNKYTOWN;
    jukebox_song_list[SONG_FUNKYTOWN_ORDER].notes_duration = notes_duration_FUNKYTOWN;

    jukebox_song_list[SONG_LET_IT_BE_ORDER].length = SONG_LEN_LET_IT_BE;
    jukebox_song_list[SONG_LET_IT_BE_ORDER].bpm = SONG_VEL_LET_IT_BE;
    jukebox_song_list[SONG_LET_IT_BE_ORDER].notes_pitch = notes_pitch_LET_IT_BE;
    jukebox_song_list[SONG_LET_IT_BE_ORDER].notes_duration = notes_duration_LET_IT_BE;

    jukebox_song_list[SONG_INDIANAJONES_ORDER].length = SONG_LEN_INDIANAJONES;
    jukebox_song_list[SONG_INDIANAJONES_ORDER].bpm = SONG_VEL_INDIANAJONES;
    jukebox_song_list[SONG_INDIANAJONES_ORDER].notes_pitch = notes_pitch_INDIANAJONES;
    jukebox_song_list[SONG_INDIANAJONES_ORDER].notes_duration = notes_duration_INDIANAJONES;

    jukebox_song_list[SONG_BOLERO_ORDER].length = SONG_LEN_BOLERO;
    jukebox_song_list[SONG_BOLERO_ORDER].bpm = SONG_VEL_BOLERO;
    jukebox_song_list[SONG_BOLERO_ORDER].notes_pitch = notes_pitch_BOLERO;
    jukebox_song_list[SONG_BOLERO_ORDER].notes_duration = notes_duration_BOLERO;

    jukebox_song_list[SONG_XFILES_ORDER].length = SONG_LEN_XFILES;
    jukebox_song_list[SONG_XFILES_ORDER].bpm = SONG_VEL_XFILES;
    jukebox_song_list[SONG_XFILES_ORDER].notes_pitch = notes_pitch_XFILES;
    jukebox_song_list[SONG_XFILES_ORDER].notes_duration = notes_duration_XFILES;


    return JUKEBOX_SONGS_TOTAL_SONGS;
}

uint8_t get_total_songs_number() {
	return JUKEBOX_SONGS_TOTAL_SONGS;
}
