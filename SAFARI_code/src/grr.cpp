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

void Grr::init(double _epsilon,int _item){
	Ldp::init(_epsilon, _item);
	
	C = new double[item+3];
	memset(C,0,sizeof(double)*(item+3));

	p = exp(_epsilon)/(exp(_epsilon)+item-1);
	q = 1.0/(exp(_epsilon)+item-1);	
}

void Grr::get(int r){
	Ldp::get(r);
	
	int y = 0;
	double pro = random_p();
	
	for( ; y<item ; y++){
		if( y == r )
			pro -= p;
		else
			pro -= q;
		if( pro < 0 )
			break;
	}
	
	C[y] += 1;
}

void Grr::compute(){
	Ldp::compute();
	
	if(item == 1){
		ldp_number[0] = 1.0;
		return ;
	}
	
	double tvd = 0;
	for(int i=0;i<item;i++){
		ldp_number[i] = (C[i] - user_number*q) / (p - q) / user_number;
		
		//printf("%f %f\n",ldp_number[i],real_number[i]);
		tvd += fabs(ldp_number[i] - real_number[i]);
	}
	//printf("tvd %f\n",tvd/2);
}