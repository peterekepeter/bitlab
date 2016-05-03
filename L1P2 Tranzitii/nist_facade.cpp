
//import

namespace nist{      
  double Runs(int n);
}

namespace nist2{
  
  int Runs(int n);
  int Runs2(int n);
  int Runs3(int n);
  int Runs4(int n);
}


unsigned char* epsilon;
unsigned char* array;

void send_to_epsilon(const void* A, int count)
{
  epsilon = new unsigned char [count];
  
  auto a = (unsigned char*) A;
  unsigned char c;
  int bit = 0;
  unsigned char *bptr = epsilon;
  while (count--)
  {
    if (!bit)
    {
      c = *a++;
      bit = 8;      
    }

    *bptr++ = c&128 ? 1 : 0;
    c <<=1;
    bit--;
  }
}

void clean_epsilon()
{
  delete[] epsilon;
}

int trcount_nist(int count)
{
  return nist::Runs(count)-1; 
}                         

int trcount_run1(const void* a, int count)
{
  array = (unsigned char*) a;
  return nist2::Runs(count)-1;
} 

int trcount_run2(const void* a, int count)
{
  array = (unsigned char*) a;
  return nist2::Runs2(count)-1;
} 

int trcount_run3(const void* a, int count)
{
  array = (unsigned char*) a;  
  return nist2::Runs3(count)-1;
}

int trcount_run4(const void* a, int count)
{
  array = (unsigned char*) a;  
  return nist2::Runs4(count)-1;
}




