#include "utils.h"

#define LIMIT_STR_LEN 1024

int stringlen(char *string) {
	for (int res = 0; res < LIMIT_STR_LEN; res++) {
		if (string[res] == '\0') {
			return res;
		}
	}
	return 0;
}

char welcome[] =
		"\nPress\n 1 for write test.\n 2 read test.\n";

char prompt() {
	for (int i = 0; i < stringlen(welcome); i++) {
		usart_send_blocking(MY_USART, welcome[i]);
	}
	return usart_recv_blocking(MY_USART);
}

int _write(int file, char *ptr, int len) {
	int i;

	if (file == 1) {
		for (i = 0; i < len; i++)
			usart_send_blocking(MY_USART, ptr[i]);
		return i;
	}

	errno = EIO;
	return -1;
}

void usart_send_string(uint8_t *string, uint16_t str_size){
	uint16_t iter = 0;
	do{
		usart_send_blocking(MY_USART, string[iter++]);
	}while(string[iter] != 0 && iter < str_size);
}

void usart_get_string(uint8_t *out_string, uint16_t str_max_size){
	uint8_t sign = 0;
	uint16_t iter = 0;

	while(iter < str_max_size)
	{
		sign = usart_recv_blocking(MY_USART);

		if(sign != '\n' && sign != '\r')
			out_string[iter++] = sign;
		else {
			out_string[iter] = 0;
			usart_send_string((uint8_t*)"\r\n", 3);
			break;
		}
	}
}

void local_ltoa_hex(uint32_t value, uint8_t *out_string){
	uint8_t iter;

	/*end of string*/
	out_string += 8;
	*(out_string--) = 0;

	for(iter=0; iter<8; iter++){
		*(out_string--) = (((value&0xf) > 0x9) ? (0x40 + ((value&0xf) - 0x9)) : (0x30 | (value&0xf)));
		value >>= 4;
	}
}

