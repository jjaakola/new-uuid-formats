#define _GNU_SOURCE

#include <endian.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <arpa/inet.h>

#include <uuid67.h>
#include <random.h>

#ifdef DEBUG
# define DEBUG_PRINT(x) printf x

void print_uint8_t(uint8_t num)
{
   for(int bit=0;bit<(sizeof(uint8_t) * 8); bit++)
   {
     DEBUG_PRINT(("%i", num & 0x01));
      num = num >> 1;
   }
}

#else
# define DEBUG_PRINT(x) do {} while (0)
#endif

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
  uuid[6] |= timestamp >> 8;
  uuid[7] = timestamp;

  /* clk_seq_hi_res and variant 10 */
  uint8_t clock_seq_hi = (v6_sequence_counter >> 8) & 0x3F;
  uuid[8] = clock_seq_hi | 1 << 7;

  /* clk_seq_low */
  uuid[9] = v6_sequence_counter;

  /* node, random */
  uint64_t random_bits = get_uint64_t();
  uuid[10] = random_bits;
  uuid[11] = random_bits >> 8;
  uuid[12] = random_bits >> 16;
  uuid[13] = random_bits >> 24;
  uuid[14] = random_bits >> 32;
  uuid[15] = random_bits >> 40;

  return 0;
}

int uuid7(UUID *uuid)
{
  struct timespec *ts = malloc(sizeof(struct timespec));
  if (ts == NULL) {
    return 1;
  }

  /* Timestamp must be in network byte order */
  uint64_t timestamp = get_time_in_nanoseconds(ts);
  if (timestamp == 0) {
    return 1;
  }

  /* To network byte order */
  timestamp = htobe64(timestamp);
  /* Drop four top bits to get 60 bits */
  timestamp = timestamp << 4;

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

  uint64_t random_bits = get_uint64_t();
  uuid[10] = random_bits;
  uuid[11] = random_bits >> 8;
  uuid[12] = random_bits >> 16;
  uuid[13] = random_bits >> 24;
  uuid[14] = random_bits >> 32;
  uuid[15] = random_bits >> 40;

#ifdef DEBUG
  DEBUG_PRINT(("OCTET\tUUID\n"));
  for (int i=0; i < 16; i++) {
    DEBUG_PRINT(("%d\t", i));
    print_uint8_t(uuid[i]);
    DEBUG_PRINT(("\n"));
  }
#endif

  return 0;
}

const char *uuid_fmt_lower = "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x";
void uuid_string(const UUID *uuid_data, char* out)
{
  snprintf(out, sizeof(char) * 37,
	   uuid_fmt_lower,
	   uuid_data[0], uuid_data[1],
	   uuid_data[2], uuid_data[3],
	   uuid_data[4], uuid_data[5],
	   uuid_data[6], uuid_data[7],
	   uuid_data[8], uuid_data[9],
	   uuid_data[10], uuid_data[11],
	   uuid_data[12], uuid_data[13],
	   uuid_data[14], uuid_data[15]);
}


int main(int argc, char** args)
{
  pcg32_init(time(NULL));


  uuid_fptr uuid_func  = uuid6;
  if (argc > 1 && strncmp("7", args[1], sizeof(char)) == 0) {
    uuid_func = uuid7;
  }
  
  for (int i=0; i<10; i++) {
    UUID *uuid_data = malloc(16);
    int rc = uuid_func(uuid_data);
    if (rc == 0) {
      char *uuids = malloc(37);
      uuid_string(uuid_data, uuids);
      printf("%s\n", uuids);
      free(uuids);
    } else {
      fprintf(stderr, "Not good, return code is: %d\n", rc);
    }
    if (uuid_data != NULL) free(uuid_data);
  }
}
