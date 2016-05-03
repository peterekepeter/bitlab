
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <vector>


bool isPrime(int x)
{
  if (x<2) return false;
  if (x==2) return true;
  if ((x&1)==0) return false;
  auto limit = int(sqrt(float(x)));
  for (int i=3; i<=limit; i+=2)
    if (x%i == 0) return false;
  return true;
}

// A = 3, B = 1000000

int main(int argc, char** argv)
{
  if (argc!=3)
  {
    fprintf(stderr, "program requires two integer arguments A, B; A < B\n");
    exit(1);
  }
  int a = atoi(argv[1]);
  int b = atoi(argv[2]);
  if (!(a<b))
  {
    fprintf(stderr, "A < B constraint violation");
    exit(2);
  }

  auto start_time = clock();

  std::vector<int> primes;

  for (int i=a; i<b; i++)
    if (isPrime(i))
      primes.push_back(i);

  auto end_time = clock();
  printf("execution time %ldms\n", end_time - start_time); 
  
  printf("first 10 numbers from the vector:\n");

  int printCount = 10;
  for (auto i = primes.begin(); i!=primes.end(); i++)
  {
    printf("%d ",*i);
    if(!printCount--)break;;
  }                                             
  printf("\n");

  printf("last 10 numbers from the vector:\n");

  printCount = 10;
  for (auto i = primes.rbegin(); i!=primes.rend(); i++)
  {
                              
    printf("%d ",*i);
    if(!printCount--)break;;
  }
  printf("\n");

  return 0;
}
