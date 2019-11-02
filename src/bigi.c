#include <stdint.h>
#include <bigi.h>
#include "bigi_impl.h"

/* TODO:
 *
 * safe coding: check inputs
 * replace unsigned integral types (int**_t)
 * MISRA: e.g., replace literal constants
 * get rid of return 1 if OK (possibly always return some status)
 *
 * */



/* -----------------------------------------------------------------------------
 *  Init, manipulate, compare, etc.
 *                                                                            */

void bigi_init_zero(bigi_t a) /* april */
{
    index_t i;
    for (i = 0; i < BIGI_MEMORY_WORDLEN; i++)
    {
        a[i] = 0;
    }
}

void bigi_copy(bigi_t out,
               const bigi_t in)
{
    index_t i;
    for (i = 0; i < BIGI_MEMORY_WORDLEN; i++)
    {
        out[i] = in[i];
    }
}

/*
Change bigi_t to CEC crypto API format
Change endianity, convert to an 8-bit array, change endianity back
*/
void bigi_to_bytes(byte_t *const out,
                   bigi_t in)
{
    index_t i,j;
    /* byte_t tmp; */
    byte_t *p;
    /* index_t rev = ((BIGI_MEMORY_WORDLEN*4)/2)-1; */

    /* TODO: why??? */
    bigi_change_endian(in);

    for (i = 0; i < BIGI_MEMORY_WORDLEN - MONTG_DOM_BUFFER_WORDLEN; i++)
    {
        p = (byte_t *)&in[i];
        /* use p[0],p[1],p[2],p[3] to access the bytes. */
        /* was: for (j = 3; j >= 0; j--) */
        /* TODO: remove literal const 4 which depends on machine word length */
        for (j = 0; j < MACHINE_WORD_BYTELEN; j++)
        {
            out[MACHINE_WORD_BYTELEN * i + j] =  p[j];
        }
    }
    bigi_change_endian(in);
    /*for (i = 0; i < rev; i++)
    {
    tmp = out[i];
    out[i] = out[(BIGI_MEMORY_WORDLEN*4) - i - 1];
    out[(BIGI_MEMORY_WORDLEN*4) - i - 1] = tmp;
    }*/
}

/*
Change CEC crypto API format to bigi_t
Change endianity & convert from 8bit array
*/
void bytes_to_bigi(bigi_t out,
                   const byte_t *const in)
{
    index_t i,j;
    byte_t *p;
    /* word_t tmp; */
    /* index_t rev = ((BIGI_MEMORY_WORDLEN*4)/2)-1; */

    /*for (i = 0; i < rev; i++)
    {
    tmp = in[i];
    in[i] = in[(BIGI_MEMORY_WORDLEN*4) - i - 1];
    in[(BIGI_MEMORY_WORDLEN*4) - i - 1] = tmp;
    }*/
    bigi_init_zero(out);

    for (i = 0; i < BIGI_MEMORY_WORDLEN - MONTG_DOM_BUFFER_WORDLEN; i++)
    {
        p = (byte_t *)&out[i];
        /* use p[0],p[1],p[2],p[3] to access the bytes. */
        for (j = 0; j < MACHINE_WORD_BYTELEN; j++)
        {
            /* TODO: dirty pointer arithmetics */
            p[j] = in[MACHINE_WORD_BYTELEN * i + j];
        }
    }
    /* TODO: why??? */
    bigi_change_endian(out);
}

word_t get_bit(const bigi_t A,
               const index_t index)
{
    /* return A[BIGI_MEMORY_WORDLEN-1-index/MACHINE_WORD_BITLEN] & (1 << (index - MACHINE_WORD_BITLEN*(index/MACHINE_WORD_BITLEN))); */
    volatile word_t word;
    volatile word_t shifted_one;

    word = A[BIGI_MEMORY_WORDLEN - 1 - index/MACHINE_WORD_BITLEN];
    shifted_one = (1 << (index - MACHINE_WORD_BITLEN*(index/MACHINE_WORD_BITLEN)));
    word = word & shifted_one;

    /* TODO: return word >> (index - MACHINE_WORD_BITLEN*(index/MACHINE_WORD_BITLEN)); */
    return word;
}

void bigi_change_endian(bigi_t A) /* april */
{
    index_t i, j;
    word_t tmp;

    for (i = BIGI_MEMORY_WORDLEN - 1, j = 0; i > j; i--, j++)
    {
        tmp  = A[i];
        A[i] = A[j];
        A[j] = tmp;
    }
}

