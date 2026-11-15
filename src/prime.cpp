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


static size_t bit_length(const uint512_t &x)
{
    for (int i = 7; i >= 0; i--)
    {
        if (x.digits[i] != 0)
            return (size_t)i * 64 + (64 - (size_t)__builtin_clzll(x.digits[i]));
    }
    return 0;
}

static uint512_t random_bits(prng_ctx *ctx, size_t bits)
{
    uint512_t r;
    for (size_t i = 0; i < 8; i++)
    {
        size_t low = i * 64; 
        if (low >= bits)
            break; 
        uint64_t limb = prng_next_u64(ctx);
        size_t need = bits - low;
        if (need < 64)
            limb &= (((uint64_t)1 << need) - 1);
        r.digits[i] = limb;
    }
    return r;
}

uint512_t generate_private_key(prng_ctx *ctx, const uint512_t &p)
{
    uint512_t two;
    two.digits[0] = 2;
    uint512_t five;
    five.digits[0] = 5;

    if (p < five)
        return uint512_t();

    const uint512_t hi = p - two;
    const size_t bits = bit_length(p);

    while (true)
    {
        uint512_t k = random_bits(ctx, bits);
        if (k >= two && k <= hi)
            return k;
    }
}
