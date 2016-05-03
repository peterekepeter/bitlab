
#include <vector>
#include <stdio.h>
#include <time.h>

inline unsigned char bitcount(int x)
{
  unsigned char acc = 0;
  while(x)
    acc+=x&1, x>>=1;
  return acc;
}

unsigned char btc_table[65536];

inline unsigned char bitc(const unsigned char x) { return btc_table[x]; }
inline unsigned char bitc(const char x) { return btc_table[unsigned char(x)]; }
inline unsigned char bitc(const unsigned short x) { return btc_table[x]; }
inline unsigned char bitc(const short x) { return btc_table[unsigned short(x)]; }
inline unsigned char bitc(const unsigned x) { return btc_table[x>>16] + btc_table[x&65535]; }
inline unsigned char bitc(const int x) { return bitc(unsigned(x)); }
inline unsigned char bitc(const unsigned long long x) { return btc_table[x&65535]+btc_table[(x>>16)&65535]+btc_table[(x>>32)&65535]+btc_table[x>>48]; }
inline unsigned char bitc(const long long x) { return bitc((unsigned long long) x); }

int bitcount_A(const void* data, size_t size)
{
  int acc = 0;          
  auto byteptr = (unsigned char*) data;
  for (int i=0; i<size; i++)
  {
    acc += bitcount(byteptr[i]);
  }
  return acc;
}

int bitcount_B(const void* data, size_t size)
{
  auto ptr = (unsigned char*) data;
  int acc = 0;
  while (size--) acc += bitc(*ptr++);
  return acc;
}

int bitcount_C(const void* data, size_t size)
{                                        
  int acc = 0;                         
  auto byteptr = (unsigned char*) data;
  auto ptr = (unsigned short*) data;
  auto count = size >> 1;
  while (count--) acc += bitc(*ptr++);
  if (size&1)
    acc += bitc(byteptr[size-1]);
  return acc;
}            

int bitcount_D(const void* data, size_t size)
{                                        
  int acc = 0;
  auto byteptr = (unsigned char*) data;
  auto ptr = (unsigned*) data;
  auto count = size >> 2;
  while (count--) acc += bitc(*ptr++);
  switch (size&3)
  {                                   
  case 3: acc += bitc(byteptr[size-3]);       
  case 2: acc += bitc(byteptr[size-2]);        
  case 1: acc += bitc(byteptr[size-1]);
  }
  return acc;
}

int bitcount_E(const void* data, size_t size)
{                                        
  int acc = 0;
  auto byteptr = (unsigned char*) data;
  auto ptr = (unsigned long long*) data;
  auto count = size >> 3;
  while (count--) acc += bitc(*ptr++);
  switch (size&7)
  {                                       
  case 7: acc += bitc(byteptr[size-7]);   
  case 6: acc += bitc(byteptr[size-6]);   
  case 5: acc += bitc(byteptr[size-5]);   
  case 4: acc += bitc(byteptr[size-4]);   
  case 3: acc += bitc(byteptr[size-3]);       
  case 2: acc += bitc(byteptr[size-2]);        
  case 1: acc += bitc(byteptr[size-1]);
  }
  return acc;
}         


int bitcount_E2(const void* data, size_t size)
{                                        
  int acc = 0,i=0;
  auto byteptr = (unsigned char*) data;
  auto ptr = (unsigned long long*) data;
  auto count = size >> 3;  
  while (count--) {
    acc += bitc(ptr[i++]);
  }
  switch (size&7)
  {                                       
  case 7: acc += bitc(byteptr[size-7]);   
  case 6: acc += bitc(byteptr[size-6]);   
  case 5: acc += bitc(byteptr[size-5]);   
  case 4: acc += bitc(byteptr[size-4]);   
  case 3: acc += bitc(byteptr[size-3]);       
  case 2: acc += bitc(byteptr[size-2]);        
  case 1: acc += bitc(byteptr[size-1]);
  }
  return acc;
}       

int bitcount_F(const void* data, size_t size)
{                                        
  int acc = 0;
  auto byteptr = (unsigned char*) data;
  auto ptr = (unsigned*) data;
  auto count = size >> 2;
  while (count--) 
  {
    auto val = *ptr++;
    unsigned char a = bitc(val&255);
    unsigned char b = bitc((val>>8)&255);
    unsigned char c = bitc((val>>16)&255);
    unsigned char d = bitc((val>>24)); 
    acc += a + b + c + d;
  }
  switch (size&3)
  {                                   
  case 3: acc += bitc(byteptr[size-3]);       
  case 2: acc += bitc(byteptr[size-2]);        
  case 1: acc += bitc(byteptr[size-1]);
  }
  return acc;
}

