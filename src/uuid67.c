#define _GNU_SOURCE

#include <endian.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <arpa/inet.h>

#include <random.h>
#include <uuid67.h>
#include <uuid67format.h>

/**
 * 100-ns intervals from Gregorian epoch to Unix epoch.
 */
int64_t FROM_GREGORIAN_EPOCH_TO_UNIX_EPOCH = 122192928000000000;

int v6_sequence_counter = 0;
uint64_t last_v6_timestamp;

int v7_sequence_counter = 0;
uint64_t last_v7_timestamp;

uint64_t get_time_in_nanoseconds(struct timespec *ts)
{
  if (clock_gettime(CLOCK_REALTIME, ts)) {
    return 0;
  }
  return (uint64_t)(ts->tv_sec) * (uint64_t)1000000000 + (uint64_t)(ts->tv_nsec);
}

int uuid6(UUID *uuid)
{
  struct timespec *ts = malloc(sizeof(struct timespec));
  if (ts == NULL) {
    uuid = NULL;
    return 1;
  }

  uint64_t nanoseconds = get_time_in_nanoseconds(ts);
  if (nanoseconds == 0) {
    return 1;
  }
  free(ts);

  uint64_t timestamp = (nanoseconds / 100);
  timestamp = timestamp + FROM_GREGORIAN_EPOCH_TO_UNIX_EPOCH;
  /* Drop four top bits to get 60 bits and add version 6 */
  timestamp = ((timestamp << 4) & 0xFFFFFFFFFFFF0000) | (timestamp & 0x0FFF) | 0x6000;

  if (nanoseconds != 0 && nanoseconds <= last_v6_timestamp) {
    v6_sequence_counter++;
  } else {
    v6_sequence_counter = 0;
  }
  last_v6_timestamp = nanoseconds;

  /* time_high */
  uuid[0] = timestamp >> 56;
  uuid[1] = timestamp >> 48;
  uuid[2] = timestamp >> 40;
  uuid[3] = timestamp >> 32;

  /* time_mid */
  uuid[4] = timestamp >> 24;
  uuid[5] = timestamp >> 16;

  /* time_low_and_version */
  uuid[6] = 0xFF & timestamp >> 8;
  uuid[7] = timestamp;

  /* clk_seq_hi_res and variant 10 */
  uint8_t clock_seq_hi = (v6_sequence_counter >> 8) & 0x3F;
  uuid[8] = clock_seq_hi | 1 << 7;

  /* clk_seq_low */
  uuid[9] = v6_sequence_counter;

  /* node, random */
  uint64_t random_bits = pcg64();
  uuid[10] = random_bits;
  uuid[11] = random_bits >> 8;
  uuid[12] = random_bits >> 16;
  uuid[13] = random_bits >> 24;
  uuid[14] = random_bits >> 32;
  uuid[15] = random_bits >> 40;

#ifdef DEBUG
  print_octets(uuid);
#endif

  return 0;
}

int uuid7(UUID *uuid)
{
  struct timespec *ts = malloc(sizeof(struct timespec));
  if (ts == NULL) {
    return 1;
  }

  uint64_t timestamp = get_time_in_nanoseconds(ts);
  if (timestamp == 0) {
    return 1;
  }
  free(ts);

  /* Timestamp must be in network byte order */
  /* To network byte order */
  timestamp = htobe64(timestamp);

  if (timestamp != 0 && timestamp <= last_v7_timestamp) {
    v7_sequence_counter++;
  } else {
    v7_sequence_counter = 0;
  }
  last_v7_timestamp = timestamp;

  /* unixts */
  uuid[0] = timestamp;
  uuid[1] = timestamp >> 8;
  uuid[2] = timestamp >> 16;
  uuid[3] = timestamp >> 24;

  /* 36 bits and start of subsec_a */
  uuid[4] = timestamp >> 32;
  uuid[5] = timestamp >> 40;

  /* ver and subsec_b */
  uint16_t ver_subsec_b = (0x7 << 4) | ((timestamp >> 44) & 0x0F);
  uuid[6] = ver_subsec_b;
  uuid[7] = timestamp >> 52;

  /* var and start of subsec_seq_node */
  uuid[8] = 0xA | (v7_sequence_counter >> 8);
  uuid[9] = v7_sequence_counter;

  uint64_t random_bits = pcg64();
  uuid[10] = random_bits;
  uuid[11] = random_bits >> 8;
  uuid[12] = random_bits >> 16;
  uuid[13] = random_bits >> 24;
  uuid[14] = random_bits >> 32;
  uuid[15] = random_bits >> 40;

#ifdef DEBUG
  print_octets(uuid);
#endif

  return 0;
}
