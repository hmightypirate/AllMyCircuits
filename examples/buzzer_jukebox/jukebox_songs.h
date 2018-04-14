#ifndef __JUKEBOX_SONGS_H
#define __JUKEBOX_SONGS_H

#include <stdio.h>
#include "libmusic.h"

struct Song {
	int length;
        int bpm;  /* beats per minute */
	uint8_t *notes_pitch;
	uint8_t *notes_duration;
};

#define JUKEBOX_SONGS_TOTAL_SONGS 45
struct Song jukebox_song_list[JUKEBOX_SONGS_TOTAL_SONGS];

#define SONG_MACGYVER_ORDER 0
#define SONG_LEN_MACGYVER 29
#define SONG_VEL_MACGYVER 160
    uint8_t notes_pitch_MACGYVER[SONG_LEN_MACGYVER] = {
        C6, C6, C6, C6, C6, C6, B5, FX5, A5, R, G5, C6, C6, R, B5, A5, B5, A5, G5, R, E6, A5, R, B5, R, C6, B5, A5, C6};
    uint8_t notes_duration_MACGYVER[SONG_LEN_MACGYVER] = {
        16, 16, 16, 16, 16, 16, 64, 16, 16, 16, 64, 16, 16, 16, 32, 16, 16, 16, 16, 16, 32, 32, 8, 32, 8, 32, 16, 16, 32};

#define SONG_UNDER_THE_SEA_ORDER 1
#define SONG_LEN_UNDER_THE_SEA 45
#define SONG_VEL_UNDER_THE_SEA 200
    uint8_t notes_pitch_UNDER_THE_SEA[SONG_LEN_UNDER_THE_SEA] = {
        D5, F5, AX5, D6, D6, AX5, C6, DX6, D6, AX5, AX4, D5, F5, AX5, AX5, C5, A5, C6, AX5, R, D5, F5, AX5, D6, D6, AX5, C6, DX6, D6, AX5, AX4, D5, F5, AX5, AX5, C5, A5, C6, AX5, D5, AX5, D5, AX5, D5, AX5};
    uint8_t notes_duration_UNDER_THE_SEA[SONG_LEN_UNDER_THE_SEA] = {
        16, 16, 16, 32, 32, 16, 32, 32, 32, 32, 16, 16, 16, 32, 32, 16, 32, 32, 32, 32, 16, 16, 16, 32, 32, 16, 32, 32, 32, 32, 16, 16, 16, 32, 32, 16, 32, 32, 8, 8, 8, 8, 8, 8, 8};

#define SONG_BATMAN_ORDER 2
#define SONG_LEN_BATMAN 19
#define SONG_VEL_BATMAN 180
    uint8_t notes_pitch_BATMAN[SONG_LEN_BATMAN] = {
        B5, B5, AX5, AX5, A5, A5, AX5, AX5, B5, B5, AX5, AX5, A5, A5, AX5, AX5, B5, R, B5};
    uint8_t notes_duration_BATMAN[SONG_LEN_BATMAN] = {
        16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 32, 16, 32};

#define SONG_LIGHTMYFIRE_ORDER 3
#define SONG_LEN_LIGHTMYFIRE 33
#define SONG_VEL_LIGHTMYFIRE 140
   uint8_t notes_pitch_LIGHTMYFIRE[SONG_LEN_LIGHTMYFIRE] = {
        B5, G5, A5, B5, D6, C6, B5, A5, G5, A5, F5, A5, C6, F6, D6, C6, AX5, G5, GX5, G5, GX5, G5, A5, B5, CX6, B5, A5, G5, F5, E5, F5, A5, A5};
    uint8_t notes_duration_LIGHTMYFIRE[SONG_LEN_LIGHTMYFIRE] = {
        16, 8, 8, 16, 16, 16, 16, 16, 16, 16, 8, 8, 16, 16, 8, 8, 8, 8, 16, 16, 16, 8, 8, 16, 16, 8, 8, 8, 8, 16, 16, 128, 32};

#define SONG_TEARS_IN_HEAVEN_ORDER 4
#define SONG_LEN_TEARS_IN_HEAVEN 17
#define SONG_VEL_TEARS_IN_HEAVEN 112
    uint8_t notes_pitch_TEARS_IN_HEAVEN[SONG_LEN_TEARS_IN_HEAVEN] = {
        R, B5, G5, D6, D6, B5, A5, G5, R, R, C6, C6, B5, A5, G5, B5, A5};
    uint8_t notes_duration_TEARS_IN_HEAVEN[SONG_LEN_TEARS_IN_HEAVEN] = {
        32, 16, 16, 32, 16, 16, 8, 32, 64, 32, 16, 16, 16, 16, 16, 32, 64};

#define SONG_AADAMS_ORDER 5
#define SONG_LEN_AADAMS 51
#define SONG_VEL_AADAMS 160
    uint8_t notes_pitch_AADAMS[SONG_LEN_AADAMS] = {
        C5, F5, A5, F5, C5, B4, G5, F5, E5, G5, E5, E4, A4, F5, C5, F5, A5, F5, C5, B4, G5, F5, E5, C5, D5, E5, F5, C5, D5, E5, F5, R, D5, E5, FX5, G5, R, D5, E5, FX5, G5, R, D5, E5, FX5, G5, R, C5, D5, E5, F5};
    uint8_t notes_duration_AADAMS[SONG_LEN_AADAMS] = {
        16, 32, 16, 32, 16, 32, 64, 16, 32, 16, 32, 16, 32, 64, 16, 32, 16, 32, 16, 32, 64, 16, 32, 16, 32, 16, 128, 16, 16, 16, 16, 128, 16, 16, 16, 16, 128, 16, 16, 16, 16, 32, 16, 16, 16, 16, 32, 16, 16, 16, 16};

