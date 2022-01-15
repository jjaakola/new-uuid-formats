#include <stdint.h>
#include <sys/random.h>

int random64(uint64_t *rand)
{
  int res = getrandom(&rand, sizeof(rand), GRND_RANDOM);
  if (res == -1 || res != sizeof(rand)) {
    return 1;
  } else {
    return 0;
  }
}