byte_t bigi_eq(const bigi_t A,
               const bigi_t B)
{
    index_t i;
    for (i = 0; i < BIGI_MEMORY_WORDLEN; i++)
    {
        if (A[i] != B[i]) return 0;
    }
    return 1;
}

byte_t bigi_ge(const bigi_t A,
               const bigi_t B)
{
    index_t i;
    for (i = 0; i < BIGI_MEMORY_WORDLEN; i++)
    {
        if (A[i] > B[i])
            return 1;
        else if (A[i] < B[i])
            return 0;
    }
    /* TODO: ge is really intended as > or >= ??? */
    return 0;
}

byte_t bigi_le(const bigi_t A,
               const bigi_t B)
{
    index_t i;
    for (i = 0; i < BIGI_MEMORY_WORDLEN; i++)
    {
        if (A[i] < B[i])
            return 1;
        else if (A[i] > B[i])
            return 0;
    }
    /* TODO: same as for ge */
    return 0;
}

byte_t bigi_is_zero(const bigi_t A)
{
    index_t i;
    for (i = 0; i < BIGI_MEMORY_WORDLEN; i++)
    {
        if (A[i] != MIN_VAL) return 0;
    }
    return 1;
}



/* -----------------------------------------------------------------------------
 *  Arithmetics
 *                                                                            */

void bigi_shift_left(bigi_t A,
                     const index_t count)
{
    index_t i,j;

    if (count == 0) return;
    for (i = 0; i < count; i++)
    {
        A[0] <<= 1;
        for (j = 1; j < BIGI_MEMORY_WORDLEN; j++)
        {
            if (A[j] & (1<<(MACHINE_WORD_BITLEN-1)))
                A[j-1]++;
            A[j] <<= 1;
        }
    }
}

void bigi_shift_right(bigi_t A,
                      const index_t count)
{
    int32_t i,j;

    if (count == 0) return;
    /* optimize for shifts > machine word */
    for (i = 0; i < count; i++)
    {
        A[BIGI_MEMORY_WORDLEN-1] >>= 1;
        for (j = BIGI_MEMORY_WORDLEN-2; j >= 0; j--)
        {
            if (A[j] & 1)
                A[j+1] += FIRST_ONE;
            A[j] >>= 1;
        }
    }
}

void bigi_and(const bigi_t A,
              const bigi_t B,
              bigi_t RES)
{
    index_t i;
    for (i = 0; i < BIGI_MEMORY_WORDLEN; i++)
    {
        RES[i] = A[i] & B[i];
    }
}

void bigi_or(const bigi_t A,
             const bigi_t B,
             bigi_t RES)
{
    index_t i;
    for (i = 0; i < BIGI_MEMORY_WORDLEN; i++)
    {
        RES[i] = A[i] | B[i];
    }
}

void bigi_add(const bigi_t A,
              const bigi_t B,
              bigi_t RES)
{
    dword_t tmp;
    byte_t carry = 0;
    int32_t i;

    for (i = BIGI_MEMORY_WORDLEN-1; i >= 0; i--)
    {
        tmp = (dword_t)A[i] + B[i] + carry;
        carry = (tmp > MAX_VAL);
        tmp &= MAX_VAL;
        RES[i] = (word_t)tmp;
    }
}

void bigi_add_one(const bigi_t A,
                  bigi_t RES)
{
    dword_t tmp;
    byte_t carry = 1;
    int32_t i;

    for (i = BIGI_MEMORY_WORDLEN-1; carry > 0; i--)
    {
        tmp = (dword_t)(A[i]) + carry;
        carry = (tmp > MAX_VAL);
        tmp &= MAX_VAL;
        RES[i] = (word_t)tmp;
    }

    for (; i >= 0; i--) /* TODO: i >= MONTG_DOM_BUFFER_WORDLEN */
    {
        RES[i] = A[i];
    }
}

void bigi_sub(const bigi_t A,
              const bigi_t B,
              bigi_t RES)
{
    volatile dword_t r;
    volatile word_t borrow = 0;
    int32_t i;

    for (i = BIGI_MEMORY_WORDLEN-1; i >= 0 ; i--)
    {
        r = (dword_t)A[i] + MAX_VAL + 1 - B[i] - borrow;
        borrow = (r <= MAX_VAL);
        r &= MAX_VAL;
        RES[i] = (word_t)r;
    }
}

