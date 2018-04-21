#ifndef __LIBMUSIC_H
#define __LIBMUSIC_H

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/gpio.h>

#define LIBMUSIC_TICKS_PER_SECOND 1000

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

int play_note(int note_number);
void play_music(int song_id,
                int beats_per_minute,
                uint16_t notes_number,
                uint8_t * notes_pitch,
                uint8_t * notes_duration);
void play_music_loop(void);
uint8_t is_music_playing(void);
void stop_music_play(void);
uint16_t get_current_total_notes(void);
int get_song_id(void);

#endif /* __LIBMUSIC_H */
