#include "AES.h"
#include "stdio.h"
/*
* Expand the cipher key into the encryption key schedule and return the
* number of rounds for the given cipher key size.
*/
int aes_setkey_enc(unsigned int rk[], const unsigned char cipherKey[], int keyBytes)
{
    int i = 0;
    unsigned int temp;

    rk[0] = GETU32(cipherKey     );
    rk[1] = GETU32(cipherKey +  4);
    rk[2] = GETU32(cipherKey +  8);
    rk[3] = GETU32(cipherKey + 12);
    if (keyBytes == 16) { // 128 bits
        for (;;) {
            temp  = rk[3];
            rk[4] = rk[0] ^
                (Te4[(temp >> 16) & 0xff] & 0xff000000) ^
                (Te4[(temp >>  8) & 0xff] & 0x00ff0000) ^
                (Te4[(temp      ) & 0xff] & 0x0000ff00) ^
                (Te4[(temp >> 24)       ] & 0x000000ff) ^
                rcon[i];
            rk[5] = rk[1] ^ rk[4];
            rk[6] = rk[2] ^ rk[5];
            rk[7] = rk[3] ^ rk[6];
            if (++i == 10) {
                return 10;
            }
            rk += 4;
        }
    }
    rk[4] = GETU32(cipherKey + 16);
    rk[5] = GETU32(cipherKey + 20);
    if (keyBytes == 24) { // 192 bits
        for (;;) {
            temp = rk[ 5];
            rk[ 6] = rk[ 0] ^
                (Te4[(temp >> 16) & 0xff] & 0xff000000) ^
                (Te4[(temp >>  8) & 0xff] & 0x00ff0000) ^
                (Te4[(temp      ) & 0xff] & 0x0000ff00) ^
                (Te4[(temp >> 24)       ] & 0x000000ff) ^
                rcon[i];
            rk[ 7] = rk[ 1] ^ rk[ 6];
            rk[ 8] = rk[ 2] ^ rk[ 7];
            rk[ 9] = rk[ 3] ^ rk[ 8];
            if (++i == 8) {
                return 12;
            }
            rk[10] = rk[ 4] ^ rk[ 9];
            rk[11] = rk[ 5] ^ rk[10];
            rk += 6;
        }
    }
    rk[6] = GETU32(cipherKey + 24);
    rk[7] = GETU32(cipherKey + 28);
    if (keyBytes == 32) { // 256 bits
        for (;;) {
            temp = rk[ 7];
            rk[ 8] = rk[ 0] ^
                (Te4[(temp >> 16) & 0xff] & 0xff000000) ^
                (Te4[(temp >>  8) & 0xff] & 0x00ff0000) ^
                (Te4[(temp      ) & 0xff] & 0x0000ff00) ^
                (Te4[(temp >> 24)       ] & 0x000000ff) ^
                rcon[i];
            rk[ 9] = rk[ 1] ^ rk[ 8];
            rk[10] = rk[ 2] ^ rk[ 9];
            rk[11] = rk[ 3] ^ rk[10];
            if (++i == 7) {
                return 14;
            }
            temp = rk[11];
            rk[12] = rk[ 4] ^
                (Te4[(temp >> 24)       ] & 0xff000000) ^
                (Te4[(temp >> 16) & 0xff] & 0x00ff0000) ^
                (Te4[(temp >>  8) & 0xff] & 0x0000ff00) ^
                (Te4[(temp      ) & 0xff] & 0x000000ff);
            rk[13] = rk[ 5] ^ rk[12];
            rk[14] = rk[ 6] ^ rk[13];
            rk[15] = rk[ 7] ^ rk[14];

            rk += 8;
        }
    }
    return 0;
}

