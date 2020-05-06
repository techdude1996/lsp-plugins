/*
 * resampling.h
 *
 *  Created on: 19 нояб. 2016 г.
 *      Author: sadko
 */

#ifndef DSP_ARCH_X86_SSE_RESAMPLING_H_
#define DSP_ARCH_X86_SSE_RESAMPLING_H_

#ifndef DSP_ARCH_X86_SSE_IMPL
    #error "This header should not be included directly"
#endif /* DSP_ARCH_X86_SSE_IMPL */

namespace sse
{
    IF_ARCH_X86(
        static const float lanczos_2x2[] __lsp_aligned16 =
        {
            +0.5731591682507563f,   // k0
            -0.0636843520278618f    // k1
        };

        // Lanczos kernel 2x3: 3 SSE registers
        static const float lanczos_2x3[] __lsp_aligned16 =
        {
            +0.6079271018540265f,   // k0
            -0.1350949115231170f,   // k1
            +0.0243170840741611f    // k2
        };

        // Lanczos kernel 3x2: 6 SSE registers
        static const float lanczos_kernel_3x2[] __lsp_aligned16 =
        {
            +0.0000000000000000f,
            -0.0315888188312782f,
            -0.0854897486982225f,
            +0.0000000000000000f,

            +0.3419589947928900f,
            +0.7897204707819555f,
            +1.0000000000000000f,
            +0.7897204707819555f,

            +0.3419589947928900f,
            +0.0000000000000000f,
            -0.0854897486982225f,
            -0.0315888188312782f,

            // Shifted by 1 left
            -0.0315888188312782f,
            -0.0854897486982225f,
            +0.0000000000000000f,
            +0.3419589947928900f,

            +0.7897204707819555f,
            +1.0000000000000000f,
            +0.7897204707819555f,
            +0.3419589947928900f,

            +0.0000000000000000f,
            -0.0854897486982225f,
            -0.0315888188312782f,
            +0.0000000000000000f
        };


        // Lanczos kernel 3x3: 9 SSE registers
        static const float lanczos_kernel_3x3[] __lsp_aligned16 =
        {
            +0.0000000000000000f,
            +0.0126609519658153f,
            +0.0310789306368038f,
            +0.0000000000000000f,

            -0.0933267410806225f,
            -0.1458230329384726f,
            +0.0000000000000000f,
            +0.3807169003008463f,

            +0.8103009258121772f,
            +1.0000000000000000f,
            +0.8103009258121772f,
            +0.3807169003008463f,

            +0.0000000000000000f,
            -0.1458230329384726f,
            -0.0933267410806225f,
            +0.0000000000000000f,

            +0.0310789306368038f,
            +0.0126609519658153f,
            +0.0000000000000000f,
            +0.0000000000000000f,

            // Shifted by 1 left
            +0.0126609519658153f,
            +0.0310789306368038f,
            +0.0000000000000000f,
            -0.0933267410806225f,

            -0.1458230329384726f,
            +0.0000000000000000f,
            +0.3807169003008463f,
            +0.8103009258121772f,

            +1.0000000000000000f,
            +0.8103009258121772f,
            +0.3807169003008463f,
            +0.0000000000000000f,

            -0.1458230329384726f,
            -0.0933267410806225f,
            -0.0000000000000000f,
            +0.0310789306368038f,

            +0.0126609519658153f,
            +0.0000000000000000f,
            +0.0000000000000000f,
            +0.0000000000000000f
        };

        // Lanczos kernel 4x2: 4 SSE registers
        static const float lanczos_kernel_4x2[] __lsp_aligned16 =
        {
            +0.0000000000000000f,
            -0.0179051851263444f,
            -0.0636843520278618f,
            -0.0847248039068907f,

            +0.0000000000000000f,
            +0.2353466775191407f,
            +0.5731591682507563f,
            +0.8773540711908775f,

            +1.0000000000000000f,
            +0.8773540711908775f,
            +0.5731591682507563f,
            +0.2353466775191407f,

            +0.0000000000000000f,
            -0.0847248039068907f,
            -0.0636843520278618f,
            -0.0179051851263444f
        };

        // Lanczos kernel 4x3: 6 SSE registers
        static const float lanczos_kernel_4x3[] __lsp_aligned16 =
        {
            +0.0000000000000000f,
            +0.0073559260471942f,
            +0.0243170840741611f,
            +0.0300210914495816f,

            +0.0000000000000000f,
            -0.0677913359005429f,
            -0.1350949115231170f,
            -0.1328710183650640f,

            +0.0000000000000000f,
            +0.2701898230462341f,
            +0.6079271018540265f,
            +0.8900670517104946f,

            +1.0000000000000000f,
            +0.8900670517104946f,
            +0.6079271018540265f,
            +0.2701898230462341f,

            +0.0000000000000000f,
            -0.1328710183650640f,
            -0.1350949115231170f,
            -0.0677913359005429f,

            +0.0000000000000000f,
            +0.0300210914495816f,
            +0.0243170840741611f,
            +0.0073559260471942f
        };

        // Lanczos kernel 6x2: 6 SSE registers
        static const float lanczos_kernel_6x2[] __lsp_aligned16 =
        {
            +0.0000000000000000f,
            -0.0078021377848166f,
            -0.0315888188312782f,
            -0.0636843520278618f,

            -0.0854897486982225f,
            -0.0719035699814534f,
            +0.0000000000000000f,
            +0.1409309971636486f,

            +0.3419589947928900f,
            +0.5731591682507563f,
            +0.7897204707819555f,
            +0.9440586719628122f,

            +1.0000000000000000f,
            +0.9440586719628122f,
            +0.7897204707819555f,
            +0.5731591682507563f,

            +0.3419589947928900f,
            +0.1409309971636486f,
            +0.0000000000000000f,
            -0.0719035699814534f,

            -0.0854897486982225f,
            -0.0636843520278618f,
            -0.0315888188312782f,
            -0.0078021377848166f
        };

        // Lanczos kernel 6x3: 9 SSE registers
        static const float lanczos_kernel_6x3[] __lsp_aligned16 =
        {
            +0.0000000000000000f,
            +0.0032875048460955f,
            +0.0126609519658153f,
            +0.0243170840741611f,

            +0.0310789306368038f,
            +0.0248005479513036f,
            -0.0000000000000000f,
            -0.0424907562338176f,

            -0.0933267410806225f,
            -0.1350949115231170f,
            -0.1458230329384726f,
            -0.1049261531488149f,

            +0.0000000000000000f,
            +0.1676517041508127f,
            +0.3807169003008463f,
            +0.6079271018540265f,

            +0.8103009258121772f,
            +0.9500889005216107f,
            +1.0000000000000000f,
            +0.9500889005216107f,

            +0.8103009258121772f,
            +0.6079271018540265f,
            +0.3807169003008463f,
            +0.1676517041508127f,

            +0.0000000000000000f,
            -0.1049261531488149f,
            -0.1458230329384726f,
            -0.1350949115231170f,

            -0.0933267410806225f,
            -0.0424907562338176f,
            -0.0000000000000000f,
            +0.0248005479513036f,

            +0.0310789306368038f,
            +0.0243170840741611f,
            +0.0126609519658153f,
            +0.0032875048460955f
        };

        // Lanczos kernel 8x2: 8 SSE registers
        static const float lanczos_kernel_8x2[] __lsp_aligned16 =
        {
            +0.0000000000000000f,
            -0.0043033145538298f,
            -0.0179051851263444f,
            -0.0393892611124141f,

            -0.0636843520278618f,
            -0.0823353965569232f,
            -0.0847248039068907f,
            -0.0600950644541902f,

            +0.0000000000000000f,
            +0.0993408208324369f,
            +0.2353466775191407f,
            +0.3985033193355084f,

            +0.5731591682507563f,
            +0.7396427919997760f,
            +0.8773540711908775f,
            +0.9682457746117045f,

            +1.0000000000000000f,
            +0.9682457746117045f,
            +0.8773540711908775f,
            +0.7396427919997760f,

            +0.5731591682507563f,
            +0.3985033193355084f,
            +0.2353466775191407f,
            +0.0993408208324369f,

            +0.0000000000000000f,
            -0.0600950644541902f,
            -0.0847248039068907f,
            -0.0823353965569232f,

            -0.0636843520278618f,
            -0.0393892611124141f,
            -0.0179051851263444f,
            -0.0043033145538298f
        };

        // Lanczos kernel 8x3: 12 SSE registers
        static const float lanczos_kernel_8x3[] __lsp_aligned16 =
        {
            +0.0000000000000000f,
            +0.0018368899607481f,
            +0.0073559260471942f,
            +0.0155961678435580f,

            +0.0243170840741611f,
            +0.0303079634725070f,
            +0.0300210914495816f,
            +0.0204366616947175f,

            +0.0000000000000000f,
            -0.0305684889733737f,
            -0.0677913359005429f,
            -0.1054383717904384f,

            -0.1350949115231170f,
            -0.1472651639056537f,
            -0.1328710183650640f,
            -0.0849124693704824f,

            +0.0000000000000000f,
            +0.1205345965259870f,
            +0.2701898230462341f,
            +0.4376469925430009f,

            +0.6079271018540265f,
            +0.7642122243343417f,
            +0.8900670517104946f,
            +0.9717147892357163f,

            +1.0000000000000000f,
            +0.9717147892357163f,
            +0.8900670517104946f,
            +0.7642122243343417f,

            +0.6079271018540265f,
            +0.4376469925430009f,
            +0.2701898230462341f,
            +0.1205345965259870f,

            +0.0000000000000000f,
            -0.0849124693704824f,
            -0.1328710183650640f,
            -0.1472651639056537f,

            -0.1350949115231170f,
            -0.1054383717904384f,
            -0.0677913359005429f,
            -0.0305684889733737f,

            +0.0000000000000000f,
            +0.0204366616947175f,
            +0.0300210914495816f,
            +0.0303079634725070f,

            +0.0243170840741611f,
            +0.0155961678435580f,
            +0.0073559260471942f,
            +0.0018368899607481f
        };
    )

    void lanczos_resample_2x2(float *dst, const float *src, size_t count)
    {
        ARCH_X86_ASM
        (
            __ASM_EMIT("movss       0x00 + %[k], %%xmm6")    // xmm6 = k0
            __ASM_EMIT("movss       0x04 + %[k], %%xmm7")    // xmm7 = k1
            __ASM_EMIT("sub         $2, %[count]")
            __ASM_EMIT("jb          2f")

            // Load samples
            __ASM_EMIT(".align 16")
            __ASM_EMIT("1:")
            __ASM_EMIT("movss       0x00(%[src]), %%xmm0")  // xmm0 = s0
            __ASM_EMIT("movss       0x04(%[src]), %%xmm1")  // xmm1 = s1

            // Do convolution
            __ASM_EMIT("movaps      %%xmm0, %%xmm2")        // xmm2 = s0
            __ASM_EMIT("movaps      %%xmm1, %%xmm3")        // xmm3 = s1
            __ASM_EMIT("mulss       %%xmm7, %%xmm2")        // xmm2 = k1*s0
            __ASM_EMIT("mulss       %%xmm7, %%xmm3")        // xmm3 = k1*s1
            __ASM_EMIT("movaps      %%xmm2, %%xmm4")        // xmm4 = k1*s0
            __ASM_EMIT("movaps      %%xmm3, %%xmm5")        // xmm5 = k1*s1
            __ASM_EMIT("addss       0x04(%[dst]), %%xmm4")  // xmm4 = k1*s0 + d1
            __ASM_EMIT("addss       0x24(%[dst]), %%xmm5")  // xmm5 = k1*s1 + d9
            __ASM_EMIT("movss       %%xmm4, 0x04(%[dst])")  // d1 += k1*s0
            __ASM_EMIT("movss       %%xmm5, 0x24(%[dst])")  // d9 += k1*s1

            __ASM_EMIT("movaps      %%xmm0, %%xmm4")        // xmm4 = s0
            __ASM_EMIT("movaps      %%xmm1, %%xmm5")        // xmm5 = s1
            __ASM_EMIT("mulss       %%xmm6, %%xmm4")        // xmm4 = k0*s0
            __ASM_EMIT("mulss       %%xmm6, %%xmm5")        // xmm5 = k0*s1
            __ASM_EMIT("addss       %%xmm4, %%xmm3")        // xmm3 = k1*s1 + k0*s0
            __ASM_EMIT("addss       %%xmm5, %%xmm2")        // xmm2 = k1*s0 + k0*s1
            __ASM_EMIT("addss       0x0c(%[dst]), %%xmm3")  // xmm3 = k1*s1 + k0*s0 + d3
            __ASM_EMIT("addss       0x1c(%[dst]), %%xmm2")  // xmm2 = k1*s0 + k0*s1 + d7
            __ASM_EMIT("movss       %%xmm3, 0x0c(%[dst])")  // d3 += k1*s1 + k0*s0
            __ASM_EMIT("movss       %%xmm2, 0x1c(%[dst])")  // d7 += k1*s0 + k0*s1

            __ASM_EMIT("addss       %%xmm5, %%xmm4")        // xmm4 = k0*s0 + k0*s1
            __ASM_EMIT("addss       0x10(%[dst]), %%xmm0")  // xmm0 = s0 + d4
            __ASM_EMIT("addss       0x18(%[dst]), %%xmm1")  // xmm1 = s1 + d6
            __ASM_EMIT("addss       0x14(%[dst]), %%xmm4")  // xmm4 = k0*s0 + k0*s1 + d5
            __ASM_EMIT("movss       %%xmm0, 0x10(%[dst])")  // d4 += s0
            __ASM_EMIT("movss       %%xmm4, 0x14(%[dst])")  // d5 += k0*s0 + k0*s1
            __ASM_EMIT("movss       %%xmm1, 0x18(%[dst])")  // d6 += s1

            // Update pointers
            __ASM_EMIT("add         $0x08, %[src]")
            __ASM_EMIT("add         $0x10, %[dst]")
            __ASM_EMIT("sub         $2, %[count]")
            __ASM_EMIT("jae         1b")

            // Complete loop
            __ASM_EMIT("2:")
            __ASM_EMIT("add         $1, %[count]")
            __ASM_EMIT("jl          3f")

            // Load sample
            __ASM_EMIT("movss       (%[src]), %%xmm0")      // xmm0 = s0

            // Do convolution
            __ASM_EMIT("movaps      %%xmm0, %%xmm1")        // xmm1 = s0
            __ASM_EMIT("movaps      %%xmm0, %%xmm2")        // xmm2 = s0
            __ASM_EMIT("mulss       %%xmm6, %%xmm1")        // xmm1 = k0*s0
            __ASM_EMIT("mulss       %%xmm7, %%xmm2")        // xmm2 = k1*s0
            __ASM_EMIT("movaps      %%xmm1, %%xmm3")        // xmm3 = k0*s0
            __ASM_EMIT("movaps      %%xmm2, %%xmm4")        // xmm4 = k1*s0

            __ASM_EMIT("addss       0x04(%[dst]), %%xmm2")  // xmm2 = k1*s0 + d1
            __ASM_EMIT("addss       0x0c(%[dst]), %%xmm1")  // xmm1 = k0*s0 + d3
            __ASM_EMIT("addss       0x10(%[dst]), %%xmm0")  // xmm0 = s0 + d4
            __ASM_EMIT("addss       0x14(%[dst]), %%xmm3")  // xmm3 = k0*s0 + d5
            __ASM_EMIT("addss       0x1c(%[dst]), %%xmm4")  // xmm4 = k1*s0 + d7

            __ASM_EMIT("movss       %%xmm2, 0x04(%[dst])")  // d1 += k1*s0
            __ASM_EMIT("movss       %%xmm1, 0x0c(%[dst])")  // d3 += k0*s0
            __ASM_EMIT("movss       %%xmm0, 0x10(%[dst])")  // d4 += s0
            __ASM_EMIT("movss       %%xmm3, 0x14(%[dst])")  // d5 += k0*s0
            __ASM_EMIT("movss       %%xmm4, 0x1c(%[dst])")  // d7 += k1*s0

            __ASM_EMIT("3:")

            : [dst] "+r" (dst), [src] "+r" (src), [count] "+r" (count)
            : [k] "o" (lanczos_2x2)
            : "cc", "memory",
              "%xmm0", "%xmm1", "%xmm2", "%xmm3",
              "%xmm4", "%xmm5", "%xmm6", "%xmm7"
        );
    }

