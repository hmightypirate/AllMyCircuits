
sed -i 's/^#define ENCODER_BUFFER_LEN.*$/#define ENCODER_BUFFER_LEN 20/' ../src/mightybug_a/encoder.h

# turbo state
sed -i 's/#define TURBO_TO_NORMAL_THRESHOLD.*/#define TURBO_TO_NORMAL_THRESHOLD SELECT_RUNNING_STATE_USING_ENCODERS? 4:20/' ../src/mightybug_a/fsm.h

# mappings
sed -i 's/^.*mapping vel normal/675, 700, 725 \/\/ mapping vel normal/' ../src/mightybug_a/fsm.c 

sed -i 's/^.*mapping vel turbo/575, 600, 625 \/\/ mapping vel turbo/' ../src/mightybug_a/fsm.c 

# pickle configuration
sed -i 's/#define TURBO_PICKLE .*/#define TURBO_PICKLE 1/' ../src/mightybug_a/fsm.h

sed -i 's/#define PICKLE_TURBO_VEL.*/#define PICKLE_TURBO_VEL 200/' ../src/mightybug_a/fsm.h

sed -i 's/^.*pickle 0 to 224/0,0,0,18,20,24,26,30, \/\/ pickle 0 to 224/' ../src/mightybug_a/motors.c 

sed -i 's/^.*pickle 256 to 480/34,38,42,46,50,54,58,60, \/\/ pickle 256 to 480/' ../src/mightybug_a/motors.c

sed -i 's/^.*pickle 512 to 736/66,70,74,78,82,86,90,94, \/\/ pickle 512 to 736/' ../src/mightybug_a/motors.c 

sed -i 's/^.*pickle 768 to 992/98,102,106,110,114,118,122,126,130,  \/\/ pickle 768 to 992/' ../src/mightybug_a/motors.c 


# Acceleration in corners
sed -i 's/#define ENABLE_INCDEC_NORMAL_FLAG.*/#define ENABLE_INCDEC_NORMAL_FLAG 1/' ../src/mightybug_a/fsm.h

sed -i 's/#define USE_ENCODERS_FOR_INCDEC.*/#define USE_ENCODERS_FOR_INCDEC 1/' ../src/mightybug_a/fsm.h

sed -i 's/#define STEP_NORMAL_QTY_DEC.*/#define STEP_NORMAL_QTY_DEC 15/' ../src/mightybug_a/fsm.h

sed -i 's/#define STEP_NORMAL_QTY_INC.*/#define STEP_NORMAL_QTY_INC 8/' ../src/mightybug_a/fsm.h 


# Antipickle configuration
sed -i 's/#define FLAG_MAX_VEL_DELAY.*$/#define FLAG_MAX_VEL_DELAY 0/' ../src/mightybug_a/fsm.h
sed -i 's/#define MAX_VEL_DELAY .*$/#define MAX_VEL_DELAY 20/' ../src/mightybug_a/fsm.h
sed -i 's/#define MAX_VEL_DELAY_STEP_DOWN.*$/#define MAX_VEL_DELAY_STEP_DOWN  700/' ../src/mightybug_a/fsm.h
sed -i 's/#define MAX_VEL_DELAY_STEP_UP.*$/#define MAX_VEL_DELAY_STEP_UP  200/' ../src/mightybug_a/fsm.h