#define SONG_DRNO_ORDER 6
#define SONG_LEN_DRNO 78
#define SONG_VEL_DRNO 112
    uint8_t notes_pitch_DRNO[SONG_LEN_DRNO] = {
        B4, C5, CX5, C5, B4, C5, CX5, C5, R, E5, FX5, FX5, FX5, FX5, E5, E5, E5, E5, G5, G5, G5, G5, FX5, FX5, E5, E5, FX5, FX5, FX5, FX5, E5, E5, E5, E5, G5, G5, G5, G5, FX5, FX5, E5, DX6, D6, D6, B5, A5, B5, E5, G5, R, DX6, D6, G5, AX5, B5, G5, A5, G5, FX5, B4, E5, CX5, R, E5, G5, R, DX6, D6, G5, AX5, B5, G5, A5, G5, FX5, B4, DX5, E5};
    uint8_t notes_duration_DRNO[SONG_LEN_DRNO] = {
        64, 64, 64, 64, 64, 64, 64, 64, 64, 16, 8, 8, 16, 32, 16, 16, 16, 16, 8, 8, 16, 32, 16, 16, 16, 16, 8, 8, 16, 32, 16, 16, 16, 16, 8, 8, 16, 32, 16, 16, 16, 16, 16, 64, 16, 16, 64, 16, 16, 16, 16, 32, 16, 16, 64, 16, 8, 8, 32, 8, 16, 64, 32, 16, 16, 16, 16, 32, 16, 16, 64, 16, 8, 8, 32, 8, 16, 64};

#define SONG_BA_BA_ORDER 7
#define SONG_LEN_BA_BA 17
#define SONG_VEL_BA_BA 100
    uint8_t notes_pitch_BA_BA[SONG_LEN_BA_BA] = {
        C5, C5, G5, G5, A5, B5, C6, A5, G5, R, F5, F5, E5, E5, D5, D5, C5};
    uint8_t notes_duration_BA_BA[SONG_LEN_BA_BA] = {
        32, 32, 32, 32, 16, 16, 16, 16, 32, 32, 32, 32, 32, 32, 32, 32, 32};

#define SONG_GEORGIA_ON_MY_MIND_ORDER 8
#define SONG_LEN_GEORGIA_ON_MY_MIND 22
#define SONG_VEL_GEORGIA_ON_MY_MIND 63
    uint8_t notes_pitch_GEORGIA_ON_MY_MIND[SONG_LEN_GEORGIA_ON_MY_MIND] = {
        E5, G5, R, E5, D5, R, R, E5, A5, E5, D5, C5, D5, E5, G5, B5, A5, F5, F5, E5, E5, C5};
    uint8_t notes_duration_GEORGIA_ON_MY_MIND[SONG_LEN_GEORGIA_ON_MY_MIND] = {
        16, 64, 16, 16, 64, 16, 32, 32, 32, 32, 64, 16, 16, 32, 32, 32, 32, 32, 32, 16, 32, 128};

#define SONG_MISSIONIMPOSSI_ORDER 9
#define SONG_LEN_MISSIONIMPOSSI 24
#define SONG_VEL_MISSIONIMPOSSI 180
    uint8_t notes_pitch_MISSIONIMPOSSI[SONG_LEN_MISSIONIMPOSSI] = {
        DX6, C6, G5, DX6, C6, FX5, DX6, C6, F5, DX5, F5, R, GX5, G5, FX6, GX5, G5, F6, GX5, G5, E6, DX6, D6, R};
    uint8_t notes_duration_MISSIONIMPOSSI[SONG_LEN_MISSIONIMPOSSI] = {
        16, 16, 128, 16, 16, 128, 16, 16, 128, 16, 16, 128, 16, 16, 128, 16, 16, 128, 16, 16, 128, 16, 16, 64};

#define SONG_BEETHOVEN_ORDER 10
#define SONG_LEN_BEETHOVEN 22
#define SONG_VEL_BEETHOVEN 160
    uint8_t notes_pitch_BEETHOVEN[SONG_LEN_BEETHOVEN] = {
        C5, E5, C5, G5, C5, C6, B5, A5, G5, A5, G5, F5, E5, F5, E5, D5, C5, E5, G5, E5, C6, G5};
    uint8_t notes_duration_BEETHOVEN[SONG_LEN_BEETHOVEN] = {
        32, 32, 32, 32, 32, 32, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 32, 32, 32, 32, 32, 32};