    void lanczos_resample_2x3(float *dst, const float *src, size_t count)
    {
        ARCH_X86_ASM
        (
            __ASM_EMIT("movss       0x00 + %[k], %%xmm6")   // xmm6 = k0
            __ASM_EMIT("movss       0x04 + %[k], %%xmm7")   // xmm7 = k1
            __ASM_EMIT("sub         $2, %[count]")
            __ASM_EMIT("jb          2f")

            // Load samples
            __ASM_EMIT(".align 16")
            __ASM_EMIT("1:")
            __ASM_EMIT("movss       0x00(%[src]), %%xmm0")  // xmm0 = s0
            __ASM_EMIT("movss       0x04(%[src]), %%xmm1")  // xmm1 = s1

            // Do convolution
            __ASM_EMIT("movaps      %%xmm0, %%xmm2")        // xmm2 = s0
            __ASM_EMIT("movaps      %%xmm1, %%xmm3")        // xmm3 = s1
            __ASM_EMIT("mulss       0x08 + %[k], %%xmm2")   // xmm2 = k2*s0
            __ASM_EMIT("mulss       0x08 + %[k], %%xmm3")   // xmm3 = k2*s1
            __ASM_EMIT("movaps      %%xmm2, %%xmm4")        // xmm4 = k2*s0
            __ASM_EMIT("movaps      %%xmm3, %%xmm5")        // xmm5 = k2*s1
            __ASM_EMIT("addss       0x04(%[dst]), %%xmm4")  // xmm4 = k2*s0 + d1
            __ASM_EMIT("addss       0x34(%[dst]), %%xmm5")  // xmm5 = k2*s1 + d13
            __ASM_EMIT("movss       %%xmm4, 0x04(%[dst])")  // d1  += k2*s0
            __ASM_EMIT("movss       %%xmm5, 0x34(%[dst])")  // d13 += k2*s1

            __ASM_EMIT("movaps      %%xmm0, %%xmm4")        // xmm4 = s0
            __ASM_EMIT("movaps      %%xmm1, %%xmm5")        // xmm5 = s1
            __ASM_EMIT("mulss       %%xmm7, %%xmm4")        // xmm4 = k1*s0
            __ASM_EMIT("mulss       %%xmm7, %%xmm5")        // xmm5 = k1*s1
            __ASM_EMIT("addss       %%xmm4, %%xmm3")        // xmm3 = k2*s1 + k1*s0
            __ASM_EMIT("addss       %%xmm5, %%xmm2")        // xmm2 = k2*s0 + k1*s1
            __ASM_EMIT("addss       0x0c(%[dst]), %%xmm3")  // xmm3 = k2*s1 + k1*s0 + d3
            __ASM_EMIT("addss       0x2c(%[dst]), %%xmm2")  // xmm2 = k2*s0 + k1*s1 + d11
            __ASM_EMIT("movss       %%xmm3, 0x0c(%[dst])")  // d3  += k2*s1 + k1*s0
            __ASM_EMIT("movss       %%xmm2, 0x2c(%[dst])")  // d11 += k2*s0 + k1*s1

            __ASM_EMIT("movaps      %%xmm0, %%xmm2")        // xmm2 = s0
            __ASM_EMIT("movaps      %%xmm1, %%xmm3")        // xmm3 = s1
            __ASM_EMIT("mulss       %%xmm6, %%xmm2")        // xmm2 = k0*s0
            __ASM_EMIT("mulss       %%xmm6, %%xmm3")        // xmm3 = k0*s1
            __ASM_EMIT("addss       %%xmm2, %%xmm5")        // xmm5 = k1*s1 + k0*s0
            __ASM_EMIT("addss       %%xmm3, %%xmm4")        // xmm4 = k1*s0 + k0*s1
            __ASM_EMIT("addss       0x14(%[dst]), %%xmm5")  // xmm5 = k1*s1 + k0*s0 + d5
            __ASM_EMIT("addss       0x24(%[dst]), %%xmm4")  // xmm4 = k1*s0 + k0*s1 + d9
            __ASM_EMIT("movss       %%xmm5, 0x14(%[dst])")  // d5  += k1*s1 + k0*s0
            __ASM_EMIT("movss       %%xmm4, 0x24(%[dst])")  // d9  += k1*s0 + k0*s1

            __ASM_EMIT("addss       %%xmm3, %%xmm2")        // xmm2 = k0*s0 + k0*s1
            __ASM_EMIT("addss       0x18(%[dst]), %%xmm0")  // xmm0 = s0 + d6
            __ASM_EMIT("addss       0x20(%[dst]), %%xmm1")  // xmm1 = s1 + d8
            __ASM_EMIT("addss       0x1c(%[dst]), %%xmm2")  // xmm2 = k0*s0 + k0*s1 + d7
            __ASM_EMIT("movss       %%xmm0, 0x18(%[dst])")  // d6  += s0
            __ASM_EMIT("movss       %%xmm2, 0x1c(%[dst])")  // d7  += k0*s0 + k0*s1
            __ASM_EMIT("movss       %%xmm1, 0x20(%[dst])")  // d8  += s1

            // Update pointers
            __ASM_EMIT("add         $0x08, %[src]")
            __ASM_EMIT("add         $0x10, %[dst]")
            __ASM_EMIT("sub         $2, %[count]")
            __ASM_EMIT("jae         1b")

            // Complete loop
            __ASM_EMIT("2:")
            __ASM_EMIT("add         $1, %[count]")
            __ASM_EMIT("jl          3f")

            // Load sample
            __ASM_EMIT("movss       (%[src]), %%xmm0")      // xmm0 = s0

            // Do convolution
            __ASM_EMIT("movaps      %%xmm0, %%xmm1")        // xmm1 = s0
            __ASM_EMIT("movaps      %%xmm0, %%xmm2")        // xmm2 = s0
            __ASM_EMIT("movaps      %%xmm0, %%xmm3")        // xmm3 = s0
            __ASM_EMIT("mulss       %%xmm6, %%xmm1")        // xmm1 = k0*s0
            __ASM_EMIT("mulss       %%xmm7, %%xmm2")        // xmm2 = k1*s0
            __ASM_EMIT("mulss       0x08 + %[k], %%xmm3")   // xmm3 = k2*s0
            __ASM_EMIT("movaps      %%xmm1, %%xmm4")        // xmm4 = k0*s0
            __ASM_EMIT("movaps      %%xmm2, %%xmm5")        // xmm5 = k1*s0
            __ASM_EMIT("movaps      %%xmm3, %%xmm6")        // xmm6 = k2*s0

            __ASM_EMIT("addss       0x04(%[dst]), %%xmm3")  // xmm3 = k2*s0 + d1
            __ASM_EMIT("addss       0x0c(%[dst]), %%xmm2")  // xmm2 = k1*s0 + d3
            __ASM_EMIT("addss       0x14(%[dst]), %%xmm1")  // xmm1 = k0*s0 + d5
            __ASM_EMIT("addss       0x18(%[dst]), %%xmm0")  // xmm0 = s0 + d6
            __ASM_EMIT("addss       0x1c(%[dst]), %%xmm4")  // xmm4 = k0*s0 + d7
            __ASM_EMIT("addss       0x24(%[dst]), %%xmm5")  // xmm5 = k1*s0 + d9
            __ASM_EMIT("addss       0x2c(%[dst]), %%xmm6")  // xmm6 = k2*s0 + d11

            __ASM_EMIT("movss       %%xmm3, 0x04(%[dst])")  // d1 += k2*s0
            __ASM_EMIT("movss       %%xmm2, 0x0c(%[dst])")  // d3 += k1*s0
            __ASM_EMIT("movss       %%xmm1, 0x14(%[dst])")  // d5 += k0*s0
            __ASM_EMIT("movss       %%xmm0, 0x18(%[dst])")  // d6 += s0
            __ASM_EMIT("movss       %%xmm4, 0x1c(%[dst])")  // d1 += k0*s0
            __ASM_EMIT("movss       %%xmm5, 0x24(%[dst])")  // d3 += k1*s0
            __ASM_EMIT("movss       %%xmm6, 0x2c(%[dst])")  // d5 += k2*s0

            __ASM_EMIT("3:")

            : [dst] "+r" (dst), [src] "+r" (src), [count] "+r" (count)
            : [k] "o" (lanczos_2x3)
            : "cc", "memory",
              "%xmm0", "%xmm1", "%xmm2", "%xmm3",
              "%xmm4", "%xmm5", "%xmm6", "%xmm7"
        );
    }

