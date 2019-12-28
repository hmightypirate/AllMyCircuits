#include "cli.h"

#define MAX_BUFFER 10

char input_buffer[MAX_BUFFER];
char command_line[MAX_BUFFER];
uint8_t buffer_str_size = 0;
uint8_t command_line_size = 0;
bool command_received = false;

uint8_t last_measure = 0;


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



uint8_t get_value_data()
{
  return last_measure;
}

/*
  @brief interruptions function of usart_1

 */
void usart1_isr(void)
{
  if (((USART_CR1(USART1) & USART_CR1_RXNEIE) != 0)
       && ((USART_SR(USART1) & USART_SR_RXNE) != 0)) {

    // FIXME: usart_recv returns an uint16_t

        
    char data = usart_recv(USART1);
    
    if ((data != '\n') && (data != '\r')) {

      input_buffer[buffer_str_size++] = data;

      if (buffer_str_size >= MAX_BUFFER) {
	clear_input_buffer();
      }
      
    } else {
      if (buffer_str_size >= 1) {
	copy_buffer_to_command();

	data = command_line[0];

	// 48 is the value of 0 in ascii
	last_measure = command_line[0] - 48;

      // FIXME For some reason atoi and strtol do not work
	//last_measure = atoi(command_line); //(uint8_t) strtol(command_line, NULL, 10);

      if (last_measure > 10)
	{
	  last_measure = 10;
	}
      
      }
      clear_input_buffer();

    }
     
      
      /*
      if (data == '0')
	{
	  last_measure = 0;
	}
      else if (data == '1')
	{
	  last_measure = 1;
	}
      else if (data == '2')
	{
	  last_measure = 2;
	}
      else if (data == '3')
	{
	  last_measure = 3;
	}
      else if (data == '4')
	{
	  last_measure = 4;
	}
      else if (data == '5')
	{
	  last_measure = 5;
	}
      else
	{
	  last_measure = 10;
	}


      */




  }
}