int bitcount_G(const void* data, size_t size)
{
  const void *d = data;
  int s = size;
  int r;
  void *lut = btc_table;
  _asm{         
    push esi
    push ebx
    mov esi,d
    mov ecx,s
    mov ebx,lut
    xor edx,edx
    xor eax,eax

    _for:
    lodsb
    xlatb
    add edx,eax
    loop _for

    pop ebx
    pop esi
    
    mov r,edx
  }
  return r;
}

int bitcount_G2(const void* data, size_t size)
{
  const void *d = data;
  int s = size;
  int r;
  void *lut = btc_table;
  _asm{         
    push esi
    push ebx
    
    mov ecx, d
    mov eax, s        
    mov ebx, lut
    xor esi,esi
    
    _for:
    movzx       edx,byte ptr [ecx]      ; lodsb
    movzx       edx,byte ptr [ebx+edx]   ; xlat
    add         esi,edx  
    inc         ecx  
    dec         eax  
    jne         _for
   
    mov r,esi
    pop ebx
    pop esi
  }
  return r;
}

void genTable()
{
  for (int i=0; i<65536; i++)
    btc_table[i]=bitcount(i);
}

double entropy(int o, int t)
{
  auto z = t-o;
  auto ot = (o/(double)t);
  auto zt = (z/(double)t);
  return -ot*log(ot)-zt*log(zt);
}

int main(int argc, char** argv)
{
  genTable();
  
  auto machine_speed_ghz = 1.3;
  auto n = 1000000000/8 - 1;
  //auto n = 100000000/8 - 1;
  unsigned char* data = new unsigned char[n];

  for (int i=0; i<n; i++)
    data[i] = rand()&255;
    
  {
    auto time_start = clock();
    auto result = bitcount_G(data, n);    
    auto runtime = clock() - time_start;                                                                     
    printf("\nmethod G\n result %d\n runtime %ldms\n %f bits/clock\n entropy %f\n",   
      result, runtime, n*8000.0/runtime/1e9/machine_speed_ghz, entropy(result,n*8)); 
  }
       
  
  {
    auto time_start = clock();
    auto result = bitcount_G2(data, n);    
    auto runtime = clock() - time_start;                                                                     
    printf("\nmethod G2\n result %d\n runtime %ldms\n %f bits/clock\n entropy %f\n",   
      result, runtime, n*8000.0/runtime/1e9/machine_speed_ghz, entropy(result,n*8)); 
  }

  {
    auto time_start = clock();
    auto result = bitcount_A(data, n);    
    auto runtime = clock() - time_start;  
    printf("\nmethod A\n result %d\n runtime %ldms\n %f bits/clock\n entropy %f\n", 
      result, runtime, n*1000.0/runtime/1e9/machine_speed_ghz, entropy(result,n*8)); 
  } 

  {
    auto time_start = clock();
    auto result = bitcount_B(data, n);    
    auto runtime = clock() - time_start;    
    printf("\nmethod B\n result %d\n runtime %ldms\n %f bits/clock\n entropy %f\n",    
      result, runtime, n*8000.0/runtime/1e9/machine_speed_ghz, entropy(result,n*8)); 
  }

  {
    auto time_start = clock();
    auto result = bitcount_C(data, n);
    auto runtime = clock() - time_start;         
    printf("\nmethod C\n result %d\n runtime %ldms\n %f bits/clock\n entropy %f\n",
      result, runtime, n*8000.0/runtime/1e9/machine_speed_ghz, entropy(result,n*8)); 
  }

  {
    auto time_start = clock();
    auto result = bitcount_D(data, n);    
    auto runtime = clock() - time_start;                                                                     
    printf("\nmethod D\n result %d\n runtime %ldms\n %f bits/clock\n entropy %f\n",   
      result, runtime, n*8000.0/runtime/1e9/machine_speed_ghz, entropy(result,n*8)); 
  }                       

  {
    auto time_start = clock();
    auto result = bitcount_E(data, n);    
    auto runtime = clock() - time_start;                                                                     
    printf("\nmethod E\n result %d\n runtime %ldms\n %f bits/clock\n entropy %f\n",   
      result, runtime, n*8000.0/runtime/1e9/machine_speed_ghz, entropy(result,n*8)); 
  }                       

  {
    auto time_start = clock();
    auto result = bitcount_E2(data, n);    
    auto runtime = clock() - time_start;                                                                     
    printf("\nmethod E2\n result %d\n runtime %ldms\n %f bits/clock\n entropy %f\n",   
      result, runtime, n*8000.0/runtime/1e9/machine_speed_ghz, entropy(result,n*8)); 
  }                  

  {
    auto time_start = clock();
    auto result = bitcount_F(data, n);    
    auto runtime = clock() - time_start;                                                                     
    printf("\nmethod F\n result %d\n runtime %ldms\n %f bits/clock\n entropy %f\n",   
      result, runtime, n*8000.0/runtime/1e9/machine_speed_ghz, entropy(result,n*8)); 
  }

  delete[] data;
  return 0;
};