    void lanczos_resample_3x2(float *dst, const float *src, size_t count)
    {
        ARCH_X86_ASM
        (
            __ASM_EMIT("sub         $2, %[count]")
            __ASM_EMIT("jb          2f")

            // Load samples
            __ASM_EMIT("1:")
            __ASM_EMIT("movss       0x00(%[src]), %%xmm0")  // xmm0 = s0
            __ASM_EMIT("movss       0x04(%[src]), %%xmm4")  // xmm4 = s1
            __ASM_EMIT("shufps      $0x00, %%xmm0, %%xmm0") // xmm0 = s0
            __ASM_EMIT("shufps      $0x00, %%xmm4, %%xmm4") // xmm4 = s1
            __ASM_EMIT("movaps      %%xmm0, %%xmm1")        // xmm1 = s0
            __ASM_EMIT("movaps      %%xmm4, %%xmm5")        // xmm5 = s1
            __ASM_EMIT("movaps      %%xmm0, %%xmm2")        // xmm2 = s0
            __ASM_EMIT("movaps      %%xmm4, %%xmm6")        // xmm6 = s1

            // Do convolution
            __ASM_EMIT("mulps       0x00 + %[k], %%xmm0")    // xmm0 = s0*m0
            __ASM_EMIT("mulps       0x10 + %[k], %%xmm1")    // xmm1 = s0*m1
            __ASM_EMIT("mulps       0x20 + %[k], %%xmm2")    // xmm2 = s0*m2
            __ASM_EMIT("mulps       0x30 + %[k], %%xmm4")    // xmm4 = s1*m3
            __ASM_EMIT("mulps       0x40 + %[k], %%xmm5")    // xmm5 = s1*m4
            __ASM_EMIT("mulps       0x50 + %[k], %%xmm6")    // xmm6 = s1*m5
            __ASM_EMIT("addps       %%xmm4, %%xmm1")        // xmm1 = s0*m1 + s1*m3
            __ASM_EMIT("addps       %%xmm5, %%xmm2")        // xmm2 = s0*m2 + s1*m4
            __ASM_EMIT("movups      0x00(%[dst]), %%xmm3")  // xmm3 = d0
            __ASM_EMIT("movups      0x10(%[dst]), %%xmm4")  // xmm4 = d1
            __ASM_EMIT("movups      0x20(%[dst]), %%xmm5")  // xmm5 = d2
            __ASM_EMIT("movups      0x30(%[dst]), %%xmm7")  // xmm7 = d3
            __ASM_EMIT("addps       %%xmm0, %%xmm3")        // xmm3 = d0 + s0*m0
            __ASM_EMIT("addps       %%xmm1, %%xmm4")        // xmm4 = d1 + s0*m1 + s1*m3
            __ASM_EMIT("addps       %%xmm2, %%xmm5")        // xmm5 = d2 + s0*m2 + s1*m4
            __ASM_EMIT("addps       %%xmm6, %%xmm7")        // xmm6 = d3 + s1*m5
            __ASM_EMIT("movups      %%xmm3, 0x00(%[dst])")  // d0  += s0*m0
            __ASM_EMIT("movups      %%xmm4, 0x10(%[dst])")  // d1  += s0*m1 + s1*m3
            __ASM_EMIT("movups      %%xmm5, 0x20(%[dst])")  // d2  += s0*m2 + s1*m4
            __ASM_EMIT("movups      %%xmm7, 0x30(%[dst])")  // d3  += s1*m5

            // Update pointers
            __ASM_EMIT("add         $0x08, %[src]")
            __ASM_EMIT("add         $0x18, %[dst]")
            __ASM_EMIT("sub         $2, %[count]")
            __ASM_EMIT("jae         1b")

            // Complete loop
            __ASM_EMIT("2:")
            __ASM_EMIT("add         $1, %[count]")
            __ASM_EMIT("jl          3f")

            // Load sample
            __ASM_EMIT("movss       0x00(%[src]), %%xmm0")  // xmm0 = s0
            __ASM_EMIT("shufps      $0x00, %%xmm0, %%xmm0") // xmm0 = s0
            __ASM_EMIT("movaps      %%xmm0, %%xmm1")        // xmm1 = s0
            __ASM_EMIT("movaps      %%xmm0, %%xmm2")        // xmm2 = s0

            // Do convolution
            __ASM_EMIT("mulps       0x00 + %[k], %%xmm0")    // xmm0 = s0*m0
            __ASM_EMIT("mulps       0x10 + %[k], %%xmm1")    // xmm1 = s0*m1
            __ASM_EMIT("mulps       0x20 + %[k], %%xmm2")    // xmm2 = s0*m2

            __ASM_EMIT("movups      0x00(%[dst]), %%xmm3")  // xmm3 = d0
            __ASM_EMIT("movups      0x10(%[dst]), %%xmm4")  // xmm4 = d1
            __ASM_EMIT("movups      0x20(%[dst]), %%xmm5")  // xmm5 = d2
            __ASM_EMIT("addps       %%xmm0, %%xmm3")        // xmm3 = d0 + s0*m0
            __ASM_EMIT("addps       %%xmm1, %%xmm4")        // xmm4 = d1 + s0*m1
            __ASM_EMIT("addps       %%xmm2, %%xmm5")        // xmm5 = d2 + s0*m2
            __ASM_EMIT("movups      %%xmm3, 0x00(%[dst])")  // d0  += s0*m0
            __ASM_EMIT("movups      %%xmm4, 0x10(%[dst])")  // d1  += s0*m1
            __ASM_EMIT("movups      %%xmm5, 0x20(%[dst])")  // d2  += s0*m2

            __ASM_EMIT("3:")

            : [dst] "+r" (dst), [src] "+r" (src), [count] "+r" (count)
            : [k] "o" (lanczos_kernel_3x2)
            : "cc", "memory",
              "%xmm0", "%xmm1", "%xmm2", "%xmm3",
              "%xmm4", "%xmm5", "%xmm6", "%xmm7"
        );
    }

    void lanczos_resample_3x3(float *dst, const float *src, size_t count)
    {
        ARCH_X86_ASM
        (
            __ASM_EMIT("sub         $2, %[count]")
            __ASM_EMIT("jb          2f")

            // Load samples
            __ASM_EMIT(".align 16")
            __ASM_EMIT("1:")
            __ASM_EMIT("movss       0x00(%[src]), %%xmm0")  // xmm0 = s0
            __ASM_EMIT("movss       0x04(%[src]), %%xmm4")  // xmm4 = s1
            __ASM_EMIT("shufps      $0x00, %%xmm0, %%xmm0") // xmm0 = s0
            __ASM_EMIT("shufps      $0x00, %%xmm4, %%xmm4") // xmm4 = s1
            __ASM_EMIT("movaps      %%xmm0, %%xmm1")        // xmm1 = s0
            __ASM_EMIT("movaps      %%xmm4, %%xmm5")        // xmm5 = s1
            __ASM_EMIT("movaps      %%xmm0, %%xmm2")        // xmm2 = s0
            __ASM_EMIT("movaps      %%xmm4, %%xmm6")        // xmm6 = s1
            __ASM_EMIT("movaps      %%xmm0, %%xmm3")        // xmm3 = s0

            // Do convolution
            __ASM_EMIT("mulps       0x00 + %[k], %%xmm1")    // xmm1 = s0*m0
            __ASM_EMIT("mulps       0x10 + %[k], %%xmm2")    // xmm2 = s0*m1
            __ASM_EMIT("mulps       0x20 + %[k], %%xmm3")    // xmm3 = s0*m2
            __ASM_EMIT("mulps       0x50 + %[k], %%xmm5")    // xmm5 = s1*m5
            __ASM_EMIT("mulps       0x60 + %[k], %%xmm6")    // xmm6 = s1*m6
            __ASM_EMIT("addps       %%xmm5, %%xmm2")        // xmm2 = s0*m1 + s1*m5
            __ASM_EMIT("addps       %%xmm6, %%xmm3")        // xmm3 = s0*m2 + s1*m6
            __ASM_EMIT("movups      0x00(%[dst]), %%xmm5")  // xmm5 = d0
            __ASM_EMIT("movups      0x10(%[dst]), %%xmm6")  // xmm6 = d1
            __ASM_EMIT("movups      0x20(%[dst]), %%xmm7")  // xmm7 = d2
            __ASM_EMIT("addps       %%xmm1, %%xmm5")        // xmm5 = d0 + s0*m0
            __ASM_EMIT("addps       %%xmm2, %%xmm6")        // xmm6 = d1 + s0*m1 + s1*m5
            __ASM_EMIT("addps       %%xmm3, %%xmm7")        // xmm7 = d2 + s0*m2 + s1*m6
            __ASM_EMIT("movups      %%xmm5, 0x00(%[dst])")  // d0  += s0*m0
            __ASM_EMIT("movups      %%xmm6, 0x10(%[dst])")  // d1  += s0*m1 + s1*m5
            __ASM_EMIT("movups      %%xmm7, 0x20(%[dst])")  // d2  += s0*m2 + s1*m6

            __ASM_EMIT("movaps      %%xmm4, %%xmm5")        // xmm4 = s1
            __ASM_EMIT("movaps      %%xmm0, %%xmm1")        // xmm1 = s0
            __ASM_EMIT("movaps      %%xmm4, %%xmm6")        // xmm6 = s1
            __ASM_EMIT("mulps       0x30 + %[k], %%xmm0")    // xmm0 = s0*m3
            __ASM_EMIT("mulps       0x40 + %[k], %%xmm1")    // xmm1 = s0*m4
            __ASM_EMIT("mulps       0x70 + %[k], %%xmm4")    // xmm4 = s1*m7
            __ASM_EMIT("mulps       0x80 + %[k], %%xmm5")    // xmm5 = s1*m8
            __ASM_EMIT("mulss       0x90 + %[k], %%xmm6")    // xmm6 = s1*m9
            __ASM_EMIT("addps       %%xmm4, %%xmm0")        // xmm0 = s0*m3 + s1*m7
            __ASM_EMIT("addps       %%xmm5, %%xmm1")        // xmm1 = s0*m4 + s1*m8
            __ASM_EMIT("movups      0x30(%[dst]), %%xmm2")  // xmm2 = d3
            __ASM_EMIT("movups      0x40(%[dst]), %%xmm3")  // xmm3 = d4
            __ASM_EMIT("addss       0x50(%[dst]), %%xmm6")  // xmm6 = d5 + s1*m9
            __ASM_EMIT("addps       %%xmm2, %%xmm0")        // xmm0 = d3 + s0*m3 + s1*m7
            __ASM_EMIT("addps       %%xmm3, %%xmm1")        // xmm1 = d1 + s0*m4 + s1*m8
            __ASM_EMIT("movups      %%xmm0, 0x30(%[dst])")  // d3  += s0*m3 + s1*m7
            __ASM_EMIT("movups      %%xmm1, 0x40(%[dst])")  // d4  += s0*m4 + s1*m8
            __ASM_EMIT("movss       %%xmm6, 0x50(%[dst])")  // d5  += s1*m9

            // Update pointers
            __ASM_EMIT("add         $0x08, %[src]")
            __ASM_EMIT("add         $0x18, %[dst]")
            __ASM_EMIT("sub         $2, %[count]")
            __ASM_EMIT("jae         1b")

            // Complete loop
            __ASM_EMIT("2:")
            __ASM_EMIT("add         $1, %[count]")
            __ASM_EMIT("jl          3f")

            // Load samples
            __ASM_EMIT("movss       0x00(%[src]), %%xmm0")  // xmm0 = s0
            __ASM_EMIT("movups      0x00(%[dst]), %%xmm5")  // xmm5 = d0
            __ASM_EMIT("shufps      $0x00, %%xmm0, %%xmm0") // xmm0 = s0
            __ASM_EMIT("movups      0x10(%[dst]), %%xmm6")  // xmm6 = d1
            __ASM_EMIT("movaps      %%xmm0, %%xmm1")        // xmm1 = s0
            __ASM_EMIT("movups      0x20(%[dst]), %%xmm7")  // xmm7 = d2
            __ASM_EMIT("movaps      %%xmm0, %%xmm2")        // xmm2 = s0
            __ASM_EMIT("movaps      %%xmm1, %%xmm3")        // xmm3 = s0
            __ASM_EMIT("movaps      %%xmm0, %%xmm4")        // xmm4 = s0

            // Do convolution
            __ASM_EMIT("mulps       0x00 + %[k], %%xmm0")    // xmm0 = s0*m0
            __ASM_EMIT("mulps       0x10 + %[k], %%xmm1")    // xmm1 = s0*m1
            __ASM_EMIT("mulps       0x20 + %[k], %%xmm2")    // xmm2 = s0*m2
            __ASM_EMIT("mulps       0x30 + %[k], %%xmm3")    // xmm3 = s0*m3
            __ASM_EMIT("mulps       0x40 + %[k], %%xmm4")    // xmm4 = s0*m4

            __ASM_EMIT("addps       %%xmm5, %%xmm0")        // xmm0 = d0 + s0*m0
            __ASM_EMIT("addps       %%xmm6, %%xmm1")        // xmm1 = d1 + s0*m1
            __ASM_EMIT("addps       %%xmm7, %%xmm2")        // xmm2 = d2 + s0*m2
            __ASM_EMIT("movups      0x30(%[dst]), %%xmm5")  // xmm5 = d3
            __ASM_EMIT("movups      0x40(%[dst]), %%xmm6")  // xmm6 = d4
            __ASM_EMIT("addps       %%xmm5, %%xmm3")        // xmm1 = d1 + s0*m1
            __ASM_EMIT("addps       %%xmm6, %%xmm4")        // xmm2 = d2 + s0*m2
            __ASM_EMIT("movups      %%xmm0, 0x00(%[dst])")  // d0  += s0*m0
            __ASM_EMIT("movups      %%xmm1, 0x10(%[dst])")  // d1  += s0*m1
            __ASM_EMIT("movups      %%xmm2, 0x20(%[dst])")  // d2  += s0*m2
            __ASM_EMIT("movups      %%xmm3, 0x30(%[dst])")  // d3  += s0*m3
            __ASM_EMIT("movups      %%xmm4, 0x40(%[dst])")  // d3  += s0*m4

            // End of loop
            __ASM_EMIT("3:")

            : [dst] "+r" (dst), [src] "+r" (src), [count] "+r" (count)
            : [k] "o" (lanczos_kernel_3x3)
            : "cc", "memory",
              "%xmm0", "%xmm1", "%xmm2", "%xmm3",
              "%xmm4", "%xmm5", "%xmm6", "%xmm7"
        );
    }

