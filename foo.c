#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "ntp_unix.h"

uint32_t nsec_to_ntp_frac(long nsec)
{

   uint64_t ntp_frac_64 = (uint32_t)(((((uint64_t)(nsec)) << 32) + (NANOSECONDS / 2)) / NANOSECONDS);
   printf("foo: \t %lx\n", ntp_frac_64);
   uint64_t start = (uint64_t)(nsec);
   printf("1: \t %lx\n", start);
   uint64_t step2 = start << 32;
   printf("2: \t %lx\n", step2);
   uint64_t step3 = step2 + (NANOSECONDS / 2);
   printf("3: \t %lx\n", step3);
   uint64_t step4 = (uint64_t)(step3 / NANOSECONDS);
   printf("4: \t %lx\n", step4);
      //(long)(((uint64_t)((nsec << 32)) + (NANOSECONDS / 2) / NANOSECONDS));
   //printf("%lx\n", ntp_frac_long);
   
   return 0;
   //return (uint32_t)(ntp_frac_long);
}


int main()
{
   struct timespec * unix_time;
   if(clock_gettime(CLOCK_REALTIME, unix_time) != 0) {
      perror("Can't get time from clock.");
      exit(EXIT_FAILURE);
   }
   printf("Unix time:\n");
   printf("\tSeconds: \t0x%lx\t(%ld)\n", unix_time->tv_sec, unix_time->tv_sec);
   printf("\tNanoseconds: \t%lx\t(%ld)\n", unix_time->tv_nsec);

   long ntp_sec_long = unix_time->tv_sec + JAN_1970;
   if(ntp_sec_long < 0) {
      fprintf(stderr, "Negative time value.");
      exit(EXIT_FAILURE);
   }
   uint32_t ntp_sec = (uint32_t)(ntp_sec_long);
   printf("NTP time:\n");
   printf("\tSeconds: \t0x%x\t(%d)\n", ntp_sec, ntp_sec);

   printf("half-second: \t%x\n", nsec_to_ntp_frac(500000000));
   printf("quarter-second: \t%x\n", nsec_to_ntp_frac(250000000));




   exit(EXIT_SUCCESS);
}
