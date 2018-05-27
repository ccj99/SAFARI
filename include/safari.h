#ifndef _SAFARI_H
#define _SAFARI_H
#include "dataset.h"

void phase_one(RankDataset *real_dataset, double epsilon, char* method,
				double mI[MAXRANK][MAXRANK][MAXRANK]);
				
void learning_hierarchical(int item, int k, double mI[MAXRANK][MAXRANK][MAXRANK],
							vector<vector<int> > &hierarchical);
							
void phase_two(RankDataset *real_dataset, vector<vector<int> >& hierarchical, 
				 double epsilon, char* method, RankDataset *ldp_dataset);
#endif