/*
 * bq_static.cpp
 *
 *  Created on: 12 окт. 2018 г.
 *      Author: sadko
 */

#include <dsp/dsp.h>
#include <test/utest.h>
#include <test/helpers.h>
#include <test/FloatBuffer.h>

#define BUF_SIZE        1024
#define BUF_STEP        16
#define TOLERANCE       1e-3f

namespace native
{
    void biquad_process_x1(float *dst, const float *src, size_t count, biquad_t *f);
    void biquad_process_x2(float *dst, const float *src, size_t count, biquad_t *f);
    void biquad_process_x4(float *dst, const float *src, size_t count, biquad_t *f);
    void biquad_process_x8(float *dst, const float *src, size_t count, biquad_t *f);
}

IF_ARCH_X86(
    namespace sse
    {
        void biquad_process_x1(float *dst, const float *src, size_t count, biquad_t *f);
        void biquad_process_x2(float *dst, const float *src, size_t count, biquad_t *f);
        void biquad_process_x4(float *dst, const float *src, size_t count, biquad_t *f);
        void biquad_process_x8(float *dst, const float *src, size_t count, biquad_t *f);
    }

    namespace sse3
    {
        void x64_biquad_process_x8(float *dst, const float *src, size_t count, biquad_t *f);
    }

    namespace avx
    {
        void biquad_process_x1(float *dst, const float *src, size_t count, biquad_t *f);
        void biquad_process_x1_fma3(float *dst, const float *src, size_t count, biquad_t *f);

        void biquad_process_x2(float *dst, const float *src, size_t count, biquad_t *f);
        void biquad_process_x2_fma3(float *dst, const float *src, size_t count, biquad_t *f);

        void biquad_process_x4(float *dst, const float *src, size_t count, biquad_t *f);
        void biquad_process_x4_fma3(float *dst, const float *src, size_t count, biquad_t *f);

        void x64_biquad_process_x8(float *dst, const float *src, size_t count, biquad_t *f);
        void biquad_process_x8_fma3(float *dst, const float *src, size_t count, biquad_t *f);
    }
)

IF_ARCH_ARM(
    namespace neon_d32
    {
        void biquad_process_x1(float *dst, const float *src, size_t count, biquad_t *f);
        void biquad_process_x2(float *dst, const float *src, size_t count, biquad_t *f);
        void biquad_process_x4(float *dst, const float *src, size_t count, biquad_t *f);
        void biquad_process_x8(float *dst, const float *src, size_t count, biquad_t *f);
    }
)

IF_ARCH_AARCH64(
    namespace asimd
    {
        void biquad_process_x1(float *dst, const float *src, size_t count, biquad_t *f);
        void biquad_process_x2(float *dst, const float *src, size_t count, biquad_t *f);
        void biquad_process_x4(float *dst, const float *src, size_t count, biquad_t *f);
        void biquad_process_x8(float *dst, const float *src, size_t count, biquad_t *f);
    }
)

typedef void (* biquad_process_t)(float *dst, const float *src, size_t count, biquad_t *f);

