#include "cli.h"

char command_line[80];
int command_line_size = 0;
char head[20];
char tail[80];
char message[80];

bool command_received = false;

bool is_command_received() {
    return command_received;
}

void execute_command() {
    command_received = false;
    check_module();
    clear_command_line(); // TODO copy command_line to another variable to avoid usart rewriting before reading 
}

void send_usart(char *message) {
    for (int i = 0; i < (int)strlen(message); i++){
        usart_send_blocking(USART1, message[i]);
    }
};

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
    send_message("STOPPING\n");
};

void command_unknown(char *command) {
    sprintf(message, "Unknown command: %s\n", command);
    send_message(message);
};

void check_command_car() {
    set_head_tail(tail);
    if (strcmp(head, "PLAY") == 0) {
        send_message("Go!\n");
    } else if (strcmp(head, "STOP") == 0) {
        command_stop();
    } else {
        command_unknown(head);
    }
};

void check_command_led() {
    set_head_tail(tail);
    if (is_head("ON")) {
        set_led();
        send_message("LED ON\n");
    } else if (is_head("OFF")) {
        clear_led();
        send_message("LED OFF\n");
    } else if (is_head("BLINK")) {
        async_blink();
        send_message("LED BLINK\n");
    } else {
        send_message("Syntax: LED ON|OFF|BLINK\n");
    }
};


void check_module() {
    set_head_tail(command_line);
    if (strcmp(head, "STOP") == 0) command_stop();
    else if (strcmp(head, "CAR") == 0) check_command_car();
    else if (strcmp(head, "PID") == 0) check_command_pid();
    else if (strcmp(head, "MTR") == 0) check_command_motor();
//     else if (strcmp(head, "ENC") == 0) check_command_encoder();
    else if (strcmp(head, "LIN") == 0) check_command_line();
    else if (strcmp(head, "BUZ") == 0) check_command_buzzer();
//     else if (strcmp(head, "RST") == 0) command_reset();
    else if (strcmp(head, "LED") == 0) check_command_led();
    else if (strcmp(head, "FSM") == 0) check_command_fsm();
    else if (strcmp(head, "CLK") == 0) check_command_clock();
    else if (is_head("VER")) welcome_message();
    else command_unknown(head);
    clear_head_tail();
};


void welcome_message() {
    send_message("Bat Bolido CLI by Mighty Escuderida\n");
    send_message("Version v1.1.0\n");
}

void send_message(char *message) {
    send_usart(message);
};


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
        sprintf(message, "%i\n", k_value);
        send_message(message);
    } else if (is_head("KI")) {
        set_ki(k_value);
        sprintf(message, "%i\n", k_value);
        send_message(message);
    } else if (is_head("KD")) {
        set_kd(k_value);
        sprintf(message, "%i\n", k_value);
        send_message(message);
    } else {
        send_message("Syntax: PID SET KP|KI|KD <value>\n");
    }
}

void check_k_pid_get() {
    
    set_head_tail(tail);

    int k_value;

    if (is_head("KP")) {
        k_value = get_kp();
        sprintf(message, "PID KP: %i\n", k_value);
        send_message(message);
    } else if (is_head("KI")) {
        k_value = get_ki();
        sprintf(message, "PID KI: %i\n", k_value);
        send_message(message);
    } else if (is_head("KD")) {
        k_value = get_kd();
        sprintf(message, "PID KD: %i\n", k_value);
        send_message(message);
    } else {
        send_message("Syntax: PID GET KP|KI|KD <value>\n");
    }      
}

void check_command_pid() {
    set_head_tail(tail);

    if (strcmp(head, "SET") == 0) {
        check_k_pid_set();
    } else if (strcmp(head, "GET") == 0) {
        check_k_pid_get();
    } else {
        send_message("Syntax: PID SET|GET KP|KI|KD <value>\n");
    }
}


void check_command_fsm() {
    set_head_tail(tail);
    if (is_head("RUN")) {
        update_state(GO_TO_RUN_EVENT);
        send_message("GO_TO_RUN");
    } else if (is_head("CAL")) {
        update_state(FORCE_CALLIBRATION_EVENT);
        send_message("CALIBRATE");
    } else {
        send_message("Syntax: FSM RUN|CAL");
    }
}

void print_values(uint16_t* values, int num_values) {
    message[0] = '\0';
    for (int i = 0; i < num_values; i++) {
        sprintf(message, "%s%i ", message, values[i]);
    }
    
    // Add a LF
    message[strlen(message)-1] = '\n';
    send_message(message);
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
        send_message("Syntax: LIN VALUES|WHITES|BLACKS|THRESHOLDS");
    }
}

void check_command_motor() {
    set_head_tail(tail);
    if (is_head("SET")) {
        int velocity = atoi(tail);
        reset_target_velocity(velocity);
        sprintf(message, "%i\n", velocity);
        send_message(message);
    } else if (is_head("GET")) {
        sprintf(message, "%i\n", get_target_velocity());
        send_message(message);
    } else if (is_head("INC")) {
        int velocity = get_target_velocity() + VELOCITY_STEP;
        reset_target_velocity(velocity);
        sprintf(message, "%i\n", velocity);
        send_message(message);        
    } else if (is_head("DEC")) {
        int velocity = get_target_velocity() - VELOCITY_STEP;
        reset_target_velocity(velocity);
        sprintf(message, "%i\n", velocity);
        send_message(message);
    } else {
        send_message("Syntax: MTR SET|GET [<value>]");
    }
}

void check_command_clock() {
    set_head_tail(tail);
    if (is_head("GET")) {
        sprintf(message, "%lu\n", get_millisecs_since_start());
        send_message(message);        
    } else {
        send_message("Syntax: CLK GET");
    }
}

void check_command_buzzer() {
    set_head_tail(tail);
    if (is_head("ON")) {
        enable_jukebox();
        send_message("Buzzer music ON");
    } else if (is_head("OFF")) {
        disable_jukebox();
        send_message("Buzzer music OFF");
    } else {
        send_message("Syntax: BUZ ON|OFF");
    }
}