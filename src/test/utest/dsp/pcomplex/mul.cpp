/*
 * mul.cpp
 *
 *  Created on: 23 авг. 2018 г.
 *      Author: sadko
 */

#include <dsp/dsp.h>
#include <test/utest.h>
#include <test/FloatBuffer.h>

namespace native
{
    void pcomplex_mul2(float *dst, const float *src, size_t count);
    void pcomplex_mul3(float *dst, const float *src1, const float *src2, size_t count);
}

IF_ARCH_X86(
    namespace sse
    {
        void pcomplex_mul2(float *dst, const float *src, size_t count);
        void pcomplex_mul3(float *dst, const float *src1, const float *src2, size_t count);
    }

    namespace sse3
    {
        void pcomplex_mul3(float *dst, const float *src1, const float *src2, size_t count);
    }

    IF_ARCH_X86_64(
        namespace sse3
        {
            void x64_pcomplex_mul3(float *dst, const float *src1, const float *src2, size_t count);
        }

        namespace avx
        {
            void x64_pcomplex_mul3(float *dst, const float *src1, const float *src2, size_t count);
            void x64_pcomplex_mul3_fma3(float *dst, const float *src1, const float *src2, size_t count);
        }
    )
)

IF_ARCH_ARM(
    namespace neon_d32
    {
        void pcomplex_mul2(float *dst, const float *src, size_t count);
        void pcomplex_mul3(float *dst, const float *src1, const float *src2, size_t count);
    }
)

typedef void (* pcomplex_mul2_t) (float *dst, const float *src, size_t count);
typedef void (* pcomplex_mul3_t) (float *dst, const float *src1, const float *src2, size_t count);


UTEST_BEGIN("dsp.pcomplex", mul)

    void call(const char *text,  size_t align, pcomplex_mul3_t func)
    {
        if (!UTEST_SUPPORTED(func))
            return;

        UTEST_FOREACH(count, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                32, 33, 37, 48, 49, 64, 65, 0x3f, 100, 999, 0x1fff)
        {
            for (size_t mask=0; mask <= 0x07; ++mask)
            {
                printf("Testing %s on input buffer of %d numbers, mask=0x%x...\n", text, int(count), int(mask));

                FloatBuffer src1(count*2, align, mask & 0x01);
                FloatBuffer src2(count*2, align, mask & 0x02);
                FloatBuffer dst1(count*2, align, mask & 0x04);
                FloatBuffer dst2(count*2, align, mask & 0x04);

                // Call functions
                native::pcomplex_mul3(dst1, src1, src2, count);
                func(dst2, src1, src2, count);

                UTEST_ASSERT_MSG(src1.valid(), "Source buffer 1 corrupted");
                UTEST_ASSERT_MSG(src2.valid(), "Source buffer 2 corrupted");
                UTEST_ASSERT_MSG(dst1.valid(), "Destination buffer 1 corrupted");
                UTEST_ASSERT_MSG(dst2.valid(), "Destination buffer 2 corrupted");

                // Compare buffers
                if (!dst1.equals_absolute(dst2, 1e-5))
                {
                    src1.dump("src1");
                    src2.dump("src2");
                    dst1.dump("dst1");
                    dst2.dump("dst2");
                    UTEST_FAIL_MSG("Output of functions for test '%s' differs", text);
                }
            }
        }
    }

    void call(const char *text,  size_t align, pcomplex_mul2_t func)
    {
        if (!UTEST_SUPPORTED(func))
            return;

        UTEST_FOREACH(count, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                32, 33, 37, 48, 49, 64, 65, 0x3f, 100, 999, 0x1fff)
        {
            for (size_t mask=0; mask <= 0x03; ++mask)
            {
                printf("Testing %s on input buffer of %d numbers, mask=0x%x...\n", text, int(count), int(mask));

                FloatBuffer src(count*2, align, mask & 0x01);
                FloatBuffer dst1(count*2, align, mask & 0x02);
                FloatBuffer dst2(dst1);

                // Call functions
                native::pcomplex_mul2(dst1, src, count);
                func(dst2, src, count);

                UTEST_ASSERT_MSG(src.valid(), "Source buffer corrupted");
                UTEST_ASSERT_MSG(dst1.valid(), "Destination buffer 1 corrupted");
                UTEST_ASSERT_MSG(dst2.valid(), "Destination buffer 2 corrupted");

                // Compare buffers
                if (!dst1.equals_absolute(dst2, 1e-5))
                {
                    src.dump("src ");
                    dst1.dump("dst1");
                    dst2.dump("dst2");
                    UTEST_FAIL_MSG("Output of functions for test '%s' differs", text);
                }
            }
        }
    }

    UTEST_MAIN
    {
        IF_ARCH_X86(call("sse:pcomplex_mul2", 16, sse::pcomplex_mul2));
        IF_ARCH_X86(call("sse:pcomplex_mul3", 16, sse::pcomplex_mul3));
        IF_ARCH_X86(call("sse3:pcomplex_mul3", 16, sse3::pcomplex_mul3));
        IF_ARCH_X86_64(call("sse3:x64_pcomplex_mul3", 16, sse3::x64_pcomplex_mul3));
        IF_ARCH_X86_64(call("avx:x64_pcomplex_mul3", 32, avx::x64_pcomplex_mul3));
        IF_ARCH_X86_64(call("fma3:x64_pcomplex_mul3", 32, avx::x64_pcomplex_mul3_fma3));
        IF_ARCH_ARM(call("neon_d32:pcomplex_mul2", 16, neon_d32::pcomplex_mul2));
        IF_ARCH_ARM(call("neon_d32:pcomplex_mul3", 16, neon_d32::pcomplex_mul3));
    }

UTEST_END;
