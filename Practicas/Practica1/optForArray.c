#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>
#include "../../utils/utils.h"


int main(int argc, char*argv[]){
 double timetick;
 unsigned long N = atol(argv[1]);
 unsigned long R = atol(argv[2]);
 unsigned long r;
 unsigned long i;
 unsigned long *a;
 a = (unsigned long*)malloc(sizeof(unsigned long)*N);

 timetick = dwalltime();
 for(r=0;r<R;r++){
	 for(i=0;i<N;i++){
		a[i] = 1;
 	}
 }

 printf("Tiempo de direccionamiento a[i]: %f \n", dwalltime() - timetick);

 timetick = dwalltime();

 register unsigned long *p;
 for(r=0;r<R;r++){
 	for(p=a ; p < a+N ; p++){
		*p = 1;
 	}
 }
 printf("Tiempo de direccionamiento *p: %f \n", dwalltime() - timetick);

 free(a);
 return 0;
}
