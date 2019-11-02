#ifndef BIGI_H
#define BIGI_H

#include <stdint.h>



/* *****************************************************************************
 *
 *  TYPES & DEFINITIONS
 *                                                                            */

/* -----------------------------------------------------------------------------
 *  General types & definitions
 *                                                                            */
#define BITS_IN_BYTE 8
#define MONTG_DOM_BUFFER_WORDLEN 3
#define MSG_WRONG_MW_BITLEN "Unsupported machine word bit-length."
typedef uint8_t byte_t;

/* -----------------------------------------------------------------------------
 *  bigi bit-/byte-length
 *                                                                            */
#ifndef BIGI_BITLEN
    #define BIGI_BITLEN 512
#endif
#define BIGI_BYTELEN ((BIGI_BITLEN)/(BITS_IN_BYTE))

/* -----------------------------------------------------------------------------
 *  Machine word bit-/byte-length (customizable, not necessarily by platform)
 *                                                                            */
#ifndef MACHINE_WORD_BITLEN
    #define MACHINE_WORD_BITLEN 32
#endif
#define MACHINE_WORD_BYTELEN ((MACHINE_WORD_BITLEN)/(BITS_IN_BYTE))
/** check that bigi bit-length is a multiple of machine word bit-length **/
#if BIGI_BITLEN % MACHINE_WORD_BITLEN != 0
    #error "Intended bigi bit-length is not a multiple of machine word bit-length."
#endif

/* -----------------------------------------------------------------------------
 *  typedef machine word type
 *                                                                            */
/* TODO: comment that 8- and 16-bit compatibility is not ready yet (only rare cases) */
#if MACHINE_WORD_BITLEN ==      8
    typedef uint8_t             word_t;
    typedef uint16_t            dword_t;
    #define DOUBLE_BASE         (((dword_t)1) << 8)
    #if BIGI_BITLEN <=          128         /* 2^(8-1) */
        typedef word_t          index_t;
    #elif BIGI_BITLEN <=        32768       /* 2^(16-1) */
        typedef dword_t         index_t;
    #else
        #error "BIGI_BITLEN > 32,768 (too large for double word indexing, incl. indexing single bits)."
    #endif
#elif MACHINE_WORD_BITLEN ==    16
    typedef uint16_t            word_t;
    typedef uint32_t            dword_t;
    #define DOUBLE_BASE         (((dword_t)1) << 16)
    #if BIGI_BITLEN <=          32768       /* 2^(16-1) */
        typedef word_t          index_t;
    #elif BIGI_BITLEN <=        2147483648  /* 2^(32-1) */
        typedef dword_t         index_t;
    #else
        #error "BIGI_BITLEN > 2,147,483,648 (too large for double word indexing, incl. indexing single bits)."
    #endif
#elif (MACHINE_WORD_BITLEN ==   32) || (MACHINE_WORD_BITLEN == 64) /* use max 64-bit types */
    typedef uint32_t            word_t;
    typedef uint64_t            dword_t;
    #define DOUBLE_BASE         (((dword_t)1) << 32)
    #if BIGI_BITLEN <=          2147483648   /* 2^(32-1) */
        typedef word_t          index_t;
        /** no use for numbers that need to be indexed with 64-bit numbers (enormously large) **/
    #else
        #error "BIGI_BITLEN > 2,147,483,648 (too large for word indexing, incl. indexing single bits)."
    #endif
#else
    #error MSG_WRONG_MW_BITLEN
#endif

/* -----------------------------------------------------------------------------
 *  bigi type length and typedef
 *                                                                            */
#define BIGI_WORDLEN ((BIGI_BITLEN) / (MACHINE_WORD_BITLEN))
#define BIGI_MEMORY_WORDLEN ((BIGI_WORDLEN) + (MONTG_DOM_BUFFER_WORDLEN))
#define BIGI_MEMORY_BYTELEN ((BIGI_MEMORY_WORDLEN) * (MACHINE_WORD_BYTELEN))
typedef word_t bigi_t[BIGI_MEMORY_WORDLEN];

/* -----------------------------------------------------------------------------
 *  Print summary
 *                                                                            */
#ifdef PRINT_PREPROC_SUMMARY
    #define TO_STR(x) #x
    #define PRINT_VAR(var) #var " = "  TO_STR(var)

    #pragma message(PRINT_VAR(BIGI_BITLEN))
    #pragma message(PRINT_VAR(MACHINE_WORD_BITLEN))
    #pragma message(PRINT_VAR(BIGI_MEMORY_WORDLEN))
#endif



/* *****************************************************************************
 *
 *      FUNCTIONS
 *                                                                            */

