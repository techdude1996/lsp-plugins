/*
 * hsum.cpp
 *
 *  Created on: 1 окт. 2018 г.
 *      Author: sadko
 */

#include <dsp/dsp.h>
#include <test/ptest.h>
#include <core/sugar.h>

#define MIN_RANK 8
#define MAX_RANK 16

namespace native
{
    float h_sum(const float *src, size_t count);
    float h_sqr_sum(const float *src, size_t count);
    float h_abs_sum(const float *src, size_t count);
}

IF_ARCH_X86(
    namespace sse
    {
        float h_sum(const float *src, size_t count);
        float h_sqr_sum(const float *src, size_t count);
        float h_abs_sum(const float *src, size_t count);
    }

    namespace avx
    {
        float h_sum(const float *src, size_t count);
        float h_sqr_sum(const float *src, size_t count);
        float h_sqr_sum_fma3(const float *src, size_t count);
        float h_abs_sum(const float *src, size_t count);
    }
)

IF_ARCH_ARM(
    namespace neon_d32
    {
        float h_sum(const float *src, size_t count);
        float h_sqr_sum(const float *src, size_t count);
        float h_abs_sum(const float *src, size_t count);
    }
)

IF_ARCH_AARCH64(
    namespace asimd
    {
        float h_sum(const float *src, size_t count);
        float h_sqr_sum(const float *src, size_t count);
        float h_abs_sum(const float *src, size_t count);
    }
)

typedef float (* h_sum_t)(const float *src, size_t count);

PTEST_BEGIN("dsp.hmath", hsum, 5, 10000)

void call(const char *label, float *src, size_t count, h_sum_t func)
{
    if (!PTEST_SUPPORTED(func))
        return;

    char buf[80];
    sprintf(buf, "%s x %d", label, int(count));
    printf("Testing %s numbers...\n", buf);

    PTEST_LOOP(buf,
        func(src, count);
    );
}

PTEST_MAIN
{
    size_t buf_size = 1 << MAX_RANK;
    uint8_t *data   = NULL;
    float *src      = alloc_aligned<float>(data, buf_size, 64);

    for (size_t i=0; i < buf_size; ++i)
        src[i]          = float(rand()) / RAND_MAX;

    #define CALL(func) \
        call(#func, src, count, func)

    for (size_t i=MIN_RANK; i <= MAX_RANK; ++i)
    {
        size_t count = 1 << i;

        CALL(native::h_sum);
        IF_ARCH_X86(CALL(sse::h_sum));
        IF_ARCH_X86(CALL(avx::h_sum));
        IF_ARCH_ARM(CALL(neon_d32::h_sum));
        IF_ARCH_AARCH64(CALL(asimd::h_sum));
        PTEST_SEPARATOR;

        CALL(native::h_sqr_sum);
        IF_ARCH_X86(CALL(sse::h_sqr_sum));
        IF_ARCH_X86(CALL(avx::h_sqr_sum));
        IF_ARCH_X86(CALL(avx::h_sqr_sum_fma3));
        IF_ARCH_ARM(CALL(neon_d32::h_sqr_sum));
        IF_ARCH_AARCH64(CALL(asimd::h_sqr_sum));
        PTEST_SEPARATOR;

        CALL(native::h_abs_sum);
        IF_ARCH_X86(CALL(sse::h_abs_sum));
        IF_ARCH_X86(CALL(avx::h_abs_sum));
        IF_ARCH_ARM(CALL(neon_d32::h_abs_sum));
        IF_ARCH_AARCH64(CALL(asimd::h_abs_sum));
        PTEST_SEPARATOR2;
    }

    free_aligned(data);
}

PTEST_END

