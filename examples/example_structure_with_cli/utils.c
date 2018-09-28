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
		"\nPress\n 1 for test 1.\n 2 for test 2.\n";

char prompt() {
	for (int i = 0; i < stringlen(welcome); i++) {
		usart_send_blocking(USART1, welcome[i]);
	}
	return usart_recv_blocking(USART1);
}

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
