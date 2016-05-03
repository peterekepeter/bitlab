#pragma once

extern unsigned char* epsilon;
extern unsigned char* array;

int trcount_nist(int count);  //data is sent through epsilon       
int trcount_run1(const void* a, int count);
int trcount_run2(const void* a, int count);
int trcount_run3(const void* a, int count);
int trcount_run4(const void* a, int count);
void send_to_epsilon(const void* A, int count);
void clean_epsilon();


namespace nist2{

  void bits(unsigned char*array, int byte_size);
  unsigned int get_nth_block4(unsigned char* array, int offset);
  unsigned int get_nth_block_effect(unsigned char* array, int offset);
  unsigned int get_block_fast(unsigned char* array, int byte_offset);
  unsigned int get_2bytes(unsigned char* array, int byte_offset);
  unsigned int get_mask(int size);
  unsigned int popCountLUT16_64(unsigned __int64* addr, unsigned __int64* endAddr);
  unsigned int popCountLUT16_32(unsigned __int32* addr, unsigned __int32* endAddr);
  //BITHACKS
  int bitcount(unsigned int n);
  unsigned int popCountBITHACK_32(unsigned __int32* addr, unsigned __int32* endAddr);
  unsigned int runsLUT16_32(unsigned __int32* addr, unsigned __int32* endAddr);
  //for whole array
  void Histogram_m(int* freq, int m);
  void LSHIFT32(int *a, int shift, int Tsize);
  void LSHIFT32_p(int* a, int shift, int Tsize);
  void LSHIFT64(__int64 *a, int shift, int Tsize);

};

