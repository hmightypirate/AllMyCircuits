#include "cli.h"

#define CLI_MAX_BUFFER_SIZE 64

uint32_t result = 0;
typedef struct {
	void *ptr;
	type_t type;
	uint8_t length;
} cli_var_t;

cli_var_t cli_vars[255];

void add_config_var(void *ptr, uint8_t id, type_t type, uint8_t length)
{
	cli_vars[id].ptr = ptr;
	cli_vars[id].type = type;
	cli_vars[id].length = length;
}


void set_var(uint8_t var_id, uint8_t index, void *value_ptr)
{
	if (index >= cli_vars[var_id].length) return;

	switch (cli_vars[var_id].type) {
		case UINT8_T:
			((uint8_t *)(cli_vars[var_id].ptr))[index] = *((uint8_t *)value_ptr);
			break;
		case UINT16_T:
			((uint16_t *)(cli_vars[var_id].ptr))[index] = *((uint16_t *)value_ptr);
			break;
		case UINT32_T:
			((uint32_t *)(cli_vars[var_id].ptr))[index] = *((uint32_t *)value_ptr);
			break;	
		case INT8_T:
			((int8_t *)(cli_vars[var_id].ptr))[index] = *((int8_t *)value_ptr);
			break;
		case INT16_T:
			((int16_t *)(cli_vars[var_id].ptr))[index] = *((int16_t *)value_ptr);
			break;
		case INT32_T:
			((int32_t *)(cli_vars[var_id].ptr))[index] = *((int32_t *)value_ptr);
			break;
		default:
			break;
	}
}

void *get_var(uint8_t var_id, uint8_t index)
{
	if (index >= cli_vars[var_id].length) return NULL;

	switch (cli_vars[var_id].type) {
		case UINT8_T:
			result = (uint32_t)(*(((uint8_t *)(cli_vars[var_id].ptr)) + index));
			break;
		case UINT16_T:
			result = (uint32_t)(*(((uint16_t *)(cli_vars[var_id].ptr)) + index));
			break;
		case UINT32_T:
			result = (uint32_t)(*(((uint32_t *)(cli_vars[var_id].ptr)) + index));
			break;
		case INT8_T:
			result = (uint32_t)(*(((int8_t *)(cli_vars[var_id].ptr)) + index));
			break;
		case INT16_T:
			result = (uint32_t)(*(((int16_t *)(cli_vars[var_id].ptr)) + index));
			break;
		case INT32_T:
			result = (uint32_t)(*(((int32_t *)(cli_vars[var_id].ptr)) + index));
			break;
		default:
			break;
	}

	return &result;
}

char input_buffer[CLI_MAX_BUFFER_SIZE];
uint8_t buffer_str_size = 0;
char command_line[CLI_MAX_BUFFER_SIZE];
uint8_t command_line_size = 0;
bool command_received = false;

bool is_command_received() {
	return command_received;
}


