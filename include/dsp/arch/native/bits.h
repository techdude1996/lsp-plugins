/*
 * bits.h
 *
 *  Created on: 06 февр. 2016 г.
 *      Author: sadko
 */

#ifndef DSP_ARCH_NATIVE_BITS_H_
#define DSP_ARCH_NATIVE_BITS_H_

// Unsigned implementation
inline uint8_t      reverse_bits(uint8_t src)
{
    return __rb[src];
}

inline uint16_t     reverse_bits(uint16_t v)
{
    return (uint16_t(__rb[v & 0xff]) << 8) | (uint16_t(__rb[v >> 8]));
}

inline uint32_t     reverse_bits(uint32_t v)
{
    return
        (uint32_t(__rb[v >> 24])) |
        (uint32_t(__rb[(v >> 16) & 0xff]) << 8) |
        (uint32_t(__rb[(v >> 8) & 0xff]) << 16) |
        (uint32_t(__rb[v & 0xff]) << 24);
}

inline uint64_t     reverse_bits(uint64_t v)
{
    return
        (uint64_t(__rb[v >> 56])) |
        (uint64_t(__rb[(v >> 48) & 0xff]) << 8) |
        (uint64_t(__rb[(v >> 40) & 0xff]) << 16) |
        (uint64_t(__rb[(v >> 32) & 0xff]) << 24) |
        (uint64_t(__rb[(v >> 24) & 0xff]) << 32) |
        (uint64_t(__rb[(v >> 16) & 0xff]) << 40) |
        (uint64_t(__rb[(v >> 8) & 0xff]) << 48) |
        (uint64_t(__rb[v & 0xff]) << 56);
}

inline uint8_t      reverse_bits(uint8_t v, size_t count)
{
    return reverse_bits(v) >> (sizeof(uint8_t) * 8 - count);
}

inline uint16_t     reverse_bits(uint16_t v, size_t count)
{
    return reverse_bits(v) >> (sizeof(uint16_t) * 8 - count);
}

inline uint32_t     reverse_bits(uint32_t v, size_t count)
{
    return reverse_bits(v) >> (sizeof(uint32_t) * 8 - count);
}

inline uint64_t     reverse_bits(uint64_t v, size_t count)
{
    return reverse_bits(v) >> (sizeof(uint64_t) * 8 - count);
}

// Signed implementation
inline int8_t __lsp_forced_inline      reverse_bits(int8_t v)
{
    return reverse_bits(uint8_t(v));
}

inline int16_t __lsp_forced_inline     reverse_bits(int16_t v)
{
    return reverse_bits(uint16_t(v));
}

inline int32_t __lsp_forced_inline     reverse_bits(int32_t v)
{
    return reverse_bits(uint32_t(v));
}

inline int64_t __lsp_forced_inline     reverse_bits(int64_t v)
{
    return reverse_bits(uint64_t(v));
}

inline int8_t __lsp_forced_inline      reverse_bits(int8_t v, size_t count)
{
    return reverse_bits(uint8_t(v), count);
}

inline int16_t __lsp_forced_inline     reverse_bits(int16_t v, size_t count)
{
    return reverse_bits(uint16_t(v), count);
}

inline int32_t __lsp_forced_inline     reverse_bits(int32_t v, size_t count)
{
    return reverse_bits(uint32_t(v), count);
}

inline int64_t __lsp_forced_inline     reverse_bits(int64_t v, size_t count)
{
    return reverse_bits(uint64_t(v), count);
}

inline int __lsp_forced_inline     int_log2(uint8_t v)
{
    int res = 0;
    if (v & 0xf0) { res += 4; v >>= 4; }
    if (v & 0x0c) { res += 2; v >>= 2; }
    if (v & 0x02) ++res;
    return res;
}

inline int __lsp_forced_inline     int_log2(uint16_t v)
{
    int res = 0;
    if (v & 0xff00) { res += 8; v >>= 8; }
    if (v & 0xf0) { res += 4; v >>= 4; }
    if (v & 0x0c) { res += 2; v >>= 2; }
    if (v & 0x02) ++res;
    return res;
}

inline int __lsp_forced_inline     int_log2(uint32_t v)
{
    int res = 0;
    if (v & 0xffff0000) { res += 16; v >>= 16; }
    if (v & 0xff00) { res += 8; v >>= 8; }
    if (v & 0xf0) { res += 4; v >>= 4; }
    if (v & 0x0c) { res += 2; v >>= 2; }
    if (v & 0x02) ++res;
    return res;
}

inline int __lsp_forced_inline     int_log2(uint64_t v)
{
    int res = 0;
    if (v & 0xffffffff00000000ULL) { res += 32; v >>= 32; }
    if (v & 0xffff0000) { res += 16; v >>= 16; }
    if (v & 0xff00) { res += 8; v >>= 8; }
    if (v & 0xf0) { res += 4; v >>= 4; }
    if (v & 0x0c) { res += 2; v >>= 2; }
    if (v & 0x02) ++res;
    return res;
}

inline int __lsp_forced_inline     int_log2(int8_t v)
{
    return int_log2(uint8_t(v));
}

inline int __lsp_forced_inline     int_log2(int16_t v)
{
    return int_log2(uint16_t(v));
}

inline int __lsp_forced_inline     int_log2(int32_t v)
{
    return int_log2(uint32_t(v));
}

inline int __lsp_forced_inline     int_log2(int64_t v)
{
    return int_log2(uint64_t(v));
}

#endif /* DSP_ARCH_NATIVE_BITS_H_ */
