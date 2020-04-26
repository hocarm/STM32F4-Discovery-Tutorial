#ifndef	__DES_3DES_H
#define __DES_3DES_H

/* Define Macro */

#define uchar unsigned char
#define uint unsigned int
#define ENCRYPT 1
#define DECRYPT 0
// Dinh nghia cac dia chi cong uart 
#define UARTLITE_DEVICE_ID          XPAR_RS232_DEVICE_ID
// kich thuoc du lieu nhan duoc tu uart
#define TEST_BUFFER_SIZE 1
// Obtain bit "b" from the left and shift it "c" places from the right
#define BITNUM(a,b,c) (((a[(b)/8] >> (7 - (b%8))) & 0x01) << (c))
#define BITNUMINTR(a,b,c) ((((a) >> (31 - (b))) & 0x00000001) << (c))
#define BITNUMINTL(a,b,c) ((((a) << (b)) & 0x80000000) >> (c))
// This macro converts a 6 bit block with the S-Box row defined as the first and last
// bits to a 6 bit block with the row defined by the first two bits.
#define SBOXBIT(a) (((a) & 0x20) | (((a) & 0x1f) >> 1) | (((a) & 0x01) << 4))



/*--------------- Declaring DES Function Prototype -----------------*/

// chu trinh tao khoa key_schedule
void key_schedule(uchar key[], uchar schedule[][6], uint mode);

// Initial (Inv)Permutation step
void IP(uint state[], uchar in[]);

void InvIP(uint state[], uchar in[]);

// f(R_state,Key) la ham ma hoa
uint f(uint state, uchar key[]);

void des_crypt(uchar in[], uchar out[], uchar key[][6]);

/*------------------ Declaring 3DES Function Prototype ---------------------*/
void three_des_key_schedule(uchar key[], uchar schedule[][16][6], uint mode);
void three_des_crypt(uchar in[], uchar out[], uchar key[][16][6]);

#endif

