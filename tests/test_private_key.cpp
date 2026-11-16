#include "dh.h"

#include <cstdint>
#include <iostream>

namespace {
int failed = 0;

void expect(bool condition, const char *name)
{
    if (condition)
        std::cout << "ok   " << name << '\n';
    else {
        std::cout << "FAIL " << name << '\n';
        ++failed;
    }
}

// Fixed seed makes tests reproducible. Never use this seed for real keys.
void seed_for_test(prng_ctx &ctx, uint8_t tag)
{
    uint8_t seed[32] = {};
    for (size_t i = 0; i < sizeof(seed); ++i)
        seed[i] = static_cast<uint8_t>(i + tag);
    prng_seed(&ctx, seed, sizeof(seed));
}

// Test 1: All samples must be in the closed interval [2, p-2].
void test_range()
{
    prng_ctx ctx;
    seed_for_test(ctx, 1);
    const uint512_t p(23);
    bool valid = true;
    for (int i = 0; i < 1000; ++i) {
        uint512_t key = generate_private_key(&ctx, p);
        if (key < uint512_t(2) || key > uint512_t(21)) {
            valid = false;
            break;
        }
    }
    expect(valid, "1000 keys are in [2, 21] when p=23");
}

// Test 2: The minimum supported prime p=5 allows only 2 and 3.
void test_small_boundary()
{
    prng_ctx ctx;
    seed_for_test(ctx, 2);
    bool valid = true;
    bool saw_two = false, saw_three = false;
    for (int i = 0; i < 1000; ++i) {
        uint512_t key = generate_private_key(&ctx, uint512_t(5));
        if (key == uint512_t(2)) saw_two = true;
        else if (key == uint512_t(3)) saw_three = true;
        else { valid = false; break; }
    }
    expect(valid, "p=5 produces only 2 or 3");
    expect(saw_two && saw_three, "p=5 can produce both permitted values");
}

// Test 3: 2^511 + 111 is a 512-bit prime. This is a RANGE test,
// not a test of safe-prime generation or Diffie-Hellman security.
void test_512_bit_prime()
{
    uint512_t p;
    p.digits[0] = 111;
    p.digits[7] = uint64_t(1) << 63;

    prng_ctx ctx;
    seed_for_test(ctx, 3);
    bool valid = true;
    bool saw_more_than_64_bits = false;
    for (int i = 0; i < 1000; ++i) {
        uint512_t key = generate_private_key(&ctx, p);
        if (key < uint512_t(2) || key > p - uint512_t(2)) {
            valid = false;
            break;
        }
        for (size_t j = 1; j < uint512_t::LIMBS; ++j)
            if (key.digits[j] != 0) saw_more_than_64_bits = true;
    }
    expect(valid, "1000 keys are in range for a 512-bit prime");
    expect(saw_more_than_64_bits, "512-bit sampling uses more than 64 bits");
}

// Test 4: Same seed => same PRNG sequence => same key sequence.
void test_reproducibility()
{
    prng_ctx a, b;
    seed_for_test(a, 4);
    seed_for_test(b, 4);
    bool same = true;
    for (int i = 0; i < 100; ++i) {
        if (generate_private_key(&a, uint512_t(23)) !=
            generate_private_key(&b, uint512_t(23))) {
            same = false;
            break;
        }
    }
    expect(same, "same seed yields same sequence");
}

// Test 5: A basic sanity check on distribution, NOT a proof
// of cryptographic security. For p=7 there are 4 valid values.
void test_distribution_sanity()
{
    prng_ctx ctx;
    seed_for_test(ctx, 5);
    int counts[4] = {0, 0, 0, 0};
    bool valid = true;
    for (int i = 0; i < 10000; ++i) {
        uint512_t key = generate_private_key(&ctx, uint512_t(7));
        if (key < uint512_t(2) || key > uint512_t(5)) {
            valid = false;
            break;
        }
        ++counts[static_cast<size_t>(key.digits[0] - 2)];
    }
    expect(valid, "p=7 produces only 2, 3, 4, 5");
    bool reasonably_balanced = valid;
    for (int i = 0; i < 4; ++i) {
        std::cout << "key=" << (i + 2) << ": " << counts[i] << " occurrences\n";
        if (counts[i] < 2000 || counts[i] > 3000)
            reasonably_balanced = false;
    }
    expect(reasonably_balanced, "rough frequency sanity check (10000 draws)");
}
} // namespace

int main()
{
    test_range();
    test_small_boundary();
    test_512_bit_prime();
    test_reproducibility();
    test_distribution_sanity();
    if (failed != 0) {
        std::cout << failed << " test(s) failed\n";
        return 1;
    }
    std::cout << "all private-key tests passed\n";
    return 0;
}
