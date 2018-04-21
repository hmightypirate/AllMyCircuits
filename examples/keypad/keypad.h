#ifndef KEYPAD_H
#define KEYPAD_H

void keypad_setup();
void keypad_loop();

/* TODO: Give buttons self explanatory names when their function is clearer */
bool keypad_read_bt1();
bool keypad_read_bt2();
bool keypad_read_bt3();

#endif /* KEYPAD_H */