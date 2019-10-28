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
/*
unsigned long long time_RDTSC()
{	union ticks
	{
		unsigned long long tx;
		struct dblword{ long tl,th; } dw;
	}t;
	asm("rdtsc\n": "=a" (t.dw.tl),"=d"(t.dw.th));
	return t.tx;
}
void time_startTSC() {TimeValue=time_RDTSC();}
long long time_stopTSC(){return time_RDTSC() - TimeValue;}
*/
/*

void tsc(long n)
{

tick = getticks(); //tsc
for(int i = 0; i < n; i++)
exp_(1500.);
tick1 = getticks(); //tsc_reg
time = (unsigned)((tick1 - tick) / 1991989);
printf("%lf\n", time / 1000.);
}*/

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

double Sim_clock(int iter)
{
	const double Pi = 3.14159;
	double S, x, a, b, h;
	int i = 0;
	while(i<iter){
		S = 0;
		a = 0;
		b = Pi;
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
	double result = 0;
	double timeresult_clock = 0;
	double timeresult_gettime = 0;
	double timeresult_TSC = 0;
	scanf("%d", &iter);
//-------------------------------------------------------------
	start = clock();
	result = Sim_clock(iter);
	stop = clock();
	printf("\n%6.10f\n", result);
	timeresult_clock = (double)(stop-start)/CLOCKS_PER_SEC ;
	printf("\ntimeresult_clock = %6.10f \n", timeresult_clock);
//--------------------------------------------------------------
	time_start();
	result = Sim_clock(iter);
	timeresult_gettime =(double)time_stop() / 1000;
	printf("\n%6.10f\n", result);
	printf("\ntimeresult_gettime = %6.10f \n", timeresult_gettime);
//---------------------------------------------------------------------
	tick = getticks();
	result = Sim_clock(iter);
	tick1 = getticks();
	ttime = (unsigned)((tick1 - tick) / 3193998);
	printf("\n%6.10f\n", result);
	printf("\ntimeresult_TSC = %6.10lf \n", ttime / 1000.);
//---------------------------------------------------------------------
	return 0;
}