    void lanczos_resample_4x2(float *dst, const float *src, size_t count)
    {
        ARCH_X86_ASM
        (
            __ASM_EMIT("sub         $2, %[count]")
            __ASM_EMIT("jb          2f")

            // Load samples
            __ASM_EMIT(".align 16")
            __ASM_EMIT("1:")
            __ASM_EMIT("movss       0x00(%[src]), %%xmm0")  // xmm0 = s0
            __ASM_EMIT("movss       0x04(%[src]), %%xmm4")  // xmm4 = s1
            __ASM_EMIT("shufps      $0x00, %%xmm0, %%xmm0") // xmm0 = s0
            __ASM_EMIT("shufps      $0x00, %%xmm4, %%xmm4") // xmm4 = s1
            __ASM_EMIT("movaps      %%xmm0, %%xmm1")        // xmm1 = s0
            __ASM_EMIT("movaps      %%xmm4, %%xmm5")        // xmm5 = s1
            __ASM_EMIT("movaps      %%xmm0, %%xmm2")        // xmm2 = s0
            __ASM_EMIT("movaps      %%xmm4, %%xmm6")        // xmm6 = s1
            __ASM_EMIT("movaps      %%xmm0, %%xmm3")        // xmm3 = s0
            __ASM_EMIT("movaps      %%xmm4, %%xmm7")        // xmm7 = s1

            // Do convolution
            __ASM_EMIT("mulps       0x00 + %[k], %%xmm0")    // xmm0 = s0*k0
            __ASM_EMIT("mulps       0x00 + %[k], %%xmm4")    // xmm4 = s1*k0
            __ASM_EMIT("mulps       0x10 + %[k], %%xmm1")    // xmm1 = s0*k1
            __ASM_EMIT("mulps       0x10 + %[k], %%xmm5")    // xmm5 = s1*k1
            __ASM_EMIT("mulps       0x20 + %[k], %%xmm2")    // xmm2 = s0*k2
            __ASM_EMIT("mulps       0x20 + %[k], %%xmm6")    // xmm6 = s1*k2
            __ASM_EMIT("mulps       0x30 + %[k], %%xmm3")    // xmm3 = s0*k3
            __ASM_EMIT("mulps       0x30 + %[k], %%xmm7")    // xmm7 = s1*k3
            __ASM_EMIT("addps       %%xmm4, %%xmm1")        // xmm1 = s0*k1 + s1*k0
            __ASM_EMIT("addps       %%xmm5, %%xmm2")        // xmm2 = s0*k2 + s1*k1
            __ASM_EMIT("addps       %%xmm6, %%xmm3")        // xmm3 = s0*k3 + s1*k2
            __ASM_EMIT("movups      0x00(%[dst]), %%xmm4")  // xmm4 = d0
            __ASM_EMIT("movups      0x10(%[dst]), %%xmm5")  // xmm5 = d1
            __ASM_EMIT("movups      0x20(%[dst]), %%xmm6")  // xmm6 = d2
            __ASM_EMIT("addps       %%xmm0, %%xmm4")        // xmm4 = d0 + s0*k0
            __ASM_EMIT("addps       %%xmm1, %%xmm5")        // xmm5 = d1 + s0*k1 + s1*k0
            __ASM_EMIT("addps       %%xmm2, %%xmm6")        // xmm6 = d2 + s0*k2 + s1*k1
            __ASM_EMIT("movups      0x30(%[dst]), %%xmm0")  // xmm0 = d3
            __ASM_EMIT("movups      0x40(%[dst]), %%xmm1")  // xmm1 = d4
            __ASM_EMIT("addps       %%xmm3, %%xmm0")        // xmm3 = d3 + s0*k3 + s1*k2
            __ASM_EMIT("addps       %%xmm7, %%xmm1")        // xmm7 = d4 + s1*k3
            __ASM_EMIT("movups      %%xmm4, 0x00(%[dst])")  // d0  += s0*k0
            __ASM_EMIT("movups      %%xmm5, 0x10(%[dst])")  // d1  += s0*k1 + s1*k0
            __ASM_EMIT("movups      %%xmm6, 0x20(%[dst])")  // d2  += s0*k2 + s1*k1
            __ASM_EMIT("movups      %%xmm0, 0x30(%[dst])")  // d3  += s0*k3 + s1*k2
            __ASM_EMIT("movups      %%xmm1, 0x40(%[dst])")  // d4  += s1*k3

            // Update pointers
            __ASM_EMIT("add         $0x08, %[src]")
            __ASM_EMIT("add         $0x20, %[dst]")
            __ASM_EMIT("sub         $2, %[count]")
            __ASM_EMIT("jae         1b")

            // Complete loop
            __ASM_EMIT("2:")
            __ASM_EMIT("add         $1, %[count]")
            __ASM_EMIT("jl          3f")

            __ASM_EMIT("movss       0x00(%[src]), %%xmm0")  // xmm0 = s0
            __ASM_EMIT("shufps      $0x00, %%xmm0, %%xmm0") // xmm0 = s0
            __ASM_EMIT("movaps      %%xmm0, %%xmm1")        // xmm1 = s0
            __ASM_EMIT("movaps      %%xmm0, %%xmm2")        // xmm2 = s0
            __ASM_EMIT("movaps      %%xmm1, %%xmm3")        // xmm3 = s0

            // Do convolution
            __ASM_EMIT("mulps       0x00 + %[k], %%xmm0")    // xmm0 = s0*k0
            __ASM_EMIT("mulps       0x10 + %[k], %%xmm1")    // xmm1 = s0*k1
            __ASM_EMIT("mulps       0x20 + %[k], %%xmm2")    // xmm2 = s0*k2
            __ASM_EMIT("mulps       0x30 + %[k], %%xmm3")    // xmm3 = s0*k3
            __ASM_EMIT("movups      0x00(%[dst]), %%xmm4")  // xmm4 = d0
            __ASM_EMIT("movups      0x10(%[dst]), %%xmm5")  // xmm5 = d1
            __ASM_EMIT("movups      0x20(%[dst]), %%xmm6")  // xmm6 = d2
            __ASM_EMIT("movups      0x30(%[dst]), %%xmm7")  // xmm7 = d3
            __ASM_EMIT("addps       %%xmm0, %%xmm4")        // xmm4 = d0 + s0*k0
            __ASM_EMIT("addps       %%xmm1, %%xmm5")        // xmm5 = d1 + s0*k1
            __ASM_EMIT("addps       %%xmm2, %%xmm6")        // xmm6 = d2 + s0*k2
            __ASM_EMIT("addps       %%xmm3, %%xmm7")        // xmm7 = d3 + s0*k3
            __ASM_EMIT("movups      %%xmm4, 0x00(%[dst])")  // d0  += s0*k0
            __ASM_EMIT("movups      %%xmm5, 0x10(%[dst])")  // d1  += s0*k1
            __ASM_EMIT("movups      %%xmm6, 0x20(%[dst])")  // d2  += s0*k2
            __ASM_EMIT("movups      %%xmm7, 0x30(%[dst])")  // d2  += s0*k2

            // End of loop
            __ASM_EMIT("3:")

            : [dst] "+r" (dst), [src] "+r" (src), [count] "+r" (count)
            : [k] "o" (lanczos_kernel_4x2)
            : "cc", "memory",
              "%xmm0", "%xmm1", "%xmm2", "%xmm3",
              "%xmm4", "%xmm5", "%xmm6", "%xmm7"
        );
    }

    void lanczos_resample_4x3(float *dst, const float *src, size_t count)
    {
        ARCH_X86_ASM
        (
            __ASM_EMIT("sub         $2, %[count]")
            __ASM_EMIT("jb          2f")

            // Load samples
            __ASM_EMIT(".align 16")
            __ASM_EMIT("1:")
            __ASM_EMIT("movss       0x00(%[src]), %%xmm0")  // xmm0 = s0
            __ASM_EMIT("movss       0x04(%[src]), %%xmm4")  // xmm4 = s1
            __ASM_EMIT("shufps      $0x00, %%xmm0, %%xmm0") // xmm0 = s0
            __ASM_EMIT("shufps      $0x00, %%xmm4, %%xmm4") // xmm4 = s1
            __ASM_EMIT("movaps      %%xmm0, %%xmm1")        // xmm1 = s0
            __ASM_EMIT("movaps      %%xmm4, %%xmm5")        // xmm5 = s1
            __ASM_EMIT("movaps      %%xmm0, %%xmm2")        // xmm2 = s0
            __ASM_EMIT("movaps      %%xmm4, %%xmm6")        // xmm6 = s1
            __ASM_EMIT("movaps      %%xmm0, %%xmm3")        // xmm3 = s0
            __ASM_EMIT("movaps      %%xmm4, %%xmm7")        // xmm7 = s1

            // Do convolution
            __ASM_EMIT("mulps       0x00 + %[k], %%xmm1")    // xmm1 = s0*k0
            __ASM_EMIT("mulps       0x00 + %[k], %%xmm5")    // xmm5 = s1*k0
            __ASM_EMIT("mulps       0x10 + %[k], %%xmm2")    // xmm2 = s0*k1
            __ASM_EMIT("mulps       0x10 + %[k], %%xmm6")    // xmm6 = s1*k1
            __ASM_EMIT("mulps       0x20 + %[k], %%xmm3")    // xmm3 = s0*k2
            __ASM_EMIT("mulps       0x20 + %[k], %%xmm7")    // xmm7 = s1*k2
            __ASM_EMIT("addps       %%xmm5, %%xmm2")        // xmm2 = s0*k1 + s1*k0
            __ASM_EMIT("addps       %%xmm6, %%xmm3")        // xmm3 = s0*k2 + s1*k1
            __ASM_EMIT("movups      0x00(%[dst]), %%xmm5")  // xmm5 = d0
            __ASM_EMIT("movups      0x10(%[dst]), %%xmm6")  // xmm6 = d1
            __ASM_EMIT("addps       %%xmm1, %%xmm5")        // xmm5 = d0 + s0*k0
            __ASM_EMIT("addps       %%xmm2, %%xmm6")        // xmm6 = d1 + s0*k1 + s1*k0
            __ASM_EMIT("movups      %%xmm5, 0x00(%[dst])")  // d0  += s0*k0
            __ASM_EMIT("movups      %%xmm6, 0x10(%[dst])")  // d1  += s0*k1 + s1*k0
            __ASM_EMIT("movups      0x20(%[dst]), %%xmm5")  // xmm5 = d2
            __ASM_EMIT("movaps      %%xmm0, %%xmm1")        // xmm1 = s0
            __ASM_EMIT("addps       %%xmm3, %%xmm5")        // xmm5 = d2 + s0*k2 + s1*k1
            __ASM_EMIT("movaps      %%xmm4, %%xmm3")        // xmm3 = s1
            __ASM_EMIT("movups      %%xmm5, 0x20(%[dst])")  // d2  += s0*k2 + s1*k1
            __ASM_EMIT("movaps      %%xmm0, %%xmm2")        // xmm2 = s0
            __ASM_EMIT("movaps      %%xmm4, %%xmm5")        // xmm5 = s1
            __ASM_EMIT("mulps       0x30 + %[k], %%xmm0")    // xmm0 = s0*k3
            __ASM_EMIT("mulps       0x30 + %[k], %%xmm3")    // xmm3 = s1*k3
            __ASM_EMIT("mulps       0x40 + %[k], %%xmm1")    // xmm1 = s0*k4
            __ASM_EMIT("mulps       0x40 + %[k], %%xmm4")    // xmm4 = s1*k4
            __ASM_EMIT("mulps       0x50 + %[k], %%xmm2")    // xmm2 = s0*k5
            __ASM_EMIT("mulps       0x50 + %[k], %%xmm5")    // xmm5 = s1*k5
            __ASM_EMIT("addps       %%xmm7, %%xmm0")        // xmm0 = s0*k3 + s1*k2
            __ASM_EMIT("addps       %%xmm3, %%xmm1")        // xmm1 = s0*k4 + s1*k3
            __ASM_EMIT("addps       %%xmm4, %%xmm2")        // xmm2 = s0*k5 + s1*k4

            __ASM_EMIT("movups      0x30(%[dst]), %%xmm3")  // xmm3 = d3
            __ASM_EMIT("movups      0x40(%[dst]), %%xmm4")  // xmm4 = d4
            __ASM_EMIT("movups      0x50(%[dst]), %%xmm6")  // xmm6 = d5
            __ASM_EMIT("movups      0x60(%[dst]), %%xmm7")  // xmm7 = d6
            __ASM_EMIT("addps       %%xmm0, %%xmm3")        // xmm3 = d3 + s0*k3 + s1*k2
            __ASM_EMIT("addps       %%xmm1, %%xmm4")        // xmm4 = d4 + s0*k4 + s1*k3
            __ASM_EMIT("addps       %%xmm2, %%xmm6")        // xmm6 = d5 + s0*k5 + s1*k4
            __ASM_EMIT("addps       %%xmm5, %%xmm7")        // xmm7 = d6 + s1*k5
            __ASM_EMIT("movups      %%xmm3, 0x30(%[dst])")  // d3  += s0*k3 + s1*k2
            __ASM_EMIT("movups      %%xmm4, 0x40(%[dst])")  // d4  += s0*k4 + s1*k3
            __ASM_EMIT("movups      %%xmm6, 0x50(%[dst])")  // d5  += s0*k5 + s1*k4
            __ASM_EMIT("movups      %%xmm7, 0x60(%[dst])")  // d6  += s1*k5

            // Update pointers
            __ASM_EMIT("add         $0x08, %[src]")
            __ASM_EMIT("add         $0x20, %[dst]")
            __ASM_EMIT("sub         $2, %[count]")
            __ASM_EMIT("jae         1b")

            // Complete loop
            __ASM_EMIT("2:")
            __ASM_EMIT("add         $1, %[count]")
            __ASM_EMIT("jl          3f")

            __ASM_EMIT("movss       0x00(%[src]), %%xmm0")  // xmm0 = s0
            __ASM_EMIT("shufps      $0x00, %%xmm0, %%xmm0") // xmm0 = s0
            __ASM_EMIT("movaps      %%xmm0, %%xmm1")        // xmm1 = s0
            __ASM_EMIT("movaps      %%xmm0, %%xmm2")        // xmm2 = s0
            __ASM_EMIT("movaps      %%xmm1, %%xmm3")        // xmm3 = s0
            __ASM_EMIT("movaps      %%xmm0, %%xmm4")        // xmm4 = s0
            __ASM_EMIT("movaps      %%xmm2, %%xmm5")        // xmm5 = s0

            // Do convolution
            __ASM_EMIT("mulps       0x00 + %[k], %%xmm0")    // xmm0 = s0*k0
            __ASM_EMIT("mulps       0x10 + %[k], %%xmm1")    // xmm1 = s0*k1
            __ASM_EMIT("mulps       0x20 + %[k], %%xmm2")    // xmm2 = s0*k2
            __ASM_EMIT("mulps       0x30 + %[k], %%xmm3")    // xmm3 = s0*k3
            __ASM_EMIT("mulps       0x40 + %[k], %%xmm4")    // xmm2 = s0*k4
            __ASM_EMIT("mulps       0x50 + %[k], %%xmm5")    // xmm3 = s0*k5
            __ASM_EMIT("movups      0x00(%[dst]), %%xmm6")  // xmm6 = d0
            __ASM_EMIT("movups      0x10(%[dst]), %%xmm7")  // xmm7 = d1
            __ASM_EMIT("addps       %%xmm0, %%xmm6")        // xmm4 = d0 + s0*k0
            __ASM_EMIT("addps       %%xmm1, %%xmm7")        // xmm5 = d1 + s0*k1
            __ASM_EMIT("movups      %%xmm6, 0x00(%[dst])")  // d0  += s0*k0
            __ASM_EMIT("movups      %%xmm7, 0x10(%[dst])")  // d1  += s0*k1
            __ASM_EMIT("movups      0x20(%[dst]), %%xmm0")  // xmm0 = d2
            __ASM_EMIT("movups      0x30(%[dst]), %%xmm1")  // xmm1 = d3
            __ASM_EMIT("movups      0x40(%[dst]), %%xmm6")  // xmm6 = d4
            __ASM_EMIT("movups      0x50(%[dst]), %%xmm7")  // xmm7 = d5
            __ASM_EMIT("addps       %%xmm2, %%xmm0")        // xmm0 = d2 + s0*k2
            __ASM_EMIT("addps       %%xmm3, %%xmm1")        // xmm1 = d3 + s0*k3
            __ASM_EMIT("addps       %%xmm4, %%xmm6")        // xmm6 = d4 + s0*k4
            __ASM_EMIT("addps       %%xmm5, %%xmm7")        // xmm7 = d5 + s0*k5
            __ASM_EMIT("movups      %%xmm0, 0x20(%[dst])")  // d2  += s0*k2
            __ASM_EMIT("movups      %%xmm1, 0x30(%[dst])")  // d3  += s0*k3
            __ASM_EMIT("movups      %%xmm6, 0x40(%[dst])")  // d4  += s0*k4
            __ASM_EMIT("movups      %%xmm7, 0x50(%[dst])")  // d5  += s0*k5

            // End of loop
            __ASM_EMIT("3:")

            : [dst] "+r" (dst), [src] "+r" (src), [count] "+r" (count)
            : [k] "o" (lanczos_kernel_4x3)
            : "cc", "memory",
              "%xmm0", "%xmm1", "%xmm2", "%xmm3",
              "%xmm4", "%xmm5", "%xmm6", "%xmm7"
        );
    }