/*
* Expand the cipher key into encryption and decryption key schedule and
* return the number of rounds for the given cipher key size.
*/
int AesGenKeySched(unsigned int rk[], unsigned int rrk[], const unsigned char cipherKey[], int keyBytes)
{
    int Nr, i;

    // expand the cipher key
    Nr = aes_setkey_enc(rk, cipherKey, keyBytes);
    // invert the order of the first round keys
    rrk += Nr * 4;
    rrk[0] = rk[0];
    rrk[1] = rk[1];
    rrk[2] = rk[2];
    rrk[3] = rk[3];

   /*
    * apply the inverse MixColumn transform to all round keys but the first
    * and the last
    */
    for (i = 1; i < Nr; i++) {
        rrk -= 4;
        rk += 4;
        rrk[0] =
            Td0[Te4[(rk[0] >> 24)       ] & 0xff] ^
            Td1[Te4[(rk[0] >> 16) & 0xff] & 0xff] ^
            Td2[Te4[(rk[0] >>  8) & 0xff] & 0xff] ^
            Td3[Te4[(rk[0]      ) & 0xff] & 0xff];
        rrk[1] =
            Td0[Te4[(rk[1] >> 24)       ] & 0xff] ^
            Td1[Te4[(rk[1] >> 16) & 0xff] & 0xff] ^
            Td2[Te4[(rk[1] >>  8) & 0xff] & 0xff] ^
            Td3[Te4[(rk[1]      ) & 0xff] & 0xff];
        rrk[2] =
            Td0[Te4[(rk[2] >> 24)       ] & 0xff] ^
            Td1[Te4[(rk[2] >> 16) & 0xff] & 0xff] ^
            Td2[Te4[(rk[2] >>  8) & 0xff] & 0xff] ^
            Td3[Te4[(rk[2]      ) & 0xff] & 0xff];
        rrk[3] =
            Td0[Te4[(rk[3] >> 24)       ] & 0xff] ^
            Td1[Te4[(rk[3] >> 16) & 0xff] & 0xff] ^
            Td2[Te4[(rk[3] >>  8) & 0xff] & 0xff] ^
            Td3[Te4[(rk[3]      ) & 0xff] & 0xff];
    }
    // invert the order of the last round keys
    rrk -= 4;
    rk += 4;
    rrk[0] = rk[0];
    rrk[1] = rk[1];
    rrk[2] = rk[2];
    rrk[3] = rk[3];

    return Nr;
}

/*
* Encrypt the plain text into cipher
*/
void AesEncBlk(AesCtx *pCtx, const unsigned char pt[], unsigned char ct[])
{
    unsigned int s0, s1, s2, s3, t0, t1, t2, t3, *iv;
    const unsigned int *rk;
    int r;

    rk = pCtx->Ek;
    iv = pCtx->Iv;
    /*
     * map byte array block to cipher state
     * and add initial round key:
     */
    s0 = GETU32(pt     ) ^ rk[0];
    s1 = GETU32(pt +  4) ^ rk[1];
    s2 = GETU32(pt +  8) ^ rk[2];
    s3 = GETU32(pt + 12) ^ rk[3];
    if (pCtx->Mode) {
        s0 = s0 ^ iv[0];
        s1 = s1 ^ iv[1];
        s2 = s2 ^ iv[2];
        s3 = s3 ^ iv[3];
    }
    /*
     * Nr - 1 full rounds:
     */
    r = pCtx->Nr >> 1;
    for (;;) {
        t0 =
            Te0[(s0 >> 24)       ] ^
            Te1[(s1 >> 16) & 0xff] ^
            Te2[(s2 >>  8) & 0xff] ^
            Te3[(s3      ) & 0xff] ^
            rk[4];
        t1 =
            Te0[(s1 >> 24)       ] ^
            Te1[(s2 >> 16) & 0xff] ^
            Te2[(s3 >>  8) & 0xff] ^
            Te3[(s0      ) & 0xff] ^
            rk[5];
        t2 =
            Te0[(s2 >> 24)       ] ^
            Te1[(s3 >> 16) & 0xff] ^
            Te2[(s0 >>  8) & 0xff] ^
            Te3[(s1      ) & 0xff] ^
            rk[6];
        t3 =
            Te0[(s3 >> 24)       ] ^
            Te1[(s0 >> 16) & 0xff] ^
            Te2[(s1 >>  8) & 0xff] ^
            Te3[(s2      ) & 0xff] ^
            rk[7];

        rk += 8;
        if (--r == 0) {
            break;
        }

        s0 =
            Te0[(t0 >> 24)       ] ^
            Te1[(t1 >> 16) & 0xff] ^
            Te2[(t2 >>  8) & 0xff] ^
            Te3[(t3      ) & 0xff] ^
            rk[0];
        s1 =
            Te0[(t1 >> 24)       ] ^
            Te1[(t2 >> 16) & 0xff] ^
            Te2[(t3 >>  8) & 0xff] ^
            Te3[(t0      ) & 0xff] ^
            rk[1];
        s2 =
            Te0[(t2 >> 24)       ] ^
            Te1[(t3 >> 16) & 0xff] ^
            Te2[(t0 >>  8) & 0xff] ^
            Te3[(t1      ) & 0xff] ^
            rk[2];
        s3 =
            Te0[(t3 >> 24)       ] ^
            Te1[(t0 >> 16) & 0xff] ^
            Te2[(t1 >>  8) & 0xff] ^
            Te3[(t2      ) & 0xff] ^
            rk[3];
    }
    /*
     * apply last round and
     * map cipher state to byte array block:
     */
    s0 =
        (Te4[(t0 >> 24)       ] & 0xff000000) ^
        (Te4[(t1 >> 16) & 0xff] & 0x00ff0000) ^
        (Te4[(t2 >>  8) & 0xff] & 0x0000ff00) ^
        (Te4[(t3      ) & 0xff] & 0x000000ff) ^
        rk[0];
    PUTU32(ct     , s0);
    s1 =
        (Te4[(t1 >> 24)       ] & 0xff000000) ^
        (Te4[(t2 >> 16) & 0xff] & 0x00ff0000) ^
        (Te4[(t3 >>  8) & 0xff] & 0x0000ff00) ^
        (Te4[(t0      ) & 0xff] & 0x000000ff) ^
        rk[1];
    PUTU32(ct +  4, s1);
    s2 =
        (Te4[(t2 >> 24)       ] & 0xff000000) ^
        (Te4[(t3 >> 16) & 0xff] & 0x00ff0000) ^
        (Te4[(t0 >>  8) & 0xff] & 0x0000ff00) ^
        (Te4[(t1      ) & 0xff] & 0x000000ff) ^
        rk[2];
    PUTU32(ct +  8, s2);
    s3 =
        (Te4[(t3 >> 24)       ] & 0xff000000) ^
        (Te4[(t0 >> 16) & 0xff] & 0x00ff0000) ^
        (Te4[(t1 >>  8) & 0xff] & 0x0000ff00) ^
        (Te4[(t2      ) & 0xff] & 0x000000ff) ^
        rk[3];
    PUTU32(ct + 12, s3);

    if (pCtx->Mode) {
        iv[0] = s0;
        iv[1] = s1;
        iv[2] = s2;
        iv[3] = s3;
    }
}

