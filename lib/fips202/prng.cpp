#include "prng.h"

#include <string.h>

#if defined(_WIN32)
#include <random>
#else
#include <errno.h>
#include <sys/random.h>
#endif

void randombytes(uint8_t *out, uint8_t outlen)
{
#if defined(_WIN32)
    // The sample uses getrandom, which is not in this MinGW header set.
    std::random_device rd;
    while (outlen > 0)
    {
        unsigned int value = rd();
        uint8_t chunk = outlen < 4 ? outlen : 4;
        for (uint8_t i = 0; i < chunk; i++)
        {
            out[i] = static_cast<uint8_t>(value & 0xffu);
            value >>= 8;
        }
        out += chunk;
        outlen = static_cast<uint8_t>(outlen - chunk);
    }
#else
    while (outlen > 0)
    {
        ssize_t ret = getrandom(out, outlen, 0);
        if ((ret == -1) && (errno == EINTR))
            continue;
        if (ret < 0)
            abort();
        out += ret;
        outlen = static_cast<uint8_t>(outlen - static_cast<uint8_t>(ret));
    }
#endif
}

void prng_seed(prng_ctx *ctx, const uint8_t *seed, size_t seedlen)
{
    shake128_init(&ctx->sponge);
    shake128_absorb(&ctx->sponge, seed, seedlen);
    shake128_finalize(&ctx->sponge);
}

uint16_t prng_next_u16(prng_ctx *ctx)
{
    uint8_t buf[2];
    shake128_squeeze(buf, sizeof(buf), &ctx->sponge);
    return (uint16_t)buf[0] | (uint16_t)(buf[1] << 8);
}

uint32_t prng_next_u32(prng_ctx *ctx)
{
    uint8_t buf[4];
    shake128_squeeze(buf, sizeof(buf), &ctx->sponge);
    return (uint32_t)buf[0] | ((uint32_t)buf[1] << 8) | ((uint32_t)buf[2] << 16) | ((uint32_t)buf[3] << 24);
}

uint64_t prng_next_u64(prng_ctx *ctx)
{
    uint8_t buf[8];
    shake128_squeeze(buf, sizeof(buf), &ctx->sponge);
    uint64_t ret = 0;
    for (size_t i = 0; i < 8; i++)
        ret |= (uint64_t)buf[i] << (8 * i);
    return ret;
}
