#ifndef DH_H
#define DH_H

#include "modular.h"
#include "prng.h"

// Safe prime p: both p and (p - 1) / 2 are prime.
// bitsize is the length of p in bits. This project stores p in uint512_t, so 2 <= bitsize <= 512.
// The course requires at least 512 bits for the submitted program.
uint512_t generate_safe_prime(prng_ctx *ctx, size_t bitsize = 512);

// Uniform private key in the closed interval [2, p - 2].
uint512_t generate_private_key(prng_ctx *ctx, const uint512_t &p);

#endif
