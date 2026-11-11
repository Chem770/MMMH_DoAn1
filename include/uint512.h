#ifndef UINT512_H
#define UINT512_H

#include <stddef.h>
#include <stdint.h>
#include <iosfwd>

// 512-bit unsigned integer.
// digits[0] is the least significant limb. Bit 0 is the least significant bit.
struct uint512_t
{
    static const size_t LIMBS = 8;
    static const int BITS = 512;

    uint64_t digits[LIMBS];

    uint512_t();
    explicit uint512_t(uint64_t value);
};

uint512_t operator+(const uint512_t &a, const uint512_t &b);
uint512_t operator-(const uint512_t &a, const uint512_t &b);
bool operator==(const uint512_t &a, const uint512_t &b);
bool operator!=(const uint512_t &a, const uint512_t &b);
bool operator<(const uint512_t &a, const uint512_t &b);
bool operator>(const uint512_t &a, const uint512_t &b);
bool operator>=(const uint512_t &a, const uint512_t &b);
bool operator<=(const uint512_t &a, const uint512_t &b);

bool is_zero(const uint512_t &value);

// index 0 is the least significant bit. Out-of-range indexes read as 0.
int bit_at(const uint512_t &value, int index);

// Shift left by one bit. The bit that leaves the top of the 512-bit word is discarded.
uint512_t shift_left_one(const uint512_t &value);

// 1 when the shift above discarded a 1.
int top_bit(const uint512_t &value);

std::ostream &operator<<(std::ostream &os, const uint512_t &value);

#endif
