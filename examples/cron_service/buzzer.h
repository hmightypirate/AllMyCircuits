#include "notes.h"

void buzzer_setup();
void beep(int tone, int duration);
void stop();
void silence(int duration);
uint32_t get_beep_remaining();
void silence_blocking(int duration);
void beep_blocking(int tone, int duration);
void play_melody_blocking();
void play_melody();
