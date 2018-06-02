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

void Rappor::init(double _epsilon,int _item){
	Ldp::init(_epsilon, _item);
	
	cnt = new double[item+3];
	memset(cnt,0,sizeof(double)*(item+3));

	if( item <= 2 )
		p = exp(_epsilon)/(exp(_epsilon)+1);
	else
		p = exp(_epsilon/2)/(exp(_epsilon/2)+1);
}

void Rappor::get(int r){
	Ldp::get(r);
	for(int i=0,s;i<item;i++){
		s = i==r;
		if( random_p() > p )
			s ^= 1;
		cnt[i] += s;
	}
}

void Rappor::compute(){
	Ldp::compute();
	
	if(item == 1){
		ldp_number[0] = 1.0;
		return ;
	}
	
	//double tvd = 0;
	for(int i=0;i<item;i++){
		double x = cnt[i]*1.0/user_number;
		ldp_number[i] = (x + p - 1) / (2*p - 1);
		
		//printf("%f %f\n",ldp_number[i],real_number[i]);
		//tvd += fabs(ldp_number[i] - real_number[i]);
	}
	//printf("tvd %f\n",tvd/2);
}