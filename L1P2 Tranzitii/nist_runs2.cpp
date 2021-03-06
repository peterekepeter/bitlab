 #include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "externs.h"
//#include "../include/cephes.h"
//#include "../include/erf.h"
//#include "../include/tools.h"
#include "nist_facade.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                              R U N S  T E S T 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace nist2{

int
Runs(int n)
{
	
	int		S, k;
	double	pi, V, erfc_arg, p_value;
/*	
	S = 0;
	for ( k=0; k<n; k++ )
		if ( epsilon[k] )
			S++;
	
	pi = (double)S / (double)n;

	if ( fabs(pi - 0.5) > (2.0 / sqrt(n)) ) {
#ifdef FILE_OUTPUT
		fprintf(stats[TEST_RUNS], "\t\t\t\tRUNS TEST\n");
		fprintf(stats[TEST_RUNS], "\t\t------------------------------------------\n");
		fprintf(stats[TEST_RUNS], "\t\tPI ESTIMATOR CRITERIA NOT MET! PI = %f\n", pi);
#endif
		p_value = 0.0;
		
#ifdef VERIFY_RESULTS
		R1.runs.p_value=p_value;
		R1.runs.pi=pi;
		R1.runs.V=0.0;
		R1.runs.erfc_arg=0.0;
#endif
	}
	else {
  */
		V = 1;
		for ( k=1; k<n; k++ )
    {
			if ( epsilon[k] != epsilon[k-1] )
      {
				V++;
      }
    }

    return V;
	  /*
		erfc_arg = fabs(V - 2.0 * n * pi * (1-pi)) / (2.0 * pi * (1-pi) * sqrt(2*n));
		p_value = erfc(erfc_arg);
	
		printf("Pval: %lf sum %lf",p_value,V);
		
#ifdef VERIFY_RESULTS
		R1.runs.p_value=p_value;
		R1.runs.pi=pi;
		R1.runs.V=V;
		R1.runs.erfc_arg=erfc_arg;
#endif

#ifdef FILE_OUTPUT
		fprintf(stats[TEST_RUNS], "\t\t\t\tRUNS TEST\n");
		fprintf(stats[TEST_RUNS], "\t\t------------------------------------------\n");
		fprintf(stats[TEST_RUNS], "\t\tCOMPUTATIONAL INFORMATION:\n");
		fprintf(stats[TEST_RUNS], "\t\t------------------------------------------\n");
		fprintf(stats[TEST_RUNS], "\t\t(a) Pi                        = %f\n", pi);
		fprintf(stats[TEST_RUNS], "\t\t(b) V_n_obs (Total # of runs) = %d\n", (int)V);
		fprintf(stats[TEST_RUNS], "\t\t(c) V_n_obs - 2 n pi (1-pi)\n");
		fprintf(stats[TEST_RUNS], "\t\t    -----------------------   = %f\n", erfc_arg);
		fprintf(stats[TEST_RUNS], "\t\t      2 sqrt(2n) pi (1-pi)\n");
		fprintf(stats[TEST_RUNS], "\t\t------------------------------------------\n");
		if ( isNegative(p_value) || isGreaterThanOne(p_value) )
			fprintf(stats[TEST_RUNS], "WARNING:  P_VALUE IS OUT OF RANGE.\n");

		fprintf(stats[TEST_RUNS], "%s\t\tp_value = %f\n\n", p_value < ALPHA ? "FAILURE" : "SUCCESS", p_value); fflush(stats[TEST_RUNS]);
#endif
	}
	
#ifdef FILE_OUTPUT
	fprintf(results[TEST_RUNS], "%f\n", p_value); fflush(results[TEST_RUNS]);	
#endif*/
}


