#include "sensors.h"

uint16_t black_sensors[NUM_SENSORS];
uint16_t white_sensors[NUM_SENSORS];
uint16_t callibrated_sensors[NUM_SENSORS];
uint16_t threshold[NUM_SENSORS];
uint8_t last_drift = LEFT_DRIFT;

static int out_of_line = 0;

/* This var stores the number of sensors correctly callibrated */
static uint8_t sensors_callibrated_count = 0;

static int started_callibration = 0;

/*
 * @brief reads a line sensor 
 * 
 * @param[in] channel specifies the channel to read
 * 
 */
static uint16_t read_adc_naiive(uint8_t channel)
{
  uint8_t channel_array[16];
  channel_array[0] = channel;
  adc_set_regular_sequence(SENSOR_ADC, 1, channel_array);
  adc_start_conversion_direct(SENSOR_ADC);
  while (!adc_eoc(SENSOR_ADC));
  uint16_t reg16 = adc_read_regular(SENSOR_ADC);
  return reg16;
}

/*
 * @brief set manually black and white thresholds to sensors
 * 
 * @note it is more advisable to callibrate the sensors rathen 
 *        that using this function
 */
void hard_reset_sensors()
{
  for (int i=0; i< NUM_SENSORS; i++)
    {
      black_sensors[i] = BLACK_MEASURE;
      white_sensors[i] = WHITE_MEASURE;
      threshold[i] = (black_sensors[i] - white_sensors[i])/2;
    }
}

/*
 * @resets callibration values
 *
 * @note called automatically only once during callibration
 */

void reset_callibration_values()
{
  for (int i=0; i< NUM_SENSORS; i++)
    {
      black_sensors[i] = WHITE_MEASURE;
      white_sensors[i] = BLACK_MEASURE;
    }
  started_callibration = 1;
}

/*
 * @brief obtain one measure of all the sensors
 * 
 * @param[in] sensor_value array where to store the sensor measures
 */
void read_line_sensors(uint16_t* sensor_value)
{  
  for (int i=0; i < NUM_SENSORS; i++)
    {
      sensor_value[i] = read_adc_naiive(i);
    }
}

uint16_t trunc_to_range(uint16_t value, uint16_t min, uint16_t max)
{
  uint16_t trunc_value = value;

  if (value < min) {
    trunc_value = min;
  } else if (value > max) {
    trunc_value = max;
  }

  return trunc_value;
}

/*
 * @brief rescale vale between min and max using scale steps
 */
uint16_t rescale_in_range(uint16_t value, uint16_t min, uint16_t max, uint16_t scale)
{
  return ((value - min) * (scale / (max - min)));
}

int get_last_known_position()
{
  int pos;

  if (last_drift == LEFT_DRIFT) {
    pos = 1*100;
  } else {
    pos = NUM_SENSORS * 100;
  }

  return pos;
}

void set_last_known_position(int pos)
{
  if (pos < ((NUM_SENSORS + 1) * 100/2)) 
    last_drift = LEFT_DRIFT;
  else 
    last_drift = RIGHT_DRIFT;
}

/*
 * @brief transform sensor measures in line position
 *
 * @param[in] value sensor measures
 *
 * @note set the flag FOLLOW_BLACK_LINE to 0 to follow a white line
 *
 */
int get_line_position(uint16_t* value)
{
  uint32_t line_value[NUM_SENSORS];
  uint8_t whites_detected = 0;
  uint8_t blacks_detected = 0;
  int pos = 0;
  int avg_sensors = 0;
  int sum_sensors = 0;

  out_of_line = 0;
  
  for (int i = 0; i < NUM_SENSORS; i++) {
    value[i] = trunc_to_range(value[i], white_sensors[i], black_sensors[i]);
    line_value[i] = rescale_in_range(value[i], white_sensors[i], black_sensors[i], K_SENSOR);
    
    avg_sensors += (uint32_t)line_value[i]*(i+1)*100;
    sum_sensors += line_value[i];

    //Check whites/blacks detected
    if (value[i] > threshold[i]) blacks_detected += 1;
    if (value[i] < threshold[i]) whites_detected += 1;
  }

  if ((blacks_detected == 0 && FOLLOW_BLACK_LINE) ||
      (whites_detected == 0 && !FOLLOW_BLACK_LINE)) {
      
      /* Out of line */
      out_of_line = 1;
      
      pos = get_last_known_position();

  } else {
      
    pos = avg_sensors/sum_sensors;

    if (!FOLLOW_BLACK_LINE) {
        pos = (NUM_SENSORS + 1) * 100 - pos;
    }

    set_last_known_position(pos);

  }

  /* Zero-center position */
  pos = pos - (NUM_SENSORS + 1) * 100/2;

  return pos;
}

/*
 * @brief return the out of line flag
 * 
 */
int is_out_of_line()
{
  return out_of_line;
}

/*
 * @brief callibrate sensors (one step)
 *
 * @param[in] values a variable that holds current measurements
 *
 */
void calibrate_sensors(uint16_t* values)
{

  /* reset callibration first time */
  if (!started_callibration)
    {
      reset_callibration_values();
    }
  
  sensors_callibrated_count = 0;
  
  for (int i=0; i<NUM_SENSORS; i++)
    {
      /* check if current value is higher than previous max value */
      if (values[i] > black_sensors[i])
        {
          black_sensors[i] = values[i];
        }
      /* check if current value is lower than previous min value */
      if (values[i] < white_sensors[i])
        {
           white_sensors[i] = values[i];
        }
      
      threshold[i] = (black_sensors[i] + white_sensors[i])/2;

      if ((black_sensors[i] - white_sensors[i]) > THRESHOLD_CALLIBRATION)
        {
          sensors_callibrated_count++;
          callibrated_sensors[i] = 1;
        }
      else
        {
          callibrated_sensors[i] = 0;
        }
    }     
}

/*
 * @brief enable sensors
 */
void enable_line_sensors()
{
  gpio_set(SENSOR_ON_PORT, SENSOR_ON_PIN);
}

/*
 * @brief disable sensors
 */
void disable_line_sensors()
{
  gpio_clear(SENSOR_ON_PORT, SENSOR_ON_PIN);
}


/*
 * @brief get the number of callibrated sensors
 */
uint8_t get_callibrated_sensors_count()
{
  return sensors_callibrated_count;
}


/*
 * @brief get value of white of each sensor
 */
uint16_t* get_whites() {
  return white_sensors;
}

/*
 * @brief get value of white of each sensor
 */
uint16_t* get_blacks() {
  return black_sensors;
}

/*
 * @brief get value of white of each sensor
 */
uint16_t* get_thresholds() {
  return threshold;
}