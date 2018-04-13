#include "cli.h"

char command_line[80];
int command_line_size = 0;
char head[20];
char tail[80];
char message[80];

bool command_received = true;

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
    if (strcmp(head, "ON") == 0) {
        send_message("LED ON\n");
    } else if (strcmp(head, "OFF") == 0) {
        send_message("LED OFF\n");
    } else {
        send_message("LED BLINK\n");
    }
};

void clear_head_tail(void) {
    head[0] = '\0';
    tail[0] = '\0';
};

bool is_head(char *key) {
    if (strcmp(head, key) == 0) return true;
    else return false;
}

void check_module() {
    set_head_tail(command_line);
    if (strcmp(head, "STOP") == 0) command_stop();
    else if (strcmp(head, "CAR") == 0) check_command_car();
    else if (strcmp(head, "PID") == 0) check_command_pid();
//     else if (strcmp(head, "MTR") == 0) check_command_motor();
//     else if (strcmp(head, "ENC") == 0) check_command_encoder();
//     else if (strcmp(head, "LIN") == 0) check_command_line();
//     else if (strcmp(head, "BUZ") == 0) check_command_buzzer();
//     else if (strcmp(head, "RST") == 0) command_reset();
    else if (strcmp(head, "LED") == 0) check_command_led();
    else command_unknown(head);
    clear_head_tail();
};


void welcome_message() {
    send_message("Agonizing Automobile CLI by Mighty Escuderida\n");
    send_message("Version v0.1\n");
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

void check_command_pid() {

    int k_value;

    set_head_tail(tail);
    if (strcmp(head, "SET") == 0) {
        set_head_tail(tail);
        k_value = atoi(tail);
        if (strcmp(head, "KP") == 0) {
            set_kp(k_value);
            sprintf(message, "%i\n", k_value);
            send_message(message);
        } else if (strcmp(head, "KI") == 0) {
            set_ki(k_value);
            sprintf(message, "%i\n", k_value);
            send_message(message);
        } else if (strcmp(head, "KD") == 0) {
            set_kd(k_value);
            sprintf(message, "%i\n", k_value);
            send_message(message);
        } else {
            send_message("Syntax: PID SET KP|KI|KD <value>\n");
        }
    } else if (strcmp(head, "GET") == 0) {
        set_head_tail(tail);
        int k_value;
        if (strcmp(head, "KP") == 0) {
            k_value = get_kp();
            sprintf(message, "PID KP: %i\n", k_value);
            send_message(message);
        } else if (strcmp(head, "KI") == 0) {
            k_value = get_ki();
            sprintf(message, "PID KI: %i\n", k_value);
            send_message(message);
        } else if (strcmp(head, "KD") == 0) {
            k_value = get_kd();
            sprintf(message, "PID KD: %i\n", k_value);
            send_message(message);
        } else {
            send_message("Syntax: PID GET KP|KI|KD <value>\n");
        }        
    } else {
        send_message("Syntax: PID SET|GET KP|KI|KD <value>\n");
    }
}
