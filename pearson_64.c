/* Author: Dominic van der Zypen
 * Pearson hashing with 2 changes
 * 1) 64 bit or more, not just 8 bit.
 * 2) No table lookup, but (simple) bijection with bit rotation and 
 * bit swapping in the bytes constituting the fixed-length hash output.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>  
#include <inttypes.h>

//-----------------------------------------------------
void bit_rotate(uint64_t* i_p)
  // RIGHT-rotation
{
  uint64_t rightmost_bit = *(i_p) & 1;
  *i_p = (*i_p >> 1) | (rightmost_bit << 63);
} // tested --> OK
//-----------------------------------------------------
void hash(unsigned char* s, uint64_t* hash_p)
  // takes string s of arbitrary length, terminated by \0.
  // produces a Pearson-like hash: but no table / lookup needed
  // instead use BIJECTION : right-rotation of uint64_t
  // Output: 64bit hash
{

   *hash_p = 0;
  
  int count_to_8 = 0; // to address the 8 byte-parts of uint64_t

  // walk through string and hash consecutive groups of <len> bytes

  while (*s != '\0')
  {
     // Step 1 of Pearson's original algo: XOR what we have
     // with character just read: 
    *(hash_p) = *(hash_p) ^ ((*s) << (count_to_8 * 8));
    count_to_8++;
                             
    if (count_to_8 == 8)  // uint64_t is "full"! -> do Pearson bijection
    { 
      count_to_8 = 0;
      
         // Step 2 of Pearson: Bijection of the XOR'ed ex_ppression!
         // Instead of table lookup (original), do a rotation:
      bit_rotate(hash_p);
    } 

    s++; // walk 1 step further in string!
  } // end of loop
}
//-----------------------------------------------------
int main()
{
  unsigned char my_string[]
      = "hell0 world, this is some lengthy string having hopefully \
         more than 32 lettaz, actually much more so that we can get \
         some XOR fun! NEW BLAAAH \
         abcdefghijklmnopqrstuvwxyz (so this gives \
         some rounds . 0123456789010111213141516171819";
  unsigned char* p;
  uint64_t x = 0;
  hash(&my_string[0], &x);
  printf("%" PRIx64 "\n", x);
  return 0;
}