void bigi_sub_one(const bigi_t A,
                  bigi_t RES)
{
    volatile dword_t r;
    volatile word_t borrow = 1;
    int32_t i;

    for (i = BIGI_MEMORY_WORDLEN-1; borrow > 0 ; i--)
    {
        r = (dword_t)A[i] + MAX_VAL + 1 - borrow;
        borrow = (r <= MAX_VAL);
        r &= MAX_VAL;
        RES[i] = (word_t)r;
    }

    for (; i >= 0; i--) /* TODO: i >= MONTG_DOM_BUFFER_WORDLEN, but: might be used in Montgomery domain? */
    {
        RES[i] = A[i];
    }
}

void bigi_mod_red(const bigi_t A, /* april */
                  const bigi_t MOD,
                  bigi_t RES)
{
    bigi_t FOO;
    bigi_div(A,MOD,RES,FOO);
}

void bigi_red_barret(const bigi_t X,
                     const bigi_t MOD,
                     const bigi_t MU,
                     bigi_t RES)
{
    bigi_t q1,q2,q3,bk,r1,r2;
    index_t i,k;
    bigi_init_zero(q1);
    bigi_init_zero(q2);
    bigi_init_zero(q3);
    bigi_init_zero(bk);

    /*determine k*/
    for (k = 0, i = 0; i < BIGI_MEMORY_WORDLEN; i++)
    {
        if (MOD[i] != 0)
        {
            k = i;
            break;
        }
    }

    bk[k] = 1;
    bigi_copy(q1,X);
    bigi_shift_left(q1, MACHINE_WORD_BITLEN * (k-1));
    bigi_mult_fit(q1,MU,q2);
    bigi_copy(q3,q2);
    bigi_shift_left(q2, MACHINE_WORD_BITLEN * (k+1));
    /*r1,r2*/
    bigi_copy(r1,X);
    for (i = 0; i < BIGI_MEMORY_WORDLEN; i++)
    {
        if (i < k + 1)
        {
            r1[i] = 0;
        }
    }

    bigi_mult_fit(q3,MOD,r2);
    for (i = 0; i < BIGI_MEMORY_WORDLEN; i++)
    {
        if (i < k + 1)
        {
            r2[i] = 0;
        }
    }

    if (bigi_ge(r2,r1))
    {
        r1[k+2] = 1;
    }

    bigi_sub(r1,r2,RES);
}

/*for multiplying bigi_t by machine word*/
void bigi_mult_word(const bigi_t A,
                    const word_t B,
                    bigi_t RES)
{
    dword_t k, t;
    index_t i, j, m = 0, n = 0;
    bigi_t TA;
    bigi_copy(TA,A);

    for (i = 0; i < BIGI_MEMORY_WORDLEN; i++)
    {
        if (TA[i] != 0)
        {
            m = BIGI_MEMORY_WORDLEN - i;
            break;
        }
    }

    n = 1;
    bigi_init_zero(RES);
    bigi_change_endian(TA);

    for (j = 0; j < n; j++)
    {
        for (k = 0, i = 0; i < m; i++)
        {
            t  = (dword_t)TA[i];
            t *= (dword_t)B;
            t += (dword_t)RES[i + j];
            t += k;
            RES[i + j] = (word_t)t;          /* (I.e., t & 0xFFFF). */
            k = t >> MACHINE_WORD_BITLEN;
        }
        RES[j + m] = (word_t)k;
    }
    bigi_change_endian(RES);
}

