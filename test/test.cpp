#include <gtest/gtest.h>
#include <cstdlib>

extern "C" {
   #include "../ntp_unix.h"
}

TEST(NsecToNtpFrac, BasicFractions)
{
   ASSERT_EQ(nsec_to_ntp_frac(500000000), (uint32_t)(1 << 31));
   ASSERT_EQ(nsec_to_ntp_frac(250000000), (1 << 30));
   ASSERT_EQ(nsec_to_ntp_frac(125000000), (1 << 29));
   ASSERT_EQ(nsec_to_ntp_frac(0), 0);
}

TEST(UnixToNtp, Basic)
{
   ASSERT_EQ(unix_to_ntp(0), (uint32_t)(JAN_1970));
}
