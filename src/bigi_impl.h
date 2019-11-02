#ifndef BIGI_IMPL_H
#define BIGI_IMPL_H

#include <bigi.h>

/* MAX_VAL must be double word type due to comparisons with numbers larger than machine word */
#if MACHINE_WORD_BITLEN == 8
    #define MAX_VAL         ((dword_t)0xFF)
    #define MIN_VAL         ( (word_t)0x00)
    #define FIRST_ONE       ( (word_t)0x80)
    #define LAST_ONE        ( (word_t)0x01)
#elif MACHINE_WORD_BITLEN == 16
    #define MAX_VAL         ((dword_t)0xFFFF)
    #define MIN_VAL         ( (word_t)0x0000)
    #define FIRST_ONE       ( (word_t)0x8000)
    #define LAST_ONE        ( (word_t)0x0001)
#elif MACHINE_WORD_BITLEN == 32
    #define MAX_VAL         ((dword_t)0xFFFFFFFF)
    #define MIN_VAL         ( (word_t)0x00000000)
    #define FIRST_ONE       ( (word_t)0x80000000)
    #define LAST_ONE        ( (word_t)0x00000001)
#else
    #error MSG_WRONG_MW_BITLEN
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  @brief        Black magic
 *
 *  @param        Unsigned integer
 *  @return       TBD
 *
 */
byte_t nlz(word_t x);

static const word_t MultiplyDeBruijnBitPosition[32] =
{
     0,  1, 28,  2, 29, 14, 24,  3, 30, 22, 20, 15, 25, 17,  4,  8,
    31, 27, 13, 23, 21, 19, 16,  7, 26, 12, 18,  6, 11,  5, 10,  9
};

#ifdef __cplusplus
}
#endif

#endif