/*only for numbers where m = n and m*n fits in BIGI_MEMORY_WORDLEN-1 words*/
/*Knuth's Algorithm M from [Knuth Vol. 2 Third edition (1998)] Hacker's Delight*/
void bigi_mult_fit(const bigi_t A,
                   const bigi_t B,
                   bigi_t RES)
{
    dword_t k, t;
    index_t i, j, m = 0, n = 0;
    bigi_t TA,TB;
    bigi_copy(TA,A);
    bigi_copy(TB,B);

    for (i = 0; i < BIGI_MEMORY_WORDLEN; i++)
    {
        if (TA[i] != 0)
        {
            m = BIGI_MEMORY_WORDLEN - i;
            break;
        }
    }

    for (i = 0; i < BIGI_MEMORY_WORDLEN; i++)
    {
        if (TB[i] != 0)
        {
            n = BIGI_MEMORY_WORDLEN - i;
            break;
        }
    }

    for (i = 0; i < BIGI_MEMORY_WORDLEN; i++)
        RES[i] = 0;
    /*change endianity of operands*/
    /*for (i = 0; i < (BIGI_MEMORY_WORDLEN/2)-1; i++)
    {
    tmp1 = TA[i];
    tmp2 = TB[i];
    TA[i] = TA[BIGI_MEMORY_WORDLEN-i-1];
    TA[BIGI_MEMORY_WORDLEN-i-1] = tmp1;
    TB[i] = TB[BIGI_MEMORY_WORDLEN-i-1];
    TB[BIGI_MEMORY_WORDLEN-i-1] = tmp2;
    }
    i = BIGI_MEMORY_WORDLEN - 1;
    j = 0;
    while (i > j)
    {
    tmp1 = TA[i];
    tmp2 = TB[i];
    TA[i] = TA[j];
    TB[i] = TB[j];
    TA[j] = tmp1;
    TB[j] = tmp2;
    i--;
    j++;
    }*/
    bigi_change_endian(TA);
    bigi_change_endian(TB);

    for (j = 0; j < n; j++)
    {
        for (k = 0, i = 0; i < m; i++)
        {
            t  = (dword_t)TA[i];
            t *= (dword_t)TB[j];
            t += (dword_t)RES[i + j];
            t += k;
            RES[i + j] = (word_t)t;          /* (I.e., t & 0xFFFF). */
            k = t >> MACHINE_WORD_BITLEN;
        }
        RES[j + m] = (word_t)k;
    }

    /*for (i = 0; i < (BIGI_MEMORY_WORDLEN/2)-1; i++)
    {
    tmp1 = RES[i];
    RES[i] = RES[BIGI_MEMORY_WORDLEN - i - 1];
    RES[BIGI_MEMORY_WORDLEN - i - 1] = tmp1;
    }
    i = BIGI_MEMORY_WORDLEN - 1;
    j = 0;
    while (i > j)
    {
    tmp1 = RES[i];
    RES[i] = RES[j];
    RES[j] = tmp1;
    i--;
    j++;
    }*/
    bigi_change_endian(RES);
}

void bigi_mult_mod(const bigi_t A,
                   const bigi_t B,
                   const bigi_t MOD,
                   bigi_t RES)
{
    bigi_t TR;
    int32_t i;

    for (i = 0; i < BIGI_MEMORY_WORDLEN; i++)
    {
        TR[i] = MIN_VAL;
    }

    for (i = BIGI_BITLEN-1; i >= 0; i--)
    {
        bigi_shift_left(TR,1);

        if (bigi_ge(TR,MOD) || bigi_eq(TR,MOD))
            bigi_sub(TR, MOD, TR);

        if (bigi_ge(TR,MOD) || bigi_eq(TR,MOD))
            bigi_sub(TR, MOD, TR);

        if (get_bit(B, i))
            bigi_add(A, TR, TR);

        if (bigi_ge(TR,MOD) || bigi_eq(TR,MOD))
            bigi_sub(TR, MOD, TR);

        if (bigi_ge(TR,MOD) || bigi_eq(TR,MOD))
            bigi_sub(TR, MOD, TR);
    }

    for (i = 0; i < BIGI_MEMORY_WORDLEN; i++)
    {
        RES[i] = TR[i];
    }
}

void bigi_mult_mod_mont(const bigi_t A,
                        const bigi_t B,
                        const bigi_t MOD,
                        const word_t MU,
                        bigi_t RES)
{
    bigi_t C;
    bigi_t TMP;
    dword_t q;
    index_t i, n_len;
    bigi_init_zero(C);

    /* for length of modulus MOD */
    /* this could be input */
    for (i = 0; i < BIGI_MEMORY_WORDLEN; i++) /* TODO: not a proper initialization of n_len */
    {
        if (MOD[i] != 0)
        {
            n_len = i;
        }
    }

    for (i = n_len; i >= 3; i--) /* TODO: 3 = ? */
    {
        bigi_mult_word(B,A[i],TMP);
        bigi_add(TMP,C,C);
        q  = (dword_t)MU * (dword_t)C[BIGI_MEMORY_WORDLEN-1];
        q %= (dword_t)(MAX_VAL + 1);
        bigi_mult_word(MOD,(word_t)q,TMP);
        bigi_add(TMP,C,C);
        /* this could be faster */
        bigi_shift_right(C, MACHINE_WORD_BITLEN);
    }

    if (bigi_ge(C,MOD))
        bigi_sub(C,MOD,C);

    bigi_copy(RES,C);
}

