#include <stdio.h>
#include "externs.h"

namespace nist2{

void bits(unsigned char*array, int byte_size)
{
	  int i,j;
	  unsigned char val;
	  for(i = 0; i < byte_size; i++)
	  {
		  val = array[i];
		  for(j = 0; j < 8; j++)
		  {
			  printf("%d",val&1);
			  val >>= 1;
		  }
		  printf(" ");
	  }
	  printf("\n");
  }

  unsigned int get_nth_block4(unsigned char* array, int offset)
  {
	  return (*(unsigned int*)(array+(offset>>3))) >> (offset & 7);//(array2[(offset >> 3)&3][(offset >> 3)] >> (offset & 7));
  }
  unsigned int get_nth_block_effect(unsigned char* array, int offset)
  {	
	  int shift = (offset & 7);
	  int byte = (offset >> 3);
	  if (shift == 0) return (*(unsigned int*)(array + byte) >> shift);
	  else return (*(unsigned int*)(array + byte) >> shift)^(*(unsigned int*)(array + byte+4) << (32 - shift));
  }

  /*
  void test_blocks(unsigned char* array, int size){
	  int i,sum = 0,value = 0,counter = 7;
	  unsigned char *copy_array = array - 1;
	

	  timings();
	  for(i = 0; i < size; i++)
	  {
		
		  sum  += get_nth_block4(array,i);
		
	  }
	  timings();
	  printf("%d",sum);
  }
  */

  int Mirrored_int(unsigned int val, int m){
	  int res = 0,i;
	  for(i=0; i < m; i++)
	  {
		  if(val & (1 << i)) res += (1 << (m - 1 - i));
	  }
	  return res;
  }
  /*
  int test(unsigned char*array, int n)
  {
	  int sum, i;

	  for(i = 0; i < n; i++)
	  {
		  sum += get_nth_block4(array,i);
	  }
	  return sum;
  }
  */

  unsigned int get_block_fast(unsigned char* array, int byte_offset)
  {
	  unsigned int res = (*((unsigned int*)(array + byte_offset)));

	  return res;
  }

  unsigned int get_2bytes(unsigned char* array, int byte_offset)
  {

	  return array[byte_offset] ^ (array[byte_offset + 1] << 8);
  }

  unsigned int get_mask(int size){
	  return (1 << size) - 1;
  }



  unsigned int popCountLUT16_64(unsigned __int64* addr, unsigned __int64* endAddr)
  {
	  unsigned int NumberOne1=0;
	  unsigned __int64 val1;

	  for(;addr<endAddr;)
	  {
		  val1=*addr;
		  addr++;
		  NumberOne1+=LUT_HW_16[val1>>48] + LUT_HW_16[(val1>>32)&0xFFFF] + LUT_HW_16[(val1>>16)&0xFFFF] + LUT_HW_16[val1&0xFFFF];	
	  }
	  return NumberOne1;
  }

  unsigned int popCountLUT16_32(unsigned __int32* addr, unsigned __int32* endAddr)
  {
	  unsigned int NumberOne1=0;
	  unsigned __int32 val1;

	  for(;addr<endAddr;)
	  {
		  val1=*addr;
		  addr++;
		  NumberOne1+=LUT_HW_16[val1>>16] + LUT_HW_16[val1&0xFFFF];	
	  }
	  return NumberOne1;
  }

  //BITHACKS
  int bitcount(unsigned int n) {
     /* works for 32-bit numbers only    */
     /* fix last line for 64-bit numbers */

     register unsigned int tmp;

     tmp = n - ((n >> 1) &  033333333333)
             - ((n >> 2) & 011111111111);
     return ((tmp + (tmp >> 3)) & 030707070707) % 63;
  }

  unsigned int popCountBITHACK_32(unsigned __int32* addr, unsigned __int32* endAddr){
	  unsigned int NumberOne1=0;
	  unsigned __int32 val1;

	  for(;addr<endAddr; addr++)
	  {
		  NumberOne1 += bitcount(*addr);
	  }
	  return NumberOne1;
  }

  unsigned int runsLUT16_32(unsigned __int32* addr, unsigned __int32* endAddr)
  {
	  unsigned int NumberOne1=0;
	  unsigned int val1,val2;
	
	  for(;addr<endAddr;)
	  {
		  val1=*addr;
		  addr++;

		  val2 = val1 ^ ((val1>>1)) ^ (*addr&1)*(1 << 31);
	
		  NumberOne1+=LUT_HW_16[val2>>16]+LUT_HW_16[val2&0xFFFF];	
	  }
	  return NumberOne1;
  }


  //for whole array
  void Histogram_m(int* freq, int m)
  {

  }


  void LSHIFT32(int *a, int shift, int Tsize)
  {
	  int i;
	  int com = sizeof(int)* 8 - shift;
	
	  for(i = 0; i < Tsize; i++)
	  {
		  a[i] = (a[i] >> shift) ^ (a[i+1] << com);
	  }
  }

  /*void LSHIFT32_p(int* a, int shift, int Tsize)
  {
	  unsigned int* p[4];
	  int i;
	  p[0] = ((unsigned char*)a + 0);
	  p[1] = ((unsigned char*)a + 1);
	  p[2] = ((unsigned char*)a + 2);
	  p[3] = ((unsigned char*)a + 3);
	
	  for( i = 0; i < Tsize; i++)
	  {
		
	  }
  } */

  void LSHIFT64(__int64 *a, int shift, int Tsize)
  {
	  int i;
	  int com = sizeof(__int64)* 8 - shift;
	
	  for(i = 0; i < Tsize; i++)
	  {
		
		  a[i] = (a[i] >> shift) ^ (a[i+1] << com);
	  }
  }

};
