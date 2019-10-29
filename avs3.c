#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

#define N 10000

clock_t start, stop;
struct timeval tv1,tv2,dtv;
struct timezone tz;

long long TimeValue = 0;
typedef unsigned long long TimeVal;

TimeVal tick, tick1, tickh;
unsigned ttime = 0;

static __inline__ TimeVal getticks()
{
	unsigned a, d;
	asm("cpuid");
	asm volatile ("rdtsc" : "=a" (a), "=d" (d));
	return (((TimeVal)a) | (((TimeVal) d) << 32));
}

void time_start() { gettimeofday(&tv1, &tz); }

long time_stop()
{
	gettimeofday(&tv2, &tz);
	dtv.tv_sec = tv2.tv_sec - tv1.tv_sec;
	dtv.tv_usec = tv2.tv_usec - tv1.tv_usec;
	if(dtv.tv_usec<0){
		dtv.tv_sec--;
		dtv.tv_usec+=1000000;
	}
	return dtv.tv_sec*1000+dtv.tv_usec/1000;
}

double F(double x)
{
	double f;
	f = sin(x)+cos(x);
	return f;
}

double Sim_clock(int iter, double A, double B)
{
	const double Pi = 3.14159;
	double S, x, a, b, h;
	int i = 0;
	while(i<iter){
		S = 0;
		a = A;
		b = B;
		h = (b-a)/N;
		x = a+h;
		while (x<b)
		{
			S = S+4*F(x);
			x = x+h;
			if(x>=b) break;
			S = S+ 2*F(x);
			x = x+h;
		}
		S = (h/3)*(S + F(a) + F(b));
		i++;
	}
	return S;
}

int main(){
	int iter = 0;
	double A=0;
	double B=0;
	double resultt=0;
	double result[3];
	for(int i=0; i<3; i++){result[i]=0;};
	double timeresult_clock = 0;
	double timeresult_gettime = 0;
	double timeresult_TSC = 0;
	scanf("%d %lf %lf", &iter, &A, &B);
//-------------------------------------------------------------
	printf("\nA=%lf B=%lf\n", A, B);
	printf("\n-----------------------------------------\n");
	for(int i=0;i<3;i++){
	  printf("\ni = %d\n",i);
	  start = clock();
	  resultt = Sim_clock(iter,A,B);
	  stop = clock();
	  printf("\nresult = %6.10lf\n", resultt);
	  timeresult_clock = (double)(stop-start)/CLOCKS_PER_SEC ;
	  result[i] = timeresult_clock;
	  printf("\ntimeresult_clock = %6.10lf \n", timeresult_clock);
	}
	
	printf("\nAb = %lf\n", abs((result[0]+result[1]+result[2])/3-result[1]));
	  printf("\nOtn = %lf\n",result[1]/((result[0]+result[1]+result[2])/3));
	printf("\n-----------------------------------------\n");
//--------------------------------------------------------------
	printf("\n-----------------------------------------\n");
	for(int i=0;i<3;i++){
	  printf("\ni = %d\n",i);
	  time_start();
	  resultt = Sim_clock(iter,A,B);
	  timeresult_gettime =(double)time_stop() / 1000;
	  result[i]=timeresult_gettime;
	  printf("\nresult = %6.10lf\n", resultt);
	  printf("\ntimeresult_gettime = %6.10lf \n", timeresult_gettime);
	}
	
	printf("\nAb = %lf\n", abs((result[0]+result[1]+result[2])/3-result[1]));
	  printf("\nOtn = %lf\n",result[1]/((result[0]+result[1]+result[2])/3));
	printf("\n-----------------------------------------\n");
//---------------------------------------------------------------------
	printf("\n-----------------------------------------\n");
	for(int i=0;i<3;i++){
	  printf("\ni = %d\n",i);
	  tick = getticks();
	  resultt = Sim_clock(iter,A,B);
	  tick1 = getticks();
	  ttime = (unsigned)((tick1 - tick) / 3193998);
	  result[i]=ttime/1000.;
	  printf("\nresult = %6.10lf\n", resultt);
	  printf("\ntimeresult_TSC = %6.10lf \n", ttime / 1000.);
	}
	
	printf("\nAb = %lf\n", abs((result[0]+result[1]+result[2])/3-result[1]));
	  printf("\nOtn = %lf\n",result[1]/((result[0]+result[1]+result[2])/3));
	printf("\n-----------------------------------------\n");
//---------------------------------------------------------------------
	return 0;
}