void bigi_mod_exp(const bigi_t A,
                  const bigi_t B,
                  const bigi_t MOD,
                  bigi_t RES)
{
    bigi_t A_loc, B_loc;
    index_t i;

    bigi_copy(A_loc, A);
    bigi_copy(B_loc, B);

    for (i = 0; i < BIGI_MEMORY_WORDLEN; i++)
    {
        RES[i] = MIN_VAL;
    }
    RES[BIGI_MEMORY_WORDLEN-1] = LAST_ONE;

    while (!bigi_is_zero(B_loc))
    {
        if (B_loc[BIGI_MEMORY_WORDLEN-1] & 1)
            bigi_mult_mod(RES,A_loc,MOD,RES);
        bigi_shift_right(B_loc,1);
        bigi_mult_mod(A_loc,A_loc,MOD,A_loc);
    }
}

void bigi_mod_exp_mont(const bigi_t A,
                       const bigi_t B,
                       const bigi_t MOD,
                       bigi_t RES)
{
    bigi_t B_loc;
    bigi_t R;
    bigi_t m0_inv;
    bigi_t M_PRIME;
    bigi_t A_R, B_R;
    word_t MU;
    index_t i;

    bigi_copy(B_loc, B);
    bigi_init_zero(R);

    for (i = 0; i < BIGI_MEMORY_WORDLEN; i++)
    {
        if (MOD[i] != 0)
        {
            R[i-1] = 1;
            break;
        }
    }

    bigi_div(R,MOD,R,A_R);
    /*algorithm 3 from paper*/
    bigi_init_zero(A_R);
    bigi_init_zero(M_PRIME);
    A_R[BIGI_MEMORY_WORDLEN-2] = 1; /* TODO: 2 = ? */
    M_PRIME[BIGI_MEMORY_WORDLEN-1] = MOD[BIGI_MEMORY_WORDLEN-1];
    bigi_mod_inv(M_PRIME, A_R, m0_inv);
    bigi_init_zero(M_PRIME);
    M_PRIME[BIGI_MEMORY_WORDLEN-1] = MAX_VAL;
    bigi_mult_mod(M_PRIME,m0_inv, A_R, m0_inv);
    MU = m0_inv[BIGI_MEMORY_WORDLEN-1];
    bigi_mult_mod(A,R,MOD,A_R);
    bigi_copy(B_R,R);

    /* TODO: ineffective */
    while (!bigi_is_zero(B_loc))
    {
        if (B_loc[BIGI_MEMORY_WORDLEN-1] & 1)
        {
            bigi_mult_mod_mont(A_R,B_R,MOD,MU,B_R);
        }
        bigi_shift_right(B_loc,1);
        bigi_mult_mod_mont(A_R,A_R,MOD,MU,A_R);
    }

    bigi_init_zero(A_R);
    A_R[BIGI_MEMORY_WORDLEN-1] = 1;
    bigi_mult_mod_mont(A_R,B_R,MOD,MU,B_R);
    bigi_copy(RES,B_R);
    /* dbg_print(RES); */
}

/* https://rosettacode.org/wiki/Modular_inverse#C */
byte_t bigi_mod_inv(const bigi_t A,
                    const bigi_t MOD,
                    bigi_t RES)
{
    bigi_t T, NT, R, NR, Q, TMP;
    bigi_t TMP2, TMP1,ONE;
    index_t i;

    for (i = 0; i < BIGI_MEMORY_WORDLEN; i++)
    {
        T[i] = 0;
        R[i] = MOD[i];
        NT[i] = 0;
        Q[i] = 0;
        TMP1[i] = 0;
    }

    NT[BIGI_MEMORY_WORDLEN-1] = 1;
    bigi_copy(ONE,NT);
    /* bigi_mult_mod(A,NT,MOD,NR); */
    bigi_copy(NR,A);

    while (!bigi_is_zero(NR) && !bigi_eq(NR,MOD))
    {
        bigi_div(R,NR,TMP1,Q);
        bigi_copy(TMP, NT);
        bigi_mult_mod(Q,NT,MOD,TMP2);

        if (bigi_ge(T,TMP2))
        {
            bigi_sub(T,TMP2,NT);
        }
        else
        {
            bigi_sub(TMP2,T,NT);
            bigi_sub(MOD,NT,NT);
        }

        bigi_copy(T,TMP);
        bigi_copy(TMP, NR);
        bigi_mult_mod(Q,NR,MOD,TMP2);
        bigi_copy(NR,TMP1);
        bigi_copy(R,TMP);
    }

    if (bigi_ge(R,ONE))
    {
        return 1;
    }

    for ( i = 0; i < BIGI_MEMORY_WORDLEN; i++)
    {
        RES[i] = T[i];
    }

    return 0;
}

