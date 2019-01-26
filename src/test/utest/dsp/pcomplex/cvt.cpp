/*
 * cvt.cpp
 *
 *  Created on: 29 авг. 2018 г.
 *      Author: sadko
 */

#include <dsp/dsp.h>
#include <test/utest.h>
#include <test/FloatBuffer.h>

namespace native
{
    void pcomplex_r2c(float *dst, const float *src, size_t count);
    void pcomplex_c2r(float *dst, const float *src, size_t count);
}

IF_ARCH_X86(
    namespace sse
    {
        void pcomplex_r2c(float *dst, const float *src, size_t count);
        void pcomplex_c2r(float *dst, const float *src, size_t count);
    }
)

IF_ARCH_ARM(
    namespace neon_d32
    {
        void pcomplex_r2c(float *dst, const float *src, size_t count);
        void pcomplex_c2r(float *dst, const float *src, size_t count);
    }
)

typedef void (* complex_cvt_t) (float *dst, const float *src, size_t count);

UTEST_BEGIN("dsp.pcomplex", cvt)

    void call(const char *label, size_t align, size_t sk, size_t dk, complex_cvt_t func1, complex_cvt_t func2)
    {
        if (!UTEST_SUPPORTED(func1))
            return;
        if (!UTEST_SUPPORTED(func2))
            return;

        UTEST_FOREACH(count, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                32, 33, 37, 48, 49, 64, 65, 0x3f, 100, 999, 0x1fff)
        {
            for (size_t mask=0; mask <= 0x03; ++mask)
            {
                printf("Testing %s on input buffer of %d numbers, mask=0x%x...\n", label, int(count), int(mask));

                FloatBuffer src(count*sk, align, mask & 0x01);
                FloatBuffer dst1(count*dk, align, mask & 0x02);
                FloatBuffer dst2(dst1);

                // Call functions
                func1(dst1, src, count);
                func2(dst2, src, count);

                UTEST_ASSERT_MSG(src.valid(), "Source buffer corrupted");
                UTEST_ASSERT_MSG(dst1.valid(), "Destination buffer 1 corrupted");
                UTEST_ASSERT_MSG(dst2.valid(), "Destination buffer 2 corrupted");

                // Compare buffers
                if (!dst1.equals_absolute(dst2, 1e-5))
                {
                    src.dump("dst");
                    dst1.dump("dst1");
                    dst2.dump("dst2");
                    UTEST_FAIL_MSG("Output of functions for test '%s' differs", label);
                }
            }
        }
    }

    UTEST_MAIN
    {
        IF_ARCH_X86(call("sse:pcomplex_r2c", 16, 1, 2, native::pcomplex_r2c, sse::pcomplex_r2c));
        IF_ARCH_X86(call("sse:pcomplex_c2r", 16, 2, 1, native::pcomplex_c2r, sse::pcomplex_c2r));

        IF_ARCH_ARM(call("neon_d32:pcomplex_r2c", 16, 1, 2, native::pcomplex_r2c, neon_d32::pcomplex_r2c));
        IF_ARCH_ARM(call("neon_d32:pcomplex_c2r", 16, 2, 1, native::pcomplex_c2r, neon_d32::pcomplex_c2r));
    }

UTEST_END