int
Runs2(int n)
{       
	int		S, k, i, diff;
	double	pi, erfc_arg, p_value;
	double V;
                /*
	short int LU_byte_weight[256] = 
	{0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5
	,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6
	,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6
	,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7
	,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6
	,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7
	,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7
	,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8};
	short int LU_byte_switches[256] = 
	{0,1,2,1,2,3,2,1,2,3,4,3,2,3,2,1,2,3,4,3,4,5,4,3,2,3,4,3,2,3,2,1
	,2,3,4,3,4,5,4,3,4,5,6,5,4,5,4,3,2,3,4,3,4,5,4,3,2,3,4,3,2,3,2,1
	,2,3,4,3,4,5,4,3,4,5,6,5,4,5,4,3,4,5,6,5,6,7,6,5,4,5,6,5,4,5,4,3
	,2,3,4,3,4,5,4,3,4,5,6,5,4,5,4,3,2,3,4,3,4,5,4,3,2,3,4,3,2,3,2,1
	,1,2,3,2,3,4,3,2,3,4,5,4,3,4,3,2,3,4,5,4,5,6,5,4,3,4,5,4,3,4,3,2
	,3,4,5,4,5,6,5,4,5,6,7,6,5,6,5,4,3,4,5,4,5,6,5,4,3,4,5,4,3,4,3,2
	,1,2,3,2,3,4,3,2,3,4,5,4,3,4,3,2,3,4,5,4,5,6,5,4,3,4,5,4,3,4,3,2
	,1,2,3,2,3,4,3,2,3,4,5,4,3,4,3,2,1,2,3,2,3,4,3,2,1,2,3,2,1,2,1,0} ;
  */    /*
	S = 0;
	for ( k=0; k*8< n; k++ )
	{
		S += LU_byte_weight[array[k]];
		//sum += LU_byte_weight[(get_nth_block4(array,i*8) & 255)];
	}
	if( k*8 > n)S -= LU_byte_weight[array[k-1] >> (8-(k*8-n))];


	pi = (double)S / (double)n;

	if ( fabs(pi - 0.5) > (2.0 / sqrt(n)) ) {
#ifdef FILE_OUTPUT
		fprintf(stats[TEST_RUNS], "\t\t\t\tRUNS TEST\n");
		fprintf(stats[TEST_RUNS], "\t\t------------------------------------------\n");
		fprintf(stats[TEST_RUNS], "\t\tPI ESTIMATOR CRITERIA NOT MET! PI = %f\n", pi);
#endif
		p_value = 0.0;

#ifdef VERIFY_RESULTS
		R2.runs.p_value=p_value;
		R2.runs.pi=pi;
		R2.runs.V=0.0;
		R2.runs.erfc_arg=0.0;
#endif


	}
	else {
          */
	V = 1;
	for ( k=0; (k+1)*8<n; k++ )
	{
		V += LU_byte_switches[array[k]];
		diff = ((array[k]>>7) - (array[k+1] & 1));
		V = V + diff*diff ;

		//if((array[k]>>7) != (array[k] & 1) ) V++;
		//sum += LU_byte_weight[(get_nth_block4(array,i*8) & 255)];
	}

  return V;

	//if((get_nth_block4(array,k*8)&1) == (get_nth_block4(array,k*8+1)&1)) V--;
                    /*
	for(i = k*8; i+1 < n; i++)
	{
		if((get_nth_block4(array,i)&1) != (get_nth_block4(array,i+1)&1)) V++;
	}

	erfc_arg = fabs((double)V - 2.0 * n * pi * (1-pi)) / (2.0 * pi * (1-pi) * sqrt(2*n));
	p_value = erfc(erfc_arg);
	printf("Pval: %lf sum %lf",p_value,V);
#ifdef VERIFY_RESULTS
	R2.runs.p_value=p_value;
	R2.runs.pi=pi;
	R2.runs.V=V;
	R2.runs.erfc_arg=erfc_arg;
#endif
		
#ifdef FILE_OUTPUT
		fprintf(stats[TEST_RUNS], "\t\t\t\tRUNS TEST\n");
		fprintf(stats[TEST_RUNS], "\t\t------------------------------------------\n");
		fprintf(stats[TEST_RUNS], "\t\tCOMPUTATIONAL INFORMATION:\n");
		fprintf(stats[TEST_RUNS], "\t\t------------------------------------------\n");
		fprintf(stats[TEST_RUNS], "\t\t(a) Pi                        = %f\n", pi);
		fprintf(stats[TEST_RUNS], "\t\t(b) V_n_obs (Total # of runs) = %d\n", (int)V);
		fprintf(stats[TEST_RUNS], "\t\t(c) V_n_obs - 2 n pi (1-pi)\n");
		fprintf(stats[TEST_RUNS], "\t\t    -----------------------   = %f\n", erfc_arg);
		fprintf(stats[TEST_RUNS], "\t\t      2 sqrt(2n) pi (1-pi)\n");
		fprintf(stats[TEST_RUNS], "\t\t------------------------------------------\n");
		if ( isNegative(p_value) || isGreaterThanOne(p_value) )
			fprintf(stats[TEST_RUNS], "WARNING:  P_VALUE IS OUT OF RANGE.\n");

		fprintf(stats[TEST_RUNS], "%s\t\tp_value = %f\n\n", p_value < ALPHA ? "FAILURE" : "SUCCESS", p_value); fflush(stats[TEST_RUNS]);
#endif
	}
	
#ifdef FILE_OUTPUT
	fprintf(results[TEST_RUNS], "%f\n", p_value); fflush(results[TEST_RUNS]);
#endif
	
	//printf("R2: %d %lf \n",S,(double)V);
	                    */
}


