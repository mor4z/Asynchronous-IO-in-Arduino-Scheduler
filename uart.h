// #define BAUD 19600
#define BAUD 57600
#define MYUBRR F_CPU/16/BAUD-1

void usart_init(int ubrr);
void usart_putchar(char data);
void usart_pstr (char *s);