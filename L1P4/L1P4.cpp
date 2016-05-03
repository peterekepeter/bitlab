
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv)
{
  if (argc!=2)
  {
    fprintf(stderr, "program requires one integer argument N\n");
    exit(1);
  }

  int n = atoi(argv[1]);    
  auto start_time = clock();

  int inside = 0;

  for (int i=0; i<n; i++)
  {
    int x = rand();
    int y = rand();
    
    if (x*x+y*y<RAND_MAX*RAND_MAX)
      inside++;
  }
                            
  double pi = 4.0*inside/n;

  auto end_time = clock();      
  printf("execution time %ldms\n", end_time - start_time);

  printf("aproximation of pi is %.16f using %d sample points\n",pi,n);
  return 0;
}