#define SONG_FLNTSTN_ORDER 11
#define SONG_LEN_FLNTSTN 35
#define SONG_VEL_FLNTSTN 200
    uint8_t notes_pitch_FLNTSTN[SONG_LEN_FLNTSTN] = {
        GX5, CX5, R, CX6, AX5, GX5, CX5, R, GX5, FX5, F5, F5, FX5, GX5, CX5, DX5, F5, R, GX5, CX5, R, CX6, AX5, GX5, CX5, R, GX5, FX5, F5, F5, FX5, GX5, CX5, DX5, CX5};
    uint8_t notes_duration_FLNTSTN[SONG_LEN_FLNTSTN] = {
        32, 32, 16, 32, 16, 32, 32, 16, 32, 16, 16, 16, 16, 16, 32, 32, 64, 64, 32, 32, 16, 32, 16, 32, 32, 16, 32, 16, 16, 16, 16, 16, 32, 32, 64};

#define SONG_LITHIUM_ORDER 12
#define SONG_LEN_LITHIUM 28
#define SONG_VEL_LITHIUM 40
    uint8_t notes_pitch_LITHIUM[SONG_LEN_LITHIUM] = {
        A5, A5, G5, A5, D6, E6, F6, CX6, CX6, DX6, C6, AX5, C6, A5, A5, A5, G5, A5, D6, E6, F6, CX6, CX6, DX6, C6, AX5, C6, A5};
    uint8_t notes_duration_LITHIUM[SONG_LEN_LITHIUM] = {
        8, 8, 4, 16, 4, 8, 16, 4, 8, 8, 16, 8, 8, 16, 8, 8, 4, 16, 4, 8, 16, 4, 8, 8, 16, 4, 8, 8};

#define SONG_DEATH_MARCH_ORDER 13
#define SONG_LEN_DEATH_MARCH 11
#define SONG_VEL_DEATH_MARCH 125
    uint8_t notes_pitch_DEATH_MARCH[SONG_LEN_DEATH_MARCH] = {
        C5, C5, C5, C5, DX5, D5, D5, C5, C5, C5, C5};
    uint8_t notes_duration_DEATH_MARCH[SONG_LEN_DEATH_MARCH] = {
        32, 32, 16, 32, 32, 16, 32, 16, 32, 16, 64};

#define SONG_DIRE_ORDER 14
#define SONG_LEN_DIRE 27
#define SONG_VEL_DIRE 160
    uint8_t notes_pitch_DIRE[SONG_LEN_DIRE] = {
        GX5, GX5, R, R, FX5, GX5, B5, GX5, FX5, E5, E5, R, R, R, FX5, GX5, B5, B5, R, R, FX5, GX5, B5, GX5, FX5, E5, E5};
    uint8_t notes_duration_DIRE[SONG_LEN_DIRE] = {
        32, 32, 32, 16, 16, 16, 32, 16, 32, 32, 32, 32, 64, 32, 16, 16, 32, 32, 32, 16, 16, 16, 32, 16, 16, 32, 32};

#define SONG_DASBOOT_ORDER 15
#define SONG_LEN_DASBOOT 28
#define SONG_VEL_DASBOOT 100
    uint8_t notes_pitch_DASBOOT[SONG_LEN_DASBOOT] = {
        DX4, D4, C4, D4, DX4, G4, AX4, A4, G4, A4, AX4, D5, F5, R, F4, E4, D4, E4, F4, A4, C5, B4, A4, B4, C5, E5, G5, R};
    uint8_t notes_duration_DASBOOT[SONG_LEN_DASBOOT] = {
        32, 16, 16, 16, 16, 16, 32, 16, 16, 16, 16, 16, 64, 32, 32, 16, 16, 16, 16, 16, 32, 16, 16, 16, 16, 16, 64, 64};

#define SONG_SUPERMAN_ORDER 16
#define SONG_LEN_SUPERMAN 50
#define SONG_VEL_SUPERMAN 200
    uint8_t notes_pitch_SUPERMAN[SONG_LEN_SUPERMAN] = {
        D5, D5, D5, G5, R, G5, D6, R, D6, E6, D6, C6, D6, R, D5, D5, D5, G5, R, G5, D6, D6, D6, E6, C6, G5, E6, D6, R, G5, G5, G5, FX6, D6, G5, G5, G5, FX6, D6, G5, G5, G5, FX6, E6, FX6, G6, G5, G5, G5, G5};
    uint8_t notes_duration_SUPERMAN[SONG_LEN_SUPERMAN] = {
        16, 16, 16, 16, 8, 16, 64, 16, 16, 16, 16, 16, 128, 16, 16, 16, 16, 16, 8, 16, 64, 16, 16, 16, 16, 16, 16, 64, 32, 16, 16, 16, 64, 32, 16, 16, 16, 64, 32, 16, 16, 16, 16, 16, 16, 64, 16, 16, 16, 64};

#define SONG_GREENDAY_ORDER 17
#define SONG_LEN_GREENDAY 44
#define SONG_VEL_GREENDAY 50
    uint8_t notes_pitch_GREENDAY[SONG_LEN_GREENDAY] = {
        B5, A6, G6, G6, G6, G6, G6, D6, D6, D6, D6, D6, E6, E6, C6, B5, A6, G6, G6, G6, G6, D6, D6, D6, D6, D6, E6, E6, C6, B5, A6, G6, G6, G6, G6, G6, D6, D6, D6, D6, D6, E6, E6, C6};
    uint8_t notes_duration_GREENDAY[SONG_LEN_GREENDAY] = {
        4, 4, 8, 8, 8, 4, 4, 8, 8, 8, 4, 4, 8, 16, 32, 4, 4, 16, 8, 4, 8, 8, 8, 8, 4, 4, 8, 16, 32, 4, 4, 8, 8, 8, 4, 4, 8, 8, 8, 4, 4, 8, 16, 16};

