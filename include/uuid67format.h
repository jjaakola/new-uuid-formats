#ifndef UUID67_FORMAT_H
#define UUID67_FORMAT_H

#include <stdint.h>

#include "uuid67.h"

void uuid_string(const UUID *uuid_data, char *out);

#ifdef DEBUG
void print_uint8_t(uint8_t num);
void print_octets(UUID *uuid);
#endif

#endif
