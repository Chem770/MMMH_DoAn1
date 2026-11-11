#include "dh.h"

#include <cstring>
#include <iostream>

namespace
{

void demo_small_exchange()
{
    // Fixed safe prime so the finished modular exponentiation can be checked
    // before generate_safe_prime exists. p = 23, (p - 1) / 2 = 11, both prime.
    const uint512_t p(23);
    const uint512_t g(5);
    const uint512_t a(6);
    const uint512_t b(15);

    uint512_t A = modular_exponentiation(g, a, p);
    uint512_t B = modular_exponentiation(g, b, p);
    uint512_t alice = modular_exponentiation(B, a, p);
    uint512_t bob = modular_exponentiation(A, b, p);

    std::cout << "Demo p=23 g=5 a=6 b=15\n";
    std::cout << "A = g^a mod p = " << A << "\n";
    std::cout << "B = g^b mod p = " << B << "\n";
    std::cout << "Alice shared secret = " << alice << "\n";
    std::cout << "Bob shared secret   = " << bob << "\n";
    std::cout << "Demo secrets match? " << (alice == bob ? "yes" : "no") << "\n";
}

} // namespace

int main()
{
    demo_small_exchange();

    uint8_t seed[32];
    std::memset(seed, 0, sizeof(seed));
    std::memcpy(seed, "seed", sizeof("seed"));

    prng_ctx ctx;
    prng_seed(&ctx, seed, sizeof(seed));

    uint512_t p = generate_safe_prime(&ctx, 512);
    if (is_zero(p))
    {
        std::cout << "\n512-bit exchange is not running yet.\n";
        std::cout << "Implement generate_safe_prime and generate_private_key in src/prime.cpp.\n";
        std::cout << "Then this main will compute A, B, and the shared secret.\n";
        return 0;
    }

    uint512_t g(2);
    uint512_t a = generate_private_key(&ctx, p);
    uint512_t b = generate_private_key(&ctx, p);

    uint512_t A = modular_exponentiation(g, a, p);
    uint512_t B = modular_exponentiation(g, b, p);
    uint512_t alice = modular_exponentiation(B, a, p);
    uint512_t bob = modular_exponentiation(A, b, p);

    std::cout << "\nPrime p = " << p << "\n";
    std::cout << "Alice shared secret = " << alice << "\n";
    std::cout << "Bob shared secret   = " << bob << "\n";
    std::cout << "Secrets match? " << (alice == bob ? "yes" : "no") << "\n";
    return (alice == bob) ? 0 : 1;
}
