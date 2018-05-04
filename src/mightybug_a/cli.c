#include "cli.h"

char command_line[80];
int command_line_size = 0;
char head[20];
char tail[80];

bool command_received = false;


/**
 * Private prototypes
 */
void usart1_isr(void);
void send_usart(char *message);
void send_message(char *message);
void command_stop(void);
void command_unknown(char *command);
void check_command_car(void);
void check_command_led(void);
void check_module(void);
void clear_head_tail(void);
void clear_command_line(void);
void set_head_tail(char *origin);
void check_command_pid(void);
void check_command_line(void);
void check_command_fsm(void);
void check_command_motor(void);
void check_command_clock(void);
void check_command_buzzer(void);
void welcome_message(void);


bool is_command_received() {
    return command_received;
}

void execute_command() {
    command_received = false;
    check_module();
    clear_command_line(); // TODO copy command_line to another variable to avoid usart rewriting before reading 
}

/**
 * _write let use printf to write to serial port
 */
int _write(int file, char *ptr, int len) {
    int i;

    if (file == 1) {
        for (i = 0; i < len; i++)
            usart_send_blocking(USART1, ptr[i]);
        return i;
    }

    errno = EIO;
    return -1;
}

void usart1_isr(void) {
    if (((USART_CR1(USART1) & USART_CR1_RXNEIE) != 0) &&
    ((USART_SR(USART1) & USART_SR_RXNE) != 0)) {
        uint8_t data = usart_recv(USART1);
        if (data != '\n') {
            command_line[command_line_size++] = data;
        } else {
            command_line[command_line_size] = '\0';
            command_received = true;
        }
    }
}

void clear_head_tail(void) {
    head[0] = '\0';
    tail[0] = '\0';
};

bool is_head(char *key) {
    if (strcmp(head, key) == 0) return true;
    else return false;
}

void command_stop() {
    printf("STOPPING\n");
};

void command_unknown(char *command) {
    printf("Unknown command: %s\n", command);
};

void check_command_car() {
    set_head_tail(tail);
    if (is_head("PLAY")) {
        printf("Go!\n");
    } else if (is_head("STOP")) {
        command_stop();
    } else {
        command_unknown(head);
    }
};

void check_command_led() {
    set_head_tail(tail);
    if (is_head("ON")) {
        set_led();
        printf("LED ON\n");
    } else if (is_head("OFF")) {
        clear_led();
        printf("LED OFF\n");
    } else if (is_head("BLINK")) {
        async_blink();
        printf("LED BLINK\n");
    } else {
        printf("Syntax: LED ON|OFF|BLINK\n");
    }
};


void check_module() {
    set_head_tail(command_line);
    if (is_head("STOP")) command_stop();
    else if (is_head("CAR")) check_command_car();
    else if (is_head("PID")) check_command_pid();
    else if (is_head("MTR")) check_command_motor();
//     else if (is_head("ENC")) check_command_encoder();
    else if (is_head("LIN")) check_command_line();
    else if (is_head("BUZ")) check_command_buzzer();
//     else if (is_head("RST")) command_reset();
    else if (is_head("LED")) check_command_led();
    else if (is_head("FSM")) check_command_fsm();
    else if (is_head("CLK")) check_command_clock();
    else if (is_head("VER")) welcome_message();
    else command_unknown(head);
    clear_head_tail();
};


void welcome_message() {
    printf("Bat Bolido CLI by Mighty Escuderida\n");
    printf("Version v1.1.0\n");
}

void clear_command_line(void) {
    command_line[0] = '\0';
    command_line_size = 0;
}

void set_head_tail(char *origin) {
    char *tail_ptr = NULL;
    int blank_pos = strlen(origin);

    tail_ptr = strchr(origin, ' ');
    if (tail_ptr != NULL)
        blank_pos = (int)(tail_ptr - origin);

    /* Set head */
    memcpy(head, origin, blank_pos);
    head[blank_pos] = '\0';

    /* Set tail */
    if (tail_ptr != NULL) {
        memcpy(tail, tail_ptr + 1, strlen(tail_ptr + 1));
        tail[strlen(tail_ptr + 1)] = '\0';
    } else
        tail[0] = '\0';
}

