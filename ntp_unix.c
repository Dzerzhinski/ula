#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>     // htonl()

#include "ntp_unix.h"

/*
 * Converts nanosecond portion of Unix time value to 32-bit fraction 
 *    of a second for NTP format.  Does not convert endianness.
 */
uint32_t nsec_to_ntp_frac(long nsec)
{
   // calculation copied from NTP reference implementation
   //    adds NANOSECONDS/2 term to round up least significant 
   //    bit before truncation
   uint32_t ntp_frac_32 = (uint32_t)(((((uint64_t)(nsec)) << 32) + \
            (NANOSECONDS / 2)) / NANOSECONDS);
   return ntp_frac_32;
}

/*
 * Converts unix time (in seconds) to NTP time (in seconds).  Does not 
 *    convert endianness.  Doesn't account for epoch, so don't use 
 *    this code after, say, 2030.
 */
uint32_t unix_to_ntp(long tv_sec)
{
   uint32_t ntp_sec = (uint32_t)(tv_sec + JAN_1970);
   return ntp_sec;
}

/*
 * Helper function for debugging.
 */
void print_ntp_time(uint32_t * ntp)
{
   printf("Ntp time struct:\n");
   printf("\t%x (%d)\n", ntp[0], ntp[0]);
   printf("\t%x (%d)\n", ntp[1], ntp[1]);
}

/*
 * Gets time from clock and returns it in NTP format.  Note that this is not 
 *    optimized like proper NTP utilities are, so intrinsic latency will 
 *    ensure that the fraction part is not accurate.  The intention is to 
 *    get the NTP time to use as a unique value.
 */
uint64_t get_ntp_time()
{
   struct timespec unix_time;
   struct ntp_time time;
   uint64_t out_time;

   if(clock_gettime(CLOCK_REALTIME, &unix_time) != 0) {
      perror("Can't get time from clock.");
      return 0;
   }
   time.ntp_sec = htonl(unix_to_ntp(unix_time.tv_sec));
   time.ntp_frac = htonl(nsec_to_ntp_frac(unix_time.tv_nsec));

   // print_ntp_time((uint32_t*)(&time));
   
   // in case struct is padded out, copy straight to uint64_t
   uint32_t * time_ptr = (uint32_t *)(&out_time);
   time_ptr = mempcpy(time_ptr, &(time.ntp_sec), 4);
   time_ptr = memcpy(time_ptr, &(time.ntp_frac), 4);

   return out_time;
}


