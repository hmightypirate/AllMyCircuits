#include "main.h"

uint16_t line_sensor_value[NUM_SENSORS];
uint32_t sync_iterations = 0;
uint32_t current_loop_millisecs = 0;

uint32_t pidvel_map_ms = 0;

uint32_t last_ms_inline = 0;
uint32_t last_ms_outline = 0;
uint32_t running_loop_millisecs = 0;

int32_t line_error = 0;

uint32_t delayed_start_time = 0;

rnstate_e last_rn_state = RUNNING_NORMAL;

void just_run_state(void);

void go_to_normal(void)
{
	reset_sequential_readings();
	update_running_state(SET_NORMAL_MODE_STATE);
}

/*
 * @brief get next sub-state (running)
 *
 */
void check_running_mode_thresholds(int16_t avg_error)
{
	switch (get_running_state()) {
	case RUNNING_NORMAL:
	case RUNNING_NOOL:
	case RUNNING_TURBO:
		if (avg_error < NORMAL_TO_TURBO_THRESHOLD) {
			if (ENABLE_TURBO_MODE)
				update_running_state(SET_TURBO_MODE_STATE);
			else
				go_to_normal();
		} else if (avg_error > NORMAL_TO_NOOL_THRESHOLD) {
			if (ENABLE_NOOL_MODE)
				update_running_state(SET_NOOL_MODE_STATE);
			else
				go_to_normal();
		} else if ((avg_error > TURBO_TO_NORMAL_THRESHOLD) &&
			   (avg_error < NOOL_TO_NORMAL_THRESHOLD)) {
			go_to_normal();
		}
		break;
	default:
		return;
	}
}

/*
 * @brief next state using inline
 */
void check_running_mode_inline()
{
	switch (get_running_state()) {
	case RUNNING_NORMAL:

		if (get_inline_change() && FORCE_STATECHANGE_ALL_INLINE) {
			update_running_state(SET_TURBO_MODE_STATE);
		}

		break;

	case RUNNING_TURBO:
		if (get_inline_change() && FORCE_STATECHANGE_ALL_INLINE) {
			update_running_state(NEAR_CORNER_EVENT);
		}

		break;
	case RUNNING_BRAKE:
		if (get_inline_change() && FORCE_STATECHANGE_ALL_INLINE) {
			update_running_state(SET_NORMAL_MODE_STATE);
		}
		break;
	default:
		break;
	}
}

void keypad_events(void)
{
	if (button_released(BUTTON1)) {
		update_state(BUTTON1_RELEASED_EVENT);
	}

	if (button_pressed(BUTTON2)) {
		update_state(BUTTON2_PRESSED_EVENT);
	}

	if (button_pressed(BUTTON3)) {
		update_state(BUTTON3_PRESSED_EVENT);
	}
}

void turbo_running_state()
{
	set_target_as_turbo();

	jukebox_setcurrent_song(NO_SONG);
	if (FLAG_CIRCUIT_MAPPING) {
		if (is_hyper_turbo_safe(ST_LINE)) {
			set_target_as_hyper_turbo();
			jukebox_setcurrent_song(SOPRANO_BEAT_ORDER);
		}

		if (get_end_of_mapping()) {
			jukebox_setcurrent_song(SONG_SUPERMAN_ORDER);
		}
	}

	reset_pids_turbo();

	if (RUNNING_STATE_PITCH)
		jukebox_setcurrent_song(SOPRANO_BEAT_ORDER);

	set_led_mode(LED_1, OFF);
	set_led_mode(LED_2, OFF);

	just_run_state();
}

void brake_running_state()
{
	set_target_as_brake();
	reset_pids_turbo();

	jukebox_setcurrent_song(NO_SONG);
	if (RUNNING_STATE_PITCH)
		jukebox_setcurrent_song(SOPRANO_BEAT_ORDER);

	set_led_mode(LED_1, ON);
	set_led_mode(LED_2, ON);

	just_run_state();
}