#ifdef __cplusplus
extern "C" {
#endif

/* -----------------------------------------------------------------------------
 *  Init, manipulate, compare, etc.
 *                                                                            */

/**
 *  @brief        Initialize bigi_t with zeroes
 *
 *  @param[in]    bigi_t to be initialized
 *
 */
void bigi_init_zero(bigi_t a);

/**
 *  @brief        Copy bigi_t
 *
 *  @param[out]   bigi_t destination
 *  @param[in]    bigi_t source
 *
 */
void bigi_copy(bigi_t out,
               const bigi_t in);

/**
 *  @brief        Convert bigi_t from an array of machine words to an array of bytes
 *
 *  @param[out]   Pointer to the array of bytes (BIGI_MEMORY_BYTELEN bytes long)
 *  @param[in]    bigi_t to be converted
 *
 */
void bigi_to_bytes(byte_t *const out,
                   bigi_t in);

/**
 *  @brief        Convert an array of bytes to bigi_t
 *
 *  @param[out]   bigi_t to be loaded
 *  @param[in]    Pointer to the array of bytes (BIGI_MEMORY_BYTELEN bytes long)
 *
 */
void bytes_to_bigi(bigi_t out,
                   const byte_t *const in);

/**
 *  @brief        Returns the value of a bit at given position within bigi_t
 *
 *  @param[in]    bigi_t
 *  @param[in]    Position of desired bit, must be within [0..BIGI_BITLEN-1]
 *  @return       {0,1}
 */
word_t get_bit(const bigi_t A,
               const index_t index);

/**
 *  @brief        Changes endianity of bigi_t
 *
 *  @param[in]    bigi_t to be converted
 *
 */
void bigi_change_endian(bigi_t A);

/**
 *  @brief        Compare two bigi_t's ( A == B )
 *
 *  @param[in]    bigi_t A
 *  @param[in]    bigi_t B
 *  @return       0 when not equal, 1 if equal   TODO: change this
 *
 */
byte_t bigi_eq(const bigi_t A,
               const bigi_t B);

/**
 *  @brief        Compare two bigi_t's ( A >= B )
 *
 *  @param[in]    bigi_t A
 *  @param[in]    bigi_t B
 *  @return       0 when not greater equal, 1 if greater or equal
 *
 */
byte_t bigi_ge(const bigi_t A,
               const bigi_t B);

/**
 *  @brief        Compare two bigi_t's ( A <= B )
 *
 *  @param[in]    bigi_t A
 *  @param[in]    bigi_t B
 *  @return       0 when not lower or equal, 1 if lower or equal
 *
 */
byte_t bigi_le(const bigi_t A,
               const bigi_t B);

/**
 *  @brief        Check if bigi_t is zero ( A == 0 )
 *
 *  @param[in]    bigi_t to be compared
 *  @return       0 if A != 0, 1 if A == 0
 *
 */
byte_t bigi_is_zero(const bigi_t A);

/* -----------------------------------------------------------------------------
 *  Arithmetics
 *                                                                            */

/**
 *  @brief        Left shift
 *
 *  @param[in]    bigi_t to be shifted
 *  @param[in]    Amount of bits to be shifted by
 *
 */
void bigi_shift_left(bigi_t A,
                     const index_t count);

/**
 *  @brief        Right shift
 *
 *  @param[in]    bigi_t to be shifted
 *  @param[in]    Amount of bits to be shifted by
 *
 */
void bigi_shift_right(bigi_t A,
                      const index_t count);

/**
 *  @brief        Bitwise logical AND
 *
 *  @param[in]    bigi_t A
 *  @param[in]    bigi_t B
 *  @param[out]   Result
 *
 */
void bigi_and(const bigi_t A,
              const bigi_t B,
              bigi_t RES);

/**
 *  @brief        Bitwise logical OR
 *
 *  @param[in]    bigi_t A
 *  @param[in]    bigi_t B
 *  @param[out]   Result
 *
 */
void bigi_or(const bigi_t A,
             const bigi_t B,
             bigi_t RES);

/**
 *  @brief        Addition
 *
 *  @param[in]    bigi_t A
 *  @param[in]    bigi_t B
 *  @param[out]   Result
 *
 */
void bigi_add(const bigi_t A,
              const bigi_t B,
              bigi_t RES);

/**
 *  @brief        Increment by 1
 *
 *  @param[in]    bigi_t to be incremented
 *  @param[out]   Result
 *
 */
void bigi_add_one(const bigi_t A,
                  bigi_t RES);

/**
 *  @brief        Subtraction
 *
 *  @param[in]    bigi_t A
 *  @param[in]    bigi_t B
 *  @param[out]   Result
 *
 */
void bigi_sub(const bigi_t A,
              const bigi_t B,
              bigi_t RES);

/**
 *  @brief        Decrement by 1
 *
 *  @param[in]    bigi_t to be decremented
 *  @param[out]   Result
 *
 */
void bigi_sub_one(const bigi_t A,
                  bigi_t RES);

/**
 *  @brief        Modular reduction A % MOD
 *
 *  @param[in]    bigi_t A
 *  @param[in]    bigi_t MOD (modulus)
 *  @param[out]   Result
 *
 */
void bigi_mod_red(const bigi_t A,
                  const bigi_t MOD,
                  bigi_t RES);

/**
 *  @brief        Barrett's modular reduction A % MOD
 *
 *  @param[in]    bigi_t A
 *  @param[in]    bigi_t MOD (modulus)
 *  @param[in]    bigi_t MU (Barrett's pre-computed constant mu)
 *  @param[out]   Result
 *
 */
void bigi_red_barret(const bigi_t X,
                     const bigi_t MOD,
                     const bigi_t MU,
                     bigi_t RES);

/**
 *  @brief        Multiplication by a machine word
 *
 *  @param[in]    bigi_t A
 *  @param[in]    machine word B
 *  @param[out]   Result
 *
 */
void bigi_mult_word(const bigi_t A,
                    const word_t B,
                    bigi_t RES);

/**
 *  @brief        Multiplication where the result fits into bigi_t
 *
 *  @param[in]    bigi_t A
 *  @param[in]    bigi_t B
 *  @param[out]   Result
 *
 */
void bigi_mult_fit(const bigi_t A,
                   const bigi_t B,
                   bigi_t RES);

/**
 *  @brief        Modular multiplication
 *
 *  @param[in]    bigi_t A
 *  @param[in]    bigi_t B
 *  @param[in]    bigi_t MOD (modulus)
 *  @param[out]   Result
 *
 */
void bigi_mult_mod(const bigi_t A,
                   const bigi_t B,
                   const bigi_t MOD,
                   bigi_t RES);

/**
 *  @brief        Modular multiplication in Mongomery domain
 *
 *  @param[in]    bigi_t A (in Montgomery domain)
 *  @param[in]    bigi_t B (in Montgomery domain)
 *  @param[in]    bigi_t MOD (modulus)
 *  @param[in]    machine word MU (Montgomery domain parameter)
 *  @param[out]   Result (in Montgomery domain)
 *
 */
void bigi_mult_mod_mont(const bigi_t A,
                        const bigi_t B,
                        const bigi_t MOD,
                        const word_t MU,
                        bigi_t RES);

/**
 *  @brief        Modular exponentiation
 *
 *  @param[in]    bigi_t A (base)
 *  @param[in]    bigi_t B (exponent)
 *  @param[in]    bigi_t MOD (modulus)
 *  @param[out]   Result
 *
 */
void bigi_mod_exp(const bigi_t A,
                  const bigi_t B,
                  const bigi_t MOD,
                  bigi_t RES);

/**
 *  @brief        Modular exponentiation that internally uses Mongomery domain multiplication
 *
 *  @param[in]    bigi_t A (base)
 *  @param[in]    bigi_t B (exponent)
 *  @param[in]    bigi_t MOD (modulus)
 *  @param[out]   Result
 *
 */
void bigi_mod_exp_mont(const bigi_t A,
                       const bigi_t B,
                       const bigi_t MOD,
                       bigi_t RES);

/**
 *  @brief        Modular inverse
 *
 *  @param[in]    bigi_t A to be inverted
 *  @param[in]    bigi_t MOD (modulus)
 *  @param[out]   Result
 *  @return       1 if the inverse exist, 0 if the inverse does not exist
 *
 */
byte_t bigi_mod_inv(const bigi_t A,
                    const bigi_t MOD,
                    bigi_t RES);

/**
 *  @brief        Integer division
 *
 *  @param[in]    bigi_t A
 *  @param[in]    bigi_t B
 *  @param[out]   Remainder, if NULL pointer is provided, only quotient is computed
 *  @param[out]   Quotient
 *
 */
byte_t bigi_div(const bigi_t A,
                const bigi_t B,
                bigi_t REM,
                bigi_t RES);

/**
 *  @brief        Greatest Common Divisor
 *
 *  @param[in]    bigi_t A
 *  @param[in]    bigi_t B
 *  @param[out]   Result
 *
 */
void bigi_gcd(const bigi_t A,
              const bigi_t B,
              bigi_t RES);

#ifdef __cplusplus
}
#endif /* #ifdef __cplusplus */

#endif /* #ifndef BIGI_H */