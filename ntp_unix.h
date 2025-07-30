#pragma once

#include <stdint.h>

// #include <stdlib.h>
// #include <stdio.h>
// #include <arpa/inet.h>


#define NANOSECONDS 1000000000l

// factor pulled from NTP reference implementation
#define JAN_1970 0x83aa7e80l

// Can't verify packing/alignment
struct ntp_time {
   uint32_t ntp_sec;
   uint32_t ntp_frac;
};

/*
 * Converts nanosecond portion of Unix time value to 32-bit fraction
 *    of a second for NTP format.  Does not convert endianness.
 */
uint32_t nsec_to_ntp_frac(long);

/*
 * Converts unix time (in seconds) to NTP time (in seconds).  Does not
 *    convert endianness.  Doesn't account for epoch, so don't use
 *    this code after, say, 2030.
 */
uint32_t unix_to_ntp(long);

/*
 * Gets time from clock and returns it in NTP format.  Note that this is not
 *    optimized like proper NTP utilities are, so intrinsic latency will
 *    ensure that the fraction part is not accurate.  The intention is to
 *    get the NTP time to use as a unique value.
 */
uint64_t get_ntp_time();
