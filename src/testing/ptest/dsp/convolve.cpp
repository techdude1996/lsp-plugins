/*
 * convolve.cpp
 *
 *  Created on: 7 сент. 2018 г.
 *      Author: sadko
 */

#include <dsp/dsp.h>
#include <test/ptest.h>

#define MIN_RANK        5
#define MAX_RANK        8

namespace native
{
    void convolve(float *dst, const float *src, const float *conv, size_t length, size_t count);
}

IF_ARCH_X86(
    namespace sse
    {
        void convolve(float *dst, const float *src, const float *conv, size_t length, size_t count);
    }

    namespace avx
    {
        void convolve(float *dst, const float *src, const float *conv, size_t length, size_t count);
        void convolve_fma3(float *dst, const float *src, const float *conv, size_t length, size_t count);
    }
)

IF_ARCH_ARM(
    namespace neon_d32
    {
        void convolve(float *dst, const float *src, const float *conv, size_t length, size_t count);
    }
)

IF_ARCH_AARCH64(
    namespace asimd
    {
        void convolve(float *dst, const float *src, const float *conv, size_t length, size_t count);
    }
)

namespace test
{
    static void convolve(float *dst, const float *src, const float *conv, size_t length, size_t count)
    {
        for (size_t i=0; i<count; ++i)
        {
            for (size_t j=0; j<length; ++j)
                dst[i+j] += src[i] * conv[j];
        }
    }

    static void convolve_sadd(float *dst, const float *src, const float *conv, size_t length, size_t count)
    {
        for (size_t i=0; i<count; ++i)
            dsp::fmadd_k3(&dst[i], conv, src[i], length);
    }
}

typedef void (* convolve_t)(float *dst, const float *src, const float *conv, size_t length, size_t count);

//-----------------------------------------------------------------------------
// Performance test for lanczos resampling
PTEST_BEGIN("dsp", convolve, 5, 1000)

    void call(const char *label, float *out, const float *in, const float *conv, size_t length, size_t count, convolve_t func)
    {
        if (!PTEST_SUPPORTED(func))
            return;

        char buf[80];
        sprintf(buf, "%s %d x %d", label, int(count), int(length));
        printf("Testing %s convolution ...\n", buf);

        PTEST_LOOP(buf,
            func(out, in, conv, length, count);
        );
    }

    PTEST_MAIN
    {
        size_t buf_size = 1 << MAX_RANK;
        uint8_t *data   = NULL;
        float *out      = alloc_aligned<float>(data, buf_size * 8, 64);
        float *in       = &out[buf_size*2];
        float *conv     = &in[buf_size];
        float *backup   = &conv[buf_size];

        for (size_t i=0; i < buf_size*4; ++i)
            out[i]          = float(rand()) / RAND_MAX;
        dsp::copy(backup, out, buf_size * 4);

        #define CALL(r1, r2, func) \
            dsp::copy(out, backup, buf_size * 4); \
            call(#func, out, in, conv, r1, r2, func)

        TEST_EXPORT(test::convolve);
        TEST_EXPORT(test::convolve_sadd);

        for (size_t i=MIN_RANK; i<=MAX_RANK; ++i)
            for (size_t j=MIN_RANK; j<=MAX_RANK; ++j)
            {
                CALL((1 << j), (1 << i), test::convolve);
                CALL((1 << j), (1 << i), test::convolve_sadd);
                CALL((1 << j), (1 << i), native::convolve);
                IF_ARCH_X86(CALL((1 << j), (1 << i), sse::convolve));
                IF_ARCH_X86(CALL((1 << j), (1 << i), avx::convolve));
                IF_ARCH_X86(CALL((1 << j), (1 << i), avx::convolve_fma3));
                IF_ARCH_ARM(CALL((1 << j), (1 << i), neon_d32::convolve));
                IF_ARCH_AARCH64(CALL((1 << j), (1 << i), asimd::convolve));

                PTEST_SEPARATOR;
            }

        CALL((1 << MAX_RANK) - 1, (1 << MAX_RANK) - 1, test::convolve);
        CALL((1 << MAX_RANK) - 1, (1 << MAX_RANK) - 1, test::convolve_sadd);
        CALL((1 << MAX_RANK) - 1, (1 << MAX_RANK) - 1, native::convolve);
        IF_ARCH_X86(CALL((1 << MAX_RANK) - 1, (1 << MAX_RANK) - 1, sse::convolve));
        IF_ARCH_X86(CALL((1 << MAX_RANK) - 1, (1 << MAX_RANK) - 1, avx::convolve));
        IF_ARCH_X86(CALL((1 << MAX_RANK) - 1, (1 << MAX_RANK) - 1, avx::convolve_fma3));
        IF_ARCH_ARM(CALL((1 << MAX_RANK) - 1, (1 << MAX_RANK) - 1, neon_d32::convolve));
        IF_ARCH_AARCH64(CALL((1 << MAX_RANK) - 1, (1 << MAX_RANK) - 1, asimd::convolve));

        PTEST_SEPARATOR;
    }

PTEST_END





