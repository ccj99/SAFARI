#include<map>
#include<algorithm>
#include<iostream>
#include<utility>
#include<vector>
#include<cmath>
#include<stdio.h>
#include "util.h"
#include "dataset.h"

using namespace std;

void RankDataset::init(int _user_number,int _item){
	user_number = _user_number;
	item = _item;
	kt.init(item);
	dataset.clear();

	for(int i=0;i<item;i++)
		first_order[i].clear();

	for(int i=0;i<item;i++)
		for(int j=i+1;j<item;j++)
			second_order[i][j].clear();
}

void RankDataset::put_rank(int *rank,double p){
	for(int i=0;i<item;i++)
		first_order[i][rank[i]] += p;

	for(int i=0;i<item;i++)
		for(int j=i+1;j<item;j++)
			second_order[i][j][make_pair(rank[i],rank[j])] += p;
	
	vector<int>q;
	for(int i=0;i<item;i++)
		q.push_back(rank[i]);
	dataset.push_back(q);
}

void RankDataset::read_file(char *file_name){
	FILE *fp = fopen(file_name, "r");
	int *rank = new int[item+3];
	
	for(int i=0;i<user_number;i++){
		for(int j=0,x;j<item;j++){
			fscanf(fp,"%d",&x);
			rank[x] = j;
		}
		put_rank(rank, 1.0/user_number);
	}
	
	fclose(fp);
}

pair<double,double> first_order(RankDataset *a,RankDataset *b){
	vector<double>res;
	double tvd=0;
	int y = 0;
	for(int i=0;i<a->item;i++){
		for(map<int, double>::iterator it=a->first_order[i].begin();it!=a->first_order[i].end();it++){
			double t = fabs(it->second - b->first_order[i][it->first]);
			tvd += t;
			res.push_back( t / max(1.0/a->user_number, it->second) );
			y += 1;
		}
		for(map<int, double>::iterator it=b->first_order[i].begin();it!=b->first_order[i].end();it++){
			if( a->first_order[i].count(it->first) > 0 )
				continue;
			double t = fabs(it->second - a->first_order[i][it->first]);
			tvd += t;
			res.push_back( t / max(1.0/a->user_number, a->first_order[i][it->first]) );
		}
	}
	//printf("y %d\n",y);

	
	tvd /= 2*a->item;
	sort(res.begin(),res.end());
	double re = res[res.size()/2];
	if ( res.size() % 2 == 1 )
		re = (res[res.size()/2-1] + res[res.size()/2])/2;
	
	return make_pair(tvd, re);
}

pair<double,double> second_order(RankDataset *a,RankDataset *b){
	vector<double>res;
	double tvd=0;
	for(int i=0;i<a->item;i++)
		for(int j=i+1;j<a->item;j++){
			for(map<pair<int,int>, double>::iterator it=a->second_order[i][j].begin();it!=a->second_order[i][j].end();it++){
				double t = fabs(it->second - b->second_order[i][j][it->first]);
				//printf("++ <%f %f>\n",it->second, b->second_order[i][j][it->first]);
				tvd += t;
				res.push_back( t / max(1.0/a->user_number, it->second) );
			}
			for(map<pair<int,int>, double>::iterator it=b->second_order[i][j].begin();it!=b->second_order[i][j].end();it++){
				if( a->second_order[i][j].count(it->first) > 0 )
					continue;
				//printf("-- <%f %f>\n",a->second_order[i][j][it->first], it->second);
				double t = fabs(it->second - a->second_order[i][j][it->first]);
				tvd += t;
				res.push_back( t / max(1.0/a->user_number, a->second_order[i][j][it->first]) );
			}
	}

	tvd /= 2 * (a->item*(a->item-1)/2);
	sort(res.begin(),res.end());
	double re = res[res.size()/2];
	if ( res.size() % 2 == 1 )
		re = (res[res.size()/2-1] + res[res.size()/2])/2;
	
	return make_pair(tvd, re);
}
