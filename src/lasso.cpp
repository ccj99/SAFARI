#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<algorithm>
#include "lasso.h"

using namespace std;

void Lasso::init(int _n,int _m){
	n = _n;
	m = _m;
	
	par_x = new double*[m+3];
	for(int i=0;i<m;i++){
		par_x[i] = new double[n+3];
		memset(par_x[i], 0, (sizeof (double))*(n+3));
	}
	
	par_y = new double[m+3];
	par_b = new double[n+3];
	K = new double[m+3];
	in = new bool[n+3];
}

double Lasso::cos2(int j){
	double xy = 0, x2 = 0 , y2 = 0;
	for(int i=0;i<m;i++){
		xy += par_x[i][j] * par_y[i];
		x2 += par_x[i][j] * par_x[i][j];
		y2 += par_y[i] * par_y[i];
	}
	return xy*xy/x2/y2;
}

double Lasso::dist(){
	double sum_dist = 0;
	for(int i=0;i<m;i++)
		sum_dist += par_y[i] * par_y[i];
	return sum_dist;
}

void Lasso::Least_Angle_Regression(){
	memset(par_b,0,(sizeof (double))*(n+3));
	memset(K,0,(sizeof (double))*(m+3));
	memset(in,0,(sizeof (bool))*(n+3));
	
	int p = -1;
	double t, maxcos2=0;
	for(int j=0;j<n;j++){
		t = cos2(j);
		if( t > maxcos2 ){
			p = j;
			maxcos2 = t;
		}
	}
	for(int i=0;i<m;i++)
		K[i] = par_x[i][p];
	in[p] = true;
	
	for(int cnt=1;cnt <= n && dist() > eps;cnt++){
		double d = 1e8;
		p = -1;
		for(int i=0;i<m;i++) if(K[i] > eps){
			d = min(d, par_y[i]/K[i]);
		}
		
		for(int j=0;j<n;j++) if(!in[j]){
			double kk = 0, yk = 0;
			for(int i=0;i<m;i++){
				kk += K[i]*(K[i]-par_x[i][j]);
				yk += par_y[i]*(K[i]-par_x[i][j]);
			}
			double tmpd = yk/kk;
			if( tmpd > 0 && tmpd < d ){
				d = tmpd;
				p = j;
			}
		}
		
		for(int j=0;j<n;j++) if(in[j])
			par_b[j] += d/cnt;
		for(int i=0;i<m;i++)
			par_y[i] -= d * K[i];
		if( p == -1 ) break;
		in[p] = true;
		for(int i=0;i<m;i++)
			K[i] = (K[i]*cnt+par_x[i][p])/(cnt+1);
	}
}
	
	