int
Runs3(int n)
{
	int		S, i;
	double	pi, erfc_arg, p_value;
	double V;
	unsigned int mask;
	unsigned char *p_tmp, *p_end;

	int LUT_HW_size = 16;
	int LUT_HW_Bsize = 2;
	signed char *LUT_HW = LUT_HW_16;

	int LUT_Switches_size = 16;
	int LUT_Switches_Bsize = 2;
	signed char *LUT_Switches = LUT_Switches_16;
	
	if(0)
	{
		LUT_HW_size = 8;
		LUT_HW_Bsize = 1;
		LUT_HW = LUT_HW_8;

		LUT_Switches_size = 8;
		LUT_Switches_Bsize = 1;
		LUT_Switches = LUT_Switches_8;
	}
                          
	mask = get_mask(LUT_HW_size);
                          
	//count bits
	S = 0;
	mask = get_mask(LUT_HW_size);
                          /*
	p_end = array + (n- (n%LUT_HW_size))/8;
	for (p_tmp = array; p_tmp < p_end; p_tmp += LUT_HW_Bsize){
		S += LUT_HW[*((unsigned int*)p_tmp) & mask];
	}
	if (n % LUT_HW_size){
		S += LUT_HW[ *((unsigned int*)p_tmp) & get_mask(n % LUT_HW_size)];
	}

	pi = (double)S / (double)n;
	if (fabs(pi - 0.5) > (2.0 / sqrt(n))) {
#ifdef FILE_OUTPUT
		fprintf(stats[TEST_RUNS], "\t\t\t\tRUNS TEST\n");
		fprintf(stats[TEST_RUNS], "\t\t------------------------------------------\n");
		fprintf(stats[TEST_RUNS], "\t\tPI ESTIMATOR CRITERIA NOT MET! PI = %f\n", pi);
#endif
		p_value = 0.0;

#ifdef VERIFY_RESULTS
		R2.runs.p_value = p_value;
		R2.runs.pi = pi;
		R2.runs.V = 0.0;
		R2.runs.erfc_arg = 0.0;
#endif


	}
	else {

                        */
		V = 1;

		mask = get_mask(LUT_Switches_size + 1);
		p_end = array + ((n-1) - (n-1)% LUT_Switches_size) /8 - LUT_Switches_Bsize + 1;
		
		for (p_tmp = array; p_tmp < p_end; p_tmp += LUT_Switches_Bsize){
			V += LUT_Switches[*((unsigned int*)p_tmp) & mask];
		}

		//last incomplete block
		if ( (n - 1 ) % LUT_Switches_size != 0){
			for (i =  n - ((n - 1) % LUT_Switches_size) ; i < n; i++)
			{
				if ((get_nth_block_effect(array, i - 1) & 1) != (get_nth_block_effect(array, i) & 1))
					V++;
			}

		}

    return V;

                            /*
    
		erfc_arg = fabs((double)V - 2.0 * n * pi * (1 - pi)) / (2.0 * pi * (1 - pi) * sqrt(2 * n));
		p_value = erfc(erfc_arg);

		printf("Pval: %lf sum %lf",p_value,V);
#ifdef VERIFY_RESULTS
		R2.runs.p_value = p_value;
		R2.runs.pi = pi;
		R2.runs.V = V;
		R2.runs.erfc_arg = erfc_arg;
#endif

#ifdef FILE_OUTPUT
		fprintf(stats[TEST_RUNS], "\t\t\t\tRUNS TEST\n");
		fprintf(stats[TEST_RUNS], "\t\t------------------------------------------\n");
		fprintf(stats[TEST_RUNS], "\t\tCOMPUTATIONAL INFORMATION:\n");
		fprintf(stats[TEST_RUNS], "\t\t------------------------------------------\n");
		fprintf(stats[TEST_RUNS], "\t\t(a) Pi                        = %f\n", pi);
		fprintf(stats[TEST_RUNS], "\t\t(b) V_n_obs (Total # of runs) = %d\n", (int)V);
		fprintf(stats[TEST_RUNS], "\t\t(c) V_n_obs - 2 n pi (1-pi)\n");
		fprintf(stats[TEST_RUNS], "\t\t    -----------------------   = %f\n", erfc_arg);
		fprintf(stats[TEST_RUNS], "\t\t      2 sqrt(2n) pi (1-pi)\n");
		fprintf(stats[TEST_RUNS], "\t\t------------------------------------------\n");
		if (isNegative(p_value) || isGreaterThanOne(p_value))
			fprintf(stats[TEST_RUNS], "WARNING:  P_VALUE IS OUT OF RANGE.\n");

		fprintf(stats[TEST_RUNS], "%s\t\tp_value = %f\n\n", p_value < ALPHA ? "FAILURE" : "SUCCESS", p_value); fflush(stats[TEST_RUNS]);
#endif
	}

#ifdef FILE_OUTPUT
	fprintf(results[TEST_RUNS], "%f\n", p_value); fflush(results[TEST_RUNS]);
#endif

	//printf("R2: %d %lf \n",S,(double)V);
                              */
}



