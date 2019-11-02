#ifndef BIGI_IO_H
#define BIGI_IO_H

#include <stdint.h>
#include <bigi.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  @brief        Convert bigi_t to a hexa string
 *
 *  @param[in]    bigi_t A
 *  @param[in]    Number of machine words to be printed
 *  @param[out]   Pointer to an output string (char buffer)
 *
 */
void bigi_to_hex(const bigi_t A,
                 const index_t words,
                 char *const str);

/**
 *  @brief        Load bigi_t from a hexa string
 *
 *  @param[in]    Pointer to the input string (char buffer)
 *  @param[in]    Number of machine words to be loaded
 *  @param[out]   bigi_t A to be loaded
 *
 */
void bigi_from_hex(const char *const str,
                   const index_t words,
                   bigi_t A);

/**
 *  @brief        Print bigi_t in hexa format
 *
 *  @param[in]    bigi_t A to be printed
 *
 */
void bigi_print(const bigi_t A);

#ifdef __MIKROC_PRO_FOR_ARM__
/**
 *  @brief        Read bigi_t from serial link
 *
 *  @param[out]   bigi_t A to be loaded
 *
 */
void bigi_from_serial(const bigi_t A);
#endif /* #ifdef __MIKROC_PRO_FOR_ARM__ */

#ifdef __cplusplus
}
#endif

#endif