/*! \brief Computes A/B
 * Most complicated operation for multi precision integers.
 * Algorithm 14.20 from Handbook of applied cryptography
 * @param Dividend
 * @param Divisor
 * @param Quotient A/B
 */
/* Implementation of Knuth's Algorithm D from http://www.hackersdelight.org/hdcodetxt/divmnu.c.txt (edited) */
byte_t bigi_div(const bigi_t A, /* april, except for s which was int */
                const bigi_t B,
                bigi_t REM,
                bigi_t RES)
{
    const dword_t b = ((dword_t)1 << MACHINE_WORD_BITLEN); /* Number base (2**MACHINE_WORD_BITLEN). */
    bigi_t un, vn;
    dword_t qhat;                   /* Estimated quotient digit. */
    dword_t rhat, tmp1_big, tmp2_big;                   /* A remainder. */
    dword_t p,tmp;                      /* Product of two digits. */
    int64_t t, k;
    byte_t s;
    int32_t i, j;
    bigi_t u,v;
    /* word_t tmp1, tmp2; */
    word_t tmp2;
    int32_t m = 0, n = 0;
    /* index_t rev = (BIGI_MEMORY_WORDLEN/2)-1; */

    bigi_copy(u,A);
    bigi_copy(v,B);

    for (i = 0; i < BIGI_MEMORY_WORDLEN; i++)
    {
        un[i] = 0;
        vn[i] = 0;
        RES[i]  = 0;
        if (REM != 0) REM[i] = 0;
    }

    for (i = 0; i < BIGI_MEMORY_WORDLEN; i++)
    {
        if (u[i] != 0)
        {
            m = BIGI_MEMORY_WORDLEN - i;
            break;
        }
    }

    for (i = 0; i < BIGI_MEMORY_WORDLEN; i++)
    {
        if (v[i] != 0)
        {
            n = BIGI_MEMORY_WORDLEN - i;
            break;
        }
    }

    /*change endianity of operands*/
    /*for (i = 0; i < rev; i++)
    {
    tmp1 = u[i];
    tmp2 = v[i];
    u[i] = u[BIGI_MEMORY_WORDLEN - i - 1];
    u[BIGI_MEMORY_WORDLEN - i - 1] = tmp1;
    v[i] = v[BIGI_MEMORY_WORDLEN - i - 1];
    v[BIGI_MEMORY_WORDLEN - i - 1] = tmp2;
    }*/
    bigi_change_endian(u);
    bigi_change_endian(v);

    if (m < n || n <= 0 || v[n-1] == 0)
    {
        return 1;
    }

    /* single word division */
    if (n == 1)
    {
        for (k = 0, j = m - 1; j >= 0; j--)
        {
            tmp =  ((k << MACHINE_WORD_BITLEN) + u[j])/ v[0];
            RES[j] = (word_t) tmp;
            k = ((k << MACHINE_WORD_BITLEN) + u[j])- RES[j]*v[0];
        }

        if (REM != 0) REM[0] = (word_t)k;
        /* revert endianity back */
        /*for (i = 0; i < rev; i++)
        {
        tmp1 = RES[i];
        RES[i] = RES[BIGI_MEMORY_WORDLEN - i - 1];
        RES[BIGI_MEMORY_WORDLEN - i - 1] = tmp1;*/
        bigi_change_endian(RES);
        if (REM != 0) bigi_change_endian(REM);

        return 0;
    }
    /* Normalize by shifting v left just enough so that its high-order
    bit is on, and shift u left the same amount. We may have to append a
    high-order digit on the dividend; we do that unconditionally. */

    s = nlz(v[n-1]);
    /* 0 <= s <= 31. */
    for (i = n - 1; i > 0; i--)
    {
        /* vn[i] = (v[i] << s) | ((dword_t)v[i-1] >> (MACHINE_WORD_BITLEN-s)); */
        vn[i] = v[i] << s;
        tmp2 = (dword_t)v[i-1] >> (MACHINE_WORD_BITLEN-s);
        vn[i] = (vn[i] | tmp2);
    }
    vn[0] = v[0] << s;

    un[m] = (dword_t)u[m-1] >> (MACHINE_WORD_BITLEN-s);
    for (i = m - 1; i > 0; i--)
    {
        /* un[i] = (u[i] << s) | ((dword_t)u[i-1] >> (MACHINE_WORD_BITLEN-s)); */
        un[i] = u[i] << s;
        tmp = (dword_t)u[i-1] >> (MACHINE_WORD_BITLEN-s);
        un[i] = (un[i] | (word_t)tmp);
    }
    un[0] = u[0] << s;

    for (j = m - n; j >= 0; j--)
    {
        /* qhat = (un[j+n]*b + un[j+n-1])/vn[n-1]; */
        qhat  = (dword_t)un[j+n];
        qhat  = (qhat << MACHINE_WORD_BITLEN);
        qhat += (dword_t)un[j+n-1];
        qhat /= vn[n-1];

        /* rhat = (un[j+n]*b + un[j+n-1]) - qhat*vn[n-1]; */
        rhat  = (dword_t)un[j+n];
        rhat  = (rhat << MACHINE_WORD_BITLEN);
        rhat += (dword_t)un[j+n-1];
        tmp   = qhat*(dword_t)vn[n-1];
        rhat -= tmp;

        /* TODO: REMOVE GOTO */
        again:
        tmp1_big = qhat;
        tmp1_big *= (dword_t)vn[n-2];
        tmp2_big = (rhat << MACHINE_WORD_BITLEN);
        tmp2_big += un[j+n-2];
        /* if (qhat >= b || qhat*vn[n-2] > b*rhat + un[j+n-2])
        { */
        if (qhat >= b || tmp1_big > tmp2_big)
        {
            qhat = qhat - 1;
            rhat += (dword_t)vn[n-1];
            if (rhat < b) goto again;
        }

        for (k = 0, i = 0; i < n; i++)
        {
            p = qhat*vn[i];
            t = (dword_t)un[i+j] - k - (p & MAX_VAL);
            un[i+j] = (word_t)t;
            k = (p >> MACHINE_WORD_BITLEN) - (t >> MACHINE_WORD_BITLEN);
        }
        t = (dword_t)un[j+n] - k;
        un[j+n] = (word_t)t;
        RES[j] = (word_t)qhat;

        if (t < 0)
        {
            RES[j] = RES[j] - 1;
            for (k = 0, i = 0; i < n; i++)
            {
                t = (dword_t)un[i+j];
                t += (dword_t)vn[i];
                t += k;
                un[i+j] = t;
                k = t >> MACHINE_WORD_BITLEN;
            }
            un[j+n] = un[j+n] + k;
        }
    } /* End j. */
    /* change endianity of result */
    /*for ( i = 0; i < rev; i++)
    {
    tmp1 = RES[i];
    RES[i] = RES[BIGI_MEMORY_WORDLEN - i - 1];
    RES[BIGI_MEMORY_WORDLEN - i - 1] = tmp1;
    }*/
    bigi_change_endian(RES);

    if (REM != 0)
    {
        for (i = 0; i < n-1; i++)
        {
            REM[i] = (word_t)(un[i] >> s);
            tmp = (dword_t)un[i+1] << (MACHINE_WORD_BITLEN-s);
            REM[i] = REM[i] | (word_t) tmp;
        }
        REM[n-1] = un[n-1] >> s;
        /*for ( i = 0; i < rev; i++)
        {
        tmp1 = REM[i];
        REM[i] = REM[BIGI_MEMORY_WORDLEN - i - 1];
        REM[BIGI_MEMORY_WORDLEN - i - 1] = tmp1;
        }*/
        bigi_change_endian(REM);
    }
    return 0;
}

