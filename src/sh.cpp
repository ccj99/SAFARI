#include<map>
#include<algorithm>
#include<iostream>
#include<utility>
#include<vector>
#include<cmath>
#include<stdio.h>
#include "util.h"
#include "ldp.h"

using namespace std;

void Sh::init(double _epsilon,int _item){
	Ldp::init(_epsilon, _item);
	
	m  = 800;
	Z = new double[m+3];
	memset(Z,0,sizeof(double)*(m+3));
	
	p = exp(_epsilon)/(exp(_epsilon)+1);
	c = (exp(_epsilon)+1)/(exp(_epsilon)-1);
	
	phi = new int *[item+3];
	for(int i=0;i<item;i++){
		phi[i] = new int[m+3];
		for(int j=0;j<m;j++)
			phi[i][j] = 2*(rand()&1)-1;
	}
}

void Sh::get(int r){
	Ldp::get(r);
	int s = rand() % m;
	if( random_p() < p )
		Z[s] += phi[ r ][ s ];
	else
		Z[s] += -phi[ r ][ s ];
}

void Sh::compute(){
	Ldp::compute();
	
	if(item == 1){
		ldp_number[0] = 1.0;
		return ;
	}
	
	for(int i=0;i<m;i++)
		Z[i] /= user_number;
	
	for(int i=0; i<item; i++){
		ldp_number[i] = 0;
		for(int j=0; j<m; j++){
			ldp_number[i] += phi[i][j] * Z[j] * c;
		}
	}
}