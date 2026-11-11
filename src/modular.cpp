#include "modular.h"

uint512_t mod_reduce(const uint512_t &x, const uint512_t &y)
{
    if (is_zero(y) || x < y)
        return is_zero(y) ? uint512_t(0) : x;

    // Remainder stays below y. One extra carry bit holds the bit that
    // shift_left_one drops, so every stored value stays inside 512 bits.
    uint512_t rem;
    for (int i = uint512_t::BITS - 1; i >= 0; i--)
    {
        int carry = top_bit(rem);
        rem = shift_left_one(rem);
        if (bit_at(x, i))
            rem = rem + uint512_t(1);

        // 2*old + bit is less than 2*y, so one subtraction brings it below y.
        // When carry is set, operator- wraps modulo 2^512 and that wrapped
        // result is the mathematical value (2^512 + rem - y).
        if (carry || !(rem < y))
            rem = rem - y;
    }
    return rem;
}

uint512_t add_mod(const uint512_t &x_in, const uint512_t &y_in, const uint512_t &p)
{
    uint512_t x = mod_reduce(x_in, p);
    uint512_t y = mod_reduce(y_in, p);
    uint512_t gap = p - y;
    if (x >= gap)
        return x - gap;
    return x + y;
}

uint512_t mul_mod(const uint512_t &x_in, const uint512_t &y, const uint512_t &p)
{
    uint512_t ret(0);
    uint512_t x = mod_reduce(x_in, p);
    for (int i = 0; i < uint512_t::BITS; i++)
    {
        if (bit_at(y, i))
            ret = add_mod(ret, x, p);
        x = add_mod(x, x, p);
    }
    return ret;
}

uint512_t modular_exponentiation(const uint512_t &base_in, const uint512_t &exponent, const uint512_t &modulus)
{
    uint512_t ret(1);
    uint512_t base = mod_reduce(base_in, modulus);
    for (int i = 0; i < uint512_t::BITS; i++)
    {
        if (bit_at(exponent, i))
            ret = mul_mod(ret, base, modulus);
        base = mul_mod(base, base, modulus);
    }
    return ret;
}
