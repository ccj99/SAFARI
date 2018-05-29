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

static struct option long_options[] =
{
    {"user_number", required_argument, NULL, 0},
    {"item_number", required_argument, NULL, 0},
    {"epsilon", required_argument, NULL, 0},
    {"input_file", required_argument, NULL, 0},
    {"output_file", required_argument, NULL, 0},
    {"method", required_argument, NULL, 0}
};

int user_number=-1, item_number=-1, option_index;
char input_file[100]= "", output_file[100]= "";
double epsilon=-1;
Ldp *ldp = NULL;
KangTuo kangtuo;
RankDataset real_dataset,synthetic_dataset;
int rank[MAXRANK];

int main(int argc, char * argv[]){
    while( getopt_long(argc, argv, "", long_options, &option_index)!= -1){
        printf("%s %s\n",long_options[option_index].name,optarg);
        switch (option_index){
        case 0:
            user_number = strtol(optarg, NULL, 10);
			break;
        case 1:
            item_number = strtol(optarg, NULL, 10);
			break;
		case 2:
            sscanf(optarg,"%lf",&epsilon);
			break;
        case 3:
            strcpy(input_file, optarg);
			break;
        case 4:
            strcpy(output_file, optarg);
			break;
		case 5:
			if( strcmp(optarg, "sh") == 0 )
				ldp = new Sh;
			else if( strcmp(optarg, "rappor") == 0 )
				ldp = new Rappor;
			else if( strcmp(optarg, "olh") == 0 )
				ldp = new Olh;
			else if( strcmp(optarg, "grr") == 0 )
				ldp = new Grr;
			else if( strcmp(optarg, "noldp") == 0 )
				ldp = new Noldp;
			break;
		}
    }

    if( user_number == -1 || item_number == -1 || epsilon == -1 || input_file[0] == 0 || output_file[0] == 0 || ldp == NULL){
        fprintf(stderr,"parameter error.");
        return -1;
    }
	
	kangtuo.init(item_number);
	ldp->init(epsilon, kangtuo.base[item_number]);
	real_dataset.init(user_number, item_number);
	synthetic_dataset.init(user_number, item_number);
	random_init();
	
	real_dataset.read_file(input_file);
	
	for(int i=0;i<(int)real_dataset.dataset.size();i++){
		for(int j=0;j<item_number;j++){
			rank[j] = real_dataset.dataset[i][j];
		}
		int r = kangtuo.Hash(rank);
		ldp->get(r);
	}
	
	ldp->compute();
	for(int h=0;h<kangtuo.base[item_number];h++){
		kangtuo.get_hash(h, rank);
		synthetic_dataset.put_rank(rank, ldp->ldp_number[h]);
	}
	
	pair<double, double> f1 = first_order(&real_dataset, &synthetic_dataset);
	pair<double, double> f2 = second_order(&real_dataset, &synthetic_dataset);
	
	printf("first order tvd:%f re:%f\n",f1.first,f1.second);
	printf("second order tvd:%f re:%f\n",f2.first,f2.second);
	
	FILE *fp = fopen(output_file, "a");
	fprintf(fp,"%f %f %f %f\n",f1.first,f1.second,f2.first,f2.second);
	fclose(fp);
	
    return 0;
}
