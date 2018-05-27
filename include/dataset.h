#ifndef _DATASET_H
#define _DATASET_H

#include<map>
#include<algorithm>
#include<iostream>
#include<utility>
#include<vector>
#include "util.h"

using namespace std;

class RankDataset{
public:
	KangTuo kt;
	int user_number, item;
	map<int, double>first_order[MAXRANK];
	map<pair<int,int>, double>second_order[MAXRANK][MAXRANK];
	vector<vector<int> >dataset;
	void init(int _user_number,int _item);
	void put_rank(int *rank,double p);
	void read_file(char *file_name);
};

pair<double,double> first_order(RankDataset *a,RankDataset *b); //a is the real dataset
pair<double,double> second_order(RankDataset *a,RankDataset *b); //a is the real dataset

#endif
