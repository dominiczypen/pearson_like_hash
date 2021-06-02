/* Author: Dominic van der Zypen
 * Last modified: 2021-06-02
 * Pearson hashing with 2 changes
 * 1) possible for 256 bit or more, not just 8 bit (length is multiple of 8)
 * 2) No table lookup, but (simple) bijection with bit rotation and 
 * bit swapping in the bytes constituting the fixed-length hash output.
 */

#include <stdio.h>
#include <stdlib.h>

//-----------------------------------------------------
void bit_rotate(unsigned char* x_p, int len)
  // RIGHT-rotates a bit string made up of <len> unsigned chars (each 8 bit).
  // A typical value for <len> will be 32 for using bit_rotate()
  // in 256-bit hash
{
  int i = 0;
  char old_right_bit; // gives least significant bit of byte (unsigned char)
  char new_left_bit;

  new_left_bit = *(x_p + len - 1) & 1; // gets the RIGHTMOST bit of whole array!

  while (i < len)
  {
    old_right_bit = *x_p & 1;
       // save old right bit before it gets flushed by right shift!
    *x_p = ((*x_p) >> 1);
    *x_p = (*x_p) | (new_left_bit << 7);// insert new left bit at left end of uint
    new_left_bit = old_right_bit; // uint32-uebergreifender shift

    x_p++;  // move pointer forward by one char position (8 bits)
    i++;
  }
  return;
} // tested --> OK
//-----------------------------------------------------
void swap_4_4(unsigned char* x_p, int len)
  // swap left 4 bits with right 4 bits in every byte of x_p
  // i.e. 0000 1111 \mapsto 1111 0000
{
  int i = 0;
  while (i < len)
  {
	  // swapping step
    *(x_p + i) = (*(x_p + i) & 0xf0) | (*(x_p + i ) & 0x0f);
    i++;
  }
}
//-----------------------------------------------------
void print_binary(unsigned char* x_p, int len)
  // print a bit string made up of <len) unsigned ints
{
  int i = 0; // loops over byte array
  int j = 0; // loops inside unsigned int
  unsigned char printed_uint; // safety copy OTHERWISE make x_p flat 0!!
  while (i < len)
  {
    printed_uint = *x_p;
    while (j < 8)
    {
      printf("%d",(printed_uint & (1<<7)) >> 7);
      printed_uint = printed_uint << 1;
      j++;
    }
    x_p++;
    j = 0;
    i++;
    if (i % 8 == 0) {printf("\n");} // formatting
  }
  printf("\n");
} // tested --> OK
//-----------------------------------------------------
void print_0x(unsigned char* x_p, int len)
  // print hex made up of <len> unsigned ints
{
  int i = 0;
  while (i < len)
  {
    printf("%02x", *x_p);
    x_p++;
    i++;
  }
  printf("\n");
} // tested --> OK
//-----------------------------------------------------
void hash(unsigned char* s, unsigned char* hash_p, int len)
  // takes string s of arbitrary length, terminated by \0.
  // produces a Pearson-like hash: but no table / lookup needed
  // instead use BIJECTION : rotation of 32 uchar = 256 bit together 
  // with swap_4_4 of all bytes (1111 0000 goes to 0000 1111)
{

  // set everything in *(hash_p + i) to 0.
  int i = 0;
  while (i < len)
  {
    *(hash_p + i) = 0;
    i++;
  }

  // walk through string and hash consecutive groups of <len> bytes

  int count_to_len= 0;

  while (*s != '\0')
  {
     // Step 1 of Pearson's original algo: XOR what we have
     // with character just read: 
    *(hash_p + count_to_len) = *(hash_p + count_to_len) ^ (*s);
    count_to_len++;

     // Step 2 of Pearson: Bijection of the XOR'ed ex_ppression!
     // Instead of table lookup (original), do a rotation:
    bit_rotate(hash_p, 32);
    swap_4_4(hash_p, 32); // some additional shuffling 

    s++; // walk 1 step further in string!
    if (count_to_len == len)
    {
      count_to_len = 0;
    }
  } // end of loop
}
//-----------------------------------------------------
int main()
{
  unsigned char my_string[]
      = "hell0 world, this is some lengthy string having hopefully \
         more than 32 lettaz, actually much more so that we can get \
         some xor fun! abcdefghijklmnopqrstuvwxyz (so this gives \
         some rounds . 0123456789010111213141516171819";
  unsigned char* p;
  unsigned char* x_p;
  // MALLOC()!!!
  x_p = (unsigned char*) malloc(32 * sizeof(unsigned char*));
  hash(&my_string[0], x_p, 32);
  //print_binary(x_p, 32);
  print_0x(x_p,32);
  return 0;
}
