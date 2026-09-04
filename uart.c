#define UART0 ((volatile unsigned int *)0x09000000)

void uart_putc(char c)
{
    *UART0 = c;
}

void uart_puts(const char *s)
{
    while(*s)
    {
        uart_putc(*s++);
    }
}
