/*
 * oversampling.cpp
 *
 *  Created on: 21 авг. 2018 г.
 *      Author: vsadovnikov
 */

#include <dsp/dsp.h>
#include <test/utest.h>
#include <test/FloatBuffer.h>

namespace native
{
    void lanczos_resample_2x2(float *dst, const float *src, size_t count);
    void lanczos_resample_2x3(float *dst, const float *src, size_t count);
    void lanczos_resample_3x2(float *dst, const float *src, size_t count);
    void lanczos_resample_3x3(float *dst, const float *src, size_t count);
    void lanczos_resample_4x2(float *dst, const float *src, size_t count);
    void lanczos_resample_4x3(float *dst, const float *src, size_t count);
    void lanczos_resample_6x2(float *dst, const float *src, size_t count);
    void lanczos_resample_6x3(float *dst, const float *src, size_t count);
    void lanczos_resample_8x2(float *dst, const float *src, size_t count);
    void lanczos_resample_8x3(float *dst, const float *src, size_t count);
}

IF_ARCH_X86(
    namespace sse
    {
        void lanczos_resample_2x2(float *dst, const float *src, size_t count);
        void lanczos_resample_2x3(float *dst, const float *src, size_t count);
        void lanczos_resample_3x2(float *dst, const float *src, size_t count);
        void lanczos_resample_3x3(float *dst, const float *src, size_t count);
        void lanczos_resample_4x2(float *dst, const float *src, size_t count);
        void lanczos_resample_4x3(float *dst, const float *src, size_t count);
        void lanczos_resample_6x2(float *dst, const float *src, size_t count);
        void lanczos_resample_6x3(float *dst, const float *src, size_t count);
        void lanczos_resample_8x2(float *dst, const float *src, size_t count);
        void lanczos_resample_8x3(float *dst, const float *src, size_t count);
    }

    namespace avx
    {
        void lanczos_resample_2x2(float *dst, const float *src, size_t count);
        void lanczos_resample_2x3(float *dst, const float *src, size_t count);
        void lanczos_resample_3x2(float *dst, const float *src, size_t count);
        void lanczos_resample_3x3(float *dst, const float *src, size_t count);
        void lanczos_resample_4x2(float *dst, const float *src, size_t count);
        void lanczos_resample_4x3(float *dst, const float *src, size_t count);
        void lanczos_resample_6x2(float *dst, const float *src, size_t count);
        void lanczos_resample_6x3(float *dst, const float *src, size_t count);
        void lanczos_resample_8x2(float *dst, const float *src, size_t count);
        void lanczos_resample_8x3(float *dst, const float *src, size_t count);
    }
)

IF_ARCH_ARM(
    namespace neon_d32
    {
        void lanczos_resample_2x2(float *dst, const float *src, size_t count);
        void lanczos_resample_2x3(float *dst, const float *src, size_t count);
        void lanczos_resample_3x2(float *dst, const float *src, size_t count);
        void lanczos_resample_3x3(float *dst, const float *src, size_t count);
        void lanczos_resample_4x2(float *dst, const float *src, size_t count);
        void lanczos_resample_4x3(float *dst, const float *src, size_t count);
        void lanczos_resample_6x2(float *dst, const float *src, size_t count);
        void lanczos_resample_6x3(float *dst, const float *src, size_t count);
        void lanczos_resample_8x2(float *dst, const float *src, size_t count);
        void lanczos_resample_8x3(float *dst, const float *src, size_t count);
    }
)

IF_ARCH_AARCH64(
    namespace asimd
    {
        void lanczos_resample_2x2(float *dst, const float *src, size_t count);
        void lanczos_resample_2x3(float *dst, const float *src, size_t count);
        void lanczos_resample_3x2(float *dst, const float *src, size_t count);
        void lanczos_resample_3x3(float *dst, const float *src, size_t count);
        void lanczos_resample_4x2(float *dst, const float *src, size_t count);
        void lanczos_resample_4x3(float *dst, const float *src, size_t count);
        void lanczos_resample_6x2(float *dst, const float *src, size_t count);
        void lanczos_resample_6x3(float *dst, const float *src, size_t count);
        void lanczos_resample_8x2(float *dst, const float *src, size_t count);
        void lanczos_resample_8x3(float *dst, const float *src, size_t count);
    }
)

