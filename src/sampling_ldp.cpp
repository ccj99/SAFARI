#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "ldp.h"
#include "ldp.h"


void Noldp::init(double _epsilon,int _item){
	Ldp::init(_epsilon, _item);
}

void Noldp::get(int r){
	Ldp::get(r);
}

void Noldp::compute(){
	Ldp::compute();
	
	if(item == 1){
		ldp_number[0] = 1.0;
		return ;
	}
	
	for(int i=0;i<item;i++)
		ldp_number[i] = real_number[i];
}

void SamplingLdp::init(int *_item_number,int _value_number,double epsilon,char *method){
	value_number = _value_number;
	user_number = 0;
	item_number = new int[value_number+3];
	
	for(int i=0;i<value_number;i++)
		item_number[i] = _item_number[i];
	
    ldp = new Ldp*[value_number+1];

    for(int i=0;i<value_number;i++){
        if( strcmp(method, "rappor") == 0 )
            ldp[i] = new Rappor;
        else if( strcmp(method, "sh") == 0 )
            ldp[i] = new Sh;
		else if( strcmp(method, "noldp") == 0 )
            ldp[i] = new Noldp;
		else{
			fprintf(stderr, "No %s method\n",method);
			exit(0);
		}
		ldp[i]->init(epsilon, item_number[i]);
    }
	for(int i=0;i<value_number;i++)
		for(int j=0;j<item_number[i];j++)
			id.push_back(i);
	
	real_number = new double *[value_number+3];
	ldp_number = new double *[value_number+3];
	for(int i=0;i<value_number;i++){
		real_number[i] = new double[item_number[i]+3];
		ldp_number[i] = new double[item_number[i]+3];
		for(int j=0;j<item_number[i];j++){
			real_number[i][j] = 0;
			ldp_number[i][j] = 0;
		}
	}
}

void SamplingLdp::get(int *items){
	int q = id[rand()%id.size()];
    ldp[q]->get(items[q]);
	user_number += 1;
	
	for(int i=0;i<value_number;i++)
		real_number[i][items[i]] += 1;
    
}

void SamplingLdp::compute(){
	for(int i=0;i<value_number;i++){
		ldp[i]->compute();
		for(int j=0;j<item_number[i];j++)
			ldp_number[i][j] = ldp[i]->ldp_number[j];  
	}
	
	for(int i=0;i<value_number;i++){
		for(int j=0;j<item_number[i];j++){
			real_number[i][j] /= user_number;
		}
	}
}