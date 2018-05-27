#include <stdio.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include <getopt.h>
#include "ldp.h"
#include "util.h"
#include "dataset.h"

using namespace std;

void getIa1a2(int a1,int a2,vector<int> A, double mI[MAXRANK][MAXRANK][MAXRANK],
                vector<int> &p){ ///compute kth smallest item in {I(x;a1,a2);x != a1,a2};
    vector<pair<double,int> >q;
    for(int i=0;i<(int)A.size();i++){
        if( A[i] != a1 && A[i] != a2 )
            q.push_back(make_pair(mI[A[i]][a1][a2],A[i]));
    }
    sort(q.begin(),q.end());
    p.clear();
    for(int i=0;i<(int)q.size();i++)
        p.push_back(q[i].second);
}

void dfs(vector<int>res_set,int k, double mI[MAXRANK][MAXRANK][MAXRANK],
            vector<vector<int> > &hierarchical){
    if( (int)res_set.size() <= k ){
        hierarchical.push_back(res_set);
        return ;
    }
    vector<int>left_set, right_set;
    if( res_set.size() == 2 ){
        left_set.push_back(res_set[0]);
        right_set.push_back(res_set[1]);
    }else{
        double min_mi = 1e8;
        for(int i=0;i<(int)res_set.size();i++)
            for(int j=0;j<(int)res_set.size();j++) if(i-j){
                vector<int>p;
                getIa1a2(res_set[i], res_set[j] , res_set, mI,p);
                vector<int>a_set, b_set;
                b_set.push_back(res_set[i]);
                b_set.push_back(res_set[j]);
                for(int i=p.size()-1;i>=0;i--)
                    b_set.push_back(p[i]);
                double f;
                f = 0;
                for(int i=0;i<(int)p.size() && i<k;i++){
                    b_set.pop_back();
                    a_set.push_back(p[i]);
                    int x = p[i];

                    for(int i=0;i<(int)b_set.size();i++)
                        for(int j=0;j<(int)a_set.size()-1;j++)
                            f += mI[b_set[i]][a_set[j]][x] + mI[b_set[i]][x][a_set[j]];

                    for(int i=0;i<(int)a_set.size()-1;i++)
                        for(int j=0;j<(int)b_set.size();j++)
                            f -= mI[a_set[i]][b_set[j]][x] + mI[a_set[i]][x][b_set[j]];

                    for(int j=0;j<(int)b_set.size();j++)
                        for(int k=0;k<(int)b_set.size();k++) if(k-j)
                            f += mI[x][b_set[j]][b_set[k]];

                    for(int j=0;j<(int)a_set.size()-1;j++)
                        for(int k=0;k<(int)a_set.size()-1;k++) if(k-j)
                            f -= mI[x][a_set[j]][a_set[k]];

                    if( f < min_mi ){
                        min_mi = f;
                        left_set = a_set;
                        right_set = b_set;
                    }
                }
            }
    }
    if( (int)left_set.size() != k ){
        swap(left_set, right_set);
    }
    dfs(left_set, k, mI, hierarchical);
    dfs(right_set, k, mI, hierarchical);
}

void learning_hierarchical(int item,int k, double mI[MAXRANK][MAXRANK][MAXRANK],
							vector<vector<int> > &hierarchical){
    printf("\nlearning_hierarchical item %d k %d\n", item, k);
    vector<int>res_set;
    for(int i=0;i<item;i++)
        res_set.push_back(i);
    
    dfs(res_set, k, mI, hierarchical);
    
	printf("hierarchical:");
    for(int i=0;i<(int)hierarchical.size();i++){
        printf("<");
        for(int j=0;j<(int)hierarchical[i].size();j++)
            printf("%d,", hierarchical[i][j]);
        printf("> ");
    }
	printf("\n");
}