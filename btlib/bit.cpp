
#include <cstdio>

namespace btlib
{        
  class Bit
  {
  private:
    void* addr;
    int offset;
  public:
    Bit(void* address, unsigned char offset):addr(address), offset(offset) {};
    inline void* GetMemAddress() { return addr; }
    inline unsigned char GetBitPosition() { return offset; }
    inline bool GetValue() { 
      return ((*(unsigned char*)addr)>>offset)&1; 
    } 
    inline void SetValue(bool bitValue) { 
      auto p = (unsigned char*)addr;
      *p = bitValue? *p|(1<<offset) : *p&~(1<<offset);
    }
    inline bool operator = (const bool value) { SetValue(value); return value; };  
    inline operator bool() { return GetValue(); };
  };

  class BitVector
  {
  private:
    void* addr;
    long long length;
    unsigned bit_offset;
  public:
    BitVector(void* address, long long length, unsigned bit_offset = 0) : addr(address), length(length), bit_offset(bit_offset) {}
    inline Bit operator [](const int i)
    {
      auto x = i + bit_offset;
      return Bit(((unsigned char*)addr)+(x>>3),x&7);
    }
    inline int Size(){ return length; }
  };
}

using namespace btlib;
using namespace std;

void main()
{
  char x = 0;
  BitVector b(&x,4,2);
  for (int i=0; i<b.Size(); i++)
  {
    b[i] = 1;
  }

  printf("%2.2x ",x);

  b[0] = b[1] = b[2] = b[3] = 0;     

  printf("%2.2x ",x);
}