int
Runs4(int n)
{
	int		S, k, i, diff;
	double	pi, erfc_arg, p_value;
	double V;

	unsigned char *startAddr, *endAddr;
	int LUT_HW_size = 64;
	int LUT_HW_Bsize = 8;

	unsigned int NumberOfRuns=0;
	unsigned int val1;
                                
	S = 0;

	startAddr = array;
	endAddr = array + (n- (n % LUT_HW_size))/8;

  /*
	S += popCountLUT16_64((unsigned __int64*)startAddr, (unsigned __int64*)endAddr);

	pi = (double)S / (double)n;

	if ( fabs(pi - 0.5) > (2.0 / sqrt(n)) ) {
#ifdef FILE_OUTPUT
		fprintf(stats[TEST_RUNS], "\t\t\t\tRUNS TEST\n");
		fprintf(stats[TEST_RUNS], "\t\t------------------------------------------\n");
		fprintf(stats[TEST_RUNS], "\t\tPI ESTIMATOR CRITERIA NOT MET! PI = %f\n", pi);
#endif
		p_value = 0.0;

#ifdef VERIFY_RESULTS
		R2.runs.p_value=p_value;
		R2.runs.pi=pi;
		R2.runs.V=0.0;
		R2.runs.erfc_arg=0.0;
#endif


	}
	else {
                                  */
	V = 1;
	endAddr = array + (n- (n % 32))/8;
	V += runsLUT16_32( (unsigned __int32*)startAddr, (unsigned __int32*)endAddr );

  return V;
	//TODO LAST Block
                                    /*
	erfc_arg = fabs((double)V - 2.0 * n * pi * (1-pi)) / (2.0 * pi * (1-pi) * sqrt(2*n));
	p_value = erfc(erfc_arg);
	printf("Pval: %lf sum %lf",p_value,V);

#ifdef VERIFY_RESULTS
	R2.runs.p_value=p_value;
	R2.runs.pi=pi;
	R2.runs.V=V;
	R2.runs.erfc_arg=erfc_arg;
#endif
		
#ifdef FILE_OUTPUT
		fprintf(stats[TEST_RUNS], "\t\t\t\tRUNS TEST\n");
		fprintf(stats[TEST_RUNS], "\t\t------------------------------------------\n");
		fprintf(stats[TEST_RUNS], "\t\tCOMPUTATIONAL INFORMATION:\n");
		fprintf(stats[TEST_RUNS], "\t\t------------------------------------------\n");
		fprintf(stats[TEST_RUNS], "\t\t(a) Pi                        = %f\n", pi);
		fprintf(stats[TEST_RUNS], "\t\t(b) V_n_obs (Total # of runs) = %d\n", (int)V);
		fprintf(stats[TEST_RUNS], "\t\t(c) V_n_obs - 2 n pi (1-pi)\n");
		fprintf(stats[TEST_RUNS], "\t\t    -----------------------   = %f\n", erfc_arg);
		fprintf(stats[TEST_RUNS], "\t\t      2 sqrt(2n) pi (1-pi)\n");
		fprintf(stats[TEST_RUNS], "\t\t------------------------------------------\n");
		if ( isNegative(p_value) || isGreaterThanOne(p_value) )
			fprintf(stats[TEST_RUNS], "WARNING:  P_VALUE IS OUT OF RANGE.\n");

		fprintf(stats[TEST_RUNS], "%s\t\tp_value = %f\n\n", p_value < ALPHA ? "FAILURE" : "SUCCESS", p_value); fflush(stats[TEST_RUNS]);
#endif
	}
	
#ifdef FILE_OUTPUT
	fprintf(results[TEST_RUNS], "%f\n", p_value); fflush(results[TEST_RUNS]);
#endif
	
	//printf("R2: %d %lf \n",S,(double)V);
	
}                                     */
}
}