/*! \brief Computes gcd(A,B)
 * Binary GCD (Stein's algorithm) from https://en.wikipedia.org/wiki/Binary_GCD_algorithm
 * @param A
 * @param B
 * @param Result
 */
void bigi_gcd(const bigi_t A,
              const bigi_t B,
              bigi_t RES)
{
    bigi_t ONE;
    bigi_t TMP;
    bigi_t A_big, B_big;
    index_t shift;
    word_t i;
    word_t index_a = 0;
    word_t index_b = 0;
    word_t index = 0;
    word_t a;
    word_t b;
    bigi_t T;

    bigi_copy(A_big, A);
    bigi_copy(B_big, B);

    for (i = 0; i < BIGI_MEMORY_WORDLEN; i++)
    {
        ONE[i] = 0;
    }
    ONE[BIGI_MEMORY_WORDLEN-1] = 1;

    if (bigi_is_zero(A_big)) bigi_copy(RES, B);
    if (bigi_is_zero(B_big)) bigi_copy(RES, A);

    for (shift = BIGI_MEMORY_WORDLEN-1; shift >= 0; shift--)
    {
        if (A_big[shift] != 0 || B_big[shift] != 0)
        {
            /* find index in word */
            index_a = 0;
            index_b = 0;
            index = 0;
            a = A_big[shift];
            b = B_big[shift];
            /* little hack taken from http://graphics.stanford.edu/~seander/bithacks.html */
            /* TODO: check out how this works for different mw bit-lengths */
            index_a = MultiplyDeBruijnBitPosition[((word_t)((a & -a) * 0x077CB531U)) >> 27];
            index_b = MultiplyDeBruijnBitPosition[((word_t)((b & -b) * 0x077CB531U)) >> 27];

            if (A_big[shift] == 0)
                index = index_b;
            else if (B_big[shift] == 0)
                index = index_a;
            else if (index_a < index_b)
                index = index_a;
            else
                index = index_b;

            shift = (BIGI_MEMORY_WORDLEN - 1 - shift) * MACHINE_WORD_BITLEN + index;
            break;
        }
    }

    bigi_shift_right(A_big, shift);
    bigi_shift_right(B_big, shift);

    bigi_and(A_big, ONE, TMP);

    while (bigi_is_zero(TMP))
    {
        bigi_shift_right(A_big,1);
        bigi_and(A_big, ONE, TMP);
    }

    do
    {
        bigi_and(B_big, ONE, TMP);
        while (bigi_is_zero(TMP))
        {
            bigi_shift_right(B_big,1);
            bigi_and(B_big, ONE, TMP);
        }
        if (bigi_ge(A_big,B_big))
        {
            /* T = B_big; */
            bigi_copy(T,B_big);
            /* B_big = A_big; */
            bigi_copy(B_big,A_big);
            /* A_big = T; */
            bigi_copy(A_big,T);
        }
        bigi_sub(B_big,A_big,B_big);
    } while (!bigi_is_zero(B_big));

    bigi_shift_left(A_big,shift);
    for (i = 0; i < BIGI_MEMORY_WORDLEN; i++)
    {
        RES[i] = A_big[i];
    }
}



