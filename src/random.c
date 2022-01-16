#include <stdint.h>

#include "config.h"

#ifdef HAVE_SYSRANDOM_H
#include <sys/random.h>

int random64(uint64_t *rand) {
    int res = getrandom(rand, sizeof(rand), GRND_RANDOM);
    if (res == -1 || res != sizeof(rand)) {
        return 1;
    }
    return 0;
}
#else
#include <stdlib.h>
#include <time.h>

int random64(uint64_t *rand) {
    srandom(time(NULL));
    uint64_t tmp = random();
    *rand = tmp;
    return 0;
}
#endif
