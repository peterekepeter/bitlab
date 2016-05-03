#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "nist_facade.h"

#define BYTE_SEQUENCE_METHODS

#define printbitsTemplate(TYPE,BITS)                                 \
void printbits(TYPE a, int count = BITS)                             \
{                                                                    \
  count = count>BITS ? BITS : count;                                 \
  char buffer[BITS+1];                                               \
  unsigned char bc = 0;                                              \
  while (count--)                                                    \
  {                                                                  \
    buffer[bc++] = a&(1<<(BITS-1)) ? '1' : '0';                      \
    a<<=1;                                                           \
  }                                                                  \
  buffer[bc]=0;                                                      \
  printf("%s",buffer);                                               \
}

printbitsTemplate(unsigned char, 8);
printbitsTemplate(unsigned short, 16);
printbitsTemplate(unsigned, 32);           
printbitsTemplate(unsigned long long, 64);
printbitsTemplate(char, 8);
printbitsTemplate(short, 16);
printbitsTemplate(int, 32);           
printbitsTemplate(long long, 64);

void printbits(const void* A, int count);

inline unsigned endianShuffle(unsigned a) 
{ return (a<<24)|((a<<8)&0x00ff0000)|((a>>8)&0x0000ff00)|(a>>24); }
inline int endianShuffle(int a) { return endianShuffle((unsigned) a); }
inline unsigned short endianShuffle(unsigned short a){ return(a<<8)|(a>>8); }
inline short endianShuffle(short a){return endianShuffle((unsigned short)a);}
inline unsigned char endianShuffle(unsigned char a) { return a; }
inline char endianShuffle(char a) { return a; }

int trcount_a(const void* A, int count)
{
  int acc = 0;
  auto a = (unsigned char*) A;
  unsigned char c;
  int bit = 0;
  char *bptr;
  unsigned char current = 0;
  unsigned char last = 0;
  
  c = *a++;
  bit = 8; 
  last = current = c&128;

  while (count--)
  {
    if (!bit)
    {
      c = *a++;
      bit = 8;      
    }

    current = c&128;
    if (current != last)
      acc++;
    last = current;
    c <<=1;
    bit--;
  }
  return acc;
}

unsigned char trtabl[65536*2];         
unsigned char trtabl2[65536*2];

int trcount_b(const void* A, int count)
{
  auto ptr = (unsigned short*) A;
  int acumulator = 0;

  int carry = *(char*)ptr; //sign extend
  carry &=0x00010000;

  while (count >= 16)
  {
    unsigned short value = *ptr++; 
    unsigned index = (unsigned)carry | endianShuffle(value);
    acumulator += trtabl[index];
    count-=16;
    carry = (index<<16)&0x00010000;
  }

  if (count)
  {
    unsigned short value = *ptr++;
    value = endianShuffle(value);
    carry = carry ? 0xffffffff : 0x00000000;
    carry <<= count;
    value >>= (16-count);
    unsigned index = (value | carry)&0x1ffff;
    acumulator += trtabl[index];
  }

  return acumulator;
}

int trcount_c(const void* A, int count)
{
  auto ptr = (unsigned short*) A;
  int acumulator = 0;

  int carry = *(char*)ptr; //sign extend
  carry &=0x00010000;

  while (count >= 128)
  {
    unsigned short value;
    unsigned index;
    unsigned xval;

    xval = ((unsigned*)ptr)[0];
    value = (unsigned short)xval; 
    index = (unsigned)carry | value;
    acumulator += trtabl2[index];
    carry = (index<<8)&0x00010000;
    value = (unsigned short)(xval>>16);
    index = (unsigned)carry | value;
    acumulator += trtabl2[index];
    carry = (index<<8)&0x00010000;
                                      
    xval = ((unsigned*)ptr)[1];
    value = (unsigned short)xval; 
    index = (unsigned)carry | value;
    acumulator += trtabl2[index];
    carry = (index<<8)&0x00010000;
    value = (unsigned short)(xval>>16);
    index = (unsigned)carry | value;
    acumulator += trtabl2[index];
    carry = (index<<8)&0x00010000;

    xval = ((unsigned*)ptr)[2];
    value = (unsigned short)xval; 
    index = (unsigned)carry | value;
    acumulator += trtabl2[index];
    carry = (index<<8)&0x00010000;
    value = (unsigned short)(xval>>16);
    index = (unsigned)carry | value;
    acumulator += trtabl2[index];
    carry = (index<<8)&0x00010000;
                                      
    xval = ((unsigned*)ptr)[3];
    value = (unsigned short)xval; 
    index = (unsigned)carry | value;
    acumulator += trtabl2[index];
    carry = (index<<8)&0x00010000;
    value = (unsigned short)(xval>>16);
    index = (unsigned)carry | value;
    acumulator += trtabl2[index];
    carry = (index<<8)&0x00010000;
             
    ptr += 8;
    count-=128;
  }

  while (count >= 16)
  {
    unsigned short value = *ptr++; 
    unsigned index = (unsigned)carry | value;
    acumulator += trtabl2[index];
    count-=16;
    carry = (index<<8)&0x00010000;
  }

  if (count)
  {
    unsigned short value = *ptr++;
    value = endianShuffle(value);
    carry = carry ? 0xffffffff : 0x00000000;
    carry <<= count;
    value >>= (16-count);
    unsigned index = (value | carry)&0x1ffff;
    acumulator += trtabl2[(index&0x10000) | ((index>>8)&0x00ff) | ((index<<8)&0xff00)];
  }

  return acumulator;
}

