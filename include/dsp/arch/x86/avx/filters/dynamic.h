/*
 * dynamic.h
 *
 *  Created on: 15 февр. 2018 г.
 *      Author: sadko
 */

#ifndef DSP_ARCH_X86_AVX_DYN_FILTERS_H_
#define DSP_ARCH_X86_AVX_DYN_FILTERS_H_

#ifndef DSP_ARCH_X86_AVX_IMPL
    #error "This header should not be included directly"
#endif /* DSP_ARCH_X86_AVX_IMPL */

namespace avx
{
    // This function is tested, works and delivers high performance
    void x64_dyn_biquad_process_x8(float *dst, const float *src, float *d, size_t count, const biquad_x8_t *f)
    {
    #ifdef ARCH_X86_64_AVX
        size_t mask;

        ARCH_X86_ASM
        (
            // Check count
            __ASM_EMIT("test            %[count], %[count]")
            __ASM_EMIT("jz              8f")

            // Initialize mask
            // ymm0=tmp, ymm1={s,s2[8]}, ymm2=p1[8], ymm3=p2[8], ymm6=d0[8], ymm7=d1[8], ymm8=mask[8]
            __ASM_EMIT("mov             $1, %[mask]")
            __ASM_EMIT("vmovaps         %[X_MASK], %%ymm8")                             // ymm8     = m
            __ASM_EMIT("vxorps          %%ymm1, %%ymm1, %%ymm1")                        // ymm1     = 0

            // Load delay buffer
            __ASM_EMIT("vmovups         0x00(%[d]), %%ymm6")                            // ymm6     = d0
            __ASM_EMIT("vmovups         0x20(%[d]), %%ymm7")                            // ymm7     = d1

            // Process first 3 steps
            __ASM_EMIT("1:")
            __ASM_EMIT("vmovss          (%[src]), %%xmm0")                              // xmm0     = *src
            __ASM_EMIT("add             $4, %[src]")                                    // src      ++
            __ASM_EMIT("vblendps        $0x01, %%ymm0, %%ymm1, %%ymm1")                 // ymm1     = s
            __ASM_EMIT("vmulps        " DYN_BIQUAD_X8_A1_SOFF "(%[f]), %%ymm1, %%ymm2") // ymm2     = s*a1
            __ASM_EMIT("vmulps        " DYN_BIQUAD_X8_A2_SOFF "(%[f]), %%ymm1, %%ymm3") // ymm3     = s*a2
            __ASM_EMIT("vmulps        " DYN_BIQUAD_X8_A0_SOFF "(%[f]), %%ymm1, %%ymm1") // ymm1     = s*a0
            __ASM_EMIT("vaddps          %%ymm6, %%ymm1, %%ymm1")                        // ymm1     = s*a0+d0 = s2
            __ASM_EMIT("vmulps        " DYN_BIQUAD_X8_B1_SOFF "(%[f]), %%ymm1, %%ymm4") // ymm4     = s2*b1
            __ASM_EMIT("vmulps        " DYN_BIQUAD_X8_B2_SOFF "(%[f]), %%ymm1, %%ymm5") // ymm5     = s2*b2
            __ASM_EMIT("vaddps          %%ymm4, %%ymm2, %%ymm2")                        // ymm2     = s*a1 + s2*b1 = p1
            __ASM_EMIT("vaddps          %%ymm5, %%ymm3, %%ymm3")                        // ymm3     = s*a2 + s2*b2 = p2
            __ASM_EMIT("vaddps          %%ymm7, %%ymm2, %%ymm2")                        // ymm2     = p1 + d1

            // Update delay only by mask
            __ASM_EMIT("vblendvps       %%ymm8, %%ymm2, %%ymm6, %%ymm6")                // ymm6     = (p1 + d1) & MASK | (d0 & ~MASK)
            __ASM_EMIT("vblendvps       %%ymm8, %%ymm3, %%ymm7, %%ymm7")                // ymm7     = (p2 & MASK) | (d1 & ~MASK)

            // Rotate buffer, AVX has better option for it
            __ASM_EMIT("vpermilps       $0x93, %%ymm1, %%ymm1")                         // ymm1     = s2[3] s2[0] s2[1] s2[2] s2[7] s2[4] s2[5] s2[6]
            __ASM_EMIT("vextractf128    $0x01, %%ymm1, %%xmm0")                         // ymm0     = s2[7] s2[4] s2[5] s2[6] ? ? ? ?
            __ASM_EMIT("vinsertf128     $0x01, %%xmm1, %%ymm0, %%ymm0")                 // ymm0     = s2[7] s2[4] s2[5] s2[6] s2[3] s2[0] s2[1] s2[2]
            __ASM_EMIT("vblendps        $0x11, %%ymm0, %%ymm1, %%ymm1")                 // ymm1     = s2[7] s2[0] s2[1] s2[2] s2[3] s2[4] s2[5] s2[6]

            // Repeat loop
            __ASM_EMIT("add          $" DYN_BIQUAD_X8_SSIZE ", %[f]")                   // f++
            __ASM_EMIT("dec             %[count]")
            __ASM_EMIT("jz              4f")                                            // jump to completion
            __ASM_EMIT("lea             0x01(,%[mask], 2), %[mask]")                    // mask     = (mask << 1) | 1
            __ASM_EMIT("vpermilps       $0x93, %%ymm8, %%ymm8")                         // ymm8     =  m[3]  m[0]  m[1]  m[2]  m[7]  m[4]  m[5]  m[6]
            __ASM_EMIT("vextractf128    $0x01, %%ymm8, %%xmm3")                         // ymm3     =  m[7]  m[4]  m[5]  m[6] ? ? ? ?
            __ASM_EMIT("vinsertf128     $0x01, %%xmm8, %%ymm3, %%ymm3")                 // ymm3     =  m[7]  m[4]  m[5]  m[6]  m[3]  m[0]  m[1]  m[2]
            __ASM_EMIT("vblendps        $0x11, %%ymm3, %%ymm8, %%ymm8")                 // ymm8     =  m[7]  m[0]  m[1]  m[2]  m[3]  m[4]  m[5]  m[6]
            __ASM_EMIT("vorps           %[X_MASK], %%ymm8, %%ymm8")                     // ymm8     =  m[0]  m[0]  m[1]  m[2]  m[3]  m[4]  m[5]  m[6]
            __ASM_EMIT("cmp             $0xff, %[mask]")
            __ASM_EMIT("jne             1b")

            // 8x filter processing without mask
            __ASM_EMIT(".align 16")
            __ASM_EMIT("3:")
            __ASM_EMIT("vmovss          (%[src]), %%xmm0")                              // xmm0     = *src
            __ASM_EMIT("add             $4, %[src]")                                    // src      ++
            __ASM_EMIT("vblendps        $0x01, %%ymm0, %%ymm1, %%ymm1")                 // ymm1     = s
            __ASM_EMIT("vmulps        " DYN_BIQUAD_X8_A1_SOFF "(%[f]), %%ymm1, %%ymm2") // ymm2     = s*a1
            __ASM_EMIT("vmulps        " DYN_BIQUAD_X8_A2_SOFF "(%[f]), %%ymm1, %%ymm3") // ymm3     = s*a2
            __ASM_EMIT("vmulps        " DYN_BIQUAD_X8_A0_SOFF "(%[f]), %%ymm1, %%ymm1") // ymm1     = s*a0
            __ASM_EMIT("vaddps          %%ymm6, %%ymm1, %%ymm1")                        // ymm1     = s*a0+d0 = s2
            __ASM_EMIT("vmulps        " DYN_BIQUAD_X8_B1_SOFF "(%[f]), %%ymm1, %%ymm4") // ymm4     = s2*b1
            __ASM_EMIT("vmulps        " DYN_BIQUAD_X8_B2_SOFF "(%[f]), %%ymm1, %%ymm5") // ymm5     = s2*b2
            __ASM_EMIT("vaddps          %%ymm4, %%ymm2, %%ymm2")                        // ymm2     = s*a1 + s2*b1 = p1
            __ASM_EMIT("vaddps          %%ymm7, %%ymm2, %%ymm6")                        // ymm6     = p1 + d1
            __ASM_EMIT("vaddps          %%ymm5, %%ymm3, %%ymm7")                        // ymm7     = s*a2 + s2*b2 = p2

            // Rotate buffer, AVX2 has better option for it
            __ASM_EMIT("vpermilps       $0x93, %%ymm1, %%ymm1")                         // ymm1     = s2[3] s2[0] s2[1] s2[2] s2[7] s2[4] s2[5] s2[6]
            __ASM_EMIT("vextractf128    $0x01, %%ymm1, %%xmm0")                         // ymm0     = s2[7] s2[4] s2[5] s2[6] ? ? ? ?
            __ASM_EMIT("vinsertf128     $0x01, %%xmm1, %%ymm0, %%ymm0")                 // ymm0     = s2[7] s2[4] s2[5] s2[6] s2[3] s2[0] s2[1] s2[2]
            __ASM_EMIT("vblendps        $0x11, %%ymm0, %%ymm1, %%ymm1")                 // ymm1     = s2[7] s2[0] s2[1] s2[2] s2[3] s2[4] s2[5] s2[6]
            __ASM_EMIT("vmovss          %%xmm1, (%[dst])")                              // *dst     = s2[7]

            // Repeat loop
            __ASM_EMIT("add          $" DYN_BIQUAD_X8_SSIZE ", %[f]")                   // f++
            __ASM_EMIT("add             $4, %[dst]")                                    // dst      ++
            __ASM_EMIT("dec             %[count]")
            __ASM_EMIT("jnz             3b")

            // Prepare last loop, shift mask
            __ASM_EMIT("4:")
            __ASM_EMIT("vxorps          %%ymm2, %%ymm2, %%ymm2")                        // ymm2     =  0
            __ASM_EMIT("vpermilps       $0x93, %%ymm8, %%ymm8")                         // ymm8     =  m[3]  m[0]  m[1]  m[2]  m[7]  m[4]  m[5]  m[6]
            __ASM_EMIT("vextractf128    $0x01, %%ymm8, %%xmm9")                         // ymm9     =  m[7]  m[4]  m[5]  m[6] ? ? ? ?
            __ASM_EMIT("vinsertf128     $0x01, %%xmm8, %%ymm2, %%ymm2")                 // ymm2     =  0     0     0     0     m[3]  m[0]  m[1]  m[2]
            __ASM_EMIT("vblendps        $0x11, %%ymm2, %%ymm8, %%ymm8")                 // ymm8     =  0     m[0]  m[1]  m[2]  m[3]  m[4]  m[5]  m[6]
            __ASM_EMIT("shl             $1, %[mask]")                                   // mask     = mask << 1

            // Process steps
            __ASM_EMIT(".align 16")
            __ASM_EMIT("5:")
            __ASM_EMIT("vmulps        " DYN_BIQUAD_X8_A1_SOFF "(%[f]), %%ymm1, %%ymm2") // ymm2     = s*a1
            __ASM_EMIT("vmulps        " DYN_BIQUAD_X8_A2_SOFF "(%[f]), %%ymm1, %%ymm3") // ymm3     = s*a2
            __ASM_EMIT("vmulps        " DYN_BIQUAD_X8_A0_SOFF "(%[f]), %%ymm1, %%ymm1") // ymm1     = s*a0
            __ASM_EMIT("vaddps          %%ymm6, %%ymm1, %%ymm1")                        // ymm1     = s*a0+d0 = s2
            __ASM_EMIT("vmulps        " DYN_BIQUAD_X8_B1_SOFF "(%[f]), %%ymm1, %%ymm4") // ymm4     = s2*b1
            __ASM_EMIT("vmulps        " DYN_BIQUAD_X8_B2_SOFF "(%[f]), %%ymm1, %%ymm5") // ymm5     = s2*b2
            __ASM_EMIT("vaddps          %%ymm4, %%ymm2, %%ymm2")                        // ymm2     = s*a1 + s2*b1 = p1
            __ASM_EMIT("vaddps          %%ymm5, %%ymm3, %%ymm3")                        // ymm3     = s*a2 + s2*b2 = p2
            __ASM_EMIT("vaddps          %%ymm7, %%ymm2, %%ymm2")                        // ymm2     = p1 + d1

            // Update delay only by mask
            __ASM_EMIT("vblendvps       %%ymm8, %%ymm2, %%ymm6, %%ymm6")                // ymm6     = (p1 + d1) & MASK | (d0 & ~MASK)
            __ASM_EMIT("vblendvps       %%ymm8, %%ymm3, %%ymm7, %%ymm7")                // ymm7     = (p2 & MASK) | (d1 & ~MASK)

            // Rotate buffer and mask, AVX2 has better option for it
            __ASM_EMIT("add          $" DYN_BIQUAD_X8_SSIZE ", %[f]")                   // f++
            __ASM_EMIT("vpermilps       $0x93, %%ymm1, %%ymm1")                         // ymm1     = s2[3] s2[0] s2[1] s2[2] s2[7] s2[4] s2[5] s2[6]
            __ASM_EMIT("vpermilps       $0x93, %%ymm8, %%ymm8")                         // ymm8     =  m[3]  m[0]  m[1]  m[2]  m[7]  m[4]  m[5]  m[6]
            __ASM_EMIT("vextractf128    $0x01, %%ymm1, %%xmm0")                         // ymm0     = s2[7] s2[4] s2[5] s2[6] ? ? ? ?
            __ASM_EMIT("vxorps          %%ymm2, %%ymm2, %%ymm2")                        // ymm2     = 0
            __ASM_EMIT("vinsertf128     $0x01, %%xmm1, %%ymm0, %%ymm0")                 // ymm0     = s2[7] s2[4] s2[5] s2[6] s2[3] s2[0] s2[1] s2[2]
            __ASM_EMIT("vinsertf128     $0x01, %%xmm8, %%ymm2, %%ymm2")                 // ymm2     =  0     0     0     0     m[3]  m[0]  m[1]  m[2]
            __ASM_EMIT("vblendps        $0x11, %%ymm0, %%ymm1, %%ymm1")                 // ymm1     = s2[7] s2[0] s2[1] s2[2] s2[3] s2[4] s2[5] s2[6]
            __ASM_EMIT("vblendps        $0x11, %%ymm2, %%ymm8, %%ymm8")                 // ymm1     =  0     m[0]  m[1]  m[2]  m[3]  m[4]  m[5]  m[6]
            __ASM_EMIT("test            $0x80, %[mask]")
            __ASM_EMIT("jz              6f")
            __ASM_EMIT("vmovss          %%xmm1, (%[dst])")                              // *dst     = s2[7]
            __ASM_EMIT("add             $4, %[dst]")                                    // dst      ++
            __ASM_EMIT("6:")

            // Repeat loop
            __ASM_EMIT("shl             $1, %[mask]")                                   // mask     = mask << 1
            __ASM_EMIT("and             $0xff, %[mask]")                                // mask     = (mask << 1) & 0xff
            __ASM_EMIT("jnz             5b")                                            // check that mask is not zero

            // Store delay buffer
            __ASM_EMIT("vmovups         %%ymm6, 0x00(%[d])")                            // *d0      = %%ymm6
            __ASM_EMIT("vmovups         %%ymm7, 0x20(%[d])")                            // *d1      = &&ymm7

            // Exit label
            __ASM_EMIT("vzeroupper")
            __ASM_EMIT("8:")

            : [dst] "+r" (dst), [src] "+r" (src), [count] "+r" (count), [f] "+r" (f),
              [mask] "=&r"(mask)
            : [d] "r" (d),
              [X_MASK] "m" (X_MASK0001)
            : "cc", "memory",
              "%xmm0", "%xmm1", "%xmm2", "%xmm3", "%xmm4", "%xmm5", "%xmm6", "%xmm7"
        );
    #endif /* ARCH_X86_64_AVX */
    }

