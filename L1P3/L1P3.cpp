

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char** argv)
{            
  if (argc!=2)
  {
    fprintf(stderr, "program requires one integer argument N\n");
    exit(1);
  }

  int n = atoi(argv[1]);        
  auto start_time = clock();

  double sum = .0;

  for (int i=0; i<n; i++)
  {
    double x = (i+.5)/n;
    sum += 4/(1+x*x);
  }

  double pi = sum / n;        
  auto end_time = clock();          

  printf("execution time %ldms\n", end_time - start_time);

  printf("aproximation of pi is %.16f using %d sample points\n",pi,n);

  return 0;
}