void check_k_pid_set() {

    set_head_tail(tail);

    int k_value;
    k_value = atoi(tail);

    if (is_head("KP")) {
        set_kp(k_value);
        printf("%i\n", k_value);
    } else if (is_head("KI")) {
        set_ki(k_value);
        printf("%i\n", k_value);
    } else if (is_head("KD")) {
        set_kd(k_value);
        printf("%i\n", k_value);
    } else {
        printf("Syntax: PID SET KP|KI|KD <value>\n");
    }
}

void check_k_pid_get() {
    
    set_head_tail(tail);

    int k_value;

    if (is_head("KP")) {
        k_value = get_kp();
        printf("PID KP: %i\n", k_value);
    } else if (is_head("KI")) {
        k_value = get_ki();
        printf("PID KI: %i\n", k_value);
    } else if (is_head("KD")) {
        k_value = get_kd();
        printf("PID KD: %i\n", k_value);
    } else {
        printf("Syntax: PID GET KP|KI|KD\n");
    }
}

void check_command_pid() {
    set_head_tail(tail);

    if (strcmp(head, "SET") == 0) {
        check_k_pid_set();
    } else if (strcmp(head, "GET") == 0) {
        check_k_pid_get();
    } else {
        printf("Syntax: PID SET|GET KP|KI|KD [<value>]\n");
    }
}


void check_command_fsm() {
    set_head_tail(tail);
    if (is_head("RUN")) {
        update_state(GO_TO_RUN_EVENT);
        printf("GO_TO_RUN\n");
    } else if (is_head("CAL")) {
        update_state(FORCE_CALLIBRATION_EVENT);
        printf("CALIBRATE\n");
    } else {
        printf("Syntax: FSM RUN|CAL\n");
    }
}

void print_values(uint16_t* values, int num_values) {
    for (int i = 0; i < num_values; i++) {
        printf("%i ", values[i]);
    }
    printf("\n");
}

void check_command_line() {
    set_head_tail(tail);
    
    /* read data from sensors */
    if (is_head("VALUES")) {
        uint16_t sensor_value[NUM_SENSORS];
        read_line_sensors(sensor_value);
        print_values(sensor_value, NUM_SENSORS);
    } else if (is_head("THRESHOLDS")) {
        print_values(get_thresholds(), NUM_SENSORS);
    } else if (is_head("WHITES")) {
        print_values(get_whites(), NUM_SENSORS);
    } else if (is_head("BLACKS")) {
        print_values(get_blacks(), NUM_SENSORS);
    } else {
        printf("Syntax: LIN VALUES|WHITES|BLACKS|THRESHOLDS\n");
    }
}

void check_command_motor() {
    set_head_tail(tail);
    if (is_head("SET")) {
        int velocity = atoi(tail);
        reset_target_velocity(velocity);
        printf("%i\n", velocity);
    } else if (is_head("GET")) {
        printf("%i\n", get_target_velocity());
    } else if (is_head("INC")) {
        int velocity = get_target_velocity() + VELOCITY_STEP;
        reset_target_velocity(velocity);
        printf("%i\n", velocity);        
    } else if (is_head("DEC")) {
        int velocity = get_target_velocity() - VELOCITY_STEP;
        reset_target_velocity(velocity);
        printf("%i\n", velocity);
    } else {
        printf("Syntax: MTR SET|GET [<value>]\n");
    }
}

void check_command_clock() {
    set_head_tail(tail);
    if (is_head("GET")) {
        printf("%lu\n", get_millisecs_since_start());        
    } else {
        printf("Syntax: CLK GET\n");
    }
}

void check_command_buzzer() {
    set_head_tail(tail);
    if (is_head("ON")) {
        enable_jukebox();
        printf("Buzzer music ON\n");
    } else if (is_head("OFF")) {
        disable_jukebox();
        printf("Buzzer music OFF\n");
    } else {
        printf("Syntax: BUZ ON|OFF\n");
    }
}