#define SONG_DON_T_WANNA_MISS_A_THING_ORDER 18
#define SONG_LEN_DON_T_WANNA_MISS_A_THING 46
#define SONG_VEL_DON_T_WANNA_MISS_A_THING 125
    uint8_t notes_pitch_DON_T_WANNA_MISS_A_THING[SONG_LEN_DON_T_WANNA_MISS_A_THING] = {
        R, A5, R, A5, R, A5, R, A5, G5, R, G5, R, R, R, G5, R, G5, R, G5, G5, R, C6, AX5, R, A5, G5, F5, G5, D5, F5, R, F5, R, C5, C5, R, A5, G5, F5, R, F5, R, C5, R, G5, F5};
    uint8_t notes_duration_DON_T_WANNA_MISS_A_THING[SONG_LEN_DON_T_WANNA_MISS_A_THING] = {
        64, 8, 8, 8, 8, 16, 8, 32, 8, 8, 64, 8, 32, 16, 8, 8, 8, 8, 8, 16, 8, 32, 8, 8, 32, 16, 32, 32, 16, 16, 8, 8, 8, 8, 16, 8, 32, 16, 8, 8, 16, 8, 8, 8, 32, 32};

#define SONG_GIRLFROMIPANE_ORDER 19
#define SONG_LEN_GIRLFROMIPANE 30
#define SONG_VEL_GIRLFROMIPANE 160
    uint8_t notes_pitch_GIRLFROMIPANE[SONG_LEN_GIRLFROMIPANE] = {
        G5, E5, E5, D5, G5, E5, E5, E5, D5, G5, E5, E5, D5, G5, G5, E5, E5, E5, D5, F5, D5, D5, D5, C5, E5, C5, C5, C5, AX4, C5};
    uint8_t notes_duration_GIRLFROMIPANE[SONG_LEN_GIRLFROMIPANE] = {
        32, 16, 16, 32, 32, 16, 32, 16, 16, 32, 32, 32, 16, 32, 16, 16, 32, 16, 16, 32, 32, 32, 16, 16, 32, 32, 32, 16, 32, 64};

#define SONG_KNIGHTRIDER_ORDER 20
#define SONG_LEN_KNIGHTRIDER 64
#define SONG_VEL_KNIGHTRIDER 125
    uint8_t notes_pitch_KNIGHTRIDER[SONG_LEN_KNIGHTRIDER] = {
        E5, R, F5, E5, E5, R, E5, E5, F5, E5, E5, E5, DX5, E5, E5, E5, E5, R, F5, E5, E5, R, F5, E5, F5, E5, E5, E5, DX5, E5, E5, E5, D5, R, E5, D5, D5, R, E5, D5, E5, D5, D5, D5, C5, D5, D5, D5, D5, R, E5, D5, D5, R, E5, D5, E5, D5, D5, D5, C5, D5, D5, D5};
    uint8_t notes_duration_KNIGHTRIDER[SONG_LEN_KNIGHTRIDER] = {
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8};

#define SONG_90210_ORDER 21
#define SONG_LEN_90210 37
#define SONG_VEL_90210 140
    uint8_t notes_pitch_90210[SONG_LEN_90210] = {
        F5, AX5, C6, D6, D6, R, F5, AX5, C6, D6, DX6, F6, F6, AX5, F5, AX5, C6, D6, DX6, F6, G6, F6, DX6, DX6, D6, C6, AX5, A5, AX5, G6, F6, DX6, D6, DX6, D6, AX5, F5};
    uint8_t notes_duration_90210[SONG_LEN_90210] = {
        16, 16, 16, 32, 64, 32, 16, 16, 16, 16, 16, 32, 32, 64, 16, 16, 16, 16, 16, 16, 16, 32, 16, 32, 32, 64, 16, 32, 32, 32, 16, 16, 16, 16, 16, 16, 32};

#define SONG_HALLOWEEN_ORDER 22
#define SONG_LEN_HALLOWEEN 40
#define SONG_VEL_HALLOWEEN 180
    uint8_t notes_pitch_HALLOWEEN[SONG_LEN_HALLOWEEN] = {
        D6, G5, G5, D6, G5, G5, D6, G5, DX6, G5, D6, G5, G5, D6, G5, G5, D6, G5, DX6, G5, CX6, FX5, FX5, CX6, FX5, FX5, CX6, FX5, D6, FX5, CX6, FX5, FX5, CX6, FX5, FX5, CX6, FX5, D6, FX5};
    uint8_t notes_duration_HALLOWEEN[SONG_LEN_HALLOWEEN] = {
        16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16};

