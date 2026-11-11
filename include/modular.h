#ifndef MODULAR_H
#define MODULAR_H

#include "uint512.h"

// Algorithms follow docs/pseudocode-modular-operation.pdf.
// mod scans bits from most significant to least significant.
// Mul and Exp scan y_i where y = sum y_i * 2^i, so index 0 is the least significant bit.

uint512_t mod_reduce(const uint512_t &x, const uint512_t &y);
uint512_t add_mod(const uint512_t &x, const uint512_t &y, const uint512_t &p);
uint512_t mul_mod(const uint512_t &x, const uint512_t &y, const uint512_t &p);

// (base ^ exponent) mod modulus
uint512_t modular_exponentiation(const uint512_t &base, const uint512_t &exponent, const uint512_t &modulus);

#endif
