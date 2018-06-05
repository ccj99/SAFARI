#include <stdio.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include "ldp.h"
#include "util.h"
#include "dataset.h"

using namespace std;

class LeftKangTuo{
public:
    int item, left_item;
    map<vector<int>, int> rank2id;
    vector<vector<int> > id2rank;
    void init(int _item,int _left_item);
    int Hash(vector<int> &rank);
	void get_hash(int h, vector<int> &rank);
	void dfs(vector<int> p);
};

void get_left_rank(const vector<int> &rank,const vector<int> &res_set, 
					const vector<int> &left_res_set,vector<int> &res_rank);

int random_choice(double *pro,int len);

void get_other_rank(const vector<int> &left_rank,int item,vector<int> &right_rank);

SamplingLdp sldp;			

void second_collect(RankDataset *real_dataset, vector<vector<int> >& hierarchical, 
				 double epsilon, char* method, RankDataset *ldp_dataset){
	printf("\nphase_two epsilon %f method %s\n\n", epsilon, method);
    
	int item = real_dataset->item;
	int set_number = hierarchical.size();
	
    vector<KangTuo> kangtuos;
    vector<LeftKangTuo> leftkangtuos;
	vector<vector<int> >res_sets;
	
    for(int i=0;i<set_number;i++){
        KangTuo kt;
        kt.init(hierarchical[i].size());
        kangtuos.push_back(kt);
    }
	
	for(int i=0;i<set_number - 1;i++){
		vector<int> res_set;
		LeftKangTuo lkt;
		
		for(int ii=i;ii<set_number;ii++)
			for(int j=0;j<(int)hierarchical[ii].size();j++)
				res_set.push_back(hierarchical[ii][j]);
        res_sets.push_back(res_set);
		
        lkt.init(res_set.size(), hierarchical[i].size());
        leftkangtuos.push_back(lkt);
    }
    
    int value_number = 2 * set_number - 1;
    int *item_number = new int[value_number+3];
    
    for(int i=0;i<value_number;i++){
		if( i < set_number ){
			item_number[i] = kangtuos[i].base[kangtuos[i].n];
		}else{
			item_number[i] = leftkangtuos[i-set_number].id2rank.size();
		}
	}
    
    sldp.init(item_number, value_number, epsilon, method);
    
    int *s = new int[value_number+3];
	int *rank = new int[MAXRANK];
    for(int d=0;d<(int)real_dataset->dataset.size();d++){
        for(int i=0;i<value_number;i++){
			if( i < set_number ){
				vector<int>res_rank;

				get_left_rank(real_dataset->dataset[d], hierarchical[i], hierarchical[i], res_rank);

				for(int j=0;j<(int)res_rank.size();j++){
					rank[j] = res_rank[j]; 
				}
				
				s[i] = kangtuos[i].Hash(rank);
			}else{
				vector<int>res_rank;
				int ii = i-set_number;

				get_left_rank(real_dataset->dataset[d], res_sets[ii], hierarchical[ii], res_rank);
				
				s[i] = leftkangtuos[ii].Hash(res_rank);
				
            }
        }
        sldp.get(s);
    }

    sldp.compute();

	for(int i=0;i<value_number;i++){
		normalize(sldp.ldp_number[i], sldp.item_number[i]);
	}
	
	//rebuild dataset
	int *left_rank = new int[item+3];
	int *right_rank = new int[item+3];
	vector<int> left_mid_rank, right_mid_rank;
	
	for(int d=0;d<(int)real_dataset->user_number;d++){
		int h = random_choice(sldp.ldp_number[set_number-1], hierarchical.back().size());
		kangtuos.back().get_hash(h, right_rank);
		
		for(int i=set_number-2;i>=0;i--){
			int left_len = hierarchical[i].size();
			int all_len  = res_sets[i].size();
			int h = random_choice(sldp.ldp_number[i], kangtuos[i].base[kangtuos[i].n]);
			kangtuos[i].get_hash(h, left_rank);
			
			h = random_choice(sldp.ldp_number[set_number+i], leftkangtuos[i].id2rank.size());
			leftkangtuos[i].get_hash(h, left_mid_rank);

			get_other_rank(left_mid_rank, all_len, right_mid_rank);
			
			for(int i=0;i<all_len;i++){
				if( i < left_len ){
					rank[i] = left_mid_rank[left_rank[i]];
				}else{
					rank[i] = right_mid_rank[right_rank[i-left_len]];
				}
			}
			for(int i=0;i<all_len;i++)
				right_rank[i] = rank[i];
		}
		
		for(int i=0;i<item;i++){
			rank[ res_sets[0][i] ] = right_rank[i];
		}
		
		ldp_dataset->put_rank(rank, 1.0/real_dataset->user_number);
	}
	
	/*for(int i=0;i<value_number;i++){
		printf("-----%d-----\n",i);
		if( i < set_number ){
			for(int j=0;j<item_number[i];j++){
				printf("%f >> ",sldp.ldp_number[i][j]);
				kangtuos[i].get_hash(j, left_rank);
				for(int z=0;z<kangtuos[i].n;z++){
					printf("%d,",left_rank[z]);
				}printf("\n");
			}
		}else{
			for(int j=0;j<item_number[i];j++){
				printf("%f >> ",sldp.ldp_number[i][j]);
				for(int z=0;z<leftkangtuos[i-set_number].id2rank[j].size();z++){
					printf("%d,",leftkangtuos[i-set_number].id2rank[j][z]);
				}printf("\n");
			}
			
		}
	}*/
}

int random_choice(double *pro,int len){
	double p = random_p();
	for(int i=0;i<len;i++){
		p -= pro[i];
		if( p < 0 ){
			return i;
		}
	}
	return len-1;
}

void LeftKangTuo::init(int _item,int _left_item){
	rank2id.clear();
	id2rank.clear();
	
	item = _item;
	left_item = _left_item;
	
	dfs(vector<int>());
}

int LeftKangTuo::Hash(vector<int> &rank){
	sort(rank.begin(), rank.end());
	assert (rank2id.count(rank) > 0 );
	
	return rank2id[rank];
}

void LeftKangTuo::get_hash(int h, vector<int> &rank){
	assert( h < (int)id2rank.size() );
	
	rank = id2rank[h];
}

void LeftKangTuo::dfs(vector<int> p){
	if( (int)p.size() == left_item ){
		rank2id[p] = id2rank.size();
		id2rank.push_back(p);
		return ;
	}
	int pos = 0;
	if(p.size() > 0){
		pos = p.back() + 1;
	}
	for(;pos<item;pos++){
		vector<int> q = p;
		q.push_back(pos);
		dfs(q);
	}
}

void get_left_rank(const vector<int> &rank,const vector<int> &res_set, 
					const vector<int> &left_res_set,vector<int> &res_rank){
    vector<int>s;
    for(int i=0;i<(int)res_set.size();i++){
        s.push_back(rank[res_set[i]]);
    }
    
    sort(s.begin(), s.end());
    
    for(int i=0;i<(int)left_res_set.size();i++){
        int r = lower_bound(s.begin(), s.end(), rank[left_res_set[i]]) - s.begin();
        res_rank.push_back(r);
    }
}

void get_other_rank(const vector<int> &left_rank,int item,vector<int> &right_rank){
	right_rank.clear();
	for(int i=0;i<item;i++){
		if( find(left_rank.begin(), left_rank.end(), i) == left_rank.end() )
			right_rank.push_back(i);
	}
}