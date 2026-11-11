#include "dh.h"

// This file is the open half of the lab. The signatures match include/dh.h.
// Both functions currently return 0 so the project compiles. Replace the body.

uint512_t generate_safe_prime(prng_ctx *ctx, size_t bitsize)
{
    // Requirements from docs/Lab01.pdf, section 2.2:
    //   p is prime, and q = (p - 1) / 2 is prime. Do not call an external big-integer library.
    //   bitsize >= 512 for the program you submit. uint512_t holds at most 512 bits.
    //
    // Use ctx through prng_next_u64. Suggested steps:
    //   1. Draw a random odd q with bitsize - 1 bits. Set the top bit so the width is exact.
    //   2. Test q with Miller-Rabin (several rounds, bases taken from the PRNG).
    //   3. Set p = 2*q + 1 and test p with Miller-Rabin.
    //   4. Repeat until both tests pass.
    // modular_exponentiation is the multiplication step inside Miller-Rabin.
    (void)ctx;
    (void)bitsize;
    return uint512_t(0);
}

uint512_t generate_private_key(prng_ctx *ctx, const uint512_t &p)
{
    // Requirements from docs/Lab01.pdf, section 2.3:
    //   Return a random integer in the closed interval [2, p - 2].
    //
    // Suggested steps:
    //   1. Fill a uint512_t from prng_next_u64.
    //   2. Reject the sample while it is outside [2, p - 2]. Reducing mod (p - 3) and adding 2 also works.
    (void)ctx;
    (void)p;
    return uint512_t(0);
}
