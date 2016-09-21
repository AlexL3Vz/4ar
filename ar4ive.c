/**************************** 
 *** Cipher Algorithm 4AR ***
 ****************************
 *** by Alex L3Vz^ **********
 ****************************
 *** 1/8/2016 ***************
 ****************************
 */

#define UINT unsigned long long int
#define R register int

typedef struct AR4_KEY
{
    UINT li[2];
    UINT co[2];
} AR4_KEY;

#define BLOCK_SIZE 64
#define MAX_ROUNDS 128

#define CIPHER_SHIFT 1

#define BIT(m,o) ((m >> o) & 0x01)
#define ROL(m,o) (((m) << (o)) | ((m) >> (BLOCK_SIZE-(o))))
#define ROR(m,o) (((m) >> (o)) | ((m) << (BLOCK_SIZE-(o))))

#define MARK(ka,kb,i) (BIT(ka, i) | ((BIT(kb, i)) << 1))

#define CHOOSE(c,m)                                                      \
    switch(c)                                                            \
    {                                                                    \
        case 3: m = ~m;                  break;                          \
        case 1: m = ROL(m,CIPHER_SHIFT); break;                          \
        case 2: m = ROR(m,CIPHER_SHIFT); break;                          \
    }
/*        case 0: m ^= ((*key).co[0]^(*key).li[1]); break;                 \*/


/* WAVE_ROTOR (C) */
#define WAVE_SHIFT 11

#define ADDITION_MASK_P ((UINT) 0xF6262D1289FECCA3) // positive
#define ADDITION_MASK_N ((UINT) 0xEEBBAABBBB11EEEE) // negative

#define WAVE_L(m,o) (~(ROL(m,o) + ADDITION_MASK_P) - ADDITION_MASK_N)
#define WAVE_R(m,o) (~(ROR(m,o) - ADDITION_MASK_N) + ADDITION_MASK_P)
#define WAVE_ROTOR(ml,mr)                                                \
    ml = WAVE_L(ml,WAVE_SHIFT);                                          \
    mr = WAVE_R(mr,WAVE_SHIFT);


#define SCROLL_G(mm,m,cp)                                                \
    mm = 0LL;                           /* cleaning static var */        \
    R lg = BLOCK_SIZE; while(lg--)                                       \
    {                                                                    \
        mm |= BIT(m[lg],cp) << lg;                                       \
    }

#define SCROLL_S(mm,m,cp)                                                \
    R ls = BLOCK_SIZE; while(ls--)                                       \
    {                                                                    \
        m[ls] &= ~(((UINT) 0x01) << cp); /* clean needed bit */          \
        m[ls] |= BIT(mm,ls) << cp;                                       \
    }

                                                    
void ar4ive_encrypt(UINT * registru, AR4_KEY * key)
{
    static UINT messager = 0ULL;

    R horisont = BLOCK_SIZE; while(horisont--)
    {
        CHOOSE(
            MARK(
                (*key).li[0],
                (*key).li[1],
                horisont
                ),
            registru[horisont]
            );
    }

    R vertical = BLOCK_SIZE; while(vertical--)
    {
        SCROLL_G(messager,registru,vertical);
        CHOOSE(
            MARK(
                (*key).co[0],
                (*key).co[1],
                vertical
                ),
            messager
            );
        SCROLL_S(messager,registru,vertical);
    }
}

void ar4ive_decrypt(UINT * registru, AR4_KEY * key)
{
    static UINT messager = 0ULL;

    R vertical = BLOCK_SIZE; while(vertical--)
    {
        SCROLL_G(messager,registru,vertical);
        CHOOSE(
            MARK(
                (*key).co[1],
                (*key).co[0],
                vertical
                ),
            messager
            );
        SCROLL_S(messager,registru,vertical);
    }

    R horisont = BLOCK_SIZE; while(horisont--)
    {
        CHOOSE(
            MARK(
                (*key).li[1],
                (*key).li[0],
                horisont
                ),
            registru[horisont]
            );
    }
}