/*
 * pow.cpp
 *
 *  Created on: 13 дек. 2018 г.
 *      Author: sadko
 */

#include <dsp/dsp.h>
#include <test/ptest.h>
#include <core/sugar.h>

#define MIN_RANK 8
#define MAX_RANK 16

namespace native
{
    void powcv1(float *v, float c, size_t count);
    void powcv2(float *dst, const float *v, float c, size_t count);
    void powvc1(float *c, float v, size_t count);
    void powvc2(float *dst, const float *c, float v, size_t count);
    void powvx1(float *v, const float *x, size_t count);
    void powvx2(float *dst, const float *v, const float *x, size_t count);
}

IF_ARCH_X86(
    namespace sse2
    {
        void powcv1(float *v, float c, size_t count);
        void powcv2(float *dst, const float *v, float c, size_t count);
        void powvc1(float *c, float v, size_t count);
        void powvc2(float *dst, const float *c, float v, size_t count);
        void powvx1(float *v, const float *x, size_t count);
        void powvx2(float *dst, const float *v, const float *x, size_t count);
    }
)

IF_ARCH_X86_64(
    namespace avx2
    {
        void x64_powcv1(float *v, float c, size_t count);
        void x64_powcv2(float *dst, const float *v, float c, size_t count);
        void x64_powvc1(float *c, float v, size_t count);
        void x64_powvc2(float *dst, const float *c, float v, size_t count);
        void x64_powvx1(float *v, const float *x, size_t count);
        void x64_powvx2(float *dst, const float *v, const float *x, size_t count);

        void x64_powcv1_fma3(float *v, float c, size_t count);
        void x64_powcv2_fma3(float *dst, const float *v, float c, size_t count);
        void x64_powvc1_fma3(float *c, float v, size_t count);
        void x64_powvc2_fma3(float *dst, const float *c, float v, size_t count);
        void x64_powvx1_fma3(float *v, const float *x, size_t count);
        void x64_powvx2_fma3(float *dst, const float *v, const float *x, size_t count);
    }
)

IF_ARCH_ARM(
    namespace neon_d32
    {
        void powcv1(float *v, float c, size_t count);
        void powcv2(float *dst, const float *v, float c, size_t count);
        void powvc1(float *c, float v, size_t count);
        void powvc2(float *dst, const float *c, float v, size_t count);
        void powvx1(float *v, const float *x, size_t count);
        void powvx2(float *dst, const float *v, const float *x, size_t count);
    }
)

typedef void (* powav1_t)(float *v, float c, size_t count);
typedef void (* powav2_t)(float *dst, const float *v, float c, size_t count);
typedef void (* powvx1_t)(float *v, const float *x, size_t count);
typedef void (* powvx2_t)(float *dst, const float *v, const float *x, size_t count);