void normal_running_state()
{
	set_target_as_normal();

	if (ENABLE_INCDEC_NORMAL_FLAG) {
		if (USE_ENCODERS_FOR_INCDEC) {
			update_target_normal_with_encoders();
		} else {
			if (sync_iterations % ITS_INCDEC_NORMAL == 0) {
				update_target_normal();
			}
		}
	}

	if (FLAG_CIRCUIT_MAPPING && ALLOW_MAPPING_IN_CORNERS) {
		if (is_hyper_turbo_safe(ST_LINE)) {
			set_target_as_hyper_turbo_corner();
			jukebox_setcurrent_song(SOPRANO_BEAT_ORDER);
		}

		if (get_end_of_mapping()) {
			jukebox_setcurrent_song(SONG_SUPERMAN_ORDER);
		}
	}

	reset_pids_normal();

	if (!FLAG_CIRCUIT_MAPPING)
		jukebox_setcurrent_song(NO_SONG);
	if (RUNNING_STATE_PITCH)
		jukebox_setcurrent_song(TENOR_BEAT_ORDER);

	set_led_mode(LED_1, OFF);
	set_led_mode(LED_2, OFF);

	just_run_state();
}

void nool_running_state()
{
	set_target_as_nool();
	reset_pids_nool();

	jukebox_setcurrent_song(NO_SONG);
	if (RUNNING_STATE_PITCH)
		jukebox_setcurrent_song(BASS_BEAT_ORDER);

	set_led_mode(LED_1, OFF);
	set_led_mode(LED_2, OFF);

	just_run_state();
}

void stop_running_state()
{
	if (!is_out_of_line()) {
		update_running_state(SET_NORMAL_MODE_STATE);
	}
	stop_motors();

	jukebox_setcurrent_song(OUT_OF_LINE_SONG);

	set_led_mode(LED_1, OFF);
	set_led_mode(LED_2, ON);
}

void recovery_running_state()
{

	// set_target_as_normal();
	// reset_pids_normal();
	if (USE_RECOVERY_VELOCITY)
		set_target_as_recovery();

	set_led_mode(LED_1, BLINK);
	set_led_mode(LED_2, BLINK);

	just_run_state();

	if ((current_loop_millisecs - last_ms_inline) >
	    MAX_RUNNING_RECOVERY_TIME) {
		update_running_state(STOP_RUNNING_EVENT);
	}
}

void check_rn_state(void)
{
	rnstate_e running_state = get_running_state();

	switch (running_state) {
	case RUNNING_TURBO:
		turbo_running_state();
		break;
	case RUNNING_NORMAL:
		normal_running_state();
		break;
	case RUNNING_NOOL:
		nool_running_state();
		break;
	case RUNNING_RECOVERY:
		recovery_running_state();
		break;
	case RUNNING_BRAKE:
		brake_running_state();
		break;
	case RUNNING_STOP:
	default:
		stop_running_state();
	}
}

void music_update(void)
{
	jukebox_update(current_loop_millisecs);
}

void calibration_state(void)
{
	read_line_sensors(line_sensor_value);

	calibrate_sensors(line_sensor_value);

	/* stop motors during calibration */
	stop_motors();
	/* led is on during calibration */
	set_led_mode(LED_1, ON);
	set_led_mode(LED_2, ON);

	jukebox_setcurrent_song(CALIBRATION_SONG);
}

void idle_state(void)
{
	/* reset synchro state */
	reset_synchro();
	/* reset out of line measurements, resets calibration */
	reset_all_inline();
	reset_calibration_values();
	stop_motors();

	// Check if the mapping was already done and we must go to the
	// synchro phase
	if (FLAG_CIRCUIT_MAPPING) {
		check_synchro_start();
	}

	delayed_start_time = 0;
	running_loop_millisecs = 0;

	jukebox_setcurrent_song(NO_SONG);

	/* Clear led during idle state */
	set_led_mode(LED_1, ON);
	set_led_mode(LED_2, OFF);
}

void out_of_battery_state(void)
{
	/* Disable sensors */
	disable_line_sensors();

	/* Stop motors */
	stop_motors();

	/* Led off */
	set_led_mode(LED_1, BLINK);
	set_led_mode(LED_2, OFF);

	jukebox_setcurrent_song(OUT_OF_BATTERY_SONG);
}

void delayed_start_inertia_state(void)
{
	if (delayed_start_time == 0) {
		delayed_start_time = current_loop_millisecs;
	}

	/* Stop motors */
	stop_motors();

	if (current_loop_millisecs - delayed_start_time >
	    DELAYED_START_WAIT_TIME) {
		delayed_start_time = 0;
		update_state(DELAYED_START_TIMEOUT_EVENT);
	}

	jukebox_setcurrent_song(NO_SONG);

	/* Led on */
	set_led_mode(LED_1, OFF);
	set_led_mode(LED_2, ON);
}

