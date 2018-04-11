#ifndef __COMMONS_H
#define __COMMONS_H


/** 
 * Maximum PWM period (max velocity)
*/
#define MAX_VEL_MOTOR 1024
#define INITIAL_TARGET_VELOCITY 200

#define DELAY(x) for (int i=0; i < x; i++) __asm__("nop");

#endif /* __COMMONS_H */