UTEST_BEGIN("dsp.resampling", oversampling)

    void call(size_t times, const char *text, size_t align,
            resampling_function_t func1,
            resampling_function_t func2
         )
    {
        if (!UTEST_SUPPORTED(func1))
            return;
        if (!UTEST_SUPPORTED(func2))
            return;

        UTEST_FOREACH(count, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
                32, 64, 100, 999)
        {
            for (size_t mask=0; mask <= 0x03; ++mask)
            {
                printf("Testing %s resampling for %d -> %d samples, mask=0x%x...\n", text, int(count), int(count * times), int(mask));

                FloatBuffer src(count, align, mask & 0x01);
                FloatBuffer dst1(count*times + RESAMPLING_RESERVED_SAMPLES, align, mask & 0x02);
                FloatBuffer dst2(dst1);

                dst1.fill_zero();
                dst2.fill_zero();

                // Call functions
                func1(dst1, src, count);
                func2(dst2, src, count);

                if (src.corrupted())
                    UTEST_FAIL_MSG("Source buffer corrupted");
                if (dst1.corrupted())
                    UTEST_FAIL_MSG("Destination buffer 1 corrupted");
                if (dst2.corrupted())
                    UTEST_FAIL_MSG("Destination buffer 2 corrupted");

                // Compare buffers
                if (!dst1.equals_absolute(dst2))
                {
                    src.dump("src1");
                    dst1.dump("dst1");
                    dst2.dump("dst2");
                    UTEST_FAIL_MSG("Output of functions for test '%s' differs", text);
                }
            }
        }
    }

    UTEST_MAIN
    {
        #define CALL(native, func, align, order) \
            call(order, #func, align, native, func)

        // Do tests
        IF_ARCH_X86(CALL(native::lanczos_resample_2x2, sse::lanczos_resample_2x2, 16, 2));
        IF_ARCH_X86(CALL(native::lanczos_resample_2x3, sse::lanczos_resample_2x3, 16, 2));
        IF_ARCH_X86(CALL(native::lanczos_resample_3x2, sse::lanczos_resample_3x2, 16, 3));
        IF_ARCH_X86(CALL(native::lanczos_resample_3x3, sse::lanczos_resample_3x3, 16, 3));
        IF_ARCH_X86(CALL(native::lanczos_resample_4x2, sse::lanczos_resample_4x2, 16, 4));
        IF_ARCH_X86(CALL(native::lanczos_resample_4x3, sse::lanczos_resample_4x3, 16, 4));
        IF_ARCH_X86(CALL(native::lanczos_resample_6x2, sse::lanczos_resample_6x2, 16, 6));
        IF_ARCH_X86(CALL(native::lanczos_resample_6x3, sse::lanczos_resample_6x3, 16, 6));
        IF_ARCH_X86(CALL(native::lanczos_resample_8x2, sse::lanczos_resample_8x2, 16, 8));
        IF_ARCH_X86(CALL(native::lanczos_resample_8x3, sse::lanczos_resample_8x3, 16, 8));

        IF_ARCH_X86(CALL(native::lanczos_resample_2x2, avx::lanczos_resample_2x2, 32, 2));
        IF_ARCH_X86(CALL(native::lanczos_resample_2x3, avx::lanczos_resample_2x3, 32, 2));
        IF_ARCH_X86(CALL(native::lanczos_resample_3x2, avx::lanczos_resample_3x2, 32, 3));
        IF_ARCH_X86(CALL(native::lanczos_resample_3x3, avx::lanczos_resample_3x3, 32, 3));
        IF_ARCH_X86(CALL(native::lanczos_resample_4x2, avx::lanczos_resample_4x2, 32, 4));
        IF_ARCH_X86(CALL(native::lanczos_resample_4x3, avx::lanczos_resample_4x3, 32, 4));
        IF_ARCH_X86(CALL(native::lanczos_resample_6x2, avx::lanczos_resample_6x2, 32, 6));
        IF_ARCH_X86(CALL(native::lanczos_resample_6x3, avx::lanczos_resample_6x3, 32, 6));
        IF_ARCH_X86(CALL(native::lanczos_resample_8x2, avx::lanczos_resample_8x2, 32, 8));
        IF_ARCH_X86(CALL(native::lanczos_resample_8x3, avx::lanczos_resample_8x3, 32, 8));

        IF_ARCH_ARM(CALL(native::lanczos_resample_2x2, neon_d32::lanczos_resample_2x2, 16, 2));
        IF_ARCH_ARM(CALL(native::lanczos_resample_2x3, neon_d32::lanczos_resample_2x3, 16, 2));
        IF_ARCH_ARM(CALL(native::lanczos_resample_3x2, neon_d32::lanczos_resample_3x2, 16, 3));
        IF_ARCH_ARM(CALL(native::lanczos_resample_3x3, neon_d32::lanczos_resample_3x3, 16, 3));
        IF_ARCH_ARM(CALL(native::lanczos_resample_4x2, neon_d32::lanczos_resample_4x2, 16, 4));
        IF_ARCH_ARM(CALL(native::lanczos_resample_4x3, neon_d32::lanczos_resample_4x3, 16, 4));
        IF_ARCH_ARM(CALL(native::lanczos_resample_6x2, neon_d32::lanczos_resample_6x2, 16, 6));
        IF_ARCH_ARM(CALL(native::lanczos_resample_6x3, neon_d32::lanczos_resample_6x3, 16, 6));
        IF_ARCH_ARM(CALL(native::lanczos_resample_8x2, neon_d32::lanczos_resample_8x2, 16, 8));
        IF_ARCH_ARM(CALL(native::lanczos_resample_8x3, neon_d32::lanczos_resample_8x3, 16, 8));

        IF_ARCH_AARCH64(CALL(native::lanczos_resample_2x2, asimd::lanczos_resample_2x2, 16, 2));
        IF_ARCH_AARCH64(CALL(native::lanczos_resample_2x3, asimd::lanczos_resample_2x3, 16, 2));
        IF_ARCH_AARCH64(CALL(native::lanczos_resample_3x2, asimd::lanczos_resample_3x2, 16, 3));
        IF_ARCH_AARCH64(CALL(native::lanczos_resample_3x3, asimd::lanczos_resample_3x3, 16, 3));
        IF_ARCH_AARCH64(CALL(native::lanczos_resample_4x2, asimd::lanczos_resample_4x2, 16, 4));
        IF_ARCH_AARCH64(CALL(native::lanczos_resample_4x3, asimd::lanczos_resample_4x3, 16, 4));
        IF_ARCH_AARCH64(CALL(native::lanczos_resample_6x2, asimd::lanczos_resample_6x2, 16, 6));
        IF_ARCH_AARCH64(CALL(native::lanczos_resample_6x3, asimd::lanczos_resample_6x3, 16, 6));
        IF_ARCH_AARCH64(CALL(native::lanczos_resample_8x2, asimd::lanczos_resample_8x2, 16, 8));
        IF_ARCH_AARCH64(CALL(native::lanczos_resample_8x3, asimd::lanczos_resample_8x3, 16, 8));
    }
UTEST_END;
