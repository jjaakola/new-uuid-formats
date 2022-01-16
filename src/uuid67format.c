#include <stdint.h>
#include <stdio.h>

#include "uuid67format.h"

const char *uuid_fmt_lower =
    "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x";
void uuid_string(const UUID *uuid_data, char *out) {
    snprintf(out, sizeof(char) * 37, uuid_fmt_lower, uuid_data[0], uuid_data[1],
             uuid_data[2], uuid_data[3], uuid_data[4], uuid_data[5],
             uuid_data[6], uuid_data[7], uuid_data[8], uuid_data[9],
             uuid_data[10], uuid_data[11], uuid_data[12], uuid_data[13],
             uuid_data[14], uuid_data[15]);
}

#ifdef DEBUG
void print_uint8_t(uint8_t num) {
    for (uint8_t bit = 0; bit < (sizeof(uint8_t) * 8); bit++) {
        printf("%i", num & 0x01);
        num = num >> 1;
    }
}

void print_octets(UUID *uuid) {
    printf(("OCTET\tUUID\n"));
    for (int i = 0; i < 16; i++) {
        printf("%d\t", i);
        print_uint8_t(uuid[i]);
        printf("\n");
    }
}
#endif
