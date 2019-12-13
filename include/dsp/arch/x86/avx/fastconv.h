/*
 * fastconv.h
 *
 *  Created on: 13 дек. 2019 г.
 *      Author: sadko
 */

#ifndef DSP_ARCH_X86_AVX_FASTCONV_H_
#define DSP_ARCH_X86_AVX_FASTCONV_H_

#ifndef DSP_ARCH_X86_AVX_IMPL
    #error "This header should not be included directly"
#endif /* DSP_ARCH_X86_AVX_IMPL */

#include <dsp/arch/x86/avx/fastconv/prepare.h>
#include <dsp/arch/x86/avx/fastconv/butterfly.h>

namespace avx
{
    void fastconv_parse(float *dst, const float *src, size_t rank)
    {
        const float *ak = &FFT_A[(rank - 2) << 4];
        const float *wk = &FFT_DW[(rank - 2) << 4];
        size_t np       = 1 << (rank - 1);
        size_t nb       = 1;

        if (np > 8)
        {
            fastconv_prepare(dst, src, ak, wk, np);
            ak         -= 16;
            wk         -= 16;
            np        >>= 1;
            nb        <<= 1;
        }
        else
            fastconv_unpack(dst, src, np);
/*
        for (; np > 8; np >>= 1, nb <<= 1)
        {
            fastconv_direct_butterfly(dst, ak, wk, np, nb);
            ak         -= 16;
            wk         -= 16;
        }*/
    }

    void fastconv_restore(float *dst, float *tmp, size_t rank)
    {
        // TODO
    }

    void fastconv_apply(float *dst, float *tmp, const float *c1, const float *c2, size_t rank)
    {
        // TODO
    }

    void fastconv_parse_apply(float *dst, float *tmp, const float *c, const float *src, size_t rank)
    {
        // TODO
    }
}

#endif /* DSP_ARCH_X86_AVX_FASTCONV_H_ */
