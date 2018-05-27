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

const int hash_p = 1000000007;

void Olh::init(double _epsilon,int _item){
	Ldp::init(_epsilon, _item);
	
	C = new double[item+3];
	memset(C,0,sizeof(double)*(item+3));
	
	g = (int)ceil(exp(_epsilon)+1);
	
	p = exp(_epsilon)/(exp(_epsilon)+g-1);
	q = 1.0/(exp(_epsilon)+g-1);	
}

int universal_hash(int x,int hash_a,int hash_b,int hash_m){
	return ((x+1LL)*hash_a + hash_b) % hash_p % hash_m;
}

void Olh::get(int r){
	Ldp::get(r);
	
	int hash_a = (1LL*rand()*RAND_MAX+rand())%hash_p;
	int hash_b = (1LL*rand()*RAND_MAX+rand())%hash_p;
	int v = universal_hash(r, hash_a, hash_b, g) , y = 0;
	
	double pro = random_p();
	
	for(;y<g;y++){
		if( y == v )
			pro -= p;
		else
			pro -= q;
		if( pro < 0 )
			break;
	}
	
	for(int x=0;x<item;x++)
		if( universal_hash(x, hash_a, hash_b, g) == y )
			C[x] += 1;
}

void Olh::compute(){
	Ldp::compute();
	
	if(item == 1){
		ldp_number[0] = 1.0;
		return ;
	}
	
	double tvd = 0;
	for(int i=0;i<item;i++){
		ldp_number[i] = (C[i] - user_number*1.0/g) / (p - 1.0/g) / user_number;
		
		//printf("%f %f\n",ldp_number[i],real_number[i]);
		tvd += fabs(ldp_number[i] - real_number[i]);
	}
	//printf("tvd %f\n",tvd/2);
}