void delayed_start_state(void)
{
	if (delayed_start_time == 0) {
		if (get_calibrated_sensors_count() <
		    NUM_SENSORS - MAX_NUM_NOT_CALLIBRATED_SENSORS) {
			update_state(FORCE_CALIBRATION_EVENT);
			return;
		}
		delayed_start_time = current_loop_millisecs;
	}

	/* Stop motors */
	stop_motors();

	if (current_loop_millisecs - delayed_start_time >
	    DELAYED_START_WAIT_TIME) {
		delayed_start_time = 0;

		if (FLAG_MAX_VEL_DELAY)
			reset_veldelay();

		reset_encoder_ticks();

		update_state(DELAYED_START_TIMEOUT_EVENT);
	}

	jukebox_setcurrent_song(NO_SONG);

	/* Led on */
	set_led_mode(LED_1, OFF);
	set_led_mode(LED_2, ON);
}

void info_map_state(void)
{
	if (pidvel_map_ms == 0) {
		pidvel_map_ms = current_loop_millisecs;
		push_enable_jukebox();
		enable_jukebox();
	}

	/* stop motors */
	stop_motors();

	set_led_mode(LED_1, ON);

	uint8_t current_pidvel_map = get_current_pidvel_map();

	if (current_pidvel_map == 0) {
		set_led_mode(LED_2, BLINK);
	} else if (current_pidvel_map == 1) {
		set_led_mode(LED_2, DOUBLE_BLINK);
	} else if (current_pidvel_map == 2) {
		set_led_mode(LED_2, TRIPLE_BLINK);
	}

	jukebox_setcurrent_song(get_map_song(current_pidvel_map));

	if (current_loop_millisecs - pidvel_map_ms >
	    AVAILABLE_MAP_CHANGE_TIME) {
		disable_jukebox();
		pull_enable_jukebox();
		pidvel_map_ms = 0;
		update_state(CHANGE_MAP_TIMEOUT_EVENT);
	}
}

void change_map_state(void)
{
	select_next_pidvel_map();
	pidvel_map_ms = current_loop_millisecs;
	update_state(CHANGED_MAP_EVENT);
}

void select_running_state(void)
{
	if (sync_iterations % TIME_BETWEEN_STORE_POS == 0) {
		set_new_reading(line_error);

		if (!SELECT_RUNNING_STATE_USING_ENCODERS) {

			// Check that the minimum number of readings was
			// performed
			if (is_enable_avg_readings()) {
				// Obtain the average number of readings
				if (FORCE_STATECHANGE_ALL_INLINE) {
					check_running_mode_inline();
				} else {
					check_running_mode_thresholds(
					    get_avg_abs_readings());
				}
			}
		} else {

			if (FORCE_STATECHANGE_ALL_INLINE) {
				check_running_mode_inline();
			} else {
				check_running_mode_thresholds(
				    get_abs_diff_encoders());
			}
		}
	}
}

/*
 * @brief change velocity to avoid wheelie at start
 */
void set_vel_antiwheelie(uint32_t current_loop_millisecs)
{
	if ((current_loop_millisecs - running_loop_millisecs) <
	    MAX_ANTIWHEELIE_TIME) {
		set_target_velocity(MAX_ANTIWHEELIE_VELOCITY);
	}
}

void just_run_state()
{

	if (ANTIWHEELIE_AT_START) {
		set_vel_antiwheelie(current_loop_millisecs);
	}

	/* pid control */
	int control = 0;
	control = pid(line_error);

	motor_control(control);

	// Set the current ms (inline)
	if (!is_out_of_line()) {
		if (current_loop_millisecs - last_ms_outline >
		    MIN_TIME_TO_SUCCESS_LINE_RECOVER) {
			last_ms_inline = current_loop_millisecs;
		}
		if (get_running_state() == RUNNING_RECOVERY) {
			if (last_rn_state == RUNNING_NORMAL)
				update_running_state(SET_NORMAL_MODE_STATE);
			else if (last_rn_state == RUNNING_TURBO)
				update_running_state(SET_TURBO_MODE_STATE);
			else if (last_rn_state == RUNNING_BRAKE)
				update_running_state(NEAR_CORNER_EVENT);
			else
				update_running_state(SET_NORMAL_MODE_STATE);
		}
	} else {
		if (get_all_inline()) {
			update_state(ALL_SENSORS_IN_LINE_EVENT);
		} else {
			if (get_running_state() != RUNNING_RECOVERY) {
				last_rn_state = get_running_state();
			}
			last_ms_outline = current_loop_millisecs;
			update_running_state(LOST_LINE_EVENT);
		}
	}
}

