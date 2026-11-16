#include "dh.h"
#include <stdexcept>
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
    // 1. Kiểm tra đầu vào
    if (ctx == nullptr || p < uint512_t(5))
        throw std::invalid_argument("Invalid input");

    // 2. Tìm số bit thực tế của p
    size_t bit_count = 0;

    for (int i = uint512_t::BITS - 1; i >= 0; --i)
    {
        if (bit_at(p, i))
        {
            bit_count = static_cast<size_t>(i) + 1;
            break;
        }
    }

    // 3. Tính số khối 64-bit cần sinh
    const size_t limbs_used = (bit_count + 63) / 64;
    const size_t high_bits = bit_count % 64;

    const uint512_t lower(2);
    const uint512_t upper = p - uint512_t(2);

    // 4. Rejection sampling
    for (;;)
    {
        uint512_t candidate; // Khởi tạo bằng 0

        for (size_t i = 0; i < limbs_used; ++i)
        {
            candidate.digits[i] = prng_next_u64(ctx);
        }

        // Xóa những bit thừa trong khối cao nhất
        if (high_bits != 0)
        {
            candidate.digits[limbs_used - 1] &=
                (uint64_t(1) << high_bits) - 1;
        }

        // 5. Chỉ nhận khóa thuộc [2, p - 2]
        if (candidate >= lower && candidate <= upper)
            return candidate;
    }
}