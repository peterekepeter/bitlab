
#include <time.h>
#include <stdio.h>
#include <stdlib.h>


/// implementare simpla
void xorv_a(const void* A, const void* B, void *C, size_t size)
{                               
  auto a = (unsigned char*) A;
  auto b = (unsigned char*) B;
  auto c = (unsigned char*) C;

  for (auto i = 0; i<size; i++)
  {
    c[i] = a[i] ^ b[i];
  }
}

/// implementara alternativa
void xorv_b(const void* A, const void* B, void *C, size_t size)
{
  auto a = (unsigned char*) A;
  auto b = (unsigned char*) B;
  auto c = (unsigned char*) C;

  while (size--)
    *c++ = *a++ ^ *b++;
}

volatile void shrv1_a(const void* A, void* B, size_t size)
{     
  auto a = (unsigned char*) A;     
  auto b = (unsigned char*) B;
  unsigned char carry = 0;
  for (auto i=0; i<size; i++)
  {
    unsigned char x = a[i];
    b[i] = (x >> 1) | carry;
    carry = x << 7;
  }
}
            
void shrv1_b(const void* A, void* B, size_t size)
{     
  auto a = (unsigned char*) A;     
  auto b = (unsigned char*) B;
  auto s = size;
  _asm{
    push esi
    push edi

    mov esi,a
    mov edi,b
    mov ecx,s
    clc

    _for:
    mov al, [esi]
    rcr al,1
    mov [edi],al
    inc esi
    inc edi
    dec ecx
    jnz _for

    pop edi
    pop esi
  }
}


void shrv1_c(const void* A, void* B, size_t size)
{     
  auto a = (unsigned char*) A;     
  auto b = (unsigned char*) B;
  auto s = size;
  _asm{
    push esi
    push edi

    mov esi,a
    mov edi,b
    mov ecx,s
    xor edx,edx
    clc

    _for:
    mov al, [esi+edx]
    rcr al,1
    mov [edi+edx],al
    inc edx
    dec ecx
    jnz _for

    pop edi
    pop esi
  }
}


/*
/// implementarea in MASM       
void xorv_C(const void* A, const void* B, void *C, size_t size)
{
  auto a = (unsigned char*) A;
  auto b = (unsigned char*) B;
  auto c = (unsigned char*) C;

}
  */

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
void stat_begin(const char* test_name)
{
  stat_time = clock();
  stat_name = test_name;
}

void stat_end()
{
  long long end_time = clock();
  printf("%s %ldms\n",stat_name, end_time - stat_time);
}


int main(int argc, char** argv)
{
  int n = atoi(argv[1]);
  unsigned char* data_A = new unsigned char[n];
  unsigned char* data_B = new unsigned char[n];
  unsigned char* data_C = new unsigned char[n];  
  unsigned char* data_D = new unsigned char[n];
  
  for (int i=0; i<n; i++)
  {
    data_A[i] = rand()&255;
  }
                                    
  stat_begin("shiftv A");
  shrv1_a(data_A, data_B, n);  
  stat_end();

  stat_begin("shiftv B");
  shrv1_b(data_A, data_C, n);  
  stat_end();
  
  stat_begin("shiftv C");
  shrv1_c(data_A, data_D, n);  
  stat_end();

  printbits(data_A,40);   
  printf("\n");

  printbits(data_B,40);   
  printf("\n");             
  printbits(data_C,40);   
  printf("\n");             
  printbits(data_D,40);   
  printf("\n");

  delete[] data_A;
  delete[] data_B;
  delete[] data_C;

  return 0;
}

int main_(int argc, char** argv)
{
  //int n = atoi(argv[1]);
  int n = 100000000;

  unsigned char* data_A = new unsigned char[n];
  unsigned char* data_B = new unsigned char[n];
  unsigned char* data_C = new unsigned char[n];

  
  for (int i=0; i<n; i++)
  {
    data_A[i] = rand()&255;    
    data_B[i] = rand()&255;
    data_C[i] = 0;
  }

  {
    auto start = clock();
    xorv_a(data_A, data_B, data_C, n);
    auto end = clock();
    printf("method A %ldms\n", end - start);
  }                                              

  {
    auto start = clock();
    xorv_b(data_A, data_B, data_C, n);
    auto end = clock();
    printf("method B %ldms\n", end - start);
  }

  delete[] data_A;
  delete[] data_B;
  delete[] data_C;

  return 0;
}
