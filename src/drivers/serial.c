#include "serial.h"
#include "pic.h"


void print_serial(const char *str) {
    while (*str) outb(0x3f8, *str++);
}

void print_uint(uint64_t v) {
    char buf[32];
    int i = 30;
    buf[31] = 0;

    if (v == 0) {
        print_serial("0");
        return;
    }

    while (v && i) {
        buf[i--] = '0' + (v % 10);
        v /= 10;
    }

    print_serial(&buf[i + 1]);
}
