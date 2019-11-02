#include <stdio.h>
#include <stdint.h>
#include <bigi.h>
#include <bigi_io.h>
#include "bigi_impl.h"
#include "bigi_io_impl.h"

int64_t hexdec(const char * hex)
{
    volatile int64_t ret;
    index_t i;
    /* char buff[20]; */

    /* UART1_Write_Text("HexDec:\n"); */
    for (ret = 0, i = 8; i > 0 && ret >= 0; i--)
    {
        ret <<= 4;
        /* sprintf(buff, "%016lx\n", ret); */
        /* UART1_Write_Text(buff); */
        ret |= (int64_t)hextable[(byte_t)*(hex++)];
        /* sprintf(buff, "%016lx\n", ret); */
        /* UART1_Write_Text(buff); */
    }

    /*while (i > 0 && ret >= 0)
    {
        ret = (ret << 4);
        sprintf(buff, "%016lx\n", ret);
        UART1_Write_Text(buff);
        UART1_Write_Text("buff: ");
        UART1_Write(*hex);
        UART1_Write('\n');
        ret = ret | (int64_t)(*hex++)-48;
        sprintf(buff, "%016lx\n", ret);
        UART1_Write_Text(buff);
        i--;
    }
    UART1_Write_Text("------------\n");*/

    return ret;
}

void bigi_to_hex(const bigi_t A,
                 const index_t words,
                 char *const str)
{
    index_t i;

    for (i = 0; i < words; i++)
    {
        sprintf(str, "%08ux", A[i]);
    }
}

/* compatible with python hex output */
void bigi_from_hex(const char *const str,
                   const index_t words,
                   bigi_t A)
{
    volatile index_t i,j, zeros;
    volatile int64_t ret;
    /* volatile char buff[20]; */

    for (i = 0, zeros = BIGI_MEMORY_WORDLEN-words; i < zeros; i++)
    {
        A[i] = 0;
    }

    /* UART1_Write_Text("Transform:\r\n"); */
    for (i = 0, j = zeros;
         i <= words * MACHINE_WORD_BYTELEN * 2; /* 2 chars per byte */
         i += MACHINE_WORD_BYTELEN * 2, j++)
    {
        ret = hexdec(&str[i]);
        /* sprintf(buff, "%016lx\n", ret); */
        /* UART1_Write_Text(buff); */
        A[j] = (word_t)ret;
    }
}

void bigi_print(const bigi_t A)
{
    index_t i;
#ifdef __MIKROC_PRO_FOR_ARM__
    char buff[20]; /* TODO: remove literal constants; 1 mw = 32 bits = 4 bytes = 8 hex chars */
    for (i = MONTG_DOM_BUFFER_WORDLEN; i < BIGI_MEMORY_WORDLEN; i++)
    {
        sprintf(buff, "%08lx", A[i]); /* TODO: remove literal constants */
        UART1_Write_Text(buff);
    }
#elif __x86_64__
    for (i = MONTG_DOM_BUFFER_WORDLEN; i < BIGI_MEMORY_WORDLEN; i++)
    {
        printf("%08x", A[i]); /* TODO: remove literal constants */
    }
#endif
}

#ifdef __MIKROC_PRO_FOR_ARM__
/* enter as ASCII 0x123456789... without newline */
void bigi_from_serial(const bigi_t A)
{
    /* load string from serial line */
    volatile char buff[4 + BIGI_BYTELEN]; /* TODO: remove literal constants */
    volatile index_t byte;

    for (byte = 0; byte < (BIGI_MEMORY_WORDLEN * 8) + 2; byte++) /* TODO: remove literal constants */
    {
        while (!UART1_Data_Ready());
        buff[byte] = UART1_Read();
    }

    /* UART1_Write_Text("byte = ");
    sprintf(buffer, "%d ", byte);
    UART1_Write_Text(buffer);
    UART1_Write('\n');
    UART1_Write_Text(buff);
    UART1_Write('\n'); */
    bigi_from_hex(buff, BIGI_MEMORY_WORDLEN-3, A);
    /* bigi_print(A); */
}
#endif
