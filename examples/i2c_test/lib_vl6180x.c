#include "lib_i2c.h"
#include "lib_vl6180x.h"

// There is also a VL618X library in
// https://www.st.com/en/embedded-software/stsw-img003.html

const uint8_t VL6180_ADDRESS = 0x29;

// Def result 0xB4
const uint16_t VL6180_IDENTIFICATION__MODEL_ID = 0x000;
// Def result 0x1
const uint16_t VL6180_IDENTIFICATION__MODEL_REV_MAJOR = 0x001;
// Def result 0x3
const uint16_t VL6180_IDENTIFICATION__MODEL_REV_MINOR = 0x002;
// Def result 0x01
const uint16_t VL6180_IDENTIFICATION__MODULE_REV_MAJOR = 0x003;
// Def result 0x02
const uint16_t VL6180_IDENTIFICATION__MODULE_REV_MINOR = 0x004;
const uint16_t VL6180_IDENTIFICATION__DATE_HI = 0x006;
const uint16_t VL6180_IDENTIFICATION__DATE_LO = 0x007;
const uint16_t VL6180_IDENTIFICATION__TIME = 0x008; // and 0x009
// this GPIO can be configured as a interrupt output
const uint16_t VL6180_SYSTEM__MODE_GPIO0 = 0x010;
// this GPIO can be configured as a interrupt output
const uint16_t VL6180_SYSTEM__MODE_GPIO1 = 0x011;
// configure and clear samples buffer
const uint16_t VL6180_SYSTEM__HISTORY_CTRL = 0x012;
const uint16_t VL6180_SYSTEM__INTERRUPT_CONFIG_GPIO = 0x014;
const uint16_t VL6180_SYSTEM__INTERRUPT_CLEAR = 0x015;
// Fresh out of reset bit, default of 1, user can set this to 0 after initial
// boot and can therefore use this to check for a reset condition
const uint16_t VL6180_SYSTEM__FRESH_OUT_OF_RESET = 0x016;
const uint16_t VL6180_SYSTEM__GROUPED_PARAMETER_HOLD = 0x017;
// Configure and trigger the ranging operation
const uint16_t VL6180_SYSRANGE__START = 0x018;
// High Threshold value for ranging comparison. Range 0-255mm.
const uint16_t VL6180_SYSRANGE__THRESH_HIGH = 0x019;
// Low Threshold value for ranging comparison. Range 0-255mm.
const uint16_t VL6180_SYSRANGE__THRESH_LOW = 0x01A;
// Time delay between measurements in Ranging continuous mode. Range 0-254
// (0 = 10ms). Step size = 10ms.
const uint16_t VL6180_SYSRANGE__INTERMEASUREMENT_PERIOD = 0x01B;
// Maximum time to run measurement in Ranging modes. Range 1 - 63 ms
// (1 code = 1 ms); Measurement aborted when limit reached to aid power
// reduction. For example, 0x01 = 1ms, 0x0a = 10ms.
const uint16_t VL6180_SYSRANGE__MAX_CONVERGENCE_TIME = 0x01C;
// User-controlled crosstalk compensation in Mcps (9.7 format).
const uint16_t VL6180_SYSRANGE__CROSSTALK_COMPENSATION_RATE = 0x01E;
// Minimum range value in mm to qualify for cross-talk compensation.
const uint16_t VL6180_SYSRANGE__CROSSTALK_VALID_HEIGHT = 0x021;
// FW carries out an estimate of convergence rate 0.5ms into each new range
// measurement. If convergence rate is below user input value, the operation
// aborts to save power.
// Note: This register must be configured otherwise ECE should be disabled via
// SYSRANGE__RANGE_CHECK_ENABLES.
const uint16_t VL6180_SYSRANGE__EARLY_CONVERGENCE_ESTIMATE = 0x022;
// 2s complement format.
const uint16_t VL6180_SYSRANGE__PART_TO_PART_RANGE_OFFSET = 0x024;
// Range below which ignore threshold is applied. Aim is to ignore the cover
// glass i.e. low signal rate at near distance. Should not be applied to low
// reflectance target at far distance. Range in mm.
// Note: It is recommended to set this register to 255 if the range ignore
// feature is used.
const uint16_t VL6180_SYSRANGE__RANGE_IGNORE_VALID_HEIGHT = 0x025;
// User configurable min threshold signal return rate. Used to filter out
// ranging due to cover glass when there is no target above the device. Mcps 9.7
// format.
// Note: Register must be initialized if this feature is used.
const uint16_t VL6180_SYSRANGE__RANGE_IGNORE_THRESHOLD = 0x026;
// User input value to multiply return_signal_count for AMB:signal ratio check.
// If (amb counts * 6) > return_signal_count * mult then abandon
// measurement due to high ambient (4.4 format).
const uint16_t VL6180_SYSRANGE__MAX_AMBIENT_LEVEL_MULT = 0x02C;
// Enables the range checks
const uint16_t VL6180_SYSRANGE__RANGE_CHECK_ENABLES = 0x02D;
// Force VHV recalibration, recalibration status
const uint16_t VL6180_SYSRANGE__VHV_RECALIBRATE = 0x02E;
// User entered repeat rate of auto VHV task (0 = off, 255 = after
// every 255 measurements)
const uint16_t VL6180_SYSRANGE__VHV_REPEAT_RATE = 0x031;
// Bit 1 sets operation mode. Start/Stop trigger based on current mode and
// system configuration of device_ready. FW clears register automatically.
// Setting bit 0 to 1 in single-shot mode starts a single measurement.
// Setting bit 0 to 1 in continuous mode will either start continuous
// operation (if stopped) or halt continuous operation (if started).
// Bit 0 is auto-cleared in both modes of operation.
const uint16_t VL6180_SYSALS__START = 0x038;
// High Threshold value for ALS comparison. Range 0-65535 codes.
const uint16_t VL6180_SYSALS__THRESH_HIGH = 0x03A;
// Low Threshold value for ALS comparison. Range 0-65535 codes.
const uint16_t VL6180_SYSALS__THRESH_LOW = 0x03C;
// Time delay between measurements in ALS continuous
// mode. Range 0-254 (0 = 10ms). Step size = 10ms.
const uint16_t VL6180_SYSALS__INTERMEASUREMENT_PERIOD = 0x03E;
// ALS analogue gain (light channel)
// 0: ALS Gain = 20
// 1: ALS Gain = 10
// 2: ALS Gain = 5.0
// 3: ALS Gain = 2.5
// 4: ALS Gain = 1.67
// 5: ALS Gain = 1.25
// 6: ALS Gain = 1.0
// 7: ALS Gain = 40
// Controls the “light” channel gain.
// Note: Upper nibble should be set to 0x4 i.e. For ALS gain of 1.0 write 0x46.
const uint16_t VL6180_SYSALS__ANALOGUE_GAIN = 0x03F;
// Integration period for ALS mode. 1 code = 1 ms (0 = 1 ms).
// Recommended setting is 100 ms (0x63).
const uint16_t VL6180_SYSALS__INTEGRATION_PERIOD = 0x040;
// bits [7:4] result__range_error_code: Specific error codes
// 0000: No error
// 0001: VCSEL Continuity Test
// 0010: VCSEL Watchdog Test
// 0011: VCSEL Watchdog
// 0100: PLL1 Lock
// 0101: PLL2 Lock
// 0110: Early Convergence Estimate
// 0111: Max Convergence
// 1000: No Target Ignore
// 1001: Not used
// 1010: Not used
// 1011: Max Signal To Noise Ratio
// 1100: Raw Ranging Algo Underflow
// 1101: Raw Ranging Algo Overflow
// 1110: Ranging Algo Underflow
// 1111: Ranging Algo Overflow
// bit[0] result__range_device_ready: Device Ready. When set to 1, indicates the
// device mode and configuration can be changed and a new start command will be
// accepted. When 0, indicates the device is busy.
const uint16_t VL6180_RESULT__RANGE_STATUS = 0x04D;
// bit[7:4] result__als_error_code: Specific error and debug codes
// 0000: No error
// 0001: Overflow error
// 0002: Underflow error
// bit[0] result__als_device_ready: Device Ready. When set to 1, indicates the
// device mode and configuration can be changed and a new start command will be
// accepted. When 0, indicates the device is busy.
const uint16_t VL6180_RESULT__ALS_STATUS = 0x04E;
// bit[7:6] result_int_error_gpio: Interrupt bits for Error:
// 0: No error reported
// 1: Laser Safety Error
// 2: PLL error (either PLL1 or PLL2)
// bit[5:3] result_int_als_gpio: Interrupt bits for ALS:
// 0: No threshold events reported
// 1: Level Low threshold event
// 2: Level High threshold event
// 3: Out Of Window threshold event
// 4: New Sample Ready threshold event
// bit[2:0] result_int_range_gpio: Interrupt bits for Range:
// 0: No threshold events reported
// 1: Level Low threshold event
// 2: Level High threshold event
// 3: Out Of Window threshold event
// 4: New Sample Ready threshold event
const uint16_t VL6180_RESULT__INTERRUPT_STATUS_GPIO = 0x04F;
// bit[15:0]result__als_ambient_light: 16 Bit ALS count output value. Lux value
// depends on Gain and integration settings and calibrated lux/count setting.
const uint16_t VL6180_RESULT__ALS_VAL = 0x050;
// 8 16bit registers with latest values with the following format:
// Range mode; Bits[15:8] range_val t0; Bits[7:0] range_val_t0-1 (so we have 16 values)
// ALS mode; Bits[15:0] als_val_latest
const uint16_t VL6180_RESULT__HISTORY_BUFFER_0 = 0x052; //0x052:0x060
// Final range result value presented to the user for use. Unit is in mm.
const uint16_t VL6180_RESULT__RANGE_VAL = 0x062;
// Raw Range result value with offset applied (no cross-talk compensation
// applied). Unit is in mm.
const uint16_t VL6180_RESULT__RANGE_RAW = 0x064;
// sensor count rate of signal returns correlated to IR emitter.
// Computed from RETURN_SIGNAL_COUNT / RETURN_CONV_TIME. Mcps 9.7 format
const uint16_t VL6180_RESULT__RANGE_RETURN_RATE = 0x066;
// sensor count rate of reference signal returns. Computed from
// REFERENCE_SIGNAL_COUNT / RETURN_CONV_TIME. Mcps 9.7 format
// Note: Both arrays converge at the same time, so using the return array
// convergence time is correct.
const uint16_t VL6180_RESULT__RANGE_REFERENCE_RATE = 0x068;
// sensor count output value attributed to signal correlated to
// IR emitter on the Return array.
const uint16_t VL6180_RESULT__RANGE_RETURN_SIGNAL_COUNT = 0x06C;
// sensor count output value attributed to signal
// correlated to IR emitter on the Reference array.
const uint16_t VL6180_RESULT__RANGE_REFERENCE_SIGNAL_COUNT = 0x070;
// sensor count output value attributed to uncorrelated ambient signal on the
// Return array. Must be multiplied by 6 if used to calculate the ambient to
// signal threshold.
const uint16_t VL6180_RESULT__RANGE_RETURN_AMB_COUNT = 0x074;
// sensor count output value attributed to uncorrelated
// ambient signal on the Reference array.
const uint16_t VL6180_RESULT__RANGE_REFERENCE_AMB_COUNT = 0x078;
// sensor count output value attributed to signal on the Return array.
const uint16_t VL6180_RESULT__RANGE_RETURN_CONV_TIME = 0x07C;
// sensor count output value attributed to signal on the Reference array.
const uint16_t VL6180_RESULT__RANGE_REFERENCE_CONV_TIME = 0x080;
// The internal readout averaging sample period can be adjusted from 0 to 255.
// Increasing the sampling period decreases noise but also reduces the
// effective max convergence time and increases power consumption:
// Effective max convergence time = max convergence time - readout averaging
// period (see DocID026171 Rev 7 Section 2.7.1: Range timing). Each unit sample
// period corresponds to around 64.5 μs additional processing time. The
// recommended setting is 48 which equates to around 4.3 ms.
const uint16_t VL6180_READOUT__AVERAGING_SAMPLE_PERIOD = 0x10A;
// FW must set bit once initial boot has been completed.
const uint16_t VL6180_FIRMWARE__BOOTUP = 0x119;
// Bits [3:0] analogue gain 1 to 16x
const uint16_t VL6180_FIRMWARE__RESULT_SCALER = 0x120;
// User programmable I 2 C address (7-bit). Device address can be re-designated
// after power-up.
const uint16_t VL6180_I2C_SLAVE__DEVICE_ADDRESS = 0x212;
// Write 0x1 to this register to select ALS+Range interleaved mode.
// Use SYSALS__START and SYSALS__INTERMEASUREMENT_PERIOD to control this mode.
// A range measurement is automatically performed immediately after each ALS
// measurement.
const uint16_t VL6180_INTERLEAVED_MODE__ENABLE = 0x2A3;