#define SONG_CASTLE_ON_A_CLOUD_ORDER 23
#define SONG_LEN_CASTLE_ON_A_CLOUD 35
#define SONG_VEL_CASTLE_ON_A_CLOUD 90
    uint8_t notes_pitch_CASTLE_ON_A_CLOUD[SONG_LEN_CASTLE_ON_A_CLOUD] = {
        A5, B5, C6, B5, A5, A5, GX5, A5, R, A5, B5, C6, B5, A5, G5, F5, E5, R, D5, E5, F5, E5, A5, B5, C6, A5, R, D5, E5, F5, E5, D5, C5, B5, A5};
    uint8_t notes_duration_CASTLE_ON_A_CLOUD[SONG_LEN_CASTLE_ON_A_CLOUD] = {
        16, 8, 8, 16, 16, 16, 16, 32, 32, 16, 8, 8, 16, 16, 16, 16, 32, 32, 16, 8, 8, 16, 16, 16, 16, 32, 32, 16, 8, 8, 16, 16, 16, 16, 32};

#define SONG_IMPERIAL_ORDER 24
#define SONG_LEN_IMPERIAL 112
#define SONG_VEL_IMPERIAL 112
    uint8_t notes_pitch_IMPERIAL[SONG_LEN_IMPERIAL] = {
        G5, R, G5, R, G5, R, DX5, R, AX5, G5, R, DX5, R, AX5, G5, R, R, D6, R, D6, R, D6, R, DX6, R, AX5, FX5, R, DX5, R, AX5, G5, R, R, G6, R, G5, R, G5, G6, R, FX6, R, F6, E6, DX6, E6, R, GX5, R, CX6, R, C6, R, B5, AX5, A5, AX5, R, DX5, R, FX5, R, DX5, R, G5, AX5, R, G5, R, AX5, D6, R, R, G6, R, G5, R, G5, G6, R, FX6, R, F6, E6, DX6, E6, R, GX5, R, CX6, R, C6, R, B5, AX5, A5, AX5, R, DX5, R, FX5, R, DX5, R, G5, G5, R, DX5, R, AX5, G5};
    uint8_t notes_duration_IMPERIAL[SONG_LEN_IMPERIAL] = {
        16, 8, 16, 8, 16, 8, 8, 4, 4, 16, 8, 8, 4, 4, 32, 16, 4, 16, 8, 16, 8, 16, 8, 8, 4, 4, 16, 8, 8, 4, 4, 32, 16, 4, 16, 8, 8, 4, 4, 16, 8, 8, 4, 4, 4, 4, 8, 16, 8, 4, 16, 8, 8, 4, 4, 4, 4, 8, 16, 8, 4, 16, 8, 8, 4, 4, 16, 8, 8, 4, 4, 32, 16, 4, 16, 8, 8, 4, 4, 16, 8, 8, 4, 4, 4, 4, 8, 16, 8, 4, 16, 8, 8, 4, 4, 4, 4, 8, 16, 8, 4, 16, 8, 8, 4, 4, 16, 8, 8, 4, 4, 32};

#define SONG_RICH_ORDER 25
#define SONG_LEN_RICH 30
#define SONG_VEL_RICH 112
    uint8_t notes_pitch_RICH[SONG_LEN_RICH] = {
        E6, E6, E6, E6, E6, E6, E5, A5, C6, E6, DX6, DX6, DX6, DX6, DX6, DX6, F5, A5, C6, DX6, D6, C6, A5, C6, C6, A5, A5, C6, E6, A6};
    uint8_t notes_duration_RICH[SONG_LEN_RICH] = {
        16, 16, 16, 16, 16, 16, 8, 8, 8, 8, 16, 16, 16, 16, 16, 16, 8, 8, 8, 8, 32, 16, 16, 16, 32, 64, 4, 4, 4, 16};

#define SONG_AXELF_ORDER 26
#define SONG_LEN_AXELF 23
#define SONG_VEL_AXELF 160
    uint8_t notes_pitch_AXELF[SONG_LEN_AXELF] = {
        FX5, A5, FX5, FX5, AX5, FX5, E5, FX5, C6, FX5, FX5, D6, CX6, A5, FX5, CX6, FX6, FX5, E5, E5, CX5, GX5, FX5};
    uint8_t notes_duration_AXELF[SONG_LEN_AXELF] = {
        32, 16, 16, 8, 16, 16, 16, 32, 16, 16, 8, 16, 16, 16, 16, 16, 16, 8, 16, 8, 16, 16, 32};

#define SONG_THINK_PINK_ORDER 27
#define SONG_LEN_THINK_PINK 39
#define SONG_VEL_THINK_PINK 70
    uint8_t notes_pitch_THINK_PINK[SONG_LEN_THINK_PINK] = {
        FX5, G5, A5, AX5, FX5, G5, A5, AX5, DX6, D6, G5, AX5, D6, CX6, C6, AX5, G5, F5, G5, R, G6, F6, D6, C6, AX5, G5, CX6, C6, CX6, C6, CX6, C6, CX6, C6, AX5, G5, F5, G5, G5};
    uint8_t notes_duration_THINK_PINK[SONG_LEN_THINK_PINK] = {
        4, 32, 4, 32, 4, 8, 4, 8, 4, 8, 4, 8, 4, 64, 4, 4, 4, 4, 32, 4, 8, 4, 8, 4, 8, 4, 4, 16, 4, 16, 4, 16, 4, 16, 4, 4, 4, 8, 32};