void generate_trtabl()
{
  unsigned char strtabl[] = { 0, 1, 1, 0 };
  for (int i=0; i<65536*2; i++)
  {
    int acc = 0;
    int x = i;
    for (int j=0; j<16; j++)
    {
      acc += strtabl[x&3];
      x = x>>1;
    }
    trtabl[i] = acc;
    int i2 = (i&0x10000) | ((i>>8)&0x00ff) | ((i<<8)&0xff00);
    trtabl2[i2] = acc;
  }
}

void printbits(const void* A, int count)
{
  auto a = (unsigned char*) A;
  unsigned char c;
  int bit = 0;
  char buffer[9] = "00000000";
  char *bptr;
  bool first = true;
  while (count--)
  {
    if (!bit)
    {
      c = *a++;
      bit = 8;      
      bptr = buffer;
      if (!first)
        printf("%s",buffer);
      else
        first = false;
    }

    *bptr++ = c&128 ? '1' : '0';
    c <<=1;
    bit--;
  }

  if (bit!=8)
  {
    buffer[8-bit]=0;
    printf("%s",buffer);
  }
}


long long stat_time;
const char* stat_name;
int test_count = 0;
void stat_begin(const char* test_name)
{
  stat_time = clock();
  stat_name = test_name;
  printf("\n--- test %d ---\n",test_count++);
}

void stat_end()
{
  long long end_time = clock();
  printf("%s %ldms\n",stat_name, end_time - stat_time);
}


int main(int argc, char** argv)
{
  generate_trtabl();
  int n = atoi(argv[1]);
  //int n = 64;
  unsigned char* data_A = new unsigned char[n];
  
  for (int i=0; i<n; i++)
  {
    data_A[i] = ~rand()&255;
  }
  
  int count;
  int offset = 0;
  int length = n;

  void *input = data_A+offset;

  stat_begin("trcount a");
  count = trcount_a(input,length);
  stat_end();                               
  printf("transition count is %d\n",count);

  stat_begin("trcount b");
  count = trcount_b(input,length);
  stat_end();                               
  printf("transition count is %d\n",count);
  
  stat_begin("trcount c");
  count = trcount_c(input,length);
  stat_end();                               
  printf("transition count is %d\n",count);
                  
#ifdef BYTE_SEQUENCE_METHODS
  stat_begin("exploding bitsequence to byte sequence");
  send_to_epsilon(data_A+offset, length);
  stat_end();
  stat_begin("trcount nist original");
  count = trcount_nist(length);
  stat_end();                                
  printf("transition count is %d\n",count); 

  stat_begin("trcount nist2 run original");
  count = trcount_run1(input,length);
  stat_end();                               
  printf("transition count is %d\n",count);   
  clean_epsilon();          
#endif
                    
  stat_begin("trcount nist2 run2");
  count = trcount_run2(input,length);
  stat_end();                               
  printf("transition count is %d\n",count);

  stat_begin("trcount nist2 run3");
  count = trcount_run3(input,length);
  stat_end();                               
  printf("transition count is %d\n",count);
  
  stat_begin("trcount nist2 run4");
  count = trcount_run4(input,length);
  stat_end();                               
  printf("transition count is %d\n",count);


  printbits(data_A+offset,16);   
  printf("\n");

  delete[] data_A;

  return 0;
}
