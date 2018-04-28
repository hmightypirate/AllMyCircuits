#include "sensors.h"

uint16_t black_sensors[NUM_SENSORS];
uint16_t white_sensors[NUM_SENSORS];
uint16_t callibrated_sensors[NUM_SENSORS];
uint16_t threshold[NUM_SENSORS];
uint8_t last_drift = LEFT_DRIFT;

static int out_of_line = 0;

/* This var stores the number of sensors correctly callibrated */
static uint8_t sensors_callibrated = 0;

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
  
  for (int i = 0; i < NUM_SENSORS; i++)
    {
      if (value[i] < white_sensors[i])
        {
          line_value[i] = white_sensors[i];
        }
      else if (value[i] > black_sensors[i])
        {
          line_value[i] = black_sensors[i];
        }
      else
        {
          line_value[i] = ((value[i] - white_sensors[i]) *
                           (K_SENSOR / (black_sensors[i] - white_sensors[i])));      
        }

      //Check whites/blacks detected
      if (value[i] > threshold[i])
        {
          blacks_detected += 1;
        }
      else if (value[i] < threshold[i])
        {
          whites_detected += 1;
        }

      avg_sensors += (uint32_t)line_value[i]*(i+1)*100;
      sum_sensors += line_value[i];
    }

  if ((blacks_detected == 0 && FOLLOW_BLACK_LINE) ||
      (whites_detected == 0 && !FOLLOW_BLACK_LINE))
    {
      /* Out of line */
      out_of_line = 1;
      
      if (last_drift == LEFT_DRIFT)
        {
          pos = 1*100;
        }
      else
        {
          pos = NUM_SENSORS * 100;
        }
    }
  else
    {
      pos = avg_sensors/sum_sensors;

      if (!FOLLOW_BLACK_LINE)
        {
          pos = (NUM_SENSORS + 1) * 100 - pos;
        }

      if (pos < ((NUM_SENSORS + 1) * 100/2))
        {
          last_drift = LEFT_DRIFT;
        }
      else if (pos > ((NUM_SENSORS + 1) * 100/2))
        {
          last_drift = RIGHT_DRIFT;
        }
    }

  /* Updating pos */
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
  
  read_line_sensors(values);

  sensors_callibrated = 0;
  
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
          sensors_callibrated++;
          callibrated_sensors[i] = 1;
        }
      else
        {
          callibrated_sensors[i] = 0;
        }
    }

  /* Add extra delay (in nop operations) after every callibration call */
  /*for (int i=0; i < x; i++)
    {
    __asm__("nop");
    }*/
  //DELAY(DELAY_CALLIBRATION_CALLS);
     
}

/*
 * @brief enable sensors
 */
void enable_sensors()
{
  gpio_set(SENSOR_ON_PORT, SENSOR_ON_PIN);
}

/*
 * @brief disable sensors
 */
void disable_sensors()
{
  gpio_clear(SENSOR_ON_PORT, SENSOR_ON_PIN);
}


/*
 * @brief get the number of callibrated sensors
 */
uint8_t get_callibrated_sensors()
{
  return sensors_callibrated;
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