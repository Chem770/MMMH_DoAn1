#include "modular.h"

#include <iostream>

namespace
{

int g_failed = 0;

void expect(bool condition, const char *name)
{
    if (condition)
    {
        std::cout << "ok   " << name << "\n";
        return;
    }
    std::cout << "FAIL " << name << "\n";
    g_failed++;
}

uint512_t limb_number(uint64_t low, uint64_t high)
{
    uint512_t value(low);
    value.digits[1] = high;
    return value;
}

} // namespace

int main()
{
    uint512_t one(1);
    uint512_t two(2);

    expect(uint512_t(0) + uint512_t(0) == uint512_t(0), "0 + 0");
    expect(one + one == two, "1 + 1");

    uint512_t max_limb(~uint64_t(0));
    uint512_t wrapped = max_limb + one;
    expect(wrapped.digits[0] == 0 && wrapped.digits[1] == 1, "carry into the next limb");

    expect(wrapped - one == max_limb, "borrow from the next limb");
    expect(two > one && one < two && one <= one && two >= two, "comparisons");
    expect(bit_at(uint512_t(8), 3) == 1 && bit_at(uint512_t(8), 0) == 0, "bit 3 of 8");

    expect(mod_reduce(uint512_t(100), uint512_t(7)) == uint512_t(2), "100 mod 7");
    expect(mod_reduce(uint512_t(5), uint512_t(13)) == uint512_t(5), "value already reduced");
    expect(mod_reduce(limb_number(0, 1), uint512_t(10)) == uint512_t(6), "2^64 mod 10");

    expect(add_mod(uint512_t(5), uint512_t(6), uint512_t(7)) == uint512_t(4), "(5+6) mod 7");
    expect(add_mod(uint512_t(6), uint512_t(6), uint512_t(7)) == uint512_t(5), "(6+6) mod 7");

    expect(mul_mod(uint512_t(6), uint512_t(6), uint512_t(7)) == uint512_t(1), "6*6 mod 7");
    expect(modular_exponentiation(uint512_t(3), uint512_t(5), uint512_t(13)) == uint512_t(9), "3^5 mod 13");
    expect(modular_exponentiation(uint512_t(2), uint512_t(100), uint512_t(13)) == uint512_t(3), "2^100 mod 13");
    expect(modular_exponentiation(uint512_t(5), uint512_t(0), uint512_t(13)) == uint512_t(1), "5^0 mod 13");
    expect(modular_exponentiation(uint512_t(0), uint512_t(5), uint512_t(13)) == uint512_t(0), "0^5 mod 13");

    uint512_t base = limb_number(5, 1);
    uint512_t exponent(uint64_t(1) << 32);
    exponent = exponent + uint512_t(7);
    uint512_t modulus = limb_number(123, 1);
    expect(modular_exponentiation(base, exponent, modulus) == uint512_t(2630404069070072883ull),
           "(2^64+5)^(2^32+7) mod (2^64+123)");

    uint512_t p(23);
    uint512_t g(5);
    uint512_t a(6);
    uint512_t b(15);
    uint512_t A = modular_exponentiation(g, a, p);
    uint512_t B = modular_exponentiation(g, b, p);
    uint512_t alice = modular_exponentiation(B, a, p);
    uint512_t bob = modular_exponentiation(A, b, p);
    expect(A == uint512_t(8) && B == uint512_t(19) && alice == uint512_t(2) && alice == bob,
           "small Diffie-Hellman on p=23");

    if (g_failed != 0)
    {
        std::cout << g_failed << " test(s) failed\n";
        return 1;
    }
    std::cout << "all tests passed\n";
    return 0;
}