/*
* Decrypt the cipher into plain text
*/
void AesDecBlk(AesCtx *pCtx, const unsigned char ct[], unsigned char pt[])
{
    unsigned int s0, s1, s2, s3, t0, t1, t2, t3, v0, v1, v2, v3, *iv;
    const unsigned int *rk;
    int r;

    rk = pCtx->Dk;
    iv = pCtx->Iv;
    /*
     * map byte array block to cipher state
     * and add initial round key:
     */
    v0 = GETU32(ct     );    s0 = v0 ^ rk[0];
    v1 = GETU32(ct +  4);    s1 = v1 ^ rk[1];
    v2 = GETU32(ct +  8);    s2 = v2 ^ rk[2];
    v3 = GETU32(ct + 12);    s3 = v3 ^ rk[3];
    /*
     * Nr - 1 full rounds:
     */
    r = pCtx->Nr >> 1;
    for (;;) {
        t0 =
            Td0[(s0 >> 24)       ] ^
            Td1[(s3 >> 16) & 0xff] ^
            Td2[(s2 >>  8) & 0xff] ^
            Td3[(s1      ) & 0xff] ^
            rk[4];
        t1 =
            Td0[(s1 >> 24)       ] ^
            Td1[(s0 >> 16) & 0xff] ^
            Td2[(s3 >>  8) & 0xff] ^
            Td3[(s2      ) & 0xff] ^
            rk[5];
        t2 =
            Td0[(s2 >> 24)       ] ^
            Td1[(s1 >> 16) & 0xff] ^
            Td2[(s0 >>  8) & 0xff] ^
            Td3[(s3      ) & 0xff] ^
            rk[6];
        t3 =
            Td0[(s3 >> 24)       ] ^
            Td1[(s2 >> 16) & 0xff] ^
            Td2[(s1 >>  8) & 0xff] ^
            Td3[(s0      ) & 0xff] ^
            rk[7];

        rk += 8;
        if (--r == 0) {
            break;
        }

        s0 =
            Td0[(t0 >> 24)       ] ^
            Td1[(t3 >> 16) & 0xff] ^
            Td2[(t2 >>  8) & 0xff] ^
            Td3[(t1      ) & 0xff] ^
            rk[0];
        s1 =
            Td0[(t1 >> 24)       ] ^
            Td1[(t0 >> 16) & 0xff] ^
            Td2[(t3 >>  8) & 0xff] ^
            Td3[(t2      ) & 0xff] ^
            rk[1];
        s2 =
            Td0[(t2 >> 24)       ] ^
            Td1[(t1 >> 16) & 0xff] ^
            Td2[(t0 >>  8) & 0xff] ^
            Td3[(t3      ) & 0xff] ^
            rk[2];
        s3 =
            Td0[(t3 >> 24)       ] ^
            Td1[(t2 >> 16) & 0xff] ^
            Td2[(t1 >>  8) & 0xff] ^
            Td3[(t0      ) & 0xff] ^
            rk[3];
    }
    /*
     * apply last round and
     * map cipher state to byte array block:
     */
    s0 =
        (Td4[(t0 >> 24)       ] & 0xff000000) ^
        (Td4[(t3 >> 16) & 0xff] & 0x00ff0000) ^
        (Td4[(t2 >>  8) & 0xff] & 0x0000ff00) ^
        (Td4[(t1      ) & 0xff] & 0x000000ff) ^
        rk[0];
    s1 =
        (Td4[(t1 >> 24)       ] & 0xff000000) ^
        (Td4[(t0 >> 16) & 0xff] & 0x00ff0000) ^
        (Td4[(t3 >>  8) & 0xff] & 0x0000ff00) ^
        (Td4[(t2      ) & 0xff] & 0x000000ff) ^
        rk[1];
    s2 =
        (Td4[(t2 >> 24)       ] & 0xff000000) ^
        (Td4[(t1 >> 16) & 0xff] & 0x00ff0000) ^
        (Td4[(t0 >>  8) & 0xff] & 0x0000ff00) ^
        (Td4[(t3      ) & 0xff] & 0x000000ff) ^
        rk[2];
    s3 =
        (Td4[(t3 >> 24)       ] & 0xff000000) ^
        (Td4[(t2 >> 16) & 0xff] & 0x00ff0000) ^
        (Td4[(t1 >>  8) & 0xff] & 0x0000ff00) ^
        (Td4[(t0      ) & 0xff] & 0x000000ff) ^
        rk[3];

    if (pCtx->Mode) {
        s0 = s0 ^ iv[0];    iv[0] = v0;
        s1 = s1 ^ iv[1];    iv[1] = v1;
        s2 = s2 ^ iv[2];    iv[2] = v2;
        s3 = s3 ^ iv[3];    iv[3] = v3;
    }

    PUTU32(pt     , s0);
    PUTU32(pt +  4, s1);
    PUTU32(pt +  8, s2);
    PUTU32(pt + 12, s3);
}

