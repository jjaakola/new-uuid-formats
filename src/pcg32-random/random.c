#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include <random.h>

static uint64_t state            = 0x4d595df4d0f33173;
static uint64_t const multiplier = 6364136223846793005u;
static uint64_t const increment  = 1442695040888963407u;
volatile int initialized = 0;

void pcg32_init(uint64_t seed)
{
  state = seed + increment;
}

static uint32_t rotr32(uint32_t x, unsigned r)
{
  return x >> r | x << (-r & 31);
}

int random32(void)
{
  if (!initialized) {
    pcg32_init(time(NULL));
  }
  uint64_t x = state;
  unsigned count = (unsigned)(x >> 59);

  state = x * multiplier + increment;
  x ^= x >> 18;
  return rotr32((uint32_t)(x >> 27), count);
}

int random64(uint64_t *rand)
{
  uint32_t leastSignificantWord = random32();
  uint32_t mostSignificantWord = random32();
  *rand = (uint64_t) mostSignificantWord << 32 | leastSignificantWord;
  return 0;
}