    void lanczos_resample_6x2(float *dst, const float *src, size_t count)
    {
        ARCH_X86_ASM
        (
            __ASM_EMIT("test        %[count], %[count]")
            __ASM_EMIT("jz          2f")

            // Load samples
            __ASM_EMIT(".align 16")
            __ASM_EMIT("1:")
            __ASM_EMIT("movss       0x00(%[src]), %%xmm0")  // xmm0 = s0
            __ASM_EMIT("shufps      $0x00, %%xmm0, %%xmm0") // xmm0 = s0
            __ASM_EMIT("movaps      %%xmm0, %%xmm1")        // xmm1 = s0
            __ASM_EMIT("movaps      %%xmm0, %%xmm2")        // xmm2 = s0
            __ASM_EMIT("movaps      %%xmm1, %%xmm3")        // xmm3 = s0
            __ASM_EMIT("movaps      %%xmm0, %%xmm4")        // xmm4 = s0

            // Do convolution (part 1)
            __ASM_EMIT("mulps       0x00 + %[k], %%xmm2")    // xmm2 = s0*k0
            __ASM_EMIT("mulps       0x10 + %[k], %%xmm3")    // xmm3 = s0*k1
            __ASM_EMIT("mulps       0x20 + %[k], %%xmm4")    // xmm4 = s0*k2
            __ASM_EMIT("movups      0x00(%[dst]), %%xmm5")  // xmm5 = d0
            __ASM_EMIT("movups      0x10(%[dst]), %%xmm6")  // xmm6 = d1
            __ASM_EMIT("movups      0x20(%[dst]), %%xmm7")  // xmm7 = d2
            __ASM_EMIT("addps       %%xmm2, %%xmm5")        // xmm5 = d0 + s0*k0
            __ASM_EMIT("addps       %%xmm3, %%xmm6")        // xmm6 = d1 + s0*k1
            __ASM_EMIT("addps       %%xmm4, %%xmm7")        // xmm7 = d2 + s0*k2
            __ASM_EMIT("movups      %%xmm5, 0x00(%[dst])")  // d0  += s0*k0
            __ASM_EMIT("movups      %%xmm6, 0x10(%[dst])")  // d1  += s0*k1
            __ASM_EMIT("movups      %%xmm7, 0x20(%[dst])")  // d2  += s0*k2

            // Do convolution (part 2)
            __ASM_EMIT("movaps      %%xmm1, %%xmm2")        // xmm2 = s0
            __ASM_EMIT("mulps       0x30 + %[k], %%xmm0")   // xmm0 = s0*k3
            __ASM_EMIT("mulps       0x40 + %[k], %%xmm1")   // xmm1 = s0*k4
            __ASM_EMIT("mulps       0x50 + %[k], %%xmm2")   // xmm2 = s0*k5
            __ASM_EMIT("movups      0x30(%[dst]), %%xmm3")  // xmm3 = d3
            __ASM_EMIT("movups      0x40(%[dst]), %%xmm4")  // xmm4 = d4
            __ASM_EMIT("movups      0x50(%[dst]), %%xmm5")  // xmm5 = d5
            __ASM_EMIT("addps       %%xmm0, %%xmm3")        // xmm3 = d3 + s0*k3
            __ASM_EMIT("addps       %%xmm1, %%xmm4")        // xmm4 = d4 + s0*k4
            __ASM_EMIT("addps       %%xmm2, %%xmm5")        // xmm5 = d5 + s0*k5
            __ASM_EMIT("movups      %%xmm3, 0x30(%[dst])")  // d3  += s0*k3
            __ASM_EMIT("movups      %%xmm4, 0x40(%[dst])")  // d4  += s0*k4
            __ASM_EMIT("movups      %%xmm5, 0x50(%[dst])")  // d5  += s0*k5

            // Update pointers
            __ASM_EMIT("add         $0x04, %[src]")
            __ASM_EMIT("add         $0x18, %[dst]")
            __ASM_EMIT("dec         %[count]")
            __ASM_EMIT("jnz         1b")

            // Complete loop
            __ASM_EMIT("2:")

            : [dst] "+r" (dst), [src] "+r" (src), [count] "+r" (count)
            : [k] "o" (lanczos_kernel_6x2)
            : "cc", "memory",
              "%xmm0", "%xmm1", "%xmm2", "%xmm3",
              "%xmm4", "%xmm5", "%xmm6", "%xmm7"
        );
    }

    void lanczos_resample_6x3(float *dst, const float *src, size_t count)
    {
        ARCH_X86_ASM
        (
            __ASM_EMIT("test        %[count], %[count]")
            __ASM_EMIT("jz          2f")

            // Load samples
            __ASM_EMIT(".align 16")
            __ASM_EMIT("1:")
            __ASM_EMIT("movss       0x00(%[src]), %%xmm0")  // xmm0 = s0
            __ASM_EMIT("shufps      $0x00, %%xmm0, %%xmm0") // xmm0 = s0
            __ASM_EMIT("movaps      %%xmm0, %%xmm1")        // xmm1 = s0
            __ASM_EMIT("movaps      %%xmm0, %%xmm2")        // xmm2 = s0
            __ASM_EMIT("movaps      %%xmm1, %%xmm3")        // xmm3 = s0
            __ASM_EMIT("movaps      %%xmm0, %%xmm4")        // xmm4 = s0

            // Do convolution (part 1)
            __ASM_EMIT("mulps       0x00 + %[k], %%xmm2")   // xmm2 = s0*k0
            __ASM_EMIT("mulps       0x10 + %[k], %%xmm3")   // xmm3 = s0*k1
            __ASM_EMIT("mulps       0x20 + %[k], %%xmm4")   // xmm4 = s0*k2
            __ASM_EMIT("movups      0x00(%[dst]), %%xmm5")  // xmm5 = d0
            __ASM_EMIT("movups      0x10(%[dst]), %%xmm6")  // xmm6 = d1
            __ASM_EMIT("movups      0x20(%[dst]), %%xmm7")  // xmm7 = d2
            __ASM_EMIT("addps       %%xmm2, %%xmm5")        // xmm5 = d0 + s0*k0
            __ASM_EMIT("addps       %%xmm3, %%xmm6")        // xmm6 = d1 + s0*k1
            __ASM_EMIT("addps       %%xmm4, %%xmm7")        // xmm7 = d2 + s0*k2
            __ASM_EMIT("movups      %%xmm5, 0x00(%[dst])")  // d0  += s0*k0
            __ASM_EMIT("movups      %%xmm6, 0x10(%[dst])")  // d1  += s0*k1
            __ASM_EMIT("movups      %%xmm7, 0x20(%[dst])")  // d2  += s0*k2

            // Do convolution (part 2)
            __ASM_EMIT("movaps      %%xmm1, %%xmm2")        // xmm2 = s0
            __ASM_EMIT("movaps      %%xmm0, %%xmm6")        // xmm6 = s0
            __ASM_EMIT("movaps      %%xmm1, %%xmm7")        // xmm7 = s0
            __ASM_EMIT("mulps       0x30 + %[k], %%xmm0")    // xmm0 = s0*k3
            __ASM_EMIT("mulps       0x40 + %[k], %%xmm1")    // xmm1 = s0*k4
            __ASM_EMIT("mulps       0x50 + %[k], %%xmm2")    // xmm2 = s0*k5
            __ASM_EMIT("movups      0x30(%[dst]), %%xmm3")  // xmm3 = d3
            __ASM_EMIT("movups      0x40(%[dst]), %%xmm4")  // xmm4 = d4
            __ASM_EMIT("movups      0x50(%[dst]), %%xmm5")  // xmm5 = d5
            __ASM_EMIT("addps       %%xmm0, %%xmm3")        // xmm3 = d3 + s0*k3
            __ASM_EMIT("addps       %%xmm1, %%xmm4")        // xmm4 = d4 + s0*k4
            __ASM_EMIT("addps       %%xmm2, %%xmm5")        // xmm5 = d5 + s0*k5
            __ASM_EMIT("movups      %%xmm3, 0x30(%[dst])")  // d3  += s0*k3
            __ASM_EMIT("movups      %%xmm4, 0x40(%[dst])")  // d4  += s0*k4
            __ASM_EMIT("movups      %%xmm5, 0x50(%[dst])")  // d5  += s0*k5

            // Do convolution (part 3)
            __ASM_EMIT("movaps      %%xmm7, %%xmm5")        // xmm5 = s0
            __ASM_EMIT("mulps       0x60 + %[k], %%xmm6")    // xmm6 = s0*k6
            __ASM_EMIT("mulps       0x70 + %[k], %%xmm7")    // xmm7 = s0*k7
            __ASM_EMIT("mulps       0x80 + %[k], %%xmm5")    // xmm5 = s0*k8
            __ASM_EMIT("movups      0x60(%[dst]), %%xmm0")  // xmm0 = d6
            __ASM_EMIT("movups      0x70(%[dst]), %%xmm1")  // xmm1 = d7
            __ASM_EMIT("movups      0x80(%[dst]), %%xmm2")  // xmm2 = d8
            __ASM_EMIT("addps       %%xmm6, %%xmm0")        // xmm0 = d6 + s0*k6
            __ASM_EMIT("addps       %%xmm7, %%xmm1")        // xmm1 = d7 + s0*k7
            __ASM_EMIT("addps       %%xmm5, %%xmm2")        // xmm2 = d8 + s0*k8
            __ASM_EMIT("movups      %%xmm0, 0x60(%[dst])")  // d6  += s0*k3
            __ASM_EMIT("movups      %%xmm1, 0x70(%[dst])")  // d7  += s0*k4
            __ASM_EMIT("movups      %%xmm2, 0x80(%[dst])")  // d8  += s0*k5

            // Update pointers
            __ASM_EMIT("add         $0x04, %[src]")
            __ASM_EMIT("add         $0x18, %[dst]")
            __ASM_EMIT("dec         %[count]")
            __ASM_EMIT("jnz         1b")

            // Complete loop
            __ASM_EMIT("2:")

            : [dst] "+r" (dst), [src] "+r" (src), [count] "+r" (count)
            : [k] "o" (lanczos_kernel_6x3)
            : "cc", "memory",
              "%xmm0", "%xmm1", "%xmm2", "%xmm3",
              "%xmm4", "%xmm5", "%xmm6", "%xmm7"
        );
    }

