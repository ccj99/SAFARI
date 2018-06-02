#include<map>
#include<algorithm>
#include<iostream>
#include<utility>
#include<vector>
#include<cmath>
#include<stdio.h>
#include<string.h>
#include "util.h"

using namespace std;

const double eps = 1e-8;

void random_init(){
	struct timeb timeSeed;
	ftime(&timeSeed);
	srand(timeSeed.time * 1000 + timeSeed.millitm);
}

double random_p(){
	return (1.0*rand()*RAND_MAX+rand() )/RAND_MAX/RAND_MAX;
}

double mutual_information(double *px,double *py,double *pxy,int lenx,int leny){
	double mi = 0;
	for(int i=0;i<lenx;i++)
		for(int j=0;j<leny;j++){
			if( fabs(px[i]) < eps || fabs(py[j]) < eps || 
				fabs(pxy[i*leny+j]) < eps )
				continue;
			mi += pxy[i*leny+j] * log(pxy[i*leny+j] / px[i] / py[j]);
		}
	return mi;
}

void normalize(double *x,int len){
	double sum = 0;
	for(int i=0;i<len;i++){
		sum += x[i];
	}
	for(int i=0;i<len;i++){
		x[i] /= sum;
	}
	
	sum = 0;
	for(int i=0;i<len;i++){
		x[i] = max(x[i], 0.0);
		sum += x[i];
	}
	for(int i=0;i<len;i++){
		x[i] /= sum;
	}
}

void KangTuo::init(int nn){
	n = nn;
	base[0] = base[1] = 1;
	for(int i=2;i<=n;i++)
		base[i] = base[i-1]*i;
}
int KangTuo::Hash(int *a)
{
	int ret=0;
	for(int i=n-1;i>=0;i--)
	{
		int c=0;
		for(int j=i+1;j<n;j++)
			if(a[j]>a[i])
				c++;
		ret += c * base[n-i-1];
	}
	return ret;
}
void KangTuo::get_hash(int h,int *a)
{
	int us[60];
	memset(us,0,sizeof us);
	for(int i=0;i<n;i++)
	{
		int c = h / base[n-i-1];
		h %= base[n-i-1];
		int j=n-1;
		while( c || us[j] )
			c -= (1^us[j--]);
		a[i] = j;
		us[j] = 1;
	}
}