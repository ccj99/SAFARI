#ifndef _SAFARI_H
#define _SAFARI_H
#include<algorithm>
#include "dataset.h"

using namespace std;

void first_collect(RankDataset *real_dataset, double epsilon, char* method,
				double mI[MAXRANK][MAXRANK][MAXRANK]);

void first_collect_with_lasso(RankDataset *real_dataset, double epsilon, char* method,
				double mI[MAXRANK][MAXRANK][MAXRANK]);
				
void learning_hierarchical(int item, int k, double mI[MAXRANK][MAXRANK][MAXRANK],
							vector<vector<int> > &hierarchical);
							
void second_collect(RankDataset *real_dataset, vector<vector<int> >& hierarchical, 
				 double epsilon, char* method, RankDataset *ldp_dataset);
#endif