    void lanczos_resample_8x2(float *dst, const float *src, size_t count)
    {
        ARCH_X86_ASM
        (
            __ASM_EMIT("test        %[count], %[count]")
            __ASM_EMIT("jz          2f")

            // Load samples
            __ASM_EMIT(".align 16")
            __ASM_EMIT("1:")

            // Do convolution (part 1)
            __ASM_EMIT("movss       0x00(%[src]), %%xmm0")  // xmm0 = s0
            __ASM_EMIT("shufps      $0x00, %%xmm0, %%xmm0") // xmm0 = s0
            __ASM_EMIT("movaps      %%xmm0, %%xmm1")        // xmm1 = s0
            __ASM_EMIT("movaps      %%xmm0, %%xmm2")        // xmm2 = s0
            __ASM_EMIT("movaps      %%xmm1, %%xmm3")        // xmm3 = s0

            __ASM_EMIT("mulps       0x00 + %[k], %%xmm0")    // xmm0 = s0*k0
            __ASM_EMIT("mulps       0x10 + %[k], %%xmm1")    // xmm1 = s0*k1
            __ASM_EMIT("mulps       0x20 + %[k], %%xmm2")    // xmm2 = s0*k2
            __ASM_EMIT("mulps       0x30 + %[k], %%xmm3")    // xmm3 = s0*k3
            __ASM_EMIT("movups      0x00(%[dst]), %%xmm4")  // xmm4 = d0
            __ASM_EMIT("movups      0x10(%[dst]), %%xmm5")  // xmm5 = d1
            __ASM_EMIT("movups      0x20(%[dst]), %%xmm6")  // xmm6 = d2
            __ASM_EMIT("movups      0x30(%[dst]), %%xmm7")  // xmm7 = d3
            __ASM_EMIT("addps       %%xmm4, %%xmm0")        // xmm0 = d0 + s0*k0
            __ASM_EMIT("addps       %%xmm5, %%xmm1")        // xmm1 = d1 + s0*k1
            __ASM_EMIT("addps       %%xmm6, %%xmm2")        // xmm2 = d2 + s0*k2
            __ASM_EMIT("addps       %%xmm7, %%xmm3")        // xmm3 = d3 + s0*k3
            __ASM_EMIT("movups      %%xmm0, 0x00(%[dst])")  // d0  += s0*k0
            __ASM_EMIT("movups      %%xmm1, 0x10(%[dst])")  // d1  += s0*k1
            __ASM_EMIT("movups      %%xmm2, 0x20(%[dst])")  // d2  += s0*k2
            __ASM_EMIT("movups      %%xmm3, 0x30(%[dst])")  // d3  += s0*k3

            // Do convolution (part 2)
            __ASM_EMIT("movss       0x00(%[src]), %%xmm0")  // xmm0 = s0
            __ASM_EMIT("shufps      $0x00, %%xmm0, %%xmm0") // xmm0 = s0
            __ASM_EMIT("movaps      %%xmm0, %%xmm1")        // xmm1 = s0
            __ASM_EMIT("movaps      %%xmm0, %%xmm2")        // xmm2 = s0
            __ASM_EMIT("movaps      %%xmm1, %%xmm3")        // xmm3 = s0

            __ASM_EMIT("mulps       0x40 + %[k], %%xmm0")    // xmm0 = s0*k4
            __ASM_EMIT("mulps       0x50 + %[k], %%xmm1")    // xmm1 = s0*k5
            __ASM_EMIT("mulps       0x60 + %[k], %%xmm2")    // xmm2 = s0*k6
            __ASM_EMIT("mulps       0x70 + %[k], %%xmm3")    // xmm3 = s0*k7
            __ASM_EMIT("movups      0x40(%[dst]), %%xmm4")  // xmm4 = d4
            __ASM_EMIT("movups      0x50(%[dst]), %%xmm5")  // xmm5 = d5
            __ASM_EMIT("movups      0x60(%[dst]), %%xmm6")  // xmm6 = d6
            __ASM_EMIT("movups      0x70(%[dst]), %%xmm7")  // xmm7 = d7
            __ASM_EMIT("addps       %%xmm4, %%xmm0")        // xmm0 = d4 + s0*k4
            __ASM_EMIT("addps       %%xmm5, %%xmm1")        // xmm1 = d5 + s0*k5
            __ASM_EMIT("addps       %%xmm6, %%xmm2")        // xmm2 = d6 + s0*k6
            __ASM_EMIT("addps       %%xmm7, %%xmm3")        // xmm3 = d7 + s0*k7
            __ASM_EMIT("movups      %%xmm0, 0x40(%[dst])")  // d4  += s0*k4
            __ASM_EMIT("movups      %%xmm1, 0x50(%[dst])")  // d5  += s0*k5
            __ASM_EMIT("movups      %%xmm2, 0x60(%[dst])")  // d6  += s0*k6
            __ASM_EMIT("movups      %%xmm3, 0x70(%[dst])")  // d7  += s0*k7

            // Update pointers
            __ASM_EMIT("add         $0x04, %[src]")
            __ASM_EMIT("add         $0x20, %[dst]")
            __ASM_EMIT("dec         %[count]")
            __ASM_EMIT("jnz         1b")

            // Complete loop
            __ASM_EMIT("2:")

            : [dst] "+r" (dst), [src] "+r" (src), [count] "+r" (count)
            : [k] "o" (lanczos_kernel_8x2)
            : "cc", "memory",
              "%xmm0", "%xmm1", "%xmm2", "%xmm3",
              "%xmm4", "%xmm5", "%xmm6", "%xmm7"
        );
    }

    void lanczos_resample_8x3(float *dst, const float *src, size_t count)
    {
        ARCH_X86_ASM
        (
            __ASM_EMIT("test        %[count], %[count]")
            __ASM_EMIT("jz          2f")

            // Load samples
            __ASM_EMIT(".align 16")
            __ASM_EMIT("1:")
            __ASM_EMIT("movss       0x00(%[src]), %%xmm0")  // xmm0 = s0
            __ASM_EMIT("shufps      $0x00, %%xmm0, %%xmm0") // xmm0 = s0
            __ASM_EMIT("movaps      %%xmm0, %%xmm1")        // xmm1 = s0

            // Do convolution (part 1)
            __ASM_EMIT("movaps      %%xmm0, %%xmm2")        // xmm2 = s0
            __ASM_EMIT("movaps      %%xmm1, %%xmm3")        // xmm3 = s0
            __ASM_EMIT("movaps      %%xmm0, %%xmm4")        // xmm4 = s0
            __ASM_EMIT("mulps       0x00 + %[k], %%xmm2")    // xmm2 = s0*k0
            __ASM_EMIT("mulps       0x10 + %[k], %%xmm3")    // xmm3 = s0*k1
            __ASM_EMIT("mulps       0x20 + %[k], %%xmm4")    // xmm4 = s0*k2
            __ASM_EMIT("movups      0x00(%[dst]), %%xmm5")  // xmm5 = d0
            __ASM_EMIT("movups      0x10(%[dst]), %%xmm6")  // xmm6 = d1
            __ASM_EMIT("movups      0x20(%[dst]), %%xmm7")  // xmm7 = d2
            __ASM_EMIT("addps       %%xmm5, %%xmm2")        // xmm2 = d0 + s0*k0
            __ASM_EMIT("addps       %%xmm6, %%xmm3")        // xmm3 = d1 + s0*k1
            __ASM_EMIT("addps       %%xmm7, %%xmm4")        // xmm4 = d2 + s0*k2
            __ASM_EMIT("movups      %%xmm2, 0x00(%[dst])")  // d0  += s0*k0
            __ASM_EMIT("movups      %%xmm3, 0x10(%[dst])")  // d1  += s0*k1
            __ASM_EMIT("movups      %%xmm4, 0x20(%[dst])")  // d2  += s0*k2

            // Do convolution (part 2)
            __ASM_EMIT("movaps      %%xmm0, %%xmm2")        // xmm2 = s0
            __ASM_EMIT("movaps      %%xmm1, %%xmm3")        // xmm3 = s0
            __ASM_EMIT("movaps      %%xmm0, %%xmm4")        // xmm4 = s0
            __ASM_EMIT("mulps       0x30 + %[k], %%xmm2")    // xmm2 = s0*k3
            __ASM_EMIT("mulps       0x40 + %[k], %%xmm3")    // xmm3 = s0*k4
            __ASM_EMIT("mulps       0x50 + %[k], %%xmm4")    // xmm4 = s0*k5
            __ASM_EMIT("movups      0x30(%[dst]), %%xmm5")  // xmm5 = d3
            __ASM_EMIT("movups      0x40(%[dst]), %%xmm6")  // xmm6 = d4
            __ASM_EMIT("movups      0x50(%[dst]), %%xmm7")  // xmm7 = d5
            __ASM_EMIT("addps       %%xmm5, %%xmm2")        // xmm2 = d3 + s0*k3
            __ASM_EMIT("addps       %%xmm6, %%xmm3")        // xmm3 = d4 + s0*k4
            __ASM_EMIT("addps       %%xmm7, %%xmm4")        // xmm4 = d5 + s0*k5
            __ASM_EMIT("movups      %%xmm2, 0x30(%[dst])")  // d3  += s0*k3
            __ASM_EMIT("movups      %%xmm3, 0x40(%[dst])")  // d4  += s0*k4
            __ASM_EMIT("movups      %%xmm4, 0x50(%[dst])")  // d5  += s0*k5

            // Do convolution (part 3)
            __ASM_EMIT("movaps      %%xmm0, %%xmm2")        // xmm2 = s0
            __ASM_EMIT("movaps      %%xmm1, %%xmm3")        // xmm3 = s0
            __ASM_EMIT("movaps      %%xmm2, %%xmm4")        // xmm4 = s0
            __ASM_EMIT("mulps       0x60 + %[k], %%xmm2")    // xmm2 = s0*k6
            __ASM_EMIT("mulps       0x70 + %[k], %%xmm3")    // xmm3 = s0*k7
            __ASM_EMIT("mulps       0x80 + %[k], %%xmm4")    // xmm4 = s0*k8
            __ASM_EMIT("movups      0x60(%[dst]), %%xmm5")  // xmm5 = d6
            __ASM_EMIT("movups      0x70(%[dst]), %%xmm6")  // xmm6 = d7
            __ASM_EMIT("movups      0x80(%[dst]), %%xmm7")  // xmm7 = d8
            __ASM_EMIT("addps       %%xmm5, %%xmm2")        // xmm2 = d6 + s0*k6
            __ASM_EMIT("addps       %%xmm6, %%xmm3")        // xmm3 = d7 + s0*k7
            __ASM_EMIT("addps       %%xmm7, %%xmm4")        // xmm4 = d8 + s0*k8
            __ASM_EMIT("movups      %%xmm2, 0x60(%[dst])")  // d6  += s0*k6
            __ASM_EMIT("movups      %%xmm3, 0x70(%[dst])")  // d7  += s0*k7
            __ASM_EMIT("movups      %%xmm4, 0x80(%[dst])")  // d8  += s0*k8

            // Do convolution (part 4)
            __ASM_EMIT("movaps      %%xmm0, %%xmm2")        // xmm2 = s0
            __ASM_EMIT("movaps      %%xmm1, %%xmm3")        // xmm3 = s0
            __ASM_EMIT("movaps      %%xmm2, %%xmm4")        // xmm4 = s0
            __ASM_EMIT("mulps       0x90 + %[k], %%xmm2")    // xmm2 = s0*k9
            __ASM_EMIT("mulps       0xa0 + %[k], %%xmm3")    // xmm3 = s0*k10
            __ASM_EMIT("mulps       0xb0 + %[k], %%xmm4")    // xmm4 = s0*k11
            __ASM_EMIT("movups      0x90(%[dst]), %%xmm5")  // xmm5 = d9
            __ASM_EMIT("movups      0xa0(%[dst]), %%xmm6")  // xmm6 = d10
            __ASM_EMIT("movups      0xb0(%[dst]), %%xmm7")  // xmm7 = d11
            __ASM_EMIT("addps       %%xmm5, %%xmm2")        // xmm2 = d9  + s0*k9
            __ASM_EMIT("addps       %%xmm6, %%xmm3")        // xmm3 = d10 + s0*k10
            __ASM_EMIT("addps       %%xmm7, %%xmm4")        // xmm4 = d11 + s0*k11
            __ASM_EMIT("movups      %%xmm2, 0x90(%[dst])")  // d9  += s0*k9
            __ASM_EMIT("movups      %%xmm3, 0xa0(%[dst])")  // d10 += s0*k10
            __ASM_EMIT("movups      %%xmm4, 0xb0(%[dst])")  // d11 += s0*k11

            // Update pointers
            __ASM_EMIT("add         $0x04, %[src]")
            __ASM_EMIT("add         $0x20, %[dst]")
            __ASM_EMIT("dec         %[count]")
            __ASM_EMIT("jnz         1b")

            // Complete loop
            __ASM_EMIT("2:")

            : [dst] "+r" (dst), [src] "+r" (src), [count] "+r" (count)
            : [k] "o" (lanczos_kernel_8x3)
            : "cc", "memory",
              "%xmm0", "%xmm1", "%xmm2", "%xmm3",
              "%xmm4", "%xmm5", "%xmm6", "%xmm7"
        );
    }