#define SONG_FIGARO_ORDER 28
#define SONG_LEN_FIGARO 99
#define SONG_VEL_FIGARO 250
    uint8_t notes_pitch_FIGARO[SONG_LEN_FIGARO] = {
        D5, CX5, D5, CX5, D5, R, R, D5, CX5, D5, E5, FX5, E5, FX5, G5, A5, GX5, A5, GX5, A5, R, R, A5, GX5, A5, AX5, B5, A5, G5, FX5, E5, DX5, E5, FX5, G5, FX5, E5, D5, CX5, D5, E5, D5, CX5, A4, B4, CX5, D5, D6, D5, D6, D5, D6, D5, D6, D5, D6, D5, D6, D5, D6, D5, D6, R, R, D5, D6, D5, D6, D5, D6, D5, D6, D5, D6, D5, D6, D5, D6, D5, D6, R, R, D5, D6, D5, D6, D5, D6, D5, D6, D5, D6, D5, D6, D5, D6, D5, D6, R};
    uint8_t notes_duration_FIGARO[SONG_LEN_FIGARO] = {
        16, 16, 16, 16, 16, 32, 4, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 32, 4, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 128, 32, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 128, 32, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 128};

#define SONG_ARGENTINA_ORDER 29
#define SONG_LEN_ARGENTINA 39
#define SONG_VEL_ARGENTINA 70
    uint8_t notes_pitch_ARGENTINA[SONG_LEN_ARGENTINA] = {
        E4, E4, E4, E4, F4, G4, A4, G4, R, G4, A4, A4, G4, C5, G4, F4, E4, R, E4, F4, G4, D4, D4, D4, E4, F4, C4, R, C4, D4, C4, E4, G4, R, G4, G4, A4, C5, R};
    uint8_t notes_duration_ARGENTINA[SONG_LEN_ARGENTINA] = {
        16, 16, 16, 16, 16, 16, 16, 32, 16, 16, 16, 16, 16, 32, 32, 16, 32, 16, 16, 16, 16, 16, 32, 16, 16, 16, 32, 8, 16, 16, 16, 16, 32, 8, 16, 16, 16, 32, 8};

#define SONG_MORE_THAN_WORDS_ORDER 30
#define SONG_LEN_MORE_THAN_WORDS 22
#define SONG_VEL_MORE_THAN_WORDS 90
    uint8_t notes_pitch_MORE_THAN_WORDS[SONG_LEN_MORE_THAN_WORDS] = {
        R, E5, G5, G5, E5, A5, G5, G5, R, G5, G5, A5, G5, G5, G5, G5, A5, G5, F5, F5, F5, G5};
    uint8_t notes_duration_MORE_THAN_WORDS[SONG_LEN_MORE_THAN_WORDS] = {
        64, 16, 16, 16, 16, 32, 16, 32, 16, 16, 32, 16, 32, 32, 32, 16, 16, 16, 16, 32, 16, 32};

#define SONG_MICROMELODY_ORDER 31
#define SONG_LEN_MICROMELODY 37
#define SONG_VEL_MICROMELODY 250
    uint8_t notes_pitch_MICROMELODY[SONG_LEN_MICROMELODY] = {
        B4, B4, B4, B4, B4, A4, G4, E4, G4, R, G4, A4, B4, B4, B4, B4, B4, A4, G4, E4, G4, R, G4, FX4, A4, A4, FX4, G4, B4, B4, C5, B4, A4, G4, FX4, G4, A4};
    uint8_t notes_duration_MICROMELODY[SONG_LEN_MICROMELODY] = {
        32, 32, 32, 32, 64, 32, 32, 32, 64, 64, 32, 32, 32, 32, 32, 32, 64, 32, 32, 32, 64, 64, 64, 32, 32, 32, 32, 32, 32, 64, 32, 32, 32, 32, 32, 32, 64};

#define SONG_SW_END_ORDER 32
#define SONG_LEN_SW_END 41
#define SONG_VEL_SW_END 225
    uint8_t notes_pitch_SW_END[SONG_LEN_SW_END] = {
        C5, F5, G5, GX5, AX5, GX5, C5, C5, F5, G5, GX5, C5, GX5, C5, C6, AX5, C5, F5, G5, GX5, F5, C6, GX5, F6, F5, GX5, G5, F5, C6, C6, GX5, F5, C5, C5, C5, C5, F5, F5, F5, F5, F5};
    uint8_t notes_duration_SW_END[SONG_LEN_SW_END] = {
        64, 128, 64, 16, 16, 128, 64, 32, 64, 32, 32, 32, 16, 16, 16, 128, 64, 64, 32, 32, 16, 32, 16, 128, 64, 16, 16, 16, 64, 16, 16, 16, 64, 16, 16, 16, 64, 16, 16, 16, 64};

#define SONG_STAIRWAY_ORDER 33
#define SONG_LEN_STAIRWAY 27
#define SONG_VEL_STAIRWAY 63
    uint8_t notes_pitch_STAIRWAY[SONG_LEN_STAIRWAY] = {
        A6, C6, E6, A6, B6, E6, C6, B6, C7, E6, C6, C7, FX6, D6, A6, FX6, E6, C6, A6, C6, E6, C6, A5, G5, G5, A5, A5};
    uint8_t notes_duration_STAIRWAY[SONG_LEN_STAIRWAY] = {
        16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 32, 16, 16, 16, 16, 16, 16, 32};