void inertia_run_state(void)
{
	if (running_loop_millisecs == 0) {
		running_loop_millisecs = current_loop_millisecs;
	}

	set_target_as_normal();
	motor_control(0);

	if (current_loop_millisecs - running_loop_millisecs >
	    INERTIA_MODE_LIMIT_TIME) {
		update_running_state(INERTIA_TIMEOUT_EVENT);
	}
}

void running_state(void)
{

	if (running_loop_millisecs == 0) {
		running_loop_millisecs = current_loop_millisecs;
		update_running_state(SET_NORMAL_MODE_STATE);
	}

	sync_iterations += 1;

	read_line_sensors(line_sensor_value);
	line_error = get_line_position(line_sensor_value);

	// update consecutive iterations increasing or decreasing error
	update_sequential_readings(line_error, get_last_error());

	// check with encoders or line position if we are in st_line or not
	select_running_state();
	// set running parameters according to running state (music, leds,
	// velocities, ...)
	check_rn_state();

	update_velocities_encoders();

	if (TELEMETRY) {
		print_telemetry(current_loop_millisecs);
	}

	if (FLAG_CIRCUIT_MAPPING) {
		// Add circuit mapping
		update_mapping_function();
	}
}

void setup_keypad(void)
{
	keypad_setup(current_loop_millisecs);
}

void check_command(void)
{
	if (is_command_received()) {
		execute_command();
	}
}

void update_modules(void)
{
	music_update();
	keypad_update();
	keypad_events();
	dma_update();
	leds_update();
}

void set_car_default_parameters(void)
{

	if (!SOFT_CALIBRATION) {
		hard_reset_sensors();
	} else {
		/* reset calibration values (needed for calibration) */
		reset_calibration_values();
	}

	/* reset pid */
	reset_pid();

	/* reset pid for target velocity in normal mode */
	reset_pid_target();

	/* default map of constants of pid */
	reset_pids_normal();

	/* reset readings for turbo calculation */
	reset_prop_readings();

	/* Setup mapping */
	if (FLAG_CIRCUIT_MAPPING) {
		reset_circuit_mapping();
	}

	if (FLAG_MAX_VEL_DELAY) {
		reset_veldelay();
	}
}

void setup_modules()
{
	/* led: setting async period */
	set_led_blink_period(LED_1, LED_BLINK_PERIOD_DEFAULT);
	set_led_blink_period(LED_2, LED_BLINK_PERIOD_DEFAULT);

	/* enable sensors */
	enable_line_sensors();

	/* setup jukebox */
	jukebox_setup();

	/* Setup keypad */
	setup_keypad();
}

void execute_state(state_e state)
{
	switch (state) {
	case RUNNING_STATE:
		running_state();
		break;
	case CALIBRATION_STATE:
		calibration_state();
		break;
	case IDLE_STATE:
		idle_state();
		break;
	case OUT_OF_BATTERY_STATE:
		out_of_battery_state();
		break;
	case DELAYED_START_STATE:
		delayed_start_state();
		break;
	case CHANGE_MAP_STATE:
		change_map_state();
		break;
	case INFO_MAP_STATE:
		info_map_state();
		break;
	case DELAYED_START_INERTIA_STATE:
		delayed_start_inertia_state();
		break;
	case INERTIA_STATE:
		inertia_run_state();
		break;
	default:
		running_state();
	}
}

void check_battery(void)
{
	if (current_loop_millisecs % VBATT_TIME_BETWEEN_READS == 0) {
		if (is_vbatt_drained()) {
			update_state(OUT_OF_BATTERY_EVENT);
		}
	}
}

/*
 * @brief Initial setup and main loop
 */
int main(void)
{
	uint32_t last_loop_millisecs = 0;

	setup_microcontroller();
	set_car_default_parameters();
	setup_modules();

	config_vars();

	while (true) {
		current_loop_millisecs = get_millisecs_since_start();

		check_command();
		check_battery();
		update_modules();

		// inner loop is executed at a fixed period of time
		if ((current_loop_millisecs - last_loop_millisecs) >=
		    LOOP_PERIOD) {
			last_loop_millisecs = current_loop_millisecs;
			execute_state(get_state());
		}
	}

	return 0;
}