void VL6180X_write_recommneded_registers() {
	// https://www.pololu.com/file/0J962/AN4545.pdf
	// Below are the recommended settings required to be loaded onto the VL6180X during the
	// initialisation of the device (see Section 1.3).
	// Mandatory : private registers
	lib_i2c_write_byte(VL6180_ADDRESS, 0x0207, 0x01);
	lib_i2c_write_byte(VL6180_ADDRESS, 0x0208, 0x01);
	lib_i2c_write_byte(VL6180_ADDRESS, 0x0096, 0x00);
	lib_i2c_write_byte(VL6180_ADDRESS, 0x0097, 0xfd);
	lib_i2c_write_byte(VL6180_ADDRESS, 0x00e3, 0x00);
	lib_i2c_write_byte(VL6180_ADDRESS, 0x00e4, 0x04);
	lib_i2c_write_byte(VL6180_ADDRESS, 0x00e5, 0x02);
	lib_i2c_write_byte(VL6180_ADDRESS, 0x00e6, 0x01);
	lib_i2c_write_byte(VL6180_ADDRESS, 0x00e7, 0x03);
	lib_i2c_write_byte(VL6180_ADDRESS, 0x00f5, 0x02);
	lib_i2c_write_byte(VL6180_ADDRESS, 0x00d9, 0x05);
	lib_i2c_write_byte(VL6180_ADDRESS, 0x00db, 0xce);
	lib_i2c_write_byte(VL6180_ADDRESS, 0x00dc, 0x03);
	lib_i2c_write_byte(VL6180_ADDRESS, 0x00dd, 0xf8);
	lib_i2c_write_byte(VL6180_ADDRESS, 0x009f, 0x00);
	lib_i2c_write_byte(VL6180_ADDRESS, 0x00a3, 0x3c);
	lib_i2c_write_byte(VL6180_ADDRESS, 0x00b7, 0x00);
	lib_i2c_write_byte(VL6180_ADDRESS, 0x00bb, 0x3c);
	lib_i2c_write_byte(VL6180_ADDRESS, 0x00b2, 0x09);
	lib_i2c_write_byte(VL6180_ADDRESS, 0x00ca, 0x09);
	lib_i2c_write_byte(VL6180_ADDRESS, 0x0198, 0x01);
	lib_i2c_write_byte(VL6180_ADDRESS, 0x01b0, 0x17);
	lib_i2c_write_byte(VL6180_ADDRESS, 0x01ad, 0x00);
	lib_i2c_write_byte(VL6180_ADDRESS, 0x00ff, 0x05);
	lib_i2c_write_byte(VL6180_ADDRESS, 0x0100, 0x05);
	lib_i2c_write_byte(VL6180_ADDRESS, 0x0199, 0x05);
	lib_i2c_write_byte(VL6180_ADDRESS, 0x01a6, 0x1b);
	lib_i2c_write_byte(VL6180_ADDRESS, 0x01ac, 0x3e);
	lib_i2c_write_byte(VL6180_ADDRESS, 0x01a7, 0x1f);
	lib_i2c_write_byte(VL6180_ADDRESS, 0x0030, 0x00);

	// Recommended : Public registers - See data sheet for more detail

	// Enables polling for ‘New Sample ready’ when measurement completes
	lib_i2c_write_byte(VL6180_ADDRESS, VL6180_SYSTEM__MODE_GPIO1, 0x10);
	// Set the averaging sample period (compromise between lower noise and
	// increased execution time)
	lib_i2c_write_byte(VL6180_ADDRESS, VL6180_READOUT__AVERAGING_SAMPLE_PERIOD,
			0x30);
	// Sets the light and dark gain (upper nibble). Dark gain should not be
	// changed.
	lib_i2c_write_byte(VL6180_ADDRESS, VL6180_SYSALS__ANALOGUE_GAIN, 0x46);
	// sets the # of range measurements after which auto calibration of system
	// is performed
	lib_i2c_write_byte(VL6180_ADDRESS, VL6180_SYSRANGE__VHV_REPEAT_RATE, 0xFF);
	// Set ALS integration time to 100ms
	lib_i2c_write_byte(VL6180_ADDRESS, VL6180_SYSALS__INTEGRATION_PERIOD, 0x63);
	// perform a single temperature calibration of the ranging sensor
	lib_i2c_write_byte(VL6180_ADDRESS, VL6180_SYSRANGE__VHV_RECALIBRATE, 0x01);

	//Optional: Public registers - See data sheet for more detail

	// Set default ranging inter-measurement period to 100ms
	lib_i2c_write_byte(VL6180_ADDRESS, VL6180_SYSRANGE__INTERMEASUREMENT_PERIOD,
			0x09);
	// Set default ALS inter-measurement period to 500ms
	lib_i2c_write_byte(VL6180_ADDRESS, VL6180_SYSALS__INTERMEASUREMENT_PERIOD,
			0x31);
	// Configures interrupt on ‘New Sample Ready threshold event’
	lib_i2c_write_byte(VL6180_ADDRESS, VL6180_SYSTEM__INTERRUPT_CONFIG_GPIO, 0x24);
}