/* -----------------------------------------------------------------------------
 *  Misc
 *                                                                            */

byte_t nlz(word_t x)
{
    byte_t n;

    if (x == 0) return(MACHINE_WORD_BITLEN);

    n = 0;
#if MACHINE_WORD_BITLEN == 8
    if (x <= 0x0F)    {n = n + 4; x = x << 4;}
    if (x <= 0x3F)    {n = n + 2; x = x << 2;}
    if (x <= 0x7F)    {n = n + 1;}
#elif MACHINE_WORD_BITLEN == 16
    if (x <= 0x00FF)    {n = n + 8; x = x << 8;}
    if (x <= 0x0FFF)    {n = n + 4; x = x << 4;}
    if (x <= 0x3FFF)    {n = n + 2; x = x << 2;}
    if (x <= 0x7FFF)    {n = n + 1;}
#elif MACHINE_WORD_BITLEN == 32
    if (x <= 0x0000FFFF)    {n = n +16; x = x <<16;}
    if (x <= 0x00FFFFFF)    {n = n + 8; x = x << 8;}
    if (x <= 0x0FFFFFFF)    {n = n + 4; x = x << 4;}
    if (x <= 0x3FFFFFFF)    {n = n + 2; x = x << 2;}
    if (x <= 0x7FFFFFFF)    {n = n + 1;}
#else
    #error MSG_WRONG_MW_BITLEN
#endif

    return n;
}
