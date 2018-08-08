#include "cli.h"

#define CLI_MAX_BUFFER_SIZE 64

char input_buffer[CLI_MAX_BUFFER_SIZE];
uint8_t buffer_str_size = 0;
char command_line[CLI_MAX_BUFFER_SIZE];
uint8_t command_line_size = 0;
bool command_received = false;

bool is_command_received() {
	return command_received;
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

static void copy_buffer_to_command() {
	command_line_size = buffer_str_size;
	for (uint8_t i = 0; i < command_line_size; i++) {
		command_line[i] = input_buffer[i];
	}
	command_line[command_line_size] = '\0';
}


static void clear_input_buffer(void) {
	input_buffer[0] = '\0';
	buffer_str_size = 0;
}


void usart1_isr(void) {
	if (((USART_CR1(USART1) & USART_CR1_RXNEIE) != 0)
			&& ((USART_SR(USART1) & USART_SR_RXNE) != 0)) {
		uint8_t data = usart_recv(USART1);
		if ((data != '\n') && (data != '\r')) {
			input_buffer[buffer_str_size++] = data;
			if (buffer_str_size >= CLI_MAX_BUFFER_SIZE) {
				clear_input_buffer();
			}
		} else {
			if (buffer_str_size > 1) {
				copy_buffer_to_command();
				command_received = true;
			}
			clear_input_buffer();
		}
	}
}


static void print_values(uint16_t* values, int num_values) {
	for (int i = 0; i < num_values; i++) {
		printf("%i ", values[i]);
	}
	printf("\n");
}


// This is the structure that describes each command
struct command_struct {
	char * text;
	char * syntax_hint;
	void (*functionPtr)(void);
	char is_a_candidate;
};


// Here comes all the data to fill the structure of commands for each command
char str_BUZ_syntax[] = "Syntax: BUZ ON|OFF\n";
char str_BUZ_ON[] = "BUZ ON";
static void cli_buz_on() {
	enable_jukebox();
	printf("Buzzer music ON\n");
}

char str_BUZ_OFF[] = "BUZ OFF";
static void cli_buz_off() {
	disable_jukebox();
	printf("Buzzer music OFF\n");
}

char str_BAT[] = "BAT";
static void cli_bat() {
	printf("%u mV\n", read_vbatt());
}

char str_CLK_syntax[] = "Syntax: CLK GET\n";
char str_CLK_GET[] = "CLK GET";
static void cli_clk_get() {
	printf("%lu\n", get_millisecs_since_start());
}

char str_MTR_syntax[] = "Syntax: MTR SET|GET|INC|DEC [<value>]\n";
char str_MTR_SET[] = "MTR SET";
static void cli_mtr_set() {
	if (command_line_size < 9) {
		printf(str_MTR_syntax);
		return;
	}
	int velocity = atoi(command_line + 8);
	reset_target_velocity(velocity);
	printf("%i\n", get_target_velocity());
}

char str_MTR_GET[] = "MTR GET";
static void cli_mtr_get() {
	printf("%i\n", get_target_velocity());
}

char str_MTR_INC[] = "MTR INC";
static void cli_mtr_inc() {
	int velocity = get_target_velocity() + VELOCITY_STEP;
	reset_target_velocity(velocity);
	printf("%i\n", velocity);
}

char str_MTR_DEC[] = "MTR DEC";
static void cli_mtr_dec() {
	int velocity = get_target_velocity() - VELOCITY_STEP;
	reset_target_velocity(velocity);
	printf("%i\n", velocity);
}

char str_LIN_syntax[] = "Syntax: LIN VALUES|WHITES|BLACKS|THRESHOLDS\n";
char str_LIN_VALUES[] = "LIN VALUES";
static void cli_lin_values() {
	uint16_t sensor_value[NUM_SENSORS];
	read_line_sensors(sensor_value);
	print_values(sensor_value, NUM_SENSORS);
}

char str_LIN_THRESHOLDS[] = "LIN THRESHOLDS";
static void cli_lin_thresholds() {
	print_values(get_thresholds(), NUM_SENSORS);
}

char str_LIN_WHITES[] = "LIN WHITES";
static void cli_lin_whites() {
	print_values(get_whites(), NUM_SENSORS);
}

char str_LIN_BLACKS[] = "LIN BLACKS";
static void cli_lin_blacks() {
	print_values(get_blacks(), NUM_SENSORS);
}

char str_FSM_syntax[] = "Syntax: FSM RUN|CAL\n";
char str_FSM_RUN[] = "FSM RUN";
static void cli_fsm_run() {
	update_state(GO_TO_RUN_EVENT);
	printf("GO_TO_RUN\n");
}

char str_FSM_CAL[] = "FSM CAL";
static void cli_fsm_cal() {
	update_state(FORCE_CALLIBRATION_EVENT);
	printf("CALIBRATE\n");
}

char str_PID_syntax[] = "Syntax: PID SET|GET KP|KI|KD [<value>]\n";
char str_PID_SET_KP[] = "PID SET KP";
static void cli_pid_set_kp() {
	if (command_line_size < 12) {
		printf(str_PID_syntax);
		return;
	}
	int k_value = atoi(command_line + 11);
	set_kp(k_value);
	printf("%i\n", k_value);
}

char str_PID_SET_KI[] = "PID SET KI";
static void cli_pid_set_ki() {
	if (command_line_size < 12) {
		printf(str_PID_syntax);
		return;
	}
	int k_value = atoi(command_line + 11);
	set_ki(k_value);
	printf("%i\n", k_value);
}

char str_PID_SET_KD[] = "PID SET KD";
static void cli_pid_set_kd() {
	if (command_line_size < 12) {
		printf(str_PID_syntax);
		return;
	}
	int k_value = atoi(command_line + 11);
	set_kd(k_value);
	printf("%i\n", k_value);
}

char str_PID_GET_KP[] = "PID GET KP";
static void cli_pid_get_kp() {
	int k_value;
	k_value = get_kp();
	printf("PID KP: %i\n", k_value);
}

char str_PID_GET_KI[] = "PID GET KI";
static void cli_pid_get_ki() {
	int k_value;
	k_value = get_ki();
	printf("PID KP: %i\n", k_value);
}

char str_PID_GET_KD[] = "PID GET KD";
static void cli_pid_get_kd() {
	int k_value;
	k_value = get_kd();
	printf("PID KP: %i\n", k_value);
}

char str_LED_syntax[] = "Syntax: LED ON|OFF|BLINK\n";
char str_LED_ON[] = "LED ON";
static void cli_led_on() {
	set_led();
	printf("LED ON\n");
}

char str_LED_OFF[] = "LED OFF";
static void cli_led_off() {
	clear_led();
	printf("LED OFF\n");
}

char str_LED_BLINK[] = "LED BLINK";
static void cli_led_blink() {
	async_blink();
	printf("LED BLINK\n");
}

char str_CAR_PLAY[] = "CAR PLAY";
static void cli_car_play(){
	printf("CAR PLAY!!\n");
}

char str_CAR_STOP[] = "CAR STOP";
static void cli_car_stop() {
	printf("CAR STOP!\n");
}

char str_VEL_syntax[] = "Syntax: VEL LEFT|RIGHT|ALL\n";
char str_VEL_LEFT[] = "VEL LEFT";
static void cli_vel_left(){
	uint32_t left_vel = get_left_velocity();
	printf("Vel (left): %zu\n", (unsigned int) left_vel);
}

char str_VEL_RIGHT[] = "VEL RIGHT";
static void cli_vel_right(){
	uint32_t right_vel = get_right_velocity();
	printf("Vel (right): %zu\n", (unsigned int) right_vel);
}

char str_VEL_ALL[] = "VEL ALL";
static void cli_vel_all(){
	uint32_t left_vel = get_left_velocity();
	uint32_t right_vel = get_right_velocity();
	printf("Vel %zu left, %zu right\n", (unsigned int) left_vel,
					(unsigned int) right_vel);
}

char str_ENC_syntax[] = "Syntax: ENC LEFT|RIGHT|ALL\n";
char str_ENC_LEFT[] = "ENC LEFT";
static void cli_enc_left(){
	uint32_t left_enc_ticks = get_left_encoder_ticks();
	printf("Ticks (left): %zu\n", (unsigned int) left_enc_ticks);
}

char str_ENC_RIGHT[] = "ENC RIGHT";
static void cli_enc_right(){
	uint32_t right_enc_ticks = get_right_encoder_ticks();
	printf("Ticks (right): %zu\n", (unsigned int) right_enc_ticks);
}

char str_ENC_ALL[] = "ENC ALL";
static void cli_enc_all(){
	uint32_t left_enc_ticks = get_left_encoder_ticks();
	uint32_t right_enc_ticks = get_right_encoder_ticks();
	printf("Ticks %zu left, %zu right\n", (unsigned int) left_enc_ticks,
			(unsigned int) right_enc_ticks);
}

char str_VER[] = "VER";
static void cli_ver() {
	printf("Disable Drive CLI by Mighty Escuderida\n");
	printf("Version v1.3.0\n");
}


// This is all the available commands data as an array of command structures
#define CLI_COMMANDS_LIST_SIZE 32
struct command_struct commands_list[] = {
	{.text = str_BUZ_ON, .functionPtr = cli_buz_on, .is_a_candidate = 'y', .syntax_hint = str_BUZ_syntax},
	{.text = str_BUZ_OFF, .functionPtr = cli_buz_off, .is_a_candidate = 'y', .syntax_hint = str_BUZ_syntax},
	{.text = str_BAT, .functionPtr = cli_bat, .is_a_candidate = 'y', .syntax_hint = NULL},
	{.text = str_CLK_GET, .functionPtr = cli_clk_get, .is_a_candidate = 'y', .syntax_hint = str_CLK_syntax},
	{.text = str_MTR_SET, .functionPtr = cli_mtr_set, .is_a_candidate = 'y', .syntax_hint = str_MTR_syntax},
	{.text = str_MTR_GET, .functionPtr = cli_mtr_get, .is_a_candidate ='y', .syntax_hint = str_MTR_syntax},
	{.text = str_MTR_INC, .functionPtr = cli_mtr_inc, .is_a_candidate = 'y', .syntax_hint = str_MTR_syntax},
	{.text = str_MTR_DEC, .functionPtr = cli_mtr_dec, .is_a_candidate = 'y', .syntax_hint = str_MTR_syntax},
	{.text = str_LIN_VALUES, .functionPtr = cli_lin_values, .is_a_candidate = 'y', .syntax_hint = str_LIN_syntax},
	{.text = str_LIN_THRESHOLDS, .functionPtr = cli_lin_thresholds, .is_a_candidate = 'y', .syntax_hint = str_LIN_syntax},
	{.text = str_LIN_WHITES, .functionPtr = cli_lin_whites, .is_a_candidate = 'y', .syntax_hint = str_LIN_syntax},
	{.text = str_LIN_BLACKS, .functionPtr = cli_lin_blacks, .is_a_candidate = 'y', .syntax_hint = str_LIN_syntax},
	{.text = str_FSM_RUN, .functionPtr = cli_fsm_run, .is_a_candidate = 'y', .syntax_hint = str_FSM_syntax},
	{.text = str_FSM_CAL, .functionPtr = cli_fsm_cal, .is_a_candidate = 'y', .syntax_hint = str_FSM_syntax},
	{.text = str_PID_SET_KP, .functionPtr = cli_pid_set_kp, .is_a_candidate = 'y', .syntax_hint = str_PID_syntax},
	{.text = str_PID_SET_KI, .functionPtr = cli_pid_set_ki, .is_a_candidate = 'y', .syntax_hint = str_PID_syntax},
	{.text = str_PID_SET_KD, .functionPtr = cli_pid_set_kd, .is_a_candidate = 'y', .syntax_hint = str_PID_syntax},
	{.text = str_PID_GET_KP, .functionPtr = cli_pid_get_kp, .is_a_candidate = 'y', .syntax_hint = str_PID_syntax},
	{.text = str_PID_GET_KI, .functionPtr = cli_pid_get_ki, .is_a_candidate = 'y', .syntax_hint = str_PID_syntax},
	{.text = str_PID_GET_KD, .functionPtr = cli_pid_get_kd, .is_a_candidate = 'y', .syntax_hint = str_PID_syntax},
	{.text = str_LED_ON, .functionPtr = cli_led_on,.is_a_candidate = 'y', .syntax_hint = str_LED_syntax},
	{.text = str_LED_OFF, .functionPtr = cli_led_off, .is_a_candidate = 'y', .syntax_hint = str_LED_syntax},
	{.text = str_LED_BLINK, .functionPtr = cli_led_blink, .is_a_candidate = 'y', .syntax_hint = str_LED_syntax},
	{.text = str_CAR_PLAY, .functionPtr = cli_car_play, .is_a_candidate = 'y', .syntax_hint = NULL},
	{.text = str_CAR_STOP, .functionPtr = cli_car_stop, .is_a_candidate = 'y', .syntax_hint = NULL},
	{.text = str_VEL_LEFT, .functionPtr = cli_vel_left, .is_a_candidate = 'y', .syntax_hint = str_VEL_syntax},
	{.text = str_VEL_RIGHT, .functionPtr = cli_vel_right, .is_a_candidate = 'y', .syntax_hint = str_VEL_syntax},
	{.text = str_VEL_ALL, .functionPtr = cli_vel_all, .is_a_candidate = 'y', .syntax_hint = str_VEL_syntax},
	{.text = str_ENC_LEFT, .functionPtr = cli_enc_left, .is_a_candidate = 'y', .syntax_hint = str_ENC_syntax},
	{.text = str_ENC_RIGHT, .functionPtr = cli_enc_right, .is_a_candidate = 'y', .syntax_hint = str_ENC_syntax},
	{.text = str_ENC_ALL, .functionPtr = cli_enc_all, .is_a_candidate = 'y', .syntax_hint = str_ENC_syntax},
	{.text = str_VER, .functionPtr = cli_ver, .is_a_candidate = 'y', .syntax_hint = NULL}
};


static void reset_choices_array(){
	for (int i = 0; i < CLI_COMMANDS_LIST_SIZE; i++){
		commands_list[i].is_a_candidate = 'y';
	}
}

// Returns the number of commands that have not been discarded yet or a negative
// number if there are not commands left. In the last case, returns a negative
// number with the index of the nearest command.
static int discard_commands_using_index(int index, int commands_left){
	int new_commands_left = commands_left;
	for (int i = 0; i < CLI_COMMANDS_LIST_SIZE; i++){
		if (commands_list[i].is_a_candidate == 'n') continue;

		if (commands_list[i].text[index] != command_line[index]){
			commands_list[i].is_a_candidate = 'n';
			new_commands_left = new_commands_left - 1;
			if (new_commands_left == 0){
				return i * -1;
			}
		}
	}
	return new_commands_left;
}


int get_choosen_command(){
	for (int i = 0; i < CLI_COMMANDS_LIST_SIZE; i++){
		if (commands_list[i].is_a_candidate == 'y') {
			return i;
		}
	}
	return -1;
}

// Discovers the command typed and executes it
void execute_command() {
	command_received = false;
	reset_choices_array();
	int commands_left = CLI_COMMANDS_LIST_SIZE;
	for (int i = 0; i < command_line_size; i++){
		commands_left = discard_commands_using_index(i, commands_left);
		if (commands_left == 1){
			int chosen_command_index = get_choosen_command();
			if ((chosen_command_index >= 0) &&
					(chosen_command_index < CLI_COMMANDS_LIST_SIZE) ){
				commands_list[chosen_command_index].functionPtr();
			} else {
				printf("Trying to execute out of the command list %s\n",
						command_line);
			}
			break;
		}
		if (commands_left <= 0){
			if (i < 4){
				printf("Unknown command: %s\n", command_line);
			} else {
				// this hack is explained in discard_commands_using_index()
				printf("%s", commands_list[commands_left * -1].syntax_hint);
			}
			break;
		}
	}
	clear_input_buffer();
}
