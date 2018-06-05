#include<iostream>
#include<vector>
#include<math.h>
#include<string.h>
#include<assert.h>
#include<map>
#include<time.h>
#include<stdio.h>
#include "safari.h"
#include "ldp.h"
#include "util.h"
using namespace std;

const double eps=1e-6;

int id[MAXRANK][MAXRANK][MAXRANK];
double px[MAXRANK+3][MAXRANK+3] , py[MAXRANK+3][MAXRANK+3][3];


void first_collect(RankDataset *real_dataset, double epsilon, char* method,
				double mI[MAXRANK][MAXRANK][MAXRANK]){
	printf("\nphase_one epsilon %f method %s\n", epsilon, method);

    
    int item = real_dataset->item;
	int value_number = 0;
	for(int i=0;i<item;i++)
        for(int j=0;j<item;j++) if(i-j)
    		for(int k=j+1;k<item;k++) if(i-k){
				id[i][j][k] = value_number;
				value_number += 1;
			}
    
	int *item_number = new int[value_number+3];
	for(int i=0;i<value_number;i++)
		item_number[i] = 2 * item;
	
	SamplingLdp sldp;
    
	sldp.init(item_number, value_number, epsilon, method);
    
	int *s = new int[value_number+3];
	for(int d=0;d<(int)real_dataset->dataset.size();d++){
		for(int i=0;i<item;i++)
			for(int j=0;j<item;j++) if(i-j)
				for(int k=j+1;k<item;k++) if(i-k){ 
					s[id[i][j][k]] = real_dataset->dataset[d][i] * 2 + 
									(real_dataset->dataset[d][j] < real_dataset->dataset[d][k]) ;
				}
		sldp.get(s);	
	}
   
	
	sldp.compute();
	
	int lenx, leny;
	lenx = item;
	leny = 2;
	memset(px,0,sizeof(px));
	memset(py,0,sizeof(py));
	
	for(int i=0;i<item;i++)
        for(int j=0;j<item;j++) if(i-j)
    		for(int k=j+1;k<item;k++) if(i-k){
				int q = id[i][j][k];
				normalize(sldp.ldp_number[q], 2*item);
				
				for(int x=0;x<item;x++)
					for(int c=0;c<2;c++){
						double t = sldp.ldp_number[ id[i][j][k] ][ 2 *x + c ];
						px[i][x] += t;
						py[j][k][c] += t;
					}
			}
	
	for(int i=0;i<item;i++)
		for(int x=0;x<item;x++){
			px[i][x] /= (value_number/item);
		}
	
	for(int j=0;j<item;j++)
		for(int k=j+1;k<item;k++)
			for(int c=0;c<2;c++){
				py[j][k][c] /= item-2;
			}
	
	for(int i=0;i<item;i++)
        for(int j=0;j<item;j++) if(i-j)
    		for(int k=j+1;k<item;k++) if(i-k){
				mI[i][j][k] = mutual_information(px[i], py[j][k], 
								sldp.ldp_number[ id[i][j][k] ], lenx, leny);
				mI[i][k][j] = mI[i][j][k];
                //printf("i $%d j %d k %d mI %f\n",i,j,k,mI[i][j][k]);
			}
	
	return ;
}
