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

void random_init(){
	struct timeb timeSeed;
	ftime(&timeSeed);
	srand(timeSeed.time * 1000 + timeSeed.millitm);
}

double random_p(){
	return (1.0*rand()*RAND_MAX+rand() )/RAND_MAX/RAND_MAX;
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