    // This function is FMA3 implementation of biquad_process_x8
    void x64_dyn_biquad_process_x8_fma3(float *dst, const float *src, float *d, size_t count, const biquad_x8_t *f)
    {
    #ifdef ARCH_X86_64_AVX
        size_t mask;

        ARCH_X86_ASM
        (
            // Check count
            __ASM_EMIT("test            %[count], %[count]")
            __ASM_EMIT("jz              8f")

            // Initialize mask
            // ymm0=tmp, ymm1={s,s2[8]}, ymm2=p1[8], ymm3=p2[8], ymm6=d0[8], ymm7=d1[8], ymm8=mask[8]
            __ASM_EMIT("mov             $1, %[mask]")
            __ASM_EMIT("vmovaps         %[X_MASK], %%ymm8")                             // ymm8     = m
            __ASM_EMIT("vxorps          %%ymm1, %%ymm1, %%ymm1")                        // ymm1     = 0

            // Load delay buffer
            __ASM_EMIT("vmovups         0x00(%[d]), %%ymm6")                            // ymm6     = d0
            __ASM_EMIT("vmovups         0x20(%[d]), %%ymm7")                            // ymm7     = d1

            // Process first 3 steps
            __ASM_EMIT("1:")
            __ASM_EMIT("vmovss          (%[src]), %%xmm0")                              // xmm0     = *src
            __ASM_EMIT("add             $4, %[src]")                                    // src      ++
            __ASM_EMIT("vblendps        $0x01, %%ymm0, %%ymm1, %%ymm1")                 // ymm1     = s
            __ASM_EMIT("vmulps        " DYN_BIQUAD_X8_A1_SOFF "(%[f]), %%ymm1, %%ymm2") // ymm2     = s*a1
            __ASM_EMIT("vmulps        " DYN_BIQUAD_X8_A2_SOFF "(%[f]), %%ymm1, %%ymm3") // ymm3     = s*a2
            __ASM_EMIT("vfmadd132ps   " DYN_BIQUAD_X8_A0_SOFF "(%[f]), %%ymm6, %%ymm1") // ymm1     = s*a0+d0 = s2
            __ASM_EMIT("vfmadd231ps   " DYN_BIQUAD_X8_B1_SOFF "(%[f]), %%ymm1, %%ymm2") // ymm2     = s*a1 + s2*b1 = p1
            __ASM_EMIT("vfmadd231ps   " DYN_BIQUAD_X8_B2_SOFF "(%[f]), %%ymm1, %%ymm3") // ymm3     = s*a2 + s2*b2 = p2
            __ASM_EMIT("vaddps          %%ymm7, %%ymm2, %%ymm2")                        // ymm2     = p1 + d1

            // Update delay only by mask
            __ASM_EMIT("vblendvps       %%ymm8, %%ymm2, %%ymm6, %%ymm6")                // ymm6     = (p1 + d1) & MASK | (d0 & ~MASK)
            __ASM_EMIT("vblendvps       %%ymm8, %%ymm3, %%ymm7, %%ymm7")                // ymm7     = (p2 & MASK) | (d1 & ~MASK)

            // Rotate buffer, AVX has better option for it
            __ASM_EMIT("vpermilps       $0x93, %%ymm1, %%ymm1")                         // ymm1     = s2[3] s2[0] s2[1] s2[2] s2[7] s2[4] s2[5] s2[6]
            __ASM_EMIT("vextractf128    $0x01, %%ymm1, %%xmm0")                         // ymm0     = s2[7] s2[4] s2[5] s2[6] ? ? ? ?
            __ASM_EMIT("vinsertf128     $0x01, %%xmm1, %%ymm0, %%ymm0")                 // ymm0     = s2[7] s2[4] s2[5] s2[6] s2[3] s2[0] s2[1] s2[2]
            __ASM_EMIT("vblendps        $0x11, %%ymm0, %%ymm1, %%ymm1")                 // ymm1     = s2[7] s2[0] s2[1] s2[2] s2[3] s2[4] s2[5] s2[6]

            // Repeat loop
            __ASM_EMIT("dec             %[count]")
            __ASM_EMIT("jz              4f")                                            // jump to completion
            __ASM_EMIT("lea             0x01(,%[mask], 2), %[mask]")                    // mask     = (mask << 1) | 1
            __ASM_EMIT("vpermilps       $0x93, %%ymm8, %%ymm8")                         // ymm8     =  m[3]  m[0]  m[1]  m[2]  m[7]  m[4]  m[5]  m[6]
            __ASM_EMIT("vextractf128    $0x01, %%ymm8, %%xmm3")                         // ymm3     =  m[7]  m[4]  m[5]  m[6] ? ? ? ?
            __ASM_EMIT("vinsertf128     $0x01, %%xmm8, %%ymm3, %%ymm3")                 // ymm3     =  m[7]  m[4]  m[5]  m[6]  m[3]  m[0]  m[1]  m[2]
            __ASM_EMIT("vblendps        $0x11, %%ymm3, %%ymm8, %%ymm8")                 // ymm8     =  m[7]  m[0]  m[1]  m[2]  m[3]  m[4]  m[5]  m[6]
            __ASM_EMIT("vorps           %[X_MASK], %%ymm8, %%ymm8")                     // ymm8     =  m[0]  m[0]  m[1]  m[2]  m[3]  m[4]  m[5]  m[6]
            __ASM_EMIT("cmp             $0xff, %[mask]")
            __ASM_EMIT("jne             1b")

            // 8x filter processing without mask
            __ASM_EMIT(".align 16")
            __ASM_EMIT("3:")
            __ASM_EMIT("vmovss          (%[src]), %%xmm0")                              // xmm0     = *src
            __ASM_EMIT("add             $4, %[src]")                                    // src      ++
            __ASM_EMIT("vblendps        $0x01, %%ymm0, %%ymm1, %%ymm1")                 // ymm1     = s
            __ASM_EMIT("vmulps        " DYN_BIQUAD_X8_A1_SOFF "(%[f]), %%ymm1, %%ymm2") // ymm2     = s*a1
            __ASM_EMIT("vmulps        " DYN_BIQUAD_X8_A2_SOFF "(%[f]), %%ymm1, %%ymm3") // ymm3     = s*a2
            __ASM_EMIT("vfmadd132ps   " DYN_BIQUAD_X8_A0_SOFF "(%[f]), %%ymm6, %%ymm1") // ymm1     = s*a0+d0 = s2
            __ASM_EMIT("vfmadd231ps   " DYN_BIQUAD_X8_B1_SOFF "(%[f]), %%ymm1, %%ymm2") // ymm2     = s*a1 + s2*b1 = p1
            __ASM_EMIT("vfmadd231ps   " DYN_BIQUAD_X8_B2_SOFF "(%[f]), %%ymm1, %%ymm3") // ymm3     = s*a2 + s2*b2 = p2
            __ASM_EMIT("vaddps          %%ymm7, %%ymm2, %%ymm6")                        // ymm6     = p1 + d1
            __ASM_EMIT("vmovaps         %%ymm3, %%ymm7")                                // ymm7     = s*a2 + s2*b2 = p2

            // Rotate buffer, AVX2 has better option for it
            __ASM_EMIT("vpermilps       $0x93, %%ymm1, %%ymm1")                         // ymm1     = s2[3] s2[0] s2[1] s2[2] s2[7] s2[4] s2[5] s2[6]
            __ASM_EMIT("vextractf128    $0x01, %%ymm1, %%xmm0")                         // ymm0     = s2[7] s2[4] s2[5] s2[6] ? ? ? ?
            __ASM_EMIT("vinsertf128     $0x01, %%xmm1, %%ymm0, %%ymm0")                 // ymm0     = s2[7] s2[4] s2[5] s2[6] s2[3] s2[0] s2[1] s2[2]
            __ASM_EMIT("vblendps        $0x11, %%ymm0, %%ymm1, %%ymm1")                 // ymm1     = s2[7] s2[0] s2[1] s2[2] s2[3] s2[4] s2[5] s2[6]
            __ASM_EMIT("vmovss          %%xmm1, (%[dst])")                              // *dst     = s2[7]

            // Repeat loop
            __ASM_EMIT("add             $4, %[dst]")                                    // dst      ++
            __ASM_EMIT("dec             %[count]")
            __ASM_EMIT("jnz             3b")

            // Prepare last loop, shift mask
            __ASM_EMIT("4:")
            __ASM_EMIT("vxorps          %%ymm2, %%ymm2, %%ymm2")                        // ymm2     =  0
            __ASM_EMIT("vpermilps       $0x93, %%ymm8, %%ymm8")                         // ymm8     =  m[3]  m[0]  m[1]  m[2]  m[7]  m[4]  m[5]  m[6]
            __ASM_EMIT("vextractf128    $0x01, %%ymm8, %%xmm9")                         // ymm9     =  m[7]  m[4]  m[5]  m[6] ? ? ? ?
            __ASM_EMIT("vinsertf128     $0x01, %%xmm8, %%ymm2, %%ymm2")                 // ymm2     =  0     0     0     0     m[3]  m[0]  m[1]  m[2]
            __ASM_EMIT("vblendps        $0x11, %%ymm2, %%ymm8, %%ymm8")                 // ymm8     =  0     m[0]  m[1]  m[2]  m[3]  m[4]  m[5]  m[6]
            __ASM_EMIT("shl             $1, %[mask]")                                   // mask     = mask << 1

            // Process steps
            __ASM_EMIT("5:")
            __ASM_EMIT("vmulps        " DYN_BIQUAD_X8_A1_SOFF "(%[f]), %%ymm1, %%ymm2") // ymm2     = s*a1
            __ASM_EMIT("vmulps        " DYN_BIQUAD_X8_A2_SOFF "(%[f]), %%ymm1, %%ymm3") // ymm3     = s*a2
            __ASM_EMIT("vfmadd132ps   " DYN_BIQUAD_X8_A0_SOFF "(%[f]), %%ymm6, %%ymm1") // ymm1     = s*a0+d0 = s2
            __ASM_EMIT("vfmadd231ps   " DYN_BIQUAD_X8_B1_SOFF "(%[f]), %%ymm1, %%ymm2") // ymm2     = s*a1 + s2*b1 = p1
            __ASM_EMIT("vfmadd231ps   " DYN_BIQUAD_X8_B2_SOFF "(%[f]), %%ymm1, %%ymm3") // ymm3     = s*a2 + s2*b2 = p2
            __ASM_EMIT("vaddps          %%ymm7, %%ymm2, %%ymm2")                        // ymm2     = p1 + d1

            // Update delay only by mask
            __ASM_EMIT("vblendvps       %%ymm8, %%ymm2, %%ymm6, %%ymm6")                // ymm6     = (p1 + d1) & MASK | (d0 & ~MASK)
            __ASM_EMIT("vblendvps       %%ymm8, %%ymm3, %%ymm7, %%ymm7")                // ymm7     = (p2 & MASK) | (d1 & ~MASK)

            // Rotate buffer and mask, AVX2 has better option for it
            __ASM_EMIT("vpermilps       $0x93, %%ymm1, %%ymm1")                         // ymm1     = s2[3] s2[0] s2[1] s2[2] s2[7] s2[4] s2[5] s2[6]
            __ASM_EMIT("vpermilps       $0x93, %%ymm8, %%ymm8")                         // ymm8     =  m[3]  m[0]  m[1]  m[2]  m[7]  m[4]  m[5]  m[6]
            __ASM_EMIT("vextractf128    $0x01, %%ymm1, %%xmm0")                         // ymm0     = s2[7] s2[4] s2[5] s2[6] ? ? ? ?
            __ASM_EMIT("vxorps          %%ymm2, %%ymm2, %%ymm2")                        // ymm2     = 0
            __ASM_EMIT("vinsertf128     $0x01, %%xmm1, %%ymm0, %%ymm0")                 // ymm0     = s2[7] s2[4] s2[5] s2[6] s2[3] s2[0] s2[1] s2[2]
            __ASM_EMIT("vinsertf128     $0x01, %%xmm8, %%ymm2, %%ymm2")                 // ymm2     =  0     0     0     0     m[3]  m[0]  m[1]  m[2]
            __ASM_EMIT("vblendps        $0x11, %%ymm0, %%ymm1, %%ymm1")                 // ymm1     = s2[7] s2[0] s2[1] s2[2] s2[3] s2[4] s2[5] s2[6]
            __ASM_EMIT("vblendps        $0x11, %%ymm2, %%ymm8, %%ymm8")                 // ymm1     =  0     m[0]  m[1]  m[2]  m[3]  m[4]  m[5]  m[6]
            __ASM_EMIT("test            $0x80, %[mask]")
            __ASM_EMIT("jz              6f")
            __ASM_EMIT("vmovss          %%xmm1, (%[dst])")                              // *dst     = s2[7]
            __ASM_EMIT("add             $4, %[dst]")                                    // dst      ++
            __ASM_EMIT("6:")

            // Repeat loop
            __ASM_EMIT("shl             $1, %[mask]")                                   // mask     = mask << 1
            __ASM_EMIT("and             $0xff, %[mask]")                                // mask     = (mask << 1) & 0xff
            __ASM_EMIT("jnz             5b")                                            // check that mask is not zero

            // Store delay buffer
            __ASM_EMIT("vmovups         %%ymm6, 0x00(%[d])")                            // *d0      = %%ymm6
            __ASM_EMIT("vmovups         %%ymm7, 0x20(%[d])")                            // *d1      = %%ymm7

            // Exit label
            __ASM_EMIT("vzeroupper")
            __ASM_EMIT("8:")

            : [dst] "+r" (dst), [src] "+r" (src), [count] "+r" (count), [f] "+r" (f),
              [mask] "=&r"(mask)
            : [d] "r" (d),
              [X_MASK] "m" (X_MASK0001)
            : "cc", "memory",
              "%xmm0", "%xmm1", "%xmm2", "%xmm3", "%xmm4", "%xmm5", "%xmm6", "%xmm7"
        );
        #endif /* ARCH_X86_64_AVX */
    }

}

#endif /* DSP_ARCH_X86_AVX_DYN_FILTERS_H_ */
