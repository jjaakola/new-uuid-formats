#include <random.h>

static uint64_t state            = 0x4d595df4d0f33173;
static uint64_t const multiplier = 6364136223846793005u;
static uint64_t const increment  = 1442695040888963407u;

static uint32_t rotr32(uint32_t x, unsigned r)
{
  return x >> r | x << (-r & 31);
}

uint32_t pcg32(void)
{
  uint64_t x = state;
  unsigned count = (unsigned)(x >> 59);

  state = x * multiplier + increment;
  x ^= x >> 18;
  return rotr32((uint32_t)(x >> 27), count);
}

uint64_t get_uint64_t(void)
{
  uint32_t leastSignificantWord = pcg32();
  uint32_t mostSignificantWord = pcg32();
  uint64_t rand = (uint64_t) mostSignificantWord << 32 | leastSignificantWord;
  return rand;
}

void pcg32_init(uint64_t seed)
{
  state = seed + increment;
  (void)pcg32();
}
