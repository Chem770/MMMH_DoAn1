#include "dh.h"

namespace
{

uint512_t shift_right_one(const uint512_t &value)
{
    uint512_t result;
    uint64_t carry = 0;
    for (int i = (int)uint512_t::LIMBS - 1; i >= 0; i--)
    {
        uint64_t next = value.digits[i] & 1ull;
        result.digits[i] = (value.digits[i] >> 1) | (carry << 63);
        carry = next;
    }
    return result;
}

// Odd integer with exactly `bits` bits. Bit 0 and the top bit are set.
uint512_t random_odd(prng_ctx *ctx, size_t bits)
{
    uint512_t value;
    int top = (int)bits - 1;
    int limb = top / 64;
    int offset = top % 64;

    for (int i = 0; i <= limb; i++)
        value.digits[i] = prng_next_u64(ctx);

    if (offset < 63)
        value.digits[limb] &= (1ull << (offset + 1)) - 1ull;
    value.digits[limb] |= 1ull << offset;
    value.digits[0] |= 1ull;
    return value;
}

bool divisible_by_small_prime(const uint512_t &n)
{
    static const unsigned small[] = {
        3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47};
    for (size_t i = 0; i < sizeof(small) / sizeof(small[0]); i++)
    {
        uint512_t prime(small[i]);
        if (n == prime)
            return false;
        if (is_zero(mod_reduce(n, prime)))
            return true;
    }
    return false;
}

// Base in [2, n - 2].
uint512_t random_base(prng_ctx *ctx, const uint512_t &n)
{
    uint512_t sample;
    for (size_t i = 0; i < uint512_t::LIMBS; i++)
        sample.digits[i] = prng_next_u64(ctx);
    return mod_reduce(sample, n - uint512_t(3)) + uint512_t(2);
}

// Miller-Rabin. Eight random bases. A `false` result is composite.
bool miller_rabin(prng_ctx *ctx, const uint512_t &n)
{
    if (n == uint512_t(2) || n == uint512_t(3))
        return true;
    if (n < uint512_t(2) || bit_at(n, 0) == 0)
        return false;
    if (divisible_by_small_prime(n))
        return false;

    uint512_t d = n - uint512_t(1);
    int s = 0;
    while (bit_at(d, 0) == 0)
    {
        d = shift_right_one(d);
        s++;
    }

    uint512_t n_minus_1 = n - uint512_t(1);
    for (int round = 0; round < 8; round++)
    {
        uint512_t x = modular_exponentiation(random_base(ctx, n), d, n);
        if (x == uint512_t(1) || x == n_minus_1)
            continue;

        bool passed = false;
        for (int r = 1; r < s; r++)
        {
            x = mul_mod(x, x, n);
            if (x == n_minus_1)
            {
                passed = true;
                break;
            }
        }
        if (!passed)
            return false;
    }
    return true;
}

} // namespace

uint512_t generate_safe_prime(prng_ctx *ctx, size_t bitsize)
{
    // p and q = (p - 1) / 2 are both prime. p has `bitsize` bits.
    if (ctx == 0 || bitsize < 3 || bitsize > (size_t)uint512_t::BITS)
        return uint512_t(0);

    for (;;)
    {
        uint512_t q = random_odd(ctx, bitsize - 1);
        if (!miller_rabin(ctx, q))
            continue;
        uint512_t p = shift_left_one(q) + uint512_t(1);
        if (miller_rabin(ctx, p))
            return p;
    }
}

uint512_t generate_private_key(prng_ctx *ctx, const uint512_t &p)
{
    // Still open. Return a random integer in [2, p - 2].
    (void)ctx;
    (void)p;
    return uint512_t(0);
}