//////////////////////////////////////////////////////////////////////////////
// API functions                                                            //
//////////////////////////////////////////////////////////////////////////////

/*
* initialize AES context
*/
int AesCtxIni(AesCtx *pCtx, unsigned char *pIV, unsigned char *pKey, unsigned int KeyLen, unsigned char Mode)
{
    if (pKey == 0 || pCtx == 0 || (KeyLen != KEY128 && KeyLen != KEY192 && KeyLen != KEY256))
        return -1;

    // generate key schedule
    pCtx->Nr = AesGenKeySched(pCtx->Ek,  pCtx->Dk, pKey, KeyLen);

    // initialize IV
    if (pIV != 0) {
        pCtx->Iv[0] = GETU32(pIV     );
        pCtx->Iv[1] = GETU32(pIV + 4 );
        pCtx->Iv[2] = GETU32(pIV + 8 );
        pCtx->Iv[3] = GETU32(pIV + 12);
    }

    // mode
    pCtx->Mode = Mode;

    return 0;
}

/*
* Encrypt plain text
*/
int AesEncrypt(AesCtx *pCtx, unsigned char *pData, unsigned char *pCipher, unsigned int DataLen)
{
    int i;

    if (pData == 0 || pCipher == 0 || pCtx == 0 || (DataLen & 0xf) != 0)
        return -1;

    for (i = 0; i < DataLen; i += BLOCKSZ) {
        // encrypt block by block
        AesEncBlk(pCtx, pData, pCipher);
        pCipher += BLOCKSZ;
        pData += BLOCKSZ;
    }
    return DataLen;
}

/*
* Decrypt cipher
*/
int AesDecrypt(AesCtx *pCtx, unsigned char *pCipher, unsigned char *pData, unsigned int CipherLen)
{
    int i;

    if (pData == 0 || pCipher == 0 || pCtx == 0 || (CipherLen & 0xf) != 0)
        return -1;

    for (i = 0; i < CipherLen; i += BLOCKSZ) {
        // decrypt block by block
        AesDecBlk(pCtx, pCipher, pData);
        pCipher += BLOCKSZ;
        pData += BLOCKSZ;
    }
    return CipherLen;
}