#define SONG_BEATLES_LET_IT_BE_ORDER 34
#define SONG_LEN_BEATLES_LET_IT_BE 27
#define SONG_VEL_BEATLES_LET_IT_BE 100
    uint8_t notes_pitch_BEATLES_LET_IT_BE[SONG_LEN_BEATLES_LET_IT_BE] = {
        E6, D6, C6, E6, G6, A6, G6, G6, G6, E6, D6, C6, A5, G5, E6, R, E6, E6, F6, E6, E6, D6, R, E6, D6, D6, C6};
    uint8_t notes_duration_BEATLES_LET_IT_BE[SONG_LEN_BEATLES_LET_IT_BE] = {
        8, 16, 32, 8, 16, 16, 16, 8, 16, 16, 8, 16, 8, 16, 32, 32, 16, 8, 16, 16, 16, 16, 8, 8, 8, 16, 64};

#define SONG_GODFATHER_ORDER 35
#define SONG_LEN_GODFATHER 37
#define SONG_VEL_GODFATHER 80
    uint8_t notes_pitch_GODFATHER[SONG_LEN_GODFATHER] = {
        E5, A5, C6, B5, A5, C6, A5, B5, A5, F5, G5, E5, E5, E5, A5, C6, B5, A5, C6, A5, B5, A5, E5, E5, D5, D5, D5, F5, GX5, B5, B5, D5, F5, GX5, A5, A5, C5};
    uint8_t notes_duration_GODFATHER[SONG_LEN_GODFATHER] = {
        16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 64, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 64, 16, 16, 16, 16, 64, 16, 16, 16, 16, 64, 16, 16};

#define SONG_CANTINA_ORDER 36
#define SONG_LEN_CANTINA 52
#define SONG_VEL_CANTINA 250
    uint8_t notes_pitch_CANTINA[SONG_LEN_CANTINA] = {
        A5, R, D6, R, A5, R, D6, R, A5, D6, R, A5, R, GX5, A5, A5, GX5, A5, G5, FX5, G5, FX5, F5, D5, R, R, A5, R, D6, R, A5, R, D6, R, A5, D6, R, A5, R, GX5, A5, R, G5, R, G5, FX5, G5, R, C6, AX5, A5, G5};
    uint8_t notes_duration_CANTINA[SONG_LEN_CANTINA] = {
        16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 32, 16, 16, 16, 32, 16, 16, 16, 32, 16, 8, 32, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 32, 16, 16, 16, 16, 32, 32, 32};

#define SONG_SWEND_ORDER 37
#define SONG_LEN_SWEND 41
#define SONG_VEL_SWEND 225
    uint8_t notes_pitch_SWEND[SONG_LEN_SWEND] = {
        C5, F5, G5, GX5, AX5, GX5, C5, C5, F5, G5, GX5, C5, GX5, C5, C6, AX5, C5, F5, G5, GX5, F5, C6, GX5, F6, F5, GX5, G5, F5, C6, C6, GX5, F5, C5, C5, C5, C5, F5, F5, F5, F5, F5};
    uint8_t notes_duration_SWEND[SONG_LEN_SWEND] = {
        64, 128, 64, 16, 16, 128, 64, 32, 64, 32, 32, 32, 16, 16, 16, 128, 64, 64, 32, 32, 16, 32, 16, 128, 64, 16, 16, 16, 64, 16, 16, 16, 64, 16, 16, 16, 64, 16, 16, 16, 64};

#define SONG_EUROPE_ORDER 38
#define SONG_LEN_EUROPE 97
#define SONG_VEL_EUROPE 140
    uint8_t notes_pitch_EUROPE[SONG_LEN_EUROPE] = {
        CX6, B5, R, CX6, R, FX5, R, D6, R, CX6, R, D6, R, CX6, R, B5, R, D6, R, CX6, R, D6, FX5, R, B5, R, A5, R, B5, R, A5, R, GX5, R, B5, R, A5, CX6, R, B5, R, CX6, FX5, R, R, D6, R, CX6, R, D6, R, CX6, R, B5, R, D6, R, CX6, R, D6, FX5, R, B5, R, A5, R, B5, R, A5, R, GX5, R, B5, R, A5, R, GX5, R, A5, R, B5, A5, B5, CX6, B5, A5, GX5, FX5, D6, CX6, R, CX6, D6, CX6, B5, CX6, R};
    uint8_t notes_duration_EUROPE[SONG_LEN_EUROPE] = {
        8, 4, 4, 16, 8, 32, 32, 4, 4, 4, 4, 4, 8, 4, 8, 32, 32, 4, 4, 4, 4, 32, 32, 32, 4, 4, 4, 4, 4, 8, 4, 8, 4, 8, 4, 8, 32, 4, 4, 4, 4, 32, 64, 32, 8, 4, 4, 4, 4, 4, 8, 4, 8, 32, 32, 4, 4, 4, 4, 32, 32, 32, 4, 4, 4, 4, 4, 8, 4, 8, 4, 8, 4, 8, 64, 8, 4, 4, 4, 4, 32, 8, 8, 16, 16, 16, 16, 32, 32, 128, 16, 8, 8, 8, 8, 64, 8};

