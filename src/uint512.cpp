#include "uint512.h"

#include <iostream>

uint512_t::uint512_t()
{
    for (size_t i = 0; i < LIMBS; i++)
        digits[i] = 0;
}

uint512_t::uint512_t(uint64_t value)
{
    for (size_t i = 0; i < LIMBS; i++)
        digits[i] = 0;
    digits[0] = value;
}

uint512_t operator+(const uint512_t &a, const uint512_t &b)
{
    uint512_t result;
    uint64_t carry = 0;
    for (size_t i = 0; i < uint512_t::LIMBS; i++)
    {
        uint64_t sum = a.digits[i] + b.digits[i];
        uint64_t carry_from_add = (sum < a.digits[i]) ? 1 : 0;
        uint64_t with_carry = sum + carry;
        uint64_t carry_from_extra = (with_carry < sum) ? 1 : 0;
        result.digits[i] = with_carry;
        carry = carry_from_add | carry_from_extra;
    }
    return result;
}

uint512_t operator-(const uint512_t &a, const uint512_t &b)
{
    uint512_t result;
    uint64_t borrow = 0;
    for (size_t i = 0; i < uint512_t::LIMBS; i++)
    {
        uint64_t diff = a.digits[i] - b.digits[i];
        uint64_t borrow_from_sub = (a.digits[i] < b.digits[i]) ? 1 : 0;
        uint64_t with_borrow = diff - borrow;
        uint64_t borrow_from_extra = (diff < borrow) ? 1 : 0;
        result.digits[i] = with_borrow;
        borrow = borrow_from_sub | borrow_from_extra;
    }
    return result;
}

bool operator==(const uint512_t &a, const uint512_t &b)
{
    for (size_t i = 0; i < uint512_t::LIMBS; i++)
    {
        if (a.digits[i] != b.digits[i])
            return false;
    }
    return true;
}

bool operator!=(const uint512_t &a, const uint512_t &b)
{
    return !(a == b);
}

bool operator<(const uint512_t &a, const uint512_t &b)
{
    for (size_t i = uint512_t::LIMBS; i > 0; i--)
    {
        uint64_t left = a.digits[i - 1];
        uint64_t right = b.digits[i - 1];
        if (left < right)
            return true;
        if (left > right)
            return false;
    }
    return false;
}

bool operator>(const uint512_t &a, const uint512_t &b)
{
    return b < a;
}

bool operator>=(const uint512_t &a, const uint512_t &b)
{
    return !(a < b);
}

bool operator<=(const uint512_t &a, const uint512_t &b)
{
    return !(b < a);
}

bool is_zero(const uint512_t &value)
{
    for (size_t i = 0; i < uint512_t::LIMBS; i++)
    {
        if (value.digits[i] != 0)
            return false;
    }
    return true;
}

int bit_at(const uint512_t &value, int index)
{
    if (index < 0 || index >= uint512_t::BITS)
        return 0;
    size_t limb = static_cast<size_t>(index) / 64;
    int offset = index % 64;
    return static_cast<int>((value.digits[limb] >> offset) & 1ull);
}

int top_bit(const uint512_t &value)
{
    return static_cast<int>(value.digits[uint512_t::LIMBS - 1] >> 63);
}

uint512_t shift_left_one(const uint512_t &value)
{
    uint512_t result;
    uint64_t carry = 0;
    for (size_t i = 0; i < uint512_t::LIMBS; i++)
    {
        uint64_t next = value.digits[i] >> 63;
        result.digits[i] = (value.digits[i] << 1) | carry;
        carry = next;
    }
    return result;
}

std::ostream &operator<<(std::ostream &os, const uint512_t &value)
{
    bool higher = false;
    for (size_t i = 1; i < uint512_t::LIMBS; i++)
    {
        if (value.digits[i] != 0)
            higher = true;
    }
    if (!higher)
    {
        os << value.digits[0];
        return os;
    }

    int start = static_cast<int>(uint512_t::LIMBS) - 1;
    while (start > 0 && value.digits[start] == 0)
        start--;

    os << "0x" << std::hex;
    os << value.digits[start];
    for (int i = start - 1; i >= 0; i--)
    {
        uint64_t limb = value.digits[i];
        for (int shift = 60; shift >= 0; shift -= 4)
        {
            unsigned nibble = static_cast<unsigned>((limb >> shift) & 0xfull);
            os << "0123456789abcdef"[nibble];
        }
    }
    os << std::dec;
    return os;
}