void _putchar(char character)
{
  write_dma(&character, 1);
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
char str_BAT[] = "BAT";
static void cli_bat() {
	printf("%u mV\n", read_vbatt());
}

char str_STREAM_BAT[] = "STREAM_BAT";
static void cli_sbat() {
	update_state(STREAM_BAT_EVENT);
}

char str_STREAM_BUZ[] = "STREAM_BUZ";
static void cli_sbuz() {
	update_state(STREAM_BUZ_EVENT);
}

char str_STREAM_LED[] = "STREAM_LED";
static void cli_sled() {
	update_state(STREAM_LED_EVENT);
}

char str_STREAM_LINE[] = "STREAM_LINE";
static void cli_sline() {
	update_state(STREAM_LINE_EVENT);
}

char str_STREAM_MOTORS[] = "STREAM_MOTORS";
static void cli_smotors() {
	update_state(STREAM_MOTORS_EVENT);
}

char str_CLK_GET[] = "CLK";
static void cli_clk_get() {
	printf("%lu\n", get_millisecs_since_start());
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


char str_VER[] = "VER";
static void cli_ver() {
	printf("Disable Drive CLI by Mighty Escuderida\n");
	printf("Version v1.3.0\n");
}


char str_GET[] = "GET";
static void cli_get() {
	uint8_t var_id = atoi(command_line + 3);
	uint8_t index = atoi(command_line + 7);
	uint32_t value = *((uint32_t *)get_var(var_id, index));
	printf("Get var %u %u %u\n", var_id, index, (unsigned int)value);
}

char str_SET[] = "SET";
static void cli_set() {
	uint8_t var_id = atoi(command_line + 3);
	uint8_t index = atoi(command_line + 7);
	int32_t value = atoi(command_line + 11);
	set_var(var_id, index, (void *)&value);
	printf("Set var %u %u %u\n", var_id, index, (unsigned int)value);
}

char str_MAP_ALL[] = "MAP ALL";

int idx_map = 0;
static void cli_map_all() {

  mapping_e last_map = get_mapping_info();

  if (idx_map > MAX_MAP_STATES) 
    idx_map = 0;
  //for (int i = 0; i < MAX_MAP_STATES; i++)
  //  {
      printf("%i State: %d, Ticks left: %u, Ticks right: %u Total: %u Lrgst: %li Endsec %li Finish %li Current %li\n",
	     idx_map,
	     last_map.mapstates[idx_map],
	     (unsigned int) last_map.agg_left_ticks[idx_map],
	     (unsigned int) last_map.agg_right_ticks[idx_map],
	     (unsigned int) last_map.agg_total_ticks[idx_map],
	     get_largest_rect(),
	     get_end_sector_largest_rect(),
	     get_finish_mapping_largest_rect(),
	     get_current_synchro_sector());

      

      //}
      idx_map += 1;

  
}

// This is all the available commands data as an array of command structures
#define CLI_COMMANDS_LIST_SIZE 15
struct command_struct commands_list[] = {
	{.text = str_BAT, .functionPtr = cli_bat, .is_a_candidate = 'y', .syntax_hint = NULL},
	{.text = str_CLK_GET, .functionPtr = cli_clk_get, .is_a_candidate = 'y', .syntax_hint = NULL},
	{.text = str_LIN_VALUES, .functionPtr = cli_lin_values, .is_a_candidate = 'y', .syntax_hint = str_LIN_syntax},
	{.text = str_LIN_THRESHOLDS, .functionPtr = cli_lin_thresholds, .is_a_candidate = 'y', .syntax_hint = str_LIN_syntax},
	{.text = str_LIN_WHITES, .functionPtr = cli_lin_whites, .is_a_candidate = 'y', .syntax_hint = str_LIN_syntax},
	{.text = str_LIN_BLACKS, .functionPtr = cli_lin_blacks, .is_a_candidate = 'y', .syntax_hint = str_LIN_syntax},
	{.text = str_GET, .functionPtr = cli_get, .is_a_candidate = 'y', .syntax_hint = NULL},
	{.text = str_SET, .functionPtr = cli_set, .is_a_candidate = 'y', .syntax_hint = NULL},
	{.text = str_VER, .functionPtr = cli_ver, .is_a_candidate = 'y', .syntax_hint = NULL},
	{.text = str_STREAM_BAT, .functionPtr = cli_sbat, .is_a_candidate = 'y', .syntax_hint = NULL},
	{.text = str_STREAM_BUZ, .functionPtr = cli_sbuz, .is_a_candidate = 'y', .syntax_hint = NULL},
	{.text = str_STREAM_LED, .functionPtr = cli_sled, .is_a_candidate = 'y', .syntax_hint = NULL},	
	{.text = str_STREAM_LINE, .functionPtr = cli_sline, .is_a_candidate = 'y', .syntax_hint = NULL},
	{.text = str_STREAM_MOTORS, .functionPtr = cli_smotors, .is_a_candidate = 'y', .syntax_hint = NULL},
	{.text = str_MAP_ALL, .functionPtr = cli_map_all, .is_a_candidate = 'y', .syntax_hint = NULL}
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
