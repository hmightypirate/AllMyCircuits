#include "fsm.h"

static state_e current_state = IDLE_STATE;
static rnstate_e running_state = RUNNING_NORMAL;

void update_running_state(rnevent_e rnevent)
{
	if (rnevent == SET_NORMAL_MODE_STATE) {
		set_running_state(RUNNING_NORMAL);
	} else if (rnevent == SET_TURBO_MODE_STATE) {
		set_running_state(RUNNING_TURBO);
	} else if (rnevent == SET_NOOL_MODE_STATE) {
		set_running_state(RUNNING_NOOL);
	} else if (rnevent == STOP_RUNNING_EVENT) {
		set_running_state(RUNNING_STOP);
	} else if (rnevent == LOST_LINE_EVENT) {
		set_running_state(RUNNING_RECOVERY);
	}
}

void update_state(event_e event)
{
	if (current_state == OUT_OF_BATTERY_STATE)
		return;

	switch (event) {
	case OUT_OF_BATTERY_EVENT:
		current_state = OUT_OF_BATTERY_STATE;
		break;
	case ALL_SENSORS_IN_LINE_EVENT:
		current_state = IDLE_STATE;
		break;
	case DELAYED_START_TIMEOUT_EVENT:
		current_state = RUNNING_STATE;
		break;
	case BUTTON1_PRESSED_EVENT:
		break;
	case BUTTON1_RELEASED_EVENT:
		switch (current_state) {
		case CALIBRATION_STATE:
			current_state = DELAYED_START_STATE;
			break;
		case DELAYED_START_STATE:
		case RUNNING_STATE:
			current_state = IDLE_STATE;
			break;
		case IDLE_STATE:
			current_state = CALIBRATION_STATE;
		default:;
		}
		break;
	case BUTTON2_PRESSED_EVENT:
		if (current_state == CALIBRATION_STATE) {
			current_state = INFO_MAP_STATE;
		} else if (current_state == INFO_MAP_STATE) {
			current_state = CHANGE_MAP_STATE;
		}
		break;
	case BUTTON2_RELEASED_EVENT:
		break;
	case BUTTON3_PRESSED_EVENT:
		if ((current_state == IDLE_STATE) ||
		    (current_state == CALIBRATION_STATE)) {
			toggle_sound();
		}
		break;
	case BUTTON3_RELEASED_EVENT:
		break;
	case FORCE_CALIBRATION_EVENT:
		current_state = CALIBRATION_STATE;
		break;
	case CHANGE_MAP_TIMEOUT_EVENT:
		current_state = CALIBRATION_STATE;
		break;
	case CHANGED_MAP_EVENT:
		current_state = INFO_MAP_STATE;
		break;
	default:
		current_state = IDLE_STATE;
	}
}

/*
 * @brief get current state
 */
state_e get_state()
{
	return current_state;
}

/*
 * @brief set state
 */
void set_state(state_e state)
{
	current_state = state;
}

/*
 * @brief set running state
 */
rnstate_e get_running_state()
{
	return running_state;
}

void set_running_state(rnstate_e state)
{
	running_state = state;
}
