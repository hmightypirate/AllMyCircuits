#ifndef KEYPAD_H
#define KEYPAD_H

/* Debounce period defined in ms */
#define DEBOUNCE_PERIOD 10

void keypad_setup();
void keypad_loop();
bool keypad_read_bt1();
bool keypad_read_bt2();
bool keypad_read_bt3();

#endif /* KEYPAD_H */