//-----------------------------------------------------------------------------
// Performance test
PTEST_BEGIN("dsp.pmath", pow, 5, 1000)

    void call(const char *label, float *dst, const float *src, size_t count, powav1_t func)
    {
        if (!PTEST_SUPPORTED(func))
            return;

        char buf[80];
        sprintf(buf, "%s x %d", label, int(count));
        printf("Testing %s numbers...\n", buf);

        PTEST_LOOP(buf,
            dsp::copy(dst, src, count);
            func(dst, M_PI, count);
        );
    }

    void call(const char *label, float *dst, const float *src, size_t count, powav2_t func)
    {
        if (!PTEST_SUPPORTED(func))
            return;

        char buf[80];
        sprintf(buf, "%s x %d", label, int(count));
        printf("Testing %s numbers...\n", buf);

        PTEST_LOOP(buf,
            func(dst, src, M_PI, count);
        );
    }

    void call(const char *label, float *dst, const float *src1, const float *src2, size_t count, powvx1_t func)
    {
        if (!PTEST_SUPPORTED(func))
            return;

        char buf[80];
        sprintf(buf, "%s x %d", label, int(count));
        printf("Testing %s numbers...\n", buf);

        PTEST_LOOP(buf,
            dsp::copy(dst, src1, count);
            func(dst, src2, count);
        );
    }

    void call(const char *label, float *dst, const float *src1, const float *src2, size_t count, powvx2_t func)
    {
        if (!PTEST_SUPPORTED(func))
            return;

        char buf[80];
        sprintf(buf, "%s x %d", label, int(count));
        printf("Testing %s numbers...\n", buf);

        PTEST_LOOP(buf,
            func(dst, src1, src2, count);
        );
    }

    PTEST_MAIN
    {
        size_t buf_size = 1 << MAX_RANK;
        uint8_t *data   = NULL;
        float *dst      = alloc_aligned<float>(data, buf_size * 3, 64);
        float *src1     = &dst[buf_size];
        float *src2     = &src1[buf_size];

        for (size_t i=0; i < buf_size*3; ++i)
            dst[i]          = float(rand()) / RAND_MAX;

        for (size_t i=MIN_RANK; i <= MAX_RANK; ++i)
        {
            size_t count = 1 << i;

            call("native::powcv1", dst, src1, count, native::powcv1);
            IF_ARCH_X86(call("sse2::powcv1", dst, src1, count, sse2::powcv1));
            IF_ARCH_X86_64(call("avx2::x64_powcv1", dst, src1, count, avx2::x64_powcv1));
            IF_ARCH_X86_64(call("avx2::x64_powcv1_fma3", dst, src1, count, avx2::x64_powcv1_fma3));
            IF_ARCH_ARM(call("neon_d32::powcv1", dst, src1, count, neon_d32::powcv1));
            PTEST_SEPARATOR;

            call("native::powcv2", dst, src1, count, native::powcv2);
            IF_ARCH_X86(call("sse2::powcv2", dst, src1, count, sse2::powcv2));
            IF_ARCH_X86_64(call("avx2::x64_powcv2", dst, src1, count, avx2::x64_powcv2));
            IF_ARCH_X86_64(call("avx2::x64_powcv2_fma3", dst, src1, count, avx2::x64_powcv2_fma3));
            IF_ARCH_ARM(call("neon_d32::powcv2", dst, src1, count, neon_d32::powcv2));
            PTEST_SEPARATOR;

            call("native::powvc1", dst, src1, count, native::powvc1);
            IF_ARCH_X86(call("sse2::powvc1", dst, src1, count, sse2::powvc1));
            IF_ARCH_X86_64(call("avx2::x64_powvc1", dst, src1, count, avx2::x64_powvc1));
            IF_ARCH_X86_64(call("avx2::x64_powvc1_fma3", dst, src1, count, avx2::x64_powvc1_fma3));
            IF_ARCH_ARM(call("neon_d32::powvc1", dst, src1, count, neon_d32::powvc1));
            PTEST_SEPARATOR;

            call("native::powvc2", dst, src1, count, native::powvc2);
            IF_ARCH_X86(call("sse2::powvc2", dst, src1, count, sse2::powvc2));
            IF_ARCH_X86_64(call("avx2::x64_powvc2", dst, src1, count, avx2::x64_powvc2));
            IF_ARCH_X86_64(call("avx2::x64_powvc2_fma3", dst, src1, count, avx2::x64_powvc2_fma3));
            IF_ARCH_ARM(call("neon_d32::powvc2", dst, src1, count, neon_d32::powvc2));
            PTEST_SEPARATOR;

            call("native::powvx1", dst, src1, src2, count, native::powvx1);
            IF_ARCH_X86(call("sse2::powvx1", dst, src1, src2, count, sse2::powvx1));
            IF_ARCH_X86_64(call("avx2::x64_powvx1", dst, src1, src2, count, avx2::x64_powvx1));
            IF_ARCH_X86_64(call("avx2::x64_powvx1_fma3", dst, src1, src2, count, avx2::x64_powvx1_fma3));
            IF_ARCH_ARM(call("neon_d32::powvx1", dst, src1, src2, count, neon_d32::powvx1));
            PTEST_SEPARATOR;

            call("native::powvx2", dst, src1, src2, count, native::powvx2);
            IF_ARCH_X86(call("sse2::powvx2", dst, src1, src2, count, sse2::powvx2));
            IF_ARCH_X86_64(call("avx2::x64_powvx2", dst, src1, src2, count, avx2::x64_powvx2));
            IF_ARCH_X86_64(call("avx2::x64_powvx2_fma3", dst, src1, src2, count, avx2::x64_powvx2_fma3));
            IF_ARCH_ARM(call("neon_d32::powvx2", dst, src1, src2, count, neon_d32::powvx2));
            PTEST_SEPARATOR2;
        }

        free_aligned(data);
    }
PTEST_END