    void downsample_2x(float *dst, const float *src, size_t count)
    {
        ARCH_X86_ASM
        (
            __ASM_EMIT("sub         $16, %[count]")
            __ASM_EMIT("jb          2f")

            // 16 x sample loop
            __ASM_EMIT(".align 16")
            __ASM_EMIT("1:")
            __ASM_EMIT("movups      0x00(%[src]), %%xmm0")  // xmm0 = s0 ? s1 ?
            __ASM_EMIT("movups      0x10(%[src]), %%xmm4")  // xmm4 = s2 ? s3 ?
            __ASM_EMIT("movups      0x20(%[src]), %%xmm1")  // xmm1 = s4 x s5 ?
            __ASM_EMIT("movups      0x30(%[src]), %%xmm5")  // xmm5 = s6 ? s7 ?
            __ASM_EMIT("movups      0x40(%[src]), %%xmm2")  // xmm2 = s8 ? s9 ?
            __ASM_EMIT("movups      0x50(%[src]), %%xmm6")  // xmm6 = s10 ? s11 ?
            __ASM_EMIT("movups      0x60(%[src]), %%xmm3")  // xmm3 = s12 ? s13 ?
            __ASM_EMIT("movups      0x70(%[src]), %%xmm7")  // xmm7 = s14 ? s15 ?

            __ASM_EMIT("shufps      $0x88, %%xmm4, %%xmm0") // xmm0 = s0 s1 s2 s3
            __ASM_EMIT("shufps      $0x88, %%xmm5, %%xmm1") // xmm1 = s4 s5 s6 s7
            __ASM_EMIT("shufps      $0x88, %%xmm6, %%xmm2") // xmm2 = s8 s9 s10 s11
            __ASM_EMIT("shufps      $0x88, %%xmm7, %%xmm3") // xmm3 = s12 s13 s14 s15

            __ASM_EMIT("movups      %%xmm0, 0x00(%[dst])")
            __ASM_EMIT("movups      %%xmm1, 0x10(%[dst])")
            __ASM_EMIT("movups      %%xmm2, 0x20(%[dst])")
            __ASM_EMIT("movups      %%xmm3, 0x30(%[dst])")

            // Repeat loop
            __ASM_EMIT("add         $0x80, %[src]")
            __ASM_EMIT("add         $0x40, %[dst]")
            __ASM_EMIT("sub         $16, %[count]")
            __ASM_EMIT("jae         1b")

            // 4x iterations
            __ASM_EMIT("2:")
            __ASM_EMIT("add         $12, %[count]")
            __ASM_EMIT("jl          4f")

            __ASM_EMIT("3:")
            __ASM_EMIT("movups      0x00(%[src]), %%xmm0")  // xmm0 = s0 ? s1 ?
            __ASM_EMIT("movups      0x10(%[src]), %%xmm4")  // xmm4 = s2 ? s3 ?
            __ASM_EMIT("shufps      $0x88, %%xmm4, %%xmm0") // xmm0 = s0 s1 s2 s3
            __ASM_EMIT("movups      %%xmm0, 0x00(%[dst])")

            __ASM_EMIT("add         $0x20, %[src]")
            __ASM_EMIT("add         $0x10, %[dst]")
            __ASM_EMIT("sub         $4, %[count]")
            __ASM_EMIT("jae         3b")

            // 1x iterations
            __ASM_EMIT("4:")
            __ASM_EMIT("add         $4, %[count]")
            __ASM_EMIT("jle         6f")

            __ASM_EMIT("5:")
            __ASM_EMIT("movss       0x00(%[src]), %%xmm0")  // xmm0 = s0
            __ASM_EMIT("movss       %%xmm0, 0x00(%[dst])")
            __ASM_EMIT("add         $0x8, %[src]")
            __ASM_EMIT("add         $0x4, %[dst]")
            __ASM_EMIT("dec         %[count]")
            __ASM_EMIT("jnz         5b")

            // End of routine
            __ASM_EMIT("6:")

            : [dst] "+r" (dst), [src] "+r" (src), [count] "+r" (count)
            :
            : "cc", "memory",
              "%xmm0", "%xmm1", "%xmm2", "%xmm3",
              "%xmm4", "%xmm5", "%xmm6", "%xmm7"
        );
    }

    void downsample_3x(float *dst, const float *src, size_t count)
    {
        ARCH_X86_ASM
        (
            __ASM_EMIT("sub         $16, %[count]")
            __ASM_EMIT("jb          2f")

            // 16 x sample loop
            __ASM_EMIT(".align 16")
            __ASM_EMIT("1:")
            __ASM_EMIT("movups      0x00(%[src]), %%xmm0")  // xmm0 = s0 ? ? s1
            __ASM_EMIT("movss       0x18(%[src]), %%xmm1")  // xmm1 = s2 ? ? ?
            __ASM_EMIT("movss       0x24(%[src]), %%xmm6")  // xmm6 = s3 ? ? ?
            __ASM_EMIT("movups      0x30(%[src]), %%xmm2")  // xmm2 = s4 ? ? s5
            __ASM_EMIT("movss       0x48(%[src]), %%xmm3")  // xmm3 = s6 ? ? ?
            __ASM_EMIT("movss       0x54(%[src]), %%xmm7")  // xmm7 = s7 ? ? ?
            __ASM_EMIT("movlhps     %%xmm6, %%xmm1")        // xmm1 = s2 ? s3 ?
            __ASM_EMIT("movlhps     %%xmm7, %%xmm3")        // xmm3 = s6 ? s7 ?
            __ASM_EMIT("shufps      $0x8c, %%xmm1, %%xmm0") // xmm0 = s0 s1 s2 s3
            __ASM_EMIT("shufps      $0x8c, %%xmm3, %%xmm2") // xmm2 = s4 s5 s6 s7

            __ASM_EMIT("movups      0x60(%[src]), %%xmm4")  // xmm4 = s8 ? ? s9
            __ASM_EMIT("movss       0x78(%[src]), %%xmm5")  // xmm5 = s10 ? ? ?
            __ASM_EMIT("movss       0x84(%[src]), %%xmm1")  // xmm1 = s11 ? ? ?
            __ASM_EMIT("movups      0x90(%[src]), %%xmm6")  // xmm6 = s12 ? ? s13
            __ASM_EMIT("movss       0xa8(%[src]), %%xmm7")  // xmm7 = s14 ? ? ?
            __ASM_EMIT("movss       0xb4(%[src]), %%xmm3")  // xmm3 = s15 ? ? ?
            __ASM_EMIT("movlhps     %%xmm1, %%xmm5")        // xmm5 = s10 ? s11 ?
            __ASM_EMIT("movlhps     %%xmm3, %%xmm7")        // xmm7 = s14 ? s15 ?
            __ASM_EMIT("shufps      $0x8c, %%xmm5, %%xmm4") // xmm4 = s8 s9 s10 s11
            __ASM_EMIT("shufps      $0x8c, %%xmm7, %%xmm6") // xmm6 = s12 s13 s14 s15

            __ASM_EMIT("movups      %%xmm0, 0x00(%[dst])")
            __ASM_EMIT("movups      %%xmm2, 0x10(%[dst])")
            __ASM_EMIT("movups      %%xmm4, 0x20(%[dst])")
            __ASM_EMIT("movups      %%xmm6, 0x30(%[dst])")

            // Repeat loop
            __ASM_EMIT("sub         $16, %[count]")
            __ASM_EMIT("add         $0xc0, %[src]")
            __ASM_EMIT("add         $0x40, %[dst]")
            __ASM_EMIT("cmp         $16, %[count]")
            __ASM_EMIT("jae         1b")

            // 4x iterations
            __ASM_EMIT("2:")
            __ASM_EMIT("add         $12, %[count]")
            __ASM_EMIT("jl          4f")

            __ASM_EMIT("3:")
            __ASM_EMIT("movups      0x00(%[src]), %%xmm0")  // xmm0 = s0 ? s1 ?
            __ASM_EMIT("movups      0x18(%[src]), %%xmm1")  // xmm1 = s2 ? ? ?
            __ASM_EMIT("movups      0x24(%[src]), %%xmm2")  // xmm2 = s3 ? ? ?
            __ASM_EMIT("movlhps     %%xmm2, %%xmm1")        // xmm1 = s2 ? s3 ?
            __ASM_EMIT("shufps      $0x8c, %%xmm1, %%xmm0") // xmm0 = s0 s1 s2 s3
            __ASM_EMIT("movups      %%xmm0, 0x00(%[dst])")

            __ASM_EMIT("add         $0x30, %[src]")
            __ASM_EMIT("add         $0x10, %[dst]")
            __ASM_EMIT("sub         $4, %[count]")
            __ASM_EMIT("jae         3b")

            // 1x iterations
            __ASM_EMIT("4:")
            __ASM_EMIT("add         $4, %[count]")
            __ASM_EMIT("jle         6f")

            __ASM_EMIT("5:")
            __ASM_EMIT("movss       0x00(%[src]), %%xmm0")  // xmm0 = s0
            __ASM_EMIT("movss       %%xmm0, 0x00(%[dst])")
            __ASM_EMIT("add         $0xc, %[src]")
            __ASM_EMIT("add         $0x4, %[dst]")
            __ASM_EMIT("dec         %[count]")
            __ASM_EMIT("jnz         5b")

            // End of routine
            __ASM_EMIT("6:")

            : [dst] "+r" (dst), [src] "+r" (src), [count] "+r" (count)
            :
            : "cc", "memory",
              "%xmm0", "%xmm1", "%xmm2", "%xmm3",
              "%xmm4", "%xmm5", "%xmm6", "%xmm7"
        );
    }

    void downsample_4x(float *dst, const float *src, size_t count)
    {
        ARCH_X86_ASM
        (
            __ASM_EMIT("sub         $16, %[count]")
            __ASM_EMIT("jb          2f")

            // 16 x sample loop
            __ASM_EMIT(".align 16")
            __ASM_EMIT("1:")
            __ASM_EMIT("movss       0x00(%[src]), %%xmm0")  // xmm0 = s0 ? ? ?
            __ASM_EMIT("movss       0x10(%[src]), %%xmm2")  // xmm2 = s1 ? ? ?
            __ASM_EMIT("movss       0x20(%[src]), %%xmm4")  // xmm4 = s2 ? ? ?
            __ASM_EMIT("movss       0x30(%[src]), %%xmm3")  // xmm3 = s3 ? ? ?
            __ASM_EMIT("movlhps     %%xmm2, %%xmm0")        // xmm0 = s0 ? s1 ?
            __ASM_EMIT("movlhps     %%xmm3, %%xmm4")        // xmm4 = s2 ? s3 ?

            __ASM_EMIT("movss       0x40(%[src]), %%xmm1")  // xmm1 = s4 ? ? ?
            __ASM_EMIT("movss       0x50(%[src]), %%xmm2")  // xmm2 = s5 ? ? ?
            __ASM_EMIT("movss       0x60(%[src]), %%xmm5")  // xmm3 = s6 ? ? ?
            __ASM_EMIT("movss       0x70(%[src]), %%xmm3")  // xmm5 = s7 ? ? ?
            __ASM_EMIT("movlhps     %%xmm2, %%xmm1")        // xmm1 = s4 ? s5 ?
            __ASM_EMIT("movlhps     %%xmm3, %%xmm5")        // xmm4 = s6 ? s7 ?
            __ASM_EMIT("shufps      $0x88, %%xmm4, %%xmm0") // xmm0 = s0 s1 s2 s3

            __ASM_EMIT("movss       0x80(%[src]), %%xmm2")  // xmm2 = s8 ? ? ?
            __ASM_EMIT("movss       0x90(%[src]), %%xmm3")  // xmm3 = s9 ? ? ?
            __ASM_EMIT("movss       0xa0(%[src]), %%xmm6")  // xmm6 = s10 ? ? ?
            __ASM_EMIT("movss       0xb0(%[src]), %%xmm7")  // xmm7 = s11 ? ? ?
            __ASM_EMIT("movlhps     %%xmm3, %%xmm2")        // xmm2 = s8 ? s9 ?
            __ASM_EMIT("movlhps     %%xmm7, %%xmm6")        // xmm6 = s10 ? s11 ?
            __ASM_EMIT("shufps      $0x88, %%xmm5, %%xmm1") // xmm1 = s4 s5 s6 s7

            __ASM_EMIT("movss       0xc0(%[src]), %%xmm3")  // xmm3 = s12 ? ? ?
            __ASM_EMIT("movss       0xd0(%[src]), %%xmm4")  // xmm4 = s13 ? ? ?
            __ASM_EMIT("movss       0xe0(%[src]), %%xmm7")  // xmm7 = s14 ? ? ?
            __ASM_EMIT("movss       0xf0(%[src]), %%xmm5")  // xmm5 = s15 ? ? ?
            __ASM_EMIT("movlhps     %%xmm4, %%xmm3")        // xmm3 = s12 ? s13 ?
            __ASM_EMIT("movlhps     %%xmm5, %%xmm7")        // xmm7 = s14 ? s15 ?
            __ASM_EMIT("shufps      $0x88, %%xmm6, %%xmm2") // xmm2 = s8 s9 s10 s11
            __ASM_EMIT("shufps      $0x88, %%xmm7, %%xmm3") // xmm3 = s12 s13 s14 s15

            __ASM_EMIT("movups      %%xmm0, 0x00(%[dst])")
            __ASM_EMIT("movups      %%xmm1, 0x10(%[dst])")
            __ASM_EMIT("movups      %%xmm2, 0x20(%[dst])")
            __ASM_EMIT("movups      %%xmm3, 0x30(%[dst])")

            // Repeat loop
            __ASM_EMIT("add         $0x100, %[src]")
            __ASM_EMIT("add         $0x40, %[dst]")
            __ASM_EMIT("sub         $16, %[count]")
            __ASM_EMIT("jae         1b")

            // 4x iterations
            __ASM_EMIT("2:")
            __ASM_EMIT("add         $12, %[count]")
            __ASM_EMIT("jl          4f")

            __ASM_EMIT("3:")
            __ASM_EMIT("movss       0x00(%[src]), %%xmm0")  // xmm0 = s0 ? ? ?
            __ASM_EMIT("movss       0x10(%[src]), %%xmm2")  // xmm2 = s1 ? ? ?
            __ASM_EMIT("movss       0x20(%[src]), %%xmm4")  // xmm4 = s2 ? ? ?
            __ASM_EMIT("movss       0x30(%[src]), %%xmm3")  // xmm3 = s3 ? ? ?
            __ASM_EMIT("movlhps     %%xmm2, %%xmm0")        // xmm0 = s0 ? s1 ?
            __ASM_EMIT("movlhps     %%xmm3, %%xmm4")        // xmm4 = s2 ? s3 ?
            __ASM_EMIT("shufps      $0x88, %%xmm4, %%xmm0") // xmm0 = s0 s1 s2 s3
            __ASM_EMIT("movups      %%xmm0, 0x00(%[dst])")

            __ASM_EMIT("add         $0x40, %[src]")
            __ASM_EMIT("add         $0x10, %[dst]")
            __ASM_EMIT("sub         $4, %[count]")
            __ASM_EMIT("jae         3b")

            // 1x iterations
            __ASM_EMIT("4:")
            __ASM_EMIT("add         $4, %[count]")
            __ASM_EMIT("jle         6f")

            __ASM_EMIT("5:")
            __ASM_EMIT("movss       0x00(%[src]), %%xmm0")  // xmm0 = s0
            __ASM_EMIT("movss       %%xmm0, 0x00(%[dst])")
            __ASM_EMIT("add         $0x10, %[src]")
            __ASM_EMIT("add         $0x4, %[dst]")
            __ASM_EMIT("dec         %[count]")
            __ASM_EMIT("jnz         5b")

            // End of routine
            __ASM_EMIT("6:")

            : [dst] "+r" (dst), [src] "+r" (src), [count] "+r" (count)
            :
            : "cc", "memory",
              "%xmm0", "%xmm1", "%xmm2", "%xmm3",
              "%xmm4", "%xmm5", "%xmm6", "%xmm7"
        );
    }

