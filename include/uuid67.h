#ifndef UUID67_H
#define UUID67_H

#include <stdint.h>

typedef uint8_t UUID;

typedef int (*uuid_fptr)(UUID*);

/**
 * Give pointer to allocated 128 bits long memory area.
 * The responsibility of deallocating the reserved memory is at the caller.
 * Returns 0 for succesful generation and 1 for failures.
 */
int uuid6(UUID* uuid);

/**
 * Give pointer to allocated 128 bits long memory area.
 * The responsibility of deallocating the reserved memory is at the caller.
 * Returns 0 for succesful generation and 1 for failures.
 */
int uuid7(UUID* uuid);

#endif
