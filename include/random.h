// SPDX-License-Identifier: MIT
#ifndef UUID67_RANDOM_H
#define UUID67_RANDOM_H

#include <stdint.h>

/*
 * Return 0 for successs, 1 for error.
 */
int random64(uint64_t *rand);

#endif