#define SONG_BARBIEGIRL_ORDER 39
#define SONG_LEN_BARBIEGIRL 23
#define SONG_VEL_BARBIEGIRL 125
    uint8_t notes_pitch_BARBIEGIRL[SONG_LEN_BARBIEGIRL] = {
        GX5, E5, GX5, CX6, A5, R, FX5, DX5, FX5, B5, GX5, FX5, E5, R, E5, CX5, FX5, CX5, R, FX5, E5, GX5, FX5};
    uint8_t notes_duration_BARBIEGIRL[SONG_LEN_BARBIEGIRL] = {
        16, 16, 16, 16, 32, 32, 16, 16, 16, 16, 32, 16, 16, 32, 16, 16, 32, 32, 32, 16, 16, 32, 32};

#define SONG_FUNKYTOWN_ORDER 40
#define SONG_LEN_FUNKYTOWN 25
#define SONG_VEL_FUNKYTOWN 125
    uint8_t notes_pitch_FUNKYTOWN[SONG_LEN_FUNKYTOWN] = {
        C6, C6, AX5, C6, R, G5, R, G5, C6, F6, E6, C6, R, C6, C6, AX5, C6, R, G5, R, G5, C6, F6, E6, C6};
    uint8_t notes_duration_FUNKYTOWN[SONG_LEN_FUNKYTOWN] = {
        16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 64, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16};

#define SONG_LET_IT_BE_ORDER 41
#define SONG_LEN_LET_IT_BE 27
#define SONG_VEL_LET_IT_BE 100
    uint8_t notes_pitch_LET_IT_BE[SONG_LEN_LET_IT_BE] = {
        E6, D6, C6, E6, G6, A6, G6, G6, G6, E6, D6, C6, A5, G5, E6, R, E6, E6, F6, E6, E6, D6, R, E6, D6, D6, C6};
    uint8_t notes_duration_LET_IT_BE[SONG_LEN_LET_IT_BE] = {
        8, 16, 32, 8, 16, 16, 16, 8, 16, 16, 8, 16, 8, 16, 32, 32, 16, 8, 16, 16, 16, 16, 8, 8, 8, 16, 64};

#define SONG_INDIANAJONES_ORDER 42
#define SONG_LEN_INDIANAJONES 55
#define SONG_VEL_INDIANAJONES 250
    uint8_t notes_pitch_INDIANAJONES[SONG_LEN_INDIANAJONES] = {
        E5, R, F5, G5, R, C6, R, D5, R, E5, F5, R, G5, R, A5, B5, R, F6, R, A5, R, B5, C6, D6, E6, E5, R, F5, G5, R, C6, R, D6, R, E6, F6, G5, R, G5, E6, R, D6, R, G5, E6, R, D6, R, G5, F6, R, E6, R, D6, C6};
    uint8_t notes_duration_INDIANAJONES[SONG_LEN_INDIANAJONES] = {
        32, 16, 16, 16, 16, 64, 16, 32, 16, 16, 128, 32, 32, 16, 16, 16, 16, 64, 32, 32, 16, 16, 64, 64, 64, 32, 16, 16, 16, 16, 128, 32, 32, 16, 16, 128, 32, 16, 16, 32, 16, 32, 16, 16, 32, 16, 32, 16, 16, 32, 16, 32, 16, 16, 128};

#define SONG_BOLERO_ORDER 43
#define SONG_LEN_BOLERO 48
#define SONG_VEL_BOLERO 80
    uint8_t notes_pitch_BOLERO[SONG_LEN_BOLERO] = {
        C6, C6, B5, C6, D6, C6, B5, A5, C6, C6, A5, C6, C6, B5, C6, A5, G5, E5, F5, G5, G5, F5, E5, D5, E5, F5, G5, A5, G5, G5, G5, A5, B5, A5, G5, F5, E5, D5, E5, D5, C5, C5, C5, D5, E5, F5, D5, G5};
    uint8_t notes_duration_BOLERO[SONG_LEN_BOLERO] = {
        32, 16, 8, 8, 8, 8, 8, 8, 16, 8, 8, 32, 16, 8, 8, 8, 8, 8, 8, 64, 8, 8, 8, 8, 8, 8, 8, 8, 32, 32, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 16, 16, 8, 8, 16, 16, 32, 64};

#define SONG_XFILES_ORDER 44
#define SONG_LEN_XFILES 44
#define SONG_VEL_XFILES 125
    uint8_t notes_pitch_XFILES[SONG_LEN_XFILES] = {
        E5, B5, A5, B5, D6, B5, R, E5, B5, A5, B5, E6, B5, R, G6, FX6, E6, D6, E6, B5, R, G6, FX6, E6, D6, FX6, B5, R, E5, B5, A5, B5, D6, B5, R, E5, B5, A5, B5, E6, B5, R, E6, B5};
    uint8_t notes_duration_XFILES[SONG_LEN_XFILES] = {
        32, 32, 32, 32, 32, 64, 128, 32, 32, 32, 32, 32, 64, 128, 32, 32, 32, 32, 32, 64, 128, 32, 32, 32, 32, 32, 64, 128, 32, 32, 32, 32, 32, 64, 128, 32, 32, 32, 32, 32, 64, 128, 32, 64};


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


#endif /* __JUKEBOX_SONGS_H */
