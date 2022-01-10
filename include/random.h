#include <stdint.h>

uint32_t pcg32(void);
uint64_t get_uint64_t(void);
void pcg32_init(); // seed with random
void pcg32_init(uint64_t seed);
