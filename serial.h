#ifndef SERIAL_H
#define SERIAL_H

void serial_init(void);
void serial_write_string(const char *s);
void serial_write_hex(unsigned int val);

#endif
