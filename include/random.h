#ifndef UUID67_RANDOM_H
#define UUID67_RANDOM_H

#include <stdint.h>

uint32_t pcg32(void);
uint64_t pcg64(void);
void pcg32_init(); // seed with random
void pcg32_init(uint64_t seed);

#endif