UTEST_BEGIN("dsp.filters", static)

    void call(const char *label, biquad_process_t func, size_t n)
    {
        if (!UTEST_SUPPORTED(func))
            return;

        biquad_t f1, f2;

        // Initialize biquad filter
        biquad_x1_t *x1 = &f1.x1;
        x1->b0      = 0.992303491f;
        x1->b1      = -1.98460698f;
        x1->b2      = 0.992303491f;
        x1->a1      = 1.98398674f;
        x1->a2      = -0.985227287f;
        x1->p0      = 0.0f;
        x1->p1      = 0.0f;
        x1->p2      = 0.0f;

        if (n == 1)
            f2.x1       = f1.x1;
        else if (n == 2)
        {
            biquad_x2_t *x2 = &f2.x2;
            for (size_t i=0; i<n; ++i)
            {
                x2->b0[i]   = x1->b0;
                x2->b1[i]   = x1->b1;
                x2->b2[i]   = x1->b2;
                x2->a1[i]   = x1->a1;
                x2->a2[i]   = x1->a2;
                x2->p[i]    = 0.0f;
            }
        }
        else if (n == 4)
        {
            biquad_x4_t *x4 = &f2.x4;
            for (size_t i=0; i<n; ++i)
            {
                x4->b0[i]   = x1->b0;
                x4->b1[i]   = x1->b1;
                x4->b2[i]   = x1->b2;
                x4->a1[i]   = x1->a1;
                x4->a2[i]   = x1->a2;
            }
        }
        else if (n == 8)
        {
            biquad_x8_t *x8 = &f2.x8;
            for (size_t i=0; i<n; ++i)
            {
                x8->b0[i]   = x1->b0;
                x8->b1[i]   = x1->b1;
                x8->b2[i]   = x1->b2;
                x8->a1[i]   = x1->a1;
                x8->a2[i]   = x1->a2;
            }
        }

        UTEST_FOREACH(count, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 0x1f, 0x40, 0x1ff)
//        size_t count=4;
        {
            FloatBuffer src(count);
            FloatBuffer dst1(count);
            FloatBuffer dst2(count);
            src.randomize_sign();
//            for (size_t i=0; i<count; ++i)
//                src[i] = (i & 1) ? -ssize_t(i+1) : ssize_t(i+1);

            printf("Testing %s on input buffer size=%d...\n", label, int(count));

            // Apply processing
            float *ptr = src.data();
            for (size_t i=0; i<n; ++i)
            {
                dsp::fill_zero(f1.d, BIQUAD_D_ITEMS);
                native::biquad_process_x1(dst1, ptr, count, &f1);
                ptr = dst1.data();
            }

            dsp::fill_zero(f2.d, BIQUAD_D_ITEMS);
            func(dst2, src, count, &f2);

            // Perform validation
            UTEST_ASSERT_MSG(src.valid(), "Source buffer corrupted");
            UTEST_ASSERT_MSG(dst1.valid(), "Destination buffer 1 corrupted");
            UTEST_ASSERT_MSG(dst2.valid(), "Destination buffer 2 corrupted");

            if (!dst1.equals_adaptive(dst2, TOLERANCE))
            {
                src.dump("src");
                dst1.dump("dst1");
                dst2.dump("dst2");
                UTEST_FAIL_MSG("Output of functions for test '%s' differs at sample %d: %.6f vs %.6f",
                        label, int(dst1.last_diff()), dst1.get_diff(), dst2.get_diff());
            }
        }
    }

    void call(const char *label, const biquad_t *bq, biquad_process_t func1, biquad_process_t func2)
    {
        if (!UTEST_SUPPORTED(func1))
            return;
        if (!UTEST_SUPPORTED(func2))
            return;

        printf("Testing %s on buffer size %d...\n", label, BUF_SIZE);

        biquad_t f1 = *bq, f2 = *bq;

        FloatBuffer src(BUF_SIZE);
        FloatBuffer dst1(BUF_SIZE);
        FloatBuffer dst2(BUF_SIZE);

//        for (size_t i=0; i<BUF_SIZE; ++i)
//            src[i] = (i & 1) ? -0.001 * float(i+1) : 0.001 * float(i+1);

        for (size_t i=0; i<BUF_SIZE; i += BUF_STEP)
        {
            size_t count = BUF_SIZE - i;
            if (count > BUF_STEP)
                count = BUF_STEP;
            func1(dst1.data(i), src.data(i), count, &f1);
            func2(dst2.data(i), src.data(i), count, &f2);
        }

        UTEST_ASSERT_MSG(src.valid(), "Source buffer corrupted");
        UTEST_ASSERT_MSG(dst1.valid(), "Destination buffer 1 corrupted");
        UTEST_ASSERT_MSG(dst2.valid(), "Destination buffer 2 corrupted");
        if (!dst1.equals_adaptive(dst2, TOLERANCE))
        {
            src.dump("src");
            dst1.dump("dst1");
            dst2.dump("dst2");
            UTEST_FAIL_MSG("Output of functions for test '%s' differs at sample %d: %.6f vs %.6f",
                    label, int(dst1.last_diff()), dst1.get_diff(), dst2.get_diff());
        }

        for (size_t j=0; j<BIQUAD_D_ITEMS; ++j)
        {
            if (float_equals_absolute(f1.d[j], f2.d[j], TOLERANCE))
                continue;
            UTEST_FAIL_MSG("Filter memory items #%d for test '%s' differ: %.6f vs %.6f",
                    int(j), label, f1.d[j], f2.d[j]);
        }
    }


    UTEST_MAIN
    {
        #define CALL(func, count) \
            call(#func, func, count)

        // PART 1, overall check
        CALL(native::biquad_process_x1, 1);
        IF_ARCH_X86(CALL(sse::biquad_process_x1, 1));
        IF_ARCH_X86(CALL(avx::biquad_process_x1, 1));
        IF_ARCH_X86(CALL(avx::biquad_process_x1_fma3, 1));
        IF_ARCH_ARM(CALL(neon_d32::biquad_process_x1, 1));
        IF_ARCH_AARCH64(CALL(asimd::biquad_process_x1, 1));

        CALL(native::biquad_process_x2, 2);
        IF_ARCH_X86(CALL(sse::biquad_process_x2, 2));
        IF_ARCH_X86(CALL(avx::biquad_process_x2, 2));
        IF_ARCH_X86(CALL(avx::biquad_process_x2_fma3, 2));
        IF_ARCH_ARM(CALL(neon_d32::biquad_process_x2, 2));
        IF_ARCH_AARCH64(CALL(asimd::biquad_process_x2, 2));

        CALL(native::biquad_process_x4, 4);
        IF_ARCH_X86(CALL(sse::biquad_process_x4, 4));
        IF_ARCH_X86(CALL(avx::biquad_process_x4, 4));
        IF_ARCH_X86(CALL(avx::biquad_process_x4_fma3, 4));
        IF_ARCH_ARM(CALL(neon_d32::biquad_process_x4, 4));
        IF_ARCH_AARCH64(CALL(asimd::biquad_process_x4, 4));

        CALL(native::biquad_process_x8, 8);
        IF_ARCH_X86(CALL(sse::biquad_process_x8, 8));
        IF_ARCH_X86(CALL(sse3::x64_biquad_process_x8, 8));
        IF_ARCH_X86(CALL(avx::x64_biquad_process_x8, 8));
        IF_ARCH_X86(CALL(avx::biquad_process_x8_fma3, 8));
        IF_ARCH_ARM(CALL(neon_d32::biquad_process_x8, 8));
        IF_ARCH_AARCH64(CALL(asimd::biquad_process_x8, 8));

        #undef CALL
        #define CALL(native, func) \
            call(#func, &bq, native, func)

        // PART 2
        biquad_t bq __lsp_aligned64;
        dsp::fill_zero(bq.d, BIQUAD_D_ITEMS);

        // Prepare 2 zero, 2 pole hi-pass filter
        biquad_x1_t *x1 = &bq.x1;
        x1->b0      = 0.992303491f;
        x1->b1      = -1.98460698f;
        x1->b2      = 0.992303491f;
        x1->a1      = 1.98398674f;
        x1->a2      = -0.985227287f;
        x1->p0      = 0.0f;
        x1->p1      = 0.0f;
        x1->p2      = 0.0f;

        IF_ARCH_X86(CALL(native::biquad_process_x1, sse::biquad_process_x1));
        IF_ARCH_X86(CALL(native::biquad_process_x1, avx::biquad_process_x1));
        IF_ARCH_X86(CALL(native::biquad_process_x1, avx::biquad_process_x1_fma3));
        IF_ARCH_ARM(CALL(native::biquad_process_x1, neon_d32::biquad_process_x1));
        IF_ARCH_AARCH64(CALL(native::biquad_process_x1, asimd::biquad_process_x1));

        // Prepare 4 zero, 4 pole shelving filter
        biquad_x2_t *x2 = &bq.x2;
        x2->b0[0]   = 0.346979439f;
        x2->b0[1]   = 0.704830527f;
        x2->b1[0]   = -0.683136344f;
        x2->b1[1]   = -1.38767684f;
        x2->b2[0]   = 0.337956876f;
        x2->b2[1]   = 0.686502695f;

        x2->a1[0]   = 1.97910678;
        x2->a1[1]   = 1.97910678;
        x2->a2[0]   = -0.981672168;
        x2->a2[1]   = -0.981672168;
        x2->p[0]    = 0.0f;
        x2->p[1]    = 0.0f;

        IF_ARCH_X86(CALL(native::biquad_process_x2, sse::biquad_process_x2));
        IF_ARCH_X86(CALL(native::biquad_process_x2, avx::biquad_process_x2));
        IF_ARCH_X86(CALL(native::biquad_process_x2, avx::biquad_process_x2_fma3));
        IF_ARCH_ARM(CALL(native::biquad_process_x2, neon_d32::biquad_process_x2));
        IF_ARCH_AARCH64(CALL(native::biquad_process_x2, asimd::biquad_process_x2));

        // Prepare 8 zero, 8 pole shelving filter
        biquad_x4_t *x4 = &bq.x4;

        x4->b0[0]   = 0.515558779;
        x4->b0[1]   = 0.878978848;
        x4->b0[2]   = 0.878978848;
        x4->b0[3]   = 0.878978848;

        x4->b1[0]   = -0.994858623;
        x4->b1[1]   = -1.69613969;
        x4->b1[2]   = -1.69613969;
        x4->b1[3]   = -1.69613969;

        x4->b2[0]   = 0.481613606;
        x4->b2[1]   = 0.821105599;
        x4->b2[2]   = 0.821105599;
        x4->b2[3]   = 0.821105599;

        x4->a1[0]   = 1.93867457;
        x4->a1[1]   = 1.93867457;
        x4->a1[2]   = 1.93867457;
        x4->a1[3]   = 1.93867457;

        x4->a2[0]   = -0.942126751;
        x4->a2[1]   = -0.942126751;
        x4->a2[2]   = -0.942126751;
        x4->a2[3]   = -0.942126751;

        IF_ARCH_X86(CALL(native::biquad_process_x4, sse::biquad_process_x4));
        IF_ARCH_X86(CALL(native::biquad_process_x4, avx::biquad_process_x4));
        IF_ARCH_X86(CALL(native::biquad_process_x4, avx::biquad_process_x4_fma3));
        IF_ARCH_ARM(CALL(native::biquad_process_x4, neon_d32::biquad_process_x4));
        IF_ARCH_AARCH64(CALL(native::biquad_process_x4, asimd::biquad_process_x4));

        // Prepare simple 16 zero, 16 pole filter
        biquad_x8_t *x8 = &bq.x8;

        x8->b0[0]   = 1.79906213f;
        x8->b0[1]   = 1.16191483f;
        x8->b0[2]   = 1.13150513f;
        x8->b0[3]   = 1.11161804f;
        x8->b0[4]   = 1.79906213f;
        x8->b0[5]   = 1.16191483f;
        x8->b0[6]   = 1.13150513f;
        x8->b0[7]   = 1.11161804f;

        x8->b1[0]   = -3.38381839f;
        x8->b1[1]   = -2.20469999f;
        x8->b1[2]   = -2.18261695f;
        x8->b1[3]   = -2.19184852f;
        x8->b1[4]   = -3.38381839f;
        x8->b1[5]   = -2.20469999f;
        x8->b1[6]   = -2.18261695f;
        x8->b1[7]   = -2.19184852f;

        x8->b2[0]   = 1.59139514f;
        x8->b2[1]   = 1.04720736f;
        x8->b2[2]   = 1.05562544f;
        x8->b2[3]   = 1.08485937f;
        x8->b2[4]   = 1.59139514f;
        x8->b2[5]   = 1.04720736f;
        x8->b2[6]   = 1.05562544f;
        x8->b2[7]   = 1.08485937f;

        x8->a1[0]   = 1.8580488f;
        x8->a1[1]   = 1.88010871f;
        x8->a1[2]   = 1.91898823f;
        x8->a1[3]   = 1.96808743f;
        x8->a1[4]   = 1.8580488f;
        x8->a1[5]   = 1.88010871f;
        x8->a1[6]   = 1.91898823f;
        x8->a1[7]   = 1.96808743f;

        x8->a2[0]   = -0.863286555f;
        x8->a2[1]   = -0.88529253f;
        x8->a2[2]   = -0.924120247f;
        x8->a2[3]   = -0.97324127f;
        x8->a2[4]   = -0.863286555f;
        x8->a2[5]   = -0.88529253f;
        x8->a2[6]   = -0.924120247f;
        x8->a2[7]   = -0.97324127f;

        IF_ARCH_X86(CALL(native::biquad_process_x8, sse::biquad_process_x8));
        IF_ARCH_X86(CALL(native::biquad_process_x8, sse3::x64_biquad_process_x8));
        IF_ARCH_X86(CALL(native::biquad_process_x8, avx::x64_biquad_process_x8));
        IF_ARCH_X86(CALL(native::biquad_process_x8, avx::biquad_process_x8_fma3));
        IF_ARCH_ARM(CALL(native::biquad_process_x8, neon_d32::biquad_process_x8));
        IF_ARCH_AARCH64(CALL(native::biquad_process_x8, asimd::biquad_process_x8));
    }

UTEST_END
