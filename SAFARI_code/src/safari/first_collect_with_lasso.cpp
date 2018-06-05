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
#include "lasso.h"
using namespace std;

const double eps=1e-6;

void first_collect_with_lasso(RankDataset *real_dataset, double epsilon, char* method,
				double mI[MAXRANK][MAXRANK][MAXRANK]){
	printf("\nphase_one with lasso epsilon %f method %s\n", epsilon, method);

	static double px[MAXRANK][MAXRANK], pxy[MAXRANK][MAXRANK][2];
	static int id[MAXRANK][MAXRANK];
	
    int item = real_dataset->item;
	
	int value_number = item;
	int *item_number = new int[value_number+3];
	for(int i=0;i<value_number;i++)
		item_number[i] = item;
	
	SamplingLdp sldp;
    
	sldp.init(item_number, value_number, epsilon, method);
	
	int *s = new int[value_number+3];
	for(int d=0;d<(int)real_dataset->dataset.size();d++){
		for(int i=0;i<item;i++){
			s[i] = real_dataset->dataset[d][i];
		}
		sldp.get(s);	
	}
   
	sldp.compute();
	
	for(int i=0;i<value_number;i++){
		normalize(sldp.ldp_number[i], sldp.item_number[i]);
		for(int j=0;j<sldp.item_number[i];j++){
			px[i][j] = sldp.ldp_number[i][j];
		}
	}
	
	//first order to second order
	//sum_{y=0~item}( P(A_x,B_y) ) = P(A_x)
	//sum_{x=0~item}( P(A_x,B_y) ) = P(B_y)
	int cnt_2 = 0;
	for(int x=0;x<item;x++)
		for(int y=0;y<item;y++) if(x-y){
			id[x][y] = cnt_2;
			cnt_2++;
		}
	
	memset(pxy, 0, sizeof pxy);
	for(int a=0;a<item;a++)
		for(int b=a+1;b<item;b++){
			Lasso lasso;
			lasso.init(cnt_2, 2*item);
			
			int x,y;
			for(x=0;x<item;x++){
				lasso.par_y[x] = px[a][x];
				for(y=0;y<item;y++) if(x-y){
					lasso.par_x[x][ id[x][y] ] = 1;
				}
			}
			
			for(y=0;y<item;y++){
				lasso.par_y[y+item] = px[b][y];
				for(x=0;x<item;x++) if(x-y) {
					lasso.par_x[y+item][ id[x][y] ] = 1;
				}
			}
			
			lasso.Least_Angle_Regression();
			for(x=0;x<item;x++)
				for(y=0;y<item;y++) if(x-y){
					pxy[a][b][x<y] += lasso.par_b[ id[x][y] ];
				}
		}
	
	//first order, second order to thirde order
	//sum_{y=0~2}( P(A_x,BC_y) ) = P(A_x)
	//sum_{x=0~item}( P(A_x,BC_y) ) = P(BC_y)
	double *real_number = new double[item*2+3];

	for(int a=0;a<item;a++)
		for(int b=0;b<item;b++) if(a-b)
			for(int c=b+1;c<item;c++) if(a-c){
				Lasso lasso;
				lasso.init(item*2, item+2);
				
				int x,y;
				for(x=0;x<item;x++){
					lasso.par_y[x] = px[a][x];
					for(y=0;y<2;y++)
						lasso.par_x[x][ 2*x+y ] = 1;
				}
				
				for(y=0;y<2;y++){
					lasso.par_y[item+y] = pxy[b][c][y]; 
					for(x=0;x<item;x++)
						lasso.par_x[item+y][2*x+y] = 1;
				}
				
				lasso.Least_Angle_Regression();
				
				memset(real_number, 0, (sizeof (double))*(item*2+3));
				
				double *cc = new double[item*2+3];
				memset(cc, 0, (sizeof (double))*(item*2+3));
				
				for(int d=0;d<(int)real_dataset->dataset.size();d++){
					real_number[ real_dataset->dataset[d][a] * 2 + 
								(real_dataset->dataset[d][b] < real_dataset->dataset[d][c]) ]
								+= 1.0/real_dataset->user_number;
				}
				
				mI[a][b][c] =  mutual_information(px[a], pxy[b][c], lasso.par_b, item, 2);
				mI[a][c][b] = mI[a][b][c];
			}
			
	return ;
}