// start single range measurement
void VL6180X_start_single_range() {
	lib_i2c_write_byte(VL6180_ADDRESS, VL6180_SYSRANGE__START, 0x01);
}

// start continuous range measurement
void VL6180X_start_continuous_range() {
	lib_i2c_write_byte(VL6180_ADDRESS, VL6180_SYSRANGE__START, 0x03);
}

// poll the VL6180X till new sample ready
void VL6180X_poll_range() {
	uint8_t status;
	uint8_t range_status;

	// check the status
	status = lib_i2c_read_byte(VL6180_ADDRESS, 0x04f);
	range_status = status & 0x07;

	// wait for new measurement ready status
	while (range_status != 0x04) {
		status = lib_i2c_read_byte(VL6180_ADDRESS, 0x04f);
		range_status = status & 0x07;
		//wait_ms(1); // (can be removed)
	}
}

// read range result
uint8_t VL6180X_read_range() {
	uint8_t range;
	range = lib_i2c_read_byte(VL6180_ADDRESS, 0x062);
	return range;
}

// clear the interrupt on VL6180X
void VL6180X_clear_interrupts() {
	lib_i2c_write_byte(VL6180_ADDRESS, VL6180_SYSTEM__INTERRUPT_CLEAR, 0x07);
}

// Setup of the VL6180X. Needs to be done once before using the device.
uint8_t VL6180X_setup() {
	uint8_t reset = 0;
	reset = lib_i2c_read_byte(VL6180_ADDRESS, VL6180_SYSTEM__FRESH_OUT_OF_RESET);
	if (reset != 1) {
		return -1;
	}

	VL6180X_write_recommneded_registers();
	lib_i2c_write_byte(VL6180_ADDRESS, VL6180_SYSTEM__FRESH_OUT_OF_RESET, 0x00);
	return 1;
}