    void downsample_6x(float *dst, const float *src, size_t count)
    {
        ARCH_X86_ASM
        (
            __ASM_EMIT("sub         $16, %[count]")
            __ASM_EMIT("jb          2f")

            // 16 x sample loop
            __ASM_EMIT(".align 16")
            __ASM_EMIT("1:")
            __ASM_EMIT("movss       0x000(%[src]), %%xmm0") // xmm0 = s0 ? ? ?
            __ASM_EMIT("movss       0x018(%[src]), %%xmm2") // xmm2 = s1 ? ? ?
            __ASM_EMIT("movss       0x030(%[src]), %%xmm4") // xmm4 = s2 ? ? ?
            __ASM_EMIT("movss       0x048(%[src]), %%xmm3") // xmm3 = s3 ? ? ?
            __ASM_EMIT("movlhps     %%xmm2, %%xmm0")        // xmm0 = s0 ? s1 ?
            __ASM_EMIT("movlhps     %%xmm3, %%xmm4")        // xmm4 = s2 ? s3 ?

            __ASM_EMIT("movss       0x060(%[src]), %%xmm1") // xmm1 = s4 ? ? ?
            __ASM_EMIT("movss       0x078(%[src]), %%xmm2") // xmm2 = s5 ? ? ?
            __ASM_EMIT("movss       0x090(%[src]), %%xmm5") // xmm3 = s6 ? ? ?
            __ASM_EMIT("movss       0x0a8(%[src]), %%xmm3") // xmm5 = s7 ? ? ?
            __ASM_EMIT("movlhps     %%xmm2, %%xmm1")        // xmm1 = s4 ? s5 ?
            __ASM_EMIT("movlhps     %%xmm3, %%xmm5")        // xmm4 = s6 ? s7 ?
            __ASM_EMIT("shufps      $0x88, %%xmm4, %%xmm0") // xmm0 = s0 s1 s2 s3

            __ASM_EMIT("movss       0x0c0(%[src]), %%xmm2") // xmm2 = s8 ? ? ?
            __ASM_EMIT("movss       0x0d8(%[src]), %%xmm3") // xmm3 = s9 ? ? ?
            __ASM_EMIT("movss       0x0f0(%[src]), %%xmm6") // xmm6 = s10 ? ? ?
            __ASM_EMIT("movss       0x108(%[src]), %%xmm7") // xmm7 = s11 ? ? ?
            __ASM_EMIT("movlhps     %%xmm3, %%xmm2")        // xmm2 = s8 ? s9 ?
            __ASM_EMIT("movlhps     %%xmm7, %%xmm6")        // xmm6 = s10 ? s11 ?
            __ASM_EMIT("shufps      $0x88, %%xmm5, %%xmm1") // xmm1 = s4 s5 s6 s7

            __ASM_EMIT("movss       0x120(%[src]), %%xmm3") // xmm3 = s12 ? ? ?
            __ASM_EMIT("movss       0x138(%[src]), %%xmm4") // xmm4 = s13 ? ? ?
            __ASM_EMIT("movss       0x150(%[src]), %%xmm7") // xmm7 = s14 ? ? ?
            __ASM_EMIT("movss       0x168(%[src]), %%xmm5") // xmm5 = s15 ? ? ?
            __ASM_EMIT("movlhps     %%xmm4, %%xmm3")        // xmm3 = s12 ? s13 ?
            __ASM_EMIT("movlhps     %%xmm5, %%xmm7")        // xmm7 = s14 ? s15 ?
            __ASM_EMIT("shufps      $0x88, %%xmm6, %%xmm2") // xmm2 = s8 s9 s10 s11
            __ASM_EMIT("shufps      $0x88, %%xmm7, %%xmm3") // xmm3 = s12 s13 s14 s15

            __ASM_EMIT("movups      %%xmm0, 0x00(%[dst])")
            __ASM_EMIT("movups      %%xmm1, 0x10(%[dst])")
            __ASM_EMIT("movups      %%xmm2, 0x20(%[dst])")
            __ASM_EMIT("movups      %%xmm3, 0x30(%[dst])")

            // Repeat loop
            __ASM_EMIT("add         $0x180, %[src]")
            __ASM_EMIT("add         $0x40, %[dst]")
            __ASM_EMIT("sub         $16, %[count]")
            __ASM_EMIT("jae         1b")

            // 4x iterations
            __ASM_EMIT("2:")
            __ASM_EMIT("add         $12, %[count]")
            __ASM_EMIT("jl          4f")

            __ASM_EMIT("3:")
            __ASM_EMIT("movss       0x000(%[src]), %%xmm0") // xmm0 = s0 ? ? ?
            __ASM_EMIT("movss       0x018(%[src]), %%xmm2") // xmm2 = s1 ? ? ?
            __ASM_EMIT("movss       0x030(%[src]), %%xmm4") // xmm4 = s2 ? ? ?
            __ASM_EMIT("movss       0x048(%[src]), %%xmm3") // xmm3 = s3 ? ? ?
            __ASM_EMIT("movlhps     %%xmm2, %%xmm0")        // xmm0 = s0 ? s1 ?
            __ASM_EMIT("movlhps     %%xmm3, %%xmm4")        // xmm4 = s2 ? s3 ?
            __ASM_EMIT("shufps      $0x88, %%xmm4, %%xmm0") // xmm0 = s0 s1 s2 s3
            __ASM_EMIT("movups      %%xmm0, 0x00(%[dst])")

            __ASM_EMIT("add         $0x60, %[src]")
            __ASM_EMIT("add         $0x10, %[dst]")
            __ASM_EMIT("sub         $4, %[count]")
            __ASM_EMIT("jae         3b")

            // 1x iterations
            __ASM_EMIT("4:")
            __ASM_EMIT("add         $4, %[count]")
            __ASM_EMIT("jle         6f")

            __ASM_EMIT("5:")
            __ASM_EMIT("movss       0x000(%[src]), %%xmm0") // xmm0 = s0
            __ASM_EMIT("movss       %%xmm0, 0x00(%[dst])")
            __ASM_EMIT("add         $0x18, %[src]")
            __ASM_EMIT("add         $0x4, %[dst]")
            __ASM_EMIT("dec         %[count]")
            __ASM_EMIT("jnz         5b")

            // End of routine
            __ASM_EMIT("6:")

            : [dst] "+r" (dst), [src] "+r" (src), [count] "+r" (count)
            :
            : "cc", "memory",
              "%xmm0", "%xmm1", "%xmm2", "%xmm3",
              "%xmm4", "%xmm5", "%xmm6", "%xmm7"
        );
    }

    void downsample_8x(float *dst, const float *src, size_t count)
    {
        ARCH_X86_ASM
        (
            __ASM_EMIT("sub         $16, %[count]")
            __ASM_EMIT("jb          2f")

            // 16 x sample loop
            __ASM_EMIT(".align 16")
            __ASM_EMIT("1:")
            __ASM_EMIT("movss       0x000(%[src]), %%xmm0") // xmm0 = s0 ? ? ?
            __ASM_EMIT("movss       0x020(%[src]), %%xmm2") // xmm2 = s1 ? ? ?
            __ASM_EMIT("movss       0x040(%[src]), %%xmm4") // xmm4 = s2 ? ? ?
            __ASM_EMIT("movss       0x060(%[src]), %%xmm3") // xmm3 = s3 ? ? ?
            __ASM_EMIT("movlhps     %%xmm2, %%xmm0")        // xmm0 = s0 ? s1 ?
            __ASM_EMIT("movlhps     %%xmm3, %%xmm4")        // xmm4 = s2 ? s3 ?

            __ASM_EMIT("movss       0x080(%[src]), %%xmm1") // xmm1 = s4 ? ? ?
            __ASM_EMIT("movss       0x0a0(%[src]), %%xmm2") // xmm2 = s5 ? ? ?
            __ASM_EMIT("movss       0x0c0(%[src]), %%xmm5") // xmm3 = s6 ? ? ?
            __ASM_EMIT("movss       0x0e0(%[src]), %%xmm3") // xmm5 = s7 ? ? ?
            __ASM_EMIT("movlhps     %%xmm2, %%xmm1")        // xmm1 = s4 ? s5 ?
            __ASM_EMIT("movlhps     %%xmm3, %%xmm5")        // xmm4 = s6 ? s7 ?
            __ASM_EMIT("shufps      $0x88, %%xmm4, %%xmm0") // xmm0 = s0 s1 s2 s3

            __ASM_EMIT("movss       0x100(%[src]), %%xmm2") // xmm2 = s8 ? ? ?
            __ASM_EMIT("movss       0x120(%[src]), %%xmm3") // xmm3 = s9 ? ? ?
            __ASM_EMIT("movss       0x140(%[src]), %%xmm6") // xmm6 = s10 ? ? ?
            __ASM_EMIT("movss       0x160(%[src]), %%xmm7") // xmm7 = s11 ? ? ?
            __ASM_EMIT("movlhps     %%xmm3, %%xmm2")        // xmm2 = s8 ? s9 ?
            __ASM_EMIT("movlhps     %%xmm7, %%xmm6")        // xmm6 = s10 ? s11 ?
            __ASM_EMIT("shufps      $0x88, %%xmm5, %%xmm1") // xmm1 = s4 s5 s6 s7

            __ASM_EMIT("movss       0x180(%[src]), %%xmm3") // xmm3 = s12 ? ? ?
            __ASM_EMIT("movss       0x1a0(%[src]), %%xmm4") // xmm4 = s13 ? ? ?
            __ASM_EMIT("movss       0x1c0(%[src]), %%xmm7") // xmm7 = s14 ? ? ?
            __ASM_EMIT("movss       0x1e0(%[src]), %%xmm5") // xmm5 = s15 ? ? ?
            __ASM_EMIT("movlhps     %%xmm4, %%xmm3")        // xmm3 = s12 ? s13 ?
            __ASM_EMIT("movlhps     %%xmm5, %%xmm7")        // xmm7 = s14 ? s15 ?
            __ASM_EMIT("shufps      $0x88, %%xmm6, %%xmm2") // xmm2 = s8 s9 s10 s11
            __ASM_EMIT("shufps      $0x88, %%xmm7, %%xmm3") // xmm3 = s12 s13 s14 s15

            __ASM_EMIT("movups      %%xmm0, 0x00(%[dst])")
            __ASM_EMIT("movups      %%xmm1, 0x10(%[dst])")
            __ASM_EMIT("movups      %%xmm2, 0x20(%[dst])")
            __ASM_EMIT("movups      %%xmm3, 0x30(%[dst])")

            // Repeat loop
            __ASM_EMIT("add         $0x200, %[src]")
            __ASM_EMIT("add         $0x40, %[dst]")
            __ASM_EMIT("sub         $16, %[count]")
            __ASM_EMIT("jae         1b")

            // 4x iterations
            __ASM_EMIT("2:")
            __ASM_EMIT("add         $12, %[count]")
            __ASM_EMIT("jl          4f")

            __ASM_EMIT("3:")
            __ASM_EMIT("movss       0x000(%[src]), %%xmm0") // xmm0 = s0 ? ? ?
            __ASM_EMIT("movss       0x020(%[src]), %%xmm2") // xmm2 = s1 ? ? ?
            __ASM_EMIT("movss       0x040(%[src]), %%xmm4") // xmm4 = s2 ? ? ?
            __ASM_EMIT("movss       0x060(%[src]), %%xmm3") // xmm3 = s3 ? ? ?
            __ASM_EMIT("movlhps     %%xmm2, %%xmm0")        // xmm0 = s0 ? s1 ?
            __ASM_EMIT("movlhps     %%xmm3, %%xmm4")        // xmm4 = s2 ? s3 ?
            __ASM_EMIT("shufps      $0x88, %%xmm4, %%xmm0") // xmm0 = s0 s1 s2 s3
            __ASM_EMIT("movups      %%xmm0, 0x00(%[dst])")

            __ASM_EMIT("add         $0x80, %[src]")
            __ASM_EMIT("add         $0x10, %[dst]")
            __ASM_EMIT("sub         $4, %[count]")
            __ASM_EMIT("jae         3b")

            // 1x iterations
            __ASM_EMIT("4:")
            __ASM_EMIT("add         $4, %[count]")
            __ASM_EMIT("jle         6f")

            __ASM_EMIT("5:")
            __ASM_EMIT("movss       0x000(%[src]), %%xmm0") // xmm0 = s0
            __ASM_EMIT("movss       %%xmm0, 0x00(%[dst])")
            __ASM_EMIT("add         $0x20, %[src]")
            __ASM_EMIT("add         $0x4, %[dst]")
            __ASM_EMIT("dec         %[count]")
            __ASM_EMIT("jnz         5b")

            // End of routine
            __ASM_EMIT("6:")

            : [dst] "+r" (dst), [src] "+r" (src), [count] "+r" (count)
            :
            : "cc", "memory",
              "%xmm0", "%xmm1", "%xmm2", "%xmm3",
              "%xmm4", "%xmm5", "%xmm6", "%xmm7"
        );
    }
}

#endif /* DSP_ARCH_X86_SSE_RESAMPLING